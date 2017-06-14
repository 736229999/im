package com.feiniu.subAccount.service;

import com.feiniu.subAccount.component.repository.ServiceGroupManager;
import com.feiniu.subAccount.component.repository.ServiceGroupMappingManager;
import com.feiniu.subAccount.domain.ServiceGroup;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;

/**
 * Created by wangchen on 2015/2/4.
 */
@Service
@Transactional
public class ServiceGroupService {
    @Autowired
    private ServiceGroupMappingManager serviceGroupMappingManager;
    @Autowired
    private ServiceGroupManager serviceGroupManager;

    public ServiceGroup addServiceGroup(ServiceGroup serviceGroup) {
        return serviceGroupManager.addServiceGroup(serviceGroup);
    }

    public void removeServiceGroup(ServiceGroup serviceGroup) {
        serviceGroupManager.deleteServiceGroup(serviceGroup);
    }

    public void renameServiceGroup(ServiceGroup serviceGroup) {
        serviceGroupManager.renameServiceGroup(serviceGroup);
    }

    public List<ServiceGroup> getServiceGroupsUnderMerchant(long merchantID) {
        return serviceGroupManager.getServiceGroupsUnderMerchant(merchantID);
    }

    public List<ServiceGroup> getServiceGroupShowOnStore(long merchantID) {
        return serviceGroupManager.getServiceGroupListShowOnStoreUnderMerchant(merchantID);
    }
}
