package com.feiniu.csWebIm.bean.domain;

import java.sql.Timestamp;

/**
 * Author: morningking
 * Date: 2016/3/11
 * Contact: 243717042@qq.com
 */
public class TouchBoardConfig {
    private int id;
    private String title;
    private String description;
    private String icon;
    private Integer showOrder;
    private char isBindGroup;
    private Integer groupID;
    private char isBindUserOrder;
    private Timestamp updateTime;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public String getIcon() {
        return icon;
    }

    public void setIcon(String icon) {
        this.icon = icon;
    }

    public Integer getShowOrder() {
        return showOrder;
    }

    public void setShowOrder(Integer showOrder) {
        this.showOrder = showOrder;
    }

    public char getIsBindGroup() {
        return isBindGroup;
    }

    public void setIsBindGroup(char isBindGroup) {
        this.isBindGroup = isBindGroup;
    }

    public Integer getGroupID() {
        return groupID;
    }

    public void setGroupID(Integer groupID) {
        this.groupID = groupID;
    }

    public char getIsBindUserOrder() {
        return isBindUserOrder;
    }

    public void setIsBindUserOrder(char isBindUserOrder) {
        this.isBindUserOrder = isBindUserOrder;
    }

    public Timestamp getUpdateTime() {
        return updateTime;
    }

    public void setUpdateTime(Timestamp updateTime) {
        this.updateTime = updateTime;
    }

}
