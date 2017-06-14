package com.feiniu.csWebIm.bean.response;


import com.feiniu.csWebIm.bean.domain.PackageDetail;

import java.util.List;

/**
 * Created by wangzhen on 2015/10/21.
 */
public class ServiceLogInfoResponse {
    private String customerId ;
    private String customerName ;
    private String customerNickName ;
    private String customerLevel ;
    private String orderId ;
    private String orderStatus ;
    private String orderList ;
    private String lastOrderTime ;
    private String lastOrderStatus ;
    private String lastOrderId  ;
    private String lastTotalCost  ;
    private String allDealCount ;
    private String totalGoodsNum ;
    private String meanPrice  ;

    private String customerServiceName;
    private List askingPictures ;
    private List<PackageDetail> packageDetailList;

    private String connectName ;
    private String connectNumber;
    private String serviceLog ;
    private List askingOptions ;
    private List historyServiceLog ;
    private List askingGoodsInfo;
    private String goodsAskingType;

    public String getCustomerId() {
        return customerId;
    }

    public void setCustomerId(String customerId) {
        this.customerId = customerId;
    }

    public String getCustomerName() {
        return customerName;
    }

    public void setCustomerName(String customerName) {
        this.customerName = customerName;
    }

    public String getCustomerNickName() {
        return customerNickName;
    }

    public void setCustomerNickName(String customerNickName) {
        this.customerNickName = customerNickName;
    }

    public String getCustomerLevel() {
        return customerLevel;
    }

    public void setCustomerLevel(String customerLevel) {
        this.customerLevel = customerLevel;
    }

    public String getOrderId() {
        return orderId;
    }

    public void setOrderId(String orderId) {
        this.orderId = orderId;
    }

    public String getOrderStatus() {
        return orderStatus;
    }

    public void setOrderStatus(String orderStatus) {
        this.orderStatus = orderStatus;
    }

    public String getOrderList() {
        return orderList;
    }

    public void setOrderList(String orderList) {
        this.orderList = orderList;
    }

    public String getLastOrderTime() {
        return lastOrderTime;
    }

    public void setLastOrderTime(String lastOrderTime) {
        this.lastOrderTime = lastOrderTime;
    }

    public String getLastOrderStatus() {
        return lastOrderStatus;
    }

    public void setLastOrderStatus(String lastOrderStatus) {
        this.lastOrderStatus = lastOrderStatus;
    }

    public String getLastOrderId() {
        return lastOrderId;
    }

    public void setLastOrderId(String lastOrderId) {
        this.lastOrderId = lastOrderId;
    }

    public String getLastTotalCost() {
        return lastTotalCost;
    }

    public void setLastTotalCost(String lastTotalCost) {
        this.lastTotalCost = lastTotalCost;
    }

    public String getAllDealCount() {
        return allDealCount;
    }

    public void setAllDealCount(String allDealCount) {
        this.allDealCount = allDealCount;
    }

    public String getMeanPrice() {
        return meanPrice;
    }

    public void setMeanPrice(String meanPrice) {
        this.meanPrice = meanPrice;
    }

    public String getTotalGoodsNum() {
        return totalGoodsNum;
    }

    public void setTotalGoodsNum(String totalGoodsNum) {
        this.totalGoodsNum = totalGoodsNum;
    }

    public String getCustomerServiceName() {
        return customerServiceName;
    }

    public void setCustomerServiceName(String customerServiceName) {
        this.customerServiceName = customerServiceName;
    }

    public List getAskingPictures() {
        return askingPictures;
    }

    public void setAskingPictures(List askingPictures) {
        this.askingPictures = askingPictures;
    }

    public String getConnectName() {
        return connectName;
    }

    public void setConnectName(String connectName) {
        this.connectName = connectName;
    }

    public String getConnectNumber() {
        return connectNumber;
    }

    public void setConnectNumber(String connectNumber) {
        this.connectNumber = connectNumber;
    }

    public String getServiceLog() {
        return serviceLog;
    }

    public void setServiceLog(String serviceLog) {
        this.serviceLog = serviceLog;
    }

    public List getAskingOptions() {
        return askingOptions;
    }

    public void setAskingOptions(List askingOptions) {
        this.askingOptions = askingOptions;
    }

    public List getHistoryServiceLog() {
        return historyServiceLog;
    }

    public void setHistoryServiceLog(List historyServiceLog) {
        this.historyServiceLog = historyServiceLog;
    }

    public List getAskingGoodsInfo() {
        return askingGoodsInfo;
    }

    public void setAskingGoodsInfo(List askingGoodsInfo) {
        this.askingGoodsInfo = askingGoodsInfo;
    }

    public String getGoodsAskingType() {
        return goodsAskingType;
    }

    public void setGoodsAskingType(String goodsAskingType) {
        this.goodsAskingType = goodsAskingType;
    }

    public List<PackageDetail> getPackageDetailList() {
        return packageDetailList;
    }

    public void setPackageDetailList(List<PackageDetail> packageDetailList) {
        this.packageDetailList = packageDetailList;
    }
}
