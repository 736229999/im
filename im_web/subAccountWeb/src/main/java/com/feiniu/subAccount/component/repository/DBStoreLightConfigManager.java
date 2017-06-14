package com.feiniu.subAccount.component.repository;

import com.feiniu.subAccount.domain.StoreLightConfig;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.orm.hibernate3.HibernateTemplate;
import org.springframework.stereotype.Repository;

/**
 * Created by wangchen on 2015/2/4.
 */
@Repository
public class DBStoreLightConfigManager implements StoreLightConfigManager {
    @Autowired
    private HibernateTemplate hibernateTemplate;

    @Override
    public void updateStoreLightConfig(StoreLightConfig storeLightConfigList) {
        hibernateTemplate.saveOrUpdate(storeLightConfigList);
    }

    @Override
    public StoreLightConfig getStoreLightConfigUnderMerchant(long merchantID) {
        return hibernateTemplate.get(StoreLightConfig.class, merchantID);
    }
}
