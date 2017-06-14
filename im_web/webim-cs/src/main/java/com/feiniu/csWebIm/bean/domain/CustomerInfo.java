package com.feiniu.csWebIm.bean.domain;

/**
 * Created by wangzhen on 2015/10/21.
 */
public class CustomerInfo {
    private boolean isDefaultAddr;
    private String customerName;
    private String customerTel;

    public boolean getDefaultAddr() {
        return isDefaultAddr;
    }

    public void SetDefaultAddr(boolean isDefaultAddr) {
        this.isDefaultAddr = isDefaultAddr;
    }

    public String getCustomerName() {
        return customerName;
    }
    public void setCustomerName(String customerName) {
        this.customerName = customerName;
    }

    public String getCustomerTel() {
        return customerTel;
    }

    public void setCustomerTel(String customerTel) {
        this.customerTel = customerTel;
    }
}
