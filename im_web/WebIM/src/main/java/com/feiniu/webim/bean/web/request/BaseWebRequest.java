package com.feiniu.webim.bean.web.request;

import com.feiniu.webim.core.Session;

/**
 * Created by wangchen on 2015/3/10.
 */
public class BaseWebRequest {
    private Session session;

    public BaseWebRequest() {
        this(null);
    }

    public BaseWebRequest(Session session) {
        this.session = session;
    }

    public Session getSession(){
        return session;
    }

    public void setSession(Session session) {
        this.session = session;
    }
}
