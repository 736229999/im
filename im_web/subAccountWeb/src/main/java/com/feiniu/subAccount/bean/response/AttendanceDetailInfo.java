package com.feiniu.subAccount.bean.response;

/**
 * Created by wangzhen on 2015/7/22.
 */
public class AttendanceDetailInfo implements Comparable<AttendanceDetailInfo> {
    private long subMerchantId;
    private int landingCount;
    //SQL查询出来的结果,类型与要显示的结果又出入的项目
    private long sqlAttendanceDate;
    private long sqlAttendanceFirstOnlineTime;
    private long sqlAttendanceLastOnlineTime;
    private long sqlAttendanceLandingTimeSum;
    private long sqlAttendanceHungupTimeSum;
    private long sqlAttendanceOfflineTimeSum;

    //要显示出来的结果
    private String date;
    private String firstOnlineTime;
    private String lastOnlineTime;
    private String landingTimeSum;
    private String hungUpTimeSum;
    private String offlineTimeSum;
    private String onlineRate;

    public long getSubMerchantId() {
        return subMerchantId;
    }

    public void setSubMerchantId(long subMerchantId) {
        this.subMerchantId = subMerchantId;
    }

    public int getLandingCount() {
        return landingCount;
    }

    public void setLandingCount(int landingCount) {
        this.landingCount = landingCount;
    }

    public long getSqlAttendanceDate() {
        return sqlAttendanceDate;
    }

    public void setSqlAttendanceDate(long sqlAttendanceDate) {
        this.sqlAttendanceDate = sqlAttendanceDate;
    }

    public long getSqlAttendanceFirstOnlineTime() {
        return sqlAttendanceFirstOnlineTime;
    }

    public void setSqlAttendanceFirstOnlineTime(long sqlAttendanceFirstOnlineTime) {
        this.sqlAttendanceFirstOnlineTime = sqlAttendanceFirstOnlineTime;
    }

    public long getSqlAttendanceLastOnlineTime() {
        return sqlAttendanceLastOnlineTime;
    }

    public void setSqlAttendanceLastOnlineTime(long sqlAttendanceLastOnlineTime) {
        this.sqlAttendanceLastOnlineTime = sqlAttendanceLastOnlineTime;
    }

    public long getSqlAttendanceLandingTimeSum() {
        return sqlAttendanceLandingTimeSum;
    }

    public void setSqlAttendanceLandingTimeSum(long sqlAttendanceLandingTimeSum) {
        this.sqlAttendanceLandingTimeSum = sqlAttendanceLandingTimeSum;
    }

    public long getSqlAttendanceHungupTimeSum() {
        return sqlAttendanceHungupTimeSum;
    }

    public void setSqlAttendanceHungupTimeSum(long sqlAttendanceHungupTimeSum) {
        this.sqlAttendanceHungupTimeSum = sqlAttendanceHungupTimeSum;
    }

    public long getSqlAttendanceOfflineTimeSum() {
        return sqlAttendanceOfflineTimeSum;
    }

    public void setSqlAttendanceOfflineTimeSum(long sqlAttendanceOfflineTimeSum) {
        this.sqlAttendanceOfflineTimeSum = sqlAttendanceOfflineTimeSum;
    }

    public String getDate() {
        return date;
    }

    public void setDate(String date) {
        this.date = date;
    }

    public String getFirstOnlineTime() {
        return firstOnlineTime;
    }

    public void setFirstOnlineTime(String firstOnlineTime) {
        this.firstOnlineTime = firstOnlineTime;
    }

    public String getLastOnlineTime() {
        return lastOnlineTime;
    }

    public void setLastOnlineTime(String lastOnlineTime) {
        this.lastOnlineTime = lastOnlineTime;
    }

    public String getLandingTimeSum() {
        return landingTimeSum;
    }

    public void setLandingTimeSum(String landingTimeSum) {
        this.landingTimeSum = landingTimeSum;
    }

    public String getHungUpTimeSum() {
        return hungUpTimeSum;
    }

    public void setHungUpTimeSum(String hungUpTimeSum) {
        this.hungUpTimeSum = hungUpTimeSum;
    }

    public String getOfflineTimeSum() {
        return offlineTimeSum;
    }

    public void setOfflineTimeSum(String offlineTimeSum) {
        this.offlineTimeSum = offlineTimeSum;
    }

    public String getOnlineRate() {
        return onlineRate;
    }

    public void setOnlineRate(String onlineRate) {
        this.onlineRate = onlineRate;
    }


    @Override
    public int compareTo(AttendanceDetailInfo o) {
        return ((Long)(this.sqlAttendanceDate - o.getSqlAttendanceDate() )).intValue();
    }
}
