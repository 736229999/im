package com.cn.feiniu.logManagement.task;

import com.cn.feiniu.logManagement.bean.dto.SubMerchantAttendanceDetail;
import com.cn.feiniu.logManagement.service.internal.SubMerchantActionStatisticsService;
import org.apache.commons.lang.time.DateUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import java.util.Calendar;
import java.util.Date;
import java.util.List;
import java.util.Map;

/**
 * Author: morningking
 * Date: 2015/7/30
 * Contact: 243717042@qq.com
 */
@Component
public class SubMerchantActionStatisticsTask implements StatisticsTask {
    @Autowired
    private SubMerchantActionStatisticsService subMerchantActionStatisticsService;
    private static final Logger LOGGER = LoggerFactory.getLogger(SubMerchantActionStatisticsTask.class);

    @Override
    public void runTask(Map<String, Object> params) throws Exception {
        Date param = (Date) params.get("date");
        Date statisticsDate = DateUtils.truncate(param, Calendar.DATE);

        List<SubMerchantAttendanceDetail> subMerchantAttendanceDetailList =
                subMerchantActionStatisticsService.countFromLog(statisticsDate);
        subMerchantActionStatisticsService.persistStatistics(subMerchantAttendanceDetailList);

        LOGGER.info("[Run-Batch-End]本次跑批共入库记录数：" + subMerchantAttendanceDetailList.size());
    }
}
