package com.feiniu.fnSubAccount.repository;

import com.feiniu.fnSubAccount.bean.SessionInfo.*;
import com.feiniu.fnSubAccount.domain.UserInfo;

import java.util.List;

/**
 * Created by wangzhen on 2016/4/8.
 */
public interface SessionInfoDao {
    Long getCustomerIdByName(String customerName);
    List<CsAccountName> getCsIdListByPartName(String partName);

    List<UserInfo> getCustomerListByIds(String ids);
    List<SessionInfoItem> getSessionInfoList(long startTime,long endTime ,
                                             Long customerId, Long serviceId,
                                             Integer startRow,Integer endRow);
    SessionTotalInfo getSessionInfoCount(long startTime,long endTime ,Long customerId, Long serviceId);

    List<SessionProfileItem> getSessionProfile(Integer type,long startTime,long endTime ,Long customerId, Long serviceId);

    List<SessionCountInfo> getSessionCountInfo(long startTime , long endTime);
}
