package com.feiniu.csWebIm.bean.domain;

import java.text.DecimalFormat;

/**
 * Created by chenwuming on 2015/11/16.
 */
public class CustomerCost {
    private Integer orderTotal;
    private Integer commodityTotal;
    private double consumeTotal;

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

    public double getConsumeTotal() {
        return consumeTotal;
    }

    public void setConsumeTotal(double consumeTotal) {
        this.consumeTotal = consumeTotal;
    }

    public String getMeanPrice() {
        DecimalFormat df   = new DecimalFormat("######0.00");
        double result = 0.0;
        if (orderTotal != 0)
            result = consumeTotal/orderTotal;
        return String.valueOf(df.format(result));//金额/笔数
    }
}
