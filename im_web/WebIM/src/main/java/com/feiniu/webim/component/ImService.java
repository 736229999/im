package com.feiniu.webim.component;

import com.fasterxml.jackson.databind.JsonNode;
import com.feiniu.webim.bean.domain.FeiniuStaffInfo;
import com.feiniu.webim.bean.domain.MerchantOnlineInfo;
import com.feiniu.webim.bean.web.request.ImWebRequest;
import com.feiniu.webim.config.ConfigManager;
import com.feiniu.webim.exception.ImserverException;
import com.feiniu.webim.exception.WrongRequestParamException;
import com.feiniu.webim.util.json.JSONUtil;
import com.feiniu.webim.util.json.JsonNodeUtil;
import com.feiniu.webim.util.web.HttpClientUtils;
import org.apache.commons.collections4.CollectionUtils;

import java.util.*;

/**
 * Author: wangchen
 * Date: 2015/7/27
 * Contact: 243717042@qq.com
 */
public class ImService {
    private ImService() {
    }

    public static final ImService IM_SERVICE = new ImService();
    private static final String IM_WEB_SERVICE_URL = ConfigManager.getProperty("service_im_sever");
    private static final String VERSION = "1.0";

    private static boolean checkResponseValidity(JsonNode jsonNode) {
        return jsonNode != null && jsonNode.has("result") && JsonNodeUtil.getBoolean(jsonNode, "result");
    }

    @SuppressWarnings("unchecked")
    public List<MerchantOnlineInfo> getMerchantOnlineInfo(List<Integer> merchantIDList) {
        if (CollectionUtils.isEmpty(merchantIDList)) {
            throw new WrongRequestParamException("参数为空");
        }

        Map<String, Object> params = new HashMap<>(1);
        params.put("ids", merchantIDList);

        ImWebRequest imWebRequest = new ImWebRequest();
        imWebRequest.setVersion(VERSION);
        imWebRequest.setMethod("getMerchantStatus");
        imWebRequest.setParams(JSONUtil.toJsonString(params));

        try {
            String response = HttpClientUtils.executeGet(IM_WEB_SERVICE_URL, "", JSONUtil.toMap(imWebRequest));
            JsonNode jsonNode = JSONUtil.toNode(response);

            if (checkResponseValidity(jsonNode) && jsonNode.has("statuses")) {
                jsonNode = jsonNode.get("statuses");

                Iterator<JsonNode> jsonNodeIterator = jsonNode.iterator();
                List<MerchantOnlineInfo> merchantOnlineInfoList = new ArrayList<>();

                while (jsonNodeIterator.hasNext()) {
                    JsonNode node = jsonNodeIterator.next();

                    MerchantOnlineInfo merchantOnlineInfo = new MerchantOnlineInfo();
                    merchantOnlineInfo.setId(JsonNodeUtil.getLong(node, "id"));
                    merchantOnlineInfo.setOnline(JsonNodeUtil.getBoolean(node, "status"));

                    merchantOnlineInfoList.add(merchantOnlineInfo);
                }

                return merchantOnlineInfoList;
            } else {
                throw new ImserverException("获取商家在线状态发生异常: " + response);
            }
        } catch (Exception e) {
            throw new ImserverException(e);
        }
    }

    @SuppressWarnings("unchecked")
    public List<MerchantOnlineInfo> getSubMerchantOnlineInfo(List<Long> subMerchantIDList) {
        if (CollectionUtils.isEmpty(subMerchantIDList)) {
            throw new WrongRequestParamException("参数为空");
        }

        Map<String, Object> params = new HashMap<>(1);
        params.put("ids", subMerchantIDList);

        ImWebRequest imWebRequest = new ImWebRequest();
        imWebRequest.setVersion(VERSION);
        imWebRequest.setMethod("getSubMerchantStatus");
        imWebRequest.setParams(JSONUtil.toJsonString(params));

        try {
            String response = HttpClientUtils.executeGet(IM_WEB_SERVICE_URL, "", JSONUtil.toMap(imWebRequest));
            JsonNode jsonNode = JSONUtil.toNode(response);

            if (checkResponseValidity(jsonNode) && jsonNode.has("statuses")) {
                jsonNode = jsonNode.get("statuses");

                Iterator<JsonNode> jsonNodeIterator = jsonNode.iterator();
                List<MerchantOnlineInfo> merchantOnlineInfoList = new ArrayList<>();

                while (jsonNodeIterator.hasNext()) {
                    JsonNode node = jsonNodeIterator.next();

                    MerchantOnlineInfo merchantOnlineInfo = new MerchantOnlineInfo();
                    merchantOnlineInfo.setId(JsonNodeUtil.getLong(node, "id"));
                    merchantOnlineInfo.setOnline(JsonNodeUtil.getBoolean(node, "status"));

                    merchantOnlineInfoList.add(merchantOnlineInfo);
                }

                return merchantOnlineInfoList;
            } else {
                throw new ImserverException("获取商家子账号在线状态发生异常: " + response);
            }
        } catch (Exception e) {
            throw new ImserverException(e);
        }
    }

    @SuppressWarnings("unchecked")
    public FeiniuStaffInfo getInnerFeiniuStaffInfo(long staffID) {
        if (staffID == 0 || Long.toString(staffID) == null) {
            throw new WrongRequestParamException("参数为空");
        }

        Map<String, Object> params = new HashMap<>(1);
        params.put("workId", staffID);

        ImWebRequest imWebRequest = new ImWebRequest();
        imWebRequest.setVersion(VERSION);
        imWebRequest.setMethod("getWorkInfo");
        imWebRequest.setParams(JSONUtil.toJsonString(params));

        try {
            String response = HttpClientUtils.executeGet(IM_WEB_SERVICE_URL, "", JSONUtil.toMap(imWebRequest));
            JsonNode jsonNode = JSONUtil.toNode(response);
            FeiniuStaffInfo feiniuStaffInfo = new FeiniuStaffInfo();
            if (checkResponseValidity(jsonNode) &&jsonNode.hasNonNull("workId")) {

                feiniuStaffInfo.setWorkId(JsonNodeUtil.getLong(jsonNode,"workId"));
                feiniuStaffInfo.setGroupName(JsonNodeUtil.getString(jsonNode, "groupname"));
                feiniuStaffInfo.setNickName(JsonNodeUtil.getString(jsonNode, "nickname"));
                feiniuStaffInfo.setUserName(JsonNodeUtil.getString(jsonNode, "username"));

                return feiniuStaffInfo;
            } else {
                throw new ImserverException("获取飞牛工作人员信息发生异常: " + response);
            }
        } catch (Exception e) {
            throw new ImserverException(e);
        }
    }
}
