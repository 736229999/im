package com.feiniu.subAccount.component.repository;

import com.feiniu.subAccount.domain.DiverseType;
import com.feiniu.subAccount.domain.MerchantDiverseConfig;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.orm.hibernate3.HibernateTemplate;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * Created by wangchen on 2015/2/4.
 */
@Repository
public class DBMerchantDiverseConfigManager implements MerchantDiverseConfigManager {
    @Autowired
    private HibernateTemplate hibernateTemplate;

    @Override
    public List<DiverseType> getDiverseTypeList() {
        return hibernateTemplate.find("from DiverseType order by isRecommend desc ,diverseTypeID asc ");
    }

    @Override
    public MerchantDiverseConfig getDiverseConfigForMerchant(long merchantID) {
        return hibernateTemplate.get(MerchantDiverseConfig.class, merchantID);
    }

    @Override
    public void setDiverseConfigForMerchant(MerchantDiverseConfig merchantDiverseConfig) {
        hibernateTemplate.saveOrUpdate(merchantDiverseConfig);
    }
}
