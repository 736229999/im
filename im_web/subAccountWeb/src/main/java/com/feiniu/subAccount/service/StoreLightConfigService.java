package com.feiniu.subAccount.service;

import com.feiniu.subAccount.component.repository.MappingGroupShowPlaceConfigManager;
import com.feiniu.subAccount.component.repository.ServiceGroupManager;
import com.feiniu.subAccount.component.repository.ServiceGroupMappingManager;
import com.feiniu.subAccount.component.repository.StoreLightConfigManager;
import com.feiniu.subAccount.domain.MappingForGroupServicePlace;
import com.feiniu.subAccount.domain.ServiceGroup;
import com.feiniu.subAccount.domain.ServiceGroupMapping;
import com.feiniu.subAccount.domain.StoreLightConfig;
import com.feiniu.subAccount.exception.BusinessException;
import com.feiniu.subAccount.service.external.WebIMService;
import org.apache.commons.collections4.CollectionUtils;
import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.*;

/**
 * Created by wangchen on 2015/2/4.
 */
@Service
@Transactional
public class StoreLightConfigService {
    @Autowired
    private StoreLightConfigManager serverGroupConfigManager;
    @Autowired
    private ServiceGroupManager serviceGroupManager;
    @Autowired
    private ServiceGroupMappingManager serviceGroupMappingManager;
    @Autowired
    private MappingGroupShowPlaceConfigManager mappingGroupShowPlaceConfigManager;
    @Autowired
    private WebIMService webIMService;
    @Autowired
    PhoneNumberMemcacheService phoneNumberMemcacheService;

    public void updateStoreLightConfig(List<ServiceGroup> serviceGroupList, StoreLightConfig storeLightConfigList) {
        serviceGroupManager.updateServiceGroupList(serviceGroupList);
        serverGroupConfigManager.updateStoreLightConfig(storeLightConfigList);
        phoneNumberMemcacheService.putMerchantConnectInfo(storeLightConfigList.getMerchantID(),storeLightConfigList.getPhone()
                                                        ,storeLightConfigList.getTelphone());
    }

    public StoreLightConfig getStoreConfigUnderMerchant(long merchantID) {
        return serverGroupConfigManager.getStoreLightConfigUnderMerchant(merchantID);
    }

    public void diagnoseStoreLight(long merchantID) {
        MappingForGroupServicePlace mappingForGroupServicePlace =
                mappingGroupShowPlaceConfigManager.getGroupShowOnStoreUnderMerchant(merchantID);
        if (mappingForGroupServicePlace == null) {
            throw new BusinessException("店铺未绑定任何分组");
        }

        if (!webIMService.isMerchantAccountOnline(merchantID)) {
            throw new BusinessException("商家没有客服账号在线");
        }

        List<ServiceGroup> serviceGroupList = serviceGroupManager.getServiceGroupListShowOnStoreUnderMerchant(merchantID);
        if (CollectionUtils.isEmpty(serviceGroupList)) {
            throw new BusinessException("店铺亮灯设置页面未勾选任何分组");
        }

        List<ServiceGroupMapping> diversedServiceGroupMapping =
                serviceGroupMappingManager.getDiversedSubAccountListUnderGroupList(serviceGroupList);
        Map<Long, ServiceGroup> unDiversedServiceGroupMap =
                new HashMap<Long, ServiceGroup>();

        for (ServiceGroup serviceGroup : serviceGroupList) {
            unDiversedServiceGroupMap.put(serviceGroup.getGroupID(), serviceGroup);
        }
        for (ServiceGroupMapping serviceGroupMapping : diversedServiceGroupMapping) {
            long serviceGroupID = serviceGroupMapping.getServiceGroup().getGroupID();
            if (unDiversedServiceGroupMap.containsKey(serviceGroupID)) {
                unDiversedServiceGroupMap.remove(serviceGroupID);
            }
        }

        if (unDiversedServiceGroupMap.size() != 0) {
            List<String> unStatisfiedServiceGroup = new ArrayList<String>();
            for (ServiceGroup serviceGroup : unDiversedServiceGroupMap.values()) {
                unStatisfiedServiceGroup.add(serviceGroup.getGroupName());
            }
            throw new BusinessException(StringUtils.join(unStatisfiedServiceGroup, ",") + " 分组灯不亮-客服分组未添加客服或组内客服未被设置为分流");
        }

        Map<Long, ServiceGroup> subAccountToGroup = new HashMap<Long, ServiceGroup>();
        List<Long> subAccountList = new ArrayList<Long>();
        List<ServiceGroup> onlinedServiceGroupSet = new ArrayList<ServiceGroup>();
        for (ServiceGroupMapping serviceGroupMapping : diversedServiceGroupMapping) {
            subAccountList.add(serviceGroupMapping.getSubAccountID());
            subAccountToGroup.put(serviceGroupMapping.getSubAccountID(), serviceGroupMapping.getServiceGroup());
        }

        Map<Long, Boolean> subAccountListOnlineInfo = webIMService.getOnlineSubAccountList(subAccountList);
        if (subAccountListOnlineInfo != null && subAccountListOnlineInfo.size() > 0) {
            for (Map.Entry<Long, Boolean> entry : subAccountListOnlineInfo.entrySet()) {
                if (entry.getValue()) {
                    onlinedServiceGroupSet.add(subAccountToGroup.get(entry.getKey()));
                }
            }
        }

        serviceGroupList.removeAll(onlinedServiceGroupSet);

        if (!serviceGroupList.isEmpty()) {
            List<String> unStatisfiedServiceGroupNameList = new ArrayList<String>();
            for (ServiceGroup serviceGroup : serviceGroupList) {
                unStatisfiedServiceGroupNameList.add(serviceGroup.getGroupName());
            }
            throw new BusinessException(StringUtils.join(unStatisfiedServiceGroupNameList, ",") + " 分组灯不亮-组内有客服不在线");
        }
    }
}
