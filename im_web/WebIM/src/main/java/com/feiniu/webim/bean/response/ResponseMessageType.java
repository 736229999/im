package com.feiniu.webim.bean.response;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by wangchen on 2014/12/8.
 */
public enum ResponseMessageType {
    LOGIN(0x212),
    PUT_MESSAGE_REPLY(0x60c),
    CHAT_FORWARD(0x60d),
    RETRIVE_MERCHANT_SUB_ACCOUNT(0x0944),
    RETRIEVE_USER_INFO(0x0416),
    RETRIEVE_HISOTRY_MESSAGE(0x0617),
    RETRIVE_RECENTLY_CONTACTS_FOR_BUYER(0x0619),
	RETRIVE_RECENTLY_MOBILE_CONTACTS_FOR_BUYER(0x0966),
    RETRIEVE_MERCHANT_ONLINE_INFO(0x0946),
    RETRIVE_SUB_MERCHANT_ONLINE_INFO(0x948),
    HEART_BEAT(0x302),
    ;

    private static final Map<Integer, ResponseMessageType> MAP = new HashMap<>();

    static {
        for (ResponseMessageType type : values()) {
            MAP.put(type.getTypeValue(), type);
        }
    }

    private int responseMessageType;

    ResponseMessageType(int responseMessageType) {
        this.responseMessageType = responseMessageType;
    }

    public int getTypeValue() {
       return responseMessageType;
    }

    public static final ResponseMessageType getResponseMessageType(int typeValue) {
        return MAP.get(typeValue);
    }
}
