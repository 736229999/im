package com.feiniu.subAccount.bean.request;

/**
 * Created by wangchen on 2015/2/7.
 */
public class DiverseConfigRequest {
    private int subMerchantID;
    private int merchantID;
    private int diverseType;

    public int getSubMerchantID() {
        return subMerchantID;
    }

    public void setSubMerchantID(int subMerchantID) {
        this.subMerchantID = subMerchantID;
    }

    public int getMerchantID() {
        return merchantID;
    }

    public void setMerchantID(int merchantID) {
        this.merchantID = merchantID;
    }

    public int getDiverseType() {
        return diverseType;
    }

    public void setDiverseType(int diverseType) {
        this.diverseType = diverseType;
    }
}
