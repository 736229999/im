package com.feiniu.csWebIm.service.external;

import com.feiniu.csWebIm.bean.domain.TotalConsumpt;
import com.feiniu.csWebIm.bean.domain.CustomerCost;
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
import java.util.Map;

/**
 * Created by chenwunmming on 2015/11/16
 */
@Service
public class TotalConsumptService {

    private static final String SUCCSS_CODE = "200";
    private static final Logger LOGGER = LoggerFactory.getLogger(TotalConsumptService.class);

    @Autowired
    private BusinessConfig businessConfig;
    public  CustomerCost getTotalCost(String guid){
        String serviceUrl = businessConfig.getTotalCostURL();
        String method = SystemConfig.TOTAL_COST;
        String response = Constants.NIL_RESPONSE;
        TotalConsumpt totalComsupt = new TotalConsumpt();
        CustomerCost customerCost = new CustomerCost();
        //guid ="BA78F1D9-6F3D-B9A9-987D-8517F0E2DF82";
        String data = "{\"memGuid\":\"" + guid + "\"}";
        Map<String, Object> map = new HashMap<String, Object>();
        map.put("data",data );
        try
        {
            response = HttpUtils.excutePost(serviceUrl + method, map);//POST请求
            JsonNode jsonNode = JsonUtil.toNode(response);
            if (jsonNode != null && SUCCSS_CODE.equals(JsonNodeUtil.getString(jsonNode, "code")) && jsonNode.has("data"))
            {
                String jsonChildNode  = JsonNodeUtil.getString(jsonNode, "data");
                totalComsupt = JsonNodeUtil.injectBean(jsonChildNode, TotalConsumpt.class);
                customerCost = totalComsupt.getCustomerCost();
            }
            else
            {
                LOGGER.error("查询累计消费失败,数据为空");
                return  null;
            }
        } catch (Exception e) {
            LOGGER.error("查询累计消费失败", e);
            return  null;
        }
        return customerCost;//返回处理后的结构

    }
}
