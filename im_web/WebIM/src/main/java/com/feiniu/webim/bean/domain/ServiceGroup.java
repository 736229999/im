package com.feiniu.webim.bean.domain;

/**
 * Created by wangchen on 2015/2/25.
 */
public class ServiceGroup {
    private long groupID;

    private String groupName;

    private long merchantID;

    private char isShowOnStore = '0';

    private int showOrder = 0;

    public long getGroupID() {
        return groupID;
    }

    public void setGroupID(long groupID) {
        this.groupID = groupID;
    }

    public String getGroupName() {
        return groupName;
    }

    public void setGroupName(String groupName) {
        this.groupName = groupName;
    }

    public long getMerchantID() {
        return merchantID;
    }

    public void setMerchantID(long merchantID) {
        this.merchantID = merchantID;
    }

    public char getIsShowOnStore() {
        return isShowOnStore;
    }

    public void setIsShowOnStore(char isShowOnStore) {
        this.isShowOnStore = isShowOnStore;
    }

    public int getShowOrder() {
        return showOrder;
    }

    public void setShowOrder(int showOrder) {
        this.showOrder = showOrder;
    }
}
