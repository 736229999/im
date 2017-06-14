package com.cn.feiniu.logManagement.repository;

import com.cn.feiniu.logManagement.bean.dto.SubMerchantAttendanceDetail;

import java.util.List;

/**
 * Created by wangchen on 2015/7/31.
 */
public interface SubMerchantActionStatisticsDao {
    void saveActionStatisticsList(List<SubMerchantAttendanceDetail> subMerchantAttendanceDetailList);
}
