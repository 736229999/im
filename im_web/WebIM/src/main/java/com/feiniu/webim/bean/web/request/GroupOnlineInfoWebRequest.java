package com.feiniu.webim.bean.web.request;

import com.feiniu.webim.core.Session;

import java.util.List;

/**
 * Created by wangchen on 2015/3/10.
 */
public class GroupOnlineInfoWebRequest extends BaseWebRequest {
    private int senderID;
    private List<Long> groupIDList;

    public GroupOnlineInfoWebRequest() {
        super();
    }

    public GroupOnlineInfoWebRequest(Session session) {
        super(session);
    }

    public int getSenderID() {
        return senderID;
    }

    public void setSenderID(int senderID) {
        this.senderID = senderID;
    }

    public List<Long> getGroupIDList() {
        return groupIDList;
    }

    public void setGroupIDList(List<Long> groupIDList) {
        this.groupIDList = groupIDList;
    }
}
