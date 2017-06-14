package com.feiniu.imServer.bean.backend;

/**
 * Created by wangzhen on 2015/11/18.
 */
public class OrganNameInfo {
    private Long id;
    private Long parentId;
    private String organName;

    public Long getParentId() {
        return parentId;
    }

    public void setParentId(Long parentId) {
        this.parentId = parentId;
    }

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public String getOrganName() {
        return organName;
    }

    public void setOrganName(String organName) {
        this.organName = organName;
    }
}
