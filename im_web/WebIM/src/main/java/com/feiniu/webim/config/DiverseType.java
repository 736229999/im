package com.feiniu.webim.config;

/**
 * Author: morningking
 * Date: 2016/4/13 15:02
 * Contact: 243717042@qq.com
 */
public enum DiverseType {
    FROM_MERCHANT_DIVERSE_AMONG_ALL(0, "商城-按商家分流"),
    FROM_MERCHANT_DIVERSE_AMONG_GROUP(1, "商城-按商家分组分流"),
    FROM_CS_DIVERSE_AMONG_GROUP(2, "自营-按组分流"),;

    private int type;
    private String description;

    DiverseType(int type, String description) {
        this.type = type;
        this.description = description;
    }

    public int getType() {
        return type;
    }
}
