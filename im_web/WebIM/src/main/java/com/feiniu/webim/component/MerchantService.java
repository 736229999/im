package com.feiniu.webim.component;

import com.fasterxml.jackson.databind.JsonNode;
import com.feiniu.webim.bean.domain.*;
import com.feiniu.webim.config.ConfigManager;
import com.feiniu.webim.exception.BusinessException;
import com.feiniu.webim.util.json.JSONUtil;
import com.feiniu.webim.util.json.JsonNodeUtil;
import com.feiniu.webim.util.web.HttpClientUtils;
import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;

import java.io.IOException;
import java.util.*;

/**
 * Created by wangchen on 2015/1/13.
 */
public class MerchantService {
    private static final String MERCHANT_API_SERVICE_URL = ConfigManager.getProperty("service_merchant_api");
    private static final String MERCHANT_SERVICE_URL = ConfigManager.getProperty("service_merchant");
    public static final MerchantService instance = new MerchantService();

    private MerchantService() {
    }

    private static final String SUCCSS_CODE = "200";

    private static final Logger LOGGER = Logger.getLogger(MerchantService.class);

    public Map<String, Integer> retriveMerchantIDsByMemberIDs(List<Integer> memberIDList) {
        FeiniuCommonRequest feiniuCommonRequest = new FeiniuCommonRequest();
        feiniuCommonRequest.setMethod("feiniu.merchant.getMerchantIdByMemberId");
        feiniuCommonRequest.setParams(StringUtils.join(memberIDList, ","));

        Map<String, Integer> result = null;
        try {
            String response = HttpClientUtils.excutePost(MERCHANT_API_SERVICE_URL, JSONUtil.toMap(feiniuCommonRequest));
            JsonNode jsonNode = JSONUtil.toNode(response);
            if (jsonNode != null && SUCCSS_CODE.equals(JsonNodeUtil.getString(jsonNode, "code"))
                    && jsonNode.hasNonNull("body")) {
                result = JSONUtil.toMap(jsonNode.get("body").toString());
            } else {
                throw new BusinessException("商家-查找主账号接口返回错误数据：" + response);
            }
        } catch (IOException e) {
            LOGGER.error("商家-获取商家主账号时发生网络错误");
            throw new RuntimeException(e);
        }

        return result;
    }

    public MerchantInfo retrieveMerchantInfo(String merchantID) {
        FeiniuCommonRequest feiniuCommonRequest = new FeiniuCommonRequest();
        feiniuCommonRequest.setMethod("feiniu.merchant.getInfoForMoumou");
        feiniuCommonRequest.setParams(merchantID);

        MerchantInfo merchantInfo = null;
        try {
            String response = HttpClientUtils.excutePost(MERCHANT_API_SERVICE_URL, JSONUtil.toMap(feiniuCommonRequest));
            JsonNode jsonNode = JSONUtil.toNode(response);

            if (jsonNode != null && SUCCSS_CODE.equals(JsonNodeUtil.getString(jsonNode, "code")) && jsonNode.hasNonNull("body")) {
                merchantInfo = JsonNodeUtil.injectBean(jsonNode.get("body").toString(), MerchantInfo.class);
            } else {
                throw new BusinessException("商家-获取商家信息接口返回数据有误：" + response);
            }
        } catch (IOException e) {
            LOGGER.error("商家-获取商家相关信息发生网络错误");
            throw new RuntimeException(e);
        }

        return merchantInfo;
    }

    public MerchantSubAccountInfo retrieveMemberInfo(String memberID) {
        FeiniuCommonRequest feiniuCommonRequest = new FeiniuCommonRequest();
        feiniuCommonRequest.setMethod("feiniu.member.getById");
        feiniuCommonRequest.setParams(memberID);

        MerchantSubAccountInfo merchantInfo = null;
        try {
            String response = HttpClientUtils.excutePost(MERCHANT_API_SERVICE_URL, JSONUtil.toMap(feiniuCommonRequest));
            JsonNode jsonNode = JSONUtil.toNode(response);

            if (jsonNode != null && SUCCSS_CODE.equals(JsonNodeUtil.getString(jsonNode, "code")) && jsonNode.hasNonNull("body")) {
                merchantInfo = JsonNodeUtil.injectBean(jsonNode.get("body").toString(), MerchantSubAccountInfo.class);
                if (merchantInfo != null) {
                    merchantInfo.setId(jsonNode.get("body").get("memberId").asInt());
                }
            } else {
                throw new BusinessException("商家-获取账号个人信息发生错误：" + response);
            }
        } catch (IOException e) {
            LOGGER.error("商家-获取账号个人信息发生网络错误");
            throw new RuntimeException(e);
        }

        return merchantInfo;
    }

    //根据商家ID得到对应的子账号主ID
    public Long retrieveMainSubMerchantId(Long merchantID) {
        List<SubMerchantInfo> subMerchantInfoList = new ArrayList<>();
        subMerchantInfoList = retrieveSubMerchantInfo(merchantID.toString());

        if (subMerchantInfoList.size() == 0)
            return null;

        for (SubMerchantInfo subMerchantInfo : subMerchantInfoList) {
            if (subMerchantInfo.getMemberType() == 1)//判断是否为主账号
                return subMerchantInfo.getMemberId();
        }

        return null;
    }

    //根据商家ID得到所有的子账号
    public List<SubMerchantInfo> retrieveSubMerchantInfo(String merchantID) {
        FeiniuCommonRequest feiniuCommonRequest = new FeiniuCommonRequest();
        feiniuCommonRequest.setMethod("feiniu.member.getSubAccountsByIds");
        feiniuCommonRequest.setParams(merchantID);

        List<SubMerchantInfo> subMerchantInfoList;

        try {
            String response = HttpClientUtils.excutePost(MERCHANT_API_SERVICE_URL, JSONUtil.toMap(feiniuCommonRequest));
            JsonNode jsonNode = JSONUtil.toNode(response);

            if (jsonNode != null && SUCCSS_CODE.equals(JsonNodeUtil.getString(jsonNode, "code")) && jsonNode.has("body")) {
                subMerchantInfoList = JSONUtil.toList(JsonNodeUtil.getString(jsonNode, "body"), SubMerchantInfo.class);
            } else {
                throw new BusinessException("商家-获取商家子账号信息发生错误");
            }
        } catch (IOException e) {
            LOGGER.error("商家-获取商家子账号信息发生错误", e);
            throw new RuntimeException(e);
        }

        return subMerchantInfoList;
    }

    //获取店铺信息
    public String retrieveStoreName(String merchantID) {
        String getMerchantInfoURL = MERCHANT_SERVICE_URL + "merchantInfo/queryMerchants.do?merchantId=" + merchantID;

        try {
            String response = HttpClientUtils.excutePost(getMerchantInfoURL, Collections.<String, Object>emptyMap());
            JsonNode jsonNode = JSONUtil.toNode(response);

            if (jsonNode != null && SUCCSS_CODE.equals(JsonNodeUtil.getString(jsonNode, "result")) && jsonNode.hasNonNull("data")) {
                JsonNode dataNode = jsonNode.get("data");

                if (dataNode != null) {
                    Iterator<JsonNode> dataArrayNode = dataNode.iterator();

                    while (dataArrayNode.hasNext()) {
                        JsonNode merchantNode = dataArrayNode.next();
                        return JsonNodeUtil.getString(merchantNode, "shopName");
                    }
                }
            }

            throw new BusinessException("商家-获取商家信息接口返回数据有误：" + response);
        } catch (IOException e) {
            LOGGER.error("商家-获取商家相关信息发生网络错误");
            throw new RuntimeException(e);
        }
    }

    //获取店铺信息
    public Map<Integer, String> retrieveStoreNameMap(Collection<Integer> merchantIDList) {
        String getMerchantInfoURL = MERCHANT_SERVICE_URL + "merchantInfo/queryMerchants.do?merchantId=" + StringUtils.join(merchantIDList, ",");

        try {
            String response = HttpClientUtils.excutePost(getMerchantInfoURL, Collections.<String, Object>emptyMap());
            JsonNode jsonNode = JSONUtil.toNode(response);

            if (jsonNode != null && SUCCSS_CODE.equals(JsonNodeUtil.getString(jsonNode, "result")) && jsonNode.hasNonNull("data")) {
                JsonNode dataNode = jsonNode.get("data");

                if (dataNode != null) {
                    Iterator<JsonNode> dataArrayNode = dataNode.iterator();
                    Map<Integer, String> map = new HashMap<>(merchantIDList.size());

                    while (dataArrayNode.hasNext()) {
                        JsonNode merchantNode = dataArrayNode.next();
                        map.put(JsonNodeUtil.getInteger(merchantNode, "id"), JsonNodeUtil.getString(merchantNode, "shopName"));
                    }

                    return map;
                }
            }

            throw new BusinessException("商家-获取商家信息接口返回数据有误：" + response);
        } catch (IOException e) {
            LOGGER.error("商家-获取商家相关信息发生网络错误");
            throw new RuntimeException(e);
        }
    }

}
