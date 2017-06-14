package com.feiniu.webim.config;

import java.util.HashMap;

/**
 * Created by wangchen on 2014/12/16.
 */
public enum EventType {
    //--------------消息类型-----------
    /**
     * 初始化聊天环境
     */
    INIT_CHAT_ENV,

    /**
     * 初始化布点环境
     */
    INIT_EMBED_ENV,

    /**
     * 获取用户个人信息*
     */
    GET_USER_INFO,

    /**
     * getlist消息,请求消息列表
     */
    GET_CHAT_MESSAGE_LIST,

    /**
     * putmsg,消息,添加消息到列表
     */
    PUT_CHAT_MESSAGE,

    /**
     * 获取手机端未读消息数
     */
    GET_CHAT_UNREAD_COUNT_LIST,
	
    /**
     * exit,用户下线
     */
    EXIT,

    /**获取历史信息列表*/
    GET_HISTORY_MESSAGE_LIST,

    /**获取商家下面的子账号信息*/
    GET_MERCHANT_SUB_ACCOUNT,

    /**获取最近联系人*/
    GET_RECENTYLE_CONTACTS_FOR_BUYER,

    /**获取手机端最近联系人*/
    GET_RECENTYLE_MOBILE_CONTACTS_FOR_BUYER,
    /**获取商家在线情况*/
    GET_MERCHANT_ONLINE_INFO,

    /**获取子账号在线情况*/
    GET_SUB_MERCHANT_ONLINE_INFO,

    /**获取客服分组在线情况*/
    GET_SERVICE_GROUP_ONLINE_INFO,

    /**获取正在排队的人数**/
    PULL_QUEUE_INFO,
    ;

    private static final HashMap<String, EventType> MAP = new HashMap<>();

    static {
        for (EventType eventType : values()) {
            MAP.put(eventType.type, eventType);
        }
    }

    private String type;
    private EventType() {
        this.type = this.name();
    }

    public static final EventType getEventType(String type) {
        return MAP.get(type);
    }

    @Override
    public String toString() {
        return type;
    }

}
