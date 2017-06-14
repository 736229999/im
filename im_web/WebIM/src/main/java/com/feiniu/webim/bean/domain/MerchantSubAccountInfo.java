package com.feiniu.webim.bean.domain;

/**
 * Created by wangchen on 2015/1/6.
 */
public class MerchantSubAccountInfo {
    private int id;              // 用户id
    private String username;     // 用户名
    private String truename;     // 呢称
    private String usersign;    //个性签名
    private boolean isOnline = true;
    private boolean isProxySubAccount; //是否是代理账号
    String headImgUrl;
    private int queueLength;

    public MerchantSubAccountInfo() {
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getTruename() {
        return truename;
    }

    public void setTruename(String truename) {
        this.truename = truename;
    }

    public boolean isProxySubAccount() {
        return isProxySubAccount;
    }

    public void setProxySubAccount(boolean isProxySubAccount) {
        this.isProxySubAccount = isProxySubAccount;
    }

    public boolean isOnline() {
        return isOnline;
    }

    public void setOnline(boolean isOnline) {
        this.isOnline = isOnline;
    }

    public String getUsersign() {
        return usersign;
    }

    public void setUsersign(String usersign) {
        this.usersign = usersign;
    }

    public String getHeadImgUrl() {
        return headImgUrl;
    }

    public void setHeadImgUrl(String headImgUrl) {
        this.headImgUrl = headImgUrl;
    }

    public int getQueueLength() {
        return queueLength;
    }

    public void setQueueLength(int queueLength) {
        this.queueLength = queueLength;
    }
}
