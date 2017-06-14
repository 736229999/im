package com.feiniu.webim.component;

import com.fasterxml.jackson.databind.JsonNode;
import com.feiniu.webim.bean.domain.StoreInfo;
import com.feiniu.webim.bean.domain.StoreInfoResponse;
import com.feiniu.webim.config.ConfigManager;
import com.feiniu.webim.constants.Constants;
import com.feiniu.webim.exception.BusinessException;
import com.feiniu.webim.exception.GetStoreInfoException;
import com.feiniu.webim.exception.HttpGetException;
import com.feiniu.webim.util.json.JSONUtil;
import com.feiniu.webim.util.json.JsonNodeUtil;
import com.feiniu.webim.util.web.HttpClientUtils;
import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;

import java.util.*;

/**
 * Created by wangchen on 2015/1/13.
 */
public class StoreService {
    public static final StoreService instance = new StoreService();

    private StoreService() {
    }

    private static final String STORE_SERVICE_URL = ConfigManager.getProperty("service_store");
    private static final Logger LOGGER = Logger.getLogger(StoreService.class);

    public StoreInfo retrieveStoreInfo(String merchantID) {
        try {
            String response = HttpClientUtils.executeGet(STORE_SERVICE_URL, "/merchantStore/getMerchantStore/" + merchantID, Collections.EMPTY_MAP);
            JsonNode jsonNode = JSONUtil.toNode(response);

            if (jsonNode != null && jsonNode.hasNonNull("flag") && "1".equals(JsonNodeUtil.getString(jsonNode, "flag"))) {
                return JsonNodeUtil.injectBean(JsonNodeUtil.getString(jsonNode, "datas"), StoreInfo.class);
            }else{
                throw new BusinessException("店铺-查找店铺接口返回错误数据：" + response);
            }
        } catch (HttpGetException e) {
            LOGGER.error("店铺-获取店铺相关信息发生错误", e);
            throw new RuntimeException(e);
        }
    }

    public List<StoreInfo> batchRetriveStoreInfo(Collection merchantIDList) {
        String response = Constants.NIL_RESPONSE;

        try {
            Map<String, Object> map = new HashMap<String, Object>();
            map.put("merchantIds", StringUtils.join(merchantIDList, ","));
            response = HttpClientUtils.executeGet(STORE_SERVICE_URL, "/merchantStore/getMerchantStoreListByMerchantIds", map);
            StoreInfoResponse storeInfoResponse = JsonNodeUtil.injectBean(response, StoreInfoResponse.class, true);

            if (storeInfoResponse != null && storeInfoResponse.getFlag() == 1){
                List<StoreInfo> storeInfos = storeInfoResponse.getDatas();
                return storeInfos;
            }else {
                throw new GetStoreInfoException("店铺-批量获取店铺信息返回数据错误：" + response);
            }
        } catch (HttpGetException e) {
            LOGGER.error("店铺-批量获取店铺相关信息发生错误", e);
            throw new GetStoreInfoException(e);
        }
    }

    public List<String> getRecommendProductList(String merchantID) {
        try {
            String response = HttpClientUtils.executeGet(STORE_SERVICE_URL, "/product/getProductRecommend/" + merchantID, Collections.EMPTY_MAP);
            JsonNode jsonNode = JSONUtil.toNode(response);

            if (jsonNode != null && jsonNode.hasNonNull("flag") && "1".equals(JsonNodeUtil.getString(jsonNode, "flag"))) {
                String skuIDs = JsonNodeUtil.getString(jsonNode, "datas");

                if (StringUtils.isEmpty(skuIDs)) {
                    return Collections.EMPTY_LIST;
                } else {
                    return Arrays.asList(skuIDs.split(","));
                }
            }else{
                throw new BusinessException("店铺-查找店铺推荐商品接口返回错误数据：" + response);
            }
        } catch (HttpGetException e) {
            LOGGER.error("店铺-查找店铺推荐商品接口返回错误数据", e);
            throw new RuntimeException(e);
        }

    }

}
