package com.feiniu.subAccount.component.repository;

import com.feiniu.subAccount.constants.BusinessConfig;
import com.feiniu.subAccount.domain.GroupServicePlaceConfig;
import com.feiniu.subAccount.domain.MappingForGroupServicePlace;
import org.apache.commons.collections4.CollectionUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.orm.hibernate3.HibernateTemplate;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * Created by wangchen on 2015/2/4.
 */
@Repository
public class DBMappingGroupShowPlaceConfigManager implements MappingGroupShowPlaceConfigManager {
    @Autowired
    private HibernateTemplate hibernateTemplate;
    @Autowired
    private BusinessConfig businessConfig;

    @Override
    public void setGroupPlaceList(List<MappingForGroupServicePlace> mappingForGroupServicePlace) {
        hibernateTemplate.saveOrUpdateAll(mappingForGroupServicePlace);
    }

    @Override
    public List<MappingForGroupServicePlace> getAllMappingUnderMerchant(long merchantID) {
        return hibernateTemplate.find("from MappingForGroupServicePlace p where p.serviceGroup.merchantID = ? ", merchantID);
    }

    public List<GroupServicePlaceConfig> getAllPlaceForServiceGroup() {
        return hibernateTemplate.find("from GroupServicePlaceConfig order by configID asc");
    }

    @Override
    public MappingForGroupServicePlace getGroupShowOnStoreUnderMerchant(long merchantID) {
        List<MappingForGroupServicePlace> mappingForGroupServicePlaceList =
                hibernateTemplate.find("from MappingForGroupServicePlace p where p.serviceGroup.merchantID = ? " +
                "and p.groupServicePlaceConfig.configName like ?", new Object[]{
                        merchantID,
                        "%" + businessConfig.getStoreShowPlaceConfigName() + "%"
        });

        if (CollectionUtils.isEmpty(mappingForGroupServicePlaceList)) {
            return null;
        } else {
            return mappingForGroupServicePlaceList.get(0);
        }
    }

    @Override
    public void removeAllMappingForServiceGroup(long serviceGroupID) {
        hibernateTemplate.getSessionFactory().getCurrentSession()
                .createQuery("delete from MappingForGroupServicePlace where serviceGroup.groupID = :groupID")
                .setParameter("groupID", serviceGroupID).executeUpdate();
    }
}
