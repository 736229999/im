package com.feiniu.webim.util.json;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.DeserializationFeature;
import com.fasterxml.jackson.databind.JavaType;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.deser.DeserializationProblemHandler;
import org.apache.log4j.Logger;

import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.*;

/**
 * Jackson类库的封装工具类，专门负责解析json数据</br>
 */
public class JSONUtil {
    private static final Logger logger = Logger.getLogger(JSONUtil.class);
	/**
	 * 默认的日期格式
	 */
	public static final String DEFAULT_DATE_PATTERN = "yyyy-MM-dd HH:mm:ss";

	/**
	 * 单例模式
	 */
	public static final ObjectMapper OBJECT_MAPPER = new ObjectMapper();

	/**
	 * 单例模式
	 */
	static {
		OBJECT_MAPPER.setDateFormat(new SimpleDateFormat(DEFAULT_DATE_PATTERN));
		// 设置输出时包含属性的风格
//		OBJECT_MAPPER.setSerializationInclusion(Include.NON_EMPTY);
//		// 设置输入时忽略在JSON字符串中存在但Java对象实际没有的属性
//		OBJECT_MAPPER.configure(
//				DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES, false);
		OBJECT_MAPPER.configure(DeserializationFeature.ACCEPT_SINGLE_VALUE_AS_ARRAY, true);
		OBJECT_MAPPER.configure(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES, false);

		OBJECT_MAPPER.addHandler(new DeserializationProblemHandler() {
		});
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

	public static Map toMap(Object bean) {
		return OBJECT_MAPPER.convertValue(bean, Map.class);
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

    public static <E> List<E> toListA(String json, Class<E> clazz) {
        List<E> list;

        try {
            list = OBJECT_MAPPER.readValue(json,
                    new TypeReference<List<E>>() {});
            if (list == null) {
                list = new ArrayList();
            }
        } catch (Exception e) {
            throw new RuntimeException(e);
        }

        return list;
    }

    public static <E> Set<E> toSet(String json, Class<E> clazz) {
        HashSet<E> list;

        try {
            list = OBJECT_MAPPER.readValue(json,
                    OBJECT_MAPPER.getTypeFactory().constructCollectionType(
                            HashSet.class, clazz));

            if (list == null) {
                list = new HashSet<E>();
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
			logger.error("Parse json-string:" + json + " to jsonObject error.");
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

    public static void main(String[] args) {
        String json = "{\"message\":null,\"success\":true,\"data\":{\"subAccountAndGroupMap\":{\"81\":[10078,10079,10080,10083,20029],\"62\":[10041,10043,10044]}}}";
        json = "[{\"label\":\"产品描述与实物相符\",\"score\":4.0,\"offset\":-20.0},{\"label\":\"卖家服务态度很好\",\"score\":5.0,\"offset\":-60.0},{\"label\":\"飞牛物流速度很快\",\"score\":4.3,\"offset\":14.0}]";

        System.out.println(toList(json, Map.class));
    }
}