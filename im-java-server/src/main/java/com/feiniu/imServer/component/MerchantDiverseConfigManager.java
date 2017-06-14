package com.feiniu.imServer.component;


import com.feiniu.imServer.domain.MerchantDiverseConfig;



public interface MerchantDiverseConfigManager {

    public void setDiverseConfigForMerchant(MerchantDiverseConfig merchantDiverseConfig);

    public MerchantDiverseConfig getDiverseConfigForMerchant(long merchantID);
}
