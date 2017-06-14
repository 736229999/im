package com.feiniu.fnSubAccount.domain;

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
@Entity
@Table(name = "t_kefu_index_config")
public class KefuIndexConfig implements Cloneable {
    @Column(name = "update_time")
    private Timestamp updateTime;

    @Id
    @Column(name = "i_config_id")
    private int configID;

    @Column(name = "v_notice")
    private String notice;

    @Column(name = "v_ad_banner_left_image_url")
    private String adBannerLeftImageUrl;

    @Column(name = "v_ad_banner_left_image_href")
    private String adBannerLeftImageHref;

    @Column(name = "v_ad_banner_right_image_url")
    private String adBannerRightImageUrl;

    @Column(name = "v_ad_banner_right_image_href")
    private String adBannerRightImageHref;

    @Column(name = "c_data_type")
    private char dataType;

    @Column(name = "v_json_of_feiniu_index", columnDefinition = "CLOB")
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

    @Override
    public Object clone() throws CloneNotSupportedException {
        return super.clone();
    }
}
