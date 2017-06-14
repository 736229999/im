package com.feiniu.csWebIm.repository.impl;

import com.feiniu.csWebIm.repository.CsService;
import org.hibernate.Query;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import java.util.HashMap;
import java.util.List;

/**
 * Created by wangzhen on 2015/11/27.
 */
@Repository
@Transactional
public class CsServiceImpl implements CsService{
    @Autowired
    private SessionFactory sessionFactory;
    @Override
    public String getTureNameByCsMoumouId(String moumouId) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        HashMap<String, Object> params = new HashMap<>(1);
        sql = " SELECT REALNAME FROM IM.IM_CSINFO WHERE USERID =" +moumouId ;

        List resultList = (List)sessionFactory.getCurrentSession().createSQLQuery(sql).list();

        if(resultList == null || resultList.size() == 0)
            return "--";

        String realName = (String)resultList.get(0);

        if (realName == null)
            return  "--";
        else
            return realName;
    }

    @Override
    public String getCsIdByCsMoumouId(String moumouId){
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        HashMap<String, Object> params = new HashMap<>(1);

        if(moumouId == null || moumouId.equals(""))
            return null;

        sql = " SELECT CSID FROM IM.IM_CSINFO WHERE USERID =" +moumouId ;

        List resultList = (List)sessionFactory.getCurrentSession().createSQLQuery(sql).list();

        if(resultList == null || resultList.size() == 0)
            return null;

        String csid = (String)resultList.get(0).toString();

        return csid;
    }


}

