package com.feiniu.webim.bean.response;

import com.feiniu.webim.bean.domain.HistoryMessage;
import com.feiniu.webim.constants.Constants;

import java.util.List;

/**
 * Created by wangchen on 2014/12/11.
 */
public class HistoryMessageListResponseBean extends BaseResponseBean {
    private int ret;
    private int over;
    private int returnnum;
    private int msgnum;
    private long time;
    private List<HistoryMessage> msgInfo;
    private int hasMore;
    private int isFailed;
    private String uuid;

    public int getHasMore() {
        return hasMore;
    }

    public int getIsFailed() {
        return isFailed;
    }

    public void setIsFailed(int isFailed) {
        this.isFailed = isFailed;
    }

    public void setHasMore(int hasMore) {
        this.hasMore = hasMore;
    }

    public int getRet() {
        return ret;
    }

    public void setRet(int ret) {
        this.ret = ret;
    }

    public int getOver() {
        return over;
    }

    public void setOver(int over) {
        this.over = over;
    }

    public long getTime() {
        return time * Constants.MSG_TIME_TRANSFORM;
    }

    public void setTime(long time) {
        this.time = time;
    }

    public int getReturnnum() {
        return returnnum;
    }

    public void setReturnnum(int returnnum) {
        this.returnnum = returnnum;
    }

    public int getMsgnum() {
        return msgnum;
    }

    public void setMsgnum(int msgnum) {
        this.msgnum = msgnum;
    }

    public List<HistoryMessage> getMsgInfo() {
        return msgInfo;
    }

    public void setMsgInfo(List<HistoryMessage> msgInfo) {
        this.msgInfo = msgInfo;
    }

    public String getUuid() {
        return uuid;
    }

    public void setUuid(String uuid) {
        this.uuid = uuid;
    }
}
