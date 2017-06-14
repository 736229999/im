package com.feiniu.subAccount.domain;

import javax.persistence.*;

/**
 * Created by wangchen on 2015/1/29.
 */
@Entity
@Table(name = "t_merchant_diverse_config")
public class MerchantDiverseConfig {
    @Id
    @Column(name = "m_merchant_id", unique = true, nullable = false)
    private long merchantID;

    @Column(name = "m_proxy_subaccount_id", nullable = false)
    private long proxySubAccountID = -1l;

    @OneToOne
    @JoinColumn(name = "m_diverse_type", nullable = false)
    private DiverseType diverseType;

    public long getMerchantID() {
        return merchantID;
    }

    public void setMerchantID(long merchantID) {
        this.merchantID = merchantID;
    }

    public DiverseType getDiverseType() {
        return diverseType;
    }

    public void setDiverseType(DiverseType diverseType) {
        this.diverseType = diverseType;
    }

    public long getProxySubAccountID() {
        return proxySubAccountID;
    }

    public void setProxySubAccountID(long proxySubAccountID) {
        this.proxySubAccountID = proxySubAccountID;
    }
}
