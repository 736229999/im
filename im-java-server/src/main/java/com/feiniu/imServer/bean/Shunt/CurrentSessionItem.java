package com.feiniu.imServer.bean.Shunt;

import java.math.BigDecimal;

/**
 * Created by wangzhen on 2016/4/20.
 */
public class CurrentSessionItem {
    private BigDecimal sessionId;
    private BigDecimal serviceId;
    private BigDecimal customerId;

    public BigDecimal getSessionId() {
        return sessionId;
    }

    public void setSessionId(BigDecimal sessionId) {
        this.sessionId = sessionId;
    }

    public BigDecimal getServiceId() {
        return serviceId;
    }

    public void setServiceId(BigDecimal serviceId) {
        this.serviceId = serviceId;
    }

    public BigDecimal getCustomerId() {
        return customerId;
    }

    public void setCustomerId(BigDecimal customerId) {
        this.customerId = customerId;
    }
}
