package com.feiniu.subAccount.util;

import java.util.List;

/**
 * Created by wangchen on 2015/2/4.
 */
public class Pager<T> {
    private List<T> resultsForCurrentPage;
    private int currentPageNo;
    private int pageSize;
    private long totalRecordSize;
    private long totalPageNum;

    public List<T> getResultsForCurrentPage() {
        return resultsForCurrentPage;
    }

    public void setResultsForCurrentPage(List<T> resultsForCurrentPage) {
        this.resultsForCurrentPage = resultsForCurrentPage;
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

    public long getTotalRecordSize() {
        return totalRecordSize;
    }

    public void setTotalRecordSize(long totalRecordSize) {
        this.totalRecordSize = totalRecordSize;
    }

    public long getTotalPageNum() {
        return totalPageNum;
    }

    public void setTotalPageNum(long totalPageNum) {
        this.totalPageNum = totalPageNum;
    }
}
