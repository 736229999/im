package com.feiniu.webim.bean.web.request;

import com.feiniu.webim.core.Session;

/**
 * Created by wangchen on 2015/3/10.
 */
public class GetRecentlyContactWebRequest extends BaseWebRequest {
    private int senderID;
    private int msgNum;

    public GetRecentlyContactWebRequest() {
        super();
    }

    public GetRecentlyContactWebRequest(Session session) {
        super(session);
    }

    public int getSenderID() {
        return senderID;
    }

    public void setSenderID(int senderID) {
        this.senderID = senderID;
    }

    public int getMsgNum() {
        return msgNum;
    }

    public void setMsgNum(int msgNum) {
        this.msgNum = msgNum;
    }
}
