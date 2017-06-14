package com.feiniu.imServer.constants;

/**
 * Created by wangzhen on 2016/3/15.
 */
public class BackendSetting {
    private String customerServiceOrganId="";
    private String customerServiceOrganName="";
    private String backendServiceURL = "";
    private String imLoggerServiceURL = "";


    public String getImLoggerServiceURL() {
        return imLoggerServiceURL;
    }

    public void setImLoggerServiceURL(String imLoggerServiceURL) {
        this.imLoggerServiceURL = imLoggerServiceURL;
    }

    public String getBackendServiceURL() {
        return backendServiceURL;
    }

    public void setBackendServiceURL(String backendServiceURL) {
        this.backendServiceURL = backendServiceURL;
    }

    public String getCustomerServiceOrganId() {
        return customerServiceOrganId;
    }

    public void setCustomerServiceOrganId(String customerServiceOrganId) {
        this.customerServiceOrganId = customerServiceOrganId;
    }

    public String getCustomerServiceOrganName() {
        return customerServiceOrganName;
    }

    public void setCustomerServiceOrganName(String customerServiceOrganName) {
        this.customerServiceOrganName = customerServiceOrganName;
    }
}
