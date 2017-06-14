package com.feiniu.subAccount.component.repository;

/**
 * Created by wangchen on 2015/5/27.
 */
public interface GuideConfigManager {
    boolean hasMerchantIDBeingGuided(long merchantID, String version);

    void updateGuideStatus(long merchantID, String version);
}
