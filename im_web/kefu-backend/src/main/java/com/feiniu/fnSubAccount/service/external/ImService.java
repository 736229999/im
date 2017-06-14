package com.feiniu.fnSubAccount.service.external;

import com.feiniu.fnSubAccount.bean.CSAccountQueryReply;
import com.feiniu.fnSubAccount.bean.request.MonitorInterveneSessionRequest;
import com.feiniu.fnSubAccount.constants.BusinessConfig;
import com.feiniu.fnSubAccount.exception.BusinessException;
import com.feiniu.fnSubAccount.util.json.JsonNodeUtil;
import com.feiniu.fnSubAccount.util.json.JsonUtil;
import com.feiniu.fnSubAccount.util.web.HttpUtils;
import org.codehaus.jackson.JsonNode;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.*;

import static com.feiniu.fnSubAccount.util.json.JsonNodeUtil.injectBean;

/**
 * Created by wangzhen on 2015/11/17.
 */
@Service
public class ImService {
    @Autowired
    private BusinessConfig businessConfig;
    private static final Logger LOGGER = LoggerFactory.getLogger(ImService.class);

    public boolean imCsInfo(Map<String, Object> map) {
        try {
            String response = HttpUtils.executeGet(businessConfig.getImServerIpURL(), "",
                    map);
            JsonNode jsonNode = JsonUtil.toNode(response);

            if (jsonNode != null) {
                if ("true".equals(JsonNodeUtil.getString(jsonNode, "result"))) {
                    return true;
                } else {
                    return false;
                }
            } else {
                throw new BusinessException("POST-批量增加用户返回数据异常");
            }
        } catch (Exception e) {
            LOGGER.error("POST-批量增加用户发生错误", e);
            throw new RuntimeException(e);
        }
    }

    public boolean modifyStatus(Map<String, Object> map) {
        try {
            String response = HttpUtils.executeGet(businessConfig.getImServerIpURL(), "",
                    map);
            JsonNode jsonNode = JsonUtil.toNode(response);

            if (jsonNode != null) {
                if ("true".equals(JsonNodeUtil.getString(jsonNode, "result"))) {
                    return true;
                } else {
                    return false;
                }
            } else {
                throw new BusinessException("POST-批量修改用户状态返回数据异常");
            }
        } catch (Exception e) {
            LOGGER.error("POST-批量修改用户状态发生错误", e);
            throw new RuntimeException(e);
        }
    }

    public List<CSAccountQueryReply> queryCSStatus(Map<String, Object> map) {
        try {
            CSAccountQueryReply csAccountQueryReply = null;
            List<CSAccountQueryReply> csAccountQueryReplyList = new ArrayList<>();
            String response = HttpUtils.executeGet(businessConfig.getImServerIpURL(), "",
                    map);
            JsonNode jsonNode = JsonUtil.toNode(response);

            if (jsonNode != null && jsonNode.has("result") && "true".equals(JsonNodeUtil.getString(jsonNode, "result")))
                if ("true".equals(JsonNodeUtil.getString(jsonNode, "result"))) {
                    Iterator<JsonNode> iterator = jsonNode.get("queryinfo").iterator();
                    while (iterator.hasNext()) {
                        csAccountQueryReply = injectBean(iterator.next().toString(), CSAccountQueryReply.class);
                        csAccountQueryReplyList.add(csAccountQueryReply);
                    }

                    return csAccountQueryReplyList;
                } else {
                    return Collections.emptyList(); // Always returns same list  ;
                }
            else {
                throw new BusinessException("POST-批量查询用户返回数据异常");
            }
        } catch (Exception e) {
            LOGGER.error("POST-批量查询用户发生错误", e);
            throw new RuntimeException(e);
        }
    }

    /**
     * 判断是否有其他客服主管插入了会话
     */
    public boolean checkIsOtherMonitorInInterveneProgress(MonitorInterveneSessionRequest request) {
        Map<String, Object> params = new HashMap<>(3);
        params.put("params", JsonUtil.toJsonString(request));
        params.put("version", "1.0");
        params.put("method", "getIsEnableInsertMsg");

        try {
            String response = HttpUtils.executeGet(businessConfig.getImServerIpURL(), "", params);
            JsonNode jsonNode = JsonUtil.toNode(response);

            if (jsonNode != null) {
                if (Boolean.TRUE.equals(JsonNodeUtil.getBoolean(jsonNode, "result"))) {
                    return false;
                } else {
                    return true;
                }
            }

            throw new BusinessException("获取 其他主管是否插入当前会话失败：" + JsonUtil.toJsonString(params));
        } catch (Exception e) {
            LOGGER.error("获取 其他主管是否插入当前会话失败", e);
            throw new RuntimeException(e);
        }
    }

    //主管可以使在线客服下线
    public boolean askCServiceOffline(Long csid) {
        try {
            Map<String, Object> params = new HashMap<>();
            params.put("csId", csid);
            Map<String, Object> map = new HashMap<>();
            map.put("params", JsonUtil.toJsonString(params));
            map.put("version", "1.0");
            map.put("method", "forceOfflineCs");
            String response = HttpUtils.executeGet(businessConfig.getImServerIpPath(), "",
                    map);
            JsonNode jsonNode = JsonUtil.toNode(response);

            if (jsonNode != null && jsonNode.has("result"))
                if ("true".equals(JsonNodeUtil.getString(jsonNode, "result"))) {

                    if ("true".equals(JsonNodeUtil.getString(jsonNode, "enable"))) {

                        return true;
                    } else {
                        LOGGER.error("result:true,enable:flase");
                        return false;
                    }

                } else {
                    LOGGER.error("result:false,error:");
                    return false;
                }
            else {
                throw new BusinessException("POST-批量查询用户返回数据异常");
            }
        } catch (Exception e) {
            LOGGER.error("POST-批量查询用户发生错误", e);
            throw new RuntimeException(e);
        }
    }
}
