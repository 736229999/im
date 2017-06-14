package com.feiniu.subAccount.service;

import com.feiniu.subAccount.component.repository.ServiceGroupMappingManager;
import com.feiniu.subAccount.domain.ServiceGroup;
import com.feiniu.subAccount.domain.ServiceGroupMapping;
import org.apache.commons.collections4.CollectionUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by wangchen on 2015/2/4.
 */
@Service
@Transactional
public class ServiceGroupMappingService {
    @Autowired
    private ServiceGroupMappingManager serviceGroupMappingManager;

    public void addSubAccountToGroup(ServiceGroupMapping serviceGroupMapping) {
        serviceGroupMappingManager.addSubAccountToGroup(serviceGroupMapping);
    }

    public void addSubAccountListToGroup(List<ServiceGroupMapping> serviceGroupMappingList) {
        serviceGroupMappingManager.addSubAccountListToGroup(serviceGroupMappingList);
    }

    public void removeSubAccountListFromGroup(List<Long> subAccountIDList) {
        serviceGroupMappingManager.removeSubAccountFromGroup(subAccountIDList);
    }

    public void transferSubAccountListToGroup(List<Long> subAccountIDList, ServiceGroup destServiceGroup) {
        serviceGroupMappingManager.transferSubAccountToGroup(subAccountIDList, destServiceGroup);
    }

    public void updateDiverseStateForSubAccount(ServiceGroupMapping serviceGroupMapping) {
        serviceGroupMappingManager.updateDiverseStateForSubAccount(serviceGroupMapping);
    }

    public List<ServiceGroupMapping> getSubAccountListUnderGroup(long groupID) {
        return serviceGroupMappingManager.getSubAccountListUnderGroup(groupID);
    }

    public List<ServiceGroupMapping> getCategorizedSubAccountList(long merchantID) {
        return serviceGroupMappingManager.getCategorizedSubAccounts(merchantID);
    }

    public Map<Long, List<Long>> getSubaccountsUnderGroups(List<Long> groupIDList) {
        Map<Long, List<Long>> subAccountsAndGroupMap = new HashMap<Long, List<Long>>();

        if (CollectionUtils.isNotEmpty(groupIDList)) {
            for (Long groupID : groupIDList) {
                subAccountsAndGroupMap.put(groupID, new ArrayList<Long>());
            }
        }
        List<ServiceGroupMapping> serviceGroupMappingList =
                serviceGroupMappingManager.getSubaccountsUnderGroups(groupIDList);
        if (CollectionUtils.isNotEmpty(serviceGroupMappingList)) {
            for (ServiceGroupMapping serviceGroupMapping : serviceGroupMappingList) {
                subAccountsAndGroupMap.get(serviceGroupMapping.getServiceGroup().getGroupID())
                        .add(serviceGroupMapping.getSubAccountID());
            }
        }

        return subAccountsAndGroupMap;
    }
}
