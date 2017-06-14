package com.feiniu.csWebIm.bean.response;

import com.feiniu.csWebIm.bean.domain.CommenQustion;
import com.feiniu.csWebIm.bean.domain.PackageDetail;

import java.util.List;

/**
 * Created by wangzhen on 2015/11/5.
 */
public class WebRightResponse {
    private String orderUrl;
    private String title;
    private String orderNo;//订单号
    private String payType;//支付方式
    private double price;//支付金额
    private String orderTime;//下单时间
    private String orderStatus;
    private List<PackageDetail> packageDetailList;
    private List<CommenQustion> commonQustionsList;

    public String getOrderUrl() {
        return orderUrl;
    }

    public void setOrderUrl(String orderUrl) {
        this.orderUrl = orderUrl;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public List<CommenQustion> getCommonQustionsList() {
        return commonQustionsList;
    }

    public void setCommonQustionsList(List<CommenQustion> commonQustionsList) {
        this.commonQustionsList = commonQustionsList;
    }

    public String getOrderNo() {
        return orderNo;
    }

    public void setOrderNo(String orderNo) {
        this.orderNo = orderNo;
    }

    public String getPayType() {
        return payType;
    }

    public void setPayType(String payType) {
        this.payType = payType;
    }

    public double getPrice() {
        return price;
    }

    public void setPrice(double price) {
        this.price = price;
    }

    public String getOrderTime() {
        return orderTime;
    }

    public void setOrderTime(String orderTime) {
        this.orderTime = orderTime;
    }

    public String getOrderStatus() {
        return orderStatus;
    }

    public void setOrderStatus(String orderStatus) {
        this.orderStatus = orderStatus;
    }

    public List<PackageDetail> getPackageDetailList() {
        return packageDetailList;
    }

    public void setPackageDetailList(List<PackageDetail> packageDetailList) {
        this.packageDetailList = packageDetailList;
    }
}
