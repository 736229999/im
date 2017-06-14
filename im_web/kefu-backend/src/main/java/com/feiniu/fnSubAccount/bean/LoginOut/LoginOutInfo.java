package com.feiniu.fnSubAccount.bean.LoginOut;

/**
 * Created by wangzhen on 2016/4/18.
 */
public class LoginOutInfo {
    private String userName;
    private String logInOutTime;
    private Integer logInOutType;

    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

    public String getLogInOutTime() {
        return logInOutTime;
    }

    public void setLogInOutTime(String logInOutTime) {
        this.logInOutTime = logInOutTime;
    }

    public Integer getLogInOutType() {
        return logInOutType;
    }

    public void setLogInOutType(Integer logInOutType) {
        this.logInOutType = logInOutType;
    }
}
