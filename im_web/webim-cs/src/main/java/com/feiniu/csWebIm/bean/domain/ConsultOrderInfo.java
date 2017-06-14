package com.feiniu.csWebIm.bean.domain;
import java.util.List;

/**
 * Created by chenwuming on 2015/11/16.
 */
public class ConsultOrderInfo {
    OrderConsultDetail orderDetail;
    List<ConsultPackageList> packageList;

    public OrderConsultDetail getOrderDetail() {
        return orderDetail;
    }

    public void setOrderDetail(OrderConsultDetail orderDetail) {
        this.orderDetail = orderDetail;
    }

    public List<ConsultPackageList> getPackageList() {
        return packageList;
    }

    public void setPackageList(List<ConsultPackageList> packageList) {
        this.packageList = packageList;
    }
}
