package com.cn.feiniu.logManagement.bean.web.hessian.req;

import java.io.Serializable;

/**
 * Author: morningking
 * Date: 2015/7/17
 * Contact: 243717042@qq.com
 */
public class SubMerchantAction implements Serializable{
    private static final long serialVersionUID = 6907685943220229072L;

    private int merchantID;
    private long subMerchantID;
    private int actionTime;
    private byte actionType;

    public int getMerchantID() {
        return merchantID;
    }

    public void setMerchantID(int merchantID) {
        this.merchantID = merchantID;
    }

    public long getSubMerchantID() {
        return subMerchantID;
    }

    public void setSubMerchantID(long subMerchantID) {
        this.subMerchantID = subMerchantID;
    }

    public int getActionTime() {
        return actionTime;
    }

    public void setActionTime(int actionTime) {
        this.actionTime = actionTime;
    }

    public byte getActionType() {
        return actionType;
    }

    public void setActionType(byte actionType) {
        this.actionType = actionType;
    }
}
