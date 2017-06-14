package com.feiniu.csWebIm.consts;

import javax.annotation.PostConstruct;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Created by wangchen on 2015/2/6.
 */
public class BusinessConfig {
    private static final Pattern BASE_URL_LIST_SPLIT_URL_PATTERN = Pattern.compile("^(http://[^\\[]+)\\[((?:\\d+,?)+)\\](.+)$");

    private String memberInfoServiceURL = "";
    private String memberLevelInfoServiceURL = "";
    private String contactInfoServiceURL = "";
    private String cartServiceURL = "";
    private String goodsItemURL = "";
    private String goodsImgURL = "";
    private String employInfoURL = "";
    private String memberServerURL = "";
    private String totalCostURL = "";
    private String recentDealsURL = "";
    private String orderConsultURL = "";
    private String logisticsDetailURL = "";
    private String productDetailURL = "";
    private String serviceMemberURL;
    private String kefuBackendURL;
    private String webimAuthURL;
    private String SaveServicelogURL = "";
    private String AskingQuestionURL = "";
    private String SaveQuestionURL = "";
    private String imServerList = "";
    private String magicNumber="1221";
    //飞牛自营图片根地址
    private String feiniuSelfSupportImgBaseUrl;
    //最新图片接口 服务器地址
    private String newImageUrlBases;
    private String[] newImageUrlBaseArray;
    //订单图片尺寸大小
    private String orderPicSize;
    //webim认证的端口
    private String webImAuthURLPort;
    //错误页跳转
    private String errorCodePageBasePath;
    //灰度测试 因子
    private int grayFactor;
    //灰度测试 幸运号码
    private int grayLuckyNumber;
    //无线接口相关配置项
    private String appMemberMd5EncryptedKey;
    private String appMemberRetriveServiceUrl;
    private String appMemberSiteKey;
	//订单灰度测试 幸运号码
    private int orderGrayLuckyNumber;
    //订单灰度测试 因子
    private int orderGrayFactor;

    public String getMagicNumber() {
        return magicNumber;
    }

    public void setMagicNumber(String magicNumber) {
        this.magicNumber = magicNumber;
    }

    public String getWebImAuthURLPort() {
        return webImAuthURLPort;
    }

    public void setWebImAuthURLPort(String webImAuthURLPort) {
        this.webImAuthURLPort = webImAuthURLPort;
    }

    public String getImServerList() {
        return imServerList;
    }

    public void setImServerList(String imServerList) {
        this.imServerList = imServerList;
    }

    public String getMemberInfoServiceURL() {
        return memberInfoServiceURL;
    }

    public void setMemberInfoServiceURL(String memberInfoServiceURL) {
        this.memberInfoServiceURL = memberInfoServiceURL;
    }

    public String getMemberLevelInfoServiceURL() {
        return memberLevelInfoServiceURL;
    }

    public void setMemberLevelInfoServiceURL(String memberLevelInfoServiceURL) {
        this.memberLevelInfoServiceURL = memberLevelInfoServiceURL;
    }

    public String getContactInfoServiceURL() {
        return contactInfoServiceURL;
    }

    public void setContactInfoServiceURL(String contactInfoServiceURL) {
        this.contactInfoServiceURL = contactInfoServiceURL;
    }

    public String getCartServiceURL() {
        return cartServiceURL;
    }

    public void setCartServiceURL(String cartServiceURL) {
        this.cartServiceURL = cartServiceURL;
    }

    public String getGoodsItemURL() {
        return goodsItemURL;
    }

    public void setGoodsItemURL(String goodsItemURL) {
        this.goodsItemURL = goodsItemURL;
    }

    public String getGoodsImgURL() {
        return goodsImgURL;
    }

    public void setGoodsImgURL(String goodsImgURL) {
        this.goodsImgURL = goodsImgURL;
    }

    public String getEmployInfoURL() {
        return employInfoURL;
    }

    public void setEmployInfoURL(String employInfoURL) {
        this.employInfoURL = employInfoURL;
    }

    public String getMemberServerURL() {
        return memberServerURL;
    }

    public void setMemberServerURL(String memberServerURL) {
        this.memberServerURL = memberServerURL;
    }

    public String getTotalCostURL() {
        return totalCostURL;
    }

    public void setTotalCostURL(String totalCostURL) {
        this.totalCostURL = totalCostURL;
    }

    public String getRecentDealsURL() {
        return recentDealsURL;
    }

    public void setRecentDealsURL(String recentDealsURL) {
        this.recentDealsURL = recentDealsURL;
    }

    public String getOrderConsultURL() {
        return orderConsultURL;
    }

    public void setOrderConsultURL(String orderConsultURL) {
        this.orderConsultURL = orderConsultURL;
    }

    public String getLogisticsDetailURL() {
        return logisticsDetailURL;
    }

    public void setLogisticsDetailURL(String logisticsDetailURL) {
        this.logisticsDetailURL = logisticsDetailURL;
    }

    public String getProductDetailURL() {
        return productDetailURL;
    }

    public void setProductDetailURL(String productDetailURL) {
        this.productDetailURL = productDetailURL;
    }

    public String getServiceMemberURL() {
        return serviceMemberURL;
    }

    public void setServiceMemberURL(String serviceMemberURL) {
        this.serviceMemberURL = serviceMemberURL;
    }

    public String getKefuBackendURL() {
        return kefuBackendURL;
    }

    public void setKefuBackendURL(String kefuBackendURL) {
        this.kefuBackendURL = kefuBackendURL;
    }

    public String getWebimAuthURL() {
        return webimAuthURL;
    }

    public void setWebimAuthURL(String webimAuthURL) {
        this.webimAuthURL = webimAuthURL;
    }

    public String getSaveServicelogURL() {
        return SaveServicelogURL;
    }

    public void setSaveServicelogURL(String saveServicelogURL) {
        SaveServicelogURL = saveServicelogURL;
    }

    public String getAskingQuestionURL() {
        return AskingQuestionURL;
    }

    public void setAskingQuestionURL(String askingQuestionURL) {
        AskingQuestionURL = askingQuestionURL;
    }

    public String getSaveQuestionURL() {
        return SaveQuestionURL;
    }

    public void setSaveQuestionURL(String saveQuestionURL) {
        SaveQuestionURL = saveQuestionURL;
    }

    public String getFeiniuSelfSupportImgBaseUrl() {
        return feiniuSelfSupportImgBaseUrl;
    }

    public void setFeiniuSelfSupportImgBaseUrl(String feiniuSelfSupportImgBaseUrl) {
        this.feiniuSelfSupportImgBaseUrl = feiniuSelfSupportImgBaseUrl;
    }

    public String getNewImageUrlBases() {
        return newImageUrlBases;
    }

    public void setNewImageUrlBases(String newImageUrlBases) {
        this.newImageUrlBases = newImageUrlBases;
    }

    public String getOrderPicSize() {
        return orderPicSize;
    }

    public void setOrderPicSize(String orderPicSize) {
        this.orderPicSize = orderPicSize;
    }

    @PostConstruct
    private void doTokenize() {
        //初始化图片下载根地址
        Matcher matcher = BASE_URL_LIST_SPLIT_URL_PATTERN.matcher(newImageUrlBases);

        //如果是beta以上环境
        if (matcher.matches()) {
            String[] numberArray = matcher.group(2).split(",");
            newImageUrlBaseArray = new String[numberArray.length];

            for (int i = 0; i < numberArray.length; i++) {
                newImageUrlBaseArray[i] = matcher.group(1) + numberArray[i] + matcher.group(3);
            }
        } else {
            newImageUrlBaseArray = new String[]{newImageUrlBases};
        }
    }

    public String getRandomUrlBase() {
        int index = 0;
        if (newImageUrlBaseArray.length > 1) {
            index = (int) Math.floor(Math.random() * newImageUrlBaseArray.length);
        }

        return newImageUrlBaseArray[index];
    }

    public String getErrorIndexPath() {
        return errorCodePageBasePath;
    }

    public void setErrorIndexPath(String errorIndexPath) {
        this.errorCodePageBasePath = errorIndexPath;
    }

    public int getGrayFactor() {
        return grayFactor;
    }

    public void setGrayFactor(int grayFactor) {
        this.grayFactor = grayFactor;
    }

    public int getGrayLuckyNumber() {
        return grayLuckyNumber;
    }

    public void setGrayLuckyNumber(int grayLuckyNumber) {
        this.grayLuckyNumber = grayLuckyNumber;
    }

	public int getOrderGrayLuckyNumber() {
        return orderGrayLuckyNumber;
    }

    public void setOrderGrayLuckyNumber(int orderGrayLuckyNumber) {
        this.orderGrayLuckyNumber = orderGrayLuckyNumber;
    }

    public int getOrderGrayFactor() {
        return orderGrayFactor;
    }

    public void setOrderGrayFactor(int orderGrayFactor) {
        this.orderGrayFactor = orderGrayFactor;
    }

    public String getAppMemberMd5EncryptedKey() {
        return appMemberMd5EncryptedKey;
    }

    public void setAppMemberMd5EncryptedKey(String appMemberMd5EncryptedKey) {
        this.appMemberMd5EncryptedKey = appMemberMd5EncryptedKey;
    }

    public String getAppMemberRetriveServiceUrl() {
        return appMemberRetriveServiceUrl;
    }

    public void setAppMemberRetriveServiceUrl(String appMemberRetriveServiceUrl) {
        this.appMemberRetriveServiceUrl = appMemberRetriveServiceUrl;
    }

    public String getAppMemberSiteKey() {
        return appMemberSiteKey;
    }

    public void setAppMemberSiteKey(String appMemberSiteKey) {
        this.appMemberSiteKey = appMemberSiteKey;
    }
}
