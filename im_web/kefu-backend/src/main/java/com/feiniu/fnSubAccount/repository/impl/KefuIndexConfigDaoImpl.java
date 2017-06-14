package com.feiniu.fnSubAccount.repository.impl;

import com.feiniu.fnSubAccount.consts.SystemConfig;
import com.feiniu.fnSubAccount.domain.KefuIndexConfig;
import com.feiniu.fnSubAccount.repository.KefuIndexConfigDao;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import java.sql.Timestamp;

/**
 * Author: morningking
 * Date: 2015/10/14
 * Contact: 243717042@qq.com
 */
@Transactional
@Repository
public class KefuIndexConfigDaoImpl implements KefuIndexConfigDao {
    @Autowired
    private SessionFactory sessionFactory;

    @Override
    public KefuIndexConfig getDesignTimeData() {
        return (KefuIndexConfig) sessionFactory.getCurrentSession().
                createQuery("from KefuIndexConfig where dataType = " + SystemConfig.DESIGN_TIME_INDICATOR)
                .list().get(0);
    }

    @Override
    public KefuIndexConfig getRuntimeData() {
        return (KefuIndexConfig) sessionFactory.getCurrentSession().
                createQuery("from KefuIndexConfig where dataType = " + SystemConfig.RUNTIME_INDICATOR)
                .list().get(0);
    }

    @Override
    public void update(KefuIndexConfig kefuIndexConfig) {
        Session session = sessionFactory.getCurrentSession();
        KefuIndexConfig saved = getDesignTimeData();

        kefuIndexConfig.setConfigID(saved.getConfigID());
        kefuIndexConfig.setJsonOfFeiniuIndex(saved.getJsonOfFeiniuIndex());

        session.merge(kefuIndexConfig);
        session.flush();
    }

    @Override
    public void updateFooter(String footerJson) {
        Session session = sessionFactory.getCurrentSession();
        KefuIndexConfig designTimeData = getDesignTimeData();
        designTimeData.setJsonOfFeiniuIndex(footerJson);

        session.update(designTimeData);
    }

    @Override
    public KefuIndexConfig syncEditToRuntime() {
        Session session = sessionFactory.getCurrentSession();
        KefuIndexConfig oldRuntimeConfig = getRuntimeData();
        KefuIndexConfig kefuIndexConfig = getDesignTimeData();

        try {
            kefuIndexConfig = (KefuIndexConfig) (kefuIndexConfig.clone());
        } catch (Exception e) {
            throw new RuntimeException(e);
        }

        kefuIndexConfig.setConfigID(oldRuntimeConfig.getConfigID());
        kefuIndexConfig.setUpdateTime(new Timestamp(System.currentTimeMillis()));
        kefuIndexConfig.setDataType(SystemConfig.RUNTIME_INDICATOR);

        session.merge(kefuIndexConfig);
        session.flush();

        return kefuIndexConfig;
    }
}
