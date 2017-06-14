package com.feiniu.webim.component;

import com.fasterxml.jackson.databind.JsonNode;
import com.feiniu.webim.bean.domain.ServiceGroup;
import com.feiniu.webim.bean.domain.StoreLightConfig;
import com.feiniu.webim.config.ConfigManager;
import com.feiniu.webim.constants.Constants;
import com.feiniu.webim.exception.BusinessException;
import com.feiniu.webim.util.json.JSONUtil;
import com.feiniu.webim.util.json.JsonNodeUtil;
import com.feiniu.webim.util.web.HttpClientUtils;
import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;

import java.io.IOException;
import java.util.*;

/**
 * Created by wangchen on 2015/2/25.
 */
public class KefuService {
    private static final String KEFU_SERVICE_URL = ConfigManager.getProperty("service_kefu");
    private static final Logger LOGGER = Logger.getLogger(KefuService.class);

    public static final KefuService instance = new KefuService();

    private KefuService() {
    }

    private static final String GET_STORE_LIGHT_CONFIG =
            KEFU_SERVICE_URL + "/retrieve-config-for-webim";

    public Map<String, Object> getStoreLightConfig(long merchantID) {
        Map<String, Object> params = new HashMap<>();
        params.put("merchantID", merchantID);

        Map<String, Object> result = new HashMap<>();
        try {
            String response = HttpClientUtils.excutePost(GET_STORE_LIGHT_CONFIG, params);

            JsonNode jsonNode = JSONUtil.toNode(response);
            if (jsonNode != null && jsonNode.has("success") && jsonNode.get("success").asBoolean()) {
                JsonNode dataNode = jsonNode.get("data");

                if (dataNode.has("generalConfig")) {
                    result.put("generalConfig", JsonNodeUtil.parseBean(
                            JsonNodeUtil.getString(dataNode, "generalConfig"), StoreLightConfig.class, new StoreLightConfig()));
                }
                if (dataNode.has("serviceGroupConfig")) {
                    result.put("serviceGroupConfig", JSONUtil.toList(
                            JsonNodeUtil.getString(dataNode, "serviceGroupConfig"), ServiceGroup.class));
                }
            } else {
                throw new BusinessException("商家服务-获取店铺点灯配置返回错误数据：" + response);
            }
        } catch (IOException e) {
            LOGGER.error("商家-获取商家主账号时发生错误");
            throw new RuntimeException(e);
        }

        return result;
    }

    private static final String GET_SUBACCOUNT_LIST_UNDER_GROUP_LIST =
            KEFU_SERVICE_URL + "/retrieve-subaccounts-under-group";

    public Map<Long, Set<Long>> getSubAccountListUnderGroupList(List<Long> groupIDList) {
        Map<String, Object> params = new HashMap<>();
        params.put("groupIDListJoinString", StringUtils.join(groupIDList, Constants.DEFAULT_WORD_SEPARATOR));

        Map<Long, Set<Long>> result = new HashMap<>();
        try {
            String response = HttpClientUtils.excutePost(GET_SUBACCOUNT_LIST_UNDER_GROUP_LIST, params);
            JsonNode jsonNode = JSONUtil.toNode(response);

            if (jsonNode != null
                    && jsonNode.has("success") && jsonNode.get("success").asBoolean()
                    && jsonNode.hasNonNull("data")
                    && jsonNode.get("data").hasNonNull("subAccountAndGroupMap")) {
                JsonNode dataNode = jsonNode.get("data").get("subAccountAndGroupMap");
                Iterator<Map.Entry<String, JsonNode>> mappingEntryIterator = dataNode.fields();

                while (mappingEntryIterator.hasNext()) {
                    Map.Entry<String, JsonNode> entry = mappingEntryIterator.next();
                    result.put(Long.parseLong(entry.getKey()), JSONUtil.toSet(entry.getValue().toString(), Long.class));
                }
            } else {
                throw new BusinessException("客服管理-获取客服分组下面的子账号集合返回错误数据：" + response);
            }
        } catch (IOException e) {
            LOGGER.error("客服管理-获取客服分组下面的子账号集合发生错误");
            throw new RuntimeException(e);
        }

        return result;
    }

    private static final String GET_PROXY_SUBACCOUNT_FOR_MERCHANT =
            KEFU_SERVICE_URL + "/retrieve-proxy-subaccount-for-merchant";

    public long getProxySubAccountForMerchant(long merchantID) {
        Map<String, Object> params = new HashMap<>();
        params.put("merchantID", merchantID);

        long merchantSubAccountID;
        try {
            String response = HttpClientUtils.excutePost(GET_PROXY_SUBACCOUNT_FOR_MERCHANT, params);
            JsonNode jsonNode = JSONUtil.toNode(response);

            if (jsonNode != null
                    && jsonNode.has("success") && jsonNode.get("success").asBoolean()
                    && jsonNode.hasNonNull("data")
                    && jsonNode.get("data").hasNonNull("proxySubAccountID")) {
                JsonNode dataNode = jsonNode.get("data");
                merchantSubAccountID = dataNode.get("proxySubAccountID").asLong();
            } else {
                throw new BusinessException("客服管理-获取商家代理账号发生错误数据：" + response);
            }
        } catch (IOException e) {
            LOGGER.error("客服管理-获取商家代理账号发生错误");
            throw new RuntimeException(e);
        }

        return merchantSubAccountID;
    }

}
