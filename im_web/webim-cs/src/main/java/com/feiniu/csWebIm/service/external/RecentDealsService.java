package com.feiniu.csWebIm.service.external;
import com.feiniu.csWebIm.bean.domain.*;
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

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.List;

/**
 * Created by chenwunmming on 2015/11/16
 */
@Service
public class RecentDealsService {

    private static final String SUCCSS_CODE = "200";
    private static final Logger LOGGER = LoggerFactory.getLogger(RecentDealsService.class);

    @Autowired
    private BusinessConfig businessConfig;

    public  RecentDeals     getRecentDeals(String guid){
        String serviceUrl = businessConfig.getRecentDealsURL();
        String method = SystemConfig.RECENT_DEALS;
        String response = Constants.NIL_RESPONSE;
        RecentDeals recentDeals = new RecentDeals();
        //guid = "2E6A05D3-E30E-4EF6-28F7-2D97B8AB9ADF";
        String data = "{\"memGuid\":\"" + guid + "\"}";
        Map<String, Object> map = new HashMap<String, Object>();
        map.put("data", data);
        try
        {
            response = HttpUtils.excutePost(serviceUrl + method, map);//POST请求
            JsonNode jsonNode = JsonUtil.toNode(response);
            if (jsonNode != null && SUCCSS_CODE.equals(JsonNodeUtil.getString(jsonNode, "code")) && jsonNode.has("data"))
            {
                String jsonChildNode  = JsonNodeUtil.getString(jsonNode, "data");
                recentDeals = JsonNodeUtil.injectBean(jsonChildNode, RecentDeals.class, true);
            }
            else
            {
                LOGGER.error("查询上一笔订单失败,数据为空");
                return  null;
            }
        } catch (Exception e) {
            LOGGER.error("查询上一笔订单失败", e);
            return  null;
        }
        return recentDeals;//返回处理后的结构

    }

    public  RecentDeals getOrderDeals(String orid,String guid)
    {
        String serviceUrl = businessConfig.getOrderConsultURL();
        String method = SystemConfig.ORDER_CONSULT;
        String response = Constants.NIL_RESPONSE;
        RecentDeals recentDeals = new RecentDeals();
//        orid = "201510CP13100055";
//        guid = "7CAD9F5F-5B03-E300-EC17-04DD297A3A54";
        String data = "{\"ogNo\":\"" + orid +"\",\"memGuid\":\""+ guid+"\"}";
        Map<String, Object> map = new HashMap<String, Object>();
        map.put("data", data);
        try
        {
            response = HttpUtils.excutePost(serviceUrl + method, map);//POST请求
            JsonNode jsonNode = JsonUtil.toNode(response);
            if (jsonNode != null && SUCCSS_CODE.equals(JsonNodeUtil.getString(jsonNode, "code")) && jsonNode.has("data"))
            {
                String jsonChildNode  = JsonNodeUtil.getString(jsonNode, "data");
                recentDeals = JsonNodeUtil.injectBean(jsonChildNode, RecentDeals.class, true);
            }
            else
            {
                LOGGER.error("查询订单信息失败,数据为空");
                return  null;
            }
        } catch (Exception e) {
            LOGGER.error("查询订单信息失败", e);
            return  null;
        }
        return recentDeals;//返回处理后的结构

    }

}
