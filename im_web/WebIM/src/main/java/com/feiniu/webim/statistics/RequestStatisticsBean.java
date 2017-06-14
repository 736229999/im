package com.feiniu.webim.statistics;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonIgnoreType;
import com.feiniu.webim.config.SystemConfig;

/**
 * Created by wangchen on 2015/1/22.
 */

public class RequestStatisticsBean {
    private int cmd;
    private long sendTime;
    private long receivedTime;
    private boolean isTimedOut;

    public RequestStatisticsBean(){
        this.sendTime = System.currentTimeMillis();
    }

    public int getCmd() {
        return cmd;
    }

    public void setCmd(int cmd) {
        this.cmd = cmd;
    }

    public long getSendTime() {
        return sendTime;
    }

    public void setSendTime(long sendTime) {
        this.sendTime = sendTime;
    }

    public long getReceivedTime() {
        return receivedTime;
    }

    public void setReceivedTime(long receivedTime) {
        this.receivedTime = receivedTime;
    }

    public boolean isTimedOut() {
        return isTimedOut;
    }

    public void setTimedOut(boolean isTimedOut) {
        this.isTimedOut = isTimedOut;
    }
}
