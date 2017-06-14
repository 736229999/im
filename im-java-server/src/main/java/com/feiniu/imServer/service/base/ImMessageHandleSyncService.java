package com.feiniu.imServer.service.base;

import com.feiniu.imServer.bean.ImServer.ImServerEvent;
import com.feiniu.imServer.bean.KVPair;
import com.feiniu.imServer.command.BaseCommand;
import com.feiniu.imServer.constants.BusinessConfig;
import com.feiniu.imServer.constants.ImServerEventTypeEnum;
import com.feiniu.imServer.repository.CsInfoDao;
import com.feiniu.imServer.service.im.ImService;
import com.feiniu.imServer.service.shunt.KefuQueueInfoService;
import com.feiniu.imServer.service.shunt.ShuntService;
import com.fn.cache.client.RedisCacheClient;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

/**
 * Created by wangzhen on 2016/2/22.
 */
@Service
public class ImMessageHandleSyncService {
    private static final Logger LOGGER = LoggerFactory.getLogger(ImMessageHandleSyncService.class);
    @Autowired
    ShuntService shuntService;
    @Autowired
    KefuQueueInfoService kefuQueueInfoService;
    @Autowired
    RedisCacheClient redisCacheClient;
    @Autowired
    BusinessConfig businessConfig;
    @Autowired
    ImService imService;
    @Autowired
    CsInfoDao csInfoDao;

    public KVPair<Boolean,Object> handleImMessageSync(ImServerEvent message) {

        if (ImServerEventTypeEnum.values().length <= message.getEventType()) {
            LOGGER.error("未知的消息：type : " + message.getEventType());
            KVPair<Boolean,Object> resultPair = new KVPair<>();
            String jsonReult = "未知的消息";
            resultPair.setKey(false);
            resultPair.setValue(jsonReult);
            return resultPair;
        }

        ImServerEventTypeEnum eventType = ImServerEventTypeEnum.getImServerEventType(message.getEventType());
        return  BaseCommand.invoke(eventType, message);
    }
}
