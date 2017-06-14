package com.feiniu.imServer.component;


import com.feiniu.imServer.domain.ServiceGroupMapping;
import org.hibernate.SessionFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;

/**
 * Created by wangchen on 2015/2/4.
 */
@Repository
@Transactional
public class DBServiceGroupMappingManager implements ServiceGroupMappingManager {
    @Autowired
    private SessionFactory sessionFactory;


    @Override
    public List<ServiceGroupMapping> getSubAccountListUnderGroup(long groupID) {
        return sessionFactory.getCurrentSession().createQuery(
                "from ServiceGroupMapping m where m.serviceGroup.groupID = " + groupID).list();
    }

    @Override
    public List<ServiceGroupMapping> getCategorizedSubAccounts(long merchantID) {
        return sessionFactory.getCurrentSession().createQuery(
                "from ServiceGroupMapping m where m.serviceGroup.merchantID = " + merchantID).list();
    }

}
