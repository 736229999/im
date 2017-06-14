package com.feiniu.webim.bean.domain;

/**
 * Created by zhangmin on 2016/1/6.
 */
public class GoodsAsking {
    private Integer status;
    private Long skuId;
    private Long goodsId;
    private String title;
    private Integer num;
    private Double skuPrice;
    private String pic01;
    private Integer sold;
    private Integer soldMonth;
    private Integer soldReturn;
    private Integer soldReturnMonth;
    private Double price;
    private Double marketPrice;
    private Integer commentCount;
    private String productURL;
    private Double salePrice;

    public Integer getStatus() {
        return status;
    }

    public void setStatus(Integer status) {
        this.status = status;
    }

    public Long getSkuId() {
        return skuId;
    }

    public void setSkuId(Long skuId) {
        this.skuId = skuId;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public Integer getNum() {
        return num;
    }

    public void setNum(Integer num) {
        this.num = num;
    }

    public Double getSkuPrice() {
        return skuPrice;
    }

    public void setSkuPrice(Double skuPrice) {
        this.skuPrice = skuPrice;
    }

    public String getPic01() {
        return pic01;
    }

    public void setPic01(String pic01) {
        this.pic01 = pic01;
    }

    public Integer getSold() {
        return sold;
    }

    public void setSold(Integer sold) {
        this.sold = sold;
    }

    public Integer getSoldMonth() {
        return soldMonth;
    }

    public void setSoldMonth(Integer soldMonth) {
        this.soldMonth = soldMonth;
    }

    public Integer getSoldReturn() {
        return soldReturn;
    }

    public void setSoldReturn(Integer soldReturn) {
        this.soldReturn = soldReturn;
    }

    public Integer getSoldReturnMonth() {
        return soldReturnMonth;
    }

    public void setSoldReturnMonth(Integer soldReturnMonth) {
        this.soldReturnMonth = soldReturnMonth;
    }

    public Double getPrice() {
        return price;
    }

    public void setPrice(Double price) {
        this.price = price;
    }

    public Double getMarketPrice() {
        return marketPrice;
    }

    public void setMarketPrice(Double marketPrice) {
        this.marketPrice = marketPrice;
    }

    public Double getSalePrice() {
        return salePrice;
    }

    public void setSalePrice(Double salePrice) {
        this.salePrice = salePrice;
    }

    public int getCommentCount() {
        return commentCount;
    }

    public void setCommentCount(int commentCount) {
        this.commentCount = commentCount;
    }

    public Long getGoodsId() {
        return goodsId;
    }

    public void setGoodsId(Long goodsId) {
        this.goodsId = goodsId;
    }

    public String getProductURL() {
        return productURL;
    }

    public void setProductURL(String productURL) {
        this.productURL = productURL;
    }
}
