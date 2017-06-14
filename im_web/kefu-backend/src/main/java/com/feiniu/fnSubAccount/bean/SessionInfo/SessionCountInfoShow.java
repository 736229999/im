package com.feiniu.fnSubAccount.bean.SessionInfo;

/**
 * Created by wangzhen on 2016/4/15.
 */
public class SessionCountInfoShow {
    private String time;
    private Integer sessionCount;
    private String sessionRate;

    public String getTime() {
        return time;
    }

    public void setTime(String time) {
        this.time = time;
    }

    public Integer getSessionCount() {
        return sessionCount;
    }

    public void setSessionCount(Integer sessionCount) {
        this.sessionCount = sessionCount;
    }

    public String getSessionRate() {
        return sessionRate;
    }

    public void setSessionRate(String sessionRate) {
        this.sessionRate = sessionRate;
    }
}
