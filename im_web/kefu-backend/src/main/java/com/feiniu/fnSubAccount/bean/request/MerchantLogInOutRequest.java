package com.feiniu.fnSubAccount.bean.request;

/**
 * Created by zhangmin on 2016/4/12.
 */
public class MerchantLogInOutRequest {
    private Long merchantId;
    private Long subMerchantId;
    private Long startTime;
    private Long endTime;
    private Integer currentPageNo = 0;
    private Integer pageSize = 10;

    public Long getMerchantId() {
        return merchantId;
    }

    public void setMerchantId(Long merchantId) {
        this.merchantId = merchantId;
    }

    public Long getSubMerchantId() {
        return subMerchantId;
    }

    public void setSubMerchantId(Long subMerchantId) {
        this.subMerchantId = subMerchantId;
    }

    public Long getStartTime() {
        return startTime;
    }

    public void setStartTime(Long startTime) {
        this.startTime = startTime;
    }

    public Long getEndTime() {
        return endTime;
    }

    public void setEndTime(Long endTime) {
        this.endTime = endTime;
    }

    public Integer getCurrentPageNo() {
        return currentPageNo;
    }

    public void setCurrentPageNo(Integer currentPageNo) {
        this.currentPageNo = currentPageNo;
    }

    public Integer getPageSize() {
        return pageSize;
    }

    public void setPageSize(Integer pageSize) {
        this.pageSize = pageSize;
    }
}
