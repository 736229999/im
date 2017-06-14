package com.feiniu.webim.bean.domain;

/**
 * Created by zhangmin on 2015/12/9.
 */
public class GoodsInfo {

    private long SM_SEQ;
    private String SM_PIC;
    private Double SM_PRICE;
//    private int SALEQTY; //这是啥？
    private int IS_ORGI_ITEM;
    private String SM_TITLE;
    private String COLOR;
    private String IT_COLOR_SEQ;
    private String CP_SEQ;
    private int IS_MAIN_SKU;
    private int STORE_ID;
    private int IS_MALL;

    //飞牛价
    private Double fnPrice = 0.0;
    //库存
    private Integer num = 1;
    //销量
    private Integer sold = 0;

    public long getSM_SEQ() {
        return SM_SEQ;
    }

    public void setSM_SEQ(long SM_SEQ) {
        this.SM_SEQ = SM_SEQ;
    }

    public String getSM_PIC() {
        return SM_PIC;
    }

    public void setSM_PIC(String SM_PIC) {
        this.SM_PIC = SM_PIC;
    }

    public Double getSM_PRICE() {
        return SM_PRICE;
    }

    public void setSM_PRICE(Double SM_PRICE) {
        this.SM_PRICE = SM_PRICE;
    }

    public int getIS_ORGI_ITEM() {
        return IS_ORGI_ITEM;
    }

    public void setIS_ORGI_ITEM(int IS_ORGI_ITEM) {
        this.IS_ORGI_ITEM = IS_ORGI_ITEM;
    }

    public String getSM_TITLE() {
        return SM_TITLE;
    }

    public void setSM_TITLE(String SM_TITLE) {
        this.SM_TITLE = SM_TITLE;
    }

    public String getCOLOR() {
        return COLOR;
    }

    public void setCOLOR(String COLOR) {
        this.COLOR = COLOR;
    }

    public String getIT_COLOR_SEQ() {
        return IT_COLOR_SEQ;
    }

    public void setIT_COLOR_SEQ(String IT_COLOR_SEQ) {
        this.IT_COLOR_SEQ = IT_COLOR_SEQ;
    }

    public String getCP_SEQ() {
        return CP_SEQ;
    }

    public void setCP_SEQ(String CP_SEQ) {
        this.CP_SEQ = CP_SEQ;
    }

    public int getIS_MAIN_SKU() {
        return IS_MAIN_SKU;
    }

    public void setIS_MAIN_SKU(int IS_MAIN_SKU) {
        this.IS_MAIN_SKU = IS_MAIN_SKU;
    }

    public int getSTORE_ID() {
        return STORE_ID;
    }

    public void setSTORE_ID(int STORE_ID) {
        this.STORE_ID = STORE_ID;
    }

    public int getIS_MALL() {
        return IS_MALL;
    }

    public void setIS_MALL(int IS_MALL) {
        this.IS_MALL = IS_MALL;
    }

    public Double getFnPrice() {
        return fnPrice;
    }

    public void setFnPrice(Double fnPrice) {
        this.fnPrice = fnPrice;
    }

    public Integer getNum() {
        return num;
    }

    public void setNum(Integer num) {
        this.num = num;
    }

    public Integer getSold() {
        return sold;
    }

    public void setSold(Integer sold) {
        this.sold = sold;
    }
}
