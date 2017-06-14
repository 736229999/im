package com.feiniu.imServer.service.external.kafka;

import com.feiniu.imServer.bean.UserInfo;
import com.feiniu.imServer.bean.config.DependentServicePaths;
import com.feiniu.imServer.util.json.JsonNodeUtil;
import com.feiniu.imServer.util.json.JsonUtil;
import com.feiniu.imServer.util.web.HttpClientUtils;
import org.apache.commons.lang.StringUtils;
import org.codehaus.jackson.JsonNode;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import javax.annotation.PostConstruct;
import java.io.IOException;
import java.util.HashMap;

/**
 * Author: morningking
 * Date: 2016/3/1
 * Contact: 243717042@qq.com
 */
@Service
public class UserService {
    @Autowired
    private DependentServicePaths dependentServicePaths;

    //获取用户信息接口地址
    private String GET_MEMBER_INFO_URL;
    //获取用户信息接口地址 成功状态码
    private static final int GET_USER_INFO_SUCCESS_CODE = 100;

    @PostConstruct
    private void init() {
        GET_MEMBER_INFO_URL = dependentServicePaths.getQueryUserInfoServicePath() + "member_api/searchMember";
    }

    public UserInfo getUserInfo(String guid) {
        String response;
        HashMap<String, Object> paramMap = new HashMap<>(3);
        paramMap.put("type", 5);
        paramMap.put("returnType", 1);
        paramMap.put("name", guid);

        //会员基本信息
        try {
            UserInfo userInfo = new UserInfo();
            response = HttpClientUtils.excutePost(GET_MEMBER_INFO_URL, paramMap);
            JsonNode jsonNode = JsonUtil.toNode(response);
            JsonNode dataNode;

            if (jsonNode.has("code") && GET_USER_INFO_SUCCESS_CODE == JsonNodeUtil.getInteger(jsonNode, "code")) {
                dataNode = jsonNode.get("data");

                userInfo.setUsername(JsonNodeUtil.getString(dataNode, "MEM_USERNAME"));
                userInfo.setNickname(JsonNodeUtil.getString(dataNode, "NICKNAME"));
                userInfo.setGuid(JsonNodeUtil.getString(dataNode, "MEM_GUID"));

                if (StringUtils.isEmpty(userInfo.getNickname())) {
                    userInfo.setNickname(userInfo.getUsername());
                }
            }

            return userInfo;
        } catch (IOException e) {
            throw new RuntimeException("从用户中心获取个人信息发生错误:", e);
        }
    }
}
