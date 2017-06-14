package com.feiniu.fnSubAccount.bean;

/**
 * Created by wangzhen on 2015/9/29.
 */
public class AppraiseStat {
    String subMerchantName;
    String subMerchantId;
    String[] appraiseCount={"0","0","0","0","0","0"};
    String goodRate;

    public String getSubMerchantId() {
        return subMerchantId;
    }

    public void setSubMerchantId(String subMerchantId) {
        this.subMerchantId = subMerchantId;
    }

    public AppraiseStat(Long name){
        subMerchantName = String.valueOf(name);
        subMerchantId = String.valueOf(name);
        goodRate = "--";
    }

    public String getSubMerchantName() {
        return subMerchantName;
    }

    public void setSubMerchantName(String subMerchantName) {
        this.subMerchantName = subMerchantName;
    }

    public String[] getAppraiseCount() {
        return appraiseCount;
    }

    public void setAppraiseCount(String[] appraiseCount) {
        this.appraiseCount = appraiseCount;
    }

    public String getGoodRate() {
        return goodRate;
    }

    public void setGoodRate(String goodRate) {
        this.goodRate = goodRate;
    }
}
