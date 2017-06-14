package com.feiniu.webim.bean.domain;

import java.util.List;

/**
 * Created by zhangmin on 2015/12/29.
 */
public class OrderMall {

    //订单基本信息
    private String orderNo;        //订单编号
    private String ogsSeq;         //包裹号
//    private Date insDt;            // 订单创建时间
    private Long insDt;            // 订单创建时间
    private Double totalPrice;      // 子订单金额（商品总额+运费）
    private Double freight;         // 子订单运费
    private String merchantRemark; // 商家备注
    private String memMsg;         // 顾客留言

    //收货人信息
    private String 	memberName;
    private String 	memberCellphone;
    private String 	memberTel;
    private String 	memberPostArea;
    private String 	memberProvince;
    private String 	memberCity;
    private String 	memberPostCode;
    private String 	memberAdd;


    //订单状态
//    private Integer status;               //支付状态     1、未支付   2、部分支付 3、已支付 4、未退款 5、已退款
    private Integer ogsStatus;            //子订单状态 1、新订单、2、待发货、3、已发货、4、已完成? 5、已取消 6、申请退货 7、已退货
//    private Integer ogsDeliverystatus;    //发货状态   1、未发货 2、已发货 3、已收货 4、未退回 5、已退回 6、确认退回 7、无需退回
//    private Integer balanceStatus;        // 结算状态 1、未请款(默认) 2、待营管确认 3、待财务汇出 4、已汇出 5、付款完成
    private Long payDt ;                  // 付款时间
    private String ogsStatusStr;          //子订单状态 1、新订单、2、待发货、3、已发货、4、已完成? 5、已取消 6、申请退货 7、已退货

    //订单中商品信息
    List<OrderMallGoods> goodsList;

    public String getOrderNo() {
        return orderNo;
    }

    public void setOrderNo(String orderNo) {
        this.orderNo = orderNo;
    }

    public String getOgsSeq() {
        return ogsSeq;
    }

    public void setOgsSeq(String ogsSeq) {
        this.ogsSeq = ogsSeq;
    }

    public Long getInsDt() {
        return insDt;
    }

    public void setInsDt(Long insDt) {
        this.insDt = insDt;
    }

    public Double getTotalPrice() {
        return totalPrice;
    }

    public void setTotalPrice(Double totalPrice) {
        this.totalPrice = totalPrice;
    }

    public Double getFreight() {
        return freight;
    }

    public void setFreight(Double freight) {
        this.freight = freight;
    }

    public String getMerchantRemark() {
        return merchantRemark;
    }

    public void setMerchantRemark(String merchantRemark) {
        this.merchantRemark = merchantRemark;
    }

    public String getMemMsg() {
        return memMsg;
    }

    public void setMemMsg(String memMsg) {
        this.memMsg = memMsg;
    }

    public String getMemberName() {
        return memberName;
    }

    public void setMemberName(String memberName) {
        this.memberName = memberName;
    }

    public String getMemberCellphone() {
        return memberCellphone;
    }

    public void setMemberCellphone(String memberCellphone) {
        this.memberCellphone = memberCellphone;
    }

    public String getMemberTel() {
        return memberTel;
    }

    public void setMemberTel(String memberTel) {
        this.memberTel = memberTel;
    }

    public String getMemberCity() {
        return memberCity;
    }

    public void setMemberCity(String memberCity) {
        this.memberCity = memberCity;
    }

    public String getMemberPostCode() {
        return memberPostCode;
    }

    public void setMemberPostCode(String memberPostCode) {
        this.memberPostCode = memberPostCode;
    }

    public String getMemberPostArea() {
        return memberPostArea;
    }

    public void setMemberPostArea(String memberPostArea) {
        this.memberPostArea = memberPostArea;
    }

    public String getMemberProvince() {
        return memberProvince;
    }

    public void setMemberProvince(String memberProvince) {
        this.memberProvince = memberProvince;
    }

    public String getMemberAdd() {
        return memberAdd;
    }

    public void setMemberAdd(String memberAdd) {
        this.memberAdd = memberAdd;
    }

    public Integer getOgsStatus() {
        return ogsStatus;
    }

    public void setOgsStatus(Integer ogsStatus) {
        this.ogsStatus = ogsStatus;
    }

    public String getOgsStatusStr() {
        return ogsStatusStr;
    }

    public void setOgsStatusStr(String ogsStatusStr) {
        this.ogsStatusStr = ogsStatusStr;
    }

    public Long getPayDt() {
        return payDt;
    }

    public void setPayDt(Long payDt) {
        this.payDt = payDt;
    }

    public List<OrderMallGoods> getGoodsList() {
        return goodsList;
    }

    public void setGoodsList(List<OrderMallGoods> goodsList) {
        this.goodsList = goodsList;
    }
}
