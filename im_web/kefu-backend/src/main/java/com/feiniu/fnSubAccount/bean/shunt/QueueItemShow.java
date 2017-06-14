package com.feiniu.fnSubAccount.bean.shunt;

/**
 * Created by wangzhen on 2016/4/12.
 */
public class QueueItemShow {
    private String customerName;
    private String customerSource;
    private String customerWaitingTime;

    public String getCustomerName() {
        return customerName;
    }

    public void setCustomerName(String customerName) {
        this.customerName = customerName;
    }

    public String getCustomerSource() {
        return customerSource;
    }

    public void setCustomerSource(String customerSource) {
        this.customerSource = customerSource;
    }

    public String getCustomerWaitingTime() {
        return customerWaitingTime;
    }

    public void setCustomerWaitingTime(String customerWaitingTime) {
        this.customerWaitingTime = customerWaitingTime;
    }
}
