package com.feiniu.webim.bean.domain;

/**
 * Created by wangyixiong on 2015/11/11.
 */
public class RecentlyMobileSeller {
    private int merchantId;
    private String merchantName;
    private String merchantLogoUrl;
    private int unreadMsgCount;
    private String lastMsg;
    private int lastMsgTime;
    private boolean isOnline = false;
    private int subAccountId;
    private String subAccountNickname;

    public int getMerchantId() {
        return merchantId;
    }

    public void setMerchantId(int merchantId) {
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

    public int getUnreadMsgCount() {
        return unreadMsgCount;
    }

    public void setUnreadMsgCount(int unreadMsgCount) {
        this.unreadMsgCount = unreadMsgCount;
    }

    public String getLastMsg() {
        return lastMsg;
    }

    public void setLastMsg(String lastMsg) {
        this.lastMsg = lastMsg;
    }

    public int getLastMsgTime() {
        return lastMsgTime;
    }

    public void setLastMsgTime(int lastMsgTime) {
        this.lastMsgTime = lastMsgTime;
    }

    public int getSubAccountId() {
        return subAccountId;
    }

    public void setSubAccountId(int subAccountId) {
        this.subAccountId = subAccountId;
    }

    public String getSubAccountNickname() {
        return subAccountNickname;
    }

    public void setSubAccountNickname(String subAccountNickname) {
        this.subAccountNickname = subAccountNickname;
    }

    public boolean isOnline() {
        return isOnline;
    }

    public void setOnline(boolean isOnline) {
        this.isOnline = isOnline;
    }
}
