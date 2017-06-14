package com.feiniu.imServer.service.external;

import com.feiniu.imServer.base.FeiniuCommonRequest;
import com.feiniu.imServer.bean.MerchantSubAccountInfo;
import com.feiniu.imServer.bean.SubMerchantInfo;
import com.feiniu.imServer.constants.BusinessConfig;
import com.feiniu.imServer.consts.ShuntConfig;
import com.feiniu.imServer.consts.SystemConfig;
import com.feiniu.imServer.exception.BusinessException;
import com.feiniu.imServer.repository.UserInfoDao;
import com.feiniu.imServer.util.JsonUtil;
import com.feiniu.imServer.util.json.JsonNodeUtil;
import com.feiniu.imServer.util.web.HttpClientUtils;
import com.feiniu.imServer.util.web.HttpUtils;
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
 * Created by chenqing on 2016/1/29.
 */
@Service
public class MerchantService {

    private static final String SUCCSS_CODE = "200";
    private static final Logger LOGGER = LoggerFactory.getLogger(MerchantService.class);
    @Autowired
    private BusinessConfig businessConfig;
    @Autowired
    private UserInfoDao userInfoDao;
    @Autowired
    RedisCacheClient redisCacheClient;

    //根据商家ID得到子账号
    public List<SubMerchantInfo> retrieveSubMerchantInfo(String merchantID) {
        List<SubMerchantInfo> allList = new ArrayList<>();
        String merchantIds[] = merchantID.split(",");
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
                    redisCacheClient.expire(infoKey, ShuntConfig.FRIEND_LIST_TIMEOUT);
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

    public Long retrieveMainSubMerchntId(Long merchantID){
        List<SubMerchantInfo> subMerchantInfoList = new ArrayList<>();
        subMerchantInfoList = retrieveSubMerchantInfo(merchantID.toString());

        StringBuilder stringBuilder = new StringBuilder();

        if (subMerchantInfoList.size() == 0)
            return null;

        for (SubMerchantInfo subMerchantInfo : subMerchantInfoList) {
            if (subMerchantInfo.getMemberType() == 1)
                return  subMerchantInfo.getMemberId();
        }

        return null;
    }

    public String retrieveSubMerchantIdList(Long merchantID,Long groupId) {
        List<SubMerchantInfo> subMerchantInfoList = new ArrayList<>();
        if (groupId == Long.valueOf(0))
            subMerchantInfoList = retrieveSubMerchantInfo(merchantID.toString());
        else
            return userInfoDao.getUserIdListInGroup(groupId);

        StringBuilder stringBuilder = new StringBuilder();

        if (subMerchantInfoList.size() == 0)
            return null;

        for (SubMerchantInfo subMerchantInfo : subMerchantInfoList) {
            stringBuilder.append(String.valueOf(subMerchantInfo.getMemberId()) + ",");
        }
        return stringBuilder.substring(0, stringBuilder.length() - 1);
    }


    //根据memberID获取merchantID
    public Map<String, Integer> retriveMerchantIDsByMemberIDs(List<Long> memberIDList) {
        if (memberIDList == null || memberIDList.size() == 0)
            return Collections.EMPTY_MAP;

        StringBuilder memberIdListBuffer = new StringBuilder();
        for (Long memberIndex : memberIDList){
            memberIdListBuffer.append(memberIndex.toString() + ",");
        }

        Map<String, Integer> result = null;
        //调用接口去获取子账号列表
        try {
            Map<String, Object> params = new HashMap<>(1);
            Map<String, Object> paramsMap = new HashMap<>(1);
            paramsMap.put("memberId",memberIdListBuffer.substring(0,memberIdListBuffer.length()-1));
            params.put("params", JsonUtil.toJsonString(paramsMap));

            String response = HttpUtils.excutePost(
                    businessConfig.getMerchantMemberApiServiceName()+"/memberApi/getMerchantIdByMemberId.do",
                    params);
            JsonNode jsonNode = JsonUtil.toNode(response);

            if (jsonNode != null && SUCCSS_CODE.equals(JsonNodeUtil.getString(jsonNode, "code"))
                    && jsonNode.has("data")) {
                result = JsonUtil.toMap(jsonNode.get("data").toString());
            } else {
                throw new BusinessException("商家-查找主账号接口返回错误数据：" + response);
            }
        } catch (IOException e) {
            LOGGER.error("商家-获取商家主账号时发生网络错误");
            throw new RuntimeException(e);
        }

        return result;
    }

    public MerchantSubAccountInfo retrieveMemberInfo(String memberID) {
        FeiniuCommonRequest feiniuCommonRequest = new FeiniuCommonRequest();
        feiniuCommonRequest.setMethod("feiniu.member.getById");
        feiniuCommonRequest.setParams(memberID);

        MerchantSubAccountInfo merchantInfo = null;
        try {
            String response = HttpClientUtils.excutePost(businessConfig.getMerchantServiceURL(), JsonUtil.toMap(feiniuCommonRequest));
            JsonNode jsonNode = JsonUtil.toNode(response);

            if (jsonNode != null && SUCCSS_CODE.equals(JsonNodeUtil.getString(jsonNode, "code")) &&
                    StringUtils.isNotEmpty(jsonNode.get("body").toString())) {
                merchantInfo = JsonNodeUtil.injectBean(jsonNode.get("body").toString(), MerchantSubAccountInfo.class);
            } else {
                throw new BusinessException("商家-获取账号个人信息发生错误：" + response );
            }
        } catch (IOException e) {
            LOGGER.error("商家-获取账号个人信息发生网络错误");
            throw new RuntimeException(e);
        }

        return merchantInfo;
    }
}
