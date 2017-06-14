package com.feiniu.webim.bean.domain;

import com.feiniu.webim.util.web.UserAgent;

/**
 * Created by wangchen on 2015/5/14.
 */
public class LinkInfo {
    private UserAgent userAgent;
    private String ipAddress;
    private String macAddress;

    public UserAgent getUserAgent() {
        return userAgent;
    }

    public void setUserAgent(UserAgent userAgent) {
        this.userAgent = userAgent;
    }

    public String getIpAddress() {
        return ipAddress;
    }

    public void setIpAddress(String ipAddress) {
        this.ipAddress = ipAddress;
    }

    public String getMacAddress() {
        return macAddress;
    }

    public void setMacAddress(String macAddress) {
        this.macAddress = macAddress;
    }
}
