package com.feiniu.fnSubAccount.repository.impl;

import com.feiniu.fnSubAccount.bean.SessionInfo.SessionQuest;
import com.feiniu.fnSubAccount.bean.request.GetHistoryMessageListWithSessionRequest;
import com.feiniu.fnSubAccount.bean.request.HistoryMessageListRequest;
import com.feiniu.fnSubAccount.domain.ChatMessage;
import com.feiniu.fnSubAccount.repository.ChatMessageDao;
import com.feiniu.fnSubAccount.util.ListUtils;
import com.feiniu.fnSubAccount.util.TransformService;
import org.apache.commons.lang.StringUtils;
import org.hibernate.Query;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;

import java.math.BigDecimal;
import java.util.*;

/**
 * Created by wangchen on 2015/2/4.
 */
@SuppressWarnings({"unused", "unchecked", "rawtypes"})
@Repository
public class ChatMessageDaoImpl implements ChatMessageDao {
    @Autowired
    private SessionFactory sessionFactory;


    @Override
    public List<ChatMessage> getHistoryChatMessageListByIdList(HistoryMessageListRequest historyMessageListRequest) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        Map<String, Object> params = new HashMap<String, Object>();

        if (StringUtils.isEmpty(historyMessageListRequest.getCustomerID())) {
            sql = "from ChatMessage c where " +
                    "(((c.fromUser.userID in (:fromID) )) " +
                    " or " +
                    " ( c.toUser.userID in (:fromID)))";

        } else {
            sql = "from ChatMessage c where " +
                    "(((c.fromUser.userID in (:fromID) and (c.toUser.userName = :toID or c.toUser.telphone = :toID or c.toUser.email = :toID))) " +
                    " or " +
                    " ((c.fromUser.userName = :toID or c.fromUser.telphone = :toID or c.fromUser.email = :toID) and c.toUser.userID in (:fromID)))";
            params.put("toID", historyMessageListRequest.getCustomerID());
        }

        params.put("fromID",
                ListUtils.transformElementToNumber(Arrays.asList(historyMessageListRequest.getSubMerchantIDList().split(",")), Long.class));

        if (historyMessageListRequest.getStartTime() != null) {
            sql += " and sendtime >= :startTime";
            params.put("startTime", historyMessageListRequest.getStartTime());
        }
        if (historyMessageListRequest.getEndTime() != null) {
            sql += " and sendTime <= :endTime";
            params.put("endTime", historyMessageListRequest.getEndTime());
        }

        sql += " order by c.sendTime desc";

        query = session.createQuery(sql);
        for (Map.Entry<String, Object> entry : params.entrySet()) {
            if (entry.getValue() instanceof List) {
                query.setParameterList(entry.getKey(), (List) entry.getValue());
            } else {
                query.setParameter(entry.getKey(), entry.getValue());
            }
        }

        return (List<ChatMessage>) query.setFirstResult((historyMessageListRequest.getCurrentPageNo() - 1) * historyMessageListRequest.getPageSize()).
                setMaxResults(historyMessageListRequest.getPageSize()).list();
    }

    @Override
    public long getTotalChatMessageNumberByIdList(HistoryMessageListRequest historyMessageListRequest) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        Map<String, Object> params = new HashMap<String, Object>();

        if (StringUtils.isEmpty(historyMessageListRequest.getCustomerID())) {
            sql = "select count(*) from ChatMessage c where " +
                    "(((c.fromUser.userID in (:fromID)  )) " +
                    " or " +
                    " (  c.toUser.userID in (:fromID)))";

        } else {
            sql = "select count(*) from ChatMessage c where " +
                    "(((c.fromUser.userID in (:fromID) and (c.toUser.userName = :toID or c.toUser.telphone = :toID or c.toUser.email = :toID))) " +
                    " or " +
                    " ((c.fromUser.userName = :toID or c.fromUser.telphone = :toID or c.fromUser.email = :toID) and c.toUser.userID in (:fromID)))";
            params.put("toID", historyMessageListRequest.getCustomerID());
        }

        params.put("fromID",
                ListUtils.transformElementToNumber(Arrays.asList(historyMessageListRequest.getSubMerchantIDList().split(",")), Long.class));

        if (historyMessageListRequest.getStartTime() != null) {
            sql += " and sendtime >= :startTime";
            params.put("startTime", historyMessageListRequest.getStartTime());
        }
        if (historyMessageListRequest.getEndTime() != null) {
            sql += " and sendTime <= :endTime";
            params.put("endTime", historyMessageListRequest.getEndTime());
        }

        query = session.createQuery(sql);
        for (Map.Entry<String, Object> entry : params.entrySet()) {
            if (entry.getValue() instanceof List) {
                query.setParameterList(entry.getKey(), (List) entry.getValue());
            } else {
                query.setParameter(entry.getKey(), entry.getValue());
            }
        }

        return (Long) query.list().get(0);
    }

    @Override
    public List<ChatMessage> getHistoryChatMessageList(HistoryMessageListRequest historyMessageListRequest) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        Map<String, Object> params = new HashMap<String, Object>();

        sql = "from ChatMessage c where " +
                "(((c.fromUser.userName = :fromID or c.fromUser.telphone = :fromID or c.fromUser.email = :fromID) " +
                "   and (c.toUser.userName = :toID or c.toUser.telphone = :toID or c.toUser.email = :toID))) " +
                " or " +
                " ((c.fromUser.userName = :toID or c.fromUser.telphone = :toID or c.fromUser.email = :toID) " +
                " and (c.toUser.userName = :fromID or c.toUser.telphone = :fromID or c.toUser.email = :fromID) )";
        params.put("fromID", historyMessageListRequest.getSubMerchantNamePart());
        params.put("toID", historyMessageListRequest.getCustomerID());

        if (historyMessageListRequest.getStartTime() != null) {
            sql += " and sendtime >= :startTime";
            params.put("startTime", historyMessageListRequest.getStartTime());
        }
        if (historyMessageListRequest.getEndTime() != null) {
            sql += " and sendTime <= :endTime";
            params.put("endTime", historyMessageListRequest.getEndTime());
        }

        sql += " order by c.sendTime desc";

        query = session.createQuery(sql);
        for (Map.Entry<String, Object> entry : params.entrySet()) {
            if (entry.getValue() instanceof List) {
                query.setParameterList(entry.getKey(), (List) entry.getValue());
            } else {
                query.setParameter(entry.getKey(), entry.getValue());
            }
        }

        return (List<ChatMessage>) query.setFirstResult((historyMessageListRequest.getCurrentPageNo() - 1) * historyMessageListRequest.getPageSize()).
                setMaxResults(historyMessageListRequest.getPageSize()).list();
    }

    @Override
    public long getTotalChatMessageNumber(HistoryMessageListRequest historyMessageListRequest) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        Map<String, Object> params = new HashMap<String, Object>();

        sql = "select count(*) " +
                " from ChatMessage c where " +
                " (((c.fromUser.userName = :fromID or c.fromUser.telphone = :fromID or c.fromUser.email = :fromID) " +
                "   and (c.toUser.userName = :toID or c.toUser.telphone = :toID or c.toUser.email = :toID))) " +
                " or " +
                " ((c.fromUser.userName = :toID or c.fromUser.telphone = :toID or c.fromUser.email = :toID) " +
                " and (c.toUser.userName = :fromID or c.toUser.telphone = :fromID or c.toUser.email = :fromID) )";

        params.put("fromID", historyMessageListRequest.getSubMerchantNamePart());
        params.put("toID", historyMessageListRequest.getCustomerID());

        if (historyMessageListRequest.getStartTime() != null) {
            sql += " and sendtime >= :startTime";
            params.put("startTime", historyMessageListRequest.getStartTime());
        }
        if (historyMessageListRequest.getEndTime() != null) {
            sql += " and sendTime <= :endTime";
            params.put("endTime", historyMessageListRequest.getEndTime());
        }

        query = session.createQuery(sql);
        for (Map.Entry<String, Object> entry : params.entrySet()) {
            if (entry.getValue() instanceof List) {
                query.setParameterList(entry.getKey(), (List) entry.getValue());
            } else {
                query.setParameter(entry.getKey(), entry.getValue());
            }
        }

        return (Long) query.list().get(0);
    }

    @Override
    public List<ChatMessage> getHistoryChatMessageListByid(long sessionId, long subMerchantId,
                                                           int startRow, int endRow) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        Map<String, Object> params = new HashMap<String, Object>();

        List<ChatMessage> result = new ArrayList<>();
//        ChatMessage aaa = new ChatMessage();
//        aaa.setDataType(0);
//        aaa.setFontColor(0);
//        aaa.setFontName("Microsoft YaHei");
//        aaa.setFontSize(12);
//        aaa.setFromNickName("x0907xbbccc177");
//        aaa.setLoginTime(1441761488);
//        aaa.setMessageID(3261618);
//        aaa.setMsg("nihao#11#2947444584");
//        aaa.setRecvFlag(2);
//        aaa.setSendTime(1441761487);
//        aaa.setVersion(285);
//        User fromuser = new User();
//        fromuser.setEmail("qa02@uitox.com");
//        fromuser.setTelphone("11111");
//        fromuser.setUserID(5006624);
//        fromuser.setUserName("test");
//
//        User touser = new User();
//        touser.setEmail("qa02@uitox.com");
//        touser.setTelphone("11111");
//        touser.setUserID(10087);
//        touser.setUserName("sdfsdfsdf");
//
//        aaa.setFromUser(fromuser);
//        aaa.setToUser(touser);
//
//
//        result.add(aaa);
//        result.add(aaa);
//
//
//        return  result;
        if (sessionId == 0) {
            return new ArrayList<>();
        }

        sql = "from ChatMessage c where c.loginTime= (:sessionId)" +
                " AND ( c.fromUser.userID = (:subMerchantId)  OR  " +
                "  c.toUser.userID = (:subMerchantId) ) ORDER BY c.sendTime desc ";

        params.put("subMerchantId", subMerchantId);
        params.put("sessionId", sessionId);

        query = session.createQuery(sql);
        for (Map.Entry<String, Object> entry : params.entrySet()) {
            if (entry.getValue() instanceof List) {
                query.setParameterList(entry.getKey(), (List) entry.getValue());
            } else {
                query.setParameter(entry.getKey(), entry.getValue());
            }
        }
        List resultList = query.setFirstResult(startRow).
                setMaxResults(endRow - startRow + 1).list();
        return resultList;
    }

    @Override
    public long getHistoryChatMessageCountByid(long sessionId, long subMerchantId) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        Map<String, Object> params = new HashMap<String, Object>();

        //for test
//        return  30;

        if (sessionId == 0) {
            return 0;
        }

        sql = "select count(*) from ChatMessage c where c.loginTime = (:sessionId)" +
                " AND ( c.fromUser.userID = (:subMerchantId)  OR  " +
                "  c.toUser.userID = (:subMerchantId) ) ";

        params.put("subMerchantId", subMerchantId);
        params.put("sessionId", sessionId);

        query = session.createQuery(sql);
        for (Map.Entry<String, Object> entry : params.entrySet()) {
            if (entry.getValue() instanceof List) {
                query.setParameterList(entry.getKey(), (List) entry.getValue());
            } else {
                query.setParameter(entry.getKey(), entry.getValue());
            }
        }

        return (Long) query.list().get(0);
    }

    @Override
    public List<ChatMessage> getHistoryChatMessageListWithinSession(GetHistoryMessageListWithSessionRequest request) {
        Session session = sessionFactory.getCurrentSession();

        String sql = "from ChatMessage c where " +
                " ((c.fromUser.userID = :fromID and (c.toUser.userID = :toID)) " +
                " or " +
                " (c.fromUser.userID = :toID and c.toUser.userID = :fromID))" +
                " and sendTime >= :startTime" +
                " order by c.sendTime asc";

        Query query = session.createQuery(sql);

        query.setParameter("toID", request.getCustomerID());
        query.setParameter("fromID", request.getServiceID());
        query.setParameter("startTime", request.getStartChatTime());

        return query.list();
    }


    @Override
    public long getCsgetCsHistoryChatMessageLenBySession(long sessionId) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        Map<String, Object> params = new HashMap<String, Object>();

        params.put("sessionId", sessionId);

        sql = "SELECT count(*) FROM(" +
                " SELECT  a.* FROM (SELECT I_CUSTOMER_ID,I_SERVICE_ID ,I_SESSION_START_TIME,I_SESSION_END_TIME FROM IM.IM_CSSESSION_INFO WHERE I_SESSION_ID = :sessionId)   B" +
                " LEFT JOIN IM_WEBDATA A" +
                " ON  ((A.FROMID = B.I_CUSTOMER_ID AND a.TOID = B.I_SERVICE_ID) OR (A.FROMID = B.I_SERVICE_ID AND a.TOID = B.I_CUSTOMER_ID))" +
                " AND a.SENDTIME >= b.I_SESSION_START_TIME AND a.SENDTIME <= b.I_SESSION_END_TIME" +
                " )WHERE fromid >0";

        params.put("sessionId", sessionId);
        query = session.createSQLQuery(sql);
        for (Map.Entry<String, Object> entry : params.entrySet()) {
            if (entry.getValue() instanceof List) {
                query.setParameterList(entry.getKey(), (List) entry.getValue());
            } else {
                query.setParameter(entry.getKey(), entry.getValue());
            }
        }

        List resultList = query.list();
        if (resultList == null || resultList.size() ==0)
            return  0;
        else
            return ((BigDecimal)resultList.get(0)).longValue();
    }

    @Override
    public List<ChatMessage> getCsHistoryChatMessageListBySession(long sessionId,int startRow,int rowCount) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        Map<String, Object> params = new HashMap<String, Object>();

        sql = "SELECT I_CUSTOMER_ID,I_SERVICE_ID ,I_SESSION_START_TIME,I_SESSION_END_TIME FROM IM.IM_CSSESSION_INFO WHERE I_SESSION_ID = :sessionId";

        params.put("sessionId", sessionId);
        List<SessionQuest> sessionQuestList = TransformService.transformResult(
                sessionFactory.getCurrentSession().createSQLQuery(sql),
                params,
                SessionQuest.class
        );

        if (sessionQuestList == null || sessionQuestList.size() ==0)
            return Collections.EMPTY_LIST;

        SessionQuest sessionQuest = sessionQuestList.get(0);

        sql = "from ChatMessage c where"+
                " ((c.fromUser.userID = (:fromId) AND c.toUser.userID = (:toId)) OR" +
                " (c.fromUser.userID = (:toId) AND c.toUser.userID = (:fromId)))" +
                " AND c.sendTime >= (:startTime) AND c.sendTime <= (:endTime)" +
                " order by c.sendTime desc" ;
        params.clear();
        params.put("toId", sessionQuest.getI_CUSTOMER_ID().longValue());
        params.put("fromId", sessionQuest.getI_SERVICE_ID().longValue());
        params.put("startTime", sessionQuest.getI_SESSION_START_TIME().intValue());
        params.put("endTime", sessionQuest.getI_SESSION_END_TIME().intValue());


        query = session.createQuery(sql);
        for (Map.Entry<String, Object> entry : params.entrySet()) {
            if (entry.getValue() instanceof List) {
                query.setParameterList(entry.getKey(), (List) entry.getValue());
            } else {
                query.setParameter(entry.getKey(), entry.getValue());
            }
        }

        return ( List<ChatMessage>) query.setFirstResult(startRow).
                setMaxResults(rowCount).list();

    }
}
