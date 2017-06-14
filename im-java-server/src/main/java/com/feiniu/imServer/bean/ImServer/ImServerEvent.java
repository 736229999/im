package com.feiniu.imServer.bean.ImServer;

/**
 * Created by wangzhen on 2016/1/4.
 */
public class ImServerEvent {
    private Integer eventType;
    private Long serviceId;
    private Long customerId;
    private Long groupId;
    private Long requestTime;
    private Integer userLevel;
    private Long toServiceId;
    private Long toServiceGroup;
    private String logInfo;
    private Integer fromDevice;
    private Integer fromPage;
    private Long loginIP;

    public Long getLoginIP() {
        return loginIP;
    }

    public void setLoginIP(Long loginIP) {
        this.loginIP = loginIP;
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

    public Integer getEventType() {
        return eventType;
    }

    public void setEventType(Integer eventType) {
        this.eventType = eventType;
    }

    public Long getServiceId() {
        return serviceId;
    }

    public void setServiceId(Long serviceId) {
        this.serviceId = serviceId;
    }

    public Long getCustomerId() {
        return customerId;
    }

    public void setCustomerId(Long customerId) {
        this.customerId = customerId;
    }

    public Long getGroupId() {
        return groupId;
    }

    public void setGroupId(Long groupId) {
        this.groupId = groupId;
    }

    public Long getRequestTime() {
        return requestTime;
    }

    public void setRequestTime(Long requestTime) {
        this.requestTime = requestTime;
    }

    public Integer getUserLevel() {
        return userLevel;
    }

    public void setUserLevel(Integer userLevel) {
        this.userLevel = userLevel;
    }

    public Long getToServiceId() {
        return toServiceId;
    }

    public void setToServiceId(Long toServiceId) {
        this.toServiceId = toServiceId;
    }

    public Long getToServiceGroup() {
        return toServiceGroup;
    }

    public void setToServiceGroup(Long toServiceGroup) {
        this.toServiceGroup = toServiceGroup;
    }

    public String getLogInfo() {
        return logInfo;
    }

    public void setLogInfo(String logInfo) {
        this.logInfo = logInfo;
    }
}
