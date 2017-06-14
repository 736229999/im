package com.feiniu.webim.bean.web.response;

/**
 * Created by wangyixiong on 2015/11/10.
 */
public class MerchantUnreadMsgListResponse {
    private Integer merchantId;
    private String merchantName;
    private String merchantLogoUrl;
    private Integer unreadMsgCount;
    private Integer lastSubAccountId;

    public Integer getMerchantId() {
        return merchantId;
    }

    public void setMerchantId(Integer merchantId) {
        this.merchantId = merchantId;
    }

    public String getMerchantName() {
        return merchantName;
    }

    public void setMerchantName(String merchantName) {
        this.merchantName = merchantName;
    }

    public String getMerchantLogoUrl() {
        return merchantLogoUrl;
    }

    public void setMerchantLogoUrl(String merchantLogoUrl) {
        this.merchantLogoUrl = merchantLogoUrl;
    }

    public Integer getUnreadMsgCount() {
        return unreadMsgCount;
    }

    public void setUnreadMsgCount(Integer unreadMsgCount) {
        this.unreadMsgCount = unreadMsgCount;
    }

    public Integer getLastSubAccountId() {
        return lastSubAccountId;
    }

    public void setLastSubAccountId(Integer lastSubAccountId) {
        this.lastSubAccountId = lastSubAccountId;
    }
}
