package com.feiniu.fnSubAccount.bean.SessionInfo;

import java.math.BigDecimal;

/**
 * Created by wangzhen on 2016/4/15.
 */
public class SessionCountInfo {
    private BigDecimal sessionTime;
    private BigDecimal sessionCount;

    public BigDecimal getSessionTime() {
        return sessionTime;
    }

    public void setSessionTime(BigDecimal sessionTime) {
        this.sessionTime = sessionTime;
    }

    public BigDecimal getSessionCount() {
        return sessionCount;
    }

    public void setSessionCount(BigDecimal sessionCount) {
        this.sessionCount = sessionCount;
    }
}
