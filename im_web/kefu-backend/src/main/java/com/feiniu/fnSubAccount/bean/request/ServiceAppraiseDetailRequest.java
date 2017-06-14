package com.feiniu.fnSubAccount.bean.request;

/**
 * Created by wangzhen on 2015/9/24.
 */
public class ServiceAppraiseDetailRequest {
    private long startTime;
    private long endTime;
    private String subMerchantID;
    private int currentPageNo;
    private int pageSize;

    private int historyChatPageSize;
    private int historyChatCurrentPageNo;
    private long historySessionId;
    private int searchType;

    public int getSearchType() {
        return searchType;
    }

    public void setSearchType(int searchType) {
        this.searchType = searchType;
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

    public String getSubMerchantID() {
        return subMerchantID;
    }

    public void setSubMerchantID(String subMerchantID) {
        this.subMerchantID = subMerchantID;
    }

    public int getCurrentPageNo() {
        return currentPageNo;
    }

    public void setCurrentPageNo(int currentPageNo) {
        this.currentPageNo = currentPageNo;
    }

    public int getPageSize() {
        return pageSize;
    }

    public void setPageSize(int pageSize) {
        this.pageSize = pageSize;
    }

    public int getHistoryChatPageSize() {
        return historyChatPageSize;
    }

    public void setHistoryChatPageSize(int historyChatPageSize) {
        this.historyChatPageSize = historyChatPageSize;
    }

    public int getHistoryChatCurrentPageNo() {
        return historyChatCurrentPageNo;
    }

    public void setHistoryChatCurrentPageNo(int historyChatCurrentPageNo) {
        this.historyChatCurrentPageNo = historyChatCurrentPageNo;
    }

    public long getHistorySessionId() {
        return historySessionId;
    }

    public void setHistorySessionId(long historySessionId) {
        this.historySessionId = historySessionId;
    }
}
