package com.feiniu.csWebIm.service.external;

import com.feiniu.csWebIm.bean.domain.AskingGoodsCart;
import com.feiniu.csWebIm.bean.response.AskingGoodsResponse;
import com.feiniu.csWebIm.consts.BusinessConfig;
import com.feiniu.csWebIm.consts.Constants;
import com.feiniu.csWebIm.consts.SystemConfig;
import com.feiniu.csWebIm.utils.json.JsonNodeUtil;
import com.feiniu.csWebIm.utils.json.JsonUtil;
import com.feiniu.csWebIm.utils.web.HttpUtils;
import org.codehaus.jackson.JsonNode;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by wangzhen on 2015/10/29.
 */
@Service
public class AskingGoodsService {

    private static final String SUCCSS_CODE = "200";
    private static final Logger LOGGER = LoggerFactory.getLogger(AskingGoodsService.class);

    @Autowired
    BusinessConfig businessConfig;

    public List<AskingGoodsCart>  getCartForCustomer(String guid){
        String serviceUrl = businessConfig.getCartServiceURL();
        String method = SystemConfig.FEINIU_CART_GET_CART_FOR_CUSTOMER;
        String response = Constants.NIL_RESPONSE;
        Map<String, Object> map = new HashMap<String, Object>();
        map.put("mem_guid",guid );
        try
        {
            response = HttpUtils.executeGet(serviceUrl , method, map);
            JsonNode jsonNode = JsonUtil.toNode(response);
            AskingGoodsResponse askingGoodsResponse= JsonNodeUtil.injectBean(response, AskingGoodsResponse.class, true);
            if (askingGoodsResponse != null && askingGoodsResponse.getCode() =="1") {
                List<AskingGoodsCart> askingGoodsCartList = askingGoodsResponse.getData();
                return askingGoodsCartList;
            } else {
                LOGGER.error("查询购物车数据失败,数据为空");
                return  null;
            }
        } catch (Exception e) {
            LOGGER.error("查询购物车数据失败~", e);
            return  null;
        }
    }
}
