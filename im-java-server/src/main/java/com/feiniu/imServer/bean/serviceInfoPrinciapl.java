package com.feiniu.imServer.bean;

/**
 * Created by wangzhen on 2016/1/29.
 */
public class serviceInfoPrinciapl {
    Long id;
    String email;
    String userName;
    String mobile;
    String nickName;
    String usersSign;
    String faceUrl;


    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

    public String getMobile() {
        return mobile;
    }

    public void setMobile(String mobile) {
        this.mobile = mobile;
    }

    public String getNickName() {
        return nickName;
    }

    public void setNickName(String nickName) {
        this.nickName = nickName;
    }

    public String getUsersSign() {
        return usersSign;
    }

    public void setUsersSign(String usersSign) {
        this.usersSign = usersSign;
    }

    public String getFaceUrl() {
        return faceUrl;
    }

    public void setFaceUrl(String faceUrl) {
        this.faceUrl = faceUrl;
    }
}
