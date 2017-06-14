package com.feiniu.csWebIm.service.external;
import com.feiniu.csWebIm.bean.domain.ContactionInfo;
import com.feiniu.csWebIm.bean.domain.ContactionInfosList;
import com.feiniu.csWebIm.bean.domain.CustomerInfo;
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
 * Author: chenwuming
 * Date: 2015/10/23
 */

@Service
public class ContactService
{

    private static final String SUCCSS_CODE = "200";
    private static final Logger LOGGER = LoggerFactory.getLogger(ContactService.class);

    @Autowired
    private BusinessConfig businessConfig;

    public CustomerInfo getContactListById(String userId)
    {
        String serviceUrl = businessConfig.getContactInfoServiceURL();
        String method = SystemConfig.CONTACTION_TEL_SEARCH;
        CustomerInfo customerinfo = new CustomerInfo();
        ContactionInfosList contactInfosList;
        CustomerInfo custInfo = new CustomerInfo();
        String response = Constants.NIL_RESPONSE;
        String data = "{\"mem_guid\":\"" + userId + "\"}";
        Map<String, Object> map = new HashMap<String, Object>();
        map.put("data", data);
        try
        {
            response = HttpUtils.excutePost(serviceUrl + method, map);//POST请求
            JsonNode jsonNode = JsonUtil.toNode(response);
            if (jsonNode != null && SUCCSS_CODE.equals(JsonNodeUtil.getString(jsonNode, "code")) && jsonNode.has("data"))
            {
                contactInfosList = JsonNodeUtil.injectBean(response, ContactionInfosList.class);
                List<ContactionInfo> ContactsList = contactInfosList.getData();
                custInfo = getDefContactInfo(ContactsList);
            }
            else
            {
                LOGGER.error("查询联系人联系方式失败,数据为空");
                return  null;
            }
        } catch (Exception e) {
            LOGGER.error("查询联系人联系方式失败", e);
            return  null;
        }
        return custInfo;//返回处理后的结构
    }

    public CustomerInfo getDefContactInfo(List<ContactionInfo> ContactsList)
    {
        boolean bDefaultAddr = false;
        ContactionInfo contactInfo;
        CustomerInfo custInfo = new CustomerInfo();
        for (int i = 0; i < ContactsList.size(); i++)
        {
            contactInfo = ContactsList.get(i);
            if(contactInfo.CheckDefaultAddr() == true)
            {
                bDefaultAddr = true;
                custInfo.SetDefaultAddr(true);
                custInfo.setCustomerName(contactInfo.getCon_name())  ;
                custInfo.setCustomerTel(contactInfo.getCon_call_phone());
                break;
            }
        }
        if(!bDefaultAddr)
        {
            custInfo.SetDefaultAddr(false);
        }
        return custInfo;
    }
}

