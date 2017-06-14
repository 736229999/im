package com.feiniu.imServer.bean;

/**
 * Created by chenqing on 2016/3/22.
 */
public class CSInfo {

    private Long userId ;
    private String userName ;
    private Long status ;
    private Long receptionLimit ;
    private Long permission;

    public Long getUserId() {
        return userId;
    }

    public void setUserId(Long userId) {
        this.userId = userId;
    }

    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

    public Long getStatus() {
        return status;
    }

    public void setStatus(Long status) {
        this.status = status;
    }

    public Long getReceptionLimit() {
        return receptionLimit;
    }

    public void setReceptionLimit(Long receptionLimit) {
        this.receptionLimit = receptionLimit;
    }

    public Long getPermission() {
        return permission;
    }

    public void setPermission(Long permission) {
        this.permission = permission;
    }
}
