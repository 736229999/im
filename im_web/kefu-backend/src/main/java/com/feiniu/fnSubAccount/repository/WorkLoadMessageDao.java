package com.feiniu.fnSubAccount.repository;


import com.feiniu.fnSubAccount.bean.ReceptionInfo;
import com.feiniu.fnSubAccount.bean.WorkLoadDetailInfo;
import com.feiniu.fnSubAccount.bean.WorkLoadInfo;

import java.util.List;

/**
 * Created by wangzhen on 2015/7/22.
 */
public interface WorkLoadMessageDao {
    List<WorkLoadInfo> getWorkLoadListByTime(long startTime, long endTime, String merchantId,String subMerchantIDlist);


    List<WorkLoadDetailInfo> getWorkLoadDetailByTime(long startTime, long endTime, String subMerchantId);

    WorkLoadInfo getTotalWorkLoadListByTime(long startTime, long endTime, String merchantId);
    WorkLoadDetailInfo getTotalWorkLoadListDetailByTime(long startTime, long endTime, String submerchantId);

    int updateReplyDetail(long startTime, long endTime);

    int updateWorkLoadList(long startTime, long endTime, String subMerchantIDs, String merchantIDs, long statisticsDate);

    List<ReceptionInfo> getReceptionInfo(String subMerchantIDs, long currentTime);

    int setReceptionTime(long receptionTime, long currentTime, long subMerchantId);
}
