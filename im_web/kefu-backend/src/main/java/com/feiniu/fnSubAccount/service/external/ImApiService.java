package com.feiniu.fnSubAccount.service.external;

import com.feiniu.fnSubAccount.bean.ImApi.CSMonitorInfoNew;
import com.feiniu.fnSubAccount.constants.BusinessConfig;
import com.feiniu.fnSubAccount.constants.Constants;
import com.feiniu.fnSubAccount.exception.BusinessException;
import com.feiniu.fnSubAccount.util.json.JsonNodeUtil;
import com.feiniu.fnSubAccount.util.web.HttpUtils;
import org.codehaus.jackson.JsonNode;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.*;

/**
 * Created by wangzhen on 2016/2/17.
 */
@SuppressWarnings({ "unused","unchecked", "rawtypes" })
@Service
public class ImApiService {
    @Autowired
    private BusinessConfig businessConfig;
    private static final Logger LOGGER = LoggerFactory.getLogger(ImApiService.class);

    public CSMonitorInfoNew getMonitorInfo(String cServiceId){
        if (!businessConfig.isUseImApi())
            return  null;

        String response = Constants.NIL_RESPONSE;
        CSMonitorInfoNew csMonitorInfoNew = null;
        Map<String, Object> map = new HashMap<String, Object>();
        map.put("cServiceId", cServiceId);
        try {
            response = HttpUtils.executeGet(businessConfig.getImAPLURL(),
                    "/userInfo/get-cs-monitor-info", map);
            JsonNode jsonNode = com.feiniu.fnSubAccount.util.JsonUtil.toNode(response);

            if (jsonNode != null && jsonNode.has("data") && JsonNodeUtil.getBoolean(jsonNode, "success")) {
                csMonitorInfoNew = JsonNodeUtil.injectBean(JsonNodeUtil.getString(jsonNode, "datas"), CSMonitorInfoNew.class);
            } else {
                throw new BusinessException("im-api:获取自营客服信息失败");
            }
        } catch (Exception e) {
            LOGGER.error("im-api:获取自营客服信息失败", e);
        }
        return csMonitorInfoNew;
    }

    public List<CSMonitorInfoNew> getMonitorInfoList(String cServiceIds){
        if (!businessConfig.isUseImApi())
            return  null;

        String response = Constants.NIL_RESPONSE;
        Map<String, Object> map = new HashMap<String, Object>();
        map.put("cServiceIds", cServiceIds);
        try {
            List<CSMonitorInfoNew> csMonitorInfoNewList = new ArrayList<>();
            response = HttpUtils.executeGet(businessConfig.getImAPLURL(),
                    "/userInfo/get-cs-monitor-info-list", map);
            JsonNode jsonNode = com.feiniu.fnSubAccount.util.JsonUtil.toNode(response);

            if (jsonNode != null && jsonNode.has("data") && JsonNodeUtil.getBoolean(jsonNode, "success")) {

                JsonNode bodyNode = jsonNode.get("data").get("csMonitorInfoList");

                Iterator<JsonNode> iterator = bodyNode.iterator();
                while (iterator.hasNext()) {
                    JsonNode currentProductNode = iterator.next();
                    csMonitorInfoNewList.add(JsonNodeUtil.injectBean(currentProductNode.toString(), CSMonitorInfoNew.class));
                }
                return csMonitorInfoNewList;
            } else {
                LOGGER.error("im-api:获取自营客服信息失败,数据为空");
                return csMonitorInfoNewList;
            }
        } catch (Exception e) {
            LOGGER.error("im-api:获取自营客服信息失败", e);
        }
        return null;
    }

    public void  modifyCsGroupMemInRedis(Long groupId,String CSIds){
        if (!businessConfig.isUseImApi())
            return  ;
        String response = Constants.NIL_RESPONSE;
        CSMonitorInfoNew csMonitorInfoNew = null;
        Map<String, Object> map = new HashMap<String, Object>();
        map.put("groupId", groupId);
        map.put("CSIds",CSIds);
        try {
            response = HttpUtils.executeGet(businessConfig.getImAPLURL(),
                    "/userInfo/modify-cs-group-member", map);
            JsonNode jsonNode = com.feiniu.fnSubAccount.util.JsonUtil.toNode(response);

            if (jsonNode != null  && JsonNodeUtil.getBoolean(jsonNode, "success")) {
                return;
            } else {
                throw new BusinessException("im-api:缓存更新组成员失败");
            }
        } catch (Exception e) {
            LOGGER.error("im-ap:缓存更新失败", e);
        }
        return ;
    }

    public void  deleteGroupInRedis(Long groupId){
        if (!businessConfig.isUseImApi())
            return  ;
        String response = Constants.NIL_RESPONSE;
        CSMonitorInfoNew csMonitorInfoNew = null;
        Map<String, Object> map = new HashMap<String, Object>();
        map.put("groupId", groupId);
        try {
            response = HttpUtils.executeGet(businessConfig.getImAPLURL(),
                    "/userInfo/delete-cs-group", map);
            JsonNode jsonNode = com.feiniu.fnSubAccount.util.JsonUtil.toNode(response);

            if (jsonNode != null  && JsonNodeUtil.getBoolean(jsonNode, "success")) {
                return;
            } else {
                throw new BusinessException("im-api:缓存删除组失败");
            }
        } catch (Exception e) {
            LOGGER.error("im-api:缓存删除组失败", e);
        }
        return ;
    }

    public void  modifyReceptionLimitInRedis(Long id ,Integer receptionLimit){
        if (!businessConfig.isUseImApi())
            return  ;
        String response = Constants.NIL_RESPONSE;
        CSMonitorInfoNew csMonitorInfoNew = null;
        Map<String, Object> map = new HashMap<String, Object>();
        map.put("id", id);
        map.put("receptionLimit",receptionLimit);
        try {
            response = HttpUtils.executeGet(businessConfig.getImAPLURL(),
                    "/userInfo/modify-cs-reception-limit", map);
            JsonNode jsonNode = com.feiniu.fnSubAccount.util.JsonUtil.toNode(response);

            if (jsonNode != null  && JsonNodeUtil.getBoolean(jsonNode, "success")) {
                return;
            } else {
                throw new BusinessException("im-api:缓存更新接待上限失败");
            }
        } catch (Exception e) {
            LOGGER.error("im-api:缓存更新接待上限失败", e);
        }
        return ;
    }

}
