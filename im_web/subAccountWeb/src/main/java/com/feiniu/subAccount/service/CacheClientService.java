package com.feiniu.subAccount.service;

import com.feiniu.subAccount.util.JsonUtil;
import com.fn.cache.client.RedisCacheClient;
import org.apache.commons.lang.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import javax.annotation.PostConstruct;
import java.io.IOException;
import java.util.List;

/**
 * Author: morningking
 * Date: 2015/11/20
 * Contact: 243717042@qq.com
 */
@Service
public class CacheClientService {
    @Autowired
    private RedisCacheClient redisCacheClient;

    //存活时间: 半小时
    private static final int TO_LIVE_TIME = 60 * 30;

    private static final Logger LOGGER = LoggerFactory.getLogger(CacheClientService.class);

    @PostConstruct
    private void init() throws IOException {

    }

    public <E> List<E> getListValue(String key, Class<E> clazz) {
        if (StringUtils.isNotEmpty(key) && clazz != null) {
            String serializedObjString = get(key);
            if (StringUtils.isNotEmpty(serializedObjString)) {
                return JsonUtil.toList(serializedObjString, clazz);
            }
        }

        return null;
    }

    public <E> E getValue(String key, Class<E> clazz) {
        if (StringUtils.isNotEmpty(key) && clazz != null) {

            String serializedObjString = get(key);
            if (StringUtils.isNotEmpty(serializedObjString)) {
                return JsonUtil.fromJson(serializedObjString, clazz);
            }
        }

        return null;
    }

    public String getValueAsString(String key) {
        if (StringUtils.isNotEmpty(key) ) {
            return  get(key);
        }
        return null;
    }

    public void delete(String key) {
        try {
            redisCacheClient.remove(key);
            LOGGER.debug("删除键值成功：" + key);
        } catch (RuntimeException e) {
            LOGGER.error("删除键值失败", e);
        }
    }

    public void put(String key, Object value) {
        try {
            String result = (value == null ? null : JsonUtil.toJsonString(value));
            redisCacheClient.put(key,result);
        } catch (RuntimeException e) {
            LOGGER.error("往缓存存放信息失败", e);
        }
    }

    private String get(String key) {
        try {
            Object value = redisCacheClient.get(key);
            return value == null ? null : value.toString();
        } catch (RuntimeException e) {
            LOGGER.error("获取缓存信息失败", e);
        }

        return null;
    }

}
