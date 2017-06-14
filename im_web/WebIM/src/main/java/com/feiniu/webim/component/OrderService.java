package com.feiniu.webim.component;

import com.fasterxml.jackson.databind.JsonNode;
import com.feiniu.webim.bean.domain.*;
import com.feiniu.webim.bean.request.OrderMallRequest;
import com.feiniu.webim.bean.request.OrderRemarkRequest;
import com.feiniu.webim.config.ConfigManager;
import com.feiniu.webim.util.general.StringUtil;
import com.feiniu.webim.util.json.JSONUtil;
import com.feiniu.webim.util.json.JsonNodeUtil;
import com.feiniu.webim.util.web.HttpClientUtils;
import org.apache.log4j.Logger;

import java.io.IOException;
import java.util.*;

/**
 * Created by wangchen on 2015/3/3.
 */
public class OrderService {
    public static final OrderService instance = new OrderService();

    private static final ConvertService CONVERT_SERVICE = ConvertService.CONVERT_SERVICE;
    private static final List<ConvertService.ConvertRule> CONVERT_GOODS_PIC_URL =
            Arrays.asList(ConvertService.ConvertRule.MEKE_HEAD_IMG_ABSOLUTE, ConvertService.ConvertRule.TRANSFORM_OLD_MERCHANT_PHOTO_FROM_EXTERNAL);

    private OrderService() {
    }

    private static final String SERVICE_ORDER_API_URL = ConfigManager.getProperty("service_order");
    private static final String SERVICE_ORDER_QUERY_URL = ConfigManager.getProperty("service_order_query");
    private static final String SERVICE_ORDER_REMARK_URL = ConfigManager.getProperty("service_order_remark");
    private static final String SUCCSS_CODE = "100";
    private static final int DEFAULT_PIC_URL_AMOUNT = 4;

    private static final Logger LOGGER = Logger.getLogger(OrderService.class);

    // 订单旧接口， 后期要废弃
    public UserRelatedOrderInfo getOrderInfoForUser(String userID, long merchantID) {
        Map<String, Object> params = new HashMap<>();
        params.put("memberId", userID);
        params.put("merchantId", merchantID);
        Map<String, Object> finalParams = new HashMap<>();
        finalParams.put("request", JSONUtil.toJsonString(params));

        UserRelatedOrderInfo userRelatedOrderInfo = new UserRelatedOrderInfo();
        userRelatedOrderInfo.setMerchantID(merchantID);
        userRelatedOrderInfo.setRecentlyTradePicUrlList(new ArrayList<String>());
        userRelatedOrderInfo.setRecentlyTradeGoodUrlList(new ArrayList<String>());

        try {
            String response = HttpClientUtils.excutePost(SERVICE_ORDER_API_URL, finalParams);
            JsonNode jsonNode = JSONUtil.toNode(response);

            if (jsonNode != null && SUCCSS_CODE.equals(JsonNodeUtil.getString(jsonNode, "code"))
                    && jsonNode.hasNonNull("result")) {
                JsonNode resultNode = jsonNode.get("result");
                Iterator<JsonNode> itemIterator = resultNode.get("itemList").elements();

                if (itemIterator != null) {
                    while (itemIterator.hasNext()) {
                        JsonNode currentItemNode = itemIterator.next();
                        if (userRelatedOrderInfo.getRecentlyTradeGoodUrlList().size() != DEFAULT_PIC_URL_AMOUNT) {
                            userRelatedOrderInfo.getRecentlyTradeGoodUrlList().add(JsonNodeUtil.getString(currentItemNode, "sourceUrl", ""));
                            userRelatedOrderInfo.getRecentlyTradePicUrlList().add(JsonNodeUtil.getString(currentItemNode, "productMainUrl", ""));
                        }
                    }
                }

                userRelatedOrderInfo.setTotalTradeNum(JsonNodeUtil.getInteger(resultNode, "count", 0));
                userRelatedOrderInfo.setTotalConsumeAmount(JsonNodeUtil.getDouble(resultNode, "totalPrice", 0.0));
            }
        } catch (IOException e) {
            LOGGER.info("获取订单接口失败");
            throw new RuntimeException(e);
        }

        return userRelatedOrderInfo;
    }

    //订单状态转化供前台使用
    private Integer OrderStatusConvert(Integer initStatus) {
         /*
        订单状态：未付款：1;         待发货：2,7,10,11,12,13,-4;
        已发货：3，-1，-3;     已完成：4，6;                已取消：5，9，-2
        */
        Integer finalStauts = 0;
        switch (initStatus) {
            case 1: {
                finalStauts = 101;
                break;
            }
            case 2:
            case 7:
            case 10:
            case 11:
            case 12:
            case 13:
            case -4: {
                finalStauts = 102;
                break;
            }
            case 3:
            case -1:
            case -3: {
                finalStauts = 103;
                break;
            }
            case 4:
            case 6: {
                finalStauts = 104;
                break;
            }
            case 5:
            case 9:
            case -2: {
                finalStauts = 105;
                break;
            }
        }
        return finalStauts;
    }

    //订单查询新接口，和商家后台订单查询一个接口
    public OrderQueryResult QueryOrderByCondition(OrderMallRequest orderMallRequest) {
        List<Integer> multOgsStatus = new ArrayList<>();
        switch (orderMallRequest.getOrderType()) {
            case 1://未付款、未发货、已发货
                multOgsStatus.addAll(Arrays.asList(orderMallRequest.getNoPayStatus()));
                multOgsStatus.addAll(Arrays.asList(orderMallRequest.getNoDeliveryStatus()));
                multOgsStatus.addAll(Arrays.asList(orderMallRequest.getDeliveryStatus()));
                break;
            case 2://已完成
                multOgsStatus.addAll(Arrays.asList(orderMallRequest.getAllFinishStatus()));
                break;
            case 3://已取消
                multOgsStatus.addAll(Arrays.asList(orderMallRequest.getCancelledStatus()));
                break;
            case 4: //all
                multOgsStatus.addAll(Arrays.asList(orderMallRequest.getNoPayStatus()));
                multOgsStatus.addAll(Arrays.asList(orderMallRequest.getNoDeliveryStatus()));
                multOgsStatus.addAll(Arrays.asList(orderMallRequest.getDeliveryStatus()));
                multOgsStatus.addAll(Arrays.asList(orderMallRequest.getAllFinishStatus()));
                multOgsStatus.addAll(Arrays.asList(orderMallRequest.getCancelledStatus()));
            default:
                break;
        }
        Map<String, Object> params = new HashMap<>();
        //default value
        params.put("queryType", "page");
        params.put("noPayStatus", Arrays.asList(orderMallRequest.getNoPayStatus()));
        params.put("noDeliveryStatus", Arrays.asList(orderMallRequest.getNoDeliveryStatus()));
        params.put("deliveryStatus", Arrays.asList(orderMallRequest.getDeliveryStatus()));
        params.put("allFinishStatus", Arrays.asList(orderMallRequest.getAllFinishStatus()));
        params.put("cancelledStatus", Arrays.asList(orderMallRequest.getCancelledStatus()));
        //其他参数
        params.put("queryByOrder", orderMallRequest.getQueryByOrder());
        params.put("curPage", orderMallRequest.getCurPage());
        params.put("pageRows", orderMallRequest.getPageRows());
        params.put("multOgsStatus", multOgsStatus);
        params.put("merchantId", orderMallRequest.getMerchantId());
        params.put("memGuid", orderMallRequest.getMemGuid());

        OrderQueryResult orderQueryResult = new OrderQueryResult();
        List<OrderMall> orderMallList = new ArrayList<OrderMall>();
        String response = null;
        try {
            response = HttpClientUtils.excutePostJson(SERVICE_ORDER_QUERY_URL, params);
            JsonNode jsonNode = JSONUtil.toNode(response);
            if (jsonNode != null && jsonNode.hasNonNull("list")) {
                //总交易金额和运费
                Double allPrice = new Double(0.0), allFreight = new Double(0.0);
                //解析分页、各类订单总数量
                orderQueryResult.setCurPage(jsonNode.get("curPage").asInt());
                orderQueryResult.setPageRows(jsonNode.get("pageRows").asInt());
                orderQueryResult.setTotalRows(jsonNode.get("totalRows").asInt());
                orderQueryResult.setNoPay(jsonNode.get("noPay").asInt());
                orderQueryResult.setNoDelivery(jsonNode.get("noDelivery").asInt());
                orderQueryResult.setDelivery(jsonNode.get("delivery").asInt());
                orderQueryResult.setAllFinish(jsonNode.get("allFinish").asInt());
                orderQueryResult.setCancelled(jsonNode.get("cancelled").asInt());

                //list层，解析订单详细信息
                Iterator<JsonNode> itemIterator = jsonNode.get("list").elements();
                if (itemIterator != null) {
                    while (itemIterator.hasNext()) {
                        OrderMall orderMall = new OrderMall();
                        JsonNode currentItemNode = itemIterator.next();
                        //订单基本信息
                        orderMall.setOrderNo(JsonNodeUtil.getString(currentItemNode, "orderNo", ""));
                        orderMall.setOgsSeq(JsonNodeUtil.getString(currentItemNode, "ogsSeq", ""));
                        orderMall.setInsDt(JsonNodeUtil.getDate(currentItemNode, "insDt").getTime() / 1000);
                        orderMall.setPayDt(JsonNodeUtil.getDate(currentItemNode, "payDt").getTime() / 1000);
                        orderMall.setTotalPrice(JsonNodeUtil.getDouble(currentItemNode, "totalPrice", 0.0));
                        orderMall.setFreight(JsonNodeUtil.getDouble(currentItemNode, "freight", 0.0));
                        orderMall.setMerchantRemark(JsonNodeUtil.getString(currentItemNode, "merchantRemark", ""));
                        orderMall.setMemMsg(JsonNodeUtil.getString(currentItemNode, "memMsg", ""));
                        //收货人信息
                        JsonNode jsonNodeMember = currentItemNode.get("orderMember");
                        if (jsonNodeMember != null) {
                            orderMall.setMemberName(JsonNodeUtil.getString(jsonNodeMember, "memberName", ""));
                            orderMall.setMemberCellphone(JsonNodeUtil.getString(jsonNodeMember, "memberCellphone", ""));
                            orderMall.setMemberTel(JsonNodeUtil.getString(jsonNodeMember, "memberTel", ""));
                            orderMall.setMemberCellphone(StringUtil.makeTelephoneSecure(orderMall.getMemberCellphone()));
                            orderMall.setMemberProvince(JsonNodeUtil.getString(jsonNodeMember, "memberProvince", ""));
                            orderMall.setMemberCity(JsonNodeUtil.getString(jsonNodeMember, "memberCity", ""));
                            orderMall.setMemberPostArea(JsonNodeUtil.getString(jsonNodeMember, "memberPostArea", ""));
                            orderMall.setMemberPostCode(JsonNodeUtil.getString(jsonNodeMember, "memberPostCode", ""));
                            orderMall.setMemberAdd(JsonNodeUtil.getString(jsonNodeMember, "memberAdd", ""));
                        }
                        //订单状态
                        orderMall.setOgsStatus(OrderStatusConvert(JsonNodeUtil.getInteger(currentItemNode, "ogsStatus", 0)));
                        orderMall.setOgsStatusStr(JsonNodeUtil.getString(currentItemNode, "ogsStatusStr"));
                        if (orderMall.getOgsStatus().equals(104)) {
                            allPrice += orderMall.getTotalPrice();
                            allFreight += orderMall.getFreight();
                        }
                        //商品信息
                        List<OrderMallGoods> orderMallGoodsList = new ArrayList<OrderMallGoods>();
                        Iterator<JsonNode> itemIterator2 = currentItemNode.get("orderListSubList").elements();
                        if (itemIterator2 != null) {
                            while (itemIterator2.hasNext()) {
                                OrderMallGoods orderMallGoods = new OrderMallGoods();
                                JsonNode currentItemNode2 = itemIterator2.next();
                                orderMallGoods.setProductName(JsonNodeUtil.getString(currentItemNode2, "productName", ""));
                                orderMallGoods.setSkuId(JsonNodeUtil.getLong(currentItemNode2, "skuId", 0l));
                                //图片url需要convert
                                orderMallGoods.setProductMainUrl(
                                        CONVERT_SERVICE.doConvert(JsonNodeUtil.getString(currentItemNode2, "productMainUrl", ""),
                                                CONVERT_GOODS_PIC_URL)
                                );
                                orderMallGoods.setColorprop(JsonNodeUtil.getString(currentItemNode2, "colorprop", ""));
                                orderMallGoods.setSalerprop(JsonNodeUtil.getString(currentItemNode2, "salerprop", ""));
                                orderMallGoods.setProductQty(JsonNodeUtil.getInteger(currentItemNode2, "productQty", 1));
                                orderMallGoods.setProductPrice(JsonNodeUtil.getBigDecimal(currentItemNode2, "productPrice"));
                                orderMallGoodsList.add(orderMallGoods);
                            }
                        }
                        orderMall.setGoodsList(orderMallGoodsList);
                        orderMallList.add(orderMall);
                    }

                }
                orderQueryResult.setAllPrice(allPrice);
                orderQueryResult.setAllFreight(allFreight);
                orderQueryResult.setOrderMallList(orderMallList);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return orderQueryResult;
    }


    //卖家写订单备注
    public boolean remarkOrder(OrderRemarkRequest orderRemarkRequest) {

        Map<String, Object> params = new HashMap<>();
        //default value
        params.put("ogs_seqs", orderRemarkRequest.getOgsSeqs());
        params.put("merchant_remark", orderRemarkRequest.getRemark());
        params.put("mod_usr", orderRemarkRequest.getModUsr());
        try {
            String response = HttpClientUtils.excutePostJson(SERVICE_ORDER_REMARK_URL, params);
            JsonNode jsonNode = JSONUtil.toNode(response);
            if (!jsonNode.get("code").asText().equals("200")) {
                return false;
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return true;
    }

}
