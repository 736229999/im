package com.feiniu.fnSubAccount.domain;

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
@Entity
@Table(name = "t_after_sale_board_config")
public class AfterSaleBoardConfig implements Cloneable{
    @Column(name = "update_time")
    private Timestamp updateTime;

    @Id
    @Column(name = "i_board_id")
    private int boardID;

    @Column(name = "v_title")
    private String title;

    @Column(name = "v_description_list")
    private String descriptionListJson;

    @Column(name = "c_is_bind_group")
    private char isBindGroup;

    @Column(name = "i_group_id")
    private int groupID;

    @Column(name = "c_is_bind_user_order")
    private char isBindUserOrder;

    @Column(name = "c_data_type")
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

    @Override
    public Object clone() throws CloneNotSupportedException {
        return super.clone();
    }
}
