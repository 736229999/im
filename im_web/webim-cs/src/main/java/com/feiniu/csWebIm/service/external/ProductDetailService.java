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
import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.*;

/**
 * Created by chenwunmming on 2015/11/16
 */
@Service
public class ProductDetailService {

    private static final String SUCCSS_CODE = "200";
    private static final Logger LOGGER = LoggerFactory.getLogger(ProductDetailService.class);

    @Autowired
    private BusinessConfig businessConfig;

    public  List<ProductData> getProductDetail(String orid) {
        String serviceUrl = businessConfig.getProductDetailURL();
        String method = SystemConfig.PRODUCT_DETAIL;
        String response = Constants.NIL_RESPONSE;
        List<ProductData> productDatas = new ArrayList<ProductData>();
       // orid = "201503CP27000140";
        String data = "{\"og_no\":\"" + orid + "\"}";
        Map<String, Object> map = new HashMap<String, Object>();
        map.put("data", data);
        try {
            response = HttpUtils.excutePost(serviceUrl + method, map);//POST请求
            JsonNode jsonNode = JsonUtil.toNode(response);
            if (jsonNode != null &&jsonNode.has("Header"))
            {
                JsonNode dataNode = jsonNode.get("Body").get("data");
                if(dataNode == null)
                {
                    return null;
                }
                Iterator<JsonNode> list = dataNode.iterator();
                while(list.hasNext()) {
                    JsonNode element = list.next();
                    String ID_NO = JsonNodeUtil.getString(element, "ID_NO");
                    String ITNAME = JsonNodeUtil.getString(element, "ITNAME");
                    String KIND = JsonNodeUtil.getString(element, "KIND");
                    String DS_TYPE = JsonNodeUtil.getString(element, "DS_TYPE");
                    String QTY = JsonNodeUtil.getString(element, "QTY");
                    String DS_NAME = JsonNodeUtil.getString(element, "DS_NAME");
                    String OG_SNO = JsonNodeUtil.getString(element, "OG_SNO");


                    ProductData productDetail = new ProductData();
                    productDetail.setID_NO(ID_NO);
                    productDetail.setITNAME(ITNAME);
                    productDetail.setKIND(KIND);
                    productDetail.setDS_TYPE(DS_TYPE);
                    productDetail.setQTY(QTY);
                    productDetail.setDS_NAME(DS_NAME);
                    productDetail.setOG_SNO(OG_SNO);
                    productDatas.add(productDetail);//保存到List
                }
            }
            else
            {
                LOGGER.error("查询ERP商品属性失败,数据为空");
                return null;
            }
        } catch (Exception e) {
            LOGGER.error("查询ERP商品属性失败", e);
            return null;
        }
        return productDatas;//返回处理后的结构

    }
}
