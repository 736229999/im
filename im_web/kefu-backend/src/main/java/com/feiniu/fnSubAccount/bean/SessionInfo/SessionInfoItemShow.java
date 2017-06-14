package com.feiniu.fnSubAccount.bean.SessionInfo;

/**
 * Created by wangzhen on 2016/4/8.
 */
public class SessionInfoItemShow {
    private String sessionID;
    private String sessionStartTimeStr;
    private String sessionEndTimeStr;
    private String customerNameStr;
    private String serviceNameStr;
    private String sessionLengthStr;
    private String customerWaitingTimeStr;
    private String mobilePortStr;
    private String pcPortStr;

    public String getSessionID() {
        return sessionID;
    }

    public void setSessionID(String sessionID) {
        this.sessionID = sessionID;
    }

    public String getSessionEndTimeStr() {
        return sessionEndTimeStr;
    }

    public void setSessionEndTimeStr(String sessionEndTimeStr) {
        this.sessionEndTimeStr = sessionEndTimeStr;
    }

    public String getCustomerNameStr() {
        return customerNameStr;
    }

    public void setCustomerNameStr(String customerNameStr) {
        this.customerNameStr = customerNameStr;
    }

    public String getServiceNameStr() {
        return serviceNameStr;
    }

    public void setServiceNameStr(String serviceNameStr) {
        this.serviceNameStr = serviceNameStr;
    }

    public String getSessionLengthStr() {
        return sessionLengthStr;
    }

    public void setSessionLengthStr(String sessionLengthStr) {
        this.sessionLengthStr = sessionLengthStr;
    }

    public String getCustomerWaitingTimeStr() {
        return customerWaitingTimeStr;
    }

    public void setCustomerWaitingTimeStr(String customerWaitingTimeStr) {
        this.customerWaitingTimeStr = customerWaitingTimeStr;
    }

    public String getMobilePortStr() {
        return mobilePortStr;
    }

    public void setMobilePortStr(String mobilePortStr) {
        this.mobilePortStr = mobilePortStr;
    }

    public String getPcPortStr() {
        return pcPortStr;
    }

    public void setPcPortStr(String pcPortStr) {
        this.pcPortStr = pcPortStr;
    }

    public String getSessionStartTimeStr() {
        return sessionStartTimeStr;
    }

    public void setSessionStartTimeStr(String sessionStartTimeStr) {
        this.sessionStartTimeStr = sessionStartTimeStr;
    }
}
