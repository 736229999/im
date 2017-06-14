package com.feiniu.csWebIm.bean.domain;

/**
 * Created by Chenwuming on 2015/11/20.
 */


public class ProductData {
    private String ID_NO;//商品ID
    private String ITNAME;//商品名称
    private String KIND;//加购
    private String DS_TYPE;//出货类型
    private String QTY;//出货数量
    private String DS_NAME;//出货商
    private String OG_SNO;//订单序号

    public String getID_NO() {
        return ID_NO;
    }

    public void setID_NO(String ID_NO) {
        this.ID_NO = ID_NO;
    }

    public String getITNAME() {
        return ITNAME;
    }

    public void setITNAME(String ITNAME) {
        this.ITNAME = ITNAME;
    }

    public String getKIND() {
        return KIND;
    }

    public void setKIND(String KIND) {
        this.KIND = KIND;
    }

    public String getDS_TYPE() {
        return DS_TYPE;
    }

    public void setDS_TYPE(String DS_TYPE) {
        this.DS_TYPE = DS_TYPE;
    }

    public String getQTY() {
        return QTY;
    }

    public void setQTY(String QTY) {
        this.QTY = QTY;
    }

    public String getDS_NAME() {
        return DS_NAME;
    }

    public void setDS_NAME(String DS_NAME) {
        this.DS_NAME = DS_NAME;
    }

    public String getOG_SNO() {
        return OG_SNO;
    }

    public void setOG_SNO(String OG_SNO) {
        this.OG_SNO = OG_SNO;
    }
}

