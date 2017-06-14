package com.feiniu.csWebIm.bean.domain;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;
import java.sql.Timestamp;

/**
 * Author: morningking
 * Date: 2015/10/14
 * Contact: 243717042@qq.com
 */
public class AfterSaleBoardConfig {
    private Timestamp updateTime;
    private int boardID;
    private String title;
    private String descriptionListJson;
    private char isBindGroup;
    private int groupID;
    private char isBindUserOrder;
    private char dataType;

    public Timestamp getUpdateTime() {
        return updateTime;
    }

    public void setUpdateTime(Timestamp updateTime) {
        this.updateTime = updateTime;
    }

    public int getBoardID() {
        return boardID;
    }

    public void setBoardID(int boardID) {
        this.boardID = boardID;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getDescriptionListJson() {
        return descriptionListJson;
    }

    public void setDescriptionListJson(String descriptionListJson) {
        this.descriptionListJson = descriptionListJson;
    }

    public char getIsBindGroup() {
        return isBindGroup;
    }

    public void setIsBindGroup(char isBindGroup) {
        this.isBindGroup = isBindGroup;
    }

    public int getGroupID() {
        return groupID;
    }

    public void setGroupID(int groupID) {
        this.groupID = groupID;
    }

    public char getIsBindUserOrder() {
        return isBindUserOrder;
    }

    public void setIsBindUserOrder(char isBindUserOrder) {
        this.isBindUserOrder = isBindUserOrder;
    }

    public char getDataType() {
        return dataType;
    }

    public void setDataType(char dataType) {
        this.dataType = dataType;
    }
}
