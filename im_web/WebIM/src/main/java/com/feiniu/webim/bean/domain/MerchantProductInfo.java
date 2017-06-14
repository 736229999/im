package com.feiniu.webim.bean.domain;

/**
 * Created by wangchen on 2015/3/12.
 * 商家看到的商品信息
 */
public class MerchantProductInfo {
    private long goodsId;
    private String title;
    private String pic01;
    private double mallPrice;
    private int stock;

    public MerchantProductInfo(){}

    public long getGoodsId() {
        return goodsId;
    }

    public void setGoodsId(long goodsId) {
        this.goodsId = goodsId;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getPic01() {
        return pic01;
    }

    public void setPic01(String pic01) {
        this.pic01 = pic01;
    }

    public double getMallPrice() {
        return mallPrice;
    }

    public void setMallPrice(double mallPrice) {
        this.mallPrice = mallPrice;
    }

    public int getStock() {
        return stock;
    }

    public void setStock(int stock) {
        this.stock = stock;
    }
}
