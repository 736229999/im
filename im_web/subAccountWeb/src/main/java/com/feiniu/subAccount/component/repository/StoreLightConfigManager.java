package com.feiniu.subAccount.component.repository;

import com.feiniu.subAccount.domain.StoreLightConfig;

/**
 * Created by wangchen on 2015/2/3.
 */
public interface StoreLightConfigManager {
    public void updateStoreLightConfig(StoreLightConfig storeLightConfig);

    public StoreLightConfig getStoreLightConfigUnderMerchant(long merchantID);
}
