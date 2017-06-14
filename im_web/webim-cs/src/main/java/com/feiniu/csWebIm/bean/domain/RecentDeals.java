package com.feiniu.csWebIm.bean.domain;
import java.util.List;

/**
 * Created by chenwuming on 2015/11/16.
 */
public class RecentDeals {
    private OrderDetail orderDetail;
    private List<PackageList> packageList;

    public OrderDetail getOrderDetail() {
        return orderDetail;
    }

    public void setOrderDetail(OrderDetail orderDetail) {
        this.orderDetail = orderDetail;
    }

    public List<PackageList> getPackageList() {
        return packageList;
    }

    public void setPackageList(List<PackageList> packageList) {
        this.packageList = packageList;
    }
}
