package com.feiniu.fnSubAccount.task;

import com.feiniu.fnSubAccount.service.FAQCategoryService;
import com.feiniu.fnSubAccount.service.FAQService;
import com.feiniu.fnSubAccount.service.MonitorService;
import org.apache.commons.lang.time.DateUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Component;

import java.util.Calendar;
import java.util.Date;

@Component("taskJob")
public class TimeBuildTask {
    @Autowired
    private MonitorService monitorService;
    @Autowired
    private FAQService faqService;
    @Autowired
    private FAQCategoryService faqCategoryService;

    @Scheduled(cron = "0 00 06 * * ?")
    public void jobInsertBuildStatus() {
        monitorService.insertBuildStatus();
    }

    @Scheduled(cron = "0 59 23 * * ?")
    public void jobUpdateDataBaseBuild() {
        monitorService.updateDataBaseBuild();
    }

    @Scheduled(cron = "0 04 02 * * ?")
    public void buildFaqInfo() {
        long currentTime = DateUtils.truncate(new Date(), Calendar.DATE).getTime();
        faqService.updateAllBrowseInfo(currentTime);
        faqCategoryService.updateAllConsultInfo(currentTime);
    }
}

