package com.feiniu.subAccount.domain;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * Created by wangchen on 2015/2/12.
 */
@Entity
@Table(name = "im_webrecomment")
public class ServiceAppraise {
    @Id
    @Column(name = "SEQID")
    private int serviceAppraseID;

    @Column(name = "FROMID", nullable = false)
    private long customerID;

    @Column(name = "TOID", nullable = false)
    private long subMerchantID;

    @Column(name = "SENDTIME", nullable = false)
    private int sendTime;

    @Column(name = "RECOMMENT", nullable = false)
    private int appraise;

    public int getServiceAppraseID() {
        return serviceAppraseID;
    }

    public void setServiceAppraseID(int serviceAppraseID) {
        this.serviceAppraseID = serviceAppraseID;
    }

    public long getCustomerID() {
        return customerID;
    }

    public void setCustomerID(long customerID) {
        this.customerID = customerID;
    }

    public long getSubMerchantID() {
        return subMerchantID;
    }

    public void setSubMerchantID(long subMerchantID) {
        this.subMerchantID = subMerchantID;
    }

    public int getSendTime() {
        return sendTime;
    }

    public void setSendTime(int sendTime) {
        this.sendTime = sendTime;
    }

    public int getAppraise() {
        return appraise;
    }

    public void setAppraise(int appraise) {
        this.appraise = appraise;
    }
}










