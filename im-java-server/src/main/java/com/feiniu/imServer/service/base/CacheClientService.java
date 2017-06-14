package com.feiniu.imServer.service.base;

import com.feiniu.imServer.consts.ShuntConfig;
import com.feiniu.imServer.util.BeanToMapUtil;
import com.feiniu.imServer.util.JsonUtil;
import com.fn.cache.client.RedisCacheClient;
import org.apache.commons.lang.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import javax.annotation.PostConstruct;
import java.io.IOException;
import java.lang.reflect.Method;
import java.util.List;
import java.util.Map;

/**
 * Author: morningking
 * Date: 2015/11/20
 * Contact: 243717042@qq.com
 */
@Service
public class CacheClientService {
    private static final Logger LOGGER = LoggerFactory.getLogger(CacheClientService.class);
    @Autowired
    private RedisCacheClient redisCacheClient;

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
        if (StringUtils.isNotEmpty(key)) {
            return get(key);
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
            redisCacheClient.put(key, result);
        } catch (RuntimeException e) {
            LOGGER.error("往缓存存放信息失败", e);
        }
    }

    public boolean tryLock(String key, int expired, long timeOut) {
        long nano = System.currentTimeMillis();
        String lock = key + ShuntConfig.LOCK_NAME;
        try {
            do {
                LOGGER.debug("try lock key: " + lock);

                boolean success = acquireLock(lock, expired);

                if (success)
                    return true;

                Thread.sleep(300);

            } while ((System.currentTimeMillis() - nano) < timeOut);
        } catch (Exception e) {
            LOGGER.error(" lock key error: " + lock);
            e.printStackTrace();
        }
        return false;
    }

    public boolean acquireLock(String key, int expired) {
        boolean success = false;
        long value = System.currentTimeMillis() + expired + 1;
        String lock = key;

        long acquired = redisCacheClient.setnx(lock, String.valueOf(value));
        if (acquired == 1)
            success = true;

        else {
            String oldValueStr = redisCacheClient.get(lock);
            //如果锁在110行与115行运行的时候  锁被删除了，返回false
            if (oldValueStr == null)
                return false;

            long oldValue = Long.valueOf(oldValueStr);
            //超时
            if (oldValue < System.currentTimeMillis()) {
                String getValue = redisCacheClient.getSet(lock, String.valueOf(value));
                // 获取锁成功
                if (Long.valueOf(getValue) == oldValue)
                    success = true;
                    // 已被其他进程捷足先登了
                else
                    success = false;
            }
            //未超时，则直接返回失败
            else
                success = false;
        }
        return success;
    }

    //释放锁
    public void releaseLock(String key) {
        String lock = key + ShuntConfig.LOCK_NAME;
        long current = System.currentTimeMillis();
        // 避免删除非自己获取得到的锁
        if (current < Long.valueOf(redisCacheClient.get(lock)))
            redisCacheClient.del(lock);
    }

    public String lIndex(String key, int index) {
        try {
            return redisCacheClient.lindex(key, index);
        } catch (RuntimeException e) {
            LOGGER.error("在列表中删除数据失败:" + key, e);
            throw e;
        }
    }

    public void lRemove(String key) {
        try {
            redisCacheClient.lpop(key);
        } catch (RuntimeException e) {
            LOGGER.error("在列表中删除数据失败:" + key, e);
            throw e;
        }
    }

    public void rPush(String key, Object value) {
        try {
            String result = (value == null ? null : JsonUtil.toJsonString(value));
            redisCacheClient.rpush(key, result);
        } catch (RuntimeException e) {
            LOGGER.error("往列表中Push数据失败:" + key, e);
            throw e;
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

    private Map<String, String> getH(String key) {
        try {
            Map<String, String> values = redisCacheClient.hgetAll(key);
            return values == null ? null : values;
        } catch (RuntimeException e) {
            LOGGER.error("获取缓存信息失败:Map :" + key, e);
            throw e;
        }
    }

    public <E> E getHValue(String key, Class<E> clazz) throws Exception {
        if (StringUtils.isNotEmpty(key) && clazz != null) {
            Map<String, String> serializedMap = getH(key);

            try {
                return BeanToMapUtil.convertMap(clazz, serializedMap);
            } catch (Exception e) {
                LOGGER.error("redis hashMap 转 JAVA 对象失败");
                e.printStackTrace();
                throw e;
            }
        }
        return null;
    }

    public <E> E putHValue(String key, Object object) throws Exception {
        if (StringUtils.isNotEmpty(key) && object != null) {
            Method[] methods = object.getClass().getMethods();
            for (Method method : methods) {
                String methodName = method.getName();
                if (methodName.startsWith("get")) {
                    String fieldName = methodName.substring(3, 4).toLowerCase() + methodName.substring(4);
                    redisCacheClient.hput(key,fieldName,method.invoke(object).toString());
                }
            }
        }
        return null;
    }


    public String getHFieldValue(String key, String field) throws Exception {
        if (StringUtils.isNotEmpty(key) && field != null) {
            try {
                String values = redisCacheClient.hget(key, field);
                return values == null ? null : values;
            } catch (RuntimeException e) {
                LOGGER.error("获取缓存信息失败:Map :" + key + "filed: " + field, e);
                throw e;
            }
        }
        return null;
    }

}
