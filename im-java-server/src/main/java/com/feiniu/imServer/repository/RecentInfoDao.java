package com.feiniu.imServer.repository;

import com.feiniu.imServer.bean.RecentConnectInfo;

import java.util.List;

/**
 * Created by wangzhen on 2016/2/1.
 */
public interface RecentInfoDao {
    //根据客户ID获取最近联系的客服
    List<RecentConnectInfo> getRecentServiceInfo(long sid, int maxNumber);

    //根据客户ID和商家ID获取最新联系的一个客服ID
    Long getRecentServiceInfoBySubMerchantList(long sid, String subMerchantIdList);

}
