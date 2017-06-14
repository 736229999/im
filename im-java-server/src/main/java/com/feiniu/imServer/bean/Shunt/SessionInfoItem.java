package com.feiniu.imServer.bean.Shunt;

/**
 * Created by wangzhen on 2016/4/7.
 */
public class SessionInfoItem {
    private Integer eventType;
    private Long groupId;
    private Long customerId;
    private Long serviceId;
    private Long sessionStartTime;
    private Long sessionEndTime;
    private Long waitingTime;
    private Integer fromDevice;
    private Integer fromPage;
    private Integer userLevel;
    private Long transId;

    public Long getTransId() {
        return transId;
    }

    public void setTransId(Long transId) {
        this.transId = transId;
    }

    public Integer getUserLevel() {
        return userLevel;
    }

    public void setUserLevel(Integer userLevel) {
        this.userLevel = userLevel;
    }

    public Integer getEventType() {
        return eventType;
    }

    public void setEventType(Integer eventType) {
        this.eventType = eventType;
    }

    public Long getGroupId() {
        return groupId;
    }

    public void setGroupId(Long groupId) {
        this.groupId = groupId;
    }

    public Long getCustomerId() {
        return customerId;
    }

    public void setCustomerId(Long customerId) {
        this.customerId = customerId;
    }

    public Long getServiceId() {
        return serviceId;
    }

    public void setServiceId(Long serviceId) {
        this.serviceId = serviceId;
    }

    public Long getSessionStartTime() {
        return sessionStartTime;
    }

    public void setSessionStartTime(Long sessionStartTime) {
        this.sessionStartTime = sessionStartTime;
    }

    public Long getSessionEndTime() {
        return sessionEndTime;
    }

    public void setSessionEndTime(Long sessionEndTime) {
        this.sessionEndTime = sessionEndTime;
    }

    public Long getWaitingTime() {
        return waitingTime;
    }

    public void setWaitingTime(Long waitingTime) {
        this.waitingTime = waitingTime;
    }

    public Integer getFromDevice() {
        return fromDevice;
    }

    public void setFromDevice(Integer fromDevice) {
        this.fromDevice = fromDevice;
    }

    public Integer getFromPage() {
        return fromPage;
    }

    public void setFromPage(Integer fromPage) {
        this.fromPage = fromPage;
    }
}
