package com.feiniu.webim.component;

import com.fasterxml.jackson.databind.JsonNode;
import com.feiniu.webim.bean.domain.UserCheckInInfo;
import com.feiniu.webim.bean.domain.UserInfo;
import com.feiniu.webim.config.ConfigManager;
import com.feiniu.webim.exception.LoginCheckException;
import com.feiniu.webim.util.json.JSONUtil;
import com.feiniu.webim.util.json.JsonNodeUtil;
import com.feiniu.webim.util.web.HttpClientUtils;
import org.apache.commons.lang.ArrayUtils;
import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;

import javax.servlet.http.Cookie;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

/**
 * Created by wangchen on 2015/1/15.
 */
public class UserService {
    public static final UserService instance = new UserService();

    private UserService() {
    }

    private static String SERVICE_MEMBER_URL = ConfigManager.getProperty("service_member");
    private static final String SERVICE_MEMBER_INFO_URL = ConfigManager.getProperty("service_member_info");
    private static final String SERVICE_MEMBER_LEVEL_URL = ConfigManager.getProperty("service_member_level");
    private static final String MEMBER_IMAGE_BASE_URL = ConfigManager.getProperty("member_image_base_url");
    private static final String CHECK_IN_URL = SERVICE_MEMBER_URL + "v1/login/checkLogin";
    private static final String GET_MEMBER_INFO_URL = SERVICE_MEMBER_INFO_URL + "member_api/searchMember";
    private static final String GET_MEMBER_LEVEL_URL = SERVICE_MEMBER_LEVEL_URL + "FeiniuScore/growth/mem/queryMemLevel";
    private static final int GET_USER_INFO_SUCCESS_CODE = 100;
    public static final String PASSPORT_SESSION_COOKIE_NAME = "8061f35e87c80aee";

    private static final Logger LOGGER = Logger.getLogger(UserService.class);

    public UserCheckInInfo checkUserInfoFromMember(Cookie[] cookies) {
        long startTime = System.currentTimeMillis();
        Map<String, Object> paramsMap = new HashMap<>(1);
        paramsMap.put("cookie", getSessionCookieValue(cookies));

        try {
            String response = HttpClientUtils.excutePost(CHECK_IN_URL, paramsMap);

            JsonNode jsonNode = JSONUtil.toNode(response);
            jsonNode = JSONUtil.toNode(jsonNode.toString());

            if (jsonNode != null && jsonNode.hasNonNull("code") && JsonNodeUtil.getInteger(jsonNode, "code") == 200) {
                jsonNode = jsonNode.get("data");
                jsonNode = JSONUtil.toNode(jsonNode.asText());

                UserCheckInInfo userCheckInInfo = new UserCheckInInfo();
                userCheckInInfo.setGuid(JsonNodeUtil.getString(jsonNode, "MEM_GUID"));
                userCheckInInfo.setUserName(JsonNodeUtil.getString(jsonNode, "MEM_USERNAME"));

                if (!jsonNode.has("MEM_EMAIL") || jsonNode.get("MEM_EMAIL").isNull()) {
                    userCheckInInfo.setEmail("");
                } else {
                    userCheckInInfo.setEmail(JsonNodeUtil.getString(jsonNode, "MEM_EMAIL"));
                }

                if (!jsonNode.has("MEM_CELL_PHONE") || jsonNode.get("MEM_CELL_PHONE").isNull()) {
                    userCheckInInfo.setTelephone("");
                } else {
                    userCheckInInfo.setTelephone(JsonNodeUtil.getString(jsonNode, "MEM_CELL_PHONE").replaceAll("\\D", ""));
                }

                LOGGER.debug("Checked Passport Sustained " + (System.currentTimeMillis() - startTime) + "ms");
                return userCheckInInfo;
            } else {
                throw new LoginCheckException("请您登陆");
            }
        } catch (IOException e) {
            LOGGER.error("获取用户信息发生错误");
            throw new RuntimeException(e);
        }
    }

    public UserInfo getUserInfo(String guid) {
        UserInfo userInfo = new UserInfo();
        String response;
        HashMap<String, Object> paramMap = new HashMap<>(3);
        paramMap.put("type", 5);
        paramMap.put("returnType", 1);
        paramMap.put("name", guid);

        //会员基本信息
        try {
            response = HttpClientUtils.excutePost(GET_MEMBER_INFO_URL, paramMap);
            JsonNode jsonNode = JSONUtil.toNode(response);
            JsonNode dataNode;

            if (jsonNode.has("code") && GET_USER_INFO_SUCCESS_CODE == JsonNodeUtil.getInteger(jsonNode, "code")) {
                dataNode = jsonNode.get("data");

                userInfo.setUsername(JsonNodeUtil.getString(dataNode, "MEM_USERNAME"));
                userInfo.setNickname(JsonNodeUtil.getString(dataNode, "NICKNAME"));
                userInfo.setGuid(JsonNodeUtil.getString(dataNode, "MEM_GUID"));

                if (StringUtils.isEmpty(userInfo.getNickname())) {
                    userInfo.setNickname(userInfo.getUsername());
                }

                String headPotrait = JsonNodeUtil.getString(dataNode, "HEAD_PORTRAIT");
                if (StringUtils.isNotEmpty(headPotrait)) {
                    userInfo.setFace_url(MEMBER_IMAGE_BASE_URL + headPotrait);
                } else {
                    userInfo.setFace_url("");
                }
            }
        } catch (IOException e) {
            throw new RuntimeException("从用户中心获取个人信息发生错误:", e);
        }

        return userInfo;
    }

    public String getUserLevel(String guid) {
        //会员等级
        Map<String, Object> levelParamMap = new HashMap<String, Object>(1);
        String data = "{\"memGuid\":\"" + guid + "\"}";
        levelParamMap.put("data", data);
        try {
            String response = HttpClientUtils.excutePost(GET_MEMBER_LEVEL_URL, levelParamMap);
            JsonNode jsonNode = JSONUtil.toNode(response);
            if (jsonNode != null && GET_USER_INFO_SUCCESS_CODE == JsonNodeUtil.getInteger(jsonNode, "code") && jsonNode.has("data")) {
                return JsonNodeUtil.getString(jsonNode.get("data"), "memLevel");
            } else {
                LOGGER.error("查询用户等级失败,数据为空");
            }
        } catch (Exception e) {
            LOGGER.error("查询用户等级失败", e);
        }

        return "T0";
    }

    public static boolean checkSessionCookieLegality(Cookie[] cookies) {
        if (ArrayUtils.isEmpty(cookies)) {
            return false;
        }

        return getSessionCookieValue(cookies) != null;
    }

    private static String getSessionCookieValue(Cookie[] cookies) {
        String cookieValue = null;

        for (Cookie cookie : cookies) {
            if (PASSPORT_SESSION_COOKIE_NAME.equals(cookie.getName())) {
                cookieValue = cookie.getValue();
                break;
            }
        }

        return cookieValue;
    }

}
