package com.feiniu.csWebIm.bean.domain;

/**
 * Created by chenwuming on 2015/11/16.
 */
public class OrderDetailInfo {
    private String ogNo;
    private double totalPay;
    private String insertDate;

    public String getOgNo() {
        return ogNo;
    }

    public void setOgNo(String ogNo) {
        this.ogNo = ogNo;
    }

    public double getTotalPay() {
        return totalPay;
    }

    public void setTotalPay(double totalPay) {
        this.totalPay = totalPay;
    }

    public String getInsertDate() {
        return insertDate;
    }

    public void setInsertDate(String insertDate) {
        this.insertDate = insertDate;
    }
}
