package com.feiniu.fnSubAccount.domain;

import javax.persistence.Column;
import javax.persistence.EmbeddedId;

/**
 * Created by wangchen on 2015/2/4.
 */

public class WorkCountResult {

    @EmbeddedId
    private WorkResultKey workResultKey;

    @Column(name = "LANDING_TIME", nullable = false)
    private float landingTime;

    @Column(name = "SERVE_COUNT", nullable = false)
    private int serveCount;

    @Column(name = "RESPONSE_TIME", nullable = false)
    private int responseTime;

    @Column(name = "RESPONSE_IN_90S_COUNT", nullable = false)
    private int responseIn90sCount;

    @Column(name = "RESPONSE_COUNT", nullable = false)
    private int responseCount;

    @Column(name = "SUB_MERCHANT_IDS", nullable = true)
    private String subMerchantIds;

    @Column(name = "CUSTOMER_IDS", nullable = true)
    private String customerIds;

    public WorkResultKey getWorkResultKey() {
        return workResultKey;
    }

    public void setWorkResultKey(WorkResultKey workResultKey) {
        this.workResultKey = workResultKey;
    }

    public float getLandingTime() {
        return landingTime;
    }

    public void setLandingTime(float landingTime) {
        this.landingTime = landingTime;
    }

    public int getServeCount() {
        return serveCount;
    }

    public void setServeCount(int serveCount) {
        this.serveCount = serveCount;
    }

    public int getResponseTime() {
        return responseTime;
    }

    public void setResponseTime(int responseTime) {
        this.responseTime = responseTime;
    }

    public int getResponseIn90sCount() {
        return responseIn90sCount;
    }

    public void setResponseIn90sCount(int responseIn90sCount) {
        this.responseIn90sCount = responseIn90sCount;
    }

    public int getResponseCount() {
        return responseCount;
    }

    public void setResponseCount(int responseCount) {
        this.responseCount = responseCount;
    }

    public String getSubMerchantIds() {
        return subMerchantIds;
    }

    public void setSubMerchantIds(String subMerchantIds) {
        this.subMerchantIds = subMerchantIds;
    }

    public String getCustomerIds() {
        return customerIds;
    }

    public void setCustomerIds(String customerIds) {
        this.customerIds = customerIds;
    }
}
