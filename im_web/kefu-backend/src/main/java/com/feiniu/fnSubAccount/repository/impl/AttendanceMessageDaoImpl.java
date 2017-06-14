package com.feiniu.fnSubAccount.repository.impl;


import com.feiniu.fnSubAccount.bean.AttendanceDetailInfo;
import com.feiniu.fnSubAccount.bean.AttendanceInfo;
import com.feiniu.fnSubAccount.repository.AttendanceMessageDao;
import org.hibernate.Query;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by wangzhen on 2015/7/22.
 */
@Repository
public class AttendanceMessageDaoImpl implements AttendanceMessageDao {

    @Autowired
    protected SessionFactory sessionFactory;

    protected Logger LOGGER = LoggerFactory.getLogger(this.getClass());

    public List<AttendanceInfo> getAttendanceListByTime(long startTime, long endTime, String merchantId,String subMerchantIdList) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        List<AttendanceInfo> resultList = new ArrayList<>();

        sql = " SELECT I_SUBMERCHANT_ID,COUNT(*) AS dayd,SUM( I_TOTAL_ONLINE_DURATION) as landingTime," +
                " SUM(I_FIRST_ONLINE_TIME-I_STATISTICS_TIME) ,SUM(I_LAST_ONLINE_TIME-I_STATISTICS_TIME)," +
                " min(I_FIRST_ONLINE_TIME-I_STATISTICS_TIME) ,max(I_LAST_ONLINE_TIME-I_STATISTICS_TIME) ," +
                " SUM(I_TOTAL_LEAVE_NUM),SUM(I_TOTAL_LEAVE_DURATION)" +
                " FROM IM_SUBMERCHANT_ATTENDANCE " +
                " WHERE I_TOTAL_ONLINE_DURATION >0 AND i_merchant_id = " + merchantId + " " +
                " AND I_STATISTICS_TIME >= " + startTime +
                " AND I_STATISTICS_TIME < " + endTime +
                " AND I_SUBMERCHANT_ID IN (" + subMerchantIdList +" ) "+
                " GROUP BY I_SUBMERCHANT_ID";

        query = session.createSQLQuery(sql);

        for (Object obj : query.list()) {
            if (obj == null) {
                return null;
            } else {
                Object[] res = (Object[]) obj;
                AttendanceInfo index = new AttendanceInfo();
                index.setSubMerchantId(((BigDecimal) res[0]).longValue());
                index.setAttendanceDayCount(((BigDecimal) res[1]).intValue());
                index.setSqlAttendanceLandingTimeSum(((BigDecimal) res[2]).longValue());
                index.setSqlAttendanceFirstLandingTimeSum(((BigDecimal) res[3]).longValue());
                index.setSqlAttendanceLastOnlineTimeSum(((BigDecimal) res[4]).longValue());
                index.setSqlAttendanceEarliestLandingTime(((BigDecimal) res[5]).longValue());
                index.setSqlAttendanceLatestOnlineTime(((BigDecimal) res[6]).longValue());
                index.setSqlAttendanceLeaveNum(((BigDecimal) res[7]).longValue());
                index.setSqlAttendanceLeaveDuration(((BigDecimal) res[8]).longValue());
                resultList.add(index);
            }
        }
        return resultList;
    }

    public List<AttendanceDetailInfo> getAttendanceDetailListByTime(long startTime, long endTime, String subMerchantId) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        List<AttendanceDetailInfo> resultList = new ArrayList<>();

        sql = "SELECT I_SUBMERCHANT_ID , I_STATISTICS_TIME," +
                " I_FIRST_ONLINE_TIME-I_STATISTICS_TIME,I_LAST_ONLINE_TIME-I_STATISTICS_TIME,I_LOGIN_COUNT,I_TOTAL_ONLINE_DURATION," +
                " I_TOTAL_HANGING_DURATION,I_TOTAL_OFFLINE_DURATION,I_TOTAL_LEAVE_NUM,I_TOTAL_LEAVE_DURATION" +
                " FROM IM_SUBMERCHANT_ATTENDANCE " +
                " WHERE I_SUBMERCHANT_ID = " + subMerchantId +
                " AND I_STATISTICS_TIME >= " + startTime +
                " AND I_STATISTICS_TIME < " + endTime+
                " ORDER BY I_STATISTICS_TIME ";

        query = session.createSQLQuery(sql);

        for (Object obj : query.list()) {
            if (obj == null) {
                return null;
            } else {
                Object[] res = (Object[]) obj;
                AttendanceDetailInfo index = new AttendanceDetailInfo();
                index.setSubMerchantId(((BigDecimal) res[0]).longValue());
                index.setSqlAttendanceDate(((BigDecimal) res[1]).longValue());
                index.setSqlAttendanceFirstOnlineTime(((BigDecimal) res[2]).longValue());
                index.setSqlAttendanceLastOnlineTime(((BigDecimal) res[3]).longValue());
                index.setLandingCount(((BigDecimal) res[4]).intValue());
                index.setSqlAttendanceLandingTimeSum(((BigDecimal) res[5]).longValue());
                index.setSqlAttendanceHungupTimeSum(((BigDecimal) res[6]).longValue());
                index.setSqlAttendanceOfflineTimeSum(((BigDecimal) res[7]).longValue());
                index.setSqlAttendanceLeaveNum(((BigDecimal) res[8]).longValue());
                index.setSqlAttendanceLeaveDuration(((BigDecimal) res[9]).longValue());
                resultList.add(index);
            }
        }
        return resultList;
    }


}
