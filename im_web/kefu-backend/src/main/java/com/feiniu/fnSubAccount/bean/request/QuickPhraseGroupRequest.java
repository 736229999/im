package com.feiniu.fnSubAccount.bean.request;

/**
 * Created by zhangmin on 2015/11/17.
 */
public class QuickPhraseGroupRequest {
    private String groupNamePart;
    private Integer currentPageNo = 1;
    private Integer pageSize = 10;

    private String oldGroupName;
    private String newGroupName;


    public String getGroupNamePart() {
        return groupNamePart;
    }

    public void setGroupNamePart(String groupNamePart) {
        this.groupNamePart = groupNamePart;
    }

    public Integer getCurrentPageNo() {
        return currentPageNo;
    }

    public void setCurrentPageNo(Integer currentPageNo) {
        this.currentPageNo = currentPageNo;
    }

    public Integer getPageSize() {
        return pageSize;
    }

    public void setPageSize(Integer pageSize) {
        this.pageSize = pageSize;
    }


    public String getOldGroupName() {
        return oldGroupName;
    }

    public void setOldGroupName(String oldGroupName) {
        this.oldGroupName = oldGroupName;
    }

    public String getNewGroupName() {
        return newGroupName;
    }

    public void setNewGroupName(String newGroupName) {
        this.newGroupName = newGroupName;
    }
}
