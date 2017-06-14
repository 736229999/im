package com.feiniu.csWebIm.constants;

import java.util.HashMap;
import java.util.Map;

/**
 * Author: morningking
 * Date: 2016/4/19 11:30
 * Contact: 243717042@qq.com
 */
public enum FromPageType {
    PC_PORTAL(0, "portal"),
    PC_CART(1, "cart"),
    PC_ORDER(2, "order"),
    UNKNOWN(-1, "unknown"),;

    private int type;
    private String descriptor;

    public static final Map<String, Integer> MAP = new HashMap<>();

    static {
        for (FromPageType fromPageType : values()) {
            MAP.put(fromPageType.descriptor, fromPageType.type);
        }
    }

    FromPageType(int type, String descriptor) {
        this.type = type;
        this.descriptor = descriptor;
    }

    public static final int getType(String descriptor) {
        Integer type = MAP.get(descriptor);

        if (null == type) {
            type = UNKNOWN.type;
        }

        return type;
    }
}
