package com.feiniu.csWebIm.bean.domain;

/**
 * Created by wangzhen on 2015/10/21.
 */
public class AskingInfo {
    private String title ;
    private String goodId ;
    private String addGood ;
    private String sellType ;
    private String sellCount ;
    private String sellerName ;
    private boolean checked ;
    private String ogsno;

    public boolean isChecked() {
        return checked;
    }

    public void setChecked(boolean checked) {
        this.checked = checked;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getGoodId() {
        return goodId;
    }

    public void setGoodId(String goodId) {
        this.goodId = goodId;
    }

    public String getAddGood() {
        return addGood;
    }

    public void setAddGood(String addGood) {
        this.addGood = addGood;
    }

    public String getSellType() {
        return sellType;
    }

    public void setSellType(String sellType) {
        this.sellType = sellType;
    }

    public String getSellCount() {
        return sellCount;
    }

    public void setSellCount(String sellCount) {
        this.sellCount = sellCount;
    }

    public String getSellerName() {
        return sellerName;
    }

    public void setSellerName(String sellerName) {
        this.sellerName = sellerName;
    }

    public String getOgsno() {
        return ogsno;
    }

    public void setOgsno(String ogsno) {
        this.ogsno = ogsno;
    }
}
