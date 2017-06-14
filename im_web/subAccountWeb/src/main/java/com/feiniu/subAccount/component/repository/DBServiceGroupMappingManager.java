package com.feiniu.subAccount.component.repository;

import com.feiniu.subAccount.domain.ServiceGroup;
import com.feiniu.subAccount.domain.ServiceGroupMapping;
import org.hibernate.HibernateException;
import org.hibernate.Session;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.orm.hibernate3.HibernateCallback;
import org.springframework.orm.hibernate3.HibernateTemplate;
import org.springframework.stereotype.Repository;

import java.sql.SQLException;
import java.util.List;

/**
 * Created by wangchen on 2015/2/4.
 */
@Repository
public class DBServiceGroupMappingManager implements ServiceGroupMappingManager {
    @Autowired
    private HibernateTemplate hibernateTemplate;

    @Override
    public void addSubAccountToGroup(ServiceGroupMapping serviceGroupMapping) {
        hibernateTemplate.save(serviceGroupMapping);
    }

    @Override
    public void addSubAccountListToGroup(List<ServiceGroupMapping> serviceGroupMappingList) {
        hibernateTemplate.saveOrUpdateAll(serviceGroupMappingList);
    }

    @Override
    public void removeSubAccountFromGroup(List<Long> subAccountIDList) {
        Session session = hibernateTemplate.getSessionFactory().getCurrentSession();
        session.createQuery("delete from ServiceGroupMapping s where s.subAccountID in (:subAccountIDList)").
                setParameterList("subAccountIDList", subAccountIDList).executeUpdate();
    }

    @Override
    public void transferSubAccountToGroup(List<Long> subAccountIDList, ServiceGroup serviceGroup) {
        Session session = hibernateTemplate.getSessionFactory().getCurrentSession();

        session.createQuery("update ServiceGroupMapping s set s.serviceGroup = (:serviceGroup) where s.subAccountID in (:subAccountIDList)").
                setParameter("serviceGroup", serviceGroup).
                setParameterList("subAccountIDList", subAccountIDList).executeUpdate();
    }

    @Override
    public void updateDiverseStateForSubAccount(ServiceGroupMapping serviceGroupMapping) {
        Session session = hibernateTemplate.getSessionFactory().getCurrentSession();
        session.createQuery("update ServiceGroupMapping m set m.diverseState = (:diverseState) where m.subAccountID = (:subAccountID)").
                setParameter("diverseState", serviceGroupMapping.getDiverseState()).
                setParameter("subAccountID", serviceGroupMapping.getSubAccountID()).
                executeUpdate();
    }

    @Override
    public List<ServiceGroupMapping> getSubAccountListUnderGroup(long groupID) {
        return hibernateTemplate.find
                ("from ServiceGroupMapping m where m.serviceGroup.groupID = ?", groupID);
    }

    @Override
    public List<ServiceGroupMapping> getDiversedSubAccountListUnderGroupList(List<ServiceGroup> groupList) {
        Session session = hibernateTemplate.getSessionFactory().getCurrentSession();

        return session.createQuery("from ServiceGroupMapping m where m.serviceGroup in (:groupList) and m.diverseState = '1'").
                setParameterList("groupList", groupList).list();
    }

    @Override
    public List<ServiceGroupMapping> getCategorizedSubAccounts(long merchantID) {
        return hibernateTemplate.find
                ("from ServiceGroupMapping m where m.serviceGroup.merchantID = ?", merchantID);
    }

    @Override
    public List<ServiceGroupMapping> getSubaccountsUnderGroups(final List<Long> groupIDList) {
        return hibernateTemplate.executeFind(new HibernateCallback<List>() {
            @Override
            public List doInHibernate(Session session) throws HibernateException, SQLException {
                return session.createQuery("from ServiceGroupMapping m where m.diverseState = '1' and m.serviceGroup.groupID in (:groupIDList)")
                        .setParameterList("groupIDList", groupIDList).list();
            }
        });
    }
}
