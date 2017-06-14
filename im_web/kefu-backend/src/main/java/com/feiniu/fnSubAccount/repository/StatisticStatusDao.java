package com.feiniu.fnSubAccount.repository;

import java.util.Map;

/**
 * Created by wangzhen on 2015/7/22.
 */
public interface StatisticStatusDao {
    int insertIntoStatus(long currentTime, int statisitcStatus, String ipAddress);

    int updateStatus(long currentTime, int statisitcStatus, String ipAddress, String retrytime);

    Map<String, Object> lockUpdateStatus(Long currentTime);

    void clearAll();
}
