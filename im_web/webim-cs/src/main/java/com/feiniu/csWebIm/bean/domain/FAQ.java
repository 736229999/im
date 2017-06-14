package com.feiniu.csWebIm.bean.domain;

import com.feiniu.csWebIm.utils.TimeStampSerializer;
import org.codehaus.jackson.map.annotate.JsonSerialize;
import org.hibernate.annotations.GenericGenerator;

import javax.persistence.*;
import java.sql.Timestamp;

/**
 * Author: morningking
 * Date: 2015/9/28
 * Contact: 243717042@qq.com
 */
@Table(name = "T_FAQ")
@Entity
public class FAQ {
    @Id
    @GenericGenerator(name = "FAQIdGenerator", strategy = "sequence",
            parameters = {@org.hibernate.annotations.Parameter(name = "sequence", value = "seq_faq")})
    @GeneratedValue(generator = "FAQIdGenerator")
    @Column(name = "i_faq_id")
    private int faqID;

    @Column(name = "c_is_show")
    private char isShow = '0';

    @Column(name = "v_title")
    private String title;

    @Column(name = "v_summary")
    private String faqSummary;

    @Column(name = "v_last_operator")
    private String lastOperator;

    @Column(name = "i_order")
    private int showOrder = -1;

    @Column(name = "update_time")
    private Timestamp updateTime;

    @Transient
    private int viewCount;

    public int getFaqID() {
        return faqID;
    }

    public void setFaqID(int faqID) {
        this.faqID = faqID;
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

    public String getFaqSummary() {
        return faqSummary;
    }

    public void setFaqSummary(String faqSummary) {
        this.faqSummary = faqSummary;
    }

    public String getLastOperator() {
        return lastOperator;
    }

    public void setLastOperator(String lastOperator) {
        this.lastOperator = lastOperator;
    }

    public int getShowOrder() {
        return showOrder;
    }

    public void setShowOrder(int showOrder) {
        this.showOrder = showOrder;
    }

    @JsonSerialize(using = TimeStampSerializer.class)
    public Timestamp getUpdateTime() {
        return updateTime;
    }

    public void setUpdateTime(Timestamp updateTime) {
        this.updateTime = updateTime;
    }

    public int getViewCount() {
        return viewCount;
    }

    public void setViewCount(int viewCount) {
        this.viewCount = viewCount;
    }
}
