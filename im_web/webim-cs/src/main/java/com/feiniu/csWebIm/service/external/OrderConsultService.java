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
public class OrderConsultService {

    private static final String SUCCSS_CODE = "200";
    private static final Logger LOGGER = LoggerFactory.getLogger(OrderConsultService.class);

    @Autowired
    private BusinessConfig businessConfig;
    public  ConsultOrderInfo getOrderConsult(String orid,String guid)
    {
        String serviceUrl = businessConfig.getOrderConsultURL();
        String method = SystemConfig.ORDER_CONSULT;
        String response = Constants.NIL_RESPONSE;
        ConsultOrderInfo orderConsult = new ConsultOrderInfo();
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
                orderConsult = JsonNodeUtil.injectBean(jsonChildNode, ConsultOrderInfo.class, true);
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
        return orderConsult;//返回处理后的结构

    }

    public OrderConsultDetail getOrderDetail(ConsultOrderInfo orderConsult)
    {
        OrderConsultDetail orderDetail = new OrderConsultDetail();
        orderDetail =  orderConsult.getOrderDetail();
        if(orderDetail!=null)
        {
            return orderDetail;
        }
        return null;
    }

    public List<ConsultPackageList> getOrderPackDetailInfo(ConsultOrderInfo orderConsult)
    {
        List<ConsultPackageList> packList = new ArrayList<ConsultPackageList>();
        packList = orderConsult.getPackageList();
        if(packList!=null)
        {
            return packList;
        }
        return null;
    }


    public List<List<ItemList>> getOrderPackageList(List<PackageList> packList)
    {
        List<List<ItemList>> orderDetailInfoList = new ArrayList<>();
        if(packList!=null) {
            for (int i = 0; i < packList.size(); i++) {
                orderDetailInfoList.add(packList.get(i).getItemList());
            }
        }
        return orderDetailInfoList;
   }
}
