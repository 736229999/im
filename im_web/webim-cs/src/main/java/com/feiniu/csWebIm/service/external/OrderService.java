package com.feiniu.csWebIm.service.external;

import com.feiniu.csWebIm.bean.vo.UserOrder;
import com.feiniu.csWebIm.consts.BusinessConfig;
import com.feiniu.csWebIm.exception.BusinessException;
import com.feiniu.csWebIm.utils.json.JsonNodeUtil;
import com.feiniu.csWebIm.utils.json.JsonUtil;
import com.feiniu.csWebIm.utils.web.HttpUtils;
import org.codehaus.jackson.JsonNode;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import javax.annotation.PostConstruct;
import java.io.IOException;
import java.util.*;

/**
 * Author: morningking
 * Date: 2015/11/23
 * Contact: 243717042@qq.com
 */
@Service
public class OrderService {
    @Autowired
    private BusinessConfig businessConfig;
    //判断订单号是否属于用户
    private String checkOrderIsUnderUserURL;
    //获取用户所有订单信息
    private String getAllOrderListURL;

    @PostConstruct
    private void init() {
        checkOrderIsUnderUserURL = businessConfig.getOrderConsultURL() + "OrderQuery/shanghai/v1/authMemberOrder";
        getAllOrderListURL = businessConfig.getOrderConsultURL() + "OrderQuery/member/getMemberOrders";
    }

    public List<UserOrder> getOrderListRecentMonth(String guid) {
        Map<String, Object> jsonField = new HashMap<>(4);
        Map<String, Object> params = new HashMap<>(1);

        jsonField.put("memberId", guid);
        jsonField.put("orderType", 0);
        jsonField.put("currPage", 1);
        jsonField.put("pageCount", 100);

        params.put("request", JsonUtil.toJsonString(jsonField));

        try {
            String response = HttpUtils.excutePost(getAllOrderListURL, params);
            JsonNode resultNode = JsonUtil.toNode(response);

            if (resultNode != null && resultNode.has("code") && "100".equals(JsonNodeUtil.getString(resultNode, "code"))) {
                List<UserOrder> userOrderList = new ArrayList<>(1);
                Iterator<JsonNode> pageVoList = resultNode.get("result").get("pageVoList").iterator();

                while (pageVoList.hasNext()) {
                    JsonNode orderNode = pageVoList.next();
                    UserOrder userOrder = new UserOrder();
                    userOrderList.add(userOrder);

                    userOrder.setSeq(JsonNodeUtil.getString(orderNode, "ogNo"));
                    userOrder.setTradeTime(JsonNodeUtil.getString(orderNode, "orderDate"));
                    userOrder.setPrice(JsonNodeUtil.getDouble(orderNode, "totalPrice", 0.0));

                    Iterator<JsonNode> packageNodes = orderNode.get("packages").iterator();
                    //获取第一个包裹
                    if (packageNodes.hasNext()) {
                        JsonNode firstPackage = packageNodes.next();
                        Iterator<JsonNode> itemNodeList = firstPackage.get("itemList").iterator();

                        int status = JsonNodeUtil.getInteger(firstPackage, "status");
                        int supplierType = JsonNodeUtil.getInteger(firstPackage, "supplierType");

                        //如果是自营包裹
                        if (supplierType == 1) {
                            userOrder.setStatus(FeiniuPackageStatus.getPackageStatus(status).getStatusDesc());
                        } else if (supplierType == 2) {
                            userOrder.setStatus(MerchantPackageStatus.getPackageStatus(status).getStatusDesc());
                        }

                        //获取包裹里面第一个商品信息
                        if (itemNodeList.hasNext()) {
                            JsonNode firstItemNode = itemNodeList.next();
                            String picUrl = JsonNodeUtil.getString(firstItemNode, "picUrl");

                            if (supplierType == 1) {
                                picUrl = businessConfig.getFeiniuSelfSupportImgBaseUrl() + picUrl;
                            } else {
                                picUrl = picUrl.replaceFirst("^http://wh-image01\\.fn\\.com(:80)?/", businessConfig.getRandomUrlBase());
                                picUrl = picUrl.replaceFirst("^http://10\\.211\\.64\\.68(:80)?/", businessConfig.getRandomUrlBase());
                                picUrl = picUrl.indexOf("://") > 0 ? picUrl : businessConfig.getRandomUrlBase() + picUrl;
                            }

                            picUrl = picUrl.replaceFirst("\\.[a-zA-Z]+$", businessConfig.getOrderPicSize() + "$0");
                            picUrl = picUrl.replaceFirst("\\.[\\w_]+\\.([a-zA-Z]+)$", businessConfig.getOrderPicSize() + "$1");

                            userOrder.setPicURL(picUrl);
                            userOrder.setTitle(JsonNodeUtil.getString(firstItemNode, "name"));
                        }
                    }
                }
                return userOrderList;
            } else {
                throw new BusinessException("订单返回数据出错：" + response);
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    //判断订单是否从属于某个用户
    public boolean isOrderSeqUnderUser(String guid, String orderSeq) {
        boolean isLegal = false;

        Map<String, Object> jsonField = new HashMap<>(2);
        Map<String, Object> params = new HashMap<>(1);

        jsonField.put("memGuid", guid);
        jsonField.put("ogNo", orderSeq);

        params.put("data", JsonUtil.toJsonString(jsonField));

        try {
            String response = HttpUtils.excutePost(checkOrderIsUnderUserURL, params);
            JsonNode resultNode = JsonUtil.toNode(response);

            if (resultNode != null && resultNode.has("code")) {
                int code = JsonNodeUtil.getInteger(resultNode, "code");

                if (200 == code) {
                    isLegal = true;
                }
            } else {
                throw new BusinessException("订单返回数据出错：" + response);
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        }

        return isLegal;
    }

    //飞牛自营包裹状态
    private enum FeiniuPackageStatus {
        WAIT_FOR_PAY(1, "待付款"),
        WAIT_FOR_SENDING(2, "待发货"),
        WAIT_FOR_RECEIVING(3, "待收货"),
        SUCCEED(4, "交易成功"),
        CANCELLED(5, "已取消"),
        TRADE_CLOSE(6, "交易关闭"),
        HAVING_RETURNED(7, "已退货"),
        DEFAULT(-1, ""),;

        private int status;
        private String statusDesc;
        private static final Map<Integer, FeiniuPackageStatus> MAP = new HashMap<>(FeiniuPackageStatus.values().length);

        static {
            for (FeiniuPackageStatus feiniuPackageStatus : FeiniuPackageStatus.values()) {
                MAP.put(feiniuPackageStatus.status, feiniuPackageStatus);
            }
        }

        FeiniuPackageStatus(int status, String statusDesc) {
            this.status = status;
            this.statusDesc = statusDesc;
        }

        public String getStatusDesc() {
            return statusDesc;
        }

        public static FeiniuPackageStatus getPackageStatus(int status) {
            FeiniuPackageStatus feiniuPackageStatus = MAP.get(status);

            if (feiniuPackageStatus == null) {
                return DEFAULT;
            } else {
                return feiniuPackageStatus;
            }
        }
    }

    //飞牛商城包裹状态
    private enum MerchantPackageStatus {
        /*跨境 10:申请报关，11:报关通过，12:申请通关，13:通关通过  都归到 待发货 */
        WAIT_FOR_PAY(1, "待付款"),
        WAIT_FOR_SENDING(2, "待发货"),
        WAIT_FOR_RECEIVING(3, "待收货"),
        SUCCEED(4, "交易成功"),
        CANCELLED(5, "已取消"),
        TRADE_CLOSE(6, "交易关闭"),
        IN_RETURNING(7, "退货中"),
        IN_RETURNING_2(8, "退货中"),
        CLOSED(9, "交易关闭"),
        GLOBAL_DECLARATION_REQUEST(10, "待发货"),
        GLOBAL_DECLARATION_PASSED(11, "待发货"),
        GLOBAL_CLEARANCE_REQUEST(12, "待发货"),
        GLOBAL_CLEARANCE_PASSED(13, "待发货"),
        DEFAULT(-1, ""),;

        private int status;
        private String statusDesc;
        private static final Map<Integer, MerchantPackageStatus> MAP = new HashMap<>(MerchantPackageStatus.values().length);

        static {
            for (MerchantPackageStatus feiniuPackageStatus : MerchantPackageStatus.values()) {
                MAP.put(feiniuPackageStatus.status, feiniuPackageStatus);
            }
        }

        MerchantPackageStatus(int status, String statusDesc) {
            this.status = status;
            this.statusDesc = statusDesc;
        }

        public String getStatusDesc() {
            return statusDesc;
        }

        public static MerchantPackageStatus getPackageStatus(int status) {
            MerchantPackageStatus merchantPackageStatus = MAP.get(status);

            if (merchantPackageStatus == null) {
                return DEFAULT;
            } else {
                return merchantPackageStatus;
            }
        }
    }
}
