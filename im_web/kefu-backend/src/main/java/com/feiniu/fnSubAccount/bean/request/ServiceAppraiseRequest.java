package com.feiniu.fnSubAccount.bean.request;

import java.util.List;

/**
 * Created by wangchen on 2015/2/12.
 */
public class ServiceAppraiseRequest {
    private String subAccountIDListStr;
    private int startTime;
    private int endTime;
    List<Long> subAccountIDList;

    public String getSubAccountIDListStr() {
        return subAccountIDListStr;
    }

    public void setSubAccountIDListStr(String subAccountIDListStr) {
        this.subAccountIDListStr = subAccountIDListStr;
    }

    public List<Long> getSubAccountIDList() {
        return subAccountIDList;
    }

    public void setSubAccountIDList(List<Long> subAccountIDList) {
        this.subAccountIDList = subAccountIDList;
    }

    public int getStartTime() {
        return startTime;
    }

    public void setStartTime(int startTime) {
        this.startTime = startTime;
    }

    public int getEndTime() {
        return endTime;
    }

    public void setEndTime(int endTime) {
        this.endTime = endTime;
    }
}
