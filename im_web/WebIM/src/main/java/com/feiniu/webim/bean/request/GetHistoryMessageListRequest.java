package com.feiniu.webim.bean.request;

/**
 * Author: morningking
 * Date: 2016/2/15
 * Contact: 243717042@qq.com
 */
public class GetHistoryMessageListRequest {
    private int customerId;
    private int sellerId;
    private boolean isMerchantId;
    private int sendTime;
    private int pageCount;

    public int getCustomerId() {
        return customerId;
    }

    public void setCustomerId(int customerId) {
        this.customerId = customerId;
    }

    public int getSellerId() {
        return sellerId;
    }

    public void setSellerId(int sellerId) {
        this.sellerId = sellerId;
    }

    public boolean getIsMerchantId() {
        return isMerchantId;
    }

    public void setIsMerchantId(boolean isMerchantID) {
        this.isMerchantId = isMerchantID;
    }

    public int getSendTime() {
        return sendTime;
    }

    public void setSendTime(int sendTime) {
        this.sendTime = sendTime;
    }

    public int getPageCount() {
        return pageCount;
    }

    public void setPageCount(int pageCount) {
        this.pageCount = pageCount;
    }
}
