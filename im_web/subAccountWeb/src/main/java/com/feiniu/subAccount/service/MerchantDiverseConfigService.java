package com.feiniu.subAccount.service;

import com.feiniu.subAccount.bean.request.DiverseConfigRequest;
import com.feiniu.subAccount.component.repository.MerchantDiverseConfigManager;
import com.feiniu.subAccount.domain.DiverseType;
import com.feiniu.subAccount.domain.MerchantDiverseConfig;
import com.feiniu.subAccount.service.external.ImApiService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;

/**
 * Created by wangchen on 2015/2/4.
 */
@Service
public class MerchantDiverseConfigService {
    @Autowired
    private MerchantDiverseConfigManager merchantDiverseConfigManager;
    @Autowired
    private ImApiService imApiService;

    @Transactional
    public List<DiverseType> getDiverseTypeList() {
        return merchantDiverseConfigManager.getDiverseTypeList();
    }

    @Transactional
    public MerchantDiverseConfig getDiverseConfigForMerchant(Long merchantID) {
        return merchantDiverseConfigManager.getDiverseConfigForMerchant(merchantID);
    }

    @Transactional
    public void setDiverseConfigForMerchant(DiverseConfigRequest diverseConfigRequest) {
        MerchantDiverseConfig merchantDiverseConfig = new MerchantDiverseConfig();
        DiverseType diverseType = new DiverseType();

        merchantDiverseConfig.setMerchantID(diverseConfigRequest.getMerchantID());
        merchantDiverseConfig.setProxySubAccountID(diverseConfigRequest.getSubMerchantID());
        diverseType.setDiverseTypeID(diverseConfigRequest.getDiverseType());
        merchantDiverseConfig.setDiverseType(diverseType);

        imApiService.modifyMerchantShuntInfoInRedis(diverseConfigRequest.getMerchantID(),
                diverseConfigRequest.getDiverseType(),diverseConfigRequest.getSubMerchantID());

        merchantDiverseConfigManager.setDiverseConfigForMerchant(merchantDiverseConfig);
    }
}
