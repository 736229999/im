package com.feiniu.csWebIm.service;

import com.feiniu.csWebIm.bean.domain.AskingInfo;
import com.feiniu.csWebIm.bean.domain.AskingOpt;
import com.feiniu.csWebIm.bean.domain.ServicelogInfo;
import com.feiniu.csWebIm.bean.request.SaveServiceLogRequest;
import com.feiniu.csWebIm.consts.BusinessConfig;
import com.feiniu.csWebIm.consts.Constants;
import com.feiniu.csWebIm.consts.SystemConfig;
import com.feiniu.csWebIm.utils.json.JsonNodeUtil;
import com.feiniu.csWebIm.utils.json.JsonUtil;
import com.feiniu.csWebIm.utils.web.HttpUtils;
import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;
import org.codehaus.jackson.JsonNode;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by wangzhen on 2015/10/22.
 */
@Service
public class ServiceLogInfoService {
    private static final String SUCCSS_CODE = "新增成功";
    private static final Logger LOGGER = LoggerFactory.getLogger(ServiceLogInfoService.class);
    @Autowired
    private BusinessConfig businessConfig;

    public boolean formatRequstJsonToList(SaveServiceLogRequest saveServiceLogRequest){
        String strogSeq = saveServiceLogRequest.getOgSeq();
        try {
            String jsonNode = saveServiceLogRequest.getAskingOptions();

            if (jsonNode != null)
            {
                Gson gson = new Gson();
                List<AskingOpt> listofaskingOpt = gson.fromJson(jsonNode, new TypeToken<List<AskingOpt>>() {
                }.getType());
                saveServiceLogRequest.setAskingOptionsList(listofaskingOpt);
            }
            else
            {
                LOGGER.error("解析传入参数AskingOpt失败");
                return  false;
            }

           if(strogSeq!=null)
           {
               jsonNode = saveServiceLogRequest.getAskingGoodsInfo();
               if (jsonNode != null) {
                   Gson gson = new Gson();
                   List<AskingInfo> listofaskingGoods = gson.fromJson(jsonNode, new TypeToken<List<AskingInfo>>() {
                   }.getType());
                   saveServiceLogRequest.setAskingGoodsInfoList(listofaskingGoods);
               } else {
                   LOGGER.error("解析传入参数AskingInfo失败");
                   return false;
               }
           }
        }catch (Exception e) {
            LOGGER.error("解析传入参数失败",e);
            return  false;
        }
        return  true;
    }

    ////保存服务小记接口
    public  boolean getSaveServicelogService(ServicelogInfo servicelogInfo) {
        String serviceUrl = businessConfig.getSaveServicelogURL();
        String method = SystemConfig.SAVE_SERVICELOG;
        String response = Constants.NIL_RESPONSE;
        String data = JsonUtil.toJsonString(servicelogInfo);
        Map<String, Object> map = new HashMap<String, Object>();
        map.put("data", data);
        try {
            response = HttpUtils.excutePost(serviceUrl + method, map);//POST请求
            JsonNode jsonNode = JsonUtil.toNode(response);
            if (jsonNode != null && jsonNode.has("Header"))
            {
                JsonNode dataNode = jsonNode.get("Header");
                String Description = JsonNodeUtil.getString(dataNode, "Description");
                if(SUCCSS_CODE.equals(Description))
                {
                    return true;
                }
                else
                {
                    LOGGER.error("保存服务小记失败");
                    return false;
                }
            }
            else
            {
                LOGGER.error("保存服务小记失败,格式错误");
                return false;
            }
        } catch (Exception e) {
            LOGGER.error("保存服务小记失败", e);
            return false;
        }
    }
}
