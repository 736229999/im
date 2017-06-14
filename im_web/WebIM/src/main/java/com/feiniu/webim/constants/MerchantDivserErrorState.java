package com.feiniu.webim.constants;

/**
 * 客服分流错误状态
 * Author: morningking
 * Date: 2016/4/19 9:07
 * Contact: 243717042@qq.com
 */
public enum MerchantDivserErrorState {
    SERVICE_NOT_AVAILABLE(-1, "没有客服在线"),
    SERVICE_ON_GOING(-2, "已经在被接待了"),;

    private int state;
    private String description;

    MerchantDivserErrorState(int state, String description) {
        this.state = state;
        this.description = description;
    }

    public int getState() {
        return state;
    }
}
