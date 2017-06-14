package com.feiniu.subAccount.service.external;

import com.feiniu.subAccount.base.FeiniuCommonRequest;
import com.feiniu.subAccount.bean.response.MerchantInfo;
import com.feiniu.subAccount.bean.response.SubMerchantInfo;
import com.feiniu.subAccount.constants.BusinessConfig;
import com.feiniu.subAccount.constants.Constants;
import com.feiniu.subAccount.consts.SystemConfig;
import com.feiniu.subAccount.domain.MerchantConnectInfo;
import com.feiniu.subAccount.exception.BusinessException;
import com.feiniu.subAccount.util.JsonUtil;
import com.feiniu.subAccount.util.json.JsonNodeUtil;
import com.feiniu.subAccount.util.web.HttpUtils;
import com.fn.cache.client.RedisCacheClient;
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
@Service
public class MerchantService {
    @Autowired
    private BusinessConfig businessConfig;
    @Autowired
    RedisCacheClient redisCacheClient;
    private static final String EMPTY_RESPONSE = "{}";
    private static final String SUCCSS_CODE = "200";

    private static final Logger LOGGER = LoggerFactory.getLogger(MerchantService.class);

    public  List<SubMerchantInfo> retrieveSubMerchantInfos(String merchantIDs) {
        List<SubMerchantInfo> allList = new ArrayList<>();
        String merchantIds[] = merchantIDs.split(",");
        StringBuilder merchantNotFindInRedis = new StringBuilder();

        //从redis获取商家子账号，如果redis里面没有，上商家ID加入merchantNotFindInRedis
        for (String merchantIdIndex :merchantIds){
            String infoKey = SystemConfig.SUB_MERCHANT_KEY+"#"+ businessConfig.getMagicNumber()+"#"+merchantIdIndex;
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
                            businessConfig.getMagicNumber()+"#"+
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

    public List<SubMerchantInfo> retrieveSubMerchantInfo(String merchantID) {
        FeiniuCommonRequest feiniuCommonRequest = new FeiniuCommonRequest();
        feiniuCommonRequest.setMethod("feiniu.member.getSubAccountList");
        feiniuCommonRequest.setParams(merchantID);

        List<SubMerchantInfo> subMerchantInfoList;

        try {
            String response = HttpUtils.excutePost(businessConfig.getMerchantServiceURL(), JsonUtil.toMap(feiniuCommonRequest));
            JsonNode jsonNode = JsonUtil.toNode(response);

            if (jsonNode != null && SUCCSS_CODE.equals(JsonNodeUtil.getString(jsonNode, "code")) && jsonNode.has("body")) {
                subMerchantInfoList = JsonUtil.toList(JsonNodeUtil.getString(jsonNode, "body"), SubMerchantInfo.class);
            } else {
                throw new BusinessException("商家-获取商家子账号信息发生错误");
            }
        } catch (IOException e) {
            LOGGER.error("商家-获取商家子账号信息发生错误", e);
            throw new RuntimeException(e);
        }

        return subMerchantInfoList;
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

    public MerchantConnectInfo doGetMerchantPhoneInfoById(String merchantID) {
        Map<String, String> params = new HashMap<>(1);

        String URL = businessConfig.getMerchantServiceURL().replace("api/","merchantInfo/")
                .replace("call","queryMerchants").replace("http://","");
        String response = Constants.NIL_RESPONSE;
        MerchantConnectInfo merchantConnectInfo = new MerchantConnectInfo();

        Map<String, Object> map = new HashMap<String, Object>();
        map.put("merchantId",merchantID );
        try {
            response = HttpUtils.executeGet(URL,"", map);
            JsonNode jsonNode = JsonUtil.toNode(response);

            if (jsonNode != null  && "200".equals(JsonNodeUtil.getString(jsonNode, "result"))) {
                jsonNode = jsonNode.get("data");
                Iterator<JsonNode> iterator = jsonNode.iterator();

                while (iterator.hasNext()) {
                    jsonNode = iterator.next();
                    merchantConnectInfo.setPhone(JsonNodeUtil.getString(jsonNode, "contact2Tel"));
                    merchantConnectInfo.setTelphone(JsonNodeUtil.getString(jsonNode, "contact2Mobile"));
                }
            } else {
                throw new BusinessException("查找商家电话失败");
            }
        } catch (Exception e) {
            LOGGER.error("查找商家电话失败", e);
        }

        return merchantConnectInfo;
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
