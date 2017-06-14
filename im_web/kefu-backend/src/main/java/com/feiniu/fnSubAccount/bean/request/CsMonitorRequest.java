package com.feiniu.fnSubAccount.bean.request;

/**
 * Created by wangzhen on 2015/11/24.
 */
public class CsMonitorRequest {
    private int pageSize;
    private int currentPageNo;

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
