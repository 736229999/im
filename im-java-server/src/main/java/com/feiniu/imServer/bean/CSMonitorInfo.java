package com.feiniu.imServer.bean;

/**
 * Created by wanbingwen on 2015/10/28.
 */
public class CSMonitorInfo implements Comparable<CSMonitorInfo> {
    private Long cServiceId;
    private Integer receptionCount;
    private Integer consultCount;
    private CachedUserInfo cachedUserInfo;

    public Long getcServiceId() {
        return cServiceId;
    }

    public void setcServiceId(Long cServiceId) {
        this.cServiceId = cServiceId;
    }

    public Integer getReceptionCount() {
        return receptionCount;
    }

    public void setReceptionCount(Integer receptionCount) {
        this.receptionCount = receptionCount;
    }

    public Integer getConsultCount() {
        return consultCount;
    }

    public void setConsultCount(Integer consultCount) {
        this.consultCount = consultCount;
    }

    public CachedUserInfo getCachedUserInfo() {
        return cachedUserInfo;
    }

    public void setCachedUserInfo(CachedUserInfo cachedUserInfo) {
        this.cachedUserInfo = cachedUserInfo;
    }

    @Override
    public int compareTo(CSMonitorInfo arg0) {
        return this.getcServiceId().compareTo(arg0.getcServiceId());
    }
}
