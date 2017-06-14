package com.feiniu.webim.core;

import com.feiniu.webim.bean.domain.ChatWindowHandle;
import com.feiniu.webim.bean.response.*;
import com.feiniu.webim.bean.web.response.MerchantUnreadMsgListResponse;
import com.feiniu.webim.exception.ChatSessionNotExistException;
import com.feiniu.webim.util.json.JsonNodeUtil;
import com.feiniu.webim.util.log.Log;
import org.apache.log4j.Logger;

import java.util.Collections;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

/**
 * Author: morningking
 * Date: 2014/12/8
 * Contact: 243717042@qq.com
 * <p/>
 * 浏览器买家端 各个动作会触发向im的请求，如登录、获取个人信息、分流、发送消息、接收消息等；
 * 将非聊天消息(后面统称为命令消息)(cmd为154[7-9])在此类统一管理，聊天消息存放于UserChatMessageCenter单独管理；
 * 命令消息会按照cmd进行split-lock管理，即对于每个cmd，会有一把不同的锁来进行相应的顺序同步
 */
public class SessionMessageCenter {
    private static final Logger LOGGER = Logger.getLogger(SessionMessageCenter.class);

    private static final Integer RETRIEVE_LOGIN_INFO_INDICATOR = 0;
    private static final Integer RETRIEVE_USER_INFO_INDICATOR = -1;
    private static final Integer RETRIEVE_SUB_MERCHANT_INDICATOR = -2;
    private static final Integer RETRIEVE_RECENTLY_SELLER_INDICATOR = -3;
    private static final Integer RETRIEVE_HISTORY_MESSAGE_INDICATOR = -4;
	private static final Integer RETRIEVE_RECENTLY_MOBILE_SELLER_INDICATOR = -5;

    /**
     * token维度消息队列（两层）
     * 聊天消息管理
     */
    private final UserChatMessageCenter userChatMessageCenter;
    /**
     * subMerchantId维度的消息队列（两层）
     * 聊天外其他消息管理
     */
    private final ConcurrentHashMap<Integer, IndividualMessageManager> messageManagerMap = new ConcurrentHashMap<>(6);
    //对于每个命令消息，使用分离锁
    private final ConcurrentHashMap<Integer, Lock> commandMessageLock = new ConcurrentHashMap<>(6);

    private Session session;

    SessionMessageCenter(Session session) {
        this.session = session;
        userChatMessageCenter = new UserChatMessageCenter();
        userChatMessageCenter.setSession(session);
        initInternal();
    }

    private void initInternal() {
        initGeneralMessageManager(RETRIEVE_LOGIN_INFO_INDICATOR);
        initGeneralMessageManager(RETRIEVE_USER_INFO_INDICATOR);
        initGeneralMessageManager(RETRIEVE_SUB_MERCHANT_INDICATOR);
        initGeneralMessageManager(RETRIEVE_RECENTLY_SELLER_INDICATOR);
        initGeneralMessageManager(RETRIEVE_HISTORY_MESSAGE_INDICATOR);
        initGeneralMessageManager(RETRIEVE_RECENTLY_MOBILE_SELLER_INDICATOR);
    }

    void destroyInternal() {
        for (IndividualMessageManager individualMessageManager : messageManagerMap.values()) {
            individualMessageManager.destroy();
        }

        messageManagerMap.clear();
        commandMessageLock.clear();

        userChatMessageCenter.destroy();
    }

    private void initGeneralMessageManager(int indicator) {
        messageManagerMap.put(indicator, new IndividualGeneralMessageManager());
        commandMessageLock.put(indicator, new ReentrantLock());
    }

    public List<MerchantUnreadMsgListResponse> getUnreadMsgList(String token) {
        return userChatMessageCenter.getUnreadMsgList(token);
    }

    public boolean getHasUnreadMsg(String token, Integer merchantID) {
        return userChatMessageCenter.getHasUnreadMsg(token, merchantID);
    }

    public void removeUnreadMap(String token, Integer merchantId) {
        userChatMessageCenter.removeUnreadMap(token, merchantId);
    }
    //添加新窗口的映射
    public void addChatWindowHandle(ChatWindowHandle chatWindowHandle) {
        userChatMessageCenter.addHandleMapping(chatWindowHandle);
    }
	//通过uuid获取uuid对应的商家ID
    public Integer getUuidToMerchantID(String uuid) {
        return userChatMessageCenter.getUuidToMerchantID(uuid);
    }

    //销毁窗口的映射
    public void destroyChatWindowHandle(ChatWindowHandle chatWindowHandle) {
        userChatMessageCenter.removeHandleMapping(chatWindowHandle);
    }

    //向会话消息队列添加消息
    public void postMessage(String messageJson) {
        BaseResponseBean message = parseMessage(messageJson);

        int result = -1;
        int messageManagerKey = -1;

        if (message != null) {
            switch (ResponseMessageType.getResponseMessageType(message.getCmd())) {
                case LOGIN:
                    messageManagerKey = RETRIEVE_LOGIN_INFO_INDICATOR;
                    break;
                case RETRIEVE_USER_INFO:
                    messageManagerKey = RETRIEVE_USER_INFO_INDICATOR;
                    break;
                case RETRIVE_MERCHANT_SUB_ACCOUNT:
                    messageManagerKey = RETRIEVE_SUB_MERCHANT_INDICATOR;
                    break;
                case RETRIVE_RECENTLY_CONTACTS_FOR_BUYER:
                    messageManagerKey = RETRIEVE_RECENTLY_SELLER_INDICATOR;
                    break;
                case RETRIVE_RECENTLY_MOBILE_CONTACTS_FOR_BUYER:
                    messageManagerKey = RETRIEVE_RECENTLY_MOBILE_SELLER_INDICATOR;
                    break;
                case CHAT_FORWARD:
                    //ChatResponseBean chatResponseBean = (ChatResponseBean) message;
                    //messageManagerKey = chatResponseBean.getSender();
                    break;
                case RETRIEVE_HISOTRY_MESSAGE:
                    messageManagerKey = RETRIEVE_HISTORY_MESSAGE_INDICATOR;
                    break;
                case PUT_MESSAGE_REPLY:
                    messageManagerKey = message.getSid();
                    break;
            }

            ChatResponseBean chatResponseBean;
            if (ResponseMessageType.CHAT_FORWARD.equals(ResponseMessageType.getResponseMessageType(message.getCmd()))) {
                chatResponseBean = (ChatResponseBean) message;
                result = userChatMessageCenter.putMessage(chatResponseBean);
            } else {
                IndividualMessageManager manager = messageManagerMap.get(messageManagerKey);
                if (manager != null) {
                    result = manager.putMessage(message);
                } else {
                    LOGGER.warn("丢弃转换后的消息：" + messageJson + ": 并没有与之对应的会话");
                }
            }

        }

        switch (result) {
            case -1:
                LOGGER.warn("丢弃转换后的消息：" + messageJson);
                break;
            case 0:
                LOGGER.error("将消息放至消息队列失败：" + messageJson);
                break;
            case 1:
                Log.debug(LOGGER, "成功将消息放至消息队列：" + messageJson);
                break;
        }

    }

    public LoginResponseBean retrieveLoginMessage() {
        return (LoginResponseBean) retrieveMessage(RETRIEVE_LOGIN_INFO_INDICATOR);
    }

    public UserInfoResponseBean retrieveUserInfoMessage() {
        return (UserInfoResponseBean) retrieveMessage(RETRIEVE_USER_INFO_INDICATOR);
    }

    public MerchantSubAccountResponseBean retrieveMerchantSubAccountMessage() {
        return (MerchantSubAccountResponseBean) retrieveMessage(RETRIEVE_SUB_MERCHANT_INDICATOR);
    }

    public RecentlyContactsForBuyerResponseBean retrieveRecentlySellerListMessage() {
        return (RecentlyContactsForBuyerResponseBean) retrieveMessage(RETRIEVE_RECENTLY_SELLER_INDICATOR);
    }

    public RecentlyMobileContactsForBuyerResponseBean retrieveRecentlyMobileSellerListMessage() {
        return (RecentlyMobileContactsForBuyerResponseBean) retrieveMessage(RETRIEVE_RECENTLY_MOBILE_SELLER_INDICATOR);
    }
	
    private Lock retrieveSplitLock(int commandID) {
        return commandMessageLock.get(commandID);
    }

    private BaseResponseBean retrieveMessage(int commandID) {
        Lock monitor = retrieveSplitLock(commandID);

        monitor.lock();
        try {
            IndividualMessageManager individualMessageManager = messageManagerMap.get(commandID);
            return individualMessageManager.retrieveGeneralMessage();
        }finally {
            monitor.unlock();
        }
    }

    public List<BaseResponseBean> retrieveAllChatMessage(ChatWindowHandle handle) {
        if (handle == null) {
            throw new ChatSessionNotExistException("聊天会话不存在或已经被销毁");
        } else {
            return userChatMessageCenter.retrieveAllChatMessage(handle);
        }
    }

    @SuppressWarnings(value = "unchecked")
    public List<BaseResponseBean> retrieveAllHistoryMessage(String uuid) {
        Lock lock = retrieveSplitLock(RETRIEVE_HISTORY_MESSAGE_INDICATOR);

        if (lock != null) {
            IndividualMessageManager individualMessageManager = messageManagerMap.get(RETRIEVE_HISTORY_MESSAGE_INDICATOR);

            lock.lock();
            try {
                return individualMessageManager.retrieveAllHistoryMessage(uuid);
            } finally {
                lock.unlock();
            }
        } else {
            return Collections.EMPTY_LIST;
        }
    }

    private static BaseResponseBean parseMessage(String messageJson) {
        try {
            BaseResponseBean baseResponseBean = JsonNodeUtil.injectBean(messageJson, BaseResponseBean.class);
            ResponseMessageType responseMessageType = ResponseMessageType.getResponseMessageType(baseResponseBean.getCmd());

            switch (responseMessageType) {
                case LOGIN:
                    baseResponseBean = JsonNodeUtil.injectBean(messageJson, LoginResponseBean.class);
                    break;
                case RETRIVE_MERCHANT_SUB_ACCOUNT:
                    baseResponseBean = JsonNodeUtil.injectBean(messageJson, MerchantSubAccountResponseBean.class);
                    break;
                case RETRIEVE_USER_INFO:
                    baseResponseBean = JsonNodeUtil.injectBean(messageJson, UserInfoResponseBean.class, true);
                    break;
                case CHAT_FORWARD:
                    baseResponseBean = JsonNodeUtil.injectBean(messageJson, ChatResponseBean.class);
                    break;
                case PUT_MESSAGE_REPLY:
                    baseResponseBean = JsonNodeUtil.injectBean(messageJson, BaseResponseBean.class);
                    break;
                case RETRIEVE_HISOTRY_MESSAGE:
                    baseResponseBean = JsonNodeUtil.injectBean(messageJson, HistoryMessageListResponseBean.class);
                    break;
                case RETRIVE_RECENTLY_CONTACTS_FOR_BUYER:
                    baseResponseBean = JsonNodeUtil.injectBean(messageJson, RecentlyContactsForBuyerResponseBean.class);
                    break;
                case RETRIVE_RECENTLY_MOBILE_CONTACTS_FOR_BUYER:
                    baseResponseBean = JsonNodeUtil.injectBean(messageJson, RecentlyMobileContactsForBuyerResponseBean.class);
                    break;
                case RETRIEVE_MERCHANT_ONLINE_INFO:
                    baseResponseBean = JsonNodeUtil.injectBean(messageJson, MerchantOnlineInfoResponseBean.class);
                    break;
                case RETRIVE_SUB_MERCHANT_ONLINE_INFO:
                    baseResponseBean = JsonNodeUtil.injectBean(messageJson, MerchantOnlineInfoResponseBean.class);
                    break;
                default:
                    baseResponseBean = null;
                    break;
            }

            return baseResponseBean;
        } catch (RuntimeException e) {
            LOGGER.warn("将服务器返回的字符串转换为消息时发生错误：" + messageJson, e);
        }

        return null;
    }
}

