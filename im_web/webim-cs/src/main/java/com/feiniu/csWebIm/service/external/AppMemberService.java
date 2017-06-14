package com.feiniu.csWebIm.service.external;

import com.feiniu.csWebIm.bean.request.AppMemeberRequest;
import com.feiniu.csWebIm.bean.response.UserCheckInInfo;
import com.feiniu.csWebIm.consts.BusinessConfig;
import com.feiniu.csWebIm.exception.NonLoginFromMemberException;
import com.feiniu.csWebIm.exception.RemoteServiceWrongDataException;
import com.feiniu.csWebIm.utils.encode.MD5;
import com.feiniu.csWebIm.utils.json.JsonNodeUtil;
import com.feiniu.csWebIm.utils.json.JsonUtil;
import com.feiniu.csWebIm.utils.web.HttpClientUtils;
import org.apache.commons.lang.StringUtils;
import org.codehaus.jackson.JsonNode;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.io.IOException;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

/**
 * Author: wangchen
 * Date: 2015/6/30
 * Contact: 243717042@qq.com
 */
@Service
public class AppMemberService {
    @Autowired
    private BusinessConfig businessConfig;

    private static String SERVICE_APP_MEMBER_SEARCH_URL;
    private static final Logger LOGGER = LoggerFactory.getLogger(AppMemberService.class);
    private static final int SUCCESS_CODE = 0;

    public UserCheckInInfo getUserInfo(String token) {
        if (StringUtils.isEmpty(SERVICE_APP_MEMBER_SEARCH_URL)) {
            retrieveAppMemberApiUrl();
        }

        Map<String, Object> paramsMap = new HashMap<>(1);
        AppMemeberRequest appMemeberRequest = new AppMemeberRequest();
        appMemeberRequest.setApiVersion("t4.8");
        appMemeberRequest.setCityCode("1");
        appMemeberRequest.setToken(token);
        paramsMap.put("data", JsonUtil.toJsonString(appMemeberRequest));

        try {
            //手机API入参添加MD5加密参数
            String paramsMD5 = JsonUtil.toJsonString(appMemeberRequest) + businessConfig.getAppMemberMd5EncryptedKey();
            String paramsMD5Encode = MD5.getMD5(paramsMD5);
            paramsMap.put("paramsMD5", paramsMD5Encode);

            String response = HttpClientUtils.executeHttpsPost(SERVICE_APP_MEMBER_SEARCH_URL, paramsMap);
            JsonNode jsonNode = JsonUtil.toNode(response);
            JsonNode bodyNode;

            if (jsonNode != null && jsonNode.has("errorCode")
                    && JsonNodeUtil.getInteger(jsonNode, "errorCode") == SUCCESS_CODE
                    && jsonNode.has("body")
                    && (bodyNode = jsonNode.get("body")) != null
                    && bodyNode.has("isExist")
                    && 1 == JsonNodeUtil.getInteger(bodyNode, "isExist")) {

                UserCheckInInfo userCheckInInfo = new UserCheckInInfo();

                if (bodyNode.has("mem_guid")) {
                    userCheckInInfo.setGuid(JsonNodeUtil.getString(bodyNode, "mem_guid"));
                } else {
                    throw new RemoteServiceWrongDataException("无线用户-接口发生错误，没有返回GUID");
                }
                if (bodyNode.has("mem_username")) {
                    userCheckInInfo.setUserName(JsonNodeUtil.getString(bodyNode, "mem_username"));
                } else {
                    throw new RemoteServiceWrongDataException("无线用户-接口发生错误，没有返回用户昵称");
                }

                if (bodyNode.has("mem_cellphone")) {
                    userCheckInInfo.setTelephone(JsonNodeUtil.getString(bodyNode, "mem_cellphone"));
                }
                if (bodyNode.has("mem_email")) {
                    userCheckInInfo.setEmail(JsonNodeUtil.getString(bodyNode, "mem_email"));
                }

                return userCheckInInfo;
            } else {
                throw new NonLoginFromMemberException("飞牛商城用户中心-获取用户信息返回错误数据：" + response);
            }
        } catch (IOException e) {
            LOGGER.error("获取用户信息发生错误");
            throw new RuntimeException(e);
        }
    }

    private void retrieveAppMemberApiUrl() {
        Map<String, Object> paramsMap = Collections.emptyMap();

        try {
            String response = HttpClientUtils.executeHttpsPost(businessConfig.getAppMemberRetriveServiceUrl(), paramsMap);
            JsonNode jsonNode = JsonUtil.toNode(response);
            JsonNode bodyNode;

            if (jsonNode != null && jsonNode.has("errorCode")
                    && JsonNodeUtil.getInteger(jsonNode, "errorCode") == 0
                    && jsonNode.has("body")
                    && (bodyNode = jsonNode.get("body")) != null
                    && bodyNode.has("wirelessAPI")) {
                JsonNode wirelessAPIList = bodyNode.get("wirelessAPI");

                SERVICE_APP_MEMBER_SEARCH_URL = JsonNodeUtil.getString(wirelessAPIList, businessConfig.getAppMemberSiteKey());
            } else {
                throw new RuntimeException("获取app-member接口地址出错");
            }
        } catch (IOException e) {
            LOGGER.error("获取app-member接口地址出错", e);
        }
    }
}
