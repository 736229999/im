package com.feiniu.subAccount.repository;

import com.feiniu.subAccount.bean.response.AttendanceDetailInfo;
import com.feiniu.subAccount.bean.response.AttendanceInfo;

import java.util.List;

/**
 * Created by wangzhen on 2015/7/22.
 */
public interface AttendanceMessageDao {
    List<AttendanceInfo> getAttendanceListByTime(long startTime, long endTime, String merchantId);

    List<AttendanceDetailInfo> getAttendanceDetailListByTime(long startTime, long endTime, String subMerchantId);

}
