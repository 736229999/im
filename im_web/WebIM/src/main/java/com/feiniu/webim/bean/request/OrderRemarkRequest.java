package com.feiniu.webim.bean.request;

/**
 * Created by zhangmin on 2016/1/12.
 */
public class OrderRemarkRequest {
    private String ogsSeqs;  //包裹号,201601COGS1110005771
    private String remark;   //备注
    private String modUsr;   //备注人  竟然是string

    public String getOgsSeqs() {
        return ogsSeqs;
    }

    public void setOgsSeqs(String ogsSeqs) {
        this.ogsSeqs = ogsSeqs;
    }

    public String getRemark() {
        return remark;
    }

    public void setRemark(String remark) {
        this.remark = remark;
    }

    public String getModUsr() {
        return modUsr;
    }

    public void setModUsr(String modUsr) {
        this.modUsr = modUsr;
    }
}
