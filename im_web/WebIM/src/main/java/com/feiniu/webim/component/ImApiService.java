package com.feiniu.webim.component;

import com.fasterxml.jackson.databind.JsonNode;
import com.feiniu.webim.bean.domain.*;
import com.feiniu.webim.bean.request.GetHistoryMessageListRequest;
import com.feiniu.webim.bean.request.GetRecentMerchantInfoRequest;
import com.feiniu.webim.bean.web.request.GetSubMerchantWebRequest;
import com.feiniu.webim.config.ConfigManager;
import com.feiniu.webim.constants.Constants;
import com.feiniu.webim.exception.ClientIllegalArgumentException;
import com.feiniu.webim.exception.HttpGetException;
import com.feiniu.webim.exception.ImserverException;
import com.feiniu.webim.exception.WrongRequestParamException;
import com.feiniu.webim.util.general.IPUtil;
import com.feiniu.webim.util.json.JSONUtil;
import com.feiniu.webim.util.json.JsonNodeUtil;
import com.feiniu.webim.util.web.HttpClientUtils;
import com.feiniu.webim.util.web.WebUtil;
import org.apache.commons.collections4.CollectionUtils;
import org.apache.commons.lang.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.*;

/**
 * Author: morningking
 * Date: 2016/2/3
 * Contact: 243717042@qq.com
 */
public class ImApiService {
    private ImApiService() {
    }

    public static final ImApiService INSTANCE = new ImApiService();
    private static final String IM_API_SERVICE_URL = ConfigManager.getProperty("service_im_api");
    private static final String GET_MERCHANT_ONLINE_SERVICE_URL = IM_API_SERVICE_URL + "userInfo/get-merchant-online-status.do";
    private static final String GET_SUB_MERCHANT_ONLINE_SERVICE_URL = IM_API_SERVICE_URL + "userInfo/get-subMerchant-online-status.do";
    private static final String GET_HISTORY_MESSAGE_LIST_SERVICE_URL = IM_API_SERVICE_URL + "userInfo/get-history-message-by-page.do";
    private static final String GET_GET_RECENT_MERCHANT_INFO_URL = IM_API_SERVICE_URL + "userInfo/get-recent-merchant-info";
    private static final String GET_SUBACCOUNT_URL = IM_API_SERVICE_URL + "event/queue-request-event-sync";
    private static final String GET_RECENT_SUBACCOUNT_LIST_URL = IM_API_SERVICE_URL + "userInfo/get-recent-subMerchant-info";
    private static final String NOTIFY_WINDOW_CLOSE_FROM_BUYER = IM_API_SERVICE_URL + "event/im-push-event-sync.do";
    private static final Long localIP = IPUtil.ipToLong(WebUtil.getLocalIP());

    private static final Logger LOGGER = LoggerFactory.getLogger(ImApiService.class);

    /**
     * 获取商家在线状态
     *
     * @param merchantIDList 商家id列表
     * @return
     */
    public List<MerchantOnlineInfo> getMerchantOnlineInfo(List<Integer> merchantIDList) {
        if (CollectionUtils.isEmpty(merchantIDList)) {
            throw new WrongRequestParamException("参数为空");
        }

        Map<String, Object> params = new HashMap<>(1);
        params.put("merchantIDs", StringUtils.join(merchantIDList, ","));

        try {
            String response = HttpClientUtils.executeGet(GET_MERCHANT_ONLINE_SERVICE_URL, "", params);
            JsonNode jsonNode = JSONUtil.toNode(response);

            if (jsonNode != null && jsonNode.has("success") && jsonNode.get("success").asBoolean()) {
                JsonNode dataNode = jsonNode.get("data");

                if (dataNode != null) {
                    JsonNode statusArrayNode = dataNode.get("statuses");
                    if (statusArrayNode != null && statusArrayNode.isArray()) {
                        Iterator<JsonNode> statusIterator = statusArrayNode.iterator();
                        List<MerchantOnlineInfo> merchantOnlineInfoList = new ArrayList<>(statusArrayNode.size());

                        while (statusIterator.hasNext()) {
                            JsonNode statusNode = statusIterator.next();
                            MerchantOnlineInfo merchantOnlineInfo = new MerchantOnlineInfo();
                            merchantOnlineInfo.setId(JsonNodeUtil.getInteger(statusNode, "merchantID"));
                            merchantOnlineInfo.setOnline(JsonNodeUtil.getBoolean(statusNode, "status"));

                            if (merchantOnlineInfo != null) {
                                merchantOnlineInfoList.add(merchantOnlineInfo);
                            }
                        }

                        return merchantOnlineInfoList;
                    }
                }
            }

            throw new RuntimeException("获取商家在线状态出错，服务器返回错误数据：" + response);
        } catch (HttpGetException e) {
            throw new ImserverException(e);
        }
    }

    /**
     * 获取子账号在线状态
     *
     * @param subMerchantIDList 子账号列表
     * @return
     */
    public List<MerchantOnlineInfo> getSubMerchantOnlineInfo(List<Long> subMerchantIDList) {
        if (CollectionUtils.isEmpty(subMerchantIDList)) {
            throw new WrongRequestParamException("参数为空");
        }

        Map<String, Object> params = new HashMap<>(1);
        params.put("subMerchantIDs", StringUtils.join(subMerchantIDList, ","));

        try {
            String response = HttpClientUtils.executeGet(GET_SUB_MERCHANT_ONLINE_SERVICE_URL, "", params);
            JsonNode jsonNode = JSONUtil.toNode(response);

            if (jsonNode != null && jsonNode.has("success") && jsonNode.get("success").asBoolean()) {
                JsonNode dataNode = jsonNode.get("data");

                if (dataNode != null) {
                    JsonNode statusArrayNode = dataNode.get("statuses");
                    if (statusArrayNode != null && statusArrayNode.isArray()) {
                        Iterator<JsonNode> statusIterator = statusArrayNode.iterator();
                        List<MerchantOnlineInfo> merchantOnlineInfoList = new ArrayList<>(statusArrayNode.size());

                        while (statusIterator.hasNext()) {
                            JsonNode statusNode = statusIterator.next();
                            MerchantOnlineInfo merchantOnlineInfo = new MerchantOnlineInfo();
                            merchantOnlineInfo.setId(JsonNodeUtil.getInteger(statusNode, "subMerchantID"));
                            merchantOnlineInfo.setOnline(JsonNodeUtil.getBoolean(statusNode, "status"));

                            if (merchantOnlineInfo != null) {
                                merchantOnlineInfoList.add(merchantOnlineInfo);
                            }
                        }

                        return merchantOnlineInfoList;
                    }
                }
            }

            throw new RuntimeException("获取商家子账号在线状态出错，服务器返回错误数据：" + response);
        } catch (HttpGetException e) {
            throw new ImserverException(e);
        }
    }

    /**
     * 获取历史消息
     *
     * @param request
     * @return
     */
    public List<HistoryMessage> getHistoryMessageList(GetHistoryMessageListRequest request) {
        try {
            String response = HttpClientUtils.executeGet(GET_HISTORY_MESSAGE_LIST_SERVICE_URL, "", JSONUtil.toMap(request));
            JsonNode jsonNode = JSONUtil.toNode(response);

            if (jsonNode != null && jsonNode.has("success") && jsonNode.get("success").asBoolean()) {
                JsonNode dataNode = jsonNode.get("data");

                if (dataNode != null) {
                    return JSONUtil.toList(JsonNodeUtil.getString(dataNode, "histroyMessage"), HistoryMessage.class);
                }
            }

            throw new RuntimeException("获取历史消息记录列表出错，服务器返回错误数据：" + response);
        } catch (HttpGetException e) {
            throw new ImserverException(e);
        }
    }

    /**
     * 获取最近联系店铺
     *
     * @param request
     * @return
     */
    public List<RecentlyMobileSeller> getRecentMerchantInfo(GetRecentMerchantInfoRequest request) {
        try {
            String response = HttpClientUtils.executeGet(GET_GET_RECENT_MERCHANT_INFO_URL, "", JSONUtil.toMap(request));
            JsonNode jsonNode = JSONUtil.toNode(response);

            if (jsonNode != null && jsonNode.has("success") && jsonNode.get("success").asBoolean()) {
                JsonNode dataNode = jsonNode.get("data");

                if (dataNode != null) {
                    return JSONUtil.toList(JsonNodeUtil.getString(dataNode, "recentlyMobileSellerList"), RecentlyMobileSeller.class);
                }
            }

            throw new RuntimeException("获取获取最近联系店铺失败，服务器返回错误数据：" + response);
        } catch (HttpGetException e) {
            throw new ImserverException(e);
        }
    }

    /**
     * 获取最近联系子账号集合
     *
     * @param request
     * @return
     */
    public List<RecentlySeller> getRecentSubAccountList(GetRecentMerchantInfoRequest request) {
        try {
            String response = HttpClientUtils.executeGet(GET_RECENT_SUBACCOUNT_LIST_URL, "", JSONUtil.toMap(request));
            JsonNode jsonNode = JSONUtil.toNode(response);

            if (jsonNode != null && jsonNode.has("success") && jsonNode.get("success").asBoolean()) {
                JsonNode dataNode = jsonNode.get("data");

                if (dataNode != null) {
                    return JSONUtil.toList(JsonNodeUtil.getString(dataNode, "recentChatInfoList"), RecentlySeller.class);
                }
            }

            throw new RuntimeException("获取最近联系商家子账号失败，服务器返回错误数据：" + response);
        } catch (HttpGetException e) {
            throw new ImserverException(e);
        }
    }

    /**
     * 商城、自营 分流接口
     *
     * @param getSubMerchantWebRequest
     * @return
     */
    public MerchantSubAccountInfo getSubAccount(GetSubMerchantWebRequest getSubMerchantWebRequest) {
        Map<String, Object> params = new HashMap<>(8);
        params.put("merchantId", getSubMerchantWebRequest.getPid());
        params.put("type", getSubMerchantWebRequest.getDiverse_type());
        params.put("customerId", getSubMerchantWebRequest.getSid());
        params.put("groupId", getSubMerchantWebRequest.getGroup_id());
        params.put("fromDevice", getSubMerchantWebRequest.getFromDeviceType());
        params.put("fromPage", getSubMerchantWebRequest.getFromPageType());

        String userLevel = getSubMerchantWebRequest.getUserLevel();
        if (StringUtils.isEmpty(userLevel)) {
            userLevel = "0";
        } else if (userLevel.equalsIgnoreCase("tp")) {
            userLevel = "3";
        }
        userLevel = userLevel.replaceAll("\\D", "");

        params.put("userLevel", StringUtils.isEmpty(userLevel) ? "0" : userLevel);
        params.put("requestTime", System.currentTimeMillis() / Constants.MSG_TIME_TRANSFORM);

        try {
            String response = HttpClientUtils.executeGet(GET_SUBACCOUNT_URL, "", params);
            JsonNode jsonNode = JSONUtil.toNode(response);

            if (jsonNode != null && jsonNode.has("success")) {
                if (jsonNode.get("success").asBoolean()) {
                    JsonNode dataNode = jsonNode.get("data");

                    if (dataNode != null) {
                        return JSONUtil.fromJson(JsonNodeUtil.getString(dataNode, "queueResponse"), MerchantSubAccountInfo.class);
                    }
                } else {
                    int errorCode = JsonNodeUtil.getInteger(jsonNode, "errorCode", 0);

                    if (400 == errorCode) {
                        throw new ClientIllegalArgumentException(JsonNodeUtil.getString(jsonNode, "message", "服务器发生故障"));
                    }
                }
            }

            throw new RuntimeException("分流失败，服务器返回错误数据：" + response);
        } catch (HttpGetException e) {
            throw new RuntimeException("发起网络请求失败", e);
        }
    }

    /**
     * 买家关闭窗口，通知imapi更新 排队/分流 信息
     *
     * @param userID    用户id
     * @param groupID   自营客服组id
     * @param serviceID 如果当前正在排队，传0，没排队，传客服id
     */
    public void notifyWindowCloseFromBuyer(int userID, int groupID, int serviceID) {
        Map<String, Object> params = new HashMap<>(3);
        params.put("eventType", 1);
        params.put("customerId", userID);
        params.put("groupId", groupID);
        params.put("serviceId", serviceID);
        params.put("loginIP", localIP);

        try {
            String paramStr = JSONUtil.toJsonString(params);
            String response = HttpClientUtils.executeGet(NOTIFY_WINDOW_CLOSE_FROM_BUYER, "", params);
            JsonNode jsonNode = JSONUtil.toNode(response);

            if (jsonNode != null && jsonNode.has("success")) {
                if (jsonNode.get("success").asBoolean()) {
                    LOGGER.info("通知im-api用户退出成功：" + paramStr);
                } else {
                    LOGGER.error("通知im-api用户退出失败：" + paramStr);
                }
            }

        } catch (HttpGetException e) {
            LOGGER.error("发起网络请求失败", e);
        }
    }
}
