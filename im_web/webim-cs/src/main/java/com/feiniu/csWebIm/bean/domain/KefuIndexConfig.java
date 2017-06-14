package com.feiniu.csWebIm.bean.domain;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;
import java.sql.Timestamp;

/**
 * Author: morningking
 * Date: 2015/10/14
 * Contact: 243717042@qq.com
 */
public class KefuIndexConfig {
    private Timestamp updateTime;
    private int configID;
    private String notice;
    private String adBannerLeftImageUrl;
    private String adBannerLeftImageHref;
    private String adBannerRightImageUrl;
    private String adBannerRightImageHref;
    private char dataType;
    private String jsonOfFeiniuIndex;

    public Timestamp getUpdateTime() {
        return updateTime;
    }

    public void setUpdateTime(Timestamp updateTime) {
        this.updateTime = updateTime;
    }

    public int getConfigID() {
        return configID;
    }

    public void setConfigID(int configID) {
        this.configID = configID;
    }

    public String getNotice() {
        return notice;
    }

    public void setNotice(String notice) {
        this.notice = notice;
    }

    public String getAdBannerLeftImageUrl() {
        return adBannerLeftImageUrl;
    }

    public void setAdBannerLeftImageUrl(String adBannerLeftImageUrl) {
        this.adBannerLeftImageUrl = adBannerLeftImageUrl;
    }

    public String getAdBannerLeftImageHref() {
        return adBannerLeftImageHref;
    }

    public void setAdBannerLeftImageHref(String adBannerLeftImageHref) {
        this.adBannerLeftImageHref = adBannerLeftImageHref;
    }

    public String getAdBannerRightImageUrl() {
        return adBannerRightImageUrl;
    }

    public void setAdBannerRightImageUrl(String adBannerRightImageUrl) {
        this.adBannerRightImageUrl = adBannerRightImageUrl;
    }

    public String getAdBannerRightImageHref() {
        return adBannerRightImageHref;
    }

    public void setAdBannerRightImageHref(String adBannerRightImageHref) {
        this.adBannerRightImageHref = adBannerRightImageHref;
    }

    public char getDataType() {
        return dataType;
    }

    public void setDataType(char dataType) {
        this.dataType = dataType;
    }

    public String getJsonOfFeiniuIndex() {
        return jsonOfFeiniuIndex;
    }

    public void setJsonOfFeiniuIndex(String jsonOfFeiniuIndex) {
        this.jsonOfFeiniuIndex = jsonOfFeiniuIndex;
    }
}
