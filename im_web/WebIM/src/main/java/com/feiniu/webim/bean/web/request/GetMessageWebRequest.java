package com.feiniu.webim.bean.web.request;

import com.feiniu.webim.core.Session;

/**
 * Created by wangchen on 2015/3/11.
 */
public class GetMessageWebRequest extends BaseWebRequest {
    private int did;
    private String token;
    private String uuid;

    public GetMessageWebRequest() {
        super();
    }

    public GetMessageWebRequest(Session session) {
        super(session);
    }

    public int getDid() {
        return did;
    }

    public void setDid(int did) {
        this.did = did;
    }

    public String getToken() {
        return token;
    }

    public void setToken(String token) {
        this.token = token;
    }

    public String getUuid() {
        return uuid;
    }

    public void setUuid(String uuid) {
        this.uuid = uuid;
    }

}
