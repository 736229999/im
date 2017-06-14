package com.feiniu.imServer.repository.impl;

import com.feiniu.imServer.bean.Shunt.CurrentSessionItem;
import com.feiniu.imServer.bean.Shunt.SessionInfoItem;
import com.feiniu.imServer.repository.SessionInfoDao;
import com.feiniu.imServer.util.TransformService;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by wangzhen on 2016/4/7.
 */
@Repository
@Transactional
public class SessionInfoDaoImpl implements SessionInfoDao {
    @Autowired
    private SessionFactory sessionFactory;

    @Override
    public void updateSessionInfoEndTime(SessionInfoItem sessionInfoItem) {
        Session session = sessionFactory.getCurrentSession();
        session.createSQLQuery("UPDATE IM.IM_CSSESSION_INFO SET " +
                " I_SESSION_END_TIME = :sessionEndTime WHERE I_CUSTOMER_ID = :customerId " +
                " AND I_SERVICE_ID =:serviceId AND I_SESSION_END_TIME=0 " )
                .setParameter("customerId", sessionInfoItem.getCustomerId())
                .setParameter("serviceId", sessionInfoItem.getServiceId())
                .setParameter("sessionEndTime", sessionInfoItem.getSessionEndTime())
                .executeUpdate();
    }

    @Override
    public void putSessionInfo(SessionInfoItem sessionInfoItem) {
        Session session = sessionFactory.getCurrentSession();
        session.createSQLQuery("INSERT INTO IM.IM_CSSESSION_INFO(I_SESSION_ID,I_CUSTOMER_ID,I_CUSTOMER_LEVEL,I_SERVICE_ID,I_SESSION_START_TIME,I_SESSION_END_TIME,I_WAITING_DURATION,I_FROM_DEVICE,I_FROM_PAGE) " +
                "  select SEQ_IM_CSSESSION_INFO.nextval,:customerId,:customerLevel,:serviceId,:sessionStartTime,:sessionEndTime,:waitingDuration,:fromDevice,:fromPage FROM dual ")
                .setParameter("customerId", sessionInfoItem.getCustomerId())
                .setParameter("serviceId", sessionInfoItem.getServiceId())
                .setParameter("sessionStartTime", sessionInfoItem.getSessionStartTime())
                .setParameter("sessionEndTime", 0)
                .setParameter("waitingDuration", sessionInfoItem.getWaitingTime())
                .setParameter("fromDevice", sessionInfoItem.getFromDevice())
                .setParameter("fromPage", sessionInfoItem.getFromPage())
                .setParameter("customerLevel",sessionInfoItem.getUserLevel())
                .executeUpdate();
    }

    @Override
    @Transactional
    public void transSessionInfo(SessionInfoItem sessionInfoItemRequest) {
        Map<String, Object> params = new HashMap<>(1);
        Long customerId = sessionInfoItemRequest.getCustomerId();
        Long serviceId = sessionInfoItemRequest.getServiceId();
        Long transId = sessionInfoItemRequest.getTransId();

        StringBuilder stringBuilder = new StringBuilder(
                "SELECT  " +
                        " - 1 AS eventType ,- 1 AS groupId,  " +
                        " I_CUSTOMER_ID AS customerId,  " +
                        " I_Service_ID AS serviceID,  " +
                        " I_SESSION_START_TIME AS sessionStartTime,  " +
                        " I_SESSION_END_TIME AS sessionEndTime,  " +
                        " I_WAITING_DURATION AS waitingTime,  " +
                        " I_FROM_DEVICE AS fromDevice,  " +
                        " I_FROM_PAGE AS fromPage,  " +
                        " I_CUSTOMER_LEVEL AS userLevel ," +
                        " -1 AS transId" +
                        " FROM  " +
                        " IM.IM_CSSESSION_INFO" +
                        " WHERE I_SESSION_END_TIME =0 AND I_CUSTOMER_ID = :customerId" +
                        " AND I_Service_ID = :serviceId");

        params.put("customerId",customerId);
        params.put("serviceId",serviceId);
        List<SessionInfoItem> sessionInfoItemList = TransformService.transformResult(
                sessionFactory.getCurrentSession().createSQLQuery(stringBuilder.toString()),
                params,
                SessionInfoItem.class
        );
        if (sessionInfoItemList == null || sessionInfoItemList.size() ==0)
            return;

        SessionInfoItem sessionInfoItem = sessionInfoItemList.get(0);
        sessionInfoItem.setSessionEndTime(System.currentTimeMillis()/1000);
        sessionInfoItem.setSessionStartTime(System.currentTimeMillis()/1000);
        sessionInfoItem.setWaitingTime(Long.valueOf(0));

        updateSessionInfoEndTime(sessionInfoItem);
        sessionInfoItem.setServiceId(transId);
        putSessionInfo(sessionInfoItem);

        return;
    }

    @Override
    @Transactional
    public List<CurrentSessionItem> getCurrentSessionItemList() {
        Map<String, Object> params = new HashMap<>(1);

        StringBuilder stringBuilder = new StringBuilder(
                " SELECT I_SESSION_ID AS sessionId ,I_SERVICE_ID as serviceId,I_CUSTOMER_ID AS " +
                        " CUSTOMERID FROM IM.IM_CSSESSION_INFO WHERE I_SESSION_END_TIME =0");

        return TransformService.transformResult(
                sessionFactory.getCurrentSession().createSQLQuery(stringBuilder.toString()),
                params,
                CurrentSessionItem.class
        );
    }
}
