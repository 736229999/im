package com.feiniu.webim.bean.request;

/**
 * Created by zhangmin on 2015/12/30.
 */
public class OrderMallRequest {
    private Integer orderType;
    private Integer curPage = 1;
    private Integer pageRows = 20;
    private Integer rowFrom = -1;
    private Integer rowTo = -1;
    private String merchantId;
    private String memGuid;
    /*
    订单状态：未付款：1;         待发货：2,7,10,11,12,13,-4;
          已发货：3，-1，-3;     已完成：4，6;                已取消：5，9，-2
  */
    private Integer[] noPayStatus = new Integer[]{1};        //未付款总数
    private Integer[] noDeliveryStatus = new Integer[]{2, 7, 10, 11, 12, 13, -4};    //待发货总数
    private Integer[] deliveryStatus = new Integer[]{3, -1, -3};    //已发货总数
    private Integer[] allFinishStatus = new Integer[]{4, 6};    //已完成
    private Integer[] cancelledStatus = new Integer[]{5, 9, -2};    //已取消的订单数

    private Integer queryByOrder = 4;

    public Integer getOrderType() {
        return orderType;
    }

    public void setOrderType(Integer orderType) {
        this.orderType = orderType;
    }

    public Integer getCurPage() {
        return curPage;
    }

    public void setCurPage(Integer curPage) {
        this.curPage = curPage;
    }

    public Integer getPageRows() {
        return pageRows;
    }

    public void setPageRows(Integer pageRows) {
        this.pageRows = pageRows;
    }

    public Integer getRowFrom() {
        return rowFrom;
    }

    public void setRowFrom(Integer rowFrom) {
        this.rowFrom = rowFrom;
    }

    public Integer getRowTo() {
        return rowTo;
    }

    public void setRowTo(Integer rowTo) {
        this.rowTo = rowTo;
    }

    public String getMerchantId() {
        return merchantId;
    }

    public void setMerchantId(String merchantId) {
        this.merchantId = merchantId;
    }

    public String getMemGuid() {
        return memGuid;
    }

    public void setMemGuid(String memGuid) {
        this.memGuid = memGuid;
    }

    public Integer[] getNoPayStatus() {
        return noPayStatus;
    }

    public void setNoPayStatus(Integer[] noPayStatus) {
        this.noPayStatus = noPayStatus;
    }


    public Integer[] getNoDeliveryStatus() {
        return noDeliveryStatus;
    }

    public void setNoDeliveryStatus(Integer[] noDeliveryStatus) {
        this.noDeliveryStatus = noDeliveryStatus;
    }

    public Integer[] getDeliveryStatus() {
        return deliveryStatus;
    }

    public void setDeliveryStatus(Integer[] deliveryStatus) {
        this.deliveryStatus = deliveryStatus;
    }

    public Integer[] getAllFinishStatus() {
        return allFinishStatus;
    }

    public void setAllFinishStatus(Integer[] allFinishStatus) {
        this.allFinishStatus = allFinishStatus;
    }

    public Integer[] getCancelledStatus() {
        return cancelledStatus;
    }

    public void setCancelledStatus(Integer[] cancelledStatus) {
        this.cancelledStatus = cancelledStatus;
    }

    public Integer getQueryByOrder() {
        return queryByOrder;
    }

    public void setQueryByOrder(Integer queryByOrder) {
        this.queryByOrder = queryByOrder;
    }
}
