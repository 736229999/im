package com.feiniu.subAccount.service.external;

import com.feiniu.subAccount.constants.BusinessConfig;
import com.feiniu.subAccount.constants.Constants;
import com.feiniu.subAccount.exception.BusinessException;
import com.feiniu.subAccount.util.JsonUtil;
import com.feiniu.subAccount.util.json.JsonNodeUtil;
import com.feiniu.subAccount.util.web.HttpUtils;
import org.codehaus.jackson.JsonNode;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by wangzhen on 2016/2/18.
 */
@Service
public class ImApiService {

    @Autowired
    private BusinessConfig businessConfig;
    private static final Logger LOGGER = LoggerFactory.getLogger(ImApiService.class);

    public void  modifyMerchantShuntInfoInRedis(Integer merchantId,Integer shuntType ,Integer proxySubAcountId){
        if (!businessConfig.isUseImApi())
            return  ;
        String response = Constants.NIL_RESPONSE;
        Map<String, Object> map = new HashMap<String, Object>();
        map.put("merchantId", merchantId);
        map.put("shuntType",shuntType);
        map.put("proxySubAcountId",proxySubAcountId);
        try {
            response = HttpUtils.executeGet(businessConfig.getImAPLURL(),
                    "/userInfo/modify-merchant-shunt-info", map);
            JsonNode jsonNode = JsonUtil.toNode(response);

            if (jsonNode != null  && JsonNodeUtil.getBoolean(jsonNode, "success")) {
                return;
            } else {
                throw new BusinessException("im-api:缓存更新商家分流方式失败");
            }
        } catch (Exception e) {
            LOGGER.error("im-api:缓存更新商家分流方式失败", e);
        }
        return ;
    }
}
