package com.feiniu.fnSubAccount.bean.request;


/**
 * Created by wangzhen on 2015/7/27.
 */
public class AttendanceRequest {
    private String subMerchantIDList;
    private String merchantID;
    private String subMerchantID;
    private String directorID;
    private long startTime;
    private long endTime;
    private int searchType;
    private int pageSize;
    private int currentPageNo;

    public String getDirectorID() {
        return directorID;
    }

    public void setDirectorID(String directorID) {
        this.directorID = directorID;
    }

    public String getSubMerchantID() {
        return subMerchantID;
    }

    public void setSubMerchantID(String subMerchantID) {
        this.subMerchantID = subMerchantID;
    }

    public String getSubMerchantIDList() {
        return subMerchantIDList;
    }

    public void setSubMerchantIDList(String subMerchantIDList) {
        this.subMerchantIDList = subMerchantIDList;
    }

    public String getMerchantID() {
        return merchantID;
    }

    public void setMerchantID(String merchantID) {
        this.merchantID = merchantID;
    }

    public long getStartTime() {
        return startTime;
    }

    public void setStartTime(long startTime) {
        this.startTime = startTime;
    }

    public long getEndTime() {
        return endTime;
    }

    public void setEndTime(long endTime) {
        this.endTime = endTime;
    }

    public int getSearchType() {
        return searchType;
    }

    public void setSearchType(int searchType) {
        this.searchType = searchType;
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
