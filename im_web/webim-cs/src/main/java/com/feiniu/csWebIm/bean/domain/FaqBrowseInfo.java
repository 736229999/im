package com.feiniu.csWebIm.bean.domain;

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
@Table(name = "t_faq_browse_info")
public class FaqBrowseInfo {
    @Id
    @Column(name = "i_faq_id")
    private int faqID;

    @Column(name = "i_view_date")
    private long viewDate;

    @Column(name = "i_view_count")
    private int viewCount;

    @Column(name = "update_time")
    private Timestamp updateTime;

    public int getFaqID() {
        return faqID;
    }

    public void setFaqID(int faqID) {
        this.faqID = faqID;
    }

    public long getViewDate() {
        return viewDate;
    }

    public void setViewDate(long viewDate) {
        this.viewDate = viewDate;
    }

    public int getViewCount() {
        return viewCount;
    }

    public void setViewCount(int viewCount) {
        this.viewCount = viewCount;
    }

    public Timestamp getUpdateTime() {
        return updateTime;
    }

    public void setUpdateTime(Timestamp updateTime) {
        this.updateTime = updateTime;
    }
}
