package com.feiniu.csWebIm.utils;

import org.apache.commons.collections4.MapUtils;
import org.hibernate.SQLQuery;
import org.hibernate.transform.Transformers;
import org.hibernate.type.*;

import java.lang.reflect.Field;
import java.util.List;
import java.util.Map;

/**
 * Created by morningking on 2014/10/10.
 */
@SuppressWarnings({ "unused","unchecked", "rawtypes" })
public class TransformService {
    public static <T> List<T> transformResult(SQLQuery sqlQuery, Map<String, Object> params, Class<T> clazz) {
        for (Field field : clazz.getDeclaredFields()){
            String fieldName = field.getName();

            String fieldType = field.getGenericType().toString();

            if ("class java.lang.Integer".equals(fieldType) || "int".equals(fieldType)) {
                sqlQuery.addScalar(fieldName, IntegerType.INSTANCE);
            } else if ("class java.lang.Double".equals(fieldType) || "double".equals(fieldType)) {
                sqlQuery.addScalar(fieldName, DoubleType.INSTANCE);
            } else if ("class java.lang.String".equals(fieldType)) {
                sqlQuery.addScalar(fieldName, StringType.INSTANCE);
            }else if ("class java.util.Date".equals(fieldType)) {
                sqlQuery.addScalar(fieldName, DateType.INSTANCE);
            }else if ("class java.math.BigDecimal".equals(fieldType)) {
                sqlQuery.addScalar(fieldName, BigDecimalType.INSTANCE);
            }else if ("class java.lang.Character".equals(fieldType) || "char".equals(fieldType)) {
                sqlQuery.addScalar(fieldName, CharacterType.INSTANCE);
            }else if ("class java.util.Timestamp".equals(fieldType)) {
                sqlQuery.addScalar(fieldName, TimestampType.INSTANCE);
            }else if ("class java.sql.Timestamp".equals(fieldType)) {
                sqlQuery.addScalar(fieldName, TimestampType.INSTANCE);
            }
        }

        if (MapUtils.isNotEmpty(params)) {
            for(Map.Entry<String, Object> entry : params.entrySet()){
                sqlQuery.setParameter(entry.getKey(), entry.getValue());
            }
        }

        return sqlQuery.setResultTransformer(Transformers.aliasToBean(clazz)).list();
    }
}
