package com.feiniu.subAccount.service;

import com.feiniu.subAccount.component.repository.GuideConfigManager;
import com.feiniu.subAccount.constants.BusinessConfig;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

/**
 * Created by wangchen on 2015/5/27.
 */
@Service
@Transactional
public class GuideConfigService {
    @Autowired
    private GuideConfigManager guideConfigManager;
    @Autowired
    BusinessConfig businessConfig;

    public boolean hasMerchantIDBeingGuided(long merchantID) {
        return guideConfigManager.hasMerchantIDBeingGuided(merchantID, businessConfig.getVersion());
    }

    public void updateGuideStatus(long merchantID) {
        guideConfigManager.updateGuideStatus(merchantID, businessConfig.getVersion());
    }
}
