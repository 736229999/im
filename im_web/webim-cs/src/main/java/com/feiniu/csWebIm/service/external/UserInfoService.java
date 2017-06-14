package com.feiniu.csWebIm.service.external;

import com.feiniu.csWebIm.bean.domain.UserInfo;
import com.feiniu.csWebIm.bean.domain.UserLevel;
import com.feiniu.csWebIm.bean.response.UserCheckInInfo;
import com.feiniu.csWebIm.consts.BusinessConfig;
import com.feiniu.csWebIm.consts.Constants;
import com.feiniu.csWebIm.consts.SystemConfig;
import com.feiniu.csWebIm.exception.BusinessException;
import com.feiniu.csWebIm.exception.NonLoginFromMemberException;
import com.feiniu.csWebIm.utils.json.JsonNodeUtil;
import com.feiniu.csWebIm.utils.json.JsonUtil;
import com.feiniu.csWebIm.utils.web.HttpClientUtils;
import com.feiniu.csWebIm.utils.web.HttpUtils;
import org.apache.commons.collections.CollectionUtils;
import org.apache.commons.lang.ArrayUtils;
import org.codehaus.jackson.JsonNode;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import javax.annotation.PostConstruct;
import javax.servlet.http.Cookie;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

/**
 * Created by wangzhen on 2015/10/23.
 */
@SuppressWarnings({"unused", "unchecked", "rawtypes"})
@Service
public class UserInfoService {
    @Autowired
    private BusinessConfig businessConfig;

    private static final String SUCCSS_CODE = "100";

    private static final Logger LOGGER = LoggerFactory.getLogger(UserInfoService.class);
    public static final String PASSPORT_SESSION_COOKIE_NAME = "8061f35e87c80aee";
    private String CHECK_IN_URL;

    @PostConstruct
    private void init() {
        CHECK_IN_URL = businessConfig.getServiceMemberURL() + "v1/login/checkLogin";
    }

    public UserInfo getUserInfoById(String userId) {
        String serviceUrl = businessConfig.getMemberInfoServiceURL();
        String method = SystemConfig.MEMBER_API_SEARCH;
        Map<String, Object> map = new HashMap<String, Object>();
        UserInfo userInfo;
        String response = Constants.NIL_RESPONSE;
        map.put("name", userId);
        map.put("type", 5);
        try {
            response = HttpUtils.excutePost(serviceUrl + method, map);
            JsonNode jsonNode = JsonUtil.toNode(response);
            if (jsonNode != null && SUCCSS_CODE.equals(JsonNodeUtil.getString(jsonNode, "code")) && jsonNode.has("data")) {
                userInfo = JsonNodeUtil.injectBeanWithoutCaseParse(JsonNodeUtil.getString(jsonNode, "data"), UserInfo.class, false);
            } else {
                LOGGER.error("查询用户信息失败,返回为空");
                return null;
            }
        } catch (Exception e) {
            LOGGER.error("查询用户信息失败", e);
            return null;
        }

        return userInfo;
    }

    public UserLevel getUserLevelInfoById(String userId) {
        String serviceUrl = businessConfig.getMemberLevelInfoServiceURL();
        String method = SystemConfig.MEMEER_LEVEL_SEARCH;
        Map<String, Object> map = new HashMap<String, Object>();
        UserLevel userLevel;
        String response = Constants.NIL_RESPONSE;
        String data = "{\"memGuid\":\"" + userId + "\"}";
        map.put("data", data);
        try {
            response = HttpUtils.excutePost(serviceUrl + method, map);
            JsonNode jsonNode = JsonUtil.toNode(response);
            if (jsonNode != null && SUCCSS_CODE.equals(JsonNodeUtil.getString(jsonNode, "code")) && jsonNode.has("data")) {
                userLevel = JsonNodeUtil.injectBean(JsonNodeUtil.getString(jsonNode, "data"), UserLevel.class, false);
            } else {
                LOGGER.error("查询用户等级失败,数据为空");
                return null;
            }
        } catch (Exception e) {
            LOGGER.error("查询用户等级失败", e);
            return null;
        }

        return userLevel;
    }

    public UserCheckInInfo checkUserInfoFromMember(Cookie[] cookies) {
        long startTime = System.currentTimeMillis();
        Map<String, Object> paramsMap = new HashMap<>(1);
        paramsMap.put("cookie", getSessionCookieValue(cookies));

        try {
            String response = HttpClientUtils.excutePost(CHECK_IN_URL, paramsMap);

            JsonNode jsonNode = JsonUtil.toNode(response);
            jsonNode = JsonUtil.toNode(jsonNode.toString());

            if (jsonNode != null && jsonNode.has("code")) {
                if (JsonNodeUtil.getInteger(jsonNode, "code") == 200) {
                    jsonNode = jsonNode.get("data");
                    jsonNode = JsonUtil.toNode(jsonNode.asText());

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

                    return userCheckInInfo;
                } else {
                    throw new NonLoginFromMemberException("请您先登录会员中心");
                }
            } else {
                throw new BusinessException("飞牛商城用户中心-获取用户信息返回错误数据：" + response);
            }
        } catch (IOException e) {
            LOGGER.error("获取用户信息发生错误");
            throw new RuntimeException(e);
        }
    }

    private static String getSessionCookieValue(Cookie[] cookies) {
        String cookieValue = null;

        if (ArrayUtils.isNotEmpty(cookies)) {
            for (Cookie cookie : cookies) {
                if (PASSPORT_SESSION_COOKIE_NAME.equals(cookie.getName())) {
                    cookieValue = cookie.getValue();
                    break;
                }
            }
        }

        return cookieValue;
    }
}
