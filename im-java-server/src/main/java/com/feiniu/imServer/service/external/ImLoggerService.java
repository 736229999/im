package com.feiniu.imServer.service.external;

import com.feiniu.imServer.constants.BackendSetting;
import com.feiniu.imServer.constants.Constants;
import com.feiniu.imServer.util.JsonUtil;
import com.feiniu.imServer.util.json.JsonNodeUtil;
import com.feiniu.imServer.util.web.HttpUtils;
import org.apache.commons.collections.map.HashedMap;
import org.codehaus.jackson.JsonNode;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.Map;

/**
 * Created by wangzhen on 2016/3/24.
 */
@Service
public class ImLoggerService {

    private static final Logger LOGGER = LoggerFactory.getLogger(ImLoggerService.class);

    @Autowired
    BackendSetting backendSetting;

    public boolean saveSeller(Long userId,String userName ,String trueName){
        String response = Constants.NIL_RESPONSE;
        String imLoggerHost = backendSetting.getImLoggerServiceURL().replace("http://", "");
        Map<String, Object> map = new HashedMap();
        map.put("userId",userId);
        map.put("userName",userName);
        map.put("password",111111);
        map.put("trueName",trueName);

        try {
            response = HttpUtils.executeGet(imLoggerHost, "/login/save-seller",
                    map);
            JsonNode jsonNode = JsonUtil.toNode(response);
            if (jsonNode != null && JsonNodeUtil.getBoolean(jsonNode, "success")) {
                return true;
            } else {
                LOGGER.error("客服-查找客服接口相关信息失败 :" );
                return false;
            }
        } catch (Exception e) {
            LOGGER.error("客服-查找客服接口相关信息失败  ", e);
            return false;
        }
    }
}
