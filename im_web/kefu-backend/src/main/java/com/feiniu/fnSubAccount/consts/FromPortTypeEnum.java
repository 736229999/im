package com.feiniu.fnSubAccount.consts;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by wangzhen on 2016/4/11.
 */
public enum FromPortTypeEnum {
    PORT_TYPE_UNKNOW(-1,"未知来源"),
    PORT_TYPE_PROTAL(0,"portal页"),
    PORT_TYPE_BUYER_CAR(1,"购物车"),
    PORT_TYPE_ORDER_PAGE(2,"订单页");

    private int id;
    private String showInfo;

    FromPortTypeEnum(int id, String showInfo) {
        this.id = id;
        this.showInfo= showInfo;
    }

    private static  final Map<Integer,FromPortTypeEnum> idToInfo = new HashMap<>(values().length);

    static {
        for (FromPortTypeEnum embedEnum : values()) {
            idToInfo.put(embedEnum.id, embedEnum);
        }
    }

    public static FromPortTypeEnum getInfo(int id){
        return  idToInfo.get(id);
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getShowInfo() {
        return showInfo;
    }

    public void setShowInfo(String showInfo) {
        this.showInfo = showInfo;
    }

    public static Map<Integer, FromPortTypeEnum> getIdToInfo() {
        return idToInfo;
    }
}
