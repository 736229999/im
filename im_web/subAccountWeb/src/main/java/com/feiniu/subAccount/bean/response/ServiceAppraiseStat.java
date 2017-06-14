package com.feiniu.subAccount.bean.response;

/**
 * Created by wangchen on 2015/2/12.
 */
public class ServiceAppraiseStat {
    private int subAccountID;
    private int appraise;
    private int counter;

    public int getSubAccountID() {
        return subAccountID;
    }

    public void setSubAccountID(int subAccountID) {
        this.subAccountID = subAccountID;
    }

    public int getAppraise() {
        return appraise;
    }

    public void setAppraise(int appraise) {
        this.appraise = appraise;
    }

    public int getCounter() {
        return counter;
    }

    public void setCounter(int counter) {
        this.counter = counter;
    }
}
