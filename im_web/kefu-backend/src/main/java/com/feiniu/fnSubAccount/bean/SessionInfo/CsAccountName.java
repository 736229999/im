package com.feiniu.fnSubAccount.bean.SessionInfo;

import java.math.BigDecimal;

/**
 * Created by wangzhen on 2016/4/21.
 */
public class CsAccountName {
    private BigDecimal userId;
    private String userName;
    private String realName;

    public BigDecimal getUserId() {
        return userId;
    }

    public void setUserId(BigDecimal userId) {
        this.userId = userId;
    }

    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

    public String getRealName() {
        return realName;
    }

    public void setRealName(String realName) {
        this.realName = realName;
    }
}
