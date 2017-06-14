package com.feiniu.fnSubAccount.bean.response;

/**
 * Created by wangzhen on 2015/8/18.
 */
public class WorkMessageTotalInfoResponse {
    int sumLandingDuration;
    int sumOnlineCustomerCount;
    int sumAskingCount;
    int sumReplyCount;
    int sumAskingTimes;
    int sumReplyTimes;
    int sumRreplyFrequency;

    float avgLandingDuration;
    float avgOnlineCustomerCount;
    float avgAskingCount;
    float avgReplyCount;
    float avgAskingTimes;
    float avgReplyTimes;
    float avgRreplyFrequency;
    float avgResponseDuration;
    float avgRate;
    int totalCount;

    public int getSumAskingTimes() {
        return sumAskingTimes;
    }

    public void setSumAskingTimes(int sumAskingTimes) {
        this.sumAskingTimes = sumAskingTimes;
    }

    public int getSumReplyTimes() {
        return sumReplyTimes;

    }

    public float getAvgRreplyFrequency() {
        return avgRreplyFrequency;
    }

    public void setAvgRreplyFrequency(float avgRreplyFrequency) {
        this.avgRreplyFrequency = avgRreplyFrequency;
    }

    public int getSumRreplyFrequency() {

        return sumRreplyFrequency;
    }

    public void setSumRreplyFrequency(int sumRreplyFrequency) {
        this.sumRreplyFrequency = sumRreplyFrequency;
    }

    public void setSumReplyTimes(int sumReplyTimes) {
        this.sumReplyTimes = sumReplyTimes;
    }

    public float getAvgAskingTimes() {
        return avgAskingTimes;
    }

    public void setAvgAskingTimes(float avgAskingTimes) {
        this.avgAskingTimes = avgAskingTimes;
    }

    public float getAvgReplyTimes() {
        return avgReplyTimes;
    }

    public void setAvgReplyTimes(float avgReplyTimes) {
        this.avgReplyTimes = avgReplyTimes;
    }

    public int getSumOnlineCustomerCount() {
        return sumOnlineCustomerCount;
    }

    public void setSumOnlineCustomerCount(int sumOnlineCustomerCount) {
        this.sumOnlineCustomerCount = sumOnlineCustomerCount;
    }

    public float getAvgOnlineCustomerCount() {
        return avgOnlineCustomerCount;
    }

    public void setAvgOnlineCustomerCount(float avgOnlineCustomerCount) {
        this.avgOnlineCustomerCount = avgOnlineCustomerCount;
    }

    public int getSumLandingDuration() {
        return sumLandingDuration;
    }

    public void setSumLandingDuration(int sumLandingDuration) {
        this.sumLandingDuration = sumLandingDuration;
    }

    public int getSumAskingCount() {
        return sumAskingCount;
    }

    public void setSumAskingCount(int sumAskingCount) {
        this.sumAskingCount = sumAskingCount;
    }

    public int getSumReplyCount() {
        return sumReplyCount;
    }

    public void setSumReplyCount(int sumReplyCount) {
        this.sumReplyCount = sumReplyCount;
    }

    public float getAvgLandingDuration() {
        return avgLandingDuration;
    }

    public void setAvgLandingDuration(float avgLandingDuration) {
        this.avgLandingDuration = avgLandingDuration;
    }

    public float getAvgAskingCount() {
        return avgAskingCount;
    }

    public void setAvgAskingCount(float avgAskingCount) {
        this.avgAskingCount = avgAskingCount;
    }

    public float getAvgReplyCount() {
        return avgReplyCount;
    }

    public void setAvgReplyCount(float avgReplyCount) {
        this.avgReplyCount = avgReplyCount;
    }

    public float getAvgResponseDuration() {
        return avgResponseDuration;
    }

    public void setAvgResponseDuration(float avgResponseDuration) {
        this.avgResponseDuration = avgResponseDuration;
    }

    public float getAvgRate() {
        return avgRate;
    }

    public void setAvgRate(float avgRate) {
        this.avgRate = avgRate;
    }

    public int getTotalCount() {
        return totalCount;
    }

    public void setTotalCount(int totalCount) {
        this.totalCount = totalCount;
    }
}
