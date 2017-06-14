package com.feiniu.csWebIm.bean.domain;

/**
 * Created by wangzhen on 2015/10/29.
 */
public class AskingGoodsCart {
    private  String fdlSeq;
    private String smSeq;
    private String name;
    private String id;
    private String pic;
    private String qty;

    public String getQty() {
        return qty;
    }

    public void setQty(String qty) {
        this.qty = qty;
    }

    public String getFdlSeq() {
        return fdlSeq;
    }

    public void setFdlSeq(String fdlSeq) {
        this.fdlSeq = fdlSeq;
    }

    public String getSmSeq() {
        return smSeq;
    }

    public void setSmSeq(String smSeq) {
        this.smSeq = smSeq;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getPic() {
        return pic;
    }

    public void setPic(String pic) {
        this.pic = pic;
    }
}
