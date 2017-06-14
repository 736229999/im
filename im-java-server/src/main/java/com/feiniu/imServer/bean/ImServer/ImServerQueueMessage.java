package com.feiniu.imServer.bean.ImServer;

import java.util.List;

/**
 * Created by wangzhen on 2015/12/30.
 */
public class ImServerQueueMessage {
    private long GroupId;
    private List<ImServerQueueInfo> Queue;
    private List<ImServerSubAccountInfo> SubAccount;
    private Long TimeStamp;
    private String logInfo;

    public String getLogInfo() {
        return logInfo;
    }

    public void setLogInfo(String logInfo) {
        this.logInfo = logInfo;
    }

    public long getGroupId() {
        return GroupId;
    }

    public void setGroupId(long groupId) {
        GroupId = groupId;
    }

    public List<ImServerQueueInfo> getQueue() {
        return Queue;
    }

    public void setQueue(List<ImServerQueueInfo> queue) {
        Queue = queue;
    }

    public List<ImServerSubAccountInfo> getSubAccount() {
        return SubAccount;
    }

    public void setSubAccount(List<ImServerSubAccountInfo> subAccount) {
        SubAccount = subAccount;
    }

    public Long getTimeStamp() {
        return TimeStamp;
    }

    public void setTimeStamp(Long timeStamp) {
        TimeStamp = timeStamp;
    }
}
