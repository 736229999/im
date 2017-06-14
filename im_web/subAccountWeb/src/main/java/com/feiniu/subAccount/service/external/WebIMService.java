package com.feiniu.subAccount.service.external;

import com.feiniu.subAccount.constants.BusinessConfig;
import com.feiniu.subAccount.util.JsonUtil;
import com.feiniu.subAccount.util.web.HttpUtils;
import org.apache.commons.lang.StringUtils;
import org.codehaus.jackson.JsonNode;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.*;

/**
 * Created by wangchen on 2015/2/11.
 */
@Service
public class WebIMService {
    @Autowired
    private BusinessConfig businessConfig;

    private static final Logger LOGGER = LoggerFactory.getLogger(WebIMService.class);

    public boolean isMerchantAccountOnline(long merchantID) {
        boolean result = false;

        Map<String, Object> params = new HashMap<String, Object>();
        params.put("event", "GET_MERCHANT_ONLINE_INFO");
        params.put("sid", -1);
        params.put("merchantids", merchantID);

        try {
            String response = HttpUtils.excutePost(businessConfig.getWebIMServiceURL(), params);
            JsonNode jsonNode = JsonUtil.toNode(response);

            if (jsonNode != null && jsonNode.get("success").asBoolean() && jsonNode.has("data")) {
                jsonNode = jsonNode.get("data");
                jsonNode = jsonNode.get("merchantOnlineInfo");
                Iterator<JsonNode> iterator = jsonNode.iterator();

                while (iterator.hasNext()) {
                    jsonNode = iterator.next();
                    boolean isOnline = jsonNode.get("online").asBoolean();

                    result = isOnline;
                    if (!result) {
                        break;
                    }
                }
            }
        } catch (Exception e) {
            LOGGER.error("从webim获取商家在线状态发生错误");
        }

        return result;
    }

    public Map<Long, Boolean> getOnlineSubAccountList(List<Long> subAccountList) {
        long subAccountID = -1l;

        Map<String, Object> params = new HashMap<String, Object>();
        params.put("event", "GET_SUB_MERCHANT_ONLINE_INFO");
        params.put("sid", -1);
        params.put("sub_merchant_ids", StringUtils.join(subAccountList, ','));

        Map<Long, Boolean> subAccountListOnlineInfoMap = new HashMap<Long, Boolean>();
        try {
            String response = HttpUtils.excutePost(businessConfig.getWebIMServiceURL(), params);
            JsonNode jsonNode = JsonUtil.toNode(response);

            if (jsonNode != null && jsonNode.get("success").asBoolean() && jsonNode.has("data")) {
                jsonNode = jsonNode.get("data");
                jsonNode = jsonNode.get("subMerchantOnlineInfo");
                Iterator<JsonNode> iterator = jsonNode.iterator();

                while (iterator.hasNext()) {
                    jsonNode = iterator.next();
                    boolean isOnline = jsonNode.get("online").asBoolean();
                    subAccountID = jsonNode.get("id").asInt();

                    subAccountListOnlineInfoMap.put(subAccountID, isOnline);
                }
            }
        } catch (Exception e) {
            LOGGER.error("从webim获取商家子账号在线状态发生错误");
            throw new RuntimeException(e);
        }

        return subAccountListOnlineInfoMap;
    }
}
