package com.feiniu.fnSubAccount.repository.impl;

import org.hibernate.Query;
import org.hibernate.Session;

import java.util.List;
import java.util.Map;

/**
 * Created by wangzhen on 2015/7/22.
 */
public class SqlHelper {
     public static Query createQueryWithParams(Session session,String sql, Map<String, Object> params, int type) {
        Query query;

        if (type == 0)
            query = session.createSQLQuery(sql);
        else
            query = session.createQuery(sql);

        for (Map.Entry<String, Object> entry : params.entrySet()) {
            if (entry.getValue() instanceof List) {
                query.setParameterList(entry.getKey(), (List) entry.getValue());
            } else {
                query.setParameter(entry.getKey(), entry.getValue());
            }
        }
        return query;
    }
}
