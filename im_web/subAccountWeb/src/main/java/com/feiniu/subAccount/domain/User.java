package com.feiniu.subAccount.domain;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * Created by wangchen on 2015/2/5.
 */
@Entity
@Table(name = "im_user")
public class User {
    @Id
    @Column(name = "userid")
    private int userID;

    @Column(name = "username")
    private String nickName;

    @Column(name = "mobile")
    private String telphone;

    @Column(name = "email")
    private String email;

    public int getUserID() {
        return userID;
    }

    public void setUserID(int userID) {
        this.userID = userID;
    }

    public String getNickName() {
        return nickName;
    }

    public void setNickName(String nickName) {
        this.nickName = nickName;
    }
}
