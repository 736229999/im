package com.feiniu.subAccount.component.repository;

import com.feiniu.subAccount.domain.ServiceGroup;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.orm.hibernate3.HibernateTemplate;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * Created by wangchen on 2015/2/4.
 */
@Repository
public class DBServiceGroupManager implements ServiceGroupManager {
    @Autowired
    private HibernateTemplate hibernateTemplate;

    @Override
    public ServiceGroup addServiceGroup(ServiceGroup serviceGroup) {
        hibernateTemplate.save(serviceGroup);
        return serviceGroup;
    }

    @Override
    public void deleteServiceGroup(ServiceGroup serviceGroup) {
        hibernateTemplate.delete(serviceGroup);
    }

    @Override
    public void renameServiceGroup(ServiceGroup serviceGroup) {
        hibernateTemplate.update(serviceGroup);
    }

    @Override
    public List<ServiceGroup> getServiceGroupsUnderMerchant(long merchantID) {
        return hibernateTemplate.find("from ServiceGroup s where s.merchantID = ? order by s.isShowOnStore desc ," +
                "s.showOrder desc ,s.groupID asc", merchantID);
    }

    @Override
    public List<ServiceGroup> getServiceGroupListShowOnStoreUnderMerchant(long merchantID) {
        return hibernateTemplate.find("from ServiceGroup s where s.merchantID = ? and s.isShowOnStore = '1' and" +
                " exists (select m.mappingID from ServiceGroupMapping m where m.serviceGroup.groupID = s.groupID and m.diverseState = '1') " +
                " order by s.showOrder desc", merchantID);
    }

    @Override
    public void updateServiceGroupList(List<ServiceGroup> serviceGroupList) {
        hibernateTemplate.saveOrUpdateAll(serviceGroupList);
    }
}
