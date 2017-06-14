package com.feiniu.fnSubAccount.domain;

import java.math.BigDecimal;

/**
 * Created by zhangmin on 2016/4/12.
 */
public class MerchantLogInOutInfo {
    private BigDecimal userId;
    private String userName;
    private BigDecimal logInOutTime;
    private Integer logInOutType;

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

    public BigDecimal getLogInOutTime() {
        return logInOutTime;
    }

    public void setLogInOutTime(BigDecimal logInOutTime) {
        this.logInOutTime = logInOutTime;
    }

    public Integer getLogInOutType() {
        return logInOutType;
    }

    public void setLogInOutType(Integer logInOutType) {
        this.logInOutType = logInOutType;
    }
}
