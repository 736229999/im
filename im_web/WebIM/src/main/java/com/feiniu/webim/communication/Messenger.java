package com.feiniu.webim.communication;

import com.fasterxml.jackson.databind.JsonNode;
import com.feiniu.webim.bean.request.BaseRequestBean;
import com.feiniu.webim.bean.request.MessengeAware;
import com.feiniu.webim.constants.Constants;
import com.feiniu.webim.core.ServletContextAware;
import com.feiniu.webim.exception.SocketResetException;
import com.feiniu.webim.servlet.WebIMServlet;
import com.feiniu.webim.statistics.Statistics;
import com.feiniu.webim.util.general.IPUtil;
import com.feiniu.webim.util.json.JSONUtil;
import com.feiniu.webim.util.web.WebUtil;
import org.apache.log4j.Logger;

import javax.servlet.ServletContext;
import java.io.*;
import java.net.Socket;
import java.net.SocketException;
import java.util.Arrays;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.concurrent.TimeUnit;

/**
 * 信使类，主要负责为客户和商家传递消息
 *
 * @author wangchen
 */
public class Messenger implements ServletContextAware {
    private volatile List<String> serverHostList;
    private int serverPort;
    private long localIP;
    private int localPort;

    private Socket socket;
    private volatile OutputStream out;
    private volatile InputStream in;
    private volatile long lastConnectedMs;

    private static final Logger LOGGER = Logger.getLogger(Messenger.class);
    private static final String SOCKET_CLOSED_MESSAGE = "io发生错误[远程服务器已经关闭]";
    private static final String SYSTEM_IO_ERROR_MESSAGE = "系统io发生错误，程序即将退出";
    private static final int RECONNECT_TO_IM_INTERVAL_S = 10;

    //上一个执行业务修复的线程
    private volatile Thread lastRepairThread;

    public Messenger(String serverHosts, int serverPort) {
        this.serverPort = serverPort;
        serverHostList = new CopyOnWriteArrayList<>(Arrays.asList(serverHosts.split(",")));

        //简单的负载均衡规则，尽量让每台webim服务器映射到不同的im服务器
        String localIP = WebUtil.getLocalIP();
        char lastChar = localIP.substring(localIP.length() - 1).charAt(0);
        int random = lastChar - '0';

        lastRetryIndex = random % serverHostList.size();
    }

    private ServletContext servletContext;
    private volatile int lastRetryIndex = 0;

    @Override
    public void setServletContext(ServletContext context) {
        this.servletContext = context;
    }

    synchronized void initNetWorkEnv() {
        LOGGER.warn("尝试连接到业务服务器：[" + serverHostList.get(lastRetryIndex) + ":" + serverPort + "]");

        //这里如果不为空，是属于正常通信情况中的断连，
        //如果是初始化的话，肯定不会进来
        if (this.socket != null) {
            try {
                Socket oldSocket = this.socket;
                oldSocket.close();
            } catch (Exception e) {
                LOGGER.error("忽略掉：关闭socket失败！", e);
            }
        }

        while (!CommunicationManager.instance.isStopped()) {
            try {
                String serverHost = serverHostList.get(lastRetryIndex);
                Socket socket = new Socket(serverHost, serverPort);
                socket.setTcpNoDelay(true);
                socket.setKeepAlive(true);
                this.socket = socket;
                this.localIP = IPUtil.ipToLong(socket.getLocalAddress().getHostAddress());
                this.localPort = socket.getLocalPort();
                this.in = new BufferedInputStream(socket.getInputStream());
                this.out = socket.getOutputStream();

                final WebIMServlet servlet = ((WebIMServlet) servletContext.getAttribute("webimServlet"));
                if (servlet != null) {
                    //停掉之前的业务修复线程
                    if (lastRepairThread != null) {
                        lastRepairThread.interrupt();
                    }

                    //开启新的业务修复线程
                    lastRepairThread = new Thread(new Runnable() {
                        @Override
                        public void run() {
                            CommunicationManager.instance.initMessengerQueue();
                            servlet.initSession();
                        }
                    });

                    lastRepairThread.setDaemon(true);
                    lastRepairThread.start();
                    LOGGER.info("业务修复线程开启，尝试重新登录所有用户，线程号：" + lastRepairThread.getName());
                }

                lastConnectedMs = System.currentTimeMillis();

                LOGGER.warn("服务器连接成功，远程主机信息：[" + serverHost + ":" + serverPort + "]");
                break;
            } catch (Exception e) {
                LOGGER.warn("服务器尚未启动，" + RECONNECT_TO_IM_INTERVAL_S + "秒后重新连接");
                try {
                    TimeUnit.SECONDS.sleep(RECONNECT_TO_IM_INTERVAL_S);
                } catch (InterruptedException ite) {
                    LOGGER.error("线程被打断，初始化网络环境出错，系统即将退出");
                    throw new Error("初始化网络环境出错", ite);
                }

                lastRetryIndex = (lastRetryIndex + 1) % serverHostList.size();
                LOGGER.warn("开始进行服务器重连, 此次重连服务器地址：" + serverHostList.get(lastRetryIndex));
            }
        }
    }

    public final void send(String message) {
        String remoteServerInfo = "地址：" + serverHostList.get(lastRetryIndex) + "; 端口" + serverPort;

        try {
            byte[] bytes = message.getBytes(Constants.PC_MESSAGE_ENCODING);
            byte[] contentBytes = new byte[bytes.length + 4];

            System.arraycopy(IPUtil.intToByteArray(bytes.length + 4), 0, contentBytes, 0, 4);
            System.arraycopy(bytes, 0, contentBytes, 4, bytes.length);

            out.write(contentBytes);
            LOGGER.info("发送数据给服务器[" + remoteServerInfo + "]完毕：" + message);
        } catch (IOException e) {
            LOGGER.error("发送数据给服务器[" + remoteServerInfo + "]失败：" + message, e);

            if (e instanceof SocketException) {
                LOGGER.error(SOCKET_CLOSED_MESSAGE);
                fixConnectionIssue();
            } else {
                throw new Error(SYSTEM_IO_ERROR_MESSAGE);
            }
        }
    }

    public final void send(BaseRequestBean baseRequestBean) {
        Statistics.putStaticsSource(BaseRequestBean.class, baseRequestBean, new Object[0]);

        if (baseRequestBean instanceof MessengeAware) {
            MessengeAware messengeAware = (MessengeAware) baseRequestBean;

            messengeAware.setIP(localIP);
            messengeAware.setPort(localPort);
        }
        //多设备id
        baseRequestBean.setDeviceid(localIP);

        send(JSONUtil.toJsonString(baseRequestBean));
    }

    /**
     * 接收从指定的服务端发回的数据.
     *
     * @return 返回从指定的服务端发回的数据.
     */
    public final String receive() {
        byte[] buffer = new byte[4];
        int readed;

        try {
            readed = readFull(buffer);
        } catch (SocketResetException e) {
            return null;
        }

        if (readed == 4) {
            int messageLength = IPUtil.byteArrayToInt(buffer) - 4;

            if (messageLength < 0) {
                LOGGER.warn("服务器发过来数据存在问题，字节数为：" + messageLength);
                return null;
            } else {
                buffer = new byte[messageLength];
            }

            try {
                readed = readFull(buffer);
            } catch (SocketResetException e) {
                return null;
            }

            if (readed == buffer.length) {
                try {
                    String info = new String(buffer, 0, buffer.length, Constants.PC_MESSAGE_ENCODING);
                    JsonNode node = JSONUtil.toNode(info);

                    if (node != null && node.hasNonNull("encoding")) {
                        String encoding = node.get("encoding").asText();
                        info = new String(buffer, 0, buffer.length, encoding);
                    } else {
                        info = new String(buffer, 0, buffer.length, Constants.PC_MESSAGE_ENCODING);
                    }

                    return info;
                } catch (UnsupportedEncodingException e) {
                    throw new RuntimeException(e);
                }
            }
        }

        LOGGER.warn("读取响应消息失败, 消息为：" + new String(buffer));
        return null;
    }

    /**
     * 让读取线程来修理连接断开问题*
     */
    private int readFull(byte[] buffer) {
        int readed = 0;

        try {
            while (readed != buffer.length) {
                int currentRead = in.read(buffer, readed, buffer.length - readed);

                if (currentRead > 0) {
                    readed += currentRead;
                }
            }
        } catch (IOException e) {
            if (e instanceof SocketException) {
                LOGGER.error(SOCKET_CLOSED_MESSAGE, e);
                fixConnectionIssue();
                throw new SocketResetException("");
            }

            throw new Error(SYSTEM_IO_ERROR_MESSAGE);
        }


        return readed;
    }

    private synchronized void fixConnectionIssue() {
        if (lastConnectedMs != 0) {
            long inteval = System.currentTimeMillis() - lastConnectedMs;
            long minimalInteval = RECONNECT_TO_IM_INTERVAL_S * 1000l;

            if (inteval < minimalInteval) {
                try {
                    TimeUnit.MILLISECONDS.sleep(minimalInteval - inteval);
                } catch (InterruptedException e) {
                    LOGGER.info("线程被打断", e);
                    throw new Error("线程被打断，准备退出系统", e);
                }
            }
        }

        initNetWorkEnv();
    }

    public final void close() {
        try {
            socket.close();
        } catch (Exception ex) {
            throw new RuntimeException(ex);
        }
    }

}