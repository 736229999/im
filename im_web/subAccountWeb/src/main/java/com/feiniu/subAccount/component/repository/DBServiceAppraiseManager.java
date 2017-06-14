package com.feiniu.subAccount.component.repository;

import com.feiniu.subAccount.bean.request.ServiceAppraiseRequest;
import com.feiniu.subAccount.bean.response.ServiceAppraiseStat;
import com.feiniu.subAccount.domain.ServiceAppraise;
import org.hibernate.Hibernate;
import org.hibernate.Session;
import org.hibernate.transform.Transformers;
import org.hibernate.type.IntegerType;
import org.hibernate.type.PrimitiveType;
import org.hibernate.type.StringType;
import org.hibernate.type.TypeFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.orm.hibernate3.HibernateTemplate;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * Created by wangchen on 2015/2/12.
 */
@Repository
public class DBServiceAppraiseManager implements ServiceAppraiseManager{
    @Autowired
    private HibernateTemplate hibernateTemplate;

    @Override
    public List<ServiceAppraiseStat> getServiceAppraiseList(ServiceAppraiseRequest serviceAppraiseRequest) {
        Session session = hibernateTemplate.getSessionFactory().getCurrentSession();

        return session.createSQLQuery("SELECT s.TOID AS subAccountID, s.RECOMMENT AS appraise, count(*) as counter " +
                "from im_webrecomment s where s.TOID in (:subMerchantIDList) and s.SENDTIME >= (:startTime)" +
                " and s.SENDTIME <= (:endTime) group by s.TOID, s.RECOMMENT order by s.TOID").
                addScalar("subAccountID", Hibernate.INTEGER).
                addScalar("appraise", Hibernate.INTEGER).
                addScalar("counter", Hibernate.INTEGER).
                setParameterList("subMerchantIDList", serviceAppraiseRequest.getSubAccountIDList()).
                setParameter("startTime", serviceAppraiseRequest.getStartTime()).
                setParameter("endTime", serviceAppraiseRequest.getEndTime()).
                setResultTransformer(Transformers.aliasToBean(ServiceAppraiseStat.class)).list();
    }
}
