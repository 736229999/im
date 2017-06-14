package com.feiniu.fnSubAccount.bean.request;

/**
 * Author: morningking
 * Date: 2016/4/14 14:02
 * Contact: 243717042@qq.com
 */
public class MonitorInterveneSessionRequest {
    private int sessionID;
    private int csId;
    private int buyerId;
    private int masterId;

    public int getSessionID() {
        return sessionID;
    }

    public void setSessionID(int sessionID) {
        this.sessionID = sessionID;
    }

    public int getCsId() {
        return csId;
    }

    public void setCsId(int csId) {
        this.csId = csId;
    }

    public int getBuyerId() {
        return buyerId;
    }

    public void setBuyerId(int buyerId) {
        this.buyerId = buyerId;
    }

    public int getMasterId() {
        return masterId;
    }

    public void setMasterId(int masterId) {
        this.masterId = masterId;
    }
}
