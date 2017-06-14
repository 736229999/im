package com.feiniu.subAccount.repository.impl;

import com.feiniu.subAccount.consts.SystemConfig;
import com.feiniu.subAccount.repository.StatisticStatusDao;
import org.hibernate.Query;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by wangzhen on 2015/7/22.
 */
@Repository
public class StatisticStatusDaoImpl implements StatisticStatusDao {
    @Autowired
    protected SessionFactory sessionFactory;

    protected Logger LOGGER = LoggerFactory.getLogger(this.getClass());
    @Override
    public int insertIntoStatus(long currentTime, int statisitcStatus, String ipAddress) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();

        sql = "INSERT INTO IM.IM_STATISITC_STATUS(" +
                "\"STATISITC_TIME\",\"STATISITC_TYPE\",\"STATISITC_STATUS\",\"RETRY_TIME\",\"TRY_IP\")" +
                "SELECT" +
                " " + String.valueOf(currentTime) + "," +
                " " + String.valueOf(SystemConfig.STATISITC_TYPE_SUB_MERCHANT) + ", " +
                " " + String.valueOf(statisitcStatus) + ", " +
                " " + String.valueOf(0) + ", " +
                "'" + ipAddress + "'" +
                " from dual where not exists (select * from im.IM_STATISITC_STATUS where STATISITC_TYPE = " + SystemConfig.STATISITC_TYPE_SUB_MERCHANT
                + " and STATISITC_TIME = " + String.valueOf(currentTime) + ")"
        ;
        query = session.createSQLQuery(sql);
        return query.executeUpdate();
    }

    @Override
    public int updateStatus(long currentTime, int statisitcStatus, String ipAddress, String retrytime) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();

        sql = "UPDATE  IM_STATISITC_STATUS SET " +
                "STATISITC_STATUS =" + String.valueOf(statisitcStatus) + ", " +
                " RETRY_TIME =" + String.valueOf(retrytime) + " ," +
                " TRY_IP = '" + ipAddress + "' " +
                " WHERE STATISITC_TIME =" + String.valueOf(currentTime) + " " +
                "AND STATISITC_TYPE =" + SystemConfig.STATISITC_TYPE_SUB_MERCHANT + " ";

        query = session.createSQLQuery(sql);
        return query.executeUpdate();
    }

    @Override
    public Map<String, Object> lockUpdateStatus(Long currentTime) {
        String sql;
        Query query;
        Map<String, Object> params = new HashMap<String, Object>();
        Integer status = SystemConfig.STATISITC_STATUS_NODATA;
        Integer retrytime = 0;

        params.put("startTime", currentTime);
        params.put("type", SystemConfig.STATISITC_TYPE_SUB_MERCHANT);

        sql = "SELECT STATISITC_STATUS ,RETRY_TIME FROM " +
                "IM_STATISITC_STATUS WHERE " +
                "STATISITC_TIME = (:startTime) " +
                "AND STATISITC_TYPE=(:type)" +
                "FOR UPDATE ";

        try {
            Session session = sessionFactory.getCurrentSession();
            query = SqlHelper.createQueryWithParams(session,sql, params, 0);

            for (Object obj : query.list()) {
                if (obj == null) {
                    status = SystemConfig.STATISITC_STATUS_NODATA;
                    retrytime = 0;
                } else {
                    Object[] res = (Object[]) obj;
                    status = Integer.valueOf(res[0].toString());
                    retrytime = Integer.valueOf(res[1].toString());
                }
            }
        } catch (Exception e) {
            LOGGER.error("统计项不存在:", e);
            return null;
        }

        params.put("status", status);
        params.put("retrytime", retrytime);
        return params;
    }
}
