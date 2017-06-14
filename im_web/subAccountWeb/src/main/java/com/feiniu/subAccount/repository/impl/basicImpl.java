package com.feiniu.subAccount.repository.impl;

import org.hibernate.Query;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;

import java.util.List;
import java.util.Map;

/**
 * Created by wangzhen on 2015/7/22.
 */
public class basicImpl {
    @Autowired
    protected SessionFactory sessionFactory;

    protected  Logger LOGGER= LoggerFactory.getLogger(this.getClass());

    protected Query createQueryWithParams(String sql, Map<String, Object> params, int type) {
        Session session = sessionFactory.getCurrentSession();
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
