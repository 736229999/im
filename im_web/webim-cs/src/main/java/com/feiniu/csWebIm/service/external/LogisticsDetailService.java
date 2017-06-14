package com.feiniu.csWebIm.service.external;

import com.feiniu.csWebIm.bean.domain.ExpInfo;
import com.feiniu.csWebIm.bean.domain.LogisticsDetail;
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
 * Created by chenwunmming on 2015/11/19
 */
@Service
public class LogisticsDetailService {

    private static final String SUCCSS_CODE = "200";
    private static final Logger LOGGER = LoggerFactory.getLogger(LogisticsDetailService.class);

    @Autowired
    private BusinessConfig businessConfig;

    public  LogisticsDetail getLogisticsInfo(String guid,String pack_no,String og_seq,Integer type)
    {
        String serviceUrl = businessConfig.getLogisticsDetailURL();
        String method = SystemConfig.LOGISTICS_INFO;//物流接口
        String response = Constants.NIL_RESPONSE;
        LogisticsDetail logisticsDetail = new LogisticsDetail();
//        guid ="4343DD09-0903-B305-4EAF-C6B2F15E054C";
//        pack_no = "20150805172449161500";
//        og_seq  = "201508CO05000169";
//        type = 2;
        String data = "{\"memGuid\":\"" + guid +"\",\"pack_no\":\""+ pack_no+"\",\"og_seq\":\""+ og_seq+"\",\"type\":\""+ type+"\"}";
        Map<String, Object> map = new HashMap<String, Object>();
        map.put("data",data );
        try
        {
            response = HttpUtils.excutePost(serviceUrl + method, map);//POST请求
            JsonNode jsonNode = JsonUtil.toNode(response);
            if (jsonNode != null && SUCCSS_CODE.equals(JsonNodeUtil.getString(jsonNode, "code")) && jsonNode.has("data"))
            {
                String jsonChildNode  = JsonNodeUtil.getString(jsonNode, "data");
                logisticsDetail = JsonNodeUtil.injectBean(jsonChildNode, LogisticsDetail.class,true);
            }
            else
            {
                LOGGER.error("查询物流信息失败,数据为空");
                return  null;
            }
        } catch (Exception e) {
            LOGGER.error("查询物流信息失败", e);
            return  null;
        }
        return logisticsDetail;
    }

    public List<ExpInfo> getLogisticsItemList(LogisticsDetail logisticsDetail)
    {
        List<ExpInfo> expInfolist = new ArrayList<ExpInfo>();
        expInfolist = logisticsDetail.getExp_info();
        if(expInfolist!=null)
        {
            return expInfolist;
        }
        return null;
    }
}
