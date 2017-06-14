package com.feiniu.fnSubAccount.bean.request;

/**
 * Created by wangzhen on 2015/10/19.
 */
public class ServiceGroupSaveRequest {
    String groupId;
    String idList;
    String groupDetail;
    String groupName;
    Boolean needUpdateDescription;
    Boolean needAddGroup;

    public String getGroupId() {
        return groupId;
    }

    public void setGroupId(String groupId) {
        this.groupId = groupId;
    }

    public String getIdList() {
        return idList;
    }

    public void setIdList(String idList) {
        this.idList = idList;
    }

    public String getGroupDetail() {
        return groupDetail;
    }

    public void setGroupDetail(String groupDetail) {
        this.groupDetail = groupDetail;
    }

    public String getGroupName() {
        return groupName;
    }

    public void setGroupName(String groupName) {
        this.groupName = groupName;
    }

    public Boolean getNeedUpdateDescription() {
        return needUpdateDescription;
    }

    public void setNeedUpdateDescription(Boolean needUpdateDescription) {
        this.needUpdateDescription = needUpdateDescription;
    }

    public Boolean getNeedAddGroup() {
        return needAddGroup;
    }

    public void setNeedAddGroup(Boolean needAddGroup) {
        this.needAddGroup = needAddGroup;
    }
}
