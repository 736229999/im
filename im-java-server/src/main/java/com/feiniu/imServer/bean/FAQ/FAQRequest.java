package com.feiniu.imServer.bean.FAQ;

/**
 * Author: morningking
 * Date: 2015/9/28
 * Contact: 243717042@qq.com
 */
public class FAQRequest {
    private Long startTime;
    private Long endTime;
    private String keyWord;
    private Boolean isShow = false;
    private Boolean isSortByViewCountAsc;

    private int currentPageNo = 1;
    private int pageSize = 10;
    private boolean isFetchAll = false;

    public Long getStartTime() {
        return startTime;
    }

    public void setStartTime(Long startTime) {
        this.startTime = startTime;
    }

    public Long getEndTime() {
        return endTime;
    }

    public void setEndTime(Long endTime) {
        this.endTime = endTime;
    }

    public Boolean getIsShow() {
        return isShow;
    }

    public void setIsShow(Boolean isShow) {
        this.isShow = isShow;
    }

    public void setIsShow(boolean isShow) {
        this.isShow = isShow;
    }

    public String getKeyWord() {
        return keyWord;
    }

    public void setKeyWord(String keyWord) {
        this.keyWord = keyWord;
    }

    public boolean isShow() {
        return isShow;
    }

    public Boolean getIsSortByViewCountAsc() {
        return isSortByViewCountAsc;
    }

    public void setIsSortByViewCountAsc(Boolean isSortByViewCountAsc) {
        this.isSortByViewCountAsc = isSortByViewCountAsc;
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

    public boolean isFetchAll() {
        return isFetchAll;
    }

    public void setIsFetchAll(boolean isFetchAll) {
        this.isFetchAll = isFetchAll;
    }
}
