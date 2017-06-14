package com.feiniu.fnSubAccount.bean.request;

/**
 * Author: morningking
 * Date: 2016/4/11 8:52
 * Contact: 243717042@qq.com
 */
public class GetHistoryMessageListWithSessionRequest {
    private long customerID;
    private long serviceID;
    private int startChatTime;

    public long getCustomerID() {
        return customerID;
    }

    public void setCustomerID(long customerID) {
        this.customerID = customerID;
    }

    public long getServiceID() {
        return serviceID;
    }

    public void setServiceID(long serviceID) {
        this.serviceID = serviceID;
    }

    public int getStartChatTime() {
        return startChatTime;
    }

    public void setStartChatTime(int startChatTime) {
        this.startChatTime = startChatTime;
    }
}
