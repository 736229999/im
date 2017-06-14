package com.feiniu.imServer.bean;

/**
 * Created by wangzhen on 2016/2/3.
 */
public class RecentlyMobileSeller {
    private int merchantId;
    private String lastMsg;
    private Long lastMsgTime;
    private Long subAccountId;
    private String subAccountNickname;

    public int getMerchantId() {
        return merchantId;
    }

    public void setMerchantId(int merchantId) {
        this.merchantId = merchantId;
    }

    public String getLastMsg() {
        return lastMsg;
    }

    public void setLastMsg(String lastMsg) {
        this.lastMsg = lastMsg;
    }

    public Long getLastMsgTime() {
        return lastMsgTime;
    }

    public void setLastMsgTime(Long lastMsgTime) {
        this.lastMsgTime = lastMsgTime;
    }

    public Long getSubAccountId() {
        return subAccountId;
    }

    public void setSubAccountId(Long subAccountId) {
        this.subAccountId = subAccountId;
    }

    public String getSubAccountNickname() {
        return subAccountNickname;
    }

    public void setSubAccountNickname(String subAccountNickname) {
        this.subAccountNickname = subAccountNickname;
    }
}
