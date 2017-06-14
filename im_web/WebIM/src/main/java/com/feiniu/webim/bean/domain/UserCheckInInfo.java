package com.feiniu.webim.bean.domain;

/**
 * Created by wangchen on 2015/4/23.
 */
public class UserCheckInInfo {
    private String guid;
    private String userName;
    private String email;
    private String telephone;

    public String getGuid() {
        return guid;
    }

    public void setGuid(String guid) {
        this.guid = guid;
    }

    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public String getTelephone() {
        return telephone;
    }

    public void setTelephone(String telephone) {
        this.telephone = telephone;
    }
}
