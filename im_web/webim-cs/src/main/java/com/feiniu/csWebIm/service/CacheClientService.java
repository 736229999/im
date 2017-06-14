package com.feiniu.csWebIm.service;

import com.feiniu.csWebIm.utils.BeanToMapUtil;
import com.feiniu.csWebIm.utils.json.JsonUtil;
import com.fn.cache.client.CacheClient;
import org.apache.commons.lang.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import javax.annotation.PostConstruct;
import java.io.IOException;
import java.util.List;
import java.util.Map;

/**
 * Author: morningking
 * Date: 2015/11/20
 * Contact: 243717042@qq.com
 */
@Service
public class CacheClientService {
    @Autowired
    private CacheClient cacheClient;

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
            cacheClient.remove(key);
            LOGGER.debug("删除键值成功：" + key);
        } catch (RuntimeException e) {
            LOGGER.error("删除键值失败", e);
        }
    }

    public void put(String key, Object value) {
        try {
            String result = (value == null ? null : JsonUtil.toJsonString(value));
            cacheClient.put(key,result);
        } catch (RuntimeException e) {
            LOGGER.error("往缓存存放信息失败", e);
        }
    }

    private String get(String key) {
        try {
            Object value = cacheClient.get(key);
            return value == null ? null : value.toString();
        } catch (RuntimeException e) {
            LOGGER.error("获取缓存信息失败 :String "+key, e);
        }

        return null;
    }

    private Map<String, String> getH(String key) {
        try {
            Map<String, String> values = cacheClient.hgetAll(key);
            return values  == null ? null : values ;
        } catch (RuntimeException e) {
            LOGGER.error("获取缓存信息失败:Map :"+ key, e);
        }

        return null;
    }

    public <E> E getHValue(String key, Class<E> clazz) throws Exception{
        if (StringUtils.isNotEmpty(key) && clazz != null) {
            Map<String, String> serializedMap = getH(key);

            try {
                return  BeanToMapUtil.convertMap(clazz, serializedMap);
            }catch (Exception e){
                LOGGER.error("redis hashMap 转 JAVA 对象失败");
                e.printStackTrace();
                throw e;
            }
        }
        return null;
    }


    public String getHFieldValue(String key, String field) throws Exception{
        if (StringUtils.isNotEmpty(key) && field != null) {
            try {
                String values = cacheClient.hget(key,field);
                return values  == null ? null : values ;
            } catch (RuntimeException e) {
                LOGGER.error("获取缓存信息失败:Map :" +key + "filed: "+ field , e);
            }
        }
        return null;
    }
}