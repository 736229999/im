package com.feiniu.fnSubAccount.bean.request;

/**
 * Created by wangzhen on 2015/10/23.
 */
public class GroupModifyStatusRequest {
    Long setStatus;
    String userId;
    String groupId;

    public String getGroupId() {
        return groupId;
    }

    public void setGroupId(String groupId) {
        this.groupId = groupId;
    }

    public Long getSetStatus() {
        return setStatus;
    }

    public void setSetStatus(Long setStatus) {
        this.setStatus = setStatus;
    }

    public String getUserId() {
        return userId;
    }

    public void setUserId(String userId) {
        this.userId = userId;
    }
}
