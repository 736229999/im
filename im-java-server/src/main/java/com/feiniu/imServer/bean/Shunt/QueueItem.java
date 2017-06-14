package com.feiniu.imServer.bean.Shunt;

/**
 * Created by wangzhen on 2016/4/7.
 */
public class QueueItem {
    private Long customerId;
    private Integer customerLevel;
    private Integer fromDevice;
    private Integer fromPage;
    private Long waitingTime;

    public Integer getCustomerLevel() {
        return customerLevel;
    }

    public void setCustomerLevel(Integer customerLevel) {
        this.customerLevel = customerLevel;
    }

    public Long getWaitingTime() {
        return waitingTime;
    }

    public void setWaitingTime(Long waitingTime) {
        this.waitingTime = waitingTime;
    }

    public Long getCustomerId() {
        return customerId;
    }

    public void setCustomerId(Long customerId) {
        this.customerId = customerId;
    }

    public Integer getFromDevice() {
        return fromDevice;
    }

    public void setFromDevice(Integer fromDevice) {
        this.fromDevice = fromDevice;
    }

    public Integer getFromPage() {
        return fromPage;
    }

    public void setFromPage(Integer fromPage) {
        this.fromPage = fromPage;
    }
}
