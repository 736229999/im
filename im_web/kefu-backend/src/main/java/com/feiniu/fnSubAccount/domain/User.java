package com.feiniu.fnSubAccount.domain;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * Created by wangchen on 2015/4/19.
 */
@Entity
@Table(name = "im_user")
public class User {
    @Id
    @Column(name = "userid")
    private long userID;

    @Column(name = "username")
    private String userName;

    @Column(name = "mobile")
    private String telphone;

    @Column(name = "email")
    private String email;

    public long getUserID() {
        return userID;
    }

    public void setUserID(long userID) {
        this.userID = userID;
    }

    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

    public String getTelphone() {
        return telphone;
    }

    public void setTelphone(String telphone) {
        this.telphone = telphone;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }
}
