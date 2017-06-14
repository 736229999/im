package com.feiniu.fnSubAccount.bean.response;

/**
 * Created by wangzhen on 2015/11/23.
 */
public class CsModifyPasswordRequest {
    Long moumouId;
    String password;

    public Long getMoumouId() {
        return moumouId;
    }

    public void setMoumouId(Long moumouId) {
        this.moumouId = moumouId;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }
}
