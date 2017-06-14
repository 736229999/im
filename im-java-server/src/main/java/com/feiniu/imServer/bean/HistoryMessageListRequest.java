package com.feiniu.imServer.bean;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by chenqing on 2016/2/4.
 */
public class HistoryMessageListRequest {
    private Integer customerId=0;
    private Integer sellerId=0;
    private Boolean isMerchantId = false;
    private long sendTime;
    private Integer pageCount = 10;
    private List<String> subMerchantIds;

    public Integer getCustomerId() {
        return customerId;
    }

    public void setCustomerId(Integer customerId) {
        this.customerId = customerId;
    }

    public Integer getSellerId() {
        return sellerId;
    }

    public void setSellerId(Integer sellerId) {
        this.sellerId = sellerId;
    }

    public Boolean isMerchantId() {
        return isMerchantId;
    }

    public void setIsMerchantId(Boolean isMerchantId) {
        this.isMerchantId = isMerchantId;
    }

    public long getSendTime() {
        return sendTime;
    }

    public void setSendTime(long sendTime) {
        this.sendTime = sendTime;
    }

    public Integer getPageCount() {
        return pageCount;
    }

    public void setPageCount(Integer pageCount) {
        this.pageCount = pageCount;
    }

    public List<String> getSubMerchantIds() {
        return subMerchantIds;
    }

    public void setSubMerchantIds(List<String> subMerchantIds) {
        this.subMerchantIds = subMerchantIds;
    }
}
