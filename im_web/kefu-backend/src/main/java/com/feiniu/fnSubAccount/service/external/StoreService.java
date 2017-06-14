package com.feiniu.fnSubAccount.service.external;

import com.feiniu.fnSubAccount.bean.StoreInfo;
import com.feiniu.fnSubAccount.bean.response.StoreInfoResponse;
import com.feiniu.fnSubAccount.constants.BusinessConfig;
import com.feiniu.fnSubAccount.constants.Constants;
import com.feiniu.fnSubAccount.consts.SystemConfig;
import com.feiniu.fnSubAccount.exception.BusinessException;
import com.feiniu.fnSubAccount.util.json.JsonNodeUtil;
import com.feiniu.fnSubAccount.util.JsonUtil;
import com.feiniu.fnSubAccount.util.web.HttpUtils;
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
@SuppressWarnings({ "unused","unchecked", "rawtypes" })
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
                storeInfo = JsonNodeUtil.injectBean(JsonNodeUtil.getString(jsonNode, "datas"), StoreInfo.class);
            } else {
                throw new BusinessException("店铺-查找店铺接口返回错误数据");
            }
        } catch (Exception e) {
            LOGGER.error("店铺-获取店铺相关信息失败", e);
        }

        return storeInfo;
    }

    public List<StoreInfo> batchRetriveStoreInfo(Collection<String> merchantIDList) {
        String response = Constants.NIL_RESPONSE;

        try {
            Map<String, Object> map = new HashMap<String, Object>();
            map.put("merchantIds", StringUtils.join(merchantIDList, ","));
            response = HttpUtils.executeGet(businessConfig.getStoreServiceURL(), "/merchantStore/getMerchantStoreListByMerchantIds", map);
            StoreInfoResponse storeInfoResponse = JsonNodeUtil.injectBean(response, StoreInfoResponse.class, true);

            if (storeInfoResponse != null && storeInfoResponse.getFlag() == 1) {
                List<StoreInfo> storeInfos = storeInfoResponse.getDatas();
                return storeInfos;
            } else {
                throw new BusinessException("店铺-批量获取店铺信息返回数据错误");
            }
        } catch (Exception e) {
            LOGGER.error("店铺-批量获取店铺相关信息失败", e);
        }

        return  Collections.EMPTY_LIST;
    }


    public List<StoreInfo> getStoreInfoListByIds(String[] idArray,int index ,int indexEnd){
        List<String> idList = new ArrayList<>();
        while (index < indexEnd) {
            idList.add(idArray[index]);
            index++;
        }
        List<StoreInfo> storeInfos = batchRetriveStoreInfo(idList);

        return storeInfos;
    }

    public Map<Integer,Integer> getMerchantStatusMap(String merchantIdList){
        String[] idArray = merchantIdList.split(",");
        Map<Integer,Integer> idToStatusMap = new HashMap<>();

        int index = 0;

        while ((idArray.length - index ) > SystemConfig.MAX_GET_STATUS_ID_COUNT) {
            int indexT = index + SystemConfig.MAX_GET_STATUS_ID_COUNT;
            List<StoreInfo> storeInfos = getStoreInfoListByIds(idArray,index,indexT);
            for (StoreInfo indexInfo : storeInfos) {
                idToStatusMap.put(indexInfo.getMerchantId(), indexInfo.getOpenType());
            }
            index = indexT;
        }

        List<StoreInfo> storeInfos = getStoreInfoListByIds(idArray,index,idArray.length);
        for (StoreInfo indexInfo : storeInfos) {
            idToStatusMap.put(indexInfo.getMerchantId(), indexInfo.getOpenType());
        }

        return  idToStatusMap;
    }

}
