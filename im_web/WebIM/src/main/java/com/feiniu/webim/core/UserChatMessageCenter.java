package com.feiniu.webim.core;

import com.fasterxml.jackson.databind.JsonNode;
import com.feiniu.webim.bean.MessageType;
import com.feiniu.webim.bean.domain.ChatWindowHandle;
import com.feiniu.webim.bean.domain.MerchantSubAccountInfo;
import com.feiniu.webim.bean.response.BaseResponseBean;
import com.feiniu.webim.bean.response.ChatResponseBean;
import com.feiniu.webim.bean.web.response.MerchantUnreadMsgListResponse;
import com.feiniu.webim.constants.Constants;
import com.feiniu.webim.constants.SwitchStatus;
import com.feiniu.webim.util.json.JSONUtil;
import com.feiniu.webim.util.json.JsonNodeUtil;

import java.util.*;

/**
 * Author: morningking
 * Date: 2015/8/20
 * Contact: 243717042@qq.com
 * <p/>
 * 每用户Session一个此对象，用来管理用户会话的聊天消息
 * 共分为两层，每个用户Session会包含多个浏览器进程过来的链接，每个链接的身份标识为authToken
 * 每个浏览器进程会包含多个会话窗口[某个时刻只会有一个会话窗口是有效的]
 */
public class UserChatMessageCenter {
    //店铺ID对应的未读消息数,按不同的token保存
    private HashMap<String, HashMap<Integer, Integer>> mapUnReadCount = new HashMap<>(1);
    //店铺ID对应的最后聊天的客服ID,按不同的token保存
    private HashMap<String, HashMap<Integer, Integer>> mapLastSubAccountID = new HashMap<>(1);

    private volatile Session session;

    public Session getSession() {
        return session;
    }

    public void setSession(Session session) {
        this.session = session;
    }

    //token --> {[uuid1 --> mq], [uuid2 --> mq] ...]}
    private HashMap<String, Map<String, MessageQueue>> tokenToChatHandleList = new HashMap<>(1);

    //uuid对应的商家ID，用于触屏版过滤不同商家的消息
    private HashMap<String, Integer> uuidToMerchantID = new HashMap<>(1);

    public boolean getHasUnreadMsg(String token, Integer merchantID) {
        synchronized (this) {
            for (Map.Entry<String, HashMap<Integer, Integer>> tmp : mapUnReadCount.entrySet()) {
                if (tmp.getKey().equals(token)) {
                    HashMap<Integer, Integer> mapMerchantToUnreadCount = tmp.getValue();
                    if (mapMerchantToUnreadCount != null && mapMerchantToUnreadCount.size() > 0) {
                        for (Map.Entry<Integer, Integer> entry : mapMerchantToUnreadCount.entrySet()) {
                            if (entry.getKey() == merchantID) {
                                break;
                            }
                            if (entry.getValue() > 0) {
                                return true;
                            }
                        }
                    }
                }
            }
            return false;
        }
    }

    //根据token获取未读消息数
    public List<MerchantUnreadMsgListResponse> getUnreadMsgList(String token) {
        synchronized (this) {
            List<MerchantUnreadMsgListResponse> unreadMsgList = new ArrayList<>(1);
            for (Map.Entry<String, HashMap<Integer, Integer>> tmp : mapUnReadCount.entrySet()) {
                if (tmp.getKey().equals(token)) {
                    for (Map.Entry<Integer, Integer> e : tmp.getValue().entrySet()) {
                        MerchantUnreadMsgListResponse unreadMsg = new MerchantUnreadMsgListResponse();
                        unreadMsg.setMerchantId(e.getKey());
                        unreadMsg.setUnreadMsgCount(e.getValue());
                        unreadMsg.setLastSubAccountId(mapLastSubAccountID.get(token).get(e.getKey()));
                        unreadMsgList.add(unreadMsg);
                    }
                    break;
                }
            }
            return unreadMsgList;
        }
    }

    //清空未读消息列表，merchantId为0时删除这个uuid的所有未读消息列表
    public void removeUnreadMap(String token, Integer merchantId) {
        synchronized (this) {
            if (merchantId == 0) {
                if (mapUnReadCount.get(token) != null) {
                    mapUnReadCount.remove(token);
                }
                if (mapLastSubAccountID.get(token) != null) {
                    mapLastSubAccountID.remove(token);
                }
            } else {
                if (mapUnReadCount.get(token) != null) {
                    if (mapUnReadCount.get(token).get(merchantId) != null) {
                        mapUnReadCount.get(token).remove(merchantId);
                    }
                }
                if (mapLastSubAccountID.get(token) != null) {
                    if (mapLastSubAccountID.get(token).get(merchantId) != null) {
                        mapLastSubAccountID.get(token).remove(merchantId);
                    }
                }
            }
        }
    }

    //获取uuid对应的商家ID，页面发送消息时只能通过uuid获取商家ID
    public Integer getUuidToMerchantID(String uuid) {
        synchronized (this) {
            if (uuidToMerchantID != null && uuidToMerchantID.size() > 0) {
                if (uuidToMerchantID.get(uuid) != null) {
                    return uuidToMerchantID.get(uuid);
                }
            }
            return 0;
        }
    }

    /**
     * 当买家在浏览器端打开聊天窗口时，会调用此方法，维系token、uuid的映射关系
     */
    public void addHandleMapping(ChatWindowHandle chatWindowHandle) {
        synchronized (this) {
            Map<String, MessageQueue> queueListGuardByCurrentToken = tokenToChatHandleList.get(chatWindowHandle.getAuthToken());

            uuidToMerchantID.put(chatWindowHandle.getUuid(), chatWindowHandle.getMerchantID());

            if (queueListGuardByCurrentToken == null) {
                queueListGuardByCurrentToken = new HashMap<>(1);
                tokenToChatHandleList.put(chatWindowHandle.getAuthToken(), queueListGuardByCurrentToken);
            }

            if (!queueListGuardByCurrentToken.containsKey(chatWindowHandle.getUuid())) {
                queueListGuardByCurrentToken.put(chatWindowHandle.getUuid(), new MessageQueue());
            }

            if (mapUnReadCount.get(chatWindowHandle.getAuthToken()) == null) {
                mapUnReadCount.put(chatWindowHandle.getAuthToken(), new HashMap<Integer, Integer>(1));
                mapLastSubAccountID.put(chatWindowHandle.getAuthToken(), new HashMap<Integer, Integer>(1));
            }
        }
    }

    /**
     * web买家端 关闭聊天窗口时，会触发此方法，除去相关映射
     */
    public void removeHandleMapping(ChatWindowHandle chatWindowHandle) {
        synchronized (this) {
            Map<String, MessageQueue> queueMapUnderToken = tokenToChatHandleList.get(chatWindowHandle.getAuthToken());

            uuidToMerchantID.remove(chatWindowHandle.getUuid());
            if (queueMapUnderToken != null) {
                MessageQueue messageQueue = queueMapUnderToken.remove(chatWindowHandle.getUuid());

                if (messageQueue != null) {
                    messageQueue.destroy();
                }
            }
        }
    }

    @SuppressWarnings(value = "unchecked")
    public List<BaseResponseBean> retrieveAllChatMessage(ChatWindowHandle chatWindowHandle) {
        MessageQueue messageQueue;
        synchronized (this) {
            Map<String, MessageQueue> queueMapUnderToken = tokenToChatHandleList.get(chatWindowHandle.getAuthToken());

            messageQueue = queueMapUnderToken.get(chatWindowHandle.getUuid());
            if (messageQueue == null) {
                return Collections.EMPTY_LIST;
            }
        }

        List list = messageQueue.pollAllChatMessage();
        Collections.sort(list);

        return list;
    }

    /**
     * 卖家发消息给买家时，应该同步到所有的 聊天会话页面
     */
    public int putMessage(BaseResponseBean responseMessage) {
        if (responseMessage == null || !(responseMessage instanceof ChatResponseBean)) {
            return -1;
        }

        boolean isMerchantIDUnreadSet = false;
        boolean isPutSuccess = false;
        ChatResponseBean chatResponseBean = (ChatResponseBean) responseMessage;
        synchronized (this) {
            for (Map.Entry<String, Map<String, MessageQueue>> chatHandlerListEntry : tokenToChatHandleList.entrySet()) {
                for (Map.Entry<String, MessageQueue> uuidEntry : chatHandlerListEntry.getValue().entrySet()) {
                    MessageType messageType = MessageType.getMessageType(chatResponseBean.getType());
                    if (messageType != null) {
                        String uuid = uuidEntry.getKey();
                        String token = chatHandlerListEntry.getKey();
                        Integer currentSubMerchantIDUnderToken = session.getCurrentSubAccountIDUnderToken(token);
                        Integer merchantID = chatResponseBean.getMerchantId();
                        Integer sender = chatResponseBean.getSender();

                        switch (messageType) {
                            case GENERAL_MESSAGE:
                                //如果是模拟消息
                                if (chatResponseBean.isFake()) {
                                    //如果为触屏版判断窗口的店铺ID如果和会话中的店铺ID是否一致，不一致则不发送消息
                                    if (session.isUuidFromMobile(uuidEntry.getKey())) {
                                        if (uuidToMerchantID.get(uuidEntry.getKey()) != null
                                                && chatResponseBean.getMerchantId() != 0
                                                && uuidToMerchantID.get(uuidEntry.getKey()) != chatResponseBean.getMerchantId()) {
                                            isPutSuccess = true;
                                            break;
                                        }
                                    }
                                    //除非窗口句柄不一致，否则不转发此模拟消息
                                    if (!chatHandlerListEntry.getKey().equals(chatResponseBean.getToken())
                                            || !uuidEntry.getKey().equals(chatResponseBean.getUuid())) {
                                        isPutSuccess = uuidEntry.getValue().add(responseMessage) || isPutSuccess;
                                    } else {
                                        isPutSuccess = true;
                                    }
                                } else {
                                    //如果来自自营
                                    if (Constants.isKefuFromCS(sender) && session.isUuidFromCS(uuid)) {
                                        MerchantSubAccountInfo merchantSubAccountInfo = session.getCurrentKefu(uuid);
                                        if (merchantSubAccountInfo != null) {
                                            if (chatResponseBean.getMsgtype() == 2409) {
                                                isPutSuccess = merchantSubAccountInfo.getId() == chatResponseBean.getSender()
                                                        && uuidEntry.getValue().add(responseMessage) || isPutSuccess;
                                            } else {
                                                isPutSuccess = uuidEntry.getValue().add(responseMessage) || isPutSuccess;
                                            }
                                        }
                                    } else if (!Constants.isKefuFromCS(sender)) {
                                        //如果是手机并且不是当前联系人[不用区分该窗口是否来自自营]
                                        if (chatResponseBean.getSender() != currentSubMerchantIDUnderToken
                                                && session.isUuidFromMobile(uuid)) {
                                            //更新店铺维度未读消息数
                                            HashMap<Integer, Integer> mapMerchantIDUnReadCount = mapUnReadCount.get(token);
                                            Integer unReadCount = mapMerchantIDUnReadCount.get(merchantID);
                                            if (unReadCount == null) {
                                                unReadCount = 0;
                                            }

                                            if (!isMerchantIDUnreadSet) {
                                                unReadCount++;
                                                isMerchantIDUnreadSet = true;
                                            }

                                            mapMerchantIDUnReadCount.put(merchantID, unReadCount);

                                            //更新店铺内最近一条消息来自哪位客服
                                            HashMap<Integer, Integer> mapLastSubMerchantIDUnderMerchant = mapLastSubAccountID.get(token);
                                            mapLastSubMerchantIDUnderMerchant.put(merchantID, chatResponseBean.getSender());
                                        } else {
                                            isPutSuccess = uuidEntry.getValue().add(responseMessage) || isPutSuccess;
                                        }
                                    }
                                }
                                break;
                            //如果是通知关闭窗口消息，那么只发给相同token的
                            case PREPARE_TO_CLOSE:
                                boolean needToPut = false;

                                //如果是来自自营客服，只转发给自营客服窗口中uuid不相同的
                                if (chatResponseBean.isFromCS()) {
                                    needToPut = !uuidEntry.getKey().equals(chatResponseBean.getUuid())
                                            && session.isUuidFromCS(uuidEntry.getKey());
                                } else {
                                    //如果不是来自自营客服，只转发给token一样，且uuid不一样的窗口
                                    needToPut = chatHandlerListEntry.getKey().equals(chatResponseBean.getToken()) &&
                                            !uuidEntry.getKey().equals(chatResponseBean.getUuid());
                                }

                                if (needToPut) {
                                    isPutSuccess = uuidEntry.getValue().add(responseMessage) || isPutSuccess;
                                }
                                break;
                            case SUB_MERCHANT_SWITCH:
//                            case MONITOR_TAKE_OVER_CHAT:
                                //如果是转接消息,找到用户正在被服务的那个窗口句柄,更新必要信息
                                if (Constants.isKefuFromCS(sender) && session.isUuidFromCS(uuid)) {
                                    ChatWindowHandle chatWindowHandle = session.getCurrentChatWindow(uuid);

                                    if (chatWindowHandle != null && chatWindowHandle.getQueueLength() == 0) {
                                        MerchantSubAccountInfo subAccountInfo = chatWindowHandle.getMerchantSubAccountInfo();

                                        if (subAccountInfo != null) {
                                            String message = chatResponseBean.getMsg();
                                            JsonNode jsonNode = JSONUtil.toNode(message);
                                            if (jsonNode != null && JsonNodeUtil.getInteger(jsonNode, "switchStatus") !=
                                                    SwitchStatus.GENERAL_SWITCH_START.getStatus()) {
                                                subAccountInfo.setUsername(JsonNodeUtil.getString(jsonNode, "toSubMerchantName"));
                                                subAccountInfo.setId(JsonNodeUtil.getInteger(jsonNode, "toSubMerchantID"));
                                            }
                                        }
                                    }
                                }
                            default:
                                if (chatResponseBean.isFake()) {
                                    isPutSuccess = uuidEntry.getValue().add(responseMessage) || isPutSuccess;
                                } else {
                                    if (Constants.isKefuFromCS(sender) && session.isUuidFromCS(uuid)) {
                                        MerchantSubAccountInfo merchantSubAccountInfo = session.getCurrentKefu(uuid);
                                        if (merchantSubAccountInfo != null) {
                                            isPutSuccess = uuidEntry.getValue().add(responseMessage) || isPutSuccess;
                                        }
                                    } else if (!Constants.isKefuFromCS(sender) && !session.isUuidFromCS(uuid)) {
                                        isPutSuccess = uuidEntry.getValue().add(responseMessage) || isPutSuccess;
                                    }
                                }

                                break;
                        }
                    }
                }
            }

            chatResponseBean.setUuid(null);
            chatResponseBean.setToken(null);
        }

        return isPutSuccess ? 1 : 0;
    }

    public synchronized void destroy() {
        tokenToChatHandleList.clear();
        tokenToChatHandleList = null;
        uuidToMerchantID.clear();
        uuidToMerchantID = null;
        //清空未读消息数列表
        if (mapLastSubAccountID != null) {
            for (Map.Entry<String, HashMap<Integer, Integer>> tmp : mapLastSubAccountID.entrySet()) {
                tmp.getValue().clear();
            }
        }
        if (mapUnReadCount != null) {
            for (Map.Entry<String, HashMap<Integer, Integer>> tmp : mapUnReadCount.entrySet()) {
                tmp.getValue().clear();
            }
        }
        mapLastSubAccountID.clear();
        mapUnReadCount.clear();
    }
}
