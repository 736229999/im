package com.feiniu.imServer.bean.request;

/**
 * Author: morningking
 * Date: 2016/3/1
 * Contact: 243717042@qq.com
 */
public class NewCommentRequest {
    private String memGUID;
    private String buyerName;
    private String ogNo;
    private String commentDate;
    private String comment;
    private String skuId;

    public String getMemGUID() {
        return memGUID;
    }

    public void setMemGUID(String memGUID) {
        this.memGUID = memGUID;
    }

    public String getBuyerName() {
        return buyerName;
    }

    public void setBuyerName(String buyerName) {
        this.buyerName = buyerName;
    }

    public String getOgNo() {
        return ogNo;
    }

    public void setOgNo(String ogNo) {
        this.ogNo = ogNo;
    }

    public String getCommentDate() {
        return commentDate;
    }

    public void setCommentDate(String commentDate) {
        this.commentDate = commentDate;
    }

    public String getComment() {
        return comment;
    }

    public void setComment(String comment) {
        this.comment = comment;
    }

    public String getSkuId() {
        return skuId;
    }

    public void setSkuId(String skuId) {
        this.skuId = skuId;
    }
}
