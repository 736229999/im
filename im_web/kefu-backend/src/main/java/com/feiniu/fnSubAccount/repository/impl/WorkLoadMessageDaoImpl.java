package com.feiniu.fnSubAccount.repository.impl;


import com.feiniu.fnSubAccount.bean.ReceptionInfo;
import com.feiniu.fnSubAccount.bean.WorkLoadDetailInfo;
import com.feiniu.fnSubAccount.bean.WorkLoadInfo;
import com.feiniu.fnSubAccount.repository.WorkLoadMessageDao;
import com.feiniu.fnSubAccount.util.FormatUtils;
import org.hibernate.Query;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.Reader;
import java.math.BigDecimal;
import java.sql.Clob;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;


/**
 * Created by wangzhen on 2015/7/22.
 */
@Repository
public class WorkLoadMessageDaoImpl implements WorkLoadMessageDao {

    @Autowired
    protected SessionFactory sessionFactory;

    protected Logger LOGGER = LoggerFactory.getLogger(this.getClass());

    public List<WorkLoadInfo> getWorkLoadListByTime(long startTime, long endTime, String merchantId,String subMerchantIDlist) {

        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        List<WorkLoadInfo> resultList = new ArrayList<>();

        sql = " SELECT PPD.* ,  " +
                " (CASE WHEN PE.ASKCOUNT IS NULL THEN 0 ELSE PE.ASKCOUNT  END),  " +
                " (CASE WHEN PE.REPLYCOUNT IS NULL THEN 0 ELSE  PE.REPLYCOUNT END) ," +
                " (CASE WHEN PE.notReplyCount IS NULL THEN 0 ELSE  PE.notReplyCount END), " +
                " (CASE WHEN PE.replyTimes IS NULL THEN 0 ELSE  PE.replyTimes + PE.notReplyTimes  END), " +
                " (CASE WHEN PE.replyTimes IS NULL THEN 0 ELSE  PE.replyTimes END) " +
                " FROM(  " +
                " SELECT  " +
                "  PC.*, 0 AS G_GROUP_NAME  " +
                " FROM(  " +
                "  (  " +
                "    SELECT  " +
                "      pa.*, (  " +
                "        CASE  " +
                "        WHEN pb.landingTime IS NULL THEN  " +
                "          0  " +
                "        ELSE  " +
                "          pb.landingTime  " +
                "        END  " +
                "      )  " +
                "    FROM  " +
                "      (  " +
                "        SELECT  " +
                "          I_SUBMERCHANT_ID,  " +
                "          SUM (I_RESPONSE_DURATION) AS responseTime,  " +
                "          SUM (I_SESSION_DURATION) AS sessionTime,  " +
                "          SUM (I_SESSION_COUNT) AS sessionCount,  " +
                "          SUM (I_RESPONSE_MESSAGE_COUNT) AS responseMessageCount,  " +
                "          SUM (I_RECEPTION_DURATION) AS receptionTime,  " +
                "          SUM (I_response_count) AS reponseCount  " +
                "        FROM  " +
                "          IM_SUBMERCHANT_WORKLOAD A  " +
                "        WHERE  " +
                "          I_MERCHANT_ID =  " + String.valueOf(merchantId) +
                "        AND I_SUBMERCHANT_ID IN (" + subMerchantIDlist +" ) "+
                "        AND I_STATISTICS_TIME >=   " +String.valueOf(startTime) +
                "        AND I_STATISTICS_TIME < "  +String.valueOf(endTime) +
                "        GROUP BY  " +
                "          I_SUBMERCHANT_ID  " +
                "        ORDER BY  " +
                "          I_SUBMERCHANT_ID  " +
                "      ) pa  " +
                "    LEFT JOIN (  " +
                "      SELECT  " +
                "        I_SUBMERCHANT_ID,  " +
                "        SUM (I_TOTAL_ONLINE_DURATION) AS landingTime  " +
                "      FROM  " +
                "        IM_SUBMERCHANT_ATTENDANCE  " +
                "      WHERE  " +
                "        I_MERCHANT_ID =   " + String.valueOf(merchantId) +
                "        AND I_SUBMERCHANT_ID IN (" + subMerchantIDlist +" ) "+
                "      AND I_STATISTICS_TIME >= " +String.valueOf(startTime) +
                "      AND I_STATISTICS_TIME <  " +String.valueOf(endTime) +
                "      GROUP BY  " +
                "        I_SUBMERCHANT_ID  " +
                "    ) pb ON pa.I_SUBMERCHANT_ID = pb.I_SUBMERCHANT_ID  " +
                "  ) pc  " +
                " )) PPD  " +
                " LEFT JOIN (  " +
                FormatUtils.getSqlGetAskingCount(startTime,endTime)+
                " ) PE ON PPD.I_SUBMERCHANT_ID = PE.i_submerchant_id   " +
                " ORDER BY  " +
                "  PPD.I_SUBMERCHANT_ID";

        query = session.createSQLQuery(sql);

        for (Object obj : query.list()) {
            if (obj == null) {
                return null;
            } else {
                Object[] res = (Object[]) obj;
                WorkLoadInfo index = new WorkLoadInfo();
                index.setSubMerchantId(((BigDecimal) res[0]).longValue());
                index.setSqlResponseTimeSum(((BigDecimal) res[1]).longValue());
                index.setSqlSessionTimeSum(((BigDecimal) res[2]).longValue());
                index.setSqlSessionCount(((BigDecimal) res[3]).longValue());
                index.setSqlReponseMessageCount(((BigDecimal) res[4]).longValue());
                index.setSqlReceptionTimeSum(((BigDecimal) res[5]).longValue());
                index.setSqlResponseCount(((BigDecimal) res[6]).longValue());
                index.setSqlLandingTimeSum(((BigDecimal) res[7]).longValue());

                index.setSubMerchantGroup("--");

                index.setSqlAskingIdsCount(((BigDecimal) res[9]).longValue());
                index.setSqlResponseIdsCount(((BigDecimal) res[10]).longValue());
                index.setSqlUnResponseIdsCount(((BigDecimal) res[11]).longValue());
                index.setSqlAskingTimes(((BigDecimal) res[12]).longValue());
                index.setSqlResponseTimes(((BigDecimal) res[13]).longValue());
                resultList.add(index);
            }
        }

        return resultList;
    }

    //oracle.sql.Clob转换为String的函数
    public String ClobToString(Clob clob) throws SQLException, IOException {
        String reString = "";
        Reader is = clob.getCharacterStream();
        BufferedReader br = new BufferedReader(is);
        String s = br.readLine();
        StringBuilder sb = new StringBuilder();
        while (s != null) {
            sb.append(s);
            s = br.readLine();
        }
        reString = sb.toString();
        return reString;
    }

    public List<WorkLoadDetailInfo> getWorkLoadDetailByTime(long startTime, long endTime, String subMerchantId) {

        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        List<WorkLoadDetailInfo> resultList = new ArrayList<>();

        sql = "  SELECT  " +
                "  PPD.*,  " +
                " (CASE WHEN PE.ASKCOUNT IS NULL THEN 0 ELSE PE.ASKCOUNT  END),  " +
                " (CASE WHEN PE.REPLYCOUNT IS NULL THEN 0 ELSE  PE.REPLYCOUNT END) ," +
                " (CASE WHEN PE.notReplyCount IS NULL THEN 0 ELSE  PE.notReplyCount END), " +
                " (CASE WHEN PE.replyTimes IS NULL THEN 0 ELSE  PE.replyTimes + PE.notReplyTimes  END), " +
                " (CASE WHEN PE.replyTimes IS NULL THEN 0 ELSE  PE.replyTimes END) " +
                " FROM  " +
                "  (  " +
                "    SELECT  " +
                "      pa.*, (  " +
                "        CASE  " +
                "        WHEN pb.I_TOTAL_ONLINE_DURATION IS NULL THEN  " +
                "          0  " +
                "        ELSE  " +
                "          pb.I_TOTAL_ONLINE_DURATION  " +
                "        END  " +
                "      )  " +
                "    FROM  " +
                "      (  " +
                "        SELECT  " +
                "          I_SUBMERCHANT_ID,  " +
                "          I_STATISTICS_TIME,  " +
                "          I_RESPONSE_DURATION AS responseTime,  " +
                "          I_SESSION_DURATION AS sessionTime,  " +
                "          I_SESSION_COUNT AS sessionCount,  " +
                "          I_RESPONSE_MESSAGE_COUNT AS responseMessageCount,  " +
                "          I_RECEPTION_DURATION AS receptionTime,  " +
                "          i_response_count AS reponseCount  " +
                "        FROM  " +
                "          IM_SUBMERCHANT_WORKLOAD A  " +
                "        WHERE  " +
                "          I_SUBMERCHANT_ID = " +String.valueOf(subMerchantId) +
                "        AND I_STATISTICS_TIME >= " +String.valueOf(startTime) +
                "        AND I_STATISTICS_TIME < " +String.valueOf(endTime) +
                "        ORDER BY  " +
                "          I_SUBMERCHANT_ID  " +
                "      ) pa  " +
                "    LEFT JOIN IM_SUBMERCHANT_ATTENDANCE pb ON pa.I_SUBMERCHANT_ID = pb.I_SUBMERCHANT_ID  " +
                "    AND PA.I_STATISTICS_TIME = PB.I_STATISTICS_TIME  " +
                "    ORDER BY  " +
                "      pa.I_STATISTICS_TIME  " +
                "  ) PPD  " +
                " LEFT JOIN (  " +
                FormatUtils.getSqlGetAskingCountDetail(startTime, endTime)+
                " ) PE ON PPD.I_SUBMERCHANT_ID = PE.i_submerchant_id AND PPD.I_STATISTICS_TIME = PE.I_STATISITC_TIME" +
                " ORDER BY  " +
                "  PPD.I_SUBMERCHANT_ID ";


        query = session.createSQLQuery(sql);

        for (Object obj : query.list()) {
            if (obj == null) {
                return null;
            } else {
                Object[] res = (Object[]) obj;
                WorkLoadDetailInfo index = new WorkLoadDetailInfo();
                index.setSubMerchantId(((BigDecimal) res[0]).longValue());
                index.setSqlDate(((BigDecimal) res[1]).longValue());
                index.setSqlResponseTimeSum(((BigDecimal) res[2]).longValue());
                index.setSqlSessionTimeSum(((BigDecimal) res[3]).longValue());
                index.setSqlSessionCount(((BigDecimal) res[4]).longValue());
                index.setSqlReponseMessageCount(((BigDecimal) res[5]).longValue());
                index.setSqlReceptionTimeSum(((BigDecimal) res[6]).longValue());
                index.setSqlResponseCount(((BigDecimal) res[7]).longValue());
                index.setSqlLandingTimeSum(((BigDecimal) res[8]).longValue());
                index.setSqlAskingIdsCount(((BigDecimal) res[9]).longValue());
                index.setSqlResponseIdsCount(((BigDecimal) res[10]).longValue());
                index.setSqlUnResponseIdsCount(((BigDecimal) res[11]).longValue());
                index.setSqlAskingTimes(((BigDecimal) res[12]).longValue());
                index.setSqlResponseTimes(((BigDecimal) res[13]).longValue());
                resultList.add(index);
            }
        }

        return resultList;

    }

    public int updateReplyDetail(long startTime, long endTime) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();

        sql = "call mini_user_pkg.STATISTICSUPDATEREPLYDETAIL(" +
                String.valueOf(startTime) + ", " +
                String.valueOf(endTime) +
                ")";

        query = session.createSQLQuery(sql);
        return query.executeUpdate();
    }

    public int updateWorkLoadList(long startTime, long endTime, String subMerchantIDs, String merchantIDs, long statisticsDate) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();

        sql = "call mini_user_pkg.STATISTICSWORKLOAD(" +
                String.valueOf(startTime) + ", " +
                String.valueOf(endTime) + ", " +
                "'" + subMerchantIDs + "', " +
                "'" + merchantIDs + "', " +
                String.valueOf(statisticsDate) +
                ")";

        query = session.createSQLQuery(sql);

        return query.executeUpdate();

    }

    public List<ReceptionInfo> getReceptionInfo(String subMerchantIDs, long currentTime) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        List<ReceptionInfo> resultList = new ArrayList<>();

        sql = "SELECT I_FROMID ,I_REPLY_START,I_REPLY_END FROM T_SESSION_DETAIL WHERE I_FROMID IN " +
                "(" + String.valueOf(subMerchantIDs) + ")" +
                " AND  I_START_TIME >= " + String.valueOf(currentTime) +
                " AND  I_START_TIME < " + String.valueOf(currentTime + 24 * 60 * 60);

        query = session.createSQLQuery(sql);

        for (Object obj : query.list()) {
            if (obj == null) {
                return null;
            } else {
                Object[] res = (Object[]) obj;
                ReceptionInfo index = new ReceptionInfo();
                index.setSubMerchantId(((BigDecimal) res[0]).longValue());
                index.setReplyStart(((BigDecimal) res[1]).longValue());
                index.setReplyEnd(((BigDecimal) res[2]).longValue());
                resultList.add(index);
            }
        }

        return resultList;
    }

    @Override
    public int setReceptionTime(long receptionTime, long currentTime, long subMerchantId) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();

        sql = "UPDATE IM.IM_SUBMERCHANT_WORKLOAD SET I_RECEPTION_DURATION = " + String.valueOf(receptionTime) +
                " WHERE I_SUBMERCHANT_ID = " + String.valueOf(subMerchantId) +
                " AND I_STATISTICS_TIME = " + String.valueOf(currentTime);

        query = session.createSQLQuery(sql);
        return query.executeUpdate();
    }


    @Override
    public WorkLoadDetailInfo getTotalWorkLoadListDetailByTime(long startTime, long endTime, String submerchantId) {
        Query query;
        Session session = sessionFactory.getCurrentSession();
        String sql = FormatUtils.getSqlGetAskingCountDetailTotal(startTime, endTime, Long.valueOf(submerchantId));
        WorkLoadDetailInfo totalInfo = new WorkLoadDetailInfo();
        totalInfo.initWithZero();
        query = session.createSQLQuery(sql);

        for (Object obj : query.list()) {
            if (obj == null) {
                totalInfo.setSubMerchantId(Long.valueOf(submerchantId));
                totalInfo.setSqlLandingTimeSum(0);
                totalInfo.setSqlAskingTimes(0);
                totalInfo.setSqlResponseTimes(0);
                return null;
            } else {
                Object[] res = (Object[]) obj;
                totalInfo.setSubMerchantId(((BigDecimal) res[0]).longValue());
                totalInfo.setSqlLandingTimeSum(((BigDecimal) res[1]).longValue());
                totalInfo.setSqlAskingIdsCount(((BigDecimal) res[2]).longValue());
                totalInfo.setSqlResponseIdsCount(((BigDecimal) res[3]).longValue());
            }
        }

        return totalInfo;
    }
    @Override
    public WorkLoadInfo getTotalWorkLoadListByTime(long startTime, long endTime, String merchantId){
        Query query;
        Session session = sessionFactory.getCurrentSession();
        String sql = FormatUtils.getSqlGetAskingCountTotal(startTime,endTime,Long.valueOf(merchantId));
        WorkLoadInfo totalInfo = new WorkLoadInfo();
        totalInfo.initWithZero();
        query = session.createSQLQuery(sql);

        for (Object obj : query.list()) {
            if (obj == null) {
                totalInfo.setMerchantId(Long.valueOf(merchantId));
                totalInfo.setSqlLandingTimeSum(0);
                totalInfo.setSqlAskingTimes(0);
                totalInfo.setSqlResponseTimes(0);
                return null;
            } else {
                Object[] res = (Object[]) obj;
                totalInfo.setMerchantId(((BigDecimal) res[0]).longValue());
                totalInfo.setSqlLandingTimeSum(((BigDecimal) res[1]).longValue());
                totalInfo.setSqlAskingIdsCount(((BigDecimal) res[2]).longValue());
                totalInfo.setSqlResponseIdsCount(((BigDecimal) res[3]).longValue());
            }
        }

        return totalInfo;
    }
}
