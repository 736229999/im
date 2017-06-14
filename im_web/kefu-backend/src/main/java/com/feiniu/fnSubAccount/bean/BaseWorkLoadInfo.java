package com.feiniu.fnSubAccount.bean;


import com.feiniu.fnSubAccount.consts.SystemConfig;

import java.math.BigDecimal;

/**
 * Created by wangzhen on 2015/7/28.
 */
public class BaseWorkLoadInfo implements Cloneable{
    private long subMerchantId;
    //sql查询出来的数据保存
    private long sqlResponseTimeSum;
    private long sqlSessionTimeSum;
    private long sqlSessionCount;
    private long sqlReponseMessageCount;
    private long sqlReceptionTimeSum;
    private long sqlResponseIdsCount;
    private long sqlUnResponseIdsCount;
    private long sqlAskingIdsCount;
    private long sqlResponseCount;
    private long sqlAskingTimes;
    private long sqlResponseTimes;
    private long sqlLandingTimeSum;

    //最终要显示的数据保存
    private String landingTimeSum;
    private String receptionTimeSum;
    private String askingCustomerCount;
    private String responseCustomerCount;
    private String askingTimes;
    private String responseTimes;
    private String unresponseCustomerCount;
    private String responseRate;
    private String responseMessageCountPerSessionAvg;
    private String timePerSessionAvg;
    private String firstResponseTimeAvg;

    @Override
    public Object clone() {
        BaseWorkLoadInfo cloneitem = null;
        try {
            cloneitem = (BaseWorkLoadInfo) super.clone();
        } catch (CloneNotSupportedException e) {
            return null;
        }
        return cloneitem;
    }

    public void initWithZero(){
        landingTimeSum = "0.0";
        receptionTimeSum = "0.0";
        askingCustomerCount = "0.0";
        responseCustomerCount = "0.0";
        unresponseCustomerCount = "0.0";
        this.responseRate = "0.0";
        responseMessageCountPerSessionAvg = "0.0";
        timePerSessionAvg = "0.0";
        firstResponseTimeAvg = "0.0";
        askingTimes="0.0";
        responseTimes="0.0";
    }

    public void div(int divInteger){

        BigDecimal landingTime =  new BigDecimal(landingTimeSum);
        BigDecimal receptionTime =  new BigDecimal(receptionTimeSum);
        BigDecimal askingCount =  new BigDecimal(askingCustomerCount);
        BigDecimal responseCount =  new BigDecimal(responseCustomerCount);
        BigDecimal unResponseCount =  new BigDecimal(unresponseCustomerCount);
        BigDecimal askingTimesBig = new BigDecimal(this.askingTimes);
        BigDecimal responseTimesBig = new BigDecimal(this.responseTimes);

        if (!responseMessageCountPerSessionAvg.equals("-")){
            BigDecimal responseMessageCount =  new BigDecimal(responseMessageCountPerSessionAvg);
            responseMessageCountPerSessionAvg = responseMessageCount.divide(new BigDecimal(divInteger),1, BigDecimal.ROUND_HALF_EVEN).setScale(SystemConfig.DEFAULT_SCALE).toString();
        }

        if (!timePerSessionAvg.equals("-")){
            BigDecimal sessionTime =  new BigDecimal(timePerSessionAvg);
            timePerSessionAvg = sessionTime.divide(new BigDecimal(divInteger),1, BigDecimal.ROUND_HALF_EVEN).setScale(SystemConfig.DEFAULT_SCALE).toString();
        }

        if(!firstResponseTimeAvg.equals("-")){
            BigDecimal responseTime =  new BigDecimal(firstResponseTimeAvg);
            firstResponseTimeAvg = responseTime.divide(new BigDecimal(divInteger), 1, BigDecimal.ROUND_HALF_EVEN).setScale(SystemConfig.DEFAULT_SCALE).toString();
        }

        landingTimeSum = landingTime.divide(new BigDecimal(divInteger),1, BigDecimal.ROUND_HALF_EVEN).setScale(SystemConfig.DEFAULT_SCALE).toString();
        receptionTimeSum = receptionTime.divide(new BigDecimal(divInteger),1, BigDecimal.ROUND_HALF_EVEN).setScale(SystemConfig.DEFAULT_SCALE).toString();
        askingCustomerCount = askingCount.divide(new BigDecimal(divInteger),1, BigDecimal.ROUND_HALF_EVEN).setScale(SystemConfig.DEFAULT_SCALE).toString();
        responseCustomerCount = responseCount.divide(new BigDecimal(divInteger),1, BigDecimal.ROUND_HALF_EVEN).setScale(SystemConfig.DEFAULT_SCALE).toString();
        unresponseCustomerCount = unResponseCount.divide(new BigDecimal(divInteger),1, BigDecimal.ROUND_HALF_EVEN).setScale(SystemConfig.DEFAULT_SCALE).toString();

        this.askingTimes  = askingTimesBig.divide(new BigDecimal(divInteger),1, BigDecimal.ROUND_HALF_EVEN).setScale(SystemConfig.DEFAULT_SCALE).toString();
        this.responseTimes = responseTimesBig.divide(new BigDecimal(divInteger),1, BigDecimal.ROUND_HALF_EVEN).setScale(SystemConfig.DEFAULT_SCALE).toString();

    }

    public void add(BaseWorkLoadInfo workloadindex){
        BigDecimal landingTime =  new BigDecimal(landingTimeSum);
        BigDecimal receptionTime =  new BigDecimal(receptionTimeSum);
        BigDecimal askingCount =  new BigDecimal(askingCustomerCount);
        BigDecimal responseCount =  new BigDecimal(responseCustomerCount);
        BigDecimal unResponseCount =  new BigDecimal(unresponseCustomerCount);
        BigDecimal askingTimesBig = new BigDecimal(this.askingTimes);
        BigDecimal responseTimesBig = new BigDecimal(this.responseTimes);

        if (!workloadindex.getResponseMessageCountPerSessionAvg().equals("-")){
            BigDecimal responseMessageCount =  new BigDecimal(responseMessageCountPerSessionAvg);
            responseMessageCountPerSessionAvg = responseMessageCount.add(new BigDecimal(workloadindex.getResponseMessageCountPerSessionAvg())).setScale(SystemConfig.DEFAULT_SCALE).toString();
        }

        if (!workloadindex.getTimePerSessionAvg().equals("-")){
            BigDecimal sessionTime =  new BigDecimal(timePerSessionAvg);
            timePerSessionAvg = sessionTime.add(new BigDecimal(workloadindex.getTimePerSessionAvg())).setScale(SystemConfig.DEFAULT_SCALE).toString();
        }

        if(!workloadindex.getFirstResponseTimeAvg().equals("-")){
            BigDecimal responseTime =  new BigDecimal(firstResponseTimeAvg);
            firstResponseTimeAvg = responseTime.add(new BigDecimal(workloadindex.getFirstResponseTimeAvg())).setScale(SystemConfig.DEFAULT_SCALE).toString();
        }

        landingTimeSum = landingTime.add(new BigDecimal(workloadindex.getLandingTimeSum())).setScale(SystemConfig.DEFAULT_SCALE).toString();
        receptionTimeSum = receptionTime.add(new BigDecimal(workloadindex.getReceptionTimeSum())).setScale(SystemConfig.DEFAULT_SCALE).toString();
        askingCustomerCount = askingCount.add(new BigDecimal(workloadindex.getAskingCustomerCount())).setScale(SystemConfig.DEFAULT_SCALE).toString();;
        responseCustomerCount = responseCount.add(new BigDecimal(workloadindex.getResponseCustomerCount())).setScale(SystemConfig.DEFAULT_SCALE).toString();
        unresponseCustomerCount = unResponseCount.add(new BigDecimal(workloadindex.getUnresponseCustomerCount())).setScale(SystemConfig.DEFAULT_SCALE).toString();
        this.askingTimes  = askingTimesBig.add(new BigDecimal(workloadindex.getAskingTimes())).setScale(SystemConfig.DEFAULT_SCALE).toString();
        this.responseTimes = responseTimesBig.add(new BigDecimal(workloadindex.getResponseTimes())).setScale(SystemConfig.DEFAULT_SCALE).toString();
    }

    public long getSqlAskingTimes() {
        return sqlAskingTimes;
    }

    public void setSqlAskingTimes(long sqlAskingTimes) {
        this.sqlAskingTimes = sqlAskingTimes;
    }

    public long getSqlResponseTimes() {
        return sqlResponseTimes;
    }

    public void setSqlResponseTimes(long sqlResponseTimes) {
        this.sqlResponseTimes = sqlResponseTimes;
    }

    public String getAskingTimes() {
        return askingTimes;
    }

    public void setAskingTimes(String askingTimes) {
        this.askingTimes = askingTimes;
    }

    public String getResponseTimes() {
        return responseTimes;
    }

    public void setResponseTimes(String responseTimes) {
        this.responseTimes = responseTimes;
    }

    public long getSubMerchantId() {
        return subMerchantId;
    }

    public void setSubMerchantId(long subMerchantId) {
        this.subMerchantId = subMerchantId;
    }

    public long getSqlResponseTimeSum() {
        return sqlResponseTimeSum;
    }

    public void setSqlResponseTimeSum(long sqlResponseTimeSum) {
        this.sqlResponseTimeSum = sqlResponseTimeSum;
    }

    public long getSqlSessionTimeSum() {
        return sqlSessionTimeSum;
    }

    public void setSqlSessionTimeSum(long sqlSessionTimeSum) {
        this.sqlSessionTimeSum = sqlSessionTimeSum;
    }

    public long getSqlSessionCount() {
        return sqlSessionCount;
    }

    public void setSqlSessionCount(long sqlSessionCount) {
        this.sqlSessionCount = sqlSessionCount;
    }

    public long getSqlReponseMessageCount() {
        return sqlReponseMessageCount;
    }

    public void setSqlReponseMessageCount(long sqlReponseMessageCount) {
        this.sqlReponseMessageCount = sqlReponseMessageCount;
    }

    public long getSqlReceptionTimeSum() {
        return sqlReceptionTimeSum;
    }

    public void setSqlReceptionTimeSum(long sqlReceptionTimeSum) {
        this.sqlReceptionTimeSum = sqlReceptionTimeSum;
    }

    public long getSqlResponseIdsCount() {
        return sqlResponseIdsCount;
    }

    public void setSqlResponseIdsCount(long sqlResponseIdsCount) {
        this.sqlResponseIdsCount = sqlResponseIdsCount;
    }

    public long getSqlAskingIdsCount() {
        return sqlAskingIdsCount;
    }

    public void setSqlAskingIdsCount(long sqlAskingIdsCount) {
        this.sqlAskingIdsCount = sqlAskingIdsCount;
    }

    public long getSqlResponseCount() {
        return sqlResponseCount;
    }

    public void setSqlResponseCount(long sqlResponseCount) {
        this.sqlResponseCount = sqlResponseCount;
    }

    public long getSqlLandingTimeSum() {
        return sqlLandingTimeSum;
    }

    public void setSqlLandingTimeSum(long sqlLandingTimeSum) {
        this.sqlLandingTimeSum = sqlLandingTimeSum;
    }

    public String getLandingTimeSum() {
        return landingTimeSum;
    }

    public void setLandingTimeSum(String landingTimeSum) {
        this.landingTimeSum = landingTimeSum;
    }

    public String getReceptionTimeSum() {
        return receptionTimeSum;
    }

    public void setReceptionTimeSum(String receptionTimeSum) {
        this.receptionTimeSum = receptionTimeSum;
    }

    public String getAskingCustomerCount() {
        return askingCustomerCount;
    }

    public void setAskingCustomerCount(String askingCustomerCount) {
        this.askingCustomerCount = askingCustomerCount;
    }

    public String getResponseCustomerCount() {
        return responseCustomerCount;
    }

    public void setResponseCustomerCount(String responseCustomerCount) {
        this.responseCustomerCount = responseCustomerCount;
    }

    public String getUnresponseCustomerCount() {
        return unresponseCustomerCount;
    }

    public void setUnresponseCustomerCount(String unresponseCustomerCount) {
        this.unresponseCustomerCount = unresponseCustomerCount;
    }

    public String getResponseRate() {
        return responseRate;
    }

    public void setResponseRate(String responseRate) {
        this.responseRate = responseRate;
    }

    public String getResponseMessageCountPerSessionAvg() {
        return responseMessageCountPerSessionAvg;
    }

    public void setResponseMessageCountPerSessionAvg(String responseMessageCountPerSessionAvg) {
        this.responseMessageCountPerSessionAvg = responseMessageCountPerSessionAvg;
    }

    public String getTimePerSessionAvg() {
        return timePerSessionAvg;
    }

    public void setTimePerSessionAvg(String timePerSessionAvg) {
        this.timePerSessionAvg = timePerSessionAvg;
    }

    public String getFirstResponseTimeAvg() {
        return firstResponseTimeAvg;
    }

    public void setFirstResponseTimeAvg(String firstResponseTimeAvg) {
        this.firstResponseTimeAvg = firstResponseTimeAvg;
    }

    public long getSqlUnResponseIdsCount() {
        return sqlUnResponseIdsCount;
    }

    public void setSqlUnResponseIdsCount(long sqlUnResponseIdsCount) {
        this.sqlUnResponseIdsCount = sqlUnResponseIdsCount;
    }
}
