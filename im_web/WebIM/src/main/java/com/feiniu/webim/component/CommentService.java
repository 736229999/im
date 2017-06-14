package com.feiniu.webim.component;

import com.feiniu.webim.config.ConfigManager;
import com.feiniu.webim.exception.HttpGetException;
import com.feiniu.webim.util.json.JSONUtil;
import com.feiniu.webim.util.web.HttpClientUtils;
import org.apache.log4j.Logger;

import java.util.*;

/**
 * Created by wangchen on 2015/2/28.
 */
public class CommentService {
    private static final String COMMENT_SERVICE_URL = ConfigManager.getProperty("service_comment");
    public static final CommentService instance = new CommentService();

    private static final Logger LOGGER = Logger.getLogger(CommentService.class);

    public List<Map> getScoreInfo(long merchantID) {
        Map<String, Object> params = new HashMap<>();
        params.put("merchantId", merchantID);

        try {
            String response =
                    HttpClientUtils.executeGet(COMMENT_SERVICE_URL, "getStoreInfoVo.html", params);

            return JSONUtil.toList(response, Map.class);
        } catch (HttpGetException e) {
            LOGGER.error("店铺评价-获取店铺评分发生错误");
            throw new RuntimeException(e);
        }
    }

    public Integer getCommentCount(long goodsID) {
        Map<String, Object> params = new HashMap<>();
        params.put("goods_id", goodsID);

        try {
            String response =
                    HttpClientUtils.executeGet(COMMENT_SERVICE_URL, "getProductCommentVo.html", params);

            List<Map> list = JSONUtil.toList(response, Map.class);

            Integer goodCommentCount = Integer.valueOf(list.get(0).get("goodCommentCount").toString());
            Integer normalCommentCount = Integer.valueOf(list.get(0).get("normalCommentCount").toString());
            Integer badCommentCount = Integer.valueOf(list.get(0).get("badCommentCount").toString());

            return goodCommentCount + normalCommentCount + badCommentCount;
        } catch (HttpGetException e) {
            LOGGER.error("店铺评价-获取店铺评价数量发生错误");
            throw new RuntimeException(e);
        }
    }
}
