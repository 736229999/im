package com.feiniu.csWebIm.bean.domain;

/**
 * Created by chenwuming on 2015/11/25.
 */
public class CustomerCostInfo {
    private Integer orderTotal;
    private Integer commodityTotal;
    private String  meanPrice;

    public Integer getOrderTotal() {
        return orderTotal;
    }

    public void setOrderTotal(Integer orderTotal) {
        this.orderTotal = orderTotal;
    }

    public Integer getCommodityTotal() {
        return commodityTotal;
    }

    public void setCommodityTotal(Integer commodityTotal) {
        this.commodityTotal = commodityTotal;
    }

    public String getMeanPrice() {
        return meanPrice;
    }

    public void setMeanPrice(String meanPrice) {
        this.meanPrice = meanPrice;
    }
}
