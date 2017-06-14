package com.feiniu.webim.bean.domain;

import java.util.List;

/**
 * Created by zhangmin on 2016/1/6.
 */
public class OrderQueryResult {
    private Integer curPage;
    private Integer pageRows;
    private Integer totalRows;
    private Integer noPay;
    private Integer noDelivery;
    private Integer delivery;
    private Integer allFinish;
    private Integer cancelled;
    private Double allPrice;  //已完成订单金额和
    private Double allFreight; //已完成订单运费和
    private List<OrderMall> orderMallList;

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

    public Integer getTotalRows() {
        return totalRows;
    }

    public void setTotalRows(Integer totalRows) {
        this.totalRows = totalRows;
    }

    public Integer getNoPay() {
        return noPay;
    }

    public void setNoPay(Integer noPay) {
        this.noPay = noPay;
    }

    public Integer getNoDelivery() {
        return noDelivery;
    }

    public void setNoDelivery(Integer noDelivery) {
        this.noDelivery = noDelivery;
    }

    public Integer getDelivery() {
        return delivery;
    }

    public void setDelivery(Integer delivery) {
        this.delivery = delivery;
    }

    public Integer getAllFinish() {
        return allFinish;
    }

    public void setAllFinish(Integer allFinish) {
        this.allFinish = allFinish;
    }

    public Integer getCancelled() {
        return cancelled;
    }

    public void setCancelled(Integer cancelled) {
        this.cancelled = cancelled;
    }

    public Double getAllPrice() {
        return allPrice;
    }

    public void setAllPrice(Double allPrice) {
        this.allPrice = allPrice;
    }

    public Double getAllFreight() {
        return allFreight;
    }

    public void setAllFreight(Double allFreight) {
        this.allFreight = allFreight;
    }

    public List<OrderMall> getOrderMallList() {
        return orderMallList;
    }

    public void setOrderMallList(List<OrderMall> orderMallList) {
        this.orderMallList = orderMallList;
    }
}
