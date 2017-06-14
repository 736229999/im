package com.feiniu.fnSubAccount.domain;

import org.hibernate.annotations.GenericGenerator;

import javax.persistence.*;
import java.sql.Timestamp;

/**
 * Author: morningking
 * Date: 2016/3/8
 * Contact: 243717042@qq.com
 */
@Entity
@Table(name = "t_touch_portal_board_config")
public class TouchPortalBoardConfig implements Cloneable {
    @Id
    @Column(name = "i_id")
    @GenericGenerator(name = "TouchPortalBoardConfigIdGenerator", strategy = "sequence",
            parameters = {@org.hibernate.annotations.Parameter(name = "sequence", value = "seq_touch_portal_board_config")})
    @GeneratedValue(generator = "TouchPortalBoardConfigIdGenerator")
    private int id;

    @Column(name = "v_title")
    private String title;

    @Column(name = "v_description")
    private String description;

    @Column(name = "v_icon")
    private String icon;

    @Column(name = "i_show_order")
    private Integer showOrder;

    @Column(name = "c_is_bind_group")
    private char isBindGroup;

    @Column(name = "i_group_id")
    private Integer groupID;

    @Column(name = "c_is_bind_user_order")
    private char isBindUserOrder;

    @Column(name = "c_data_type")
    private char dataType;

    @Column(name = "update_time")
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

    public char getDataType() {
        return dataType;
    }

    public void setDataType(char dataType) {
        this.dataType = dataType;
    }

    public Timestamp getUpdateTime() {
        return updateTime;
    }

    public void setUpdateTime(Timestamp updateTime) {
        this.updateTime = updateTime;
    }

    @Override
    public TouchPortalBoardConfig clone() throws CloneNotSupportedException {
        return (TouchPortalBoardConfig) super.clone();
    }
}
