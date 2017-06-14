package com.feiniu.imServer.component;


import com.feiniu.imServer.domain.MerchantDiverseConfig;
import org.hibernate.SessionFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

/**
 * Created by wangchen on 2015/2/4.
 */
@Repository
@Transactional
public class DBMerchantDiverseConfigManager implements MerchantDiverseConfigManager {
    @Autowired
    private SessionFactory sessionFactory;


    @Override
    public MerchantDiverseConfig getDiverseConfigForMerchant(long merchantID) {
        return (MerchantDiverseConfig)sessionFactory.getCurrentSession().
                get(MerchantDiverseConfig.class, merchantID);
    }

    @Override
    public void setDiverseConfigForMerchant(MerchantDiverseConfig merchantDiverseConfig) {
        sessionFactory.getCurrentSession().saveOrUpdate(merchantDiverseConfig);
    }
}
