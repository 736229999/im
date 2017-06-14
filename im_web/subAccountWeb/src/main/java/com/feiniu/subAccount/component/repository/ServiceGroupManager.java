package com.feiniu.subAccount.component.repository;

import com.feiniu.subAccount.domain.ServiceGroup;

import java.util.List;

/**
 * Created by wangchen on 2015/2/3.
 */
public interface ServiceGroupManager {
    public ServiceGroup addServiceGroup(ServiceGroup serviceGroup);

    public void deleteServiceGroup(ServiceGroup serviceGroup);

    public void renameServiceGroup(ServiceGroup serviceGroup);

    public List<ServiceGroup> getServiceGroupsUnderMerchant(long merchantID);

    public List<ServiceGroup> getServiceGroupListShowOnStoreUnderMerchant(long merchantID);

    public void updateServiceGroupList(List<ServiceGroup> serviceGroupList);
}
