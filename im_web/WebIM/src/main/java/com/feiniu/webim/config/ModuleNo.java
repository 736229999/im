package com.feiniu.webim.config;

import java.util.HashMap;
import java.util.Map;

/**
 * Author: morningking
 * Date: 2015/11/6
 * Contact: 243717042@qq.com
 * <p/>
 * 区分请求来源模块
 */
public enum ModuleNo {
    MERCHANT_WEBIM(0),
    CS_WEBIM(1),
    ;

    private int no;

    ModuleNo(int no) {
        this.no = no;
    }

    private static final Map<Integer, ModuleNo> MAP = new HashMap<>(ModuleNo.values().length);
    public int getNo() {
        return no;
    }

    public static ModuleNo getModuleNo(int type) {
        return MAP.get(type);
    }

    static {
        for (ModuleNo moduleNo : values()) {
            MAP.put(moduleNo.no, moduleNo);
        }
    }
}
