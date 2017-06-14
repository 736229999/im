package com.feiniu.fnSubAccount.bean.response;

/**
 * Created by wangzhen on 2015/8/18.
 */
public class WorkMessageResponse {
    long merchantID;
    String merchantName;
    String shopName;
    String merchantStatus;
    String mainCategoryName;


    float sqlSubMerchantOnlineCount;
    float sqlSumLandingTime;
    float sqlAskingCount;
    float sqlReplyCount;
    float sqlAvgResponseDuration;
    float sqlResponseIn90sRate;
    float sqlAskingTimes;
    float sqlReplyTimes;

    //show infos
    String merchantIDStr;
    String subMerchantOnlineCount;
    String sumLandingTime;
    String askingCount;
    String replyCount;
    String avgResponseDuration;
    String responseIn90sRate;
    String askingTimes;
    String replyTimes;
    String replyFrequency;



    public float getSqlAskingTimes() {
        return sqlAskingTimes;
    }

    public void setSqlAskingTimes(float sqlAskingTimes) {
        this.sqlAskingTimes = sqlAskingTimes;
    }

    public float getSqlReplyTimes() {
        return sqlReplyTimes;
    }

    public void setSqlReplyTimes(float sqlReplyTimes) {
        this.sqlReplyTimes = sqlReplyTimes;
    }

    public String getAskingTimes() {
        return askingTimes;
    }

    public void setAskingTimes(String askingTimes) {
        this.askingTimes = askingTimes;
    }

    public String getReplyTimes() {
        return replyTimes;
    }

    public void setReplyTimes(String replyTimes) {
        this.replyTimes = replyTimes;
    }

    public String getReplyFrequency() {
        return replyFrequency;
    }

    public void setReplyFrequency(String replyFrequency) {
        this.replyFrequency = replyFrequency;
    }


    public long getMerchantID() {
        return merchantID;
    }

    public void setMerchantID(long merchantID) {
        this.merchantID = merchantID;
    }

    public String getMerchantName() {
        return merchantName;
    }

    public void setMerchantName(String merchantName) {
        this.merchantName = merchantName;
    }

    public String getShopName() {
        return shopName;
    }

    public void setShopName(String shopName) {
        this.shopName = shopName;
    }

    public String getMerchantStatus() {
        return merchantStatus;
    }

    public void setMerchantStatus(String merchantStatus) {
        this.merchantStatus = merchantStatus;
    }

    public float getSqlSubMerchantOnlineCount() {
        return sqlSubMerchantOnlineCount;
    }

    public void setSqlSubMerchantOnlineCount(float sqlSubMerchantOnlineCount) {
        this.sqlSubMerchantOnlineCount = sqlSubMerchantOnlineCount;
    }

    public float getSqlSumLandingTime() {
        return sqlSumLandingTime;
    }

    public void setSqlSumLandingTime(float sqlSumLandingTime) {
        this.sqlSumLandingTime = sqlSumLandingTime;
    }

    public float getSqlAskingCount() {
        return sqlAskingCount;
    }

    public void setSqlAskingCount(float sqlAskingCount) {
        this.sqlAskingCount = sqlAskingCount;
    }

    public float getSqlReplyCount() {
        return sqlReplyCount;
    }

    public void setSqlReplyCount(float sqlReplyCount) {
        this.sqlReplyCount = sqlReplyCount;
    }

    public float getSqlAvgResponseDuration() {
        return sqlAvgResponseDuration;
    }

    public void setSqlAvgResponseDuration(float sqlAvgResponseDuration) {
        this.sqlAvgResponseDuration = sqlAvgResponseDuration;
    }

    public float getSqlResponseIn90sRate() {
        return sqlResponseIn90sRate;
    }

    public void setSqlResponseIn90sRate(float sqlResponseIn90sRate) {
        this.sqlResponseIn90sRate = sqlResponseIn90sRate;
    }

    public String getMerchantIDStr() {
        return merchantIDStr;
    }

    public void setMerchantIDStr(String merchantIDStr) {
        this.merchantIDStr = merchantIDStr;
    }

    public String getSubMerchantOnlineCount() {
        return subMerchantOnlineCount;
    }

    public void setSubMerchantOnlineCount(String subMerchantOnlineCount) {
        this.subMerchantOnlineCount = subMerchantOnlineCount;
    }

    public String getSumLandingTime() {
        return sumLandingTime;
    }

    public void setSumLandingTime(String sumLandingTime) {
        this.sumLandingTime = sumLandingTime;
    }

    public String getAskingCount() {
        return askingCount;
    }

    public void setAskingCount(String askingCount) {
        this.askingCount = askingCount;
    }

    public String getReplyCount() {
        return replyCount;
    }

    public void setReplyCount(String replyCount) {
        this.replyCount = replyCount;
    }

    public String getAvgResponseDuration() {
        return avgResponseDuration;
    }

    public void setAvgResponseDuration(String avgResponseDuration) {
        this.avgResponseDuration = avgResponseDuration;
    }

    public String getResponseIn90sRate() {
        return responseIn90sRate;
    }

    public void setResponseIn90sRate(String responseIn90sRate) {
        this.responseIn90sRate = responseIn90sRate;
    }

    public String getMainCategoryName() {
        return mainCategoryName;
    }

    public void setMainCategoryName(String mainCategoryName) {
        this.mainCategoryName = mainCategoryName;
    }
}
