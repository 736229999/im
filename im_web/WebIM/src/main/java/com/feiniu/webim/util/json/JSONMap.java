/**
 * 
 */
package com.feiniu.webim.util.json;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Map;

/** 
 * @ClassName: JSONMap 
 * @Description: jsonMap 方便获取各种类型属性 
 * @author Lucian
 * @date 2014年10月11日 下午3:52:43 
 *  
 */
public class JSONMap {
	public static final String DEFAULT_DATE_PATTERN = "yyyy-MM-dd HH:mm:ss";
	private String jsonString;
	private Map jsonMap;
	private SimpleDateFormat simpleDateFormat = null;

	public JSONMap(String jsonString) {
		this.jsonString = jsonString;
		simpleDateFormat = new SimpleDateFormat(DEFAULT_DATE_PATTERN);
		jsonMap = JSONUtil.toMap(jsonString);
	}
	
	/**
	 * 获取属性
	 * @param key
	 * @param clazz
	 * @param defaultValue
	 * @return
	 */
	public <T> T get(String key, Class<T> clazz, Object defaultValue)  {
		T value = null;
		if(jsonMap != null && jsonMap.size() > 0) {
			Object valueObj = jsonMap.get(key);
			if(valueObj == null) { //给默认值
				if(defaultValue != null) {
					valueObj = defaultValue;
				} else {
					return null;
				}
			}
			
			if(clazz == Integer.class) {
				value = (T) Integer.valueOf(valueObj.toString());
			} else if(clazz == Long.class) {
				value = (T) Long.valueOf(valueObj.toString());
			} else if(clazz == String.class) {
				value = (T) valueObj.toString();
			} else if(clazz == Double.class) {
				value = (T) Double.valueOf(valueObj.toString());
			} else if(clazz == Float.class) {
				value = (T) Float.valueOf(valueObj.toString());
			} else if(clazz == Short.class) {
				value = (T) Short.valueOf(valueObj.toString());
			} else if(clazz == Boolean.class) {
				value = (T) Boolean.valueOf(valueObj.toString());
			} else if(clazz == Date.class) {
				try {
					value = (T) simpleDateFormat.parse(valueObj.toString());
				} catch (ParseException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			} else {
				value = (T) valueObj;
			}
		}
		return value;
	}
	
	/**
	 *  获取属性
	 * @param key
	 * @param clazz
	 * @return
	 */
	public <T> T get(String key, Class<T> clazz) {
		return get(key, clazz, null);
	}

	public String getJsonString() {
		return jsonString;
	}

	public void setJsonString(String jsonString) {
		this.jsonString = jsonString;
	}
	
	
}
