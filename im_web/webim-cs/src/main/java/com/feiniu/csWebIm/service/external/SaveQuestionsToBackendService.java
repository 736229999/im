package com.feiniu.csWebIm.service.external;

import com.feiniu.csWebIm.bean.domain.AskingQuestion;
import com.feiniu.csWebIm.bean.domain.FaqCategoryList;
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
import java.util.List;
import java.util.Map;

/**
 * Created by chenwuming on 2015/11/26.
 */
@Service
public class SaveQuestionsToBackendService {

    private static final String SUCCSS_CODE = "true";
    private static final Logger LOGGER = LoggerFactory.getLogger(SaveQuestionsToBackendService.class);

    @Autowired
    BusinessConfig businessConfig;

    public boolean SaveQuestionToBackend(String categoryIDs){
        String serviceUrl = businessConfig.getSaveQuestionURL();
        String method = SystemConfig.SAVE_QUESTIONS_TO_BACKEND;
        String response = Constants.NIL_RESPONSE;
        Map<String, Object> map = new HashMap<String, Object>();
        map.put("categoryIDs",categoryIDs );
        try
        {
            response = HttpUtils.excutePost(serviceUrl+method, map);
            JsonNode jsonNode = JsonUtil.toNode(response);
            if (jsonNode != null && SUCCSS_CODE.equals(JsonNodeUtil.getString(jsonNode, "success")) && jsonNode.has("data"))
            {
                return true;
            }
            else
            {
                LOGGER.error("查询咨询问题失败,数据为空");
                return  false;
            }
        } catch (Exception e) {
            LOGGER.error("查询咨询问题失败~", e);
            return  false;
        }
    }
}
