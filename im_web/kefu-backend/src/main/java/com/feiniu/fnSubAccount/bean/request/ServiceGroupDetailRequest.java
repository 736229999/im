package com.feiniu.fnSubAccount.bean.request;

/**
 * Created by wangzhen on 2015/10/19.
 */
public class ServiceGroupDetailRequest {
    private String groupId;
    private String orgnName;
    private String serviceName;
    private String moumouName;

    private int pageSize;
    private int currentPageNo;

    public String getGroupId() {
        return groupId;
    }

    public void setGroupId(String groupId) {
        this.groupId = groupId;
    }

    public String getOrgnName() {
        return orgnName;
    }

    public void setOrgnName(String orgnName) {
        this.orgnName = orgnName;
    }

    public String getServiceName() {
        return serviceName;
    }

    public void setServiceName(String serviceName) {
        this.serviceName = serviceName;
    }

    public String getMoumouName() {
        return moumouName;
    }

    public void setMoumouName(String moumouName) {
        this.moumouName = moumouName;
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
