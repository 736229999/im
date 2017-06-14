package com.feiniu.csWebIm.bean.domain;

/**
 * Created by Chenwuming on 2015/11/20.
 */


public class MoumouProductDetail {
    private ProductStatus Header;
    private ProductBody Body;
    private ProductInfo Info;

    public ProductStatus getHeader() {
        return Header;
    }

    public void setHeader(ProductStatus header) {
        Header = header;
    }

    public ProductBody getBody() {
        return Body;
    }

    public void setBody(ProductBody body) {
        Body = body;
    }

    public ProductInfo getInfo() {
        return Info;
    }

    public void setInfo(ProductInfo info) {
        Info = info;
    }
}
