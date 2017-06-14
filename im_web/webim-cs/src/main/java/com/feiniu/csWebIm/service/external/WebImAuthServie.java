package com.feiniu.csWebIm.service.external;

import com.feiniu.csWebIm.base.ImWebRequest;
import com.feiniu.csWebIm.bean.domain.AuthToken;
import com.feiniu.csWebIm.bean.response.CachedUserInfo;
import com.feiniu.csWebIm.consts.BusinessConfig;
import com.feiniu.csWebIm.consts.Constants;
import com.feiniu.csWebIm.consts.SystemConfig;
import com.feiniu.csWebIm.service.EndecryptedManager;
import com.feiniu.csWebIm.service.CacheClientService;
import com.feiniu.csWebIm.utils.json.JsonNodeUtil;
import com.feiniu.csWebIm.utils.json.JsonUtil;
import com.feiniu.csWebIm.utils.web.HttpUtils;
import org.apache.commons.codec.binary.Base64;
import org.codehaus.jackson.JsonNode;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.net.URLEncoder;
import java.util.*;

/**
 * Created by wangzhen on 2015/11/26.
 */
@Service
public class WebImAuthServie {
    private static final Logger LOGGER = LoggerFactory.getLogger(WebImAuthServie.class);
    private static final String SUCCSS_CODE = "200";
    @Autowired
    BusinessConfig businessConfig;
    @Autowired
    CacheClientService cacheClientService;

    public String getCachedToken(String serviceId){
        try {
            String auth_key=SystemConfig.TOKEN_KEY.replace("userid",serviceId).replace("magicNumber",businessConfig.getMagicNumber());
            String sessionKey = cacheClientService.getHFieldValue(auth_key, SystemConfig.SESSION_KEY);

            if (sessionKey == null) {
                LOGGER.error("权限认证失败， auth_key :" + auth_key);
                LOGGER.error("权限认证失败，memcache和imserver中没找到token");
                return null;
            }

            return sessionKey;
        }catch (Exception e){
            LOGGER.error("memcache 获取token异常");
            LOGGER.error(e.toString());
            //memcache异常，去imserver中获取数据
            String token = imServerAuthWithServiceId(serviceId);
            return token;
        }
    }

    public String authWithSid(String sid ,String token){
        try {
            sid = URLEncoder.encode(sid, "utf-8");
            token = URLEncoder.encode(token,"utf-8");


            String decodedSid = EndecryptedManager.INSTANCE.decryptedWithURLDncode(sid);
            decodedSid = decodedSid.substring(0,decodedSid.indexOf(","));

            String serviceUrl = decodedSid + businessConfig.getWebImAuthURLPort();
            String response = Constants.NIL_RESPONSE;
            String customerId = null;

            Map<String, Object> map = new HashMap<String, Object>();
            map.put("VENDER_INFO_TYPE","CHECK_SESSION_TOKEN_LEGALITY");
            map.put("sid",sid);
            map.put("token",token);

            response = HttpUtils.executeGet(serviceUrl,"", map);//GET请求
            JsonNode jsonNode = JsonUtil.toNode(response);
            if (jsonNode != null && true == JsonNodeUtil.getBoolean(jsonNode, "success") && jsonNode.has("data")) {
                customerId = JsonNodeUtil.getString(jsonNode.get("data"),"guid");
            }else {
                LOGGER.error("调用webim认证接口失败");
            }

            return customerId;
        }catch (Exception e){
            LOGGER.error("调用webim认证接口失败",e);
            return  null;
        }
    }

    private String doImServerAuthWithServiceId(String serviceUrl,String serviceId){
        try {
            String method = SystemConfig.IM_AUTH_METHOD;
            String response = Constants.NIL_RESPONSE;

            String VERSION = "1.0";
            Map<String, Object> params = new HashMap<>(1);
            List<Long> serviceIdList = new ArrayList<>();
            serviceIdList.add(Long.valueOf(serviceId));
            params.put("ids", serviceIdList);

            ImWebRequest imWebRequest = new ImWebRequest();
            imWebRequest.setVersion(VERSION);
            imWebRequest.setMethod(method);
            imWebRequest.setParams(JsonUtil.toJsonString(params));

            response = HttpUtils.executeGet(serviceUrl ,"", JsonUtil.toMap(imWebRequest) );//GET请求

            JsonNode jsonNode = JsonUtil.toNode(response);
            if (jsonNode != null && true == JsonNodeUtil.getBoolean(jsonNode, "result") && jsonNode.has("tokens")) {
                jsonNode = jsonNode.get("tokens");
                Iterator<JsonNode> jsonNodeIterator = jsonNode.iterator();
                while (jsonNodeIterator.hasNext()) {
                    JsonNode node = jsonNodeIterator.next();
                    AuthToken  authToken = new AuthToken();
                    authToken.setId(JsonNodeUtil.getLong(node, "id"));
                    authToken.setToken(JsonNodeUtil.getString(node, "token"));
                    return authToken.getToken();
                }
                return null;
            }else {
                LOGGER.error("调用imserver接口失败，数据返回不正常");
                return  null;
            }
        }catch (Exception e){
            LOGGER.error("调用imserver接口失败",e);
            return  null;
        }
    }

    public String imServerAuthWithServiceId(String serviceId){

        String imServiceListStr = businessConfig.getImServerList();
        String[] imServiceArray= imServiceListStr.split(";");

        for (String imserverUrl:imServiceArray){
            String tokenIndex = doImServerAuthWithServiceId(imserverUrl,serviceId);

            if (tokenIndex !=null){
                return  new String(Base64.encodeBase64(tokenIndex.getBytes()));
            }
        }
        return  null;
    }
}
