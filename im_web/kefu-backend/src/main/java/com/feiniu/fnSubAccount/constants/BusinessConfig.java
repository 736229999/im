package com.feiniu.fnSubAccount.constants;

/**
 * Created by wangchen on 2015/2/6.
 */
public class BusinessConfig {
    private String merchantServiceURL = "";
    private String storeServiceURL = "";
    private String webIMServiceURL = "";
    private String backendServiceURL = "";
    private String imageServiceURL = "";
    private String imServerIpURL = "";
    private String imServerIpPath = "";
    private String imageAccessUrlList = "";
    private String memcacheMagicNumber = "";
    private String memcacheServerIp = "";
    private String errorPageBasePath;
    private String customerServiceOrganId="";
    private String customerServiceOrganName="";
    private String merchantInfoAPI="";
    private boolean useImApi=true;
    private String imAPLURL="";
    private String merchantInfoURL="";
    private boolean isPreview;
    private String merchantMemberApiServiceName;

    public String getMerchantMemberApiServiceName() {
        return merchantMemberApiServiceName;
    }

    public void setMerchantMemberApiServiceName(String merchantMemberApiServiceName) {
        this.merchantMemberApiServiceName = merchantMemberApiServiceName;
    }

    public String getImAPLURL() {
        return imAPLURL;
    }

    public void setImAPLURL(String imAPLURL) {
        this.imAPLURL = imAPLURL;
    }

    public boolean isUseImApi() {
        return useImApi;
    }

    public void setUseImApi(boolean useImApi) {
        this.useImApi = useImApi;
    }

    public String getMerchantInfoAPI() {
        return merchantInfoAPI;
    }

    public void setMerchantInfoAPI(String merchantInfoAPI) {
        this.merchantInfoAPI = merchantInfoAPI;
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

    public String getImServerIpURL() {
        return imServerIpURL;
    }

    public void setImServerIpURL(String imServerIpURL) {
        this.imServerIpURL = imServerIpURL;
    }

    private String storeShowPlaceConfigName = "";

    public String getBackendServiceURL() {
        return backendServiceURL;
    }

    public void setBackendServiceURL(String backendServiceURL) {
        this.backendServiceURL = backendServiceURL;
    }

    public String getMerchantServiceURL() {
        return merchantServiceURL;
    }

    public void setMerchantServiceURL(String merchantServiceURL) {
        this.merchantServiceURL = merchantServiceURL;
    }

    public String getStoreServiceURL() {
        return storeServiceURL;
    }

    public void setStoreServiceURL(String storeServiceURL) {
        this.storeServiceURL = storeServiceURL;
    }

    public String getWebIMServiceURL() {
        return webIMServiceURL;
    }

    public void setWebIMServiceURL(String webIMServiceURL) {
        this.webIMServiceURL = webIMServiceURL;
    }

    public String getStoreShowPlaceConfigName() {
        return storeShowPlaceConfigName;
    }

    public void setStoreShowPlaceConfigName(String storeShowPlaceConfigName) {
        this.storeShowPlaceConfigName = storeShowPlaceConfigName;
    }

    public String getImageServiceURL() {
        return imageServiceURL;
    }

    public void setImageServiceURL(String imageServiceURL) {
        this.imageServiceURL = imageServiceURL;
    }

    public String getImageAccessUrlList() {
        return imageAccessUrlList;
    }

    public void setImageAccessUrlList(String imageAccessUrlList) {
        this.imageAccessUrlList = imageAccessUrlList;
    }

    public String getMemcacheMagicNumber() {
        return memcacheMagicNumber;
    }

    public void setMemcacheMagicNumber(String memcacheMagicNumber) {
        this.memcacheMagicNumber = memcacheMagicNumber;
    }

    public String getMemcacheServerIp() {
        return memcacheServerIp;
    }

    public void setMemcacheServerIp(String memcacheServerIp) {
        this.memcacheServerIp = memcacheServerIp;
    }

    public String getErrorPageBasePath() {
        return errorPageBasePath;
    }

    public void setErrorPageBasePath(String errorPageBasePath) {
        this.errorPageBasePath = errorPageBasePath;
    }

    public boolean isPreview() {
        return isPreview;
    }

    public void setIsPreview(boolean isPreview) {
        this.isPreview = isPreview;
    }

    public String getMerchantInfoURL() {
        return merchantInfoURL;
    }

    public void setMerchantInfoURL(String merchantInfoURL) {
        this.merchantInfoURL = merchantInfoURL;
    }

    public String getImServerIpPath() {
        return imServerIpPath;
    }

    public void setImServerIpPath(String imServerIpPath) {
        this.imServerIpPath = imServerIpPath;
    }
}
