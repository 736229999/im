package com.feiniu.subAccount.component.repository;

import com.feiniu.subAccount.domain.DiverseType;
import com.feiniu.subAccount.domain.MerchantDiverseConfig;

import java.util.List;

/**
 * Created by wangchen on 2015/2/3.
 * 商家分流配置
 */

public interface MerchantDiverseConfigManager {
    public List<DiverseType> getDiverseTypeList();

    public void setDiverseConfigForMerchant(MerchantDiverseConfig merchantDiverseConfig);

    public MerchantDiverseConfig getDiverseConfigForMerchant(long merchantID);
}
