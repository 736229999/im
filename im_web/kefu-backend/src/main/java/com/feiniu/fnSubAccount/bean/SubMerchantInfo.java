package com.feiniu.fnSubAccount.bean;

/**
 * Created by wangchen on 2015/2/6.
 */
public class SubMerchantInfo {
    private long memberId;
    private long merchantId;
    private int memberType;
    private String username;
    private String truename;

    public long getMemberId() {
        return memberId;
    }

    public void setMemberId(long memberId) {
        this.memberId = memberId;
    }

    public long getMerchantId() {
        return merchantId;
    }

    public void setMerchantId(long merchantId) {
        this.merchantId = merchantId;
    }

    public int getMemberType() {
        return memberType;
    }

    public void setMemberType(int memberType) {
        this.memberType = memberType;
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getTruename() {
        return truename;
    }

    public void setTruename(String truename) {
        this.truename = truename;
    }

    public boolean isMainAccount() {
        return this.memberType == 1;
    }
}
