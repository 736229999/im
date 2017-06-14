package com.feiniu.fnSubAccount.repository;


import com.feiniu.fnSubAccount.bean.AttendanceDetailInfo;
import com.feiniu.fnSubAccount.bean.AttendanceInfo;

import java.util.List;

/**
 * Created by wangzhen on 2015/7/22.
 */
public interface AttendanceMessageDao {
    List<AttendanceInfo> getAttendanceListByTime(long startTime, long endTime, String merchantId,String subMerchantIdList);

    List<AttendanceDetailInfo> getAttendanceDetailListByTime(long startTime, long endTime, String subMerchantId);

}
