package com.feiniu.fnSubAccount.bean;

/**
 * Created by wangzhen on 2015/10/16.
 */
public class ServiceGroupDetail {
    String userId;
    String csId;
    String MoumouName;
    String ServiceName;
    String orgnName;
    String groupName;
    String Status;

    public String getCsId() {
        return csId;
    }

    public void setCsId(String csId) {
        this.csId = csId;
    }

    public String getUserId() {
        return userId;
    }

    public void setUserId(String userId) {
        this.userId = userId;
    }

    public String getMoumouName() {
        return MoumouName;
    }

    public void setMoumouName(String moumouName) {
        MoumouName = moumouName;
    }

    public String getServiceName() {
        return ServiceName;
    }

    public void setServiceName(String serviceName) {
        ServiceName = serviceName;
    }

    public String getOrgnName() {
        return orgnName;
    }

    public void setOrgnName(String orgnName) {
        this.orgnName = orgnName;
    }

    public String getGroupName() {
        return groupName;
    }

    public void setGroupName(String groupName) {
        this.groupName = groupName;
    }

    public String getStatus() {
        return Status;
    }

    public void setStatus(String status) {
        Status = status;
    }
}
