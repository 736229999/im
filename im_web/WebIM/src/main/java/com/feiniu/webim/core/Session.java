package com.feiniu.webim.core;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.feiniu.webim.bean.MessageType;
import com.feiniu.webim.bean.domain.ChatWindowHandle;
import com.feiniu.webim.bean.domain.MerchantSubAccountInfo;
import com.feiniu.webim.bean.domain.RecentlySeller;
import com.feiniu.webim.bean.domain.UserInfo;
import com.feiniu.webim.bean.helper.KVPair;
import com.feiniu.webim.bean.request.BaseRequestBean;
import com.feiniu.webim.bean.request.ExitRequestBean;
import com.feiniu.webim.bean.response.BaseResponseBean;
import com.feiniu.webim.bean.response.ChatResponseBean;
import com.feiniu.webim.communication.CommunicationManager;
import com.feiniu.webim.component.EndecryptedManager;
import com.feiniu.webim.component.ImApiService;
import com.feiniu.webim.config.ConfigManager;
import com.feiniu.webim.config.SystemConfig;
import com.feiniu.webim.constants.Constants;
import com.feiniu.webim.util.json.JSONUtil;
import com.feiniu.webim.util.web.WebUtil;
import org.apache.commons.collections4.MapUtils;
import org.apache.commons.lang.StringUtils;
import org.apache.commons.lang.math.NumberUtils;
import org.apache.log4j.Logger;

import java.util.*;
import java.util.concurrent.ConcurrentHashMap;

/**
 * Author: morningking
 * Date: 2015/8/21
 * Contact: 243717042@qq.com
 * <p/>
 * 会话核心类,维护一个用户与会话相关的所有信息
 */
public class Session {
    private static final long LEASE_TIME_MILLIS = ConfigManager.getLongProperty(SystemConfig.SESSION_TIMEOUT_MINS.toString()) * 1000;
    private static final int CHAT_WINDOW_NON_ACTIVE_TORELANT_MINIS =
            ConfigManager.getIntProperty(SystemConfig.CHAT_WINDOW_NON_ACTIVE_TORELANT_MINIS.toString()) * 1000;
    private static final Logger LOGGER = Logger.getLogger(Session.class);
    private static final EndecryptedManager ENDECRYPTED_MANAGER = EndecryptedManager.INSTANCE;
    public static final ImApiService IM_API_SERVICE = ImApiService.INSTANCE;

    private volatile String id;
    private volatile UserInfo userInfo;
    private volatile String userName;

    //窗口uuid 对应窗口详细信息
    private volatile ConcurrentHashMap<String, ChatWindowHandle> mapUuidToChatWindowHandle = new ConcurrentHashMap<>(1);

    //当前正在聊天的客服
    private volatile ConcurrentHashMap<String, Integer> mapCurrentSubAccountId = new ConcurrentHashMap<>(1);

    //当前Session正在联系人
    private Set<RecentlySeller> subMerchantInChating = new HashSet<>(1);
    //与用户Session关联的消息管理器
    private volatile SessionMessageCenter messageCenter;
    //如果是未收到im登录消息，此值为false；之后，一直为true
    private volatile boolean isLogin = false;
    //买家最后动作时间
    private volatile long lastUpdateTime = WebUtil.now();
    private volatile KVPair<String, String> authTokenPair = new KVPair<>();

    //连接数相关信息
    private volatile int linkCount;
    private Map<String, Integer> linkCountDetail = new HashMap<>(2);

    //在某些时间节点 提醒买家注意发言
    private Map<Integer, Boolean> isNodeNotifiedMap =
            Collections.synchronizedMap(new HashMap<Integer, Boolean>(Constants.ALERT_SPEAK_NODE_LIST.size()));

    public Session(String sid) {
        this.id = sid;
        this.messageCenter = new SessionMessageCenter(this);

        for (Integer timeNode : Constants.ALERT_SPEAK_NODE_LIST) {
            isNodeNotifiedMap.put(timeNode, false);
        }
    }

    //判断uuid标识的聊天窗口是否是来自手机
    public boolean isUuidFromMobile(String uuid) {
        ChatWindowHandle chatWindowHandle = mapUuidToChatWindowHandle.get(uuid);
        if (chatWindowHandle != null) {
            return chatWindowHandle.isFromMobile();
        }

        return false;
    }

    //判断uuid标识的聊天窗口是否来自自营客服
    public boolean isUuidFromCS(String uuid) {
        ChatWindowHandle chatWindowHandle = mapUuidToChatWindowHandle.get(uuid);
        if (chatWindowHandle != null) {
            return chatWindowHandle.isFromCS();
        }

        return false;
    }

    //是否存在自营聊天窗口
    public boolean isChatWindowOnCSExist() {
        Collection<ChatWindowHandle> chatWindowHandles = mapUuidToChatWindowHandle.values();
        boolean isExist = false;

        for (ChatWindowHandle chatWindowHandle : chatWindowHandles) {
            if (chatWindowHandle.isFromCS()) {
                isExist = true;
                break;
            }
        }

        return isExist;
    }

    public void setCurrentSubAccountIdMap(String token, Integer subMerchantID) {
        mapCurrentSubAccountId.put(token, subMerchantID);
    }

    public Integer getCurrentSubAccountIDUnderToken(String token) {
        return mapCurrentSubAccountId.get(token);
    }

    /**
     * ChatSession ID
     *
     * @return id
     */
    public String getId() {
        return id;
    }

    public void setId(String id) {
        if (StringUtils.isNotEmpty(id) && NumberUtils.isDigits(id)) {
            this.id = id;
        } else {
            throw new IllegalArgumentException("非法设置会话id发生：id=" + id);
        }
    }

    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        if (StringUtils.isNotEmpty(userName)) {
            this.userName = userName;
        } else {
            throw new IllegalArgumentException("非法设置用户姓名：用户名=" + userName);
        }
    }

    /**
     * 是否存活
     *
     * @return true/false
     */
    public boolean isExpired() {
        return lastUpdateTime + LEASE_TIME_MILLIS < WebUtil.now();
    }

    int getTimedNode() {
        int maxTimeNode = -1;
        int leasedMinute = (int) ((WebUtil.now() - lastUpdateTime) / 60000);

        for (Integer timeNode : Constants.ALERT_SPEAK_NODE_LIST) {
            if (leasedMinute >= timeNode) {
                if (timeNode > maxTimeNode) {
                    maxTimeNode = timeNode;
                }
            }
        }

        return maxTimeNode;
    }

    /**
     * 更新Session时间
     */
    public void updateTime() {
        lastUpdateTime = WebUtil.now();

        for (Integer timeNode : Constants.ALERT_SPEAK_NODE_LIST) {
            isNodeNotifiedMap.put(timeNode, false);
        }
    }

    public void remove() {
        SessionManager.getInstance().removeSession(this);

        sendSelfStateChangeMessage();
    }

    /**
     * 向其他session发出 当前session状态发生变化的消息
     */
    protected void sendSelfStateChangeMessage() {
        if (getId().replaceAll("\\d", "").length() > 0) {
            return;
        }

        Integer sid = Integer.parseInt(getId());

        ExitRequestBean exitRequestBean = ExitRequestBean.getMessage();
        exitRequestBean.setSeq(0);
        exitRequestBean.setSid(sid);
        exitRequestBean.setId(sid);
        exitRequestBean.setAuthData((byte) 0);

        sendRequest(exitRequestBean);
    }

    private void sendRequest(BaseRequestBean baseRequestBean) {
        CommunicationManager.instance.sendMessage(baseRequestBean);
    }

    @JsonIgnore
    public SessionMessageCenter getMessageManagerCenter() {
        return messageCenter;
    }

    @JsonIgnore
    public UserInfo getUserInfo() {
        return userInfo;
    }

    public void setUserInfo(UserInfo userInfo) {
        if (userInfo != null) {
            this.userInfo = userInfo;
            LOGGER.info(userInfo.getNickname() + " set link, current link, " + linkCount);
        }
    }

    //生成当前session的全局authtoken
    public synchronized void generateGlobalToken() {
        if (userInfo != null) {

            authTokenPair.setKey(ENDECRYPTED_MANAGER.encryptedWithURLEncode(WebUtil.getLocalIP() + "," + id));
            authTokenPair.setValue(ENDECRYPTED_MANAGER.encryptedWithURLEncode(userInfo.getGuid() + System.currentTimeMillis()));

        }
    }

    public KVPair<String, String> getGlobalToken() {
        return authTokenPair;
    }

    public boolean isGUIDLegal(String guid) {
        return userInfo != null && userInfo.getGuid().equals(guid);
    }

    public boolean isLogin() {
        return isLogin;
    }

    public void setLogin(boolean isLogin) {
        this.isLogin = isLogin;
    }

    public void addChatLink(ChatWindowHandle chatWindowHandle) {
        String token = chatWindowHandle.getAuthToken();

        if (StringUtils.isNotEmpty(token)) {
            synchronized (this) {
                if (linkCountDetail.containsKey(token)) {
                    linkCountDetail.put(token, linkCountDetail.get(token) + 1);
                } else {
                    linkCountDetail.put(token, 1);
                }

                ++linkCount;
            }

            mapUuidToChatWindowHandle.put(chatWindowHandle.getUuid(), chatWindowHandle);
            messageCenter.addChatWindowHandle(chatWindowHandle);

            //对于自营客服，同一个客户，如果之前还有窗口在排队，那么要通知其不在排队
            if (chatWindowHandle.isFromCS()) {
                for (Map.Entry<String, ChatWindowHandle> uuidToHandlerEntry : mapUuidToChatWindowHandle.entrySet()) {
                    ChatWindowHandle handle = uuidToHandlerEntry.getValue();
                    //如果当前被遍历到的窗口是来自自营客服，并且还在处于排队中，置位其排队信息
                    if (handle != null && handle != chatWindowHandle && handle.isFromCS() && handle.getQueueLength() > 0) {
                        handle.setQueueLength(-2);
                    }
                }
            }
        }
    }

    public boolean removeChatLink(String uuid) {
        ChatWindowHandle chatWindowHandle = mapUuidToChatWindowHandle.get(uuid);
        if (uuid == null || chatWindowHandle == null) {
            return false;
        }

        synchronized (this) {
            String token = chatWindowHandle.getAuthToken();

            if (linkCountDetail.containsKey(token)) {
                int count = linkCountDetail.get(token);

                if (count > 0) {
                    if (count == 1) {
                        linkCountDetail.remove(token);
                    } else {
                        linkCountDetail.put(token, count - 1);
                    }

                    --linkCount;
                    messageCenter.destroyChatWindowHandle(chatWindowHandle);
                }
            }
        }

        ChatWindowHandle handle = mapUuidToChatWindowHandle.remove(chatWindowHandle.getUuid());
        return handle != null;
    }

    public int getLinkCount() {
        return linkCount;
    }

    public int getLastQueueLength(String uuid) {
        ChatWindowHandle chatWindowHandle = mapUuidToChatWindowHandle.get(uuid);
        if (chatWindowHandle == null) {
            return Integer.MAX_VALUE;
        }

        return chatWindowHandle.getQueueLength();
    }

    public void setLastQueueLength(String uuid, int lastQueueLength) {
        ChatWindowHandle chatWindowHandle = mapUuidToChatWindowHandle.get(uuid);
        if (chatWindowHandle != null) {
            chatWindowHandle.setQueueLength(lastQueueLength);
        }
    }

    //获取可以服务的客服信息
    public MerchantSubAccountInfo getCurrentKefu(String uuid) {
        ChatWindowHandle chatWindowHandle = mapUuidToChatWindowHandle.get(uuid);
        if (chatWindowHandle != null && chatWindowHandle.getQueueLength() == 0) {
            return chatWindowHandle.getMerchantSubAccountInfo();
        }

        return null;
    }

    public boolean updateChatWindowActiveTime(String uuid) {
        ChatWindowHandle chatWindowHandle = mapUuidToChatWindowHandle.get(uuid);
        if (chatWindowHandle != null) {
            chatWindowHandle.setLastUpdateTime(WebUtil.now());
            return true;
        } else {
            return false;
        }
    }

    //判断当前连接的uuid是否处于活动状态，如果不是，将其踢掉
    public void doCheckChatWindowsExpiration() {
        //如果未登录，不用检查
        if (!isLogin() || !NumberUtils.isNumber(id)) {
            return;
        }

        //扫描出非活动状态的窗口
        List<String> uuidWillBeRemoved = new ArrayList<>(1);
        for (Map.Entry<String, ChatWindowHandle> uuidToHandlerEntry : mapUuidToChatWindowHandle.entrySet()) {
            ChatWindowHandle handle = uuidToHandlerEntry.getValue();
            if (handle != null) {
                if (WebUtil.now() - handle.getLastUpdateTime() > CHAT_WINDOW_NON_ACTIVE_TORELANT_MINIS) {
                    uuidWillBeRemoved.add(uuidToHandlerEntry.getKey());
                }
            }
        }

        //向im服务器发送消息，让其释放掉这些链接
        int userID = Integer.parseInt(id);
        for (String uuid : uuidWillBeRemoved) {
            ChatWindowHandle handle = mapUuidToChatWindowHandle.get(uuid);

            if (handle != null) {
                boolean removeSucc = removeChatLink(uuid);

                if (handle.isFromCS() && handle.getQueueLength() >= 0) {
                    if (removeSucc) {
                        sendWindowCloseMessageFromCSBuyer(handle);
                        LOGGER.info("开始清除自营已排队或者已被接待的买家僵尸窗口，并向服务器发送断开消息, useID: " + userID + ", uuid: " + uuid);
                    }
                } else {
                    LOGGER.info("开始清除僵尸窗口：来自买家[userID: " + id + ", uuid: " + uuid + "]");
                }
            }
        }

        destroyIfNecessary();
    }

    //根据im推送的消息，更新排队信息
    public void updateQueueInfoFromIm(int groupID, int queueLength, MerchantSubAccountInfo merchantSubAccountInfo) {
        for (Map.Entry<String, ChatWindowHandle> entry : mapUuidToChatWindowHandle.entrySet()) {
            ChatWindowHandle chatWindowHandle = entry.getValue();
            if (chatWindowHandle != null && chatWindowHandle.isFromCS()
                    && chatWindowHandle.getGroupID() == groupID && chatWindowHandle.getQueueLength() > 0) {
                chatWindowHandle.setQueueLength(queueLength);

                if (queueLength == 0) {
                    chatWindowHandle.setMerchantSubAccountInfo(merchantSubAccountInfo);
                }
            }
        }
    }

    public void destroyIfNecessary() {
        synchronized (this) {
            if (linkCount == 0 && messageCenter != null) {
                remove();

                messageCenter.destroyInternal();
                linkCountDetail.clear();

                messageCenter = null;
                linkCountDetail = null;
            }
        }
    }

    void destroy() {
        if (!isLogin() || !NumberUtils.isNumber(id)) {
            remove();
            return;
        }

        for (ChatWindowHandle chatWindowHandle : new ArrayList<>(mapUuidToChatWindowHandle.values())) {
            if (chatWindowHandle == null) {
                continue;
            }

            boolean removeSucc = removeChatLink(chatWindowHandle.getUuid());
            if (chatWindowHandle.isFromCS() && chatWindowHandle.getQueueLength() >= 0 && removeSucc) {
                sendWindowCloseMessageFromCSBuyer(chatWindowHandle);
            }
        }

        destroyIfNecessary();
    }

    public boolean isTokenLegal(String token) {
        if (StringUtils.isEmpty(token)) {
            return false;
        } else {
            synchronized (this) {
                return MapUtils.isNotEmpty(linkCountDetail) && linkCountDetail.containsKey(token);
            }
        }
    }

    public synchronized void addSubMerchantListInChatting(RecentlySeller subMerchant) {
        subMerchantInChating.remove(subMerchant);
        subMerchantInChating.add(subMerchant);
    }

    public synchronized RecentlySeller[] getAllSubMerchantListInChatting() {
        return subMerchantInChating.toArray(new RecentlySeller[0]);
    }

    public Map<String, ChatWindowHandle> getChatWindowInfo() {
        return mapUuidToChatWindowHandle;
    }

    public ChatWindowHandle getCurrentChatWindow(String uuid) {
        return mapUuidToChatWindowHandle.get(uuid);
    }

    public Map<String, Integer> getLinkCountDetail() {
        return linkCountDetail;
    }

    //如果有需要，向买家推送聊天时间警示消息
    void alertSpeakTime(int timeNode) {
        Boolean isNotified = isNodeNotifiedMap.get(timeNode);

        if (isNotified != null && !isNotified) {
            notifySpeakTime(timeNode);
            isNodeNotifiedMap.put(timeNode, true);
        }
    }

    private void notifySpeakTime(int timeNode) {
        ChatResponseBean chatResponseBean = ChatResponseBean.getFakeResponseBean();
        chatResponseBean.setMsg(timeNode + "");
        chatResponseBean.setType(MessageType.ALERT_CUSTOMER_SPEAK_TIME.getType());

        postMessage(chatResponseBean);
    }

    public void postMessage(BaseResponseBean baseResponseBean) {
        messageCenter.postMessage(JSONUtil.toJsonString(baseResponseBean));
    }

    //买家访问自营客服时，关闭窗口消息推送给im和imapi
    private void sendWindowCloseMessageFromCSBuyer(ChatWindowHandle chatWindowHandle) {
        MerchantSubAccountInfo subAccountInfo = chatWindowHandle.getMerchantSubAccountInfo();

        Integer sid = Integer.parseInt(getId());

        ExitRequestBean exitRequestBean = ExitRequestBean.getMessage();
        exitRequestBean.setSeq(0);
        exitRequestBean.setSid(sid);
        exitRequestBean.setId(sid);
        exitRequestBean.setAuthData((byte) 1);

        sendRequest(exitRequestBean);
        IM_API_SERVICE.notifyWindowCloseFromBuyer(sid, chatWindowHandle.getGroupID(), subAccountInfo == null ? 0 : subAccountInfo.getId());
    }
}

