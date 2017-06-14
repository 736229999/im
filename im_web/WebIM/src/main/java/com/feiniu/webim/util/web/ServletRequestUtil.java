package com.feiniu.webim.util.web;

import com.fasterxml.jackson.databind.JsonNode;
import com.feiniu.webim.bean.response.HistoryMessageListResponseBean;
import com.feiniu.webim.exception.ServletParamExtractException;
import com.feiniu.webim.util.json.JSONUtil;
import com.feiniu.webim.util.json.JsonNodeUtil;
import org.apache.commons.collections4.MapUtils;
import org.apache.commons.lang.ArrayUtils;
import org.apache.commons.lang.StringUtils;

import javax.servlet.http.HttpServletRequest;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by wangchen on 2014/12/12.
 */
public class ServletRequestUtil {
    public static String getString(HttpServletRequest request, String propertyName) {
        return getString(request, propertyName, null);
    }

    public static int getInteger(HttpServletRequest request, String propertyName) {
        String value = getString(request, propertyName);

        try {
            return Integer.parseInt(value);
        } catch (RuntimeException e) {
            throw new ServletParamExtractException(getErrorMessage(propertyName, value));
        }
    }

    public static double getDouble(HttpServletRequest request, String propertyName) {
        String value = getString(request, propertyName);

        try {
            return Double.parseDouble(value);
        } catch (RuntimeException e) {
            throw new ServletParamExtractException(getErrorMessage(propertyName, value));
        }
    }

    public static long getLong(HttpServletRequest request, String propertyName) {
        String value = getString(request, propertyName);

        try {
            return Long.parseLong(value);
        } catch (RuntimeException e) {
            throw new ServletParamExtractException(getErrorMessage(propertyName, value));
        }
    }

    public static String getString(HttpServletRequest request, String propertyName, String defaultValue) {
        String value = request.getParameter(propertyName);

        if (value == null) {
            return defaultValue;
        } else {
            return value;
        }
    }

    public static int getInteger(HttpServletRequest request, String propertyName, int defalutValue) {
        String stringValue = getString(request, propertyName);

        if (stringValue == null) {
            return defalutValue;
        } else {
            return getInteger(request, propertyName);
        }
    }

    public static double getDouble(HttpServletRequest request, String propertyName, double defaultValue) {
        String stringValue = getString(request, propertyName);

        if (stringValue == null) {
            return defaultValue;
        } else {
            return getDouble(request, propertyName);
        }
    }

    public static long getLong(HttpServletRequest request, String propertyName, long defaultValue) {
        String stringValue = getString(request, propertyName);

        if (StringUtils.isEmpty(stringValue)) {
            return defaultValue;
        } else {
            return getLong(request, propertyName);
        }
    }

    public static <T> T getObject(HttpServletRequest request, Class<T> clazz) {
        String stringValue = "";
        Map<String, String> paramsMap = new HashMap<>();

        if (MapUtils.isNotEmpty(request.getParameterMap())) {
            for (Map.Entry<String, String[]> entry : request.getParameterMap().entrySet()) {
                if (ArrayUtils.isNotEmpty(entry.getValue())) {
                    paramsMap.put(entry.getKey(), StringUtils.join(entry.getValue(), ","));
                }
            }
        }

        stringValue = JSONUtil.toJsonString(paramsMap);

        try {
            T value = clazz.newInstance();

            return webRequestParseBean(stringValue, clazz, value);
        } catch (ReflectiveOperationException e) {
            throw new RuntimeException(e);
        } catch (RuntimeException e) {
            throw e;
        }
    }

    public static <T> T webRequestParseBean(String messageJson, Class<T> clazz, T object) {
        if (messageJson == null) {
            return null;
        } else {
            return webRequestParseBean(messageJson, Arrays.asList(clazz.getDeclaredFields()), object);
        }
    }

    private static <T> T webRequestParseBean(String messageJson, List<Field> fieldList, T object) {
        JsonNode jsonNode = JSONUtil.toNode(messageJson);
        String fieldName = "";
        String nodeValue = "";

        try {
            for (Field field : fieldList) {
                field.setAccessible(true);
                String fieldType = field.getGenericType().toString();
                fieldName = field.getName();
                nodeValue = JsonNodeUtil.getString(jsonNode, fieldName);

                if ("class java.lang.Boolean".equals(fieldType)) {
                    if (StringUtils.isNotEmpty(nodeValue)) {
                        field.set(object, Boolean.valueOf(nodeValue));
                    }
                } else if ("boolean".equals(fieldType)) {
                    if (StringUtils.isEmpty(nodeValue)) {
                        throw new IllegalArgumentException("Field: " + fieldName + " is null");
                    } else {
                        field.set(object, Boolean.parseBoolean(nodeValue));
                    }
                } else if ("class java.lang.Byte".equals(fieldType)) {
                    if (StringUtils.isNotEmpty(nodeValue)) {
                        field.set(object, Byte.valueOf(nodeValue));
                    }
                } else if ("byte".equals(fieldType)) {
                    if (StringUtils.isEmpty(nodeValue)) {
                        throw new IllegalArgumentException("Field: " + fieldName + " is null");
                    } else {
                        field.set(object, Byte.parseByte(nodeValue));
                    }
                } else if ("class java.lang.Character".equals(fieldType)) {
                    if (StringUtils.isNotEmpty(nodeValue)) {
                        field.set(object, nodeValue.charAt(0));
                    }
                } else if ("char".equals(fieldType)) {
                    if (StringUtils.isEmpty(nodeValue)) {
                        throw new IllegalArgumentException("Field: " + fieldName + " is null");
                    } else {
                        field.set(object, nodeValue.charAt(0));
                    }
                } else if ("class java.lang.Integer".equals(fieldType)) {
                    if (StringUtils.isNotEmpty(nodeValue)) {
                        field.set(object, Integer.valueOf(nodeValue));
                    }
                } else if ("int".equals(fieldType)) {
                    if (StringUtils.isNotEmpty(nodeValue)) {
                        field.setInt(object, Integer.parseInt(nodeValue));
                    } else {
                        throw new IllegalArgumentException("Field: " + fieldName + " is null");
                    }
                } else if ("class java.lang.Long".equals(fieldType)) {
                    if (StringUtils.isNotEmpty(nodeValue)) {
                        field.set(object, Long.valueOf(nodeValue));
                    }
                } else if ("long".equals(fieldType)) {
                    if (StringUtils.isNotEmpty(nodeValue)) {
                        field.setLong(object, Long.parseLong(nodeValue));
                    } else {
                        throw new IllegalArgumentException("Field: " + fieldName + " is null");
                    }
                } else if ("class java.lang.Double".equals(fieldType)) {
                    if (StringUtils.isNotEmpty(nodeValue)) {
                        field.set(object, Double.valueOf(nodeValue));
                    }
                } else if ("double".equals(fieldType)) {
                    if (StringUtils.isNotEmpty(nodeValue)) {
                        field.setDouble(object, Double.parseDouble(nodeValue));
                    } else {
                        throw new IllegalArgumentException("Field: " + fieldName + " is null");
                    }
                } else if ("class java.lang.String".equals(fieldType)) {
                    field.set(object, nodeValue);
                }
            }
        } catch (ReflectiveOperationException e) {
            throw new RuntimeException(e);
        } catch (RuntimeException e) {
            throw new ServletParamExtractException(getErrorMessage(fieldName, nodeValue));
        }

        return object;
    }

    private static final String getErrorMessage(String properName, String value) {
        return "解析字段'" + properName + "'失败，系统接受到的值为：[" + value + "]";
    }

    public static void main(String[] args) {
        Method[] methods = HistoryMessageListResponseBean.class.getMethods();

        for (Method method : methods) {
            System.out.println(method.getName());
            System.out.print(method.getGenericReturnType() + ";");
            Type type = method.getGenericReturnType();

            if (type instanceof ParameterizedType) {
                Class<?> clazz = (Class<?>) ((ParameterizedType) type).getActualTypeArguments()[0];
                System.out.println(clazz);
            }
        }
    }
}
