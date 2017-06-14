package com.feiniu.subAccount.service.external;

import com.feiniu.subAccount.bean.response.StoreInfo;
import com.feiniu.subAccount.bean.response.StoreInfoResponse;
import com.feiniu.subAccount.constants.BusinessConfig;
import com.feiniu.subAccount.constants.Constants;
import com.feiniu.subAccount.exception.BusinessException;
import com.feiniu.subAccount.util.JsonUtil;
import com.feiniu.subAccount.util.json.JsonNodeUtil;
import com.feiniu.subAccount.util.web.HttpUtils;
import org.apache.commons.lang.StringUtils;
import org.codehaus.jackson.JsonNode;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.*;

/**
 * Created by wangchen on 2015/1/13.
 */
@Service
public class StoreService {
    @Autowired
    private BusinessConfig businessConfig;

    private static final Logger LOGGER = LoggerFactory.getLogger(StoreService.class);

    public StoreInfo retrieveStoreInfo(String merchantID) {
        String response = Constants.NIL_RESPONSE;
        StoreInfo storeInfo = null;

        try {
            response = HttpUtils.executeGet(businessConfig.getStoreServiceURL(),
                    "/merchantStore/getMerchantStore/" + merchantID, Collections.EMPTY_MAP);
            JsonNode jsonNode = JsonUtil.toNode(response);

            if (jsonNode != null && jsonNode.has("flag") && "1".equals(JsonNodeUtil.getString(jsonNode, "flag"))) {
                storeInfo = JsonNodeUtil.getBean(JsonNodeUtil.getString(jsonNode, "datas"), StoreInfo.class);
            }else{
                throw new BusinessException("店铺-查找店铺接口返回错误数据");
            }
        } catch (Exception e) {
            LOGGER.error("店铺-获取店铺相关信息失败", e);
        }

        return storeInfo;
    }

    public List<StoreInfo> batchRetriveStoreInfo(Collection merchantIDList) {
        String response = Constants.NIL_RESPONSE;

        try {
            Map<String, Object> map = new HashMap<String, Object>();
            map.put("merchantIds", StringUtils.join(merchantIDList, ","));
            response = HttpUtils.executeGet(businessConfig.getStoreServiceURL(), "/merchantStore/getMerchantStoreListByMerchantIds", map);
            StoreInfoResponse storeInfoResponse = JsonNodeUtil.getBean(response, StoreInfoResponse.class, true);

            if (storeInfoResponse != null && storeInfoResponse.getFlag() == 1){
                List<StoreInfo> storeInfos = storeInfoResponse.getDatas();
                return storeInfos;
            }else {
                throw new BusinessException("店铺-批量获取店铺信息返回数据错误");
            }
        } catch (Exception e) {
            LOGGER.error("店铺-批量获取店铺相关信息失败", e);
        }

        return Collections.EMPTY_LIST;
    }

}
