package com.feiniu.subAccount.task;

import com.feiniu.subAccount.service.StatisticsService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Component;

/**
 * Created by wangzhen on 2015/7/22.
 */
@Component("taskJob")
public class TimeBuildTask {
    @Autowired
    private StatisticsService statisticsService;

    @Scheduled(cron = "0 01 6 * * ?")
    public void jobInsertBuildStatus() {
        statisticsService.insertBuildStatus();
    }
    @Scheduled(cron = "0 55 23 * * ?")
    public void jobUpdateDataBaseBuild() {
        statisticsService.updateDataBaseBuild();
    }
}
