package com.feiniu.csWebIm.bean.domain;

/**
 * Created by chenwuming on 2015/11/26.
 * */

 public class FaqCategoryList {
    private String categoryID;
    private String isShow;
    private String title;
    private String remark;
    private String lastOperator;
    private String updateTime;
    private String consultCount;

    public String getCategoryID() {
        return categoryID;
    }

    public void setCategoryID(String categoryID) {
        this.categoryID = categoryID;
    }

    public String getIsShow() {
        return isShow;
    }

    public void setIsShow(String isShow) {
        this.isShow = isShow;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getRemark() {
        return remark;
    }

    public void setRemark(String remark) {
        this.remark = remark;
    }

    public String getLastOperator() {
        return lastOperator;
    }

    public void setLastOperator(String lastOperator) {
        this.lastOperator = lastOperator;
    }

    public String getUpdateTime() {
        return updateTime;
    }

    public void setUpdateTime(String updateTime) {
        this.updateTime = updateTime;
    }

    public String getConsultCount() {
        return consultCount;
    }

    public void setConsultCount(String consultCount) {
        this.consultCount = consultCount;
    }
}