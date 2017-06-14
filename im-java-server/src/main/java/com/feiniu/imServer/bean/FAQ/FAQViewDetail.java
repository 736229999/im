package com.feiniu.imServer.bean.FAQ;

/**
 * Author: morningking
 * Date: 2015/9/28
 * Contact: 243717042@qq.com
 */
public class FAQViewDetail {
    private int faqID;
    private String title;
    private int viewCount;

    public int getFaqID() {
        return faqID;
    }

    public void setFaqID(int faqID) {
        this.faqID = faqID;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public int getViewCount() {
        return viewCount;
    }

    public void setViewCount(int viewCount) {
        this.viewCount = viewCount;
    }
}
