package com.feiniu.imServer.bean;

/**
 * Created by wangzhen on 2016/1/29.
 */
public class ShuntRequestPrincipal {
    private Integer type;
    private Long merchantId;
    private Long customerId;
    private Long groupId;
    private Integer userLevel;
    private Long requestTime;
    private Integer fromDevice;
    private Integer fromPage;


    public Integer getFromDevice() {
        return fromDevice;
    }

    public void setFromDevice(Integer fromDevice) {
        this.fromDevice = fromDevice;
    }

    public Integer getFromPage() {
        return fromPage;
    }

    public void setFromPage(Integer fromPage) {
        this.fromPage = fromPage;
    }

    public Integer getType() {
        return type;
    }

    public void setType(Integer type) {
        this.type = type;
    }

    public Long getMerchantId() {
        return merchantId;
    }

    public void setMerchantId(Long merchantId) {
        this.merchantId = merchantId;
    }

    public Long getCustomerId() {
        return customerId;
    }

    public void setCustomerId(Long customerId) {
        this.customerId = customerId;
    }

    public Long getGroupId() {
        return groupId;
    }

    public void setGroupId(Long groupId) {
        this.groupId = groupId;
    }

    public Integer getUserLevel() {
        return userLevel;
    }

    public void setUserLevel(Integer userLevel) {
        this.userLevel = userLevel;
    }

    public Long getRequestTime() {
        return requestTime;
    }

    public void setRequestTime(Long requestTime) {
        this.requestTime = requestTime;
    }
}
