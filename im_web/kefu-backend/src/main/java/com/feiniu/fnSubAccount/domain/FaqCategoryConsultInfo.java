package com.feiniu.fnSubAccount.domain;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;
import java.sql.Timestamp;

/**
 * Author: morningking
 * Date: 2015/11/11
 * Contact: 243717042@qq.com
 */
@Entity
@Table(name = "t_faq_category_consult_info")
public class FaqCategoryConsultInfo {
    @Id
    @Column(name = "i_faq_category_id")
    private int faqCategoryID;

    @Column(name = "i_consult_date")
    private long consultDate;

    @Column(name = "i_consult_count")
    private int consultCount;

    @Column(name = "update_time")
    private Timestamp updateTime;

    public int getFaqCategoryID() {
        return faqCategoryID;
    }

    public void setFaqCategoryID(int faqCategoryID) {
        this.faqCategoryID = faqCategoryID;
    }

    public long getConsultDate() {
        return consultDate;
    }

    public void setConsultDate(long consultDate) {
        this.consultDate = consultDate;
    }

    public int getConsultCount() {
        return consultCount;
    }

    public void setConsultCount(int consultCount) {
        this.consultCount = consultCount;
    }

    public Timestamp getUpdateTime() {
        return updateTime;
    }

    public void setUpdateTime(Timestamp updateTime) {
        this.updateTime = updateTime;
    }
}
