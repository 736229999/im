package com.feiniu.subAccount.repository;

import com.feiniu.subAccount.bean.response.ReceptionInfo;
import com.feiniu.subAccount.bean.response.SubMerchantInfo;
import com.feiniu.subAccount.bean.response.WorkLoadDetailInfo;
import com.feiniu.subAccount.bean.response.WorkLoadInfo;

import java.util.List;

/**
 * Created by wangzhen on 2015/7/22.
 */
public interface WorkLoadMessageDao {
    List<WorkLoadInfo> getWorkLoadListByTime(long startTime, long endTime, String merchantId);
    WorkLoadInfo getTotalWorkLoadListByTime(long startTime, long endTime, String merchantId);

    List<WorkLoadDetailInfo> getWorkLoadDetailByTime(long startTime, long endTime, String subMerchantId);
    WorkLoadDetailInfo getTotalWorkLoadListDetailByTime(long startTime, long endTime, String submerchantId);

    List<SubMerchantInfo> getAllDirectorSubMerchantInfoList();

    int updateReplyDetail(long startTime, long endTime);

    int updateWorkLoadList(long startTime, long endTime, String subMerchantIDs, String merchantIDs, long statisticsDate);

    List<ReceptionInfo> getReceptionInfo(String subMerchantIDs, long currentTime);

    int setReceptionTime(long receptionTime, long currentTime, long subMerchantId);
}
