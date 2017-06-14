package com.feiniu.fnSubAccount.domain;

import javax.persistence.Column;
import javax.persistence.Embeddable;
import java.io.Serializable;

/**
 * Created by wangzhen on 2015/6/23.
 */
@Embeddable
public class WorkResultKey implements Serializable {

    private static final long serialVersionUID = 2393640434179857271L;
    @Column(name = "MERCHANT_ID", nullable = false)
    private long merchantID;

    @Column(name = "STATISTICS_DATE", nullable = false)
    private int date;

    @Override
    public boolean equals(Object obj) {
        if (obj instanceof WorkResultKey) {
            WorkResultKey workPk = (WorkResultKey) obj;
            if (!(workPk.getMerchantID() == merchantID)) {
                return false;
            }
            if (!(workPk.getDate() == date)) {
                return false;
            }
            return true;
        }
        return false;
    }

    @Override
    public int hashCode() {
        return String.valueOf(merchantID).hashCode() + String.valueOf(date).hashCode();
    }

    public long getMerchantID() {
        return merchantID;
    }

    public void setMerchantID(long merchantID) {
        this.merchantID = merchantID;
    }

    public int getDate() {
        return date;
    }

    public void setDate(int date) {
        this.date = date;
    }
}
