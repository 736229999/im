package com.feiniu.fnSubAccount.domain;

import com.feiniu.fnSubAccount.util.TimeStampSerializer;
import org.codehaus.jackson.map.annotate.JsonSerialize;
import org.hibernate.annotations.GenericGenerator;

import javax.persistence.*;
import java.sql.Timestamp;

/**
 * Author: morningking
 * Date: 2015/10/12
 * Contact: 243717042@qq.com
 */
@Entity
@Table(name = "t_faq_category")
public class FAQCategory {
    @Id
    @Column(name = "i_id")
    @GenericGenerator(name = "FAQCategoryIdGenerator", strategy = "sequence",
            parameters = {@org.hibernate.annotations.Parameter(name = "sequence", value = "seq_faq_category")})
    @GeneratedValue(generator = "FAQCategoryIdGenerator")
    private int categoryID;

    @Column(name = "c_is_show", columnDefinition = "char(1)")
    private char isShow = '0';

    @Column(name = "v_title")
    private String title;

    @Column(name = "v_summary")
    private String remark;

    @Column(name = "v_last_operator")
    private String lastOperator;

    @Column(name = "update_time")
    private Timestamp updateTime;

    @Transient
    private int consultCount = 0;

    public int getCategoryID() {
        return categoryID;
    }

    public void setCategoryID(int categoryID) {
        this.categoryID = categoryID;
    }

    public char getIsShow() {
        return isShow;
    }

    public void setIsShow(char isShow) {
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

    @JsonSerialize(using = TimeStampSerializer.class)
    public Timestamp getUpdateTime() {
        return updateTime;
    }

    public void setUpdateTime(Timestamp updateTime) {
        this.updateTime = updateTime;
    }

    public int getConsultCount() {
        return consultCount;
    }

    public void setConsultCount(int consultCount) {
        this.consultCount = consultCount;
    }
}
