package com.feiniu.fnSubAccount.bean;

/**
 * Created by wangzhen on 2015/7/22.
 */
public class WorkLoadInfo extends BaseWorkLoadInfo implements Cloneable {
    private long merchantId;

    private String subMerchantName;
    private String subMerchantGroup;

    @Override
    public Object clone(){
        WorkLoadInfo cloneitem= cloneitem =(WorkLoadInfo)super.clone();

        return cloneitem;
    }

    public long getMerchantId() {
        return merchantId;
    }

    public void setMerchantId(long merchantId) {
        this.merchantId = merchantId;
    }

    public String getSubMerchantName() {
        return subMerchantName;
    }

    public void setSubMerchantName(String subMerchantName) {
        this.subMerchantName = subMerchantName;
    }

    public String getSubMerchantGroup() {
        return subMerchantGroup;
    }

    public void setSubMerchantGroup(String subMerchantGroup) {
        this.subMerchantGroup = subMerchantGroup;
    }
}
