package com.feiniu.imServer.repository.impl;

import com.feiniu.imServer.bean.HistoryMessage;
import com.feiniu.imServer.bean.HistoryMessageListRequest;
import com.feiniu.imServer.bean.OfflineMessage;
import com.feiniu.imServer.bean.OfflineMessageRequest;
import com.feiniu.imServer.constants.BusinessConfig;
import com.feiniu.imServer.consts.SystemConfig;
import com.feiniu.imServer.repository.PagingMsgDao;
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
 * Created by chenqing on 2016/1/28.
 */
@SuppressWarnings({ " unused" ," unchecked" , " rawtypes"  })
@Repository
@Transactional
public class PagingMsgDaoImpl implements PagingMsgDao {
    private static final Logger LOGGER = LoggerFactory.getLogger(PagingMsgDaoImpl.class);
    @Autowired
    SessionFactory sessionFactory;
    @Autowired
    private BusinessConfig businessConfig;

    @Override
    //历史消息记录分页查询
    public List<HistoryMessage> getHistoryPagingMessgeById(HistoryMessageListRequest historyMessageListRequest) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        ArrayList<HistoryMessage> messageList = new ArrayList<>();
        Map<String, Object> params = new HashMap<String, Object>();
        long sendTime = historyMessageListRequest.getSendTime();
        Integer pageCount = historyMessageListRequest.getPageCount();
        List<String> subMerchantIds = historyMessageListRequest.getSubMerchantIds();
        Integer customerId = historyMessageListRequest.getCustomerId();

        sql= "SELECT  ROWNUM,A .*"+
                " FROM "+
                " ( "+
                "   SELECT "+
                "       ID, "+
                "       SENDTIME, "+
                "       msg, "+
                "       FONTSIZE, "+
                "       FONTCOLOR, "+
                "       FONTSTYLE, "+
                "       FONTNAME, "+
                "       nickName, "+
                "       msgid "+
                "   FROM "+
                "   ( "+
                "       SELECT "+
                "           FROMID AS ID, "+
                "           TOID, "+
                "           SENDTIME , "+
                "           MSG AS msg, "+
                "           FONTSIZE, "+
                "           FONTCOLOR, "+
                "           FONTSTYLE, "+
                "           FONTNAME, "+
                "           msgid AS msgid "+
                "       FROM "+
                "       IM.IM_WEBDATA "+
                "  ) f "+
                " LEFT JOIN ( "+
                "       SELECT "+
                "           userid, "+
                "           nickname AS nickName "+
                "       FROM "+
                "       IM.IM_USERINFO "+
                " ) b ON b.userid = f. ID "+
                "  WHERE "+
                " ( "+
                "   (ID = :customerId AND TOID IN(:sids)) "+
                "   OR (ID IN(:sids) AND TOID = :customerId) "+
                "   ) AND SENDTIME <= :sendTime " +
                " ORDER BY "+
                "  SENDTIME DESC "+
                " ) A "+
                "  WHERE "+
                " ROWNUM <= :pageCount";

        if(sendTime == 0){
            sendTime = SystemConfig.MAX_END_TIME;
        }
        params.put("customerId",customerId);
        params.put("sids",subMerchantIds);
        params.put("sendTime",sendTime);
        params.put("pageCount",pageCount);
        query = session.createSQLQuery(sql);
        for (Map.Entry<String, Object> entry : params.entrySet()) {
            if (entry.getValue() instanceof List) {
                query.setParameterList(entry.getKey(), (List) entry.getValue());
            } else {
                query.setParameter(entry.getKey(), entry.getValue());
            }
        }
        for (Object row : query.list()) {
            if (row == null) {
                return messageList;
            } else {
                Object[] res = (Object[]) row;
                HistoryMessage bean = new HistoryMessage();
                byte fontStyle = res[6] != null ? ((BigDecimal) res[6]).byteValue() : (byte) 0;
                byte weight = (byte) (fontStyle & 0x04);
                byte style = (byte) (fontStyle & 0x01);
                byte line = (byte) (fontStyle & 0x02);

                if (weight == 0x04) {
                    weight = 1;
                } else {
                    weight = 0;
                }
                if (style == 0x01) {
                    style = 1;
                } else {
                    style = 0;
                }
                if (line == 0x02) {
                    line = 1;
                } else {
                    line = 0;
                }
                bean.setId(res[1] != null ? ((BigDecimal) res[1]).intValue() : 0);
                bean.setTime(res[2] != null ? ((BigDecimal) res[2]).longValue() : 0);
                bean.setMsg(res[3] != null ? res[3].toString() : "");
                bean.getFeatures().setSize(res[4] != null ? ((BigDecimal) res[4]).byteValue() : (byte) 0);
                bean.getFeatures().setColor(res[5] != null ? ((BigDecimal) res[5]).intValue() : 0);
                bean.getFeatures().setStyle(style);
                bean.getFeatures().setLine(line);
                bean.getFeatures().setWeight(weight);
                bean.getFeatures().setFamily(res[7] != null ? res[7].toString() : "");
                bean.setNickName(res[8] != null ? res[8].toString() : "");
                bean.setMsgid(res[9] != null ? ((BigDecimal) res[9]).intValue() : 0);
                messageList.add(bean);
            }
        }

        return messageList;
    }

    //离线消息记录分页查询
    @Override
    public List<OfflineMessage> getOfflinePagingMessgeById(OfflineMessageRequest offlineMessageRequest) {

        Query query;
        Session session=sessionFactory.getCurrentSession();
        Map<String,Object> params=new HashMap<String,Object>();
        ArrayList<OfflineMessage> messageList = new ArrayList<OfflineMessage>();

        Integer toId=offlineMessageRequest.getId();
        long startTime=offlineMessageRequest.getStartTime();
        long endTime=offlineMessageRequest.getEndTime();
        long msgId=offlineMessageRequest.getMsgId();
        long pageCount=offlineMessageRequest.getPageCount();

         String sql="SELECT * FROM ( "+
            "   SELECT "+
            "       FROMID, "+
            "       TOID, "+
            "       SENDTIME, "+
            "       FONTSIZE, "+
            "       FONTCOLOR, "+
            "       FONTSTYLE, "+
            "       FONTNAME, "+
            "       FROMNICKNAME, "+
            "       MSG, "+
            "       MSGID, "+
            "       LOGINTIME, "+
            "       UUID "+
            "    FROM "+
            "    IM.IM_WEBDATA "+
            "    WHERE "+
            "       TOID = :toId "+
            "       AND SENDTIME>=:startTime "+
            "       AND SENDTIME<=:endTime "+
            "       AND msgid>:msgId "+
            "   ORDER BY MSGID "+
            " ) "+
            " where rownum <= :pageCount";
        query=session.createSQLQuery(sql);

        params.put("toId",toId);
        params.put("startTime",startTime);
        params.put("endTime",endTime);
        params.put("msgId",msgId);
        params.put("pageCount",pageCount);
        for (Map.Entry<String, Object> entry : params.entrySet()) {
            if (entry.getValue() instanceof List) {
                query.setParameterList(entry.getKey(), (List) entry.getValue());
            } else {
                query.setParameter(entry.getKey(), entry.getValue());
            }
        }
        for (Object row : query.list()) {
            if (row == null) {
                return messageList;
            } else {
                Object[] res = (Object[]) row;
                OfflineMessage bean = new OfflineMessage();

                bean.setFromId(res[0] != null ? ((BigDecimal) res[0]).intValue() : 0);
                bean.setToId(res[1] != null ? ((BigDecimal) res[1]).intValue() : 0);
                bean.setSendTime(res[2] != null ? ((BigDecimal) res[2]).longValue() : 0);
                bean.getFeatures().setSize(res[3] != null ? ((BigDecimal) res[3]).byteValue() : (byte) 0);
                bean.getFeatures().setColor(res[4] != null ? ((BigDecimal) res[4]).intValue() : 0);
                bean.getFeatures().setStyle(res[5] != null ? ((BigDecimal) res[5]).byteValue() : (byte) 0);
                bean.getFeatures().setFamily(res[6] != null ? res[6].toString() : "");
                bean.setNickName(res[7] != null ? res[7].toString() : "");
                bean.setMsg(res[8] != null ? res[8].toString() : "");
                bean.setMsgid(res[9] != null ? ((BigDecimal) res[9]).intValue() : 0);
                bean.setLoginTime(res[10] != null ? ((BigDecimal) res[10]).longValue() : 0);
                bean.setUuId(res[11] != null ? res[11].toString() : "");
                messageList.add(bean);

            }
        }

        return messageList;
    }

    //根据本次上线时间得到上次离线时间
    @Override
    public long getLastOfflineTimeById(Integer serviceId, Long onlineTime) {

        Query query;
        Session session=sessionFactory.getCurrentSession();
        Map<String,Object> params=new HashMap<String,Object>();

        String sql="select t.* from( " +
                "   SELECT " +
                "       LOGOUTTIME " +
                "   FROM " +
                "   IM_LOGINOUTTIME " +
                "   WHERE " +
                "       USERID =:sid " +
                "       AND LOGINTIME <:onlineTime" +
                "   ORDER BY " +
                "       LOGINTIME DESC " +
                " )t " +
                " where ROWNUM<=1";
        query=session.createSQLQuery(sql);

        params.put("sid",serviceId);
        params.put("onlineTime",onlineTime);

        for (Map.Entry<String, Object> entry : params.entrySet()) {
            if (entry.getValue() instanceof List) {
                query.setParameterList(entry.getKey(), (List) entry.getValue());
            } else {
                query.setParameter(entry.getKey(), entry.getValue());
            }
        }

        List resultTimeList = query.list();


        return resultTimeList.size() == 0 ? 0 : Long.valueOf(resultTimeList.get(0).toString());
    }
}
