package com.feiniu.subAccount.constants;

/**
 * Created by wangchen on 2015/2/6.
 */
public class BusinessConfig {
    private String merchantServiceURL = "";
    private String storeServiceURL = "";
    private String webIMServiceURL = "";
    private String version;
    private String storeShowPlaceConfigName = "";
    private String errorPageBasePath;
    private String magicNumber="";
    private boolean useImApi=true;
    private String imAPLURL="";
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

    public String getMagicNumber() {
        return magicNumber;
    }

    public void setMagicNumber(String magicNumber) {
        this.magicNumber = magicNumber;
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

    public String getVersion() {
        return version;
    }

    public void setVersion(String version) {
        this.version = version;
    }

    public String getErrorPageBasePath() {
        return errorPageBasePath;
    }

    public void setErrorPageBasePath(String errorPageBasePath) {
        this.errorPageBasePath = errorPageBasePath;
    }
}
