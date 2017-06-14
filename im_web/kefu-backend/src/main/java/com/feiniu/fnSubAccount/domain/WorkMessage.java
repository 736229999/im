package com.feiniu.fnSubAccount.domain;

import javax.persistence.Column;
import javax.persistence.Id;
import javax.persistence.JoinColumn;

/**
 * Created by wangchen on 2015/2/4.
 */

public class WorkMessage {
    @Id
    @Column(name = "sellerName", nullable = false)
    private String sellerName = "";

    @Column(name = "sellerCode", nullable = false)
    @JoinColumn(name = "sellerCode", nullable = false)
    private String sellerCode = "";

    @Column(name = "serviceCount", nullable = false)
    @JoinColumn(name = "serviceCount", nullable = false)
    private String serviceCount;

    @Column(name = "storeName", nullable = false)
    private String storeName = "";

    @Column(name = "onlineTime", nullable = false)
    private String onlineTime = "0.0";

    @Column(name = "customCount", nullable = false)
    private String customCount = "0.0";

    @Column(name = "avgResponseTime", nullable = false)
    private String avgResponseTime = "";

    @Column(name = "in90sResponseRate", nullable = false)
    private String in90sResponseRate = "";

    private int responseTime;

    private int in90sResponseCount = 0;

    private int responseCount = 0;

    private boolean foundInResult = false;
    private String subMerchantIDList = "";

    private String customerIDList = "";

    public String getSellerName() {
        return sellerName;
    }

    public void setSellerName(String sellerName) {
        this.sellerName = sellerName;
    }

    public String getSellerCode() {
        return sellerCode;
    }

    public void setSellerCode(String sellerCode) {
        this.sellerCode = sellerCode;
    }

    public String getServiceCount() {
        return serviceCount;
    }

    public void setServiceCount(String serviceCount) {
        this.serviceCount = serviceCount;
    }

    public String getStoreName() {
        return storeName;
    }

    public void setStoreName(String storeName) {
        this.storeName = storeName;
    }

    public String getOnlineTime() {
        return onlineTime;
    }

    public void setOnlineTime(String onlineTime) {
        this.onlineTime = onlineTime;
    }

    public String getCustomCount() {
        return customCount;
    }

    public void setCustomCount(String customCount) {
        this.customCount = customCount;
    }

    public String getAvgResponseTime() {
        return avgResponseTime;
    }

    public void setAvgResponseTime(String avgResponseTime) {
        this.avgResponseTime = avgResponseTime;
    }

    public String getIn90sResponseRate() {
        return in90sResponseRate;
    }

    public void setIn90sResponseRate(String in90sResponseRate) {
        this.in90sResponseRate = in90sResponseRate;
    }

    public int getResponseTime() {
        return responseTime;
    }

    public void setResponseTime(int responseTime) {
        this.responseTime = responseTime;
    }

    public int getIn90sResponseCount() {
        return in90sResponseCount;
    }

    public void setIn90sResponseCount(int in90sResponseCount) {
        this.in90sResponseCount = in90sResponseCount;
    }

    public int getResponseCount() {
        return responseCount;
    }

    public void setResponseCount(int responseCount) {
        this.responseCount = responseCount;
    }

    public boolean isFoundInResult() {
        return foundInResult;
    }

    public void setFoundInResult(boolean foundInResult) {
        this.foundInResult = foundInResult;
    }

    public String getSubMerchantIDList() {
        return subMerchantIDList;
    }

    public void setSubMerchantIDList(String subMerchantIDList) {
        this.subMerchantIDList = subMerchantIDList;
    }

    public String getCustomerIDList() {
        return customerIDList;
    }

    public void setCustomerIDList(String customerIDList) {
        this.customerIDList = customerIDList;
    }
}
