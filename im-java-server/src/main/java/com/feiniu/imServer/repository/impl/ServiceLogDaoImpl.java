package com.feiniu.imServer.repository.impl;

import com.feiniu.imServer.bean.ServiceLog.HistoryServiceLog;
import com.feiniu.imServer.repository.ServiceLogDao;
import com.feiniu.imServer.util.time.TimeFormatUtils;
import org.hibernate.Query;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by wangzhen on 2015/10/29.
 */
@Transactional
@Repository
public class ServiceLogDaoImpl implements ServiceLogDao {

    protected Logger LOGGER = LoggerFactory.getLogger(this.getClass());
    @Autowired
    private SessionFactory sessionFactory;

    public int saveServiceLog(String timeInSeconds, String customerUUID, String serviceName, String logDetail) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();

        sql = "INSERT INTO IM.IM_SERVICE_LOG(SAVE_TIME,CUSTOMER_UUID,SERVICE_NAME,LOG_DETAIL)" +
                "VALUES (" + timeInSeconds + "," +
                "'" + customerUUID + "'," +
                "'" + serviceName + "'," +
                "'" + logDetail + "'" + ")";

        query = session.createSQLQuery(sql);
        return query.executeUpdate();
    }

    public List<HistoryServiceLog> getLastTenHistoryLog(String customerUUID) {
        String sql;
        Query query;
        Map<String, Object> params = new HashMap<String, Object>();
        List<HistoryServiceLog> result = new ArrayList<>();

        params.put("customerUUID", customerUUID);

        sql = "SELECT * FROM " +
                "IM.IM_SERVICE_LOG WHERE CUSTOMER_UUID =(:customerUUID) ORDER BY SAVE_TIME DESC";


        Session session = sessionFactory.getCurrentSession();
        query = session.createSQLQuery(sql);
        for (Map.Entry<String, Object> entry : params.entrySet()) {
            if (entry.getValue() instanceof List) {
                query.setParameterList(entry.getKey(), (List) entry.getValue());
            } else {
                query.setParameter(entry.getKey(), entry.getValue());
            }
        }

        for (Object obj : query.setMaxResults(10).list()) {
            if (obj == null) {
                return result;
            } else {
                HistoryServiceLog index = new HistoryServiceLog();
                Object[] res = (Object[]) obj;
                index.setTime(TimeFormatUtils.convertNumberToDateAndTime(((BigDecimal) res[0]).longValue()));
                index.setServiceName(String.valueOf(res[2]));
                index.setDetail(String.valueOf(res[3]));
                result.add(index);
            }
        }

        return result;

    }
}
