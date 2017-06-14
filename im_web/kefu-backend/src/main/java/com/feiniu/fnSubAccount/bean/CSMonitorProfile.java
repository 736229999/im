package com.feiniu.fnSubAccount.bean;

/**
 * Created by wanbingwen on 2015/10/29.
 */
public class CSMonitorProfile {
    private Integer CSCount = 0;    //账号总数
    private Integer onlineCSCount = 0; //在线客服
    private Integer arriveRecpLimitCount = 0;;//已达接待上限
    private Integer offlineCSCount = 0;  //离线客服
    private Integer consultCount= 0;//客人咨询量
    private Integer CSRecpCount= 0; //客服接待量
    private Integer queuingCount= 0; //正在排队人数
    private String lastrefreshTime; //最后修改时间

    public Integer getCSCount() {
        return CSCount;
    }

    public void setCSCount(Integer CSCount) {
        this.CSCount = CSCount;
    }

    public Integer getOnlineCSCount() {
        return onlineCSCount;
    }

    public void setOnlineCSCount(Integer onlineCSCount) {
        this.onlineCSCount = onlineCSCount;
    }

    public Integer getArriveRecpLimitCount() {
        return arriveRecpLimitCount;
    }

    public void setArriveRecpLimitCount(Integer arriveRecpLimitCount) {
        this.arriveRecpLimitCount = arriveRecpLimitCount;
    }

    public Integer getOfflineCSCount() {
        return offlineCSCount;
    }

    public void setOfflineCSCount(Integer offlineCSCount) {
        this.offlineCSCount = offlineCSCount;
    }

    public Integer getConsultCount() {
        return consultCount;
    }

    public void setConsultCount(Integer consultCount) {
        this.consultCount = consultCount;
    }

    public Integer getCSRecpCount() {
        return CSRecpCount;
    }

    public void setCSRecpCount(Integer CSRecpCount) {
        this.CSRecpCount = CSRecpCount;
    }

    public Integer getQueuingCount() {
        return queuingCount;
    }

    public void setQueuingCount(Integer queuingCount) {
        this.queuingCount = queuingCount;
    }

    public String getLastrefreshTime() {
        return lastrefreshTime;
    }

    public void setLastrefreshTime(String lastrefreshTime) {
        this.lastrefreshTime = lastrefreshTime;
    }
}
