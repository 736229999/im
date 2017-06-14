package com.feiniu.webim.bean.domain;

/**
 * Created by wangchen on 2015/1/14.
 */
public class ProductInfo {
    private long goodsId;
    private String defaultPic;
    private String title;
    private double price;
    private String productURL;
    private String commentURL;
    private int sold = 0;
    private int commentCount = 0;
    //是否系列品
    private boolean isSeries = true;

    public long getGoodsId() {
        return goodsId;
    }

    public void setGoodsId(long goodsId) {
        this.goodsId = goodsId;
    }

    public boolean isSeries() {
        return isSeries;
    }

    public void setIsSeries(boolean isSeries) {
        this.isSeries = isSeries;
    }

    public String getDefaultPic() {
        return defaultPic;
    }

    public void setDefaultPic(String defaultPic) {
        this.defaultPic = defaultPic;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public double getPrice() {
        return price;
    }

    public void setPrice(double price) {
        this.price = price;
    }

    public String getProductURL() {
        return productURL;
    }

    public String getCommentURL() {
        return commentURL;
    }

    public void setCommentURL(String commentURL) {
        this.commentURL = commentURL;
    }

    public void setProductURL(String productURL) {
        this.productURL = productURL;
    }

    public int getSold() {
        return sold;
    }

    public void setSold(int sold) {
        this.sold = sold;
    }

    public int getCommentCount() {
        return commentCount;
    }

    public void setCommentCount(int commentCount) {
        this.commentCount = commentCount;
    }
}
