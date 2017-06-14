package com.feiniu.subAccount.bean.response;

/**
 * Created by wangzhen on 2015/8/7.
 */
public class ReceptionInfo {

    private long subMerchantId;
    private long replyStart;
    private long replyEnd;

    public long getSubMerchantId() {
        return subMerchantId;
    }

    public void setSubMerchantId(long subMerchantId) {
        this.subMerchantId = subMerchantId;
    }

    public long getReplyStart() {
        return replyStart;
    }

    public void setReplyStart(long replyStart) {
        this.replyStart = replyStart;
    }

    public long getReplyEnd() {
        return replyEnd;
    }

    public void setReplyEnd(long replyEnd) {
        this.replyEnd = replyEnd;
    }
}
