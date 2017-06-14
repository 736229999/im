package com.feiniu.fnSubAccount.domain;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * Created by wangchen on 2015/2/5.
 */
@Entity
@Table(name = "im_userinfo")
public class UserInfo {
    @Id
    @Column(name = "userid")
    private int userID;

    @Column(name = "nickname")
    private String nickName;

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
