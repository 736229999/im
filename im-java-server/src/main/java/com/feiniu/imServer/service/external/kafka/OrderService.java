package com.feiniu.imServer.service.external.kafka;

import com.feiniu.imServer.bean.UserInfo;
import com.feiniu.imServer.bean.config.DependentServicePaths;
import com.feiniu.imServer.bean.pb.Base;
import com.feiniu.imServer.bean.pb.Improtocol;
import com.feiniu.imServer.bean.pb.Improtocol.DataPacket;
import com.feiniu.imServer.bean.request.GetReturnOrderRequest;
import com.feiniu.imServer.bean.request.NewCommentRequest;
import com.feiniu.imServer.service.base.ImageConvertService;
import com.feiniu.imServer.util.FormatUtils;
import com.feiniu.imServer.util.JsonUtil;
import com.feiniu.imServer.util.json.JsonNodeUtil;
import com.feiniu.imServer.util.web.HttpUtils;
import com.google.protobuf.ByteString;
import com.google.protobuf.GeneratedMessageLite;
import org.apache.commons.lang.StringUtils;
import org.codehaus.jackson.JsonNode;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import javax.annotation.PostConstruct;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.sql.Timestamp;
import java.util.*;

/**
 * Created by zhangmin on 2016/2/17.
 */
@Service
public class OrderService {
    @Autowired
    private ImageConvertService imageConvertService;
    @Autowired
    private DependentServicePaths dependentServicePaths;
    @Autowired
    private UserService userService;

    private static final Logger LOGGER = LoggerFactory.getLogger(OrderService.class);
    //传给im-server时，需要转码
    private static final String TO_C_PLUS_SERVER_ENCODING = "gbk";
    //返回正常 状态码
    private static final int CODE_SUCCESS = 200;
    //返回正常 描述
    private static final String MSG_SUCCESS = "success";
    //默认取商城包裹的前三个商品
    private static final int MAX_ITEM_TO_SEND = 3;
    //商城商品类型
    private static final int TYPE_MERCHANT = 2;

    private String GET_RETURN_ORDER_LIST;
    private String GET_NEW_ORDER_LIST;

    @PostConstruct
    private void init() {
        GET_RETURN_ORDER_LIST = dependentServicePaths.getOrderReturnServicePath() + "OrderReturn/orderReturn/getReturnInfo";
        GET_NEW_ORDER_LIST = dependentServicePaths.getQueryNewOrderServicePath() + "OrderQuery/customerdesk/customerDeskOrderDetail";
    }

    /**
     * 订单查询
     *
     * @param memGuid
     * @param ogNo
     * @return
     */
    public JsonNode queryOrder(String memGuid, String ogNo) {
        Map<String, Object> datas = new HashMap<>(2);
        datas.put("ogNo", ogNo);
        datas.put("memGuid", memGuid);

        Map<String, Object> params = new HashMap<>(1);
        params.put("data", JsonUtil.toJsonString(datas));
        String response = null;

        try {
            response = HttpUtils.excutePost(GET_NEW_ORDER_LIST, params);

            JsonNode jsonNode = JsonUtil.toNode(response);
            if (jsonNode != null && jsonNode.get("code").asText().equals("200")
                    && jsonNode.get("msg").asText().equals("success")) {
                return jsonNode.get("data");
            } else {
                LOGGER.error("查询订单返回错误数据：" + response);
            }
        } catch (IOException e) {
            LOGGER.error("查询订单信息失败,网络发生故障：", e);
        }

        return null;
    }

    /**
     * 给一个订单中的所有商家发送订单消息
     */
    public void sendNewOrderAllMerchant(JsonNode jsonNodeData) throws UnsupportedEncodingException {
        Improtocol.NewOrder.Builder newOrderBuilder = Improtocol.NewOrder.newBuilder();
        if (jsonNodeData != null) {
            JsonNode jsonNodeOrderDetail = jsonNodeData.get("orderDetail");
            if (jsonNodeOrderDetail != null) {
                String insertDateString = JsonNodeUtil.getString(jsonNodeOrderDetail, "insertDate", "");
                newOrderBuilder.setOgNo(JsonNodeUtil.getString(jsonNodeOrderDetail, "ogNo", ""));
                newOrderBuilder.setTotalPay(JsonNodeUtil.getDouble(jsonNodeOrderDetail, "totalPay", 0.0));
                newOrderBuilder.setInsertDate(insertDateString);

                UserInfo userInfo = userService.getUserInfo(JsonNodeUtil.getString(jsonNodeOrderDetail, "memGuid"));
                String buyerName = userInfo.getNickname();
                if (StringUtils.isEmpty(buyerName)) {
                    buyerName = userInfo.getUsername();
                }

                newOrderBuilder.setBuyernameBytes(ByteString.copyFrom(buyerName, TO_C_PLUS_SERVER_ENCODING));
                newOrderBuilder.setMemGuid(userInfo.getGuid());
            }

            //packageList
            JsonNode jsonNodePackageList = jsonNodeData.get("packageList");
            if (jsonNodePackageList != null) {
                //每个package要清空一次
                Iterator<JsonNode> packageIterator = jsonNodePackageList.iterator();
                if (packageIterator != null) {
                    while (packageIterator.hasNext()) {
                        //package
                        JsonNode jsonNodePackage = packageIterator.next();
                        //非商城订单，pass
                        if (jsonNodePackage.get("type").asInt() != TYPE_MERCHANT) {
                            continue;
                        }
                        newOrderBuilder.setPackNo(jsonNodePackage.get("packNo").toString())
                                .setFreight(jsonNodePackage.get("freight").asDouble());
                        //itemList
                        JsonNode jsonNodeItemList = jsonNodePackage.get("itemList");
                        if (jsonNodeItemList != null) {
                            Iterator<JsonNode> itemIterator = jsonNodeItemList.iterator();
                            if (itemIterator != null) {
                                int itemNum = 0;
                                //清空goodList，因为后面是addGoodList
                                newOrderBuilder.clearGoodslist();
                                while (itemIterator.hasNext() && itemNum++ < MAX_ITEM_TO_SEND) {
                                    //item
                                    JsonNode jsonNodeItem = itemIterator.next();
                                    Improtocol.OrderGoodsInfo.Builder goodInfoBuilder = Improtocol.OrderGoodsInfo.newBuilder();
                                    newOrderBuilder.setMerchantId(JsonNodeUtil.getString(jsonNodeItem, "merchantId"));

                                    String productName = JsonNodeUtil.getString(jsonNodeItem, "productName", "");
                                    goodInfoBuilder.setProductNameBytes(ByteString.copyFrom(productName, TO_C_PLUS_SERVER_ENCODING));

                                    goodInfoBuilder.setPicUrl(imageConvertService.getConvertedMerchantImage(
                                            JsonNodeUtil.getString(jsonNodeItem, "picUrl", "")));

                                    goodInfoBuilder.setQty(JsonNodeUtil.getInteger(jsonNodeItem, "qty", 1));
                                    goodInfoBuilder.setSubTotalPrice(JsonNodeUtil.getDouble(jsonNodeItem, "subTotalPrice", 0.0));

                                    String color = JsonNodeUtil.getString(jsonNodeItem, "color");
                                    String size = JsonNodeUtil.getString(jsonNodeItem, "size");
                                    color = StringUtils.isNotEmpty(color) ? color : "";
                                    size = StringUtils.isNotEmpty(size) ? size : "";

                                    goodInfoBuilder.setColorBytes(ByteString.copyFrom(color, TO_C_PLUS_SERVER_ENCODING));
                                    goodInfoBuilder.setSizeBytes(ByteString.copyFrom(size, TO_C_PLUS_SERVER_ENCODING));
                                    Improtocol.OrderGoodsInfo orderGoodsInfo = goodInfoBuilder.build();
                                    newOrderBuilder.addGoodslist(orderGoodsInfo);
                                }
                            }
                        }
                        //imtelist搞完就发送一次,单个商家
                        Improtocol.NewOrder newOrderPerMerchant = newOrderBuilder.build();

                        sendMessageToServer(newOrderPerMerchant, Base.EuMsgPush.MP_NEWORDER);
                    }
                }

            }
        }

    }

    /**
     * 发送支付订单 提醒
     *
     * @param node
     */
    public void sendPayOrderNotify(JsonNode node) throws UnsupportedEncodingException {
        if (node != null && !node.isNull()) {
            Improtocol.PayOrder.Builder payOrder = Improtocol.PayOrder.newBuilder();
            JsonNode orderDetailNode = node.get("orderDetail");
            UserInfo userInfo = userService.getUserInfo(JsonNodeUtil.getString(orderDetailNode, "memGuid"));
            String buyerName = userInfo.getNickname();
            if (StringUtils.isEmpty(buyerName)) {
                buyerName = userInfo.getUsername();
            }

            payOrder.setOgNo(JsonNodeUtil.getString(orderDetailNode, "ogNo", ""));
            payOrder.setTotalPay(JsonNodeUtil.getDouble(orderDetailNode, "totalPay", 0.0));
            payOrder.setInsertDate(JsonNodeUtil.getString(orderDetailNode, "insertDate", ""));
            payOrder.setMemGuid(userInfo.getGuid());

            payOrder.setBuyernameBytes(ByteString.copyFrom(buyerName.getBytes(TO_C_PLUS_SERVER_ENCODING)));

            JsonNode packageListNode = node.get("packageList");
            if (packageListNode != null && !packageListNode.isNull() && packageListNode.isArray()) {
                Iterator<JsonNode> packageListIterator = packageListNode.iterator();
                //遍历包裹
                while (packageListIterator.hasNext()) {
                    JsonNode packageNode = packageListIterator.next();

                    if (packageNode.get("type").asInt() != TYPE_MERCHANT) {
                        continue;
                    }

                    payOrder.setPackQueryNo(JsonNodeUtil.getString(packageNode, "packQueryNo", ""));
                    payOrder.setFreight(JsonNodeUtil.getDouble(packageNode, "freight", 0.0));
                    payOrder.setPayDate(FormatUtils.formatTime(new Date()));
                    int numOfItemListForCurrentPackage = 0;

                    JsonNode itemListNode = packageNode.get("itemList");
                    if (itemListNode != null && !itemListNode.isNull() && itemListNode.isArray()) {
                        Iterator<JsonNode> itemListIterator = itemListNode.iterator();
                        Map<String, List<Improtocol.OrderGoodsInfo>> goodsOfmerchantId = new HashMap<>(1);
                        Improtocol.OrderGoodsInfo.Builder builder = Improtocol.OrderGoodsInfo.newBuilder();

                        //遍历每个包裹里面的商品
                        while (itemListIterator.hasNext()) {
                            JsonNode itemNode = itemListIterator.next();
                            int isMall = JsonNodeUtil.getInteger(itemNode, "isMall");
                            if (isMall != 1) {
                                continue;
                            } else {
                                if (numOfItemListForCurrentPackage == MAX_ITEM_TO_SEND) {
                                    break;
                                } else {
                                    numOfItemListForCurrentPackage++;
                                }
                            }

                            builder.setProductName(JsonNodeUtil.getString(itemNode, "productName", ""));
                            builder.setPicUrl(JsonNodeUtil.getString(itemNode, "picUrl", ""));
                            builder.setQty(JsonNodeUtil.getInteger(itemNode, "qty", 0));
                            builder.setSubTotalPrice(JsonNodeUtil.getDouble(itemNode, "subTotalPrice", 0.0));
                            builder.setCoupons(JsonNodeUtil.getDouble(itemNode, "coupons", 0.0));
                            builder.setVoucher(JsonNodeUtil.getDouble(itemNode, "voucher", 0.0));
                            builder.setPromote(JsonNodeUtil.getDouble(itemNode, "promote", 0.0));

                            String color = JsonNodeUtil.getString(itemNode, "color");
                            String size = JsonNodeUtil.getString(itemNode, "size");
                            color = StringUtils.isNotEmpty(color) ? color : "";
                            size = StringUtils.isNotEmpty(size) ? size : "";

                            builder.setPicUrl(imageConvertService.getConvertedMerchantImage(builder.getPicUrl()));
                            builder.setProductNameBytes(ByteString.copyFrom(builder.getProductName().getBytes(TO_C_PLUS_SERVER_ENCODING)));
                            builder.setColorBytes(ByteString.copyFrom(color.getBytes(TO_C_PLUS_SERVER_ENCODING)));
                            builder.setSizeBytes(ByteString.copyFrom(size.getBytes(TO_C_PLUS_SERVER_ENCODING)));

                            String merchantId = JsonNodeUtil.getString(itemNode, "merchantId");
                            List<Improtocol.OrderGoodsInfo> list = goodsOfmerchantId.get(merchantId);
                            if (list == null) {
                                list = new ArrayList<>(1);
                                goodsOfmerchantId.put(merchantId, list);
                            }
                            list.add(builder.build());
                        }

                        //按商家维度依次发送消息
                        for (Map.Entry<String, List<Improtocol.OrderGoodsInfo>> entry : goodsOfmerchantId.entrySet()) {
                            payOrder.setMerchantId(entry.getKey());
                            payOrder.clearGoodslist();
                            payOrder.addAllGoodslist(entry.getValue());

                            sendMessageToServer(payOrder.build(), Base.EuMsgPush.MP_PAYORDER);
                        }
                    }
                }
            }
        }
    }

    /**
     * 新评论 提醒
     */
    public void sendCommentNotify(JsonNode node, NewCommentRequest newCommentRequest) throws UnsupportedEncodingException {
        if (node != null && !node.isNull()) {
            Improtocol.GoodsComment.Builder goodsComment = Improtocol.GoodsComment.newBuilder();
            JsonNode orderDetailNode = node.get("orderDetail");
            UserInfo userInfo = userService.getUserInfo(JsonNodeUtil.getString(orderDetailNode, "memGuid"));
            String buyerName = userInfo.getNickname();
            if (StringUtils.isEmpty(buyerName)) {
                buyerName = userInfo.getUsername();
            }

            goodsComment.setOgNo(JsonNodeUtil.getString(orderDetailNode, "ogNo", ""));
            goodsComment.setInsertDate(JsonNodeUtil.getString(orderDetailNode, "insertDate", ""));
            goodsComment.setCommentDate(FormatUtils.formatTime(new Date()));
            goodsComment.setMemGuid(userInfo.getGuid());

            goodsComment.setBugernameBytes(ByteString.copyFrom(newCommentRequest.getBuyerName().getBytes(TO_C_PLUS_SERVER_ENCODING)));
            goodsComment.setEvaluatecontextBytes(ByteString.copyFrom(newCommentRequest.getComment().getBytes(TO_C_PLUS_SERVER_ENCODING)));

            JsonNode packageListNode = node.get("packageList");
            if (packageListNode != null && !packageListNode.isNull() && packageListNode.isArray()) {
                Iterator<JsonNode> packageListIterator = packageListNode.iterator();
                //遍历包裹
                while (packageListIterator.hasNext()) {
                    JsonNode packageNode = packageListIterator.next();
                    if (packageNode.get("type").asInt() != TYPE_MERCHANT) {
                        continue;
                    }

                    JsonNode itemListNode = packageNode.get("itemList");
                    if (itemListNode != null && !itemListNode.isNull() && itemListNode.isArray()) {
                        Iterator<JsonNode> itemListIterator = itemListNode.iterator();
                        Improtocol.OrderGoodsInfo.Builder builder = Improtocol.OrderGoodsInfo.newBuilder();
                        int numOfItemListForCurrentPackage = 0;

                        //遍历每个包裹里面的商城商品
                        while (itemListIterator.hasNext()) {
                            JsonNode itemNode = itemListIterator.next();
                            int isMall = JsonNodeUtil.getInteger(itemNode, "isMall");
                            if (isMall != 1) {
                                continue;
                            } else {
                                if (numOfItemListForCurrentPackage == MAX_ITEM_TO_SEND) {
                                    break;
                                } else {
                                    numOfItemListForCurrentPackage++;
                                }
                            }

                            String skuId = JsonNodeUtil.getString(itemNode, "skuId");
                            if (skuId == null || !skuId.equals(newCommentRequest.getSkuId())) {
                                continue;
                            }

                            builder.setProductName(JsonNodeUtil.getString(itemNode, "productName", ""));
                            builder.setPicUrl(JsonNodeUtil.getString(itemNode, "picUrl", ""));
                            builder.setQty(JsonNodeUtil.getInteger(itemNode, "qty", 0));
                            builder.setSubTotalPrice(JsonNodeUtil.getDouble(itemNode, "subTotalPrice", 0.0));
                            builder.setCoupons(JsonNodeUtil.getDouble(itemNode, "coupons", 0.0));
                            builder.setVoucher(JsonNodeUtil.getDouble(itemNode, "voucher", 0.0));
                            builder.setPromote(JsonNodeUtil.getDouble(itemNode, "promote", 0.0));

                            String color = JsonNodeUtil.getString(itemNode, "color");
                            String size = JsonNodeUtil.getString(itemNode, "size");
                            color = StringUtils.isNotEmpty(color) ? color : "";
                            size = StringUtils.isNotEmpty(size) ? size : "";

                            builder.setPicUrl(imageConvertService.getConvertedMerchantImage(builder.getPicUrl()));
                            builder.setProductNameBytes(ByteString.copyFrom(builder.getProductName().getBytes(TO_C_PLUS_SERVER_ENCODING)));
                            builder.setColorBytes(ByteString.copyFrom(color.getBytes(TO_C_PLUS_SERVER_ENCODING)));
                            builder.setSizeBytes(ByteString.copyFrom(size.getBytes(TO_C_PLUS_SERVER_ENCODING)));

                            String merchantId = JsonNodeUtil.getString(itemNode, "merchantId");
                            goodsComment.setMerchantId(merchantId);
                            goodsComment.setGoods(builder.build());

                            sendMessageToServer(goodsComment.build(), Base.EuMsgPush.MP_GOODSCOMMENT);
                            break;
                        }
                    }
                }
            }
        }
    }

    /**
     * 发送退货消息
     *
     * @param getReturnOrderRequest
     */
    public JsonNode getReturnedOrder(GetReturnOrderRequest getReturnOrderRequest) {
        Map<String, Object> params = new HashMap<>();
        params.put("data", JsonUtil.toJsonString(getReturnOrderRequest));

        try {
            String response = HttpUtils.excutePost(GET_RETURN_ORDER_LIST, params);

            JsonNode resultNode = JsonUtil.toNode(response);
            if (resultNode != null && CODE_SUCCESS == JsonNodeUtil.getInteger(resultNode, "code")
                    && MSG_SUCCESS.equals(JsonNodeUtil.getString(resultNode, "msg"))) {
                return resultNode.get("data").get("returnInfo");
            } else {
                LOGGER.error("远程接口返回错误数据：", response);
            }
        } catch (IOException e) {
            LOGGER.error("请求退单信息失败,网络发生故障：", e);
        }

        return null;
    }

    public void sendReturnOrderInfo(JsonNode dataNode, GetReturnOrderRequest getReturnOrderRequest) throws UnsupportedEncodingException {
        if (dataNode != null && !dataNode.isNull() && dataNode.isArray()) {
            UserInfo userInfo = userService.getUserInfo(getReturnOrderRequest.getMemGuid());
            String buyerName = userInfo.getNickname();
            if (StringUtils.isEmpty(buyerName)) {
                buyerName = userInfo.getUsername();
            }

            Iterator<JsonNode> merchantsNode = dataNode.iterator();

            if (merchantsNode.hasNext()) {
                Improtocol.ReturnOrder.Builder returnOrderBuilder = Improtocol.ReturnOrder.newBuilder();

                while (merchantsNode.hasNext()) {
                    JsonNode merchantNode = merchantsNode.next();

                    //商家维度信息
                    returnOrderBuilder.setOgNo(JsonNodeUtil.getString(merchantNode, "ogNo"));
                    returnOrderBuilder.setOgsSeq(JsonNodeUtil.getString(merchantNode, "ogsSeq"));
                    returnOrderBuilder.setMerchantId(JsonNodeUtil.getString(merchantNode, "merchantId"));
                    returnOrderBuilder.setApplyDate(FormatUtils.formatTime(new Timestamp(JsonNodeUtil.getLong(merchantNode, "applyDt"))));
                    returnOrderBuilder.setApplyPrice(JsonNodeUtil.getDouble(merchantNode, "applyPrice"));
                    returnOrderBuilder.setBuyernameBytes(ByteString.copyFrom(buyerName.getBytes(TO_C_PLUS_SERVER_ENCODING)));
                    returnOrderBuilder.setMemGuid(userInfo.getGuid());

                    //商品维度信息
                    returnOrderBuilder.clearGoodslist();
                    JsonNode productInfoListNode = merchantNode.get("productInfos");
                    int numOfItemListForCurrentPackage = 0;

                    if (productInfoListNode != null && !productInfoListNode.isNull()
                            && productInfoListNode.isArray()) {
                        Iterator<JsonNode> productInfoIterator = productInfoListNode.iterator();
                        while (productInfoIterator.hasNext()) {
                            JsonNode productNode = productInfoIterator.next();
                            if (numOfItemListForCurrentPackage == MAX_ITEM_TO_SEND) {
                                break;
                            } else {
                                numOfItemListForCurrentPackage++;
                            }

                            Improtocol.ReturnGoodsInfo.Builder builder = Improtocol.ReturnGoodsInfo.newBuilder();

                            builder.setSkuId(JsonNodeUtil.getString(productNode, "skuId"));
                            builder.setProductPrice(JsonNodeUtil.getDouble(productNode, "productPrice"));
                            builder.setProductQty(JsonNodeUtil.getInteger(productNode, "productQty"));
                            builder.setReturnQty(JsonNodeUtil.getInteger(productNode, "returnQty"));
                            builder.setProductMainUrl(imageConvertService.getConvertedMerchantImage(
                                    JsonNodeUtil.getString(productNode, "productMainUrl")
                            ));
                            LOGGER.info(builder.getProductMainUrl());

                            String color = JsonNodeUtil.getString(productNode, "color");
                            String size = JsonNodeUtil.getString(productNode, "size");
                            color = color == null ? "" : color;
                            size = size == null ? "" : size;

                            builder.setColor(color).setSize(size);
                            builder.setReason(JsonNodeUtil.getString(productNode, "reason"));
                            builder.setAprnPromote(JsonNodeUtil.getDouble(productNode, "aprnPromote"));
                            builder.setAprnVoucher(JsonNodeUtil.getDouble(productNode, "aprnVoucher"));
                            builder.setShippingTax(JsonNodeUtil.getDouble(productNode, "shippingTax"));
                            builder.setOrigPrice(JsonNodeUtil.getDouble(productNode, "origPrice"));
                            builder.setAprnPoint(JsonNodeUtil.getDouble(productNode, "aprnPoint"));
                            builder.setAprnPointPrice(JsonNodeUtil.getDouble(productNode, "aprnPointPrice"));
                            builder.setAprnCash(JsonNodeUtil.getDouble(productNode, "aprnCash"));
                            builder.setRefundablePrice(JsonNodeUtil.getDouble(productNode, "refundablePrice"));
                            builder.setProductName(JsonNodeUtil.getString(productNode, "productName"));
                            builder.setRssSeq(JsonNodeUtil.getString(productNode, "rssSeq"));

                            builder.setReasonBytes(ByteString.copyFrom(builder.getReason().getBytes(TO_C_PLUS_SERVER_ENCODING)));
                            builder.setProductNameBytes(ByteString.copyFrom(builder.getProductName().getBytes(TO_C_PLUS_SERVER_ENCODING)));
                            builder.setColorBytes(ByteString.copyFrom(builder.getColor().getBytes(TO_C_PLUS_SERVER_ENCODING)));
                            builder.setSizeBytes(ByteString.copyFrom(builder.getSize().getBytes(TO_C_PLUS_SERVER_ENCODING)));

                            returnOrderBuilder.addGoodslist(builder);
                        }
                    }
                }

                sendMessageToServer(returnOrderBuilder.build(), Base.EuMsgPush.MP_RETURNORDER);
            }

        }
    }

    //发送消息给im-server
    private void sendMessageToServer(GeneratedMessageLite message, Base.EuMsgPush msgPush) {
        //MsgPushInd
        Improtocol.MsgPushInd.Builder msgPushBuilder = Improtocol.MsgPushInd.newBuilder();
        msgPushBuilder.setMpType(msgPush);
        msgPushBuilder.setMpData(message.toByteString());
        Improtocol.MsgPushInd msgPushInd = msgPushBuilder.build();

        //XT_HEAD_PB
        Improtocol.XT_HEAD_PB.Builder xtHeadPbBuilder = Improtocol.XT_HEAD_PB.newBuilder();
        xtHeadPbBuilder.setCmd(61441);
        Improtocol.XT_HEAD_PB xt_head_pb = xtHeadPbBuilder.build();

        //DataPacket
        DataPacket.Builder dataPacketbuilder = DataPacket.newBuilder();
        dataPacketbuilder.setHead(xt_head_pb);
        dataPacketbuilder.setData(msgPushInd.toByteString());
        DataPacket dataPacket = dataPacketbuilder.build();
        ZmqPubService.instance.sendMessage(dataPacket.toByteArray());
    }

}
