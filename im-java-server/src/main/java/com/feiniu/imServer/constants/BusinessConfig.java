package com.feiniu.imServer.constants;

/**
 * Created by wangchen on 2015/2/6.
 */
public class BusinessConfig {

    private String topicWebImShunt;
    private String topicImServerShunt;
    private String magicNumber;
    private String memcacheServerIp = "";
    private String merchantServiceURL = "";
    private String imServerUrl="";
    private String merchantMemberApiServiceName;

    public String getMerchantMemberApiServiceName() {
        return merchantMemberApiServiceName;
    }

    public void setMerchantMemberApiServiceName(String merchantMemberApiServiceName) {
        this.merchantMemberApiServiceName = merchantMemberApiServiceName;
    }

    public String getImServerUrl() {
        return imServerUrl;
    }

    public void setImServerUrl(String imServerUrl) {
        this.imServerUrl = imServerUrl;
    }

    public String getMerchantServiceURL() {
        return merchantServiceURL;
    }

    public void setMerchantServiceURL(String merchantServiceURL) {
        this.merchantServiceURL = merchantServiceURL;
    }

    public String getMemcacheServerIp() {
        return memcacheServerIp;
    }

    public void setMemcacheServerIp(String memcacheServerIp) {
        this.memcacheServerIp = memcacheServerIp;
    }

    public String getMagicNumber() {
        return magicNumber;
    }

    public void setMagicNumber(String magicNumber) {
        this.magicNumber = magicNumber;
    }

    public String getTopicImServerShunt() {
        return topicImServerShunt;
    }

    public void setTopicImServerShunt(String topicImServerShunt) {
        this.topicImServerShunt = topicImServerShunt;
    }

    public String getTopicWebImShunt() {
        return topicWebImShunt;
    }

    public void setTopicWebImShunt(String topicWebImShunt) {
        this.topicWebImShunt = topicWebImShunt;
    }
}
