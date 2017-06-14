package com.feiniu.webim.bean.domain;

import java.math.BigDecimal;

/**
 * Created by zhangmin on 2015/12/29.
 */
public class OrderMallGoods {
    /**
     商品规格
     */
    private String productName;         // 商品名称/优惠名称
    private Long skuId;
    private String productMainUrl;      //商品SKU主图
    private String colorprop;           // SKU颜色属性
    private String salerprop;           // 尺码属性
    private Integer productQty;         // 商品数量
    private BigDecimal productPrice;    // 商品单价/优惠券金额（若为券，则为负数）

    public String getProductName() {
        return productName;
    }

    public void setProductName(String productName) {
        this.productName = productName;
    }

    public Long getSkuId() {
        return skuId;
    }

    public void setSkuId(Long skuId) {
        this.skuId = skuId;
    }

    public String getProductMainUrl() {
        return productMainUrl;
    }

    public void setProductMainUrl(String productMainUrl) {
        this.productMainUrl = productMainUrl;
    }

    public String getColorprop() {
        return colorprop;
    }

    public void setColorprop(String colorprop) {
        this.colorprop = colorprop;
    }

    public String getSalerprop() {
        return salerprop;
    }

    public void setSalerprop(String salerprop) {
        this.salerprop = salerprop;
    }

    public Integer getProductQty() {
        return productQty;
    }

    public void setProductQty(Integer productQty) {
        this.productQty = productQty;
    }

    public BigDecimal getProductPrice() {
        return productPrice;
    }

    public void setProductPrice(BigDecimal productPrice) {
        this.productPrice = productPrice;
    }
}
