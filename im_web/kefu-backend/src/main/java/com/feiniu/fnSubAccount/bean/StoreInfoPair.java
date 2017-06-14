package com.feiniu.fnSubAccount.bean;

/**
 * Created by wangchen on 2015/5/22.
 */
public class StoreInfoPair {
    private String storeName;
    private long merchantID;

    public String getStoreName() {
        return storeName;
    }

    public void setStoreName(String storeName) {
        this.storeName = storeName;
    }

    public long getMerchantID() {
        return merchantID;
    }

    public void setMerchantID(long merchantID) {
        this.merchantID = merchantID;
    }
}
