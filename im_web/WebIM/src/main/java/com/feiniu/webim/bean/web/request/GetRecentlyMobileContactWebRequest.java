package com.feiniu.webim.bean.web.request;

import com.feiniu.webim.core.Session;

/**
 * Created by wangyixiong on 2015/11/11.
 */
public class GetRecentlyMobileContactWebRequest extends BaseWebRequest {
    private int senderID;

    public GetRecentlyMobileContactWebRequest() {
        super();
    }

    public GetRecentlyMobileContactWebRequest(Session session) {
        super(session);
    }

    public int getSenderID() {
        return senderID;
    }

    public void setSenderID(int senderID) {
        this.senderID = senderID;
    }

}
