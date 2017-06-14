package com.feiniu.imServer.bean;

/**
 * Created by wangzhen on 2015/11/6.
 */
public class CachedUserInfo {
    private String id;
    private String username;
    private String lanip;
    private String nickname;
    private String eMail;
    private String headURL;
    private String flow;
    private String phone;
    private String userSign;
    private String csUserID;
    private String csTeamID;

    private String receptionLimit;
    private String status;

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getLanip() {
        return lanip;
    }

    public void setLanip(String lanip) {
        this.lanip = lanip;
    }

    public String getNickname() {
        return nickname;
    }

    public void setNickname(String nickname) {
        this.nickname = nickname;
    }

    public String geteMail() {
        return eMail;
    }

    public void seteMail(String eMail) {
        this.eMail = eMail;
    }

    public String getHeadURL() {
        return headURL;
    }

    public void setHeadURL(String headURL) {
        this.headURL = headURL;
    }

    public String getFlow() {
        return flow;
    }

    public void setFlow(String flow) {
        this.flow = flow;
    }

    public String getPhone() {
        return phone;
    }

    public void setPhone(String phone) {
        this.phone = phone;
    }

    public String getUserSign() {
        return userSign;
    }

    public void setUserSign(String userSign) {
        this.userSign = userSign;
    }

    public String getCsUserID() {
        return csUserID;
    }

    public void setCsUserID(String csUserID) {
        this.csUserID = csUserID;
    }

    public String getCsTeamID() {
        return csTeamID;
    }

    public void setCsTeamID(String csTeamID) {
        this.csTeamID = csTeamID;
    }

    public String getReceptionLimit() {
        return receptionLimit;
    }

    public void setReceptionLimit(String receptionLimit) {
        this.receptionLimit = receptionLimit;
    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }

}
