package com.feiniu.fnSubAccount.repository;

import com.feiniu.fnSubAccount.bean.WorkMessageRequestParams;
import com.feiniu.fnSubAccount.bean.response.WorkMessageResponse;
import com.feiniu.fnSubAccount.bean.response.WorkMessageTotalInfoResponse;
import com.feiniu.fnSubAccount.domain.MerchantLogInOutInfo;

import java.util.List;
import java.util.Map;

/**
 * Created by wangchen on 2015/2/4.
 */
public interface WorkMessageDao {
    /*
    * 定时任务相关函数
    * */
    int updateStatus(Long currentTime,int statisitcStatus, String ipAddress,String retrytime);

    int insertIntoStatus(Long currentTime,int statisitcStatus, String ipAddress);

    void updateLoginMap(Map<Long,Long> subMerchantIdToMerchantIdMap,long startTime,long endTime);

    int updateImWorkDetail(long merchantId,long statisticsDate,int landingDur);

    Map<String, Object> lockUpdateStatus(Long currentTime);

    List<MerchantLogInOutInfo> getMerchantLogInOut(long startTime,long endTime ,int pageNo, int pageSize,
            List<Long> listUserId);

    Integer getMerchantLogInOutCount(long startTime,long endTime ,List<Long> listUserId);

    /*
    * 数据查询相关函数
    * */
    WorkMessageTotalInfoResponse getWorkTotalResponse(WorkMessageRequestParams workMessageRequestParams);

    List<WorkMessageResponse> getWorkMessageResponseList(WorkMessageRequestParams workMessageRequestParams,long isNeedmerchantId);

    int getMerchantOnlineDur(String subMerchantIds,long startTime,long endTime);



}
