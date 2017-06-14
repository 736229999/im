package com.feiniu.fnSubAccount.bean.request;

/**
 * Created by wangchen on 2015/2/4.
 */
public class HistoryMessageListRequest {
    private String customerID;
    private Integer startTime;
    private Integer endTime;
    private Integer isCustomerService;
    private String dirctorID;
    private String subMerchantNamePart;
    private String subMerchantIDList;
    private String storeName;
    private int pageSize;
    private int currentPageNo;

    public String getStoreName() {
        return storeName;
    }

    public void setStoreName(String storeName) {
        this.storeName = storeName;
    }

    public String getCustomerID() {
        return customerID;
    }

    public void setCustomerID(String customerID) {
        this.customerID = customerID;
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

    public Integer getIsCustomerService() {
        return isCustomerService;
    }

    public void setIsCustomerService(Integer isCustomerService) {
        this.isCustomerService = isCustomerService;
    }

    public String getDirctorID() {
        return dirctorID;
    }

    public void setDirctorID(String dirctorID) {
        this.dirctorID = dirctorID;
    }

    public String getSubMerchantNamePart() {
        return subMerchantNamePart;
    }

    public void setSubMerchantNamePart(String subMerchantNamePart) {
        this.subMerchantNamePart = subMerchantNamePart;
    }

    public String getSubMerchantIDList() {
        return subMerchantIDList;
    }

    public void setSubMerchantIDList(String subMerchantIDList) {
        this.subMerchantIDList = subMerchantIDList;
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
}
