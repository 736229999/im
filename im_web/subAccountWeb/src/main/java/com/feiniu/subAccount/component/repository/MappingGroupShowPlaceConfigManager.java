package com.feiniu.subAccount.component.repository;

import com.feiniu.subAccount.domain.GroupServicePlaceConfig;
import com.feiniu.subAccount.domain.MappingForGroupServicePlace;

import java.util.List;

/**
 * Created by wangchen on 2015/2/3.
 * 设置客服分组服务地点
 */
public interface MappingGroupShowPlaceConfigManager {
    void setGroupPlaceList(List<MappingForGroupServicePlace> mappingForGroupServicePlace);

    List<MappingForGroupServicePlace> getAllMappingUnderMerchant(long merchantID);

    List<GroupServicePlaceConfig> getAllPlaceForServiceGroup();

    MappingForGroupServicePlace getGroupShowOnStoreUnderMerchant(long merchantID);

    void removeAllMappingForServiceGroup(long serviceGroupID);
}
