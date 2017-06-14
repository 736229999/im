package com.feiniu.fnSubAccount.bean.SessionInfo;

import java.math.BigDecimal;

/**
 * Created by wangzhen on 2016/4/11.
 */
public class SessionQuest {
    private BigDecimal I_CUSTOMER_ID;
    private BigDecimal I_SERVICE_ID;
    private BigDecimal I_SESSION_START_TIME;
    private BigDecimal I_SESSION_END_TIME;

    public BigDecimal getI_CUSTOMER_ID() {
        return I_CUSTOMER_ID;
    }

    public void setI_CUSTOMER_ID(BigDecimal i_CUSTOMER_ID) {
        I_CUSTOMER_ID = i_CUSTOMER_ID;
    }

    public BigDecimal getI_SERVICE_ID() {
        return I_SERVICE_ID;
    }

    public void setI_SERVICE_ID(BigDecimal i_SERVICE_ID) {
        I_SERVICE_ID = i_SERVICE_ID;
    }

    public BigDecimal getI_SESSION_START_TIME() {
        return I_SESSION_START_TIME;
    }

    public void setI_SESSION_START_TIME(BigDecimal i_SESSION_START_TIME) {
        I_SESSION_START_TIME = i_SESSION_START_TIME;
    }

    public BigDecimal getI_SESSION_END_TIME() {
        return I_SESSION_END_TIME;
    }

    public void setI_SESSION_END_TIME(BigDecimal i_SESSION_END_TIME) {
        I_SESSION_END_TIME = i_SESSION_END_TIME;
    }
}
