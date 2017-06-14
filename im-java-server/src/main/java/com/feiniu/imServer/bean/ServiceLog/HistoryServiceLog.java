package com.feiniu.imServer.bean.ServiceLog;

/**
 * Created by wangzhen on 2015/10/21.
 */
public class HistoryServiceLog {

    private String time;
    private String serviceName;
    private String detail;


    public String getTime() {
        return time;
    }

    public void setTime(String time) {
        this.time = time;
    }

    public String getServiceName() {
        return serviceName;
    }

    public void setServiceName(String serviceName) {
        this.serviceName = serviceName;
    }

    public String getDetail() {
        return detail;
    }

    public void setDetail(String detail) {
        this.detail = detail;
    }
}
