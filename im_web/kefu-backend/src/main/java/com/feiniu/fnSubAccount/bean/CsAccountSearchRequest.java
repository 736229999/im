package com.feiniu.fnSubAccount.bean;

/**
 * Created by wangzhen on 2015/11/19.
 */
public class CsAccountSearchRequest {
    private String moumouName;
    private String importCsIdList;
    private String csIdList;
    private int status;
    private String startTime;
    private String endTime;
    private int pageSize;
    private int pageNo;

    public int getPageSize() {
        return pageSize;
    }

    public void setPageSize(int pageSize) {
        this.pageSize = pageSize;
    }

    public int getPageNo() {
        return pageNo;
    }

    public void setPageNo(int pageNo) {
        this.pageNo = pageNo;
    }

    public String getMoumouName() {
        return moumouName;
    }

    public void setMoumouName(String moumouName) {
        this.moumouName = moumouName;
    }

    public String getImportCsIdList() {
        return importCsIdList;
    }

    public void setImportCsIdList(String importCsIdList) {
        this.importCsIdList = importCsIdList;
    }

    public String getCsIdList() {
        return csIdList;
    }

    public void setCsIdList(String csIdList) {
        this.csIdList = csIdList;
    }

    public int getStatus() {
        return status;
    }

    public void setStatus(int status) {
        this.status = status;
    }

    public String getStartTime() {
        return startTime;
    }

    public void setStartTime(String startTime) {
        this.startTime = startTime;
    }

    public String getEndTime() {
        return endTime;
    }

    public void setEndTime(String endTime) {
        this.endTime = endTime;
    }
}
