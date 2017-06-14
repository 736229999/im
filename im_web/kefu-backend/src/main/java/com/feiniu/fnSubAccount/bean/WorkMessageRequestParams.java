package com.feiniu.fnSubAccount.bean;

/**
 * Created by wangzhen on 2015/8/18.
 */
public class WorkMessageRequestParams {
    long startTime;
    long endTime;
    int startCount;
    int endCount;
    float startRate;
    float endRate;
    float startLandingDur;
    float endLandingDur;

    int pageSize;
    int currentPageNo;

    public long getStartTime() {
        return startTime;
    }

    public void setStartTime(long startTime) {
        this.startTime = startTime;
    }

    public long getEndTime() {
        return endTime;
    }

    public void setEndTime(long endTime) {
        this.endTime = endTime;
    }

    public int getStartCount() {
        return startCount;
    }

    public void setStartCount(int startCount) {
        this.startCount = startCount;
    }

    public int getEndCount() {
        return endCount;
    }

    public void setEndCount(int endCount) {
        this.endCount = endCount;
    }

    public float getStartRate() {
        return startRate;
    }

    public void setStartRate(float startRate) {
        this.startRate = startRate;
    }

    public float getEndRate() {
        return endRate;
    }

    public void setEndRate(float endRate) {
        this.endRate = endRate;
    }

    public float getStartLandingDur() {
        return startLandingDur;
    }

    public void setStartLandingDur(float startLandingDur) {
        this.startLandingDur = startLandingDur;
    }

    public float getEndLandingDur() {
        return endLandingDur;
    }

    public void setEndLandingDur(float endLandingDur) {
        this.endLandingDur = endLandingDur;
    }

    public int getPageSize() {
        return pageSize;
    }

    public void setPageSize(int pageSize) {
        this.pageSize = pageSize;
    }

    public int getCurrentPageNo() {
        return currentPageNo;
    }

    public void setCurrentPageNo(int currentPageNo) {
        this.currentPageNo = currentPageNo;
    }
}
