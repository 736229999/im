package com.feiniu.csWebIm.repository.impl;

import com.feiniu.csWebIm.bean.domain.AfterSaleBoardConfig;
import com.feiniu.csWebIm.bean.domain.KefuIndexConfig;
import com.feiniu.csWebIm.consts.SystemConfig;
import com.feiniu.csWebIm.repository.HomePageDao;
import org.hibernate.SessionFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;

/**
 * Author: morningking
 * Date: 2015/11/24
 * Contact: 243717042@qq.com
 */
@Repository
@Transactional
public class HomePageDaoImpl implements HomePageDao{
    @Autowired
    private SessionFactory sessionFactory;

    @Override
    public List<AfterSaleBoardConfig> getAfterSaleBoardConfigList() {
        return sessionFactory.getCurrentSession().
                createQuery("from AfterSaleBoardConfig where dataType = " + SystemConfig.RUNTIME_INDICATOR + " order by boardID asc ")
                .list();
    }

    @Override
    public KefuIndexConfig getIndexConfig() {
        return (KefuIndexConfig) sessionFactory.getCurrentSession().
                createQuery("from KefuIndexConfig where dataType = " + SystemConfig.RUNTIME_INDICATOR)
                .list().get(0);
    }
}
