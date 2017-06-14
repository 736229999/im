package com.feiniu.subAccount.service;

import com.feiniu.subAccount.component.repository.MappingGroupShowPlaceConfigManager;
import com.feiniu.subAccount.domain.GroupServicePlaceConfig;
import com.feiniu.subAccount.domain.MappingForGroupServicePlace;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;

/**
 * Created by wangchen on 2015/2/4.
 */
@Service
@Transactional
public class MappingGroupShowPlaceConfigService {
    @Autowired
    private MappingGroupShowPlaceConfigManager mappingGroupShowPlaceConfigManager;

    public void setGroupPlaceList(List<MappingForGroupServicePlace> mappingForGroupServicePlaceList, Long serviceGroupID) {
        if (serviceGroupID != null) {
            mappingGroupShowPlaceConfigManager.removeAllMappingForServiceGroup(serviceGroupID);
            for (MappingForGroupServicePlace mappingForGroupServicePlace : mappingForGroupServicePlaceList) {
                mappingForGroupServicePlace.setMappingID(0);
            }
        }

        mappingGroupShowPlaceConfigManager.setGroupPlaceList(mappingForGroupServicePlaceList);
    }

    public List<MappingForGroupServicePlace> getAllMappingUnderMerchant(Long merchantID) {
        return mappingGroupShowPlaceConfigManager.getAllMappingUnderMerchant(merchantID);
    }

    public List<GroupServicePlaceConfig> getAllShowPlace() {
        return mappingGroupShowPlaceConfigManager.getAllPlaceForServiceGroup();
    }
}
