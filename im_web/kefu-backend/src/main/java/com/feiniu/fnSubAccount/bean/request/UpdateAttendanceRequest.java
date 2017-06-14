package com.feiniu.fnSubAccount.bean.request;


import com.feiniu.fnSubAccount.util.FormatUtils;

/**
 * Created by wangzhen on 2015/8/5.
 */
public class UpdateAttendanceRequest {
    private  String startTime;
    private  String endTime;

    public Long getStartTime() {
        return FormatUtils.formatStringDate(startTime).getTime()/1000;
    }

    public Long getEndTime() {
        return FormatUtils.formatStringDate(endTime).getTime()/1000;
    }

    public void setStartTime(String startTime) {
        this.startTime = startTime;
    }

    public void setEndTime(String endTime) {
        this.endTime = endTime;
    }
}
