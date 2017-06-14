package com.feiniu.webim.bean.domain;

import java.util.List;

/**
 * Created by wangchen on 2015/3/3.
 */
public class UserRelatedOrderInfo {
    private long merchantID;
    private int totalTradeNum = 0;
    private double totalConsumeAmount = 0.0;
    private List<String> recentlyTradePicUrlList;
    private List<String> recentlyTradeGoodUrlList;
    private String allOrderUrl;

    public long getMerchantID() {
        return merchantID;
    }

    public void setMerchantID(long merchantID) {
        this.merchantID = merchantID;
    }

    public int getTotalTradeNum() {
        return totalTradeNum;
    }

    public void setTotalTradeNum(int totalTradeNum) {
        this.totalTradeNum = totalTradeNum;
    }

    public double getTotalConsumeAmount() {
        return totalConsumeAmount;
    }

    public void setTotalConsumeAmount(double totalConsumeAmount) {
        this.totalConsumeAmount = totalConsumeAmount;
    }

    public List<String> getRecentlyTradeGoodUrlList() {
        return recentlyTradeGoodUrlList;
    }

    public void setRecentlyTradeGoodUrlList(List<String> recentlyTradeGoodUrlList) {
        this.recentlyTradeGoodUrlList = recentlyTradeGoodUrlList;
    }

    public List<String> getRecentlyTradePicUrlList() {
        return recentlyTradePicUrlList;
    }

    public void setRecentlyTradePicUrlList(List<String> recentlyTradePicUrlList) {
        this.recentlyTradePicUrlList = recentlyTradePicUrlList;
    }

    public String getAllOrderUrl() {
        return allOrderUrl;
    }

    public void setAllOrderUrl(String allOrderUrl) {
        this.allOrderUrl = allOrderUrl;
    }
}
