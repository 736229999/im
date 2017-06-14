package com.feiniu.fnSubAccount.repository.impl;

import com.feiniu.fnSubAccount.bean.CSAccountInfo;
import com.feiniu.fnSubAccount.bean.KVPair;
import com.feiniu.fnSubAccount.bean.SessionInfo.*;
import com.feiniu.fnSubAccount.domain.User;
import com.feiniu.fnSubAccount.domain.UserInfo;
import com.feiniu.fnSubAccount.repository.SessionInfoDao;
import com.feiniu.fnSubAccount.util.TransformService;
import org.hibernate.SessionFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import java.util.*;

/**
 * Created by wangzhen on 2016/4/8.
 */
@Repository
public class SessionInfoDaoImpl implements SessionInfoDao{
    @Autowired
    private SessionFactory sessionFactory;

    @Override
    @Transactional
    public List<CsAccountName> getCsIdListByPartName(String partName) {
        String sql;
        Map<String, Object> params = new HashMap<>(1);
        List<KVPair<Long, String>> moumouNameList = new ArrayList<>();
        sql = "SELECT CSID as userId,REALNAME as realName,'--' as userName FROM IM_CSINFO WHERE " +
                "REALNAME like \'%"+partName+"%\' GROUP BY CSID,REALNAME ";


        return TransformService.transformResult(
                sessionFactory.getCurrentSession().createSQLQuery(sql),
                Collections.EMPTY_MAP,
                CsAccountName.class
        );
    }

    @Override
    public List<SessionInfoItem> getSessionInfoList(long startTime, long endTime, Long customerId, Long serviceId, Integer startRow, Integer endRow) {
        Map<String, Object> params = new HashMap<>(1);

        StringBuilder stringBuilder = new StringBuilder(
                "SELECT * from ( "+
                        "SELECT rownum as rowIndex,PA.* FROM ( " +
                        " SELECT I_CUSTOMER_LEVEL AS customerLevel,I_CUSTOMER_ID AS customerId,I_SERVICE_ID AS serviceId, " +
                        " I_SESSION_ID AS sessionID ,I_SESSION_START_TIME AS sessionStartTimeSql ,I_SESSION_END_TIME AS sessionEndTimeSql, " +
                        " (I_SESSION_END_TIME - I_SESSION_START_TIME) AS sessionLengthSql ,I_WAITING_DURATION AS customerWaitingTimeSql, "+
                        " I_FROM_DEVICE AS fromDeviceSql,I_FROM_PAGE AS fromPageSql " +
                        " from IM.IM_CSSESSION_INFO " +
                        " WHERE I_SESSION_START_TIME >= :startTime " +
                        " AND I_SESSION_START_TIME <= :endTime " +
                        " AND I_SESSION_END_TIME <> 0 " +
                        " ") ;

        params.put("startTime", startTime);
        params.put("endTime", endTime);
        if (customerId != null){
            stringBuilder.append(" AND I_CUSTOMER_ID = :customerId ");
            params.put("customerId",customerId);
        }
        if (serviceId != null){
            stringBuilder.append(" AND I_SERVICE_ID IN (SELECT USERID  FROM IM.IM_CSINFO WHERE CSID=:serviceId  )");
            params.put("serviceId",serviceId);
        }
        stringBuilder.append("ORDER BY I_SESSION_START_TIME DESC )PA) where rowIndex > :fromRowIndex and rowIndex <= :toRowIndex ");

        params.put("fromRowIndex", startRow);
        params.put("toRowIndex",endRow) ;

        return TransformService.transformResult(
                sessionFactory.getCurrentSession().createSQLQuery(stringBuilder.toString()),
                params,
                SessionInfoItem.class
        );
    }

    @Override
    public SessionTotalInfo getSessionInfoCount(long startTime, long endTime, Long customerId, Long serviceId) {
        Map<String, Object> params = new HashMap<>(1);

        StringBuilder stringBuilder = new StringBuilder(
                "SELECT COUNT(*) AS recodeSize ,AVG(sessionLengthSql) as avgLen,AVG(customerWaitingTimeSql) as avgWaitingTime FROM ( " +
                        " SELECT rownum as rowIndex,I_SESSION_ID AS sessionID ,I_SESSION_START_TIME AS sessionStartTimeSql ,I_SESSION_END_TIME AS sessionEndTimeSql, " +
                        " (I_SESSION_END_TIME - I_SESSION_START_TIME) AS sessionLengthSql ,I_WAITING_DURATION AS customerWaitingTimeSql, "+
                        " I_FROM_DEVICE AS fromDeviceSql,I_FROM_PAGE AS fromPageSql " +
                        " from IM.IM_CSSESSION_INFO " +
                        " WHERE I_SESSION_START_TIME >= :startTime " +
                        " AND I_SESSION_START_TIME <= :endTime " +
                        " AND I_SESSION_END_TIME <> 0");

        params.put("startTime", startTime);
        params.put("endTime", endTime);
        if (customerId != null ){
            stringBuilder.append(" AND I_CUSTOMER_ID = :customerId");
            params.put("customerId",customerId.longValue());
        }
        if (serviceId != null){
            stringBuilder.append(" AND I_SERVICE_ID IN (SELECT USERID  FROM IM.IM_CSINFO WHERE CSID=:serviceId  )");
            params.put("serviceId",serviceId.longValue());
        }

        stringBuilder.append(") ");

        List<SessionTotalInfo>  sessionTotalInfoList=TransformService.transformResult(
                sessionFactory.getCurrentSession().createSQLQuery(stringBuilder.toString()),
                params,
                SessionTotalInfo.class
        );

        if (sessionTotalInfoList == null || sessionTotalInfoList.size() == 0)
            return null;

        return sessionTotalInfoList.get(0);
    }

   public List<SessionProfileItem> getSessionProfile(Integer type,long startTime,long endTime ,Long customerId, Long serviceId){
        Map<String, Object> params = new HashMap<>(1);

        StringBuilder stringBuilder = new StringBuilder(
                "SELECT '' AS title ,I_FROM_PAGE AS typeSql,COUNT(I_FROM_PAGE) AS " +
                        " count FROM IM.IM_CSSESSION_INFO  " +
                        " WHERE I_FROM_DEVICE =:type AND " +
                        " I_SESSION_START_TIME >= :startTime " +
                        " AND I_SESSION_START_TIME <= :endTime " +
                        " AND I_SESSION_END_TIME <> 0"
        );

        params.put("startTime", startTime);
        params.put("endTime", endTime);
        params.put("type",type);
        if (customerId != null){
            stringBuilder.append(" AND I_CUSTOMER_ID = :customerId");
            params.put("customerId",customerId);
        }
        if (serviceId != null){
            stringBuilder.append(" AND I_SERVICE_ID IN (SELECT USERID  FROM IM.IM_CSINFO WHERE CSID=:serviceId  ) ");
            params.put("serviceId",serviceId);
        }
        stringBuilder.append(" GROUP BY I_FROM_PAGE");


        return TransformService.transformResult(
                sessionFactory.getCurrentSession().createSQLQuery(stringBuilder.toString()),
                params,
                SessionProfileItem.class
        );
    }

    @Override
    public Long getCustomerIdByName(String customerName) {

        List<User> resultList = sessionFactory.getCurrentSession().
                createQuery("from User where username=:customerName or mobile=:customerName or email=:customerName" )
                .setParameter("customerName",customerName)
                .list();

        if (resultList == null || resultList.size() == 0)
            return  null;
        else
            return  resultList.get(0).getUserID();
    }

    @Override
    @Transactional
    public List<UserInfo> getCustomerListByIds(String ids) {
        return sessionFactory.getCurrentSession().
                createQuery("from UserInfo where userID in ("+ids+")")
                .list();
    }

    @Override
    @Transactional
    public List<SessionCountInfo> getSessionCountInfo(long startTime, long endTime) {

        Map<String, Object> params = new HashMap<>(1);

        StringBuilder stringBuilder = new StringBuilder(
                "SELECT " +
                        "  sessionTime, " +
                        "  COUNT (sessionTime) AS sessionCount " +
                        " FROM " +
                        "  ( " +
                        "    SELECT " +
                        "      floor ( " +
                        "        MOD ( " +
                        "          I_SESSION_START_TIME - :startTime, " +
                        "          86400 " +
                        "        ) / 3600 " +
                        "      ) AS sessionTime " +
                        "    FROM " +
                        "      IM.IM_CSSESSION_INFO " +
                        "    WHERE  " +
                        "      I_SESSION_START_TIME >= :startTime " +
                        "      AND I_SESSION_END_TIME <= :endTime " +
                        "  ) " +
                        " GROUP BY " +
                        "  sessionTime");

        params.put("startTime", startTime);
        params.put("endTime", endTime);


        List<SessionCountInfo>  sessionCountInfoList=TransformService.transformResult(
                sessionFactory.getCurrentSession().createSQLQuery(stringBuilder.toString()),
                params,
                SessionCountInfo.class
        );

        if (sessionCountInfoList == null || sessionCountInfoList.size() == 0)
            return null;

        return sessionCountInfoList;
    }
}
