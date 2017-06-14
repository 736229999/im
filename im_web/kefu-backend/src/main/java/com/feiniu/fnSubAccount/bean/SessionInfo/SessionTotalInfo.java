package com.feiniu.fnSubAccount.bean.SessionInfo;

/**
 * Created by wangzhen on 2016/4/11.
 */
public class SessionTotalInfo {
    private Integer recodeSize;
    private Integer avgLen;
    private Integer avgWaitingTime;


    public Integer getRecodeSize() {
        return recodeSize;
    }

    public void setRecodeSize(Integer recodeSize) {
        this.recodeSize = recodeSize;
    }

    public Integer getAvgLen() {
        return avgLen;
    }

    public void setAvgLen(Integer avgLen) {
        this.avgLen = avgLen;
    }

    public Integer getAvgWaitingTime() {
        return avgWaitingTime;
    }

    public void setAvgWaitingTime(Integer avgWaitingTime) {
        this.avgWaitingTime = avgWaitingTime;
    }
}
