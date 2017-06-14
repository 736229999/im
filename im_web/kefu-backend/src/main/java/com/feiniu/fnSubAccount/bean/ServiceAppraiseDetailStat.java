package com.feiniu.fnSubAccount.bean;

/**
 * Created by wangzhen on 2015/9/24.
 */
public class ServiceAppraiseDetailStat {
    private int subMerchantID;
    private String subMerchantName;
    private int buyerId;
    private long sessionId;
    private String buyerName;
    private String comment;

    private int sqlAppraise;
    private long sqlAppraiseTime;
    private long sqlSessionStartTime;
    private long sqlSessionEndTime;

    private String appraiseStr;
    private String appraiseTimeStr;
    private String sessionStartTimeStr;
    private String sessionEndTimeStr;

    public int getSubMerchantID() {
        return subMerchantID;
    }

    public void setSubMerchantID(int subMerchantID) {
        this.subMerchantID = subMerchantID;
    }

    public String getSubMerchantName() {
        return subMerchantName;
    }

    public void setSubMerchantName(String subMerchantName) {
        this.subMerchantName = subMerchantName;
    }

    public int getBuyerId() {
        return buyerId;
    }

    public void setBuyerId(int buyerId) {
        this.buyerId = buyerId;
    }

    public long getSessionId() {
        return sessionId;
    }

    public void setSessionId(long sessionId) {
        this.sessionId = sessionId;
    }

    public String getBuyerName() {
        return buyerName;
    }

    public void setBuyerName(String buyerName) {
        this.buyerName = buyerName;
    }

    public String getComment() {
        return comment;
    }

    public void setComment(String comment) {
        this.comment = comment;
    }

    public int getSqlAppraise() {
        return sqlAppraise;
    }

    public void setSqlAppraise(int sqlAppraise) {
        this.sqlAppraise = sqlAppraise;
    }

    public long getSqlAppraiseTime() {
        return sqlAppraiseTime;
    }

    public void setSqlAppraiseTime(long sqlAppraiseTime) {
        this.sqlAppraiseTime = sqlAppraiseTime;
    }

    public long getSqlSessionStartTime() {
        return sqlSessionStartTime;
    }

    public void setSqlSessionStartTime(long sqlSessionStartTime) {
        this.sqlSessionStartTime = sqlSessionStartTime;
    }

    public long getSqlSessionEndTime() {
        return sqlSessionEndTime;
    }

    public void setSqlSessionEndTime(long sqlSessionEndTime) {
        this.sqlSessionEndTime = sqlSessionEndTime;
    }

    public String getAppraiseStr() {
        return appraiseStr;
    }

    public void setAppraiseStr(String appraiseStr) {
        this.appraiseStr = appraiseStr;
    }

    public String getAppraiseTimeStr() {
        return appraiseTimeStr;
    }

    public void setAppraiseTimeStr(String appraiseTimeStr) {
        this.appraiseTimeStr = appraiseTimeStr;
    }

    public String getSessionStartTimeStr() {
        return sessionStartTimeStr;
    }

    public void setSessionStartTimeStr(String sessionStartTimeStr) {
        this.sessionStartTimeStr = sessionStartTimeStr;
    }

    public String getSessionEndTimeStr() {
        return sessionEndTimeStr;
    }

    public void setSessionEndTimeStr(String sessionEndTimeStr) {
        this.sessionEndTimeStr = sessionEndTimeStr;
    }
}
