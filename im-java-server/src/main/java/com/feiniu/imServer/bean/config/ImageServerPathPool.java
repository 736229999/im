package com.feiniu.imServer.bean.config;

import javax.annotation.PostConstruct;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Author: morningking
 * Date: 2016/2/25
 * Contact: 243717042@qq.com
 */
public class ImageServerPathPool {
    private static final Pattern BASE_URL_LIST_SPLIT_URL_PATTERN = Pattern.compile("^(http://[^\\[]+)\\[((?:\\d+,?)+)\\](.+)$");
    private String imgAccessUrlList;
    //自营图片根地址
    private String selfSupportImgAccessUrl;
    private String[] BASE_URL_LIST;

    @PostConstruct
    private void init() {
        Matcher matcher = BASE_URL_LIST_SPLIT_URL_PATTERN.matcher(imgAccessUrlList);

        //如果是beta以上环境
        if (matcher.matches()) {
            String[] numberArray = matcher.group(2).split(",");
            BASE_URL_LIST = new String[numberArray.length];

            for (int i = 0; i < numberArray.length; i++) {
                BASE_URL_LIST[i] = matcher.group(1) + numberArray[i] + matcher.group(3);
            }
        } else {
            BASE_URL_LIST = new String[]{imgAccessUrlList};
        }
    }

    //随机的获取商城图片下载服务器地址中的一个作为根地址
    public String getRandomMerchantImageServerUrlBase() {
        int index = 0;
        if (BASE_URL_LIST.length > 1) {
            index = (int) Math.floor(Math.random() * BASE_URL_LIST.length);
        }

        return BASE_URL_LIST[index];
    }

    public void setMerchantImgAccessUrlList(String merchantImgAccessUrlList) {
        this.imgAccessUrlList = merchantImgAccessUrlList;
    }

    public String getSelfSupportImgAccessUrl() {
        return selfSupportImgAccessUrl;
    }

    public void setSelfSupportImgAccessUrl(String selfSupportImgAccessUrl) {
        this.selfSupportImgAccessUrl = selfSupportImgAccessUrl;
    }
}
