package com.feiniu.fnSubAccount.bean;

/**
 * Author: morningking
 * Date: 2016/4/7 13:50
 * Contact: 243717042@qq.com
 */
public class ChatSession {
    private int id;
    private int customerID;
    private String customerName;
    private int serviceID;
    private String serviceName;
    private int fromDevice;
    private int fromPage;
    private int queuedTime;
    private int startChatTime;
    private int customerLevel;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getCustomerID() {
        return customerID;
    }

    public void setCustomerID(int customerID) {
        this.customerID = customerID;
    }

    public String getCustomerName() {
        return customerName;
    }

    public void setCustomerName(String customerName) {
        this.customerName = customerName;
    }

    public int getServiceID() {
        return serviceID;
    }

    public void setServiceID(int serviceID) {
        this.serviceID = serviceID;
    }

    public String getServiceName() {
        return serviceName;
    }

    public void setServiceName(String serviceName) {
        this.serviceName = serviceName;
    }

    public int getFromDevice() {
        return fromDevice;
    }

    public void setFromDevice(int fromDevice) {
        this.fromDevice = fromDevice;
    }

    public int getFromPage() {
        return fromPage;
    }

    public void setFromPage(int fromPage) {
        this.fromPage = fromPage;
    }

    public int getQueuedTime() {
        return queuedTime;
    }

    public void setQueuedTime(int queuedTime) {
        this.queuedTime = queuedTime;
    }

    public int getStartChatTime() {
        return startChatTime;
    }

    public void setStartChatTime(int startChatTime) {
        this.startChatTime = startChatTime;
    }

    public int getCustomerLevel() {
        return customerLevel;
    }

    public void setCustomerLevel(int customerLevel) {
        this.customerLevel = customerLevel;
    }
}
