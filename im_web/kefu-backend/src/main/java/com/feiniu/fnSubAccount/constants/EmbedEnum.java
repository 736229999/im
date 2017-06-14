package com.feiniu.fnSubAccount.constants;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by wangchen on 2016/1/19.
 */
//布点常量信息
public enum EmbedEnum {
    PORTAL(0, "portal", "Portal"),
    CART(1, "cart", "购物车"),
    ORDER(2, "order", "订单页");

    private int id;
    private String name;
    private String showName;
    private static final Map<Integer, EmbedEnum> idToName = new HashMap<>(values().length);

    static {
        for (EmbedEnum embedEnum : values()) {
            idToName.put(embedEnum.id, embedEnum);
        }
    }

    EmbedEnum(int id, String name, String showName) {
        this.id = id;
        this.name = name;
        this.showName = showName;
    }

    public static final String getName(int id) {
        EmbedEnum embedEnum = idToName.get(id);

        if (embedEnum == null) {
            return null;
        } else {
            return embedEnum.name;
        }
    }

    public static final String getShowName(int id) {
        EmbedEnum embedEnum = idToName.get(id);

        if (embedEnum == null) {
            return null;
        } else {
            return embedEnum.showName;
        }
    }

    public int getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public String getShowName() {
        return showName;
    }
}
