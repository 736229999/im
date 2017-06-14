package com.feiniu.webim.component;

import com.fasterxml.jackson.databind.JsonNode;
import com.feiniu.webim.bean.domain.UserCheckInInfo;
import com.feiniu.webim.bean.web.request.AppMemeberRequest;
import com.feiniu.webim.config.ConfigManager;
import com.feiniu.webim.exception.LoginCheckException;
import com.feiniu.webim.util.encrypt.MD5;
import com.feiniu.webim.util.json.JSONUtil;
import com.feiniu.webim.util.json.JsonNodeUtil;
import com.feiniu.webim.util.web.HttpClientUtils;
import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;

import java.io.IOException;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

/**
 * Author: wangchen
 * Date: 2015/6/30
 * Contact: 243717042@qq.com
 */
public class AppMemberService {
    public static final AppMemberService instance = new AppMemberService();

    private AppMemberService() {
    }

    private static final String MD5_ENCRYPTED_KEY;
    private static final String SERVICE_RETRIEVE_APP_MEMBER_API_URL;
    private static final String APP_MEMBER_SITE_KEY;
    private static String SERVICE_APP_MEMBER_SEARCH_URL;
    private static final Logger LOGGER = Logger.getLogger(AppMemberService.class);
    private static final int SUCCESS_CODE = 0;

    static {
        APP_MEMBER_SITE_KEY = ConfigManager.getProperty("service_app_member_site_key");
        SERVICE_RETRIEVE_APP_MEMBER_API_URL = ConfigManager.getProperty("service_app_member");
        MD5_ENCRYPTED_KEY = ConfigManager.getProperty("service_app_md5_key");
    }

    public UserCheckInInfo getUserInfo(String token) {
        if (StringUtils.isEmpty(SERVICE_APP_MEMBER_SEARCH_URL)) {
            retrieveAppMemberApiUrl();
        }

        Map<String, Object> paramsMap = new HashMap<>(1);
        AppMemeberRequest appMemeberRequest = new AppMemeberRequest();
        appMemeberRequest.setApiVersion("t4.8");
        appMemeberRequest.setCityCode("1");
        appMemeberRequest.setToken(token);
        paramsMap.put("data", JSONUtil.toJsonString(appMemeberRequest));

        try {
            //手机API入参添加MD5加密参数
            String paramsMD5 = JSONUtil.toJsonString(appMemeberRequest) + MD5_ENCRYPTED_KEY;
            String paramsMD5Encode = MD5.getMD5(paramsMD5);
            paramsMap.put("paramsMD5", paramsMD5Encode);

            String response = HttpClientUtils.executeHttpsPost(SERVICE_APP_MEMBER_SEARCH_URL, paramsMap);
            JsonNode jsonNode = JSONUtil.toNode(response);
            JsonNode bodyNode;

            if (jsonNode != null && jsonNode.hasNonNull("errorCode")
                    && JsonNodeUtil.getInteger(jsonNode, "errorCode") == SUCCESS_CODE
                    && jsonNode.has("body")
                    && (bodyNode = jsonNode.get("body")) != null
                    && bodyNode.has("isExist")
                    && 1 == JsonNodeUtil.getInteger(bodyNode, "isExist")) {

                UserCheckInInfo userCheckInInfo = new UserCheckInInfo();

                if (bodyNode.has("mem_guid")) {
                    userCheckInInfo.setGuid(JsonNodeUtil.getString(bodyNode, "mem_guid"));
                } else {
                    throw new LoginCheckException("无线用户-接口发生错误，没有返回GUID");
                }
                if (bodyNode.has("mem_username")) {
                    userCheckInInfo.setUserName(JsonNodeUtil.getString(bodyNode, "mem_username"));
                } else {
                    throw new LoginCheckException("无线用户-接口发生错误，没有返回用户昵称");
                }

                if (bodyNode.has("mem_cellphone")) {
                    userCheckInInfo.setTelephone(JsonNodeUtil.getString(bodyNode, "mem_cellphone"));
                }
                if (bodyNode.has("mem_email")) {
                    userCheckInInfo.setEmail(JsonNodeUtil.getString(bodyNode, "mem_email"));
                }

                return userCheckInInfo;
            } else {
                throw new LoginCheckException("飞牛商城用户中心-获取用户信息返回错误数据：" + response);
            }
        } catch (IOException e) {
            LOGGER.error("获取用户信息发生错误");
            throw new LoginCheckException(e);
        }
    }

    private static void retrieveAppMemberApiUrl() {
        Map<String, Object> paramsMap = Collections.emptyMap();

        try {
            String response = HttpClientUtils.executeHttpsPost(SERVICE_RETRIEVE_APP_MEMBER_API_URL, paramsMap);
            JsonNode jsonNode = JSONUtil.toNode(response);
            JsonNode bodyNode;

            if (jsonNode != null && jsonNode.hasNonNull("errorCode")
                    && JsonNodeUtil.getInteger(jsonNode, "errorCode") == 0
                    && jsonNode.has("body")
                    && (bodyNode = jsonNode.get("body")) != null
                    && bodyNode.has("wirelessAPI")) {
                JsonNode wirelessAPIList = bodyNode.get("wirelessAPI");

                SERVICE_APP_MEMBER_SEARCH_URL = JsonNodeUtil.getString(wirelessAPIList, APP_MEMBER_SITE_KEY);
            } else {
                throw new RuntimeException("获取app-member接口地址出错");
            }
        } catch (IOException e) {
            LOGGER.error("获取app-member接口地址出错", e);
        }
    }
}
