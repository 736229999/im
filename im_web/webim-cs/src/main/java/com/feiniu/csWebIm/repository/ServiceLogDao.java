package com.feiniu.csWebIm.repository;


import com.feiniu.csWebIm.bean.domain.HistoryServiceLog;

import java.util.List;

/**
 * Created by wangzhen on 2015/10/29.
 */
public interface ServiceLogDao {
    int saveServiceLog(String timeInSeconds,String customerUUID,String serviceName,String logDetail);
    List<HistoryServiceLog> getLastTenHistoryLog(String customerUUID);
}
