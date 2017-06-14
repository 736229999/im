package com.feiniu.fnSubAccount.bean.request;

/**
 * Author: morningking
 * Date: 2016/4/7 13:57
 * Contact: 243717042@qq.com
 */
public class GetChatSessionRequest {
    private Boolean isQueuedTimeAsc;
    private Boolean isStartChatTimeAsc;

    public Boolean getIsQueuedTimeAsc() {
        return isQueuedTimeAsc;
    }

    public void setIsQueuedTimeAsc(Boolean isQueuedTimeAsc) {
        this.isQueuedTimeAsc = isQueuedTimeAsc;
    }

    public Boolean getIsStartChatTimeAsc() {
        return isStartChatTimeAsc;
    }

    public void setIsStartChatTimeAsc(Boolean isStartChatTimeAsc) {
        this.isStartChatTimeAsc = isStartChatTimeAsc;
    }
}
