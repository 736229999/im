package com.feiniu.csWebIm.utils.json;

import org.codehaus.jackson.JsonNode;
import org.codehaus.jackson.JsonProcessingException;
import org.codehaus.jackson.map.DeserializationConfig;
import org.codehaus.jackson.map.ObjectMapper;
import org.codehaus.jackson.map.SerializationConfig;
import org.codehaus.jackson.map.annotate.JsonSerialize;
import org.codehaus.jackson.type.JavaType;
import org.codehaus.jackson.type.TypeReference;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by morningking on 2014/8/23.
 */
@SuppressWarnings({ "unused","unchecked", "rawtypes" })
public class JsonUtil {
    private static final Logger logger = LoggerFactory.getLogger(JsonUtil.class);
    /**
     * 默认的日期格式
     */
    public static final String DEFAULT_DATE_PATTERN = "yyyy-MM-dd HH:mm:ss";

    /**
     * 单例模式
     */
    public static final ObjectMapper OBJECT_MAPPER = new ObjectMapper();

    static {
        OBJECT_MAPPER.setDateFormat(new SimpleDateFormat(DEFAULT_DATE_PATTERN));
        OBJECT_MAPPER.enable(SerializationConfig.Feature.INDENT_OUTPUT);
        OBJECT_MAPPER.disable(DeserializationConfig.Feature.FAIL_ON_UNKNOWN_PROPERTIES);

        OBJECT_MAPPER.setSerializationInclusion(JsonSerialize.Inclusion.NON_NULL);
    }

    /**
     * 转换为json.
     *
     * @param object
     * @return
     */
    public static String toJsonString(Object object) {

        try {
            return OBJECT_MAPPER.writeValueAsString(object);
        } catch (IOException e) {
            logger.error("write to json string error:" + object, e);
            return null;
        }
    }

    /**
     * json转换map
     * @param jsonString
     * @return
     */
    public static Map toMap(String jsonString) {
        return fromJson(jsonString, Map.class);
    }

    public static Map<String,Object> toMap(Object object) {
        try {
            return OBJECT_MAPPER.readValue(toJsonString(object),
                    new TypeReference<HashMap>(){});
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    public static <E> List<E> toList(String json, Class<E> clazz) {
        List<E> list;

        try {
            list = OBJECT_MAPPER.readValue(json,
                    OBJECT_MAPPER.getTypeFactory().constructCollectionType(
                            ArrayList.class, clazz));

            if (list == null) {
                list = new ArrayList<E>();
            }
        } catch (Exception e) {
            throw new RuntimeException(e);
        }

        return list;
    }

    public static JsonNode toNode(String json) {
        try{
            return OBJECT_MAPPER.readValue(json, JsonNode.class);
        } catch( Exception e){
            logger.error("Parse json-string:" + json + " to jsonObject error.", e);
        }

        return null;
    }

    /**
     * json转换为对象.
     *
     * @param jsonString
     * @param clazz
     * @return
     */
    public static <T> T fromJson(String jsonString, Class<T> clazz) {
        if (jsonString == null || jsonString.trim().equals("")) {
            return null;
        }

        try {
            return OBJECT_MAPPER.readValue(jsonString, clazz);
        } catch (IOException e) {
            //logger.error("parse json string error:" + jsonString, e);
            return null;
        }
    }

    /**
     * 如果JSON字符串为Null或"null"字符串, 返回Null. 如果JSON字符串为"[]", 返回空集合.
     *
     * 如需读取集合如List/Map, 且不是List<String>这种简单类型时,先使用函數constructParametricType构造类型.
     *
     * @see #constructParametricType(Class, Class...)
     */
    public static <T> T fromJson(String jsonString, JavaType javaType) {
        if (jsonString == null || jsonString.trim().equals("")) {
            return null;
        }
        try {
            return (T) OBJECT_MAPPER.readValue(jsonString, javaType);
        } catch (IOException e) {
            logger.error("parse json string error:" + jsonString, e);
            return null;
        }
    }

    /**
     * 構造泛型的Type如List<MyBean>,
     * 则调用constructParametricType(ArrayList.class,MyBean.class)
     * Map<String,MyBean>则调用(HashMap.class,String.class, MyBean.class)
     */
    public static JavaType constructParametricType(Class<?> parametrized,
                                                   Class<?>... parameterClasses) {
        return OBJECT_MAPPER.getTypeFactory().constructParametricType(
                parametrized, parameterClasses);
    }

    /**
     * 當JSON裡只含有Bean的部分屬性時，更新一個已存在Bean，只覆蓋該部分的屬性.
     */
    public static <T> T update(T object, String jsonString) {
        try {
            return (T) OBJECT_MAPPER.readerForUpdating(object).readValue(
                    jsonString);
        } catch (JsonProcessingException e) {
            logger.error("update json string:" + jsonString + " to object:"
                    + object + " error.", e);
        } catch (IOException e) {
            logger.error("update json string:" + jsonString + " to object:"
                    + object + " error.", e);
        }
        return null;
    }

}
