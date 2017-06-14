package com.feiniu.csWebIm.service.external;

import com.feiniu.csWebIm.bean.domain.AfterSaleBoardConfig;
import com.feiniu.csWebIm.bean.domain.KefuIndexConfig;
import com.feiniu.csWebIm.bean.domain.TouchBoardConfig;
import com.feiniu.csWebIm.consts.BusinessConfig;
import com.feiniu.csWebIm.exception.BusinessException;
import com.feiniu.csWebIm.utils.json.JsonNodeUtil;
import com.feiniu.csWebIm.utils.json.JsonUtil;
import com.feiniu.csWebIm.utils.web.HttpClientUtils;
import com.feiniu.csWebIm.utils.web.HttpUtils;
import org.codehaus.jackson.JsonNode;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import javax.annotation.PostConstruct;
import java.io.IOException;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Author: morningking
 * Date: 2015/11/24
 * Contact: 243717042@qq.com
 */
@Service
public class KefuBackendService {
    @Autowired
    private BusinessConfig businessConfig;

    private String homePageConfigURL;
    private String updateFAQCountURL;
    private String getEmbedSetURL;
    private String getTouchBoardConfigListURL;
    private static final Logger LOGGER = LoggerFactory.getLogger(KefuBackendService.class);

    @PostConstruct
    private void init() {
        homePageConfigURL = businessConfig.getKefuBackendURL() + "get-home-page-config.do";
        updateFAQCountURL = businessConfig.getKefuBackendURL() + "/faq/update-browse-info.do";
        getEmbedSetURL = businessConfig.getKefuBackendURL() + "get-embed-related-group.do";
        getTouchBoardConfigListURL = businessConfig.getKefuBackendURL() + "get-touch-board-config-list.do";
    }

    public Map<String, Object> getHomePageConfig() {
        try {
            String response = HttpUtils.excutePost(homePageConfigURL, Collections.<String, Object>emptyMap());

            JsonNode result = JsonUtil.toNode(response);
            if (result != null && result.has("success") && result.get("success").getBooleanValue()) {
                JsonNode dataNode = result.get("data");
                Map<String, Object> resultMap = new HashMap<>(dataNode.size());

                resultMap.put("kefuIndexConfig",
                        JsonUtil.fromJson(JsonNodeUtil.getString(dataNode, "kefuIndexConfig"), KefuIndexConfig.class));
                resultMap.put("afterSaleBoardConfigList",
                        JsonUtil.toList(JsonNodeUtil.getString(dataNode, "afterSaleBoardList"), AfterSaleBoardConfig.class));

                return resultMap;
            } else {
                throw new BusinessException("获取客服首页数据出错：" + response);
            }
        } catch (IOException ex) {
            LOGGER.error("获取客服首页配置出错", ex);
            throw new RuntimeException(ex);
        }
    }

    public boolean updateFAQCount(String questionId) {
        try {
            if (Integer.valueOf(questionId) < 0)
                return false;

            Map<String, Object> map = new HashMap<String, Object>();
            map.put("faqID", questionId);

            String response = HttpUtils.excutePost(updateFAQCountURL, map);
            JsonNode result = JsonUtil.toNode(response);
            if (result != null && result.has("success") && result.get("success").getBooleanValue()) {
                return true;
            } else {
                throw new BusinessException("更新常见问题读取次数错误：" + response);
            }
        } catch (IOException ex) {
            LOGGER.error("更新常见问题读取次数错误：", ex);
            throw new RuntimeException(ex);
        }
    }

    public Map<String, Integer> getEmbedToGroupMap() {
        try {
            String response = HttpUtils.excutePost(getEmbedSetURL, Collections.<String, Object>emptyMap());

            JsonNode result = JsonUtil.toNode(response);
            if (result != null && result.has("success") && result.get("success").getBooleanValue()) {
                JsonNode dataNode = result.get("data");

                return JsonUtil.toMap(JsonNodeUtil.getString(dataNode, "embedSet"));
            } else {
                throw new BusinessException("获取客服首页数据出错：" + response);
            }
        } catch (IOException ex) {
            LOGGER.error("获取客服首页配置出错", ex);
            throw new RuntimeException(ex);
        }
    }

    public List<TouchBoardConfig> getTouchBoardList() {
        try {
            String response = HttpClientUtils.excutePost(getTouchBoardConfigListURL, Collections.<String, Object>emptyMap());
            JsonNode jsonNode = JsonUtil.toNode(response);

            if (jsonNode != null
                    && jsonNode.has("success") && jsonNode.get("success").asBoolean()
                    && jsonNode.has("data")
                    && jsonNode.get("data").has("list")) {
                return JsonUtil.toList(jsonNode.get("data").get("list").toString(), TouchBoardConfig.class);
            } else {
                throw new BusinessException("客服管理-获取触屏portal页版块设置失败" + response);
            }
        } catch (IOException e) {
            LOGGER.error("客服管理-获取触屏portal页版块设置 发生网络错误");
            throw new RuntimeException(e);
        }
    }
}
