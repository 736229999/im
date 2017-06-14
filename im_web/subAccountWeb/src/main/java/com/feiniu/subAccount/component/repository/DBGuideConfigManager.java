package com.feiniu.subAccount.component.repository;

import com.feiniu.subAccount.domain.GuideConfig;
import org.apache.commons.collections4.CollectionUtils;
import org.hibernate.HibernateException;
import org.hibernate.Session;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.orm.hibernate3.HibernateCallback;
import org.springframework.orm.hibernate3.HibernateTemplate;
import org.springframework.stereotype.Repository;

import java.sql.SQLException;
import java.util.List;

/**
 * Created by wangchen on 2015/5/27.
 */
@Repository
public class DBGuideConfigManager implements GuideConfigManager {
    @Autowired
    private HibernateTemplate hibernateTemplate;

    @Override
    public void updateGuideStatus(long merchantID, String version) {
        GuideConfig guideConfig = new GuideConfig();
        guideConfig.setMerchantID(merchantID);
        guideConfig.setHasGuided('Y');
        guideConfig.setVersion(version);

        hibernateTemplate.getSessionFactory().getCurrentSession()
                .saveOrUpdate(guideConfig);
    }

    @Override
    public boolean hasMerchantIDBeingGuided(long merchantID, String version) {
        List result = hibernateTemplate.find("from GuideConfig where merchantID = ? and hasGuided = 'Y' and version = ?",
                new Object[]{merchantID, version});

        return CollectionUtils.isNotEmpty(result);
    }
}
