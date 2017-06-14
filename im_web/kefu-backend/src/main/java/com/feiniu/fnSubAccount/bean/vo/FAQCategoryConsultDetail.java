package com.feiniu.fnSubAccount.bean.vo;

/**
 * Author: morningking
 * Date: 2015/10/12
 * Contact: 243717042@qq.com
 */
public class FAQCategoryConsultDetail {
    private int faqCategoryID;
    private String title;
    private int consultCount;

    public int getFaqCategoryID() {
        return faqCategoryID;
    }

    public void setFaqCategoryID(int faqCategoryID) {
        this.faqCategoryID = faqCategoryID;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public int getConsultCount() {
        return consultCount;
    }

    public void setConsultCount(int consultCount) {
        this.consultCount = consultCount;
    }
}
