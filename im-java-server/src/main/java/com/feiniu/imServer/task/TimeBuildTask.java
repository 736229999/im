package com.feiniu.imServer.task;

import com.feiniu.imServer.bean.Shunt.CurrentSessionItem;
import com.feiniu.imServer.repository.SessionInfoDao;
import com.feiniu.imServer.service.im.ImService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Component;

import java.util.List;

@Component("taskJob")
public class TimeBuildTask {
    @Autowired
    SessionInfoDao sessionInfoDao;
    @Autowired
    ImService imService;

    private static final Logger LOGGER = LoggerFactory.getLogger(TimeBuildTask.class);

    @Scheduled(cron = "00 30,00 * * * ?")
    public void jobInsertBuildStatus() {
        LOGGER.info("定时任务：处理异常的会话");
        List<CurrentSessionItem> currentSessions =sessionInfoDao.getCurrentSessionItemList();
        if (currentSessions == null)
            return;

        for(CurrentSessionItem currentSessionItem:currentSessions){
            Long customerId =  currentSessionItem.getCustomerId().longValue();
            Long serviceId = currentSessionItem.getServiceId().longValue();

            if (!imService.getCustomerOnlineStats(customerId)){
                LOGGER.info("定时任务：检查到一个异常会话 : " +
                        "sessionId:"+currentSessionItem.getSessionId().toString() +
                        "customerId:"+currentSessionItem.getCustomerId().toString() +
                        "serviceId :"+ currentSessionItem.getServiceId().toString()
                );

                imService.removeReceptionRelationIfRelated(serviceId, customerId);
            }
        }
    }

}

