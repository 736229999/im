package com.feiniu.csWebIm.bean.request;

/**
 * Created by wangzhen on 2015/10/21.
 */
public class getServiceLogInfoRequest {
    private String customerId;
    private String orId;
    private String pack_no;
    private String og_seq;
    private Integer type;
    private String askingGoodIdList;
    private String goodsAskingType;

    public String getCustomerId() {
        return customerId;
    }

    public void setCustomerId(String customerId) {
        this.customerId = customerId;
    }

    public String getOrId() {
        return orId;
    }

    public void setOrId(String orId) {
        this.orId = orId;
    }

    public String getAskingGoodIdList() {
        return askingGoodIdList;
    }

    public void setAskingGoodIdList(String askingGoodIdList) {
        this.askingGoodIdList = askingGoodIdList;
    }

    public String getGoodsAskingType() {
        return goodsAskingType;
    }

    public void setGoodsAskingType(String goodsAskingType) {
        this.goodsAskingType = goodsAskingType;
    }

    public String getPack_no() {
        return pack_no;
    }

    public void setPack_no(String pack_no) {
        this.pack_no = pack_no;
    }

    public String getOg_seq() {
        return og_seq;
    }

    public void setOg_seq(String og_seq) {
        this.og_seq = og_seq;
    }

    public Integer getType() {
        return type;
    }

    public void setType(Integer type) {
        this.type = type;
    }
}
