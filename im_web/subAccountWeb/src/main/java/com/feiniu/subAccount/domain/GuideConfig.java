package com.feiniu.subAccount.domain;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * Created by wangchen on 2015/5/27.
 */
@Entity
@Table(name = "t_guide_config")
public class GuideConfig {
    @Id
    @Column(name = "d_merchant_id", nullable = false)
    private long merchantID;

    @Column(name = "s_version", nullable = false)
    private String version;

    @Column(name = "b_has_guided", nullable = false)
    private char hasGuided = 'N';

    public long getMerchantID() {
        return merchantID;
    }

    public void setMerchantID(long merchantID) {
        this.merchantID = merchantID;
    }

    public char getHasGuided() {
        return hasGuided;
    }

    public void setHasGuided(char hasGuided) {
        this.hasGuided = hasGuided;
    }

    public String getVersion() {
        return version;
    }

    public void setVersion(String version) {
        this.version = version;
    }
}
