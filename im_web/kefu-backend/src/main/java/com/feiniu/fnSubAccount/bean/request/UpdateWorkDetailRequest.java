package com.feiniu.fnSubAccount.bean.request;

import com.feiniu.fnSubAccount.util.FormatUtils;

import java.util.Date;

/**
 * Created by wangzhen on 2015/7/10.
 */
public class UpdateWorkDetailRequest {
    Date startTime;
    Date endTime;

    public Long getStartTime() {
        return startTime.getTime()/1000;
    }

    public void setStartTime(String startTime) {
        this.startTime = FormatUtils.formatStringDate(startTime);
    }

    public Long getEndTime() {
        return endTime.getTime()/1000;
    }

    public void setEndTime(String endTime) {
        this.endTime = FormatUtils.formatStringDate(endTime);
    }
}
