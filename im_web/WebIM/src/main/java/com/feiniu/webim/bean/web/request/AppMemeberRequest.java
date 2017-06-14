package com.feiniu.webim.bean.web.request;

import java.util.Collections;
import java.util.Map;

/**
 * Author: wangchen
 * Date: 2015/6/30
 * Contact: 243717042@qq.com
 */
public class AppMemeberRequest {
    private Map body = Collections.emptyMap();
    private String apiVersion;
    private String cityCode;
    private String token;

    public Object getBody() {
        return body;
    }

    public void setBody(Map body) {
        this.body = body;
    }

    public String getApiVersion() {
        return apiVersion;
    }

    public void setApiVersion(String apiVersion) {
        this.apiVersion = apiVersion;
    }

    public String getCityCode() {
        return cityCode;
    }

    public void setCityCode(String cityCode) {
        this.cityCode = cityCode;
    }

    public String getToken() {
        return token;
    }

    public void setToken(String token) {
        this.token = token;
    }
}
