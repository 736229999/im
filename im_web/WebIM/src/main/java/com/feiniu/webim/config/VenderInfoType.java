package com.feiniu.webim.config;

import java.util.HashMap;

/**
 * Created by wangchen on 2015/1/14.
 */
public enum VenderInfoType {
    GET_MERCHANT_INFO,
    GET_STORE_INFO,
    GET_STORE_RELATED_INFO_FOR_PC,
    GET_PRODUCT_INFO,
    GET_PRODUCT_INFO_FOR_MERCHANT,
    GET_USER_INFO,
    GET_STORE_LIGHT_CONFIG,
    GET_STORE_COMMENT_INFO,
    GET_USER_RELATED_STORE_ORDER_INFO,
    GET_PRODUCT_ITEM_URL_BY_GOOD_ID,
    GET_RECOMMEND_PRODUCT_LIST,
    GET_MERCHANT_ONLINE_INFO,
    GET_SUB_MERCHANT_ONLINE_INFO,
    GET_FEINIU_ITEM_INFO,
    CHECK_SESSION_TOKEN_LEGALITY,
    GET_STAFF_INFO,
    //获取主子商家账号
    GET_MAIN_SUB_MERCHANT_ID,
    ////获取子账号对应的商家账号
    GET_MERCHANT_IDS_BY_MEMBER_IDS,
    /**
     * 同步系统时间
     */
    SYNC_TIME,
    GET_SHOP_GOODS_LIST,
    GET_BUYER_INFO,
    GET_ORDER_INFO,
    REMARK_ORDER,
    ADD_COOKIE,
    //查询用户是否登录
    QUERY_USER_LOGIN_INFO,
    //查询店铺名称
    GET_STORE_NAME;

    private static final HashMap<String, VenderInfoType> MAP = new HashMap<>();

    static {
        for (VenderInfoType eventType : values()) {
            MAP.put(eventType.type, eventType);
        }
    }

    private String type;

    VenderInfoType() {
        this.type = this.name();
    }

    public static final VenderInfoType getVenderInfoType(String type) {
        return MAP.get(type);
    }

    @Override
    public String toString() {
        return type;
    }

}
