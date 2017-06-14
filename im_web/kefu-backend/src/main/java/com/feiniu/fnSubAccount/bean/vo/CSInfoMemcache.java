package com.feiniu.fnSubAccount.bean.vo;

import java.util.List;

/**
 * Created by wanbingwen on 2015/11/3.
 */
public class CSInfoMemcache {
    String CServiceId;
    String CServiceName;
    String State;
    String Recption;
    String Consult;
    String RecpLimit;
    List<String> Customerlist;

    public List<String> getCustomerlist() {
        return Customerlist;
    }

    public void setCustomerlist(List<String> customerlist) {
        Customerlist = customerlist;
    }

    public String getCServiceId() {
        return CServiceId;
    }

    public void setCServiceId(String CServiceId) {
        this.CServiceId = CServiceId;
    }

    public String getCServiceName() {
        return CServiceName;
    }

    public void setCServiceName(String CServiceName) {
        this.CServiceName = CServiceName;
    }

    public String getRecption() {
        return Recption;
    }

    public void setRecption(String recption) {
        Recption = recption;
    }

    public String getState() {
        return State;
    }

    public void setState(String state) {
        State = state;
    }

    public String getConsult() {
        return Consult;
    }

    public void setConsult(String consult) {
        Consult = consult;
    }

    public String getRecpLimit() {
        return RecpLimit;
    }

    public void setRecpLimit(String recpLimit) {
        RecpLimit = recpLimit;
    }


}
