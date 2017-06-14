package com.feiniu.fnSubAccount.bean.SessionInfo;

/**
 * Created by wangzhen on 2016/4/6.
 */
public class SessionInfoItem {
    private String sessionID;
    private String customerId;
    private String serviceId;
    private Integer customerLevel;
    private Integer sessionStartTimeSql;
    private Integer sessionEndTimeSql;
    private Integer sessionLengthSql;
    private Integer customerWaitingTimeSql;
    private Integer fromDeviceSql;
    private Integer fromPageSql;

    public Integer getCustomerLevel() {
        return customerLevel;
    }

    public void setCustomerLevel(Integer customerLevel) {
        this.customerLevel = customerLevel;
    }

    public String getSessionID() {
        return sessionID;
    }

    public void setSessionID(String sessionID) {
        this.sessionID = sessionID;
    }

    public String getCustomerId() {
        return customerId;
    }

    public void setCustomerId(String customerId) {
        this.customerId = customerId;
    }

    public String getServiceId() {
        return serviceId;
    }

    public void setServiceId(String serviceId) {
        this.serviceId = serviceId;
    }

    public Integer getSessionStartTimeSql() {
        return sessionStartTimeSql;
    }

    public void setSessionStartTimeSql(Integer sessionStartTimeSql) {
        this.sessionStartTimeSql = sessionStartTimeSql;
    }

    public Integer getSessionEndTimeSql() {
        return sessionEndTimeSql;
    }

    public void setSessionEndTimeSql(Integer sessionEndTimeSql) {
        this.sessionEndTimeSql = sessionEndTimeSql;
    }

    public Integer getSessionLengthSql() {
        return sessionLengthSql;
    }

    public void setSessionLengthSql(Integer sessionLengthSql) {
        this.sessionLengthSql = sessionLengthSql;
    }

    public Integer getCustomerWaitingTimeSql() {
        return customerWaitingTimeSql;
    }

    public void setCustomerWaitingTimeSql(Integer customerWaitingTimeSql) {
        this.customerWaitingTimeSql = customerWaitingTimeSql;
    }

    public Integer getFromDeviceSql() {
        return fromDeviceSql;
    }

    public void setFromDeviceSql(Integer fromDeviceSql) {
        this.fromDeviceSql = fromDeviceSql;
    }

    public Integer getFromPageSql() {
        return fromPageSql;
    }

    public void setFromPageSql(Integer fromPageSql) {
        this.fromPageSql = fromPageSql;
    }
}
