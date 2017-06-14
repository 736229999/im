package com.feiniu.webim.bean.request;

/**
 * Created by wangyixiong on 2016/2/24.
 */
public class GetRecentMerchantInfoRequest {
    int customerId;
    int maxNumber;

    public int getMaxNumber() {
        return maxNumber;
    }

    public void setMaxNumber(int maxNumber) {
        this.maxNumber = maxNumber;
    }

    public int getCustomerId() {
        return customerId;
    }

    public void setCustomerId(int customerId) {
        this.customerId = customerId;
    }
}
