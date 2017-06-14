package com.feiniu.webim.communication;

import com.feiniu.webim.bean.request.BaseRequestBean;
import com.feiniu.webim.bean.request.HeartRequestBean;
import com.feiniu.webim.bean.response.BaseResponseBean;
import com.feiniu.webim.bean.response.ResponseMessageType;
import com.feiniu.webim.config.ConfigManager;
import com.feiniu.webim.config.SystemConfig;
import com.feiniu.webim.constants.Constants;
import com.feiniu.webim.core.Dispatcher;
import com.feiniu.webim.core.ServletContextAware;
import com.feiniu.webim.core.Session;
import com.feiniu.webim.core.SessionManager;
import com.feiniu.webim.util.json.JSONUtil;
import com.feiniu.webim.util.json.JsonNodeUtil;
import com.feiniu.webim.util.web.WebUtil;
import org.apache.commons.lang.StringUtils;
import org.apache.commons.lang.math.RandomUtils;
import org.apache.log4j.Logger;

import javax.servlet.ServletContext;
import java.util.*;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.TimeUnit;

public class CommunicationManager implements ServletContextAware {
    private Timer heartBeatTimer = new Timer();

    private static final Logger LOGGER = Logger.getLogger(CommunicationManager.class);
    private final String serverHost = ConfigManager.getProperty(SystemConfig.SERVER_HOST.toString());
    private final int serverPort = ConfigManager.getLongProperty(SystemConfig.SERVER_PORT.toString()).intValue();
    private final int MESSENGER_SENDING_QUEUE_CAPACITY =
            ConfigManager.getIntProperty(SystemConfig.MESSENGER_SENDING_QUEUE_CAPACITY.toString());
    private final int MESSENGER_RECEIVING_QUEUE_CAPACITY =
            ConfigManager.getIntProperty(SystemConfig.MESSENGER_RECEIVING_QUEUE_CAPACITY.toString());
    private final int MESSENGER_SENDING_QUEUE_PUT_MAX_RETRY_TIME =
            ConfigManager.getIntProperty(SystemConfig.MESSENGER_SENDING_QUEUE_PUT_MAX_RETRY_TIME.toString());
    private final int MESSENGER_SENDING_QUEUE_PUT_TIME_OUT =
            ConfigManager.getIntProperty(SystemConfig.MESSENGER_SENDING_QUEUE_PUT_TIMEOUT.toString());
    private final int HEART_BEAT_INTERVAL_MILLS =
            ConfigManager.getIntProperty(SystemConfig.HEART_BEAT_INTERVAL_MILLS.toString());

    private volatile boolean isContinue = true;

    private volatile Thread sendingThread;
    private volatile Messenger messenger;
    private volatile BlockingQueue<BaseRequestBean> messengerSendingQueue;
    private volatile BlockingQueue<String> messengerReceivingQueue;

    private ServletContext servletContext;

    @Override
    public void setServletContext(ServletContext context) {
        this.servletContext = context;
    }

    /**
     * 单例
     */
    public static final CommunicationManager instance = new CommunicationManager();

    public void initCommunicationEnv() {
        initMessenger();
        initMessengerQueue();
        startDispatching();
        startListening();
        startSending();
        startHeartbeat();
    }

    private void initMessenger() {
        messenger = new Messenger(serverHost, serverPort);
        messenger.setServletContext(servletContext);
        messenger.initNetWorkEnv();
    }

    void initMessengerQueue() {
        if (messengerSendingQueue == null) {
            messengerSendingQueue = new ArrayBlockingQueue<>(MESSENGER_SENDING_QUEUE_CAPACITY);
            messengerReceivingQueue = new ArrayBlockingQueue<>(MESSENGER_RECEIVING_QUEUE_CAPACITY);
        } else {
            messengerSendingQueue.clear();
            messengerReceivingQueue.clear();
        }
    }

    private void startSending() {
        sendingThread = new Thread(new Runnable() {
            @Override
            public void run() {
                while (isContinue) {
                    try {
                        BaseRequestBean baseRequestBean = messengerSendingQueue.take();

                        if (baseRequestBean != null) {
                            messenger.send(baseRequestBean);
                        }
                    } catch (InterruptedException e) {
                        LOGGER.warn("发送线程读取队列时被打断");
                        throw new RuntimeException();
                    } catch (RuntimeException e) {
                        LOGGER.error("发送消息出现异常", e);
                    }
                }
            }
        }, "发送线程");

        sendingThread.start();
        LOGGER.info("初始化发送者线程成功");
    }

    private void startListening() {
        Thread listeningThread = new Thread(new Runnable() {
            @Override
            public void run() {
                String messageJSON = "";

                /**获取im-时间**/
                while (true) {
                    messageJSON = messenger.receive();
                    Map map = JSONUtil.toMap(messageJSON);

                    if (map.containsKey("serverTime")) {
                        int imTime = Integer.parseInt(map.get("serverTime").toString());
                        int webimTime = (int) (System.currentTimeMillis() / 1000);

                        LOGGER.info("SYNC_TIME[同步im时间]: webim-now:" + webimTime + "s, im-now:" + imTime + 's');

                        Constants.TIME_DELTA_BETWEEN_WEBIM_AND_IM = imTime - webimTime;
                        break;
                    }
                }

                while (isContinue) {
                    try {
                        messageJSON = messenger.receive();

                        if (StringUtils.isNotEmpty(messageJSON)) {
                            boolean isPutSuccess = messengerReceivingQueue.offer(messageJSON);
                            LOGGER.info("{监听线程}服务器返回数据：" + messageJSON
                                    + "，是否成功放入转发消息队列：" + (isPutSuccess ? "是" : "否"));
                        } else {
                            LOGGER.warn("{监听线程}服务器返回空数据：" + messageJSON);
                        }
                    } catch (Exception e) {
                        LOGGER.error("{监听线程}将服务器返回数据解析为Json对象失败：" + messageJSON, e);
                    }
                }
            }
        }, "接收线程");

        listeningThread.start();
        LOGGER.info("初始化接收者线程成功");
    }

    private void startDispatching() {
        Thread dispatchThread = new Thread(new Runnable() {
            @Override
            public void run() {
                while (isContinue) {
                    try {
                        String messageJSON = messengerReceivingQueue.take();
                        BaseResponseBean message = parseMessage(messageJSON);

                        if (message == null) {
                            continue;
                        }

                        ResponseMessageType responseMessageType = ResponseMessageType.getResponseMessageType(message.getCmd());
                        if (ResponseMessageType.RETRIEVE_HISOTRY_MESSAGE.equals(responseMessageType)
                                || ResponseMessageType.RETRIVE_RECENTLY_CONTACTS_FOR_BUYER.equals(responseMessageType)
								|| ResponseMessageType.RETRIVE_RECENTLY_MOBILE_CONTACTS_FOR_BUYER.equals(responseMessageType)) {
                            Dispatcher.getInstance().unicast(messageJSON, message.getSid() + "");
                        } else if (ResponseMessageType.CHAT_FORWARD.equals(responseMessageType)
                                || ResponseMessageType.RETRIEVE_USER_INFO.equals(responseMessageType)
                                || ResponseMessageType.RETRIVE_MERCHANT_SUB_ACCOUNT.equals(responseMessageType)
                                || ResponseMessageType.RETRIEVE_MERCHANT_ONLINE_INFO.equals(responseMessageType)
                                || ResponseMessageType.RETRIVE_SUB_MERCHANT_ONLINE_INFO.equals(responseMessageType)
                                || ResponseMessageType.PUT_MESSAGE_REPLY.equals(responseMessageType)) {
                            Dispatcher.getInstance().unicast(messageJSON, message.getDid() + "");
                        } else if (ResponseMessageType.LOGIN.equals(responseMessageType)) {
                            Dispatcher.getInstance().unicast(messageJSON, message.getSessionID());
                        }
                    } catch (Exception e) {
                        LOGGER.error("转发消息时发生错误", e);
                    }
                }
            }
        }, "转发线程");

        dispatchThread.start();
        LOGGER.info("初始化转发线程成功");
    }

    private void startHeartbeat() {
        heartBeatTimer.schedule(new TimerTask() {
            @Override
            public void run() {
                try {
                    Collection<Session> sessionList = SessionManager.getInstance().getIdTosessions();
                    List<Session> loginedSessionList = new ArrayList<>();

                    for (Session session : sessionList) {
                        if (!session.isLogin()) {
                            continue;
                        } else {
                            loginedSessionList.add(session);
                        }
                    }

                    if (loginedSessionList.size() > 0) {
                        Session randomSession = loginedSessionList.get(RandomUtils.nextInt(loginedSessionList.size()));
                        int seq = Integer.parseInt(WebUtil.dateFormat("HHmmssSSS"));
                        HeartRequestBean heartRequestBean = HeartRequestBean.getMessage();
                        heartRequestBean.setSeq(seq);
                        heartRequestBean.setSid(Integer.parseInt(randomSession.getId()));
                        heartRequestBean.setId(heartRequestBean.getSid());

                        sendMessage(heartRequestBean);
                    }

                    LOGGER.info("Logined Session已登录会话列表个数：" + loginedSessionList.size()
                            + ", 未登录会话列表个数：" + (sessionList.size() - loginedSessionList.size()));
                    LOGGER.debug(JSONUtil.toJsonString(loginedSessionList));
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }, 1000, HEART_BEAT_INTERVAL_MILLS);
    }

    public boolean sendMessage(BaseRequestBean baseRequestBean) {
        boolean isPutSuccess = false;
        int retryTime = 0;

        while (!isPutSuccess && retryTime < MESSENGER_SENDING_QUEUE_PUT_MAX_RETRY_TIME) {
            try {
                LOGGER.info("线程[" + Thread.currentThread().getName() + "] 第" + retryTime + "次尝试将消息放入消息队列");
                isPutSuccess = messengerSendingQueue.offer(baseRequestBean, MESSENGER_SENDING_QUEUE_PUT_TIME_OUT, TimeUnit.MILLISECONDS);
                retryTime++;
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
        }

        if (isPutSuccess) {
            LOGGER.info("线程[" + Thread.currentThread().getName() + "] 在第" + retryTime + "次成功将消息放入消息队列");
        } else {
            LOGGER.info("线程[" + Thread.currentThread().getName() + "] 将消息放入消息队列失败");
        }

        return isPutSuccess;
    }

    public void stop() {
        isContinue = false;

        heartBeatTimer.cancel();

        if (sendingThread != null) {
            sendingThread.interrupt();
        }

        messenger.close();
    }

    public boolean isStopped() {
        return !isContinue;
    }

    private BaseResponseBean parseMessage(String messageJson) {
        BaseResponseBean message = new BaseResponseBean();
        JsonNodeUtil.parseBean(messageJson, BaseResponseBean.class, message);

        return message;
    }

    public static void main(String[] args) {
        ConfigManager.load(CommunicationManager.class.getResource("/").getFile());
        CommunicationManager.instance.startListening();
    }

}
