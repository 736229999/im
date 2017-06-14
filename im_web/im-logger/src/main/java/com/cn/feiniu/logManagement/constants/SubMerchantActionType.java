package com.cn.feiniu.logManagement.constants;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by wangchen on 2015/8/3.
 */
public enum SubMerchantActionType {
    TYPE_LOGIN((byte) 1, "登录"),
    TYPE_LOGOUT((byte) 2, "登出"),
    TYPE_HANG_START((byte) 3, "挂起开始"),
    TYPE_HANG_OVER((byte) 4, "挂起结束"),
    TYPE_LEAVE_START((byte) 5, "离开开始"),
    TYPE_LEAVE_END((byte) 6, "离开结束"),;

    SubMerchantActionType(byte type, String description) {
        this.type = type;
        this.descripton = description;
    }

    private byte type;
    private String descripton;
    private static final Map<Byte, SubMerchantActionType> typeToEnum;

    public static final SubMerchantActionType getType(byte type) {
        return typeToEnum.get(type);
    }

    static {
        typeToEnum = new HashMap<>(SubMerchantActionType.values().length);
        for (SubMerchantActionType subMerchantActionType : values()) {
            typeToEnum.put(subMerchantActionType.type, subMerchantActionType);
        }
    }
}
