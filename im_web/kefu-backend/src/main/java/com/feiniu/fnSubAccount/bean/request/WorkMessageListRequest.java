package com.feiniu.fnSubAccount.bean.request;

import com.feiniu.fnSubAccount.domain.WorkMessage;

import java.util.List;

/**
 * Created by wangchen on 2015/2/4.
 */
public class WorkMessageListRequest {
    private String subMerchantIDList;
    private int subMerchantIDListCount;
    private String storeName;
    private String sellerID;
    private String sellerName;
    private Float startOnlineTime;
    private Float endOnlineTime;
    private Float startResponseIn90sRate;
    private Float endResponseIn90sRate;
    private Integer startTime;
    private Integer endTime;
    private int pageSize;
    private int currentPageNo;
    private List<WorkMessage> workmessages;
    //为了第一页添加汇总信息和平均信息的拓展
    private boolean needCountTotalAndAVG;
    private String allIdList;

    public String getSubMerchantIDList() {
        return subMerchantIDList;
    }

    public void setSubMerchantIDList(String subMerchantIDList) {
        this.subMerchantIDList = subMerchantIDList;
    }

    public int getSubMerchantIDListCount() {
        return subMerchantIDListCount;
    }

    public void setSubMerchantIDListCount(int subMerchantIDListCount) {
        this.subMerchantIDListCount = subMerchantIDListCount;
    }

    public String getStoreName() {
        return storeName;
    }

    public void setStoreName(String storeName) {
        this.storeName = storeName;
    }

    public String getSellerID() {
        return sellerID;
    }

    public void setSellerID(String sellerID) {
        this.sellerID = sellerID;
    }

    public String getSellerName() {
        return sellerName;
    }

    public void setSellerName(String sellerName) {
        this.sellerName = sellerName;
    }

    public Float getStartOnlineTime() {
        return startOnlineTime;
    }

    public void setStartOnlineTime(Float startOnlineTime) {
        this.startOnlineTime = startOnlineTime;
    }

    public Float getEndOnlineTime() {
        return endOnlineTime;
    }

    public void setEndOnlineTime(Float endOnlineTime) {
        this.endOnlineTime = endOnlineTime;
    }

    public Float getStartResponseIn90sRate() {
        return startResponseIn90sRate;
    }

    public void setStartResponseIn90sRate(Float startResponseIn90sRate) {
        this.startResponseIn90sRate = startResponseIn90sRate;
    }

    public Float getEndResponseIn90sRate() {
        return endResponseIn90sRate;
    }

    public void setEndResponseIn90sRate(Float endResponseIn90sRate) {
        this.endResponseIn90sRate = endResponseIn90sRate;
    }

    public Integer getStartTime() {
        return startTime;
    }

    public void setStartTime(Integer startTime) {
        this.startTime = startTime;
    }

    public Integer getEndTime() {
        return endTime;
    }

    public void setEndTime(Integer endTime) {
        this.endTime = endTime;
    }

    public int getPageSize() {
        return pageSize;
    }

    public void setPageSize(int pageSize) {
        this.pageSize = pageSize;
    }

    public int getCurrentPageNo() {
        return currentPageNo;
    }

    public void setCurrentPageNo(int currentPageNo) {
        this.currentPageNo = currentPageNo;
    }

    public List<WorkMessage> getWorkmessages() {
        return workmessages;
    }

    public void setWorkmessages(List<WorkMessage> workmessages) {
        this.workmessages = workmessages;
    }

    public boolean isNeedCountTotalAndAVG() {
        return needCountTotalAndAVG;
    }

    public void setNeedCountTotalAndAVG(boolean needCountTotalAndAVG) {
        this.needCountTotalAndAVG = needCountTotalAndAVG;
    }

    public String getAllIdList() {
        return allIdList;
    }

    public void setAllIdList(String allIdList) {
        this.allIdList = allIdList;
    }
}
