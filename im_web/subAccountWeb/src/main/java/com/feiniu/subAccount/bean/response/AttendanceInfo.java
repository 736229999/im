package com.feiniu.subAccount.bean.response;

/**
 * Created by wangzhen on 2015/7/22.
 */
public class AttendanceInfo {
    private long subMerchantId;
    private String subMerchantName;
    private int  attendanceDayCount;

    private long sqlAttendanceLandingTimeSum;
    private long sqlAttendanceFirstLandingTimeSum;
    private long sqlAttendanceLastOnlineTimeSum;
    private long sqlAttendanceEarliestLandingTime;
    private long sqlAttendanceLatestOnlineTime;

    private String attendanceTimeSum;
    private String attendanceTimeAvg;
    private String attendanceStartTimeAvg;
    private String attendanceEndTimeAvg;
    private String attendanceEarliestTime;
    private String attendanceLatestTime;

    public long getSubMerchantId() {
        return subMerchantId;
    }

    public void setSubMerchantId(long subMerchantId) {
        this.subMerchantId = subMerchantId;
    }

    public String getSubMerchantName() {
        return subMerchantName;
    }

    public void setSubMerchantName(String subMerchantName) {
        this.subMerchantName = subMerchantName;
    }

    public int getAttendanceDayCount() {
        return attendanceDayCount;
    }

    public void setAttendanceDayCount(int attendanceDayCount) {
        this.attendanceDayCount = attendanceDayCount;
    }

    public long getSqlAttendanceLandingTimeSum() {
        return sqlAttendanceLandingTimeSum;
    }

    public void setSqlAttendanceLandingTimeSum(long sqlAttendanceLandingTimeSum) {
        this.sqlAttendanceLandingTimeSum = sqlAttendanceLandingTimeSum;
    }

    public long getSqlAttendanceFirstLandingTimeSum() {
        return sqlAttendanceFirstLandingTimeSum;
    }

    public void setSqlAttendanceFirstLandingTimeSum(long sqlAttendanceFirstLandingTimeSum) {
        this.sqlAttendanceFirstLandingTimeSum = sqlAttendanceFirstLandingTimeSum;
    }

    public long getSqlAttendanceLastOnlineTimeSum() {
        return sqlAttendanceLastOnlineTimeSum;
    }

    public void setSqlAttendanceLastOnlineTimeSum(long sqlAttendanceLastOnlineTimeSum) {
        this.sqlAttendanceLastOnlineTimeSum = sqlAttendanceLastOnlineTimeSum;
    }

    public long getSqlAttendanceEarliestLandingTime() {
        return sqlAttendanceEarliestLandingTime;
    }

    public void setSqlAttendanceEarliestLandingTime(long sqlAttendanceEarliestLandingTime) {
        this.sqlAttendanceEarliestLandingTime = sqlAttendanceEarliestLandingTime;
    }

    public long getSqlAttendanceLatestOnlineTime() {
        return sqlAttendanceLatestOnlineTime;
    }

    public void setSqlAttendanceLatestOnlineTime(long sqlAttendanceLatestOnlineTime) {
        this.sqlAttendanceLatestOnlineTime = sqlAttendanceLatestOnlineTime;
    }

    public String getAttendanceTimeSum() {
        return attendanceTimeSum;
    }

    public void setAttendanceTimeSum(String attendanceTimeSum) {
        this.attendanceTimeSum = attendanceTimeSum;
    }

    public String getAttendanceTimeAvg() {
        return attendanceTimeAvg;
    }

    public void setAttendanceTimeAvg(String attendanceTimeAvg) {
        this.attendanceTimeAvg = attendanceTimeAvg;
    }

    public String getAttendanceStartTimeAvg() {
        return attendanceStartTimeAvg;
    }

    public void setAttendanceStartTimeAvg(String attendanceStartTimeAvg) {
        this.attendanceStartTimeAvg = attendanceStartTimeAvg;
    }

    public String getAttendanceEndTimeAvg() {
        return attendanceEndTimeAvg;
    }

    public void setAttendanceEndTimeAvg(String attendanceEndTimeAvg) {
        this.attendanceEndTimeAvg = attendanceEndTimeAvg;
    }

    public String getAttendanceEarliestTime() {
        return attendanceEarliestTime;
    }

    public void setAttendanceEarliestTime(String attendanceEarliestTime) {
        this.attendanceEarliestTime = attendanceEarliestTime;
    }

    public String getAttendanceLatestTime() {
        return attendanceLatestTime;
    }

    public void setAttendanceLatestTime(String attendanceLatestTime) {
        this.attendanceLatestTime = attendanceLatestTime;
    }
}
