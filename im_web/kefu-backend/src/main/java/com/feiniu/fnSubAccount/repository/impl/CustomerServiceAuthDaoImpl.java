package com.feiniu.fnSubAccount.repository.impl;

import com.feiniu.fnSubAccount.bean.SubMerchantInfo;
import com.feiniu.fnSubAccount.repository.CustomerServiceAuthDao;
import org.hibernate.Query;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.transform.Transformers;
import org.hibernate.type.StandardBasicTypes;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;

import java.math.BigDecimal;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by wangzhen on 2015/10/10.
 */
@SuppressWarnings({ "unused","unchecked", "rawtypes" })
@Repository
public class CustomerServiceAuthDaoImpl implements CustomerServiceAuthDao{
    @Autowired
    private SessionFactory sessionFactory;

    @Override
    public boolean isIdDirector(long id){
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        Map<String, Object> params = new HashMap<String, Object>();

        params.put("id", id);

        sql = "SELECT PERMISSION FROM IM.IM_CSINFO WHERE CSID = (:id)";
        query = session.createSQLQuery(sql);

        for (Map.Entry<String, Object> entry : params.entrySet()) {
            if (entry.getValue() instanceof List) {
                query.setParameterList(entry.getKey(), (List) entry.getValue());
            } else {
                query.setParameter(entry.getKey(), entry.getValue());
            }
        }
        List<BigDecimal> resultObj = query.list();

        if (resultObj == null ||resultObj.size() == 0)
            return  false;
        else {
            return  (resultObj.get(0)).intValue() == 0;
        }
    }


    @Override
    public Long convertIDToMoumouId(long directorId){
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        Map<String, Object> params = new HashMap<String, Object>();

        params.put("id", directorId);

        sql = "SELECT USERID FROM IM.IM_CSINFO WHERE CSID = (:id)";
        query = session.createSQLQuery(sql);

        for (Map.Entry<String, Object> entry : params.entrySet()) {
            if (entry.getValue() instanceof List) {
                query.setParameterList(entry.getKey(), (List) entry.getValue());
            } else {
                query.setParameter(entry.getKey(), entry.getValue());
            }
        }
        List<BigDecimal> resultObj = query.list();

        if (resultObj == null ||resultObj.size() == 0)
            return  null;
        else {
            return  (resultObj.get(0)).longValue();
        }
    }

    @Override
    public List<SubMerchantInfo> getAllSubMerchantInfoList(){
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();

        sql = "SELECT userid AS memberId, -1 AS merchantId ,USERNAME AS username, REALNAME AS truename,STATUS AS memberType " +
                "FROM im.IM_CSINFO";

        return (List<SubMerchantInfo>)session.createSQLQuery(sql).
                addScalar("memberId", StandardBasicTypes.LONG).
                addScalar("merchantId", StandardBasicTypes.LONG).
                addScalar("username", StandardBasicTypes.STRING).
                addScalar("truename", StandardBasicTypes.STRING).
                addScalar("memberType", StandardBasicTypes.INTEGER).
                setResultTransformer(Transformers.aliasToBean(SubMerchantInfo.class)).list();
    }


    @Override
    public List<SubMerchantInfo> getSubMerchantInfoListByIdList(String idList){
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();

        sql = " SELECT userid AS memberId, -1 AS merchantId ,USERNAME AS username, REALNAME AS truename,STATUS AS memberType " +
                " FROM im.IM_CSINFO" +
                " WHERE CSID IN (" + idList + ") " ;

        return (List<SubMerchantInfo>)session.createSQLQuery(sql).
                addScalar("memberId", StandardBasicTypes.LONG).
                addScalar("merchantId", StandardBasicTypes.LONG).
                addScalar("username", StandardBasicTypes.STRING).
                addScalar("truename", StandardBasicTypes.STRING).
                addScalar("memberType", StandardBasicTypes.INTEGER).
                setResultTransformer(Transformers.aliasToBean(SubMerchantInfo.class)).list();
    }
}
