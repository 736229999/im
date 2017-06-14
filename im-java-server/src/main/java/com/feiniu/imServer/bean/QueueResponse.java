package com.feiniu.imServer.bean;

/**
 * Created by wangzhen on 2016/2/29.
 */
public class QueueResponse {
    int id;              // 用户id
    String username;     // 用户名
    String truename;     // 呢称
    String usersign;    //个性签名
    String headImgUrl;
    int queueLength;
    private boolean isOnline = true;
    private boolean isProxySubAccount; //是否是代理账号


    public boolean isProxySubAccount() {
        return isProxySubAccount;
    }

    public void setIsProxySubAccount(boolean isProxySubAccount) {
        this.isProxySubAccount = isProxySubAccount;
    }

    public boolean isOnline() {
        return isOnline;
    }

    public void setIsOnline(boolean isOnline) {
        this.isOnline = isOnline;
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
