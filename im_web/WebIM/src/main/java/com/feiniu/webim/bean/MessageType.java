package com.feiniu.webim.bean;

import java.util.HashMap;

/**
 * Created by wangchen on 2014/12/5.
 */
public enum MessageType {
    //普通消息
    GENERAL_MESSAGE(0),
    //服务端推送评价
    SELLER_PUSH_EVALUATE(7),
    //webim初始化聊天环境
    INIT_CHAT_ENV_MESSAGE_TYPE(18),
    //图片真实地址消息
    SENDING_REAL_PHOTO_URL(19),
    // 客服转接消息
    SUB_MERCHANT_SWITCH(20),
    // 买家首次欢迎光临消息
    WELCOME(21),
    //同步正在联系的子账号
    SYNC_SUBMERCHANT_LIST_IN_CHATTING(23),
    //提示之前打开的窗口准备关闭
    PREPARE_TO_CLOSE(24),
    //提示买家注意发言时间
    ALERT_CUSTOMER_SPEAK_TIME(25),
    //主管插入会话
    MONITOR_TAKE_OVER_CHAT(26),
    ;

    private int type;
    private static final HashMap<Integer, MessageType> MAP = new HashMap<>(values().length);

    MessageType(int type) {
        this.type = type;
    }

    public int getType() {
        return type;
    }

    public static MessageType getMessageType(int type) {
        return MAP.get(type);
    }

    static {
        for (MessageType messageType : values()) {
            MAP.put(messageType.getType(), messageType);
        }
    }
}
