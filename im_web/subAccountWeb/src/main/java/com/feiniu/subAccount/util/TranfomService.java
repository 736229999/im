package com.feiniu.subAccount.util;

import org.hibernate.SQLQuery;
import org.hibernate.SessionFactory;
import org.hibernate.transform.Transformers;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import java.lang.reflect.Field;
import java.util.List;
import java.util.Map;

/**
 * Created by morningking on 2014/10/10.
 */
@Repository
@Transactional
public class TranfomService {
    @Autowired
    private static SessionFactory sessionFactory;

    public static <T> List<T> transformResult(String url, Map<String, Object> params, Class<T> clazz) {
        SQLQuery query = sessionFactory.getCurrentSession().createSQLQuery(url);
        for (Field field : clazz.getDeclaredFields()){
            String fieldName = field.getName();
            query.addScalar(fieldName);
        }

        for(Map.Entry<String, Object> entry : params.entrySet()){
            query.setParameter(entry.getKey(), entry.getValue());
        }

        return query.setResultTransformer(Transformers.aliasToBean(clazz)).list();
    }
}
