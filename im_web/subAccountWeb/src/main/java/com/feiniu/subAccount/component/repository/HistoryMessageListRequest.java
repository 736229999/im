package com.feiniu.subAccount.component.repository;

/**
 * Created by wangchen on 2015/2/4.
 */
public class HistoryMessageListRequest {
    private String subMerchantIDList;
    private String customerName;
    private Integer startTime;
    private Integer endTime;
    private int pageSize;
    private int currentPageNo;

    public String getSubMerchantIDList() {
        return subMerchantIDList;
    }

    public void setSubMerchantIDList(String subMerchantIDList) {
        this.subMerchantIDList = subMerchantIDList;
    }

    public String getCustomerName() {
        return customerName;
    }

    public void setCustomerName(String customerName) {
        this.customerName = customerName;
    }

    public int getPageSize() {
        return pageSize;
    }

    public void setPageSize(int pageSize) {
        this.pageSize = pageSize;
    }

    public int getCurrentPageNo() {
        return currentPageNo;
    }

    public void setCurrentPageNo(int currentPageNo) {
        this.currentPageNo = currentPageNo;
    }

    public Integer getStartTime() {
        return startTime;
    }

    public void setStartTime(Integer startTime) {
        this.startTime = startTime;
    }

    public Integer getEndTime() {
        return endTime;
    }

    public void setEndTime(Integer endTime) {
        this.endTime = endTime;
    }
}
