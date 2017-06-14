package util.webim;

import com.fasterxml.jackson.databind.JsonNode;
import com.feiniu.webim.bean.request.BaseRequestBean;
import com.feiniu.webim.bean.request.MessengeAware;
import com.feiniu.webim.constants.Constants;
import com.feiniu.webim.core.SessionManager;
import com.feiniu.webim.exception.SocketResetException;
import com.feiniu.webim.util.general.IPUtil;
import com.feiniu.webim.util.json.JSONUtil;
import org.apache.log4j.Logger;

import javax.servlet.ServletContext;
import java.io.*;
import java.net.Socket;
import java.net.SocketException;
import java.util.concurrent.TimeUnit;

/**
 * 信使类，主要负责为客户和商家传递消息
 *
 * @author wangchen
 */
public class Messenger {
    private String serverHost;
    private int serverPort;
    private long localIP;
    private int localPort;

    private Socket socket;
    private volatile OutputStream out;
    private volatile InputStream in;

    private static final Logger LOGGER = Logger.getLogger(Messenger.class);
    private static final String SOCKET_CLOSED_MESSAGE = "io发生错误[远程服务器已经关闭]";
    private static final String SYSTEM_IO_ERROR_MESSAGE = "系统io发生错误，程序即将退出";

    public Messenger(String serverHost, int serverPort) {
        this.serverHost = serverHost;
        this.serverPort = serverPort;
    }

    private ServletContext servletContext;

    synchronized void initNetWorkEnv() {
        System.out.println("尝试连接到业务服务器：[" + serverHost + ":" + serverPort + "]");

        //这里如果不为空，是属于正常通信情况中的断连，
        //如果是初始化的话，肯定不会进来
        if (this.socket != null) {
            try {
                Socket oldSocket = this.socket;
                oldSocket.close();
            } catch (Exception e) {
                System.out.println("忽略掉：关闭socket失败！");
                System.out.println(e);
            }
        }

        while (true) {
            try {
                Socket socket = new Socket(serverHost, serverPort);
                socket.setTcpNoDelay(true);
                socket.setKeepAlive(true);
                this.socket = socket;
                this.localIP = IPUtil.ipToLong(socket.getLocalAddress().getHostAddress());
                this.localPort = socket.getLocalPort();
                this.in = new BufferedInputStream(socket.getInputStream());
                this.out = socket.getOutputStream();

                System.out.println("服务器连接成功，远程主机信息：[" + serverHost + ":" + serverPort + "]");
                break;
            } catch (Exception e) {
                SessionManager.getInstance().removeAllSessions();
                System.out.println("服务器尚未启动，10秒后重新连接：[" + serverHost + ":" + serverPort + "]");
                try {
                    TimeUnit.SECONDS.sleep(10);
                } catch (InterruptedException ite) {
                    System.out.println("线程被打断，初始化网络环境出错，系统即将退出");
                    throw new Error("初始化网络环境出错");
                }
                System.out.println("开始进行服务器重连.....");
            }
        }
    }

    public final void send(String message) {
        String remoteServerInfo = "地址：" + serverHost + "; 端口" + serverPort;

        try {
            byte[] bytes = message.getBytes(Constants.PC_MESSAGE_ENCODING);
            byte[] contentBytes = new byte[bytes.length + 4];

            System.arraycopy(IPUtil.intToByteArray(bytes.length + 4), 0, contentBytes, 0, 4);
            System.arraycopy(bytes, 0, contentBytes, 4, bytes.length);

            out.write(contentBytes);
            System.out.println("发送数据给服务器[" + remoteServerInfo + "]完毕：" + message);
        } catch (IOException e) {
            System.out.println("发送数据给服务器[" + remoteServerInfo + "]失败：" + message);
            System.out.println(e);

            if (e instanceof SocketException) {
                System.out.println(SOCKET_CLOSED_MESSAGE);
                fixConnectionIssue();
            } else {
                throw new Error(SYSTEM_IO_ERROR_MESSAGE);
            }
        }
    }

    public final void send(BaseRequestBean baseRequestBean) {
        if (baseRequestBean instanceof MessengeAware) {
            MessengeAware messengeAware = (MessengeAware) baseRequestBean;

            messengeAware.setIP(localIP);
            messengeAware.setPort(localPort);
        }

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
                System.out.println("服务器发过来数据存在问题，字节数为：" + messageLength);
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

        System.out.println("读取响应消息失败, 消息为：" + new String(buffer));
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
                System.out.println(SOCKET_CLOSED_MESSAGE + e);
                fixConnectionIssue();
                throw new SocketResetException("");
            }

            throw new Error(SYSTEM_IO_ERROR_MESSAGE);
        }


        return readed;
    }

    private synchronized void fixConnectionIssue() {
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