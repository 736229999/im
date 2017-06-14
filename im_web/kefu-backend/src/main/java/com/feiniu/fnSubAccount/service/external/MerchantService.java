package com.feiniu.fnSubAccount.service.external;

import com.feiniu.fnSubAccount.base.FeiniuCommonRequest;
import com.feiniu.fnSubAccount.bean.Merchant;
import com.feiniu.fnSubAccount.bean.MerchantInfo;
import com.feiniu.fnSubAccount.bean.StoreInfoPair;
import com.feiniu.fnSubAccount.bean.SubMerchantInfo;
import com.feiniu.fnSubAccount.constants.BusinessConfig;
import com.feiniu.fnSubAccount.consts.SystemConfig;
import com.feiniu.fnSubAccount.exception.BusinessException;
import com.feiniu.fnSubAccount.util.JsonUtil;
import com.feiniu.fnSubAccount.util.json.JsonNodeUtil;
import com.feiniu.fnSubAccount.util.web.HttpUtils;
import com.fn.cache.client.RedisCacheClient;
import org.apache.commons.lang.StringUtils;
import org.codehaus.jackson.JsonNode;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.io.IOException;
import java.util.*;

/**
 * Created by wangchen on 2015/2/6.
 */
@SuppressWarnings({ "unused","unchecked", "rawtypes" })
@Service
public class MerchantService {
    @Autowired
    private BusinessConfig businessConfig;
    @Autowired
    RedisCacheClient redisCacheClient;

    private static final String SUCCSS_CODE = "200";

    private static final Logger LOGGER = LoggerFactory.getLogger(MerchantService.class);

    public int doretrieveSubMerchantCount(String merchantIDs) {
        List<SubMerchantInfo> subMerchantInfoList = retrieveSubMerchantInfo(merchantIDs);

        return subMerchantInfoList.size();
    }

    public int retrieveSubMerchantCount(String merchantIDs) {
        int count = 0;
        int index = 0;
        int lastfound = 0;

        while (index >= 0) {
            int num = 100;
            while (num > 0 && index >= 0) {
                index = merchantIDs.indexOf(",", index + 1);
                num--;
            }
            if (index < 0) {
                count += doretrieveSubMerchantCount(merchantIDs.substring(lastfound));
                return count;
            } else
                count += doretrieveSubMerchantCount(merchantIDs.substring(lastfound, index));

            lastfound = index + 1;
        }
        return count;
    }


    public  List<SubMerchantInfo> retrieveSubMerchantListInfos(String merchantIDs) {
        List<SubMerchantInfo> subMerchantInfoList = retrieveSubMerchantInfo(merchantIDs);
        return subMerchantInfoList;
    }

    public Map<Long, List<SubMerchantInfo>> retrieveSubMerchantInfos(String merchantIDs) {

        Map<Long, List<SubMerchantInfo>> subMerchantInfoMap = new HashMap<Long, List<SubMerchantInfo>>();

        List<SubMerchantInfo> subMerchantInfoList = retrieveSubMerchantInfo(merchantIDs);

        for (SubMerchantInfo index : subMerchantInfoList) {
            if (subMerchantInfoMap.get(index.getMerchantId()) == null) {
                List<SubMerchantInfo> node = new ArrayList<SubMerchantInfo>();
                subMerchantInfoMap.put(index.getMerchantId(), node);
            }
            subMerchantInfoMap.get(index.getMerchantId()).add(index);
        }

        return subMerchantInfoMap;
    }

    public List<Merchant> getMerchantInfos(String merchantIDs) {
        if (merchantIDs.isEmpty())
            return null;
        else {
            return dogetMerchantInfoByParam("merchantId", merchantIDs);
        }
    }

    public List<SubMerchantInfo> retrieveSubMerchantInfoByPartName(String merchantID,String partName){
        List<SubMerchantInfo>  subMerchantInfoList = retrieveSubMerchantInfo(merchantID);

        Iterator<SubMerchantInfo> iter = subMerchantInfoList.iterator();

        while (iter.hasNext()){
            SubMerchantInfo index = iter.next();

            if (index.getUsername().indexOf(partName) < 0 )
                iter.remove();
        }

        return  subMerchantInfoList;
    }

    public List<SubMerchantInfo> retrieveSubMerchantInfo(String merchantID) {
        List<SubMerchantInfo> allList = new ArrayList<>();
        String merchantIds[] = merchantID.split(",");
        StringBuilder merchantNotFindInRedis = new StringBuilder();

        //从redis获取商家子账号，如果redis里面没有，上商家ID加入merchantNotFindInRedis
        for (String merchantIdIndex :merchantIds){
            String infoKey = SystemConfig.SUB_MERCHANT_KEY+"#"+ businessConfig.getMemcacheMagicNumber()+"#"+merchantIdIndex;
            Map<String, String> subMerchantMap = redisCacheClient.hgetAll(infoKey);
            if (subMerchantMap == null || subMerchantMap.size() == 0) {
                merchantNotFindInRedis.append(merchantIdIndex + ",");
                break;
            }
            for (Map.Entry<String,String> mapIndex:subMerchantMap.entrySet()){
                allList.add(
                        JsonNodeUtil.injectBean(mapIndex.getValue(), SubMerchantInfo.class));
            }
        }
        //所有商家子账号都在redis里面找到了
        if (merchantNotFindInRedis.length() == 0){
            return allList;
        }

        //调用接口去获取子账号列表
        try {
            Map<String, Object> params = new HashMap<>(1);
            params.put("params", merchantNotFindInRedis.substring(0,merchantNotFindInRedis.length()-1));

            String response = HttpUtils.excutePost(
                    businessConfig.getMerchantMemberApiServiceName()+"/memberApi/getSubAccountsByIds.do",
                    params);
            JsonNode jsonNode = JsonUtil.toNode(response);

            if (jsonNode != null ) {
                List<SubMerchantInfo> httpList = JsonUtil.toList(jsonNode.toString(), SubMerchantInfo.class);
                allList.addAll(httpList);

                for (SubMerchantInfo httpListIndex :httpList){
                    String infoKey = SystemConfig.SUB_MERCHANT_KEY+"#"+
                            businessConfig.getMemcacheMagicNumber()+"#"+
                            String.valueOf(httpListIndex.getMerchantId());
                    redisCacheClient.hput(infoKey,
                            String.valueOf(httpListIndex.getMemberId()),
                            JsonUtil.toJsonString(httpListIndex) );

                    redisCacheClient.expire(infoKey, SystemConfig.FRIEND_LIST_TIMEOUT);
                }
            } else {
                throw new BusinessException("商家-获取商家子账号信息发生错误");
            }
        } catch (IOException e) {
            LOGGER.error("商家-获取商家子账号信息发生错误", e);
            throw new RuntimeException(e);
        }
        return allList;
    }

    public String getMerchantIDByStoreName(String storeName) {
        if (StringUtils.isEmpty(storeName)) {
            LOGGER.error("调用商家服务-通过店铺名称获取商家id失败，店铺名称参数为空");
            throw new NullPointerException();
        } else {
            String merchantID = null;
            try {
                Map<String, Object> map = new HashMap<String, Object>();
                map.put("shopName", storeName);
                String response = HttpUtils.executeGet(businessConfig.getMerchantInfoAPI(), "",
                        map);
                JsonNode jsonNode = JsonUtil.toNode(response);
                if (jsonNode != null && SUCCSS_CODE.equals(JsonNodeUtil.getString(jsonNode, "result")) && jsonNode.has("data")) {
                    if (jsonNode.get("data").isNull()) {
//                        throw new BusinessException("店铺不存在");
                        return null;
                    }
                    merchantID = JsonNodeUtil.getString(jsonNode, "data");
                } else {
                    throw new BusinessException("商家-获取商家子账号信息发生错误");
                }
            } catch (Exception e) {
                LOGGER.error("商家-获取商家子账号信息发生错误", e);
                throw new RuntimeException(e);
            }

            return merchantID;
        }
    }


    //获取 主营经营大类
    public Map<String,String> getMainCategoryNameByMerchantIds(String merchantIds) {
        if (merchantIds.isEmpty())
            return null;
        else {
            return getMainCategoryNameByParam(merchantIds);
        }
    }
    public String getMainCategoryNameByMerchantId(String merchantId) {
        if (merchantId.isEmpty())
            return null;
        else {
            Map<String,String>  mainCategoryNameMap = getMainCategoryNameByParam(merchantId);
            if (mainCategoryNameMap.size() == 0)
                return null;

            return mainCategoryNameMap.get((Object)merchantId);
        }
    }
    public Map<String,String> getMainCategoryNameByParam(String merchantId) {
        if (StringUtils.isEmpty(merchantId)) {
            LOGGER.error("调用商家服务-通过店铺编号获取商家主营经营大类失败，店铺编号参数为空");
            throw new NullPointerException();
        } else {
            Map<String,String> mainCategoryNameMap = new HashMap<>();
            String mainCategoryName = null;
            try {
                Map<String, Object> params = new HashMap<>(1);
                params.put("merchantId", merchantId);

                FeiniuCommonRequest feiniuCommonRequest = new FeiniuCommonRequest();
                feiniuCommonRequest.setParams(JsonUtil.toJsonString(params));
                String response = HttpUtils.excutePost(businessConfig.getMerchantInfoURL(), params);
                JsonNode jsonNode = JsonUtil.toNode(response);
                if (jsonNode != null && SUCCSS_CODE.equals(JsonNodeUtil.getString(jsonNode, "result")) && jsonNode.has("data")) {
                    if (jsonNode.get("data").isNull()) {
//                        throw new BusinessException("店铺不存在");
                        return mainCategoryNameMap;
                    }
                    JsonNode dataNode = jsonNode.get("data");

                    for(int index=0;index<dataNode.size();index++){
                        JsonNode dataNodeindex = dataNode.get(index);
                        dataNodeindex.get("mainCategoryName");
                        if(dataNodeindex!=null && dataNodeindex.has("mainCategoryName") && !dataNodeindex.get("mainCategoryName").isNull()){
                            mainCategoryNameMap.put(JsonNodeUtil.getString(dataNodeindex, "id"),JsonNodeUtil.getString(dataNodeindex, "mainCategoryName"));
                        }else{
                            return mainCategoryNameMap;
                        }
                    }

                } else {
                    //throw new BusinessException("商家-获取商家账号信息发生错误");
                    LOGGER.error("商家-获取商家账号信息发生错误");

                }
            } catch (Exception e) {
                LOGGER.error("商家-获取商家账号信息发生错误", e);
                throw new RuntimeException(e);
            }

            return mainCategoryNameMap;
        }
    }

    public List<StoreInfoPair> getStoreInfoPairListByStoreNamePart(String storeNamePart) {
        if (StringUtils.isEmpty(storeNamePart)) {
            LOGGER.error("调用商家服务-通过店铺名称获取商家id失败，店铺名称参数为空");
            throw new NullPointerException();
        } else {
            Map<String, String> params = new HashMap<>(1);
            params.put("storeName", storeNamePart);

            FeiniuCommonRequest feiniuCommonRequest = new FeiniuCommonRequest();
            feiniuCommonRequest.setMethod("feiniu.merchant.getMerchantShopNames");
            feiniuCommonRequest.setParams(JsonUtil.toJsonString(params));

            try {
                String response = HttpUtils.excutePost(businessConfig.getMerchantServiceURL(), JsonUtil.toMap(feiniuCommonRequest));
                JsonNode jsonNode = JsonUtil.toNode(response);

                if (jsonNode != null && SUCCSS_CODE.equals(JsonNodeUtil.getString(jsonNode, "code")) && jsonNode.has("body")) {
                    JsonNode bodyNode = jsonNode.get("body");

                    if (bodyNode.get("storeList").isNull()) {
                        return Collections.EMPTY_LIST;
                    } else {
                        List<StoreInfoPair> storeInfoPairList = new ArrayList<>();

                        JsonNode listNode = bodyNode.get("storeList");
                        Iterator<JsonNode> iterator = listNode.iterator();
                        while (iterator.hasNext()) {
                            JsonNode currentProductNode = iterator.next();
                            storeInfoPairList.add(JsonNodeUtil.parseBean(currentProductNode.toString(), StoreInfoPair.class));
                        }

                        return storeInfoPairList;
                    }
                } else {
                    throw new BusinessException("商家-获取商家店铺信息发生错误");
                }
            } catch (IOException e) {
                LOGGER.error("商家-获取商家店铺信息发生错误", e);
                throw new RuntimeException(e);
            }
        }
    }

    public List<Merchant> dogetMerchantInfoByParam(String key, String param) {
        Map<String, String> params = new HashMap<>(1);
        params.put(key, param);

        FeiniuCommonRequest feiniuCommonRequest = new FeiniuCommonRequest();
        feiniuCommonRequest.setMethod("feiniu.merchant.queryMerchantsForMouMou");
        feiniuCommonRequest.setParams(JsonUtil.toJsonString(params));

        try {
            String response = HttpUtils.excutePost(businessConfig.getMerchantServiceURL(), JsonUtil.toMap(feiniuCommonRequest));
            JsonNode jsonNode = JsonUtil.toNode(response);


            if (jsonNode != null && SUCCSS_CODE.equals(JsonNodeUtil.getString(jsonNode, "code")) && jsonNode.has("body")) {
                JsonNode bodyNode = jsonNode.get("body");

                List<Merchant> storeInfoPairList = new ArrayList<>();

                Iterator<JsonNode> iterator = bodyNode.iterator();
                while (iterator.hasNext()) {
                    JsonNode currentProductNode = iterator.next();
                    storeInfoPairList.add(JsonNodeUtil.injectBean(currentProductNode.toString(), Merchant.class));
                }

                return storeInfoPairList;

            } else {
                throw new BusinessException("商家-获取商家店铺信息发生错误");
            }
        } catch (IOException e) {
            LOGGER.error("通过商家名称获取商家信息失败", e);
            throw new RuntimeException(e);
        }
    }

    public Merchant getMerchantInfoByMerchantName(String merchantName) {
        if (merchantName.isEmpty())
            return null;
        else {
            List<Merchant>  storeInfoPairList = dogetMerchantInfoByParam("merchantName", merchantName);
            if (storeInfoPairList.size() == 0)
                return null;

            return storeInfoPairList.get(0);
        }
    }

    public Merchant getMerchantInfoByMerchantId(String merchantId) {
        if (merchantId.isEmpty())
            return null;
        else {
            List<Merchant>  storeInfoPairList = dogetMerchantInfoByParam("merchantId", merchantId);
            if (storeInfoPairList.size() == 0)
                return null;

            return storeInfoPairList.get(0);
        }
    }



    public Merchant getMerchantInfoByShopName(String storename) {
        if (storename.isEmpty())
            return null;
        else {
            List<Merchant>  storeInfoPairList = dogetMerchantInfoByParam("shopName", storename);
            if (storeInfoPairList.size() == 0)
                return null;
            return storeInfoPairList.get(0);
        }
    }

    public List<Merchant> getMerchantInfoListByMerchantName(String merchantName) {
        return dogetMerchantInfoByParam("merchantName", merchantName);
    }


    public List<MerchantInfo> doGetMerchantInfoByParam(String key, String param) {
        Map<String, String> params = new HashMap<>(1);
        params.put(key, param);

        FeiniuCommonRequest feiniuCommonRequest = new FeiniuCommonRequest();
        feiniuCommonRequest.setMethod("feiniu.merchant.queryMerchantsForMouMou");
        feiniuCommonRequest.setParams(JsonUtil.toJsonString(params));

        try {
            String response = HttpUtils.excutePost(businessConfig.getMerchantServiceURL(), JsonUtil.toMap(feiniuCommonRequest));
            JsonNode jsonNode = JsonUtil.toNode(response);

            if (jsonNode != null && SUCCSS_CODE.equals(JsonNodeUtil.getString(jsonNode, "code")) && jsonNode.has("body")) {
                JsonNode bodyNode = jsonNode.get("body");

                List<MerchantInfo> storeInfoPairList = new ArrayList<>();

                Iterator<JsonNode> iterator = bodyNode.iterator();
                while (iterator.hasNext()) {
                    JsonNode currentProductNode = iterator.next();
                    storeInfoPairList.add(JsonNodeUtil.injectBean(currentProductNode.toString(), MerchantInfo.class));
                }

                return storeInfoPairList;

            } else {
                throw new BusinessException("商家-获取商家店铺信息发生错误");
            }
        } catch (IOException e) {
            LOGGER.error("通过商家名称获取商家信息失败", e);
            throw new RuntimeException(e);
        }
    }

    public List<Long> getAllMerchantIds() {
        List<MerchantInfo> merchantInfoList = doGetMerchantInfoByParam("merchantName", "");

        List<Long> resultString= new ArrayList<>();
        for (MerchantInfo merchantInfo : merchantInfoList) {
            resultString.add(merchantInfo.getId());
        }

        return  resultString;
    }

}
