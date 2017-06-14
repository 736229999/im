package com.feiniu.csWebIm.bean.domain;

/**
 * Created by chenwuming on 2015/11/16.
 */
public class OrderDetail {
    private String ogNo = "--";
    private String ogSeq= "--";
    private String memGuid= "--";
    private double totalPay=0.0;//Web页面显示金额
    private String memberName= "--";
    private String memberCellphone= "--";
    private String insertDate= "--";
    private double orderPrice= 0.0;//哞哞客户端显示金额
    private String payType= "--";

    public String getOgNo() {
        return ogNo;
    }

    public void setOgNo(String ogNo) {
        this.ogNo = ogNo;
    }

    public String getOgSeq() {
        return ogSeq;
    }

    public void setOgSeq(String ogSeq) {
        this.ogSeq = ogSeq;
    }

    public String getMemGuid() {
        return memGuid;
    }

    public void setMemGuid(String memGuid) {
        this.memGuid = memGuid;
    }

    public double getTotalPay() {
        return totalPay;
    }

    public void setTotalPay(double totalPay) {
        this.totalPay = totalPay;
    }

    public String getMemberName() {
        return memberName;
    }

    public void setMemberName(String memberName) {
        this.memberName = memberName;
    }

    public String getMemberCellphone() {
        return memberCellphone;
    }

    public void setMemberCellphone(String memberCellphone) {
        this.memberCellphone = memberCellphone;
    }

    public String getInsertDate() {
        return insertDate;
    }

    public void setInsertDate(String insertDate) {
        this.insertDate = insertDate;
    }

    public double getOrderPrice() {
        return orderPrice;
    }

    public void setOrderPrice(double orderPrice) {
        this.orderPrice = orderPrice;
    }

    public String getPayType() {
        return payType;
    }

    public void setPayType(String payType) {
        this.payType = payType;
    }
}
