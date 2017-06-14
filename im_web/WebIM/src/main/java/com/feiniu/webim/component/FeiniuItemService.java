package com.feiniu.webim.component;

import com.fasterxml.jackson.databind.JsonNode;
import com.feiniu.webim.bean.domain.FeiniuItemInfo;
import com.feiniu.webim.config.ConfigManager;
import com.feiniu.webim.exception.BusinessException;
import com.feiniu.webim.util.json.JSONUtil;
import com.feiniu.webim.util.json.JsonNodeUtil;
import com.feiniu.webim.util.web.HttpClientUtils;
import org.apache.log4j.Logger;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

/**
 * Author: morningking
 * Date: 2015/11/10
 * Contact: 243717042@qq.com
 * <p/>
 * 飞牛自营详情页相关服务
 */
public class FeiniuItemService {
    public static final FeiniuItemService instance = new FeiniuItemService();

    private FeiniuItemService() {
    }

    private static final Logger LOGGER = Logger.getLogger(FeiniuItemService.class);
    private static final String MEMBER_IMAGE_BASE_URL = ConfigManager.getProperty("member_image_base_url");
    private static final String FEINIU_ITEM_BASE_URL = ConfigManager.getProperty("service_feiniu_item") + "/commodityService/rest";
    private static final String GET_ITEM_SUMMARY_INFO = FEINIU_ITEM_BASE_URL + "/www/item/get_commodity_pool_for_sitemenu";

    //获取自营商品摘要信息
    public FeiniuItemInfo getItemInfo(String smSeq) {
        Map<String, Object> fields = new HashMap<>(1);
        fields.put("queryID", smSeq);
        fields.put("type", 2);

        Map<String, Object> params = new HashMap<>(1);
        params.put("data", JSONUtil.toJsonString(fields));

        try {
            String response = HttpClientUtils.excutePost(GET_ITEM_SUMMARY_INFO, params);
            JsonNode jsonNode = JSONUtil.toNode(response);

            if (jsonNode != null) {
                if ("1".equals(JsonNodeUtil.getString(jsonNode, "success"))) {
                    JsonNode resultNode = jsonNode.get("data").get("result");
                    JsonNode itemNode = resultNode.iterator().next();

                    FeiniuItemInfo feiniuItemInfo = JsonNodeUtil.injectBean(itemNode.toString(), FeiniuItemInfo.class);
                    feiniuItemInfo.setSmPic(MEMBER_IMAGE_BASE_URL + feiniuItemInfo.getSmPic());

                    return feiniuItemInfo;
                } else {
                    throw new BusinessException("商品-获取商品接口出错:" + JsonNodeUtil.getString(jsonNode, "msg"));
                }
            } else {
                throw new BusinessException("商品-获取商品接口返回错误数据：" + response);
            }
        } catch (IOException e) {
            LOGGER.error("商品-获取商品相关信息发生错误");
            throw new RuntimeException(e);
        }

    }
}
