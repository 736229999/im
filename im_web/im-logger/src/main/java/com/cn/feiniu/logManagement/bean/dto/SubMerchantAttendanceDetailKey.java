package com.cn.feiniu.logManagement.bean.dto;

import javax.persistence.Column;
import javax.persistence.Embeddable;
import java.io.Serializable;
import java.util.Objects;

/**
 * Author: morningking
 * Date: 2015/8/4
 * Contact: 243717042@qq.com
 */
@Embeddable
public class SubMerchantAttendanceDetailKey implements Serializable{
    private static final long serialVersionUID = 727300180480203380L;

    @Column(name = "i_submerchant_id", nullable = false)
    private long subMerchantID;

    @Column(name = "i_merchant_id", nullable = false)
    private long merchantID;

    @Column(name = "i_statistics_time", nullable = false)
    private int statisticsTime;

    public long getSubMerchantID() {
        return subMerchantID;
    }

    public void setSubMerchantID(long subMerchantID) {
        this.subMerchantID = subMerchantID;
    }

    public long getMerchantID() {
        return merchantID;
    }

    public void setMerchantID(long merchantID) {
        this.merchantID = merchantID;
    }

    public int getStatisticsTime() {
        return statisticsTime;
    }

    public void setStatisticsTime(int statisticsTime) {
        this.statisticsTime = statisticsTime;
    }

    @Override
    public boolean equals(Object obj) {
        if (obj != null && obj instanceof SubMerchantAttendanceDetailKey) {
            SubMerchantAttendanceDetailKey other = (SubMerchantAttendanceDetailKey) obj;

            return other.subMerchantID == subMerchantID
                    && other.merchantID == merchantID
                    && other.statisticsTime == statisticsTime;
        } else {
            return false;
        }
    }

    @Override
    public int hashCode() {
        return Objects.hashCode(subMerchantID) ^ Objects.hashCode(merchantID) ^ Objects.hashCode(statisticsTime);
    }
}
