package com.feiniu.csWebIm.service.external;

import com.feiniu.csWebIm.bean.domain.BaseEmployeeInfo;
import com.feiniu.csWebIm.bean.domain.EmployeeInfo;
import com.feiniu.csWebIm.bean.domain.FAQ;
import com.feiniu.csWebIm.bean.response.FAQResponse;
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
 * Created by wangzhen on 2015/12/23.
 */
@Service
public class FAQService {
    private static final String SUCCSS_CODE = "200";
    private static final Logger LOGGER = LoggerFactory.getLogger(EmployeeService.class);
    @Autowired
    private BusinessConfig businessConfig;

    public List<FAQ> getFAQList(){
        String serviceUrl = businessConfig.getKefuBackendURL();
        serviceUrl = serviceUrl.replace(SystemConfig.HTTP_HEAD,"");
        String method = SystemConfig.FAQ_METHOD;
        String response = Constants.NIL_RESPONSE;
        Map<String, Object> map = new HashMap<String, Object>();
        try {
            response = HttpUtils.executeGet(serviceUrl, method, map);//GET请求
            FAQResponse faqResponse = JsonNodeUtil.injectBean(response, FAQResponse.class,true);
            if (faqResponse != null && faqResponse.getSuccess()) {
                return faqResponse.getData().getFaq();
            } else {
                LOGGER.error("查询当前客服信息失败，数据为空" );
                return  null;
            }
        } catch (Exception e) {
            LOGGER.error("查询当前客服信息失败", e);
            return  null;
        }
    }
}
