package com.feiniu.fnSubAccount.bean;

/**
 * Created by wangzhen on 2015/11/24.
 */
public class CSMoitorInfoShow {
    public String userNameFull;
    public String onlineNameFull;
    public String limitNameFull;
    public String offlineNameFull;
    public String askingCont;
    public String receptionCount;
    public String status;
    private Long cServiceId;

    public String getUserNameFull() {
        return userNameFull;
    }

    public void setUserNameFull(String userNameFull) {
        this.userNameFull = userNameFull;
    }

    public String getOnlineNameFull() {
        return onlineNameFull;
    }

    public void setOnlineNameFull(String onlineNameFull) {
        this.onlineNameFull = onlineNameFull;
    }

    public String getLimitNameFull() {
        return limitNameFull;
    }

    public void setLimitNameFull(String limitNameFull) {
        this.limitNameFull = limitNameFull;
    }

    public String getOfflineNameFull() {
        return offlineNameFull;
    }

    public void setOfflineNameFull(String offlineNameFull) {
        this.offlineNameFull = offlineNameFull;
    }

    public String getAskingCont() {
        return askingCont;
    }

    public void setAskingCont(String askingCont) {
        this.askingCont = askingCont;
    }

    public String getReceptionCount() {
        return receptionCount;
    }

    public void setReceptionCount(String receptionCount) {
        this.receptionCount = receptionCount;
    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }

    public Long getcServiceId() {
        return cServiceId;
    }

    public void setcServiceId(Long cServiceId) {
        this.cServiceId = cServiceId;
    }
}
