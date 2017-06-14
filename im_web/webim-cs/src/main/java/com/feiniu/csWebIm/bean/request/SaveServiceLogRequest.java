package com.feiniu.csWebIm.bean.request;

import com.feiniu.csWebIm.bean.domain.AskingInfo;
import com.feiniu.csWebIm.bean.domain.AskingOpt;

import java.util.List;

/**
 * Created by wangzhen on 2015/10/22.
 */
public class SaveServiceLogRequest {
    private String customerUUID;
    private String serviceName;
    private String ogSeq;
    private String savedName;
    private String savedTel;
    private String savedType;
    private String savedLevel;
    private String savedStatus;
    private String askingOptions;
    private String askingGoodsInfo;
    private List<AskingOpt> askingOptionsList;
    private List<AskingInfo> askingGoodsInfoList;
    private String serviceLog;


    public String getOgSeq() {
        return ogSeq;
    }

    public void setOgSeq(String ogSeq) {
        this.ogSeq = ogSeq;
    }

    public String getCustomerUUID() {
        return customerUUID;
    }

    public void setCustomerUUID(String customerUUID) {
        this.customerUUID = customerUUID;
    }

    public String getServiceName() {
        return serviceName;
    }

    public void setServiceName(String serviceName) {
        this.serviceName = serviceName;
    }

    public String getSavedName() {
        return savedName;
    }

    public void setSavedName(String savedName) {
        this.savedName = savedName;
    }

    public String getSavedTel() {
        return savedTel;
    }

    public void setSavedTel(String savedTel) {
        this.savedTel = savedTel;
    }

    public String getSavedType() {
        return savedType;
    }

    public void setSavedType(String savedType) {
        this.savedType = savedType;
    }

    public String getSavedLevel() {
        return savedLevel;
    }

    public void setSavedLevel(String savedLevel) {
        this.savedLevel = savedLevel;
    }

    public String getSavedStatus() {
        return savedStatus;
    }

    public void setSavedStatus(String savedStatus) {
        this.savedStatus = savedStatus;
    }

    public String getAskingOptions() {
        return askingOptions;
    }

    public void setAskingOptions(String askingOptions) {
        this.askingOptions = askingOptions;
    }

    public String getAskingGoodsInfo() {
        return askingGoodsInfo;
    }

    public void setAskingGoodsInfo(String askingGoodsInfo) {
        this.askingGoodsInfo = askingGoodsInfo;
    }

    public List<AskingOpt> getAskingOptionsList() {
        return askingOptionsList;
    }

    public void setAskingOptionsList(List<AskingOpt> askingOptionsList) {
        this.askingOptionsList = askingOptionsList;
    }

    public List<AskingInfo> getAskingGoodsInfoList() {
        return askingGoodsInfoList;
    }

    public void setAskingGoodsInfoList(List<AskingInfo> askingGoodsInfoList) {
        this.askingGoodsInfoList = askingGoodsInfoList;
    }

    public String getServiceLog() {
        return serviceLog;
    }

    public void setServiceLog(String serviceLog) {
        this.serviceLog = serviceLog;
    }
}
