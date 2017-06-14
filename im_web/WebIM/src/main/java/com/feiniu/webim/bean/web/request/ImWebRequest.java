package com.feiniu.webim.bean.web.request;

/**
 * Author: wangchen
 * Date: 2015/7/27
 * Contact: 243717042@qq.com
 */
public class ImWebRequest {
    private String version;
    private String method;
    private String params;

    public String getVersion() {
        return version;
    }

    public void setVersion(String version) {
        this.version = version;
    }

    public String getMethod() {
        return method;
    }

    public void setMethod(String method) {
        this.method = method;
    }

    public String getParams() {
        return params;
    }

    public void setParams(String params) {
        this.params = params;
    }
}
