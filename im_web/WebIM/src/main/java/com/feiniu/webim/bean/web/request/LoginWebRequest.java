package com.feiniu.webim.bean.web.request;

import com.feiniu.webim.core.Session;

/**
 * Created by wangchen on 2015/3/10.
 */
public class LoginWebRequest extends BaseWebRequest {
    private String sname;
    private String guid;
    private String telephone;
    private String email;
    private Integer deviceid;

    public LoginWebRequest() {
        super();
    }

    public LoginWebRequest(Session session) {
        super(session);
    }

    public String getSname() {
        return sname;
    }

    public void setSname(String sname) {
        this.sname = sname;
    }

    public String getGuid() {
        return guid;
    }

    public void setGuid(String guid) {
        this.guid = guid;
    }

    public String getTelephone() {
        return telephone;
    }

    public void setTelephone(String telephone) {
        this.telephone = telephone;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public Integer getDeviceid() {
        return deviceid;
    }

    public void setDeviceid(Integer deviceid) {
        this.deviceid = deviceid;
    }
}
