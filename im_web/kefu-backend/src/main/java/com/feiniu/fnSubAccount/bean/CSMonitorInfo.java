package com.feiniu.fnSubAccount.bean;

import java.util.List;

/**
 * Created by wanbingwen on 2015/10/28.
 */
public class CSMonitorInfo implements  Comparable<CSMonitorInfo >{
    private Long CServiceId;
    List<String> Customerlist;
    private String CServiceName;
    private Long ChattingBuyerId;
    private Integer Consult;
    private Long DirectId;
    private Integer Permission;
    private Integer RecpLimit;
    private Integer Recption;
    private Integer State;

    public Long getCServiceId() {
        return CServiceId;
    }

    public void setCServiceId(Long CServiceId) {
        this.CServiceId = CServiceId;
    }

    public String getCServiceName() {
        return CServiceName;
    }

    public void setCServiceName(String CServiceName) {
        this.CServiceName = CServiceName;
    }

    public Long getChattingBuyerId() {
        return ChattingBuyerId;
    }

    public void setChattingBuyerId(Long chattingBuyerId) {
        ChattingBuyerId = chattingBuyerId;
    }

    public Integer getConsult() {
        return Consult;
    }

    public void setConsult(Integer consult) {
        Consult = consult;
    }

    public Long getDirectId() {
        return DirectId;
    }

    public void setDirectId(Long directId) {
        DirectId = directId;
    }

    public Integer getPermission() {
        return Permission;
    }

    public void setPermission(Integer permission) {
        Permission = permission;
    }

    public Integer getRecpLimit() {
        return RecpLimit;
    }

    public void setRecpLimit(Integer recpLimit) {
        RecpLimit = recpLimit;
    }

    public Integer getRecption() {
        return Recption;
    }

    public void setRecption(Integer recption) {
        Recption = recption;
    }

    public Integer getState() {
        return State;
    }

    public void setState(Integer state) {
        State = state;
    }

    public List<String> getCustomerlist() {
        return Customerlist;
    }

    public void setCustomerlist(List<String> customerlist) {
        Customerlist = customerlist;
    }

    @Override
    public int compareTo(CSMonitorInfo arg0) {
        return this.getCServiceId().compareTo(arg0.getCServiceId());
    }
}
