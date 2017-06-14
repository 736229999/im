package com.cn.feiniu.logManagement.bean.dto;

import javax.persistence.Column;
import javax.persistence.EmbeddedId;
import javax.persistence.Entity;
import javax.persistence.Table;

/**
 * Author: morningking
 * Date: 2015/7/30
 * Contact: 243717042@qq.com
 */
@Entity
@Table(name = "IM_SUBMERCHANT_ATTENDANCE")
public class SubMerchantAttendanceDetail {
    @EmbeddedId
    private SubMerchantAttendanceDetailKey subMerchantAttendanceDetailKey;

    @Column(name = "i_login_count")
    private int loginCount;

    @Column(name = "i_first_online_time")
    private int firstOnlineTime;

    @Column(name = "i_last_online_time")
    private int lastOnlineTime;

    @Column(name = "i_total_online_duration")
    private int totalOnlineDuration;

    @Column(name = "i_total_hanging_duration")
    private int totalHangingDuration;

    @Column(name = "i_total_offline_duration")
    private int totalOfflineDuration;

    @Column(name = "i_total_leave_num")
    private int totalLeaveNum;

    @Column(name = "i_total_leave_duration")
    private int totalLeaveDuration;

    public SubMerchantAttendanceDetailKey getSubMerchantAttendanceDetailKey() {
        return subMerchantAttendanceDetailKey;
    }

    public void setSubMerchantAttendanceDetailKey(SubMerchantAttendanceDetailKey subMerchantAttendanceDetailKey) {
        this.subMerchantAttendanceDetailKey = subMerchantAttendanceDetailKey;
    }

    public int getLoginCount() {
        return loginCount;
    }

    public void setLoginCount(int loginCount) {
        this.loginCount = loginCount;
    }

    public int getFirstOnlineTime() {
        return firstOnlineTime;
    }

    public void setFirstOnlineTime(int firstOnlineTime) {
        this.firstOnlineTime = firstOnlineTime;
    }

    public int getLastOnlineTime() {
        return lastOnlineTime;
    }

    public void setLastOnlineTime(int lastOnlineTime) {
        this.lastOnlineTime = lastOnlineTime;
    }

    public int getTotalOnlineDuration() {
        return totalOnlineDuration;
    }

    public void setTotalOnlineDuration(int totalOnlineDuration) {
        this.totalOnlineDuration = totalOnlineDuration;
    }

    public int getTotalHangingDuration() {
        return totalHangingDuration;
    }

    public void setTotalHangingDuration(int totalHangingDuration) {
        this.totalHangingDuration = totalHangingDuration;
    }

    public int getTotalOfflineDuration() {
        return totalOfflineDuration;
    }

    public void setTotalOfflineDuration(int totalOfflineDuration) {
        this.totalOfflineDuration = totalOfflineDuration;
    }

    public int getTotalLeaveNum() {
        return totalLeaveNum;
    }

    public void setTotalLeaveNum(int totalLeaveNum) {
        this.totalLeaveNum = totalLeaveNum;
    }

    public int getTotalLeaveDuration() {
        return totalLeaveDuration;
    }

    public void setTotalLeaveDuration(int totalLeaveDuration) {
        this.totalLeaveDuration = totalLeaveDuration;
    }
}
