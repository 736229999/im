package com.feiniu.subAccount.component.repository;

import com.feiniu.subAccount.domain.ChatMessage;
import com.feiniu.subAccount.util.ListUtils;
import org.apache.commons.lang.StringUtils;
import org.hibernate.Query;
import org.hibernate.Session;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.orm.hibernate3.HibernateTemplate;
import org.springframework.stereotype.Repository;

import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by wangchen on 2015/2/4.
 */
@Repository
public class DBChatMessageManager implements ChatMessageManager {
    @Autowired
    private HibernateTemplate hibernateTemplate;

    @Override
    public List<ChatMessage> getHistoryChatMessageList(HistoryMessageListRequest historyMessageListRequest) {
        Query query = null;
        Session session = hibernateTemplate.getSessionFactory().getCurrentSession();
        String sql = "";
        Map<String, Object> params = new HashMap<String, Object>();

        if (StringUtils.isEmpty(historyMessageListRequest.getCustomerName())) {
            sql = "from ChatMessage c where (c.fromUser.userID in (:toID) or c.toUser.userID in (:toID)) ";
            params.put("toID", ListUtils.transformElementToNumber(Arrays.asList(historyMessageListRequest.getSubMerchantIDList().split(";")), Integer.class));
        } else {
            sql = "from ChatMessage c where " +
                    "((c.fromUser.userID in (:fromID) and (c.toUser.nickName = :toName or c.toUser.email = :toName or c.toUser.telphone = :toName))" +
                    " or " +
                    " ((c.fromUser.nickName = :toName or c.fromUser.email = :toName or c.fromUser.telphone = :toName) and c.toUser.userID in (:fromID)))";
            params.put("fromID", ListUtils.transformElementToNumber(Arrays.asList(historyMessageListRequest.getSubMerchantIDList().split(";")), Integer.class));
            params.put("toName", historyMessageListRequest.getCustomerName());
        }
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

        return query.setFirstResult((historyMessageListRequest.getCurrentPageNo() - 1) * historyMessageListRequest.getPageSize()).
                setMaxResults(historyMessageListRequest.getPageSize()).list();
    }

    @Override
    public long getTotalChatMessageNumber(HistoryMessageListRequest historyMessageListRequest) {
        Query query = null;
        Session session = hibernateTemplate.getSessionFactory().getCurrentSession();
        String sql = "";
        Map<String, Object> params = new HashMap<String, Object>();
        if (StringUtils.isEmpty(historyMessageListRequest.getCustomerName())) {
            sql = "select count(*) from ChatMessage c where (c.fromUser.userID in (:toID) or c.toUser.userID in (:toID)) ";
            params.put("toID", ListUtils.transformElementToNumber(Arrays.asList(historyMessageListRequest.getSubMerchantIDList().split(";")), Integer.class));
        } else {
            sql = "select count(*) from ChatMessage c where " +
                    "((c.fromUser.userID in (:fromID) and (c.toUser.nickName = :toName or c.toUser.email = :toName or c.toUser.telphone = :toName))" +
                    " or " +
                    " ((c.fromUser.nickName = :toName or c.fromUser.email = :toName or c.fromUser.telphone = :toName) and c.toUser.userID in (:fromID)))";

            params.put("fromID", ListUtils.transformElementToNumber(Arrays.asList(historyMessageListRequest.getSubMerchantIDList().split(";")), Integer.class));
            params.put("toName", historyMessageListRequest.getCustomerName());
        }
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
}
