package com.feiniu.csWebIm.utils.json;

import org.apache.commons.lang.StringUtils;
import org.codehaus.jackson.JsonNode;

import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.lang.reflect.ParameterizedType;
import java.util.Arrays;
import java.util.List;

/**
 * Created by wangchen on 2015/2/6.
 */
@SuppressWarnings({ "unused","unchecked", "rawtypes" })
public class JsonNodeUtil {
    public static Long getLong(JsonNode node, String property, Long defaultValue) {
        if (node.has(property)) {
            return node.get(property).asLong();
        } else {
            return defaultValue;
        }
    }

    public static Long getLong(JsonNode node, String property) {
        return getLong(node, property, null);
    }

    public static Integer getInteger(JsonNode node, String property, Integer defaultValue) {
        if (node.has(property)) {
            return node.get(property).asInt();
        } else {
            return defaultValue;
        }
    }

    public static Integer getInteger(JsonNode node, String property) {
        return getInteger(node, property, null);
    }

    public static Byte getByte(JsonNode node, String property, Byte defaultValue) {
        if (node.has(property)) {
            return (byte) node.get(property).asInt();
        } else {
            return defaultValue;
        }
    }

    public static Byte getByte(JsonNode node, String property) {
        return getByte(node, property, null);
    }

    public static Boolean getBoolean(JsonNode node, String property, Boolean defaultValue) {
        if (node.has(property)) {
            return node.get(property).asBoolean();
        } else {
            return defaultValue;
        }
    }

    public static Boolean getBoolean(JsonNode node, String property) {
        return getBoolean(node, property, null);
    }

    public static Double getDouble(JsonNode node, String property, Double defaultValue) {
        if (node.has(property)) {
            return node.get(property).asDouble();
        } else {
            return defaultValue;
        }
    }

    public static Double getDouble(JsonNode node, String property) {
        return getDouble(node, property, null);
    }

    public static String getString(JsonNode node, String property, String defaultValue) {
        if (node.has(property)) {
            if (node.get(property).asText().length() == 0) {
                return node.get(property).toString();
            } else {
                return node.get(property).asText();
            }
        } else {
            return defaultValue;
        }
    }

    public static String getString(JsonNode node, String property) {
        return getString(node, property, null);
    }

    private static <T> T parseBean(String messageJson, List<Field> fieldList, T object) {
        JsonNode jsonNode = JsonUtil.toNode(messageJson);

        try {
            for (Field field : fieldList) {
                field.setAccessible(true);
                String fieldType = field.getGenericType().toString();
                String fieldName = field.getName();


                if ("class java.lang.Boolean".equals(fieldType)) {
                    field.set(object, JsonNodeUtil.getBoolean(jsonNode, fieldName));
                }else if ("boolean".equals(fieldType)) {
                    field.setBoolean(object, JsonNodeUtil.getBoolean(jsonNode, fieldName));
                }else if ("class java.lang.Byte".equals(fieldType)) {
                    field.set(object, JsonNodeUtil.getByte(jsonNode, fieldName));
                }else if ("byte".equals(fieldType)) {
                    field.setByte(object, JsonNodeUtil.getByte(jsonNode, fieldName));
                } else if ("class java.lang.Character".equals(fieldType)) {
                    String nodeValue = JsonNodeUtil.getString(jsonNode, fieldName);
                    if (StringUtils.isNotEmpty(nodeValue)) {
                        field.set(object, JsonNodeUtil.getString(jsonNode, fieldName).charAt(0));
                    } else {
                        field.set(object, null);
                    }
                } else if ("char".equals(fieldType)) {
                    field.setChar(object, JsonNodeUtil.getString(jsonNode, fieldName).charAt(0));
                }else if ("class java.lang.Integer".equals(fieldType)){
                    field.set(object, JsonNodeUtil.getInteger(jsonNode, fieldName));
                }else if("int".equals(fieldType)) {
                    field.setInt(object, JsonNodeUtil.getInteger(jsonNode, fieldName));
                } else if ("class java.lang.Double".equals(fieldType)) {
                    field.set(object, JsonNodeUtil.getDouble(jsonNode, fieldName));
                }else if("double".equals(fieldType)){
                    field.setDouble(object, JsonNodeUtil.getDouble(jsonNode, fieldName));
                } else if ("class java.lang.String".equals(fieldType)) {
                    field.set(object, JsonNodeUtil.getString(jsonNode, fieldName));
                } else if ("class java.lang.Long".equals(fieldType)) {
                    field.set(object, JsonNodeUtil.getLong(jsonNode, fieldName));
                }else if ("long".equals(fieldType)) {
                    field.setLong(object, JsonNodeUtil.getLong(jsonNode, fieldName));
                }
            }
        } catch (Exception e) {
            throw new RuntimeException(e);
        }

        return object;
    }

    public static <T> T parseBean(String messageJson, Class<T> clazz, T object) {
        if (messageJson == null) {
            return null;
        } else {
            return parseBean(messageJson, Arrays.asList(clazz.getDeclaredFields()), object);
        }
    }

    public static <T> T parseBean(String messageJson, Class<T> clazz) {
        if (messageJson == null) {
            return null;
        } else {
            try{
                return parseBean(messageJson, Arrays.asList(clazz.getDeclaredFields()), clazz.newInstance());
            } catch(Exception e) {
                throw new RuntimeException(e);
            }
        }
    }

    public static <T> T injectBean(String messageJson, Class<T> clazz, T object) {
        if (messageJson == null) {
            throw new NullPointerException();
        } else {
            return injectBean(messageJson, Arrays.asList(clazz.getDeclaredFields()), object);
        }
    }

    private static <T> T injectBean(String messageJson, List<Field> fieldList, T object) {
        JsonNode jsonNode = JsonUtil.toNode(messageJson);

        try {
            for (Field field : fieldList) {
                field.setAccessible(true);
                String fieldType = field.getGenericType().toString();
                String fieldName = field.getName();

                if ((field.getModifiers() & Modifier.STATIC) == Modifier.STATIC
                        || (field.getModifiers() & Modifier.FINAL) == Modifier.FINAL) {
                    continue;
                }

                if ("class java.lang.Byte".equals(fieldType) || "byte".equals(fieldType)) {
                    field.set(object, JsonNodeUtil.getInteger(jsonNode, fieldName));
                } else if ("class java.lang.Integer".equals(fieldType) || "int".equals(fieldType)) {
                    field.set(object, JsonNodeUtil.getInteger(jsonNode, fieldName));
                } else if ("class java.lang.Double".equals(fieldType) || "double".equals(fieldType)) {
                    field.set(object, JsonNodeUtil.getInteger(jsonNode, fieldName));
                } else if ("class java.lang.Long".equals(fieldType) || "long".equals(fieldType)) {
                    field.set(object, JsonNodeUtil.getLong(jsonNode, fieldName));
                } else if ("class java.lang.String".equals(fieldType)) {
                    field.set(object, JsonNodeUtil.getString(jsonNode, fieldName));
                }
            }
        } catch (Exception e) {
            throw new RuntimeException(e);
        }

        return object;
    }
    /**
     * 将Json串转换为Java-Bean，目前支持基础类型[int, double, byte, string]、对象类型 以及 数组类型
     * 如果因为封装时，将某些字段封装到下层Java-Bean中，可置标志位hasPulledUpObject为true，
     * 这样可以进行层级解析，不过这样可能会导致速度慢点，因为会额外的解析其他的字段
     *
     * @param messageJson
     * @param clazz
     * @param hasPulledUpObject
     * @param <T>
     * @return
     */
    public static <T> T injectBean(String messageJson, Class<T> clazz, boolean hasPulledUpObject) {
        try {
            JsonNode jsonNode = JsonUtil.toNode(messageJson);
            T value = clazz.newInstance();

            if (jsonNode == null) {
                return null;
            }

            Method[] methods = clazz.getMethods();
            for (Method method : methods) {
                String methodName = method.getName();
                if (methodName.startsWith("set")) {
                    String fieldName = methodName.substring(3, 4).toLowerCase() + methodName.substring(4);
                    JsonNode currentFieldNode = jsonNode.get(fieldName);
                    Object parsedValue;
                    Class returnType = method.getParameterTypes()[0];

                    if (jsonNode.has(fieldName)) {
                        if (boolean.class.equals(returnType) || Boolean.class.equals(returnType)) {
                            parsedValue = currentFieldNode.asBoolean();
                        } else if (int.class.equals(returnType) || Integer.class.equals(returnType)) {
                            parsedValue = currentFieldNode.asInt();
                        } else if (byte.class.equals(returnType) || Byte.class.equals(returnType)) {
                            parsedValue = (byte) (currentFieldNode.asInt());
                        } else if (double.class.equals(returnType) || Double.class.equals(returnType)) {
                            parsedValue = currentFieldNode.asDouble();
                        } else if (String.class.equals(returnType)) {
                            parsedValue = currentFieldNode.asText();
                        } else if (long.class.equals(returnType) || Long.class.equals(returnType)) {
                            parsedValue = currentFieldNode.asLong();
                        } else if (List.class.isAssignableFrom(returnType)) {
                            Class<?> genericType =
                                    (Class<?>) ((ParameterizedType) method.getGenericParameterTypes()[0]).getActualTypeArguments()[0];
                            parsedValue = JsonUtil.toList(currentFieldNode.toString(), genericType);
                        } else {
                            parsedValue = injectBean(currentFieldNode.toString(), returnType);
                        }
                    } else {
                        if (hasPulledUpObject) {
                            parsedValue = injectBean(messageJson, returnType);
                        } else {
                            continue;
                        }
                    }

                    method.invoke(value, parsedValue);
                }
            }
            return value;
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    public static <T> T injectBean(String messageJson, Class<T> clazz) {
        return injectBean(messageJson, clazz, false);
    }
    public static <T> T injectBeanWithoutCaseParse(String messageJson, Class<T> clazz, boolean hasPulledUpObject) {
        try {
            JsonNode jsonNode = JsonUtil.toNode(messageJson);
            T value = clazz.newInstance();

            if (jsonNode == null) {
                return null;
            }

            Method[] methods = clazz.getMethods();
            for (Method method : methods) {
                String methodName = method.getName();
                if (methodName.startsWith("set")) {
                    String fieldName = methodName.substring(3);
                    JsonNode currentFieldNode = jsonNode.get(fieldName);
                    Object parsedValue;
                    Class returnType = method.getParameterTypes()[0];

                    if (jsonNode.has(fieldName)) {
                        if (boolean.class.equals(returnType) || Boolean.class.equals(returnType)) {
                            parsedValue = currentFieldNode.asBoolean();
                        } else if (int.class.equals(returnType) || Integer.class.equals(returnType)) {
                            parsedValue = currentFieldNode.asInt();
                        } else if (byte.class.equals(returnType) || Byte.class.equals(returnType)) {
                            parsedValue = (byte) (currentFieldNode.asInt());
                        } else if (double.class.equals(returnType) || Double.class.equals(returnType)) {
                            parsedValue = currentFieldNode.asDouble();
                        } else if (String.class.equals(returnType)) {
                            parsedValue = currentFieldNode.asText();
                        } else if (long.class.equals(returnType) || Long.class.equals(returnType)) {
                            parsedValue = currentFieldNode.asLong();
                        } else if (List.class.isAssignableFrom(returnType)) {
                            Class<?> genericType =
                                    (Class<?>) ((ParameterizedType) method.getGenericParameterTypes()[0]).getActualTypeArguments()[0];
                            parsedValue = JsonUtil.toList(currentFieldNode.toString(), genericType);
                        } else {
                            parsedValue = injectBean(currentFieldNode.toString(), returnType);
                        }
                    } else {
                        if (hasPulledUpObject) {
                            parsedValue = injectBean(messageJson, returnType);
                        } else {
                            continue;
                        }
                    }

                    method.invoke(value, parsedValue);
                }
            }
            return value;
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }
    private static String getHistoryMessageListJson() {
        String json =
                "{" +
                        "    \"vlink\": 0," +
                        "    \"flag\": 1," +
                        "    \"from\": 1001," +
                        "    \"cmd\": 1045," +
                        "    \"seq\": 1," +
                        "    \"dtype\": 1," +
                        "    \"len\": 0," +
                        "    \"sid\": 131," +
                        "    \"did\": 125," +
                        "    \"returnnum\": 25," +
                        "    \"msgInfo\": [" +
                        "        {" +
                        "            \"id\": 131," +
                        "            \"msgtime\": 144444444444," +
                        "            \"msg\": \"hello\"," +
                        "            \"features\": {" +
                        "                \"weight\": 0," +
                        "                \"style\": 1," +
                        "                \"line\": 1," +
                        "                \"color\": 19999," +
                        "                \"family\": \"Microsoft YaHei\"," +
                        "                \"size\": \"14\"" +
                        "            }" +
                        "        }," +
                        "        {" +
                        "            \"id\": 125," +
                        "            \"msgtime\": 144444444445," +
                        "            \"msg\": \"ene\"," +
                        "            \"features\": {" +
                        "                \"weight\": 0," +
                        "                \"style\": 0," +
                        "                \"line\": 0," +
                        "                \"color\": 0," +
                        "                \"family\": \"Microsoft YaHei\"," +
                        "                \"size\": \"14\"" +
                        "            }" +
                        "        }" +
                        "    ]" +
                        "}";

        return json;
    }

    private static String getUserInfoJson() {
        return "{" +
                "    \"address\": \"\"," +
                "    \"avatoruri\": \"\"," +
                "    \"birthday\": \"\"," +
                "    \"career\": 0," +
                "    \"city\": 0," +
                "    \"cmd\": 1046," +
                "    \"country\": 0," +
                "    \"description\": \"\"," +
                "    \"did\": 131," +
                "    \"dtype\": 1," +
                "    \"email\": \" zhangbei@feiniu.com\"," +
                "    \"face_url\": \"\"," +
                "    \"familyflag\": 0," +
                "    \"flag\": 245," +
                "    \"from\": 256," +
                "    \"gold_money\": 0," +
                "    \"id\": 131," +
                "    \"integral\": 0," +
                "    \"len\": 664," +
                "    \"linkpop\": 1," +
                "    \"mobile\": \"13545087166\"," +
                "    \"mobilestatus\": 0," +
                "    \"nickname\": \"�ű�\"," +
                "    \"online_time\": 120," +
                "    \"province\": 0," +
                "    \"ret\": 0," +
                "    \"rule\": 0," +
                "    \"seq\": 150731273," +
                "    \"sex\": 255," +
                "    \"sid\": 0," +
                "    \"username\": \"zhangbei@feiniu.com\"," +
                "    \"usersign\": \"\"," +
                "    \"vlink\": 0" +
                "}";
    }

    private static String getRecentlyForBuyer() {
        return "{\"cmd\":1561,\"did\":131,\"dtype\":1,\"flag\":245,\"from\":256,\"id\":131,\"len\":234,\"list\":[{\"id\":125,\"nickname\":\"�����\"},{\"id\":125,\"nickname\":\"�����\"},{\"id\":125,\"nickname\":\"�����\"},{\"id\":125,\"nickname\":\"�����\"},{\"id\":125,\"nickname\":\"�����\"},{\"id\":125,\"nickname\":\"�����\"},{\"id\":125,\"nickname\":\"�����\"},{\"id\":125,\"nickname\":\"�����\"},{\"id\":125,\"nickname\":\"�����\"},{\"id\":125,\"nickname\":\"�����\"},{\"id\":125,\"nickname\":\"�����\"},{\"id\":125,\"nickname\":\"�����\"},{\"id\":125,\"nickname\":\"�����\"},{\"id\":125,\"nickname\":\"�����\"},{\"id\":125,\"nickname\":\"�����\"},{\"id\":125,\"nickname\":\"�����\"},{\"id\":125,\"nickname\":\"�����\"}],\"msgnum\":20,\"ret\":0,\"seq\":134719248,\"sid\":131,\"vlink\":0}";
    }
}
