package com.feiniu.csWebIm.bean.request;

/**
 * Created by wangzhen on 2015/11/2.
 */
public class ServiceLogRequset {

    String customerServiceName;//客服名称
    String customerServiceId;//客服ID
    String customerId ;//顾客ID
    String goodsAskingType ;
    String orId;

    public String getOrId() {
        return orId;
    }

    public void setOrId(String orId) {
        this.orId = orId;
    }

    public String getCustomerServiceName() {
        return customerServiceName;
    }

    public void setCustomerServiceName(String customerServiceName) {
        this.customerServiceName = customerServiceName;
    }

    public String getCustomerServiceId() {
        return customerServiceId;
    }

    public void setCustomerServiceId(String customerServiceId) {
        this.customerServiceId = customerServiceId;
    }

    public String getCustomerId() {
        return customerId;
    }

    public void setCustomerId(String customerId) {
        this.customerId = customerId;
    }

    public String getGoodsAskingType() {
        return goodsAskingType;
    }

    public void setGoodsAskingType(String goodsAskingType) {
        this.goodsAskingType = goodsAskingType;
    }
}
