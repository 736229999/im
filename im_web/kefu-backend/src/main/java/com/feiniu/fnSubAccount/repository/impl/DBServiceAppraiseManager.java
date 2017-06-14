package com.feiniu.fnSubAccount.repository.impl;

import com.feiniu.fnSubAccount.bean.ServiceAppraiseDetailStat;
import com.feiniu.fnSubAccount.bean.ServiceAppraiseStat;
import com.feiniu.fnSubAccount.bean.request.ServiceAppraiseRequest;
import com.feiniu.fnSubAccount.repository.ServiceAppraiseManager;
import org.hibernate.Query;
import org.hibernate.Session;
import org.hibernate.transform.Transformers;
import org.hibernate.type.StandardBasicTypes;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.orm.hibernate3.HibernateTemplate;
import org.springframework.stereotype.Repository;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by wangchen on 2015/2/12.
 */
@SuppressWarnings({ "unused","unchecked", "rawtypes" })
@Repository
public class DBServiceAppraiseManager implements ServiceAppraiseManager {
    @Autowired
    private HibernateTemplate hibernateTemplate;

    @Override
    public List<ServiceAppraiseStat> getServiceAppraiseList(ServiceAppraiseRequest serviceAppraiseRequest) {
        Session session = hibernateTemplate.getSessionFactory().getCurrentSession();

        return session.createSQLQuery("SELECT s.TOID AS subAccountID, s.RECOMMENT AS appraise, count(*) as counter " +
                "from im_webrecomment s where s.TOID in (:subMerchantIDList) and s.SENDTIME >= (:startTime)" +
                " and s.SENDTIME <= (:endTime) group by s.TOID, s.RECOMMENT order by s.TOID").
                addScalar("subAccountID", StandardBasicTypes.INTEGER).
                addScalar("appraise", StandardBasicTypes.INTEGER).
                addScalar("counter", StandardBasicTypes.INTEGER).
                setParameterList("subMerchantIDList", serviceAppraiseRequest.getSubAccountIDList()).
                setParameter("startTime", serviceAppraiseRequest.getStartTime()).
                setParameter("endTime", serviceAppraiseRequest.getEndTime()).
                setResultTransformer(Transformers.aliasToBean(ServiceAppraiseStat.class)).list();
    }

    @Override
    public List<ServiceAppraiseDetailStat> getServiceAppraiseList(long startTime,long endTime,String subMerchantId,
                                                                  int searchType,int startRow, int endRow){

        List<ServiceAppraiseDetailStat> result = new ArrayList<>();
        Query query;
        Session session;

        String sql = " WITH commentTab AS ( " +
                "SELECT * FROM ( " +
                " SELECT ppa.* ,ROWNUM rn FROM(  " +
                " SELECT " +
                "  pa.FROMID, " +
                "  PA.TOID, " +
                "  PA.SENDTIME, " +
                "  PA.RECOMMENT, " +
                "  (CASE WHEN PB.NICKNAME IS NULL THEN '--' ELSE PB.NICKNAME END) AS buyerName, " +
                "  (CASE WHEN pc.NICKNAME IS NULL THEN '--' ELSE pc.NICKNAME END) AS submerchantName, " +
                "  v_recomment_text AS commentText, " +
                "  i_session_id AS sessionId " +
                " FROM " +
                "  ( " +
                "    SELECT " +
                "      * " +
                "    FROM " +
                "      IM.IM_WEBRECOMMENT " +
                "    WHERE " +
                "      TOID =  " +String.valueOf(subMerchantId) +
                "    AND SENDTIME >=  " +String.valueOf(startTime)+
                "    AND SENDTIME <=  " +String.valueOf(endTime)+
                "    AND RECOMMENT = " + String.valueOf(searchType) +
                "  ) pa " +
                " LEFT JOIN IM_USERINFO pb ON pa.FROMID = pb.USERID " +
                " LEFT JOIN IM_USERINFO pc ON pa.TOID = pc.USERID " +
                " )  ppa " +
                " ) " +
                " WHERE RN >=  " + String.valueOf(startRow)+
                " AND RN <=  " + String.valueOf(endRow)+
                " )  " +
                " SELECT bbb.* ," +
                " (CASE WHEN aaa.sessionStart IS NULL THEN 0 ELSE aaa.sessionStart END) AS sessionStart , " +
                " (CASE WHEN AAA.sessionEnd IS NULL THEN 0 ELSE AAA.sessionEnd END) AS sessionend  " +
                " FROM commentTab bbb " +
                " LEFT JOIN" +
                " (  " +
                " SELECT MIN(SENDTIME) AS sessionStart,MAX(SENDTIME) AS sessionEnd ,LOGINTIME from IM.IM_WEBDATA WHERE DATATYPE <>21  " +
                " AND LOGINTIME IN (SELECT SESSIONID FROM commentTab) " +
                " GROUP BY LOGINTIME ) aaa " +
                " ON aaa.LOGINTIME = bbb.sessionid "
                ;


        session = hibernateTemplate.getSessionFactory().getCurrentSession();
        query = session.createSQLQuery(sql);

        for (Object obj : query.list()) {
            if (obj == null) {
                return result;
            } else {
                Object[] res = (Object[]) obj;
                ServiceAppraiseDetailStat index = new ServiceAppraiseDetailStat();
                index.setBuyerId(((BigDecimal) res[0]).intValue());
                index.setSubMerchantID(((BigDecimal) res[1]).intValue());
                index.setSqlAppraiseTime(((BigDecimal) res[2]).longValue());
                index.setSqlAppraise(((BigDecimal) res[3]).intValue());
                index.setBuyerName(((String) res[4]));
                index.setSubMerchantName(((String) res[5]));
                index.setComment(((String) res[6]));
                if (res[7] != null)
                    index.setSessionId(((BigDecimal) res[7]).longValue());
                else
                    index.setSessionId(-2);

                index.setSqlSessionStartTime(((BigDecimal) res[9]).longValue());
                index.setSqlSessionEndTime(((BigDecimal) res[10]).longValue());
                result.add(index);
            }
        }

        return result;
    }

}
