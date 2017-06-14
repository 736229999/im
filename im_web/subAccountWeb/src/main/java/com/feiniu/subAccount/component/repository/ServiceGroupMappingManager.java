package com.feiniu.subAccount.component.repository;

import com.feiniu.subAccount.domain.ServiceGroup;
import com.feiniu.subAccount.domain.ServiceGroupMapping;

import java.util.List;
import java.util.Map;

/**
 * Created by wangchen on 2015/2/3.
 */
public interface ServiceGroupMappingManager {
    public void addSubAccountToGroup(ServiceGroupMapping serviceGroupMapping);

    public void addSubAccountListToGroup(List<ServiceGroupMapping> serviceGroupMappingList);

    public void removeSubAccountFromGroup(List<Long> subAccountIDList);

    public void transferSubAccountToGroup(List<Long> subAccountIDList, ServiceGroup serviceGroup);

    public void updateDiverseStateForSubAccount(ServiceGroupMapping serviceGroupMapping);

    public List<ServiceGroupMapping> getSubAccountListUnderGroup(long groupID);

    public List<ServiceGroupMapping> getDiversedSubAccountListUnderGroupList(List<ServiceGroup> groupList);

    public List<ServiceGroupMapping> getCategorizedSubAccounts(long merchantID);

    public List<ServiceGroupMapping> getSubaccountsUnderGroups(List<Long> groupIDList);
}
