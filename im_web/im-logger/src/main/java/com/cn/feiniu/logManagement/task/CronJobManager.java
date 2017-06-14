package com.cn.feiniu.logManagement.task;

import com.cn.feiniu.logManagement.util.general.FormatUtils;
import org.apache.commons.lang.time.DateUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Component;

import java.util.Date;
import java.util.HashMap;
import java.util.Map;

/**
 * Author: morningking
 * Date: 2015/8/6
 * Contact: 243717042@qq.com
 */
@Component
public class CronJobManager {
    @Autowired
    private SubMerchantActionStatisticsTask subMerchantActionStatisticsTask;
    private static final Logger LOGGER = LoggerFactory.getLogger(CronJobManager.class);

    @Scheduled(cron = "0 05 00 * * ?")
    public void jobSubMerchantActionStatistics() {
        Date toStatisticsDate = DateUtils.addDays(new Date(), -1);
        Map<String, Object> params = new HashMap<>(1);
        params.put("date", toStatisticsDate);

        LOGGER.info("[Run-Batch-Start]开始进行跑批，此次跑批的数据来自日期：" + FormatUtils.formatDate(toStatisticsDate));

        try {
            subMerchantActionStatisticsTask.runTask(params);
        } catch (Exception e) {
            LOGGER.error("跑批发生失败", e);
        }
    }
}
