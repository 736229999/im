package com.feiniu.fnSubAccount.repository.impl;

import com.feiniu.fnSubAccount.bean.ChatSession;
import com.feiniu.fnSubAccount.bean.request.GetChatSessionRequest;
import com.feiniu.fnSubAccount.repository.ChatSessionMonitorDao;
import com.feiniu.fnSubAccount.util.TransformService;
import org.hibernate.SessionFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;

import java.util.Collections;
import java.util.List;

/**
 * Author: morningking
 * Date: 2016/4/7 13:54
 * Contact: 243717042@qq.com
 */
@Repository
public class ChatSessionMonitorDaoImpl implements ChatSessionMonitorDao {
    @Autowired
    private SessionFactory sessionFactory;

    @Override
    public List<ChatSession> getActiveChatSessionList(GetChatSessionRequest request) {
        String sql =
                "SELECT" +
                        "  csi.I_SESSION_ID                          AS id," +
                        "  csi.I_CUSTOMER_ID                         AS customerID," +
                        "  ui1.USERNAME                              AS customerName," +
                        "  csi.I_CUSTOMER_LEVEL                      AS customerLevel," +
                        "  u2.USERNAME || '（' || ui2.REALNAME || '）' AS serviceName," +
                        "  csi.I_SERVICE_ID                          AS serviceID," +
                        "  csi.I_FROM_DEVICE                         AS fromDevice," +
                        "  csi.I_FROM_PAGE                           AS fromPage," +
                        "  csi.I_WAITING_DURATION                    AS queuedTime," +
                        "  csi.I_SESSION_START_TIME                  AS startChatTime" +
                        " FROM IM_CSSESSION_INFO csi, IM_USER ui1, IM_USER u2, IM_CSINFO ui2" +
                        " WHERE csi.I_SESSION_END_TIME = 0 AND csi.I_CUSTOMER_ID = ui1.USERID" +
                        "      AND csi.I_SERVICE_ID = u2.USERID AND csi.I_SERVICE_ID = ui2.USERID";

        if (null != request.getIsQueuedTimeAsc()) {
            sql += " order by csi.I_WAITING_DURATION " + (request.getIsQueuedTimeAsc() ? "asc" : "desc");
        } else if (null != request.getIsStartChatTimeAsc()) {
            sql += " order by csi.I_SESSION_START_TIME " + (request.getIsStartChatTimeAsc() ? "asc" : "desc");
        } else {
            sql += " ORDER BY csi.I_SESSION_ID DESC";
        }

        return TransformService.transformResult(sessionFactory.getCurrentSession().createSQLQuery(sql),
                Collections.<String, Object>emptyMap(), ChatSession.class);
    }

    @Override
    public boolean isChatSessionClosed(int sessionID) {
        return ((Number) (sessionFactory.getCurrentSession().createSQLQuery("SELECT count(1) FROM IM_CSSESSION_INFO " +
                "WHERE I_SESSION_ID = :id AND I_SESSION_END_TIME = 0")
                .setParameter("id", sessionID)
                .uniqueResult())).intValue() == 0;
    }
}
