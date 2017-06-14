package com.feiniu.imServer.command;

import com.feiniu.imServer.bean.ImServer.ImServerEvent;
import com.feiniu.imServer.bean.KVPair;
import com.feiniu.imServer.constants.ImServerEventTypeEnum;
import com.feiniu.imServer.repository.CsInfoDao;
import com.feiniu.imServer.service.im.ImService;
import com.feiniu.imServer.service.shunt.KefuQueueInfoService;
import com.feiniu.imServer.service.shunt.ShuntService;
import com.feiniu.imServer.util.JsonUtil;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;

import javax.annotation.PostConstruct;
import java.util.HashMap;
import java.util.Map;

/**
 * Created by wangzhen on 2016/2/25.
 */
public abstract class BaseCommand {
    protected static final Logger LOGGER = LoggerFactory.getLogger(BaseCommand.class);

    @Autowired
    protected ShuntService shuntService;
    @Autowired
    protected ImService imService;
    @Autowired
    protected KefuQueueInfoService kefuQueueInfoService;
    @Autowired
    CsInfoDao csInfoDao;

    private static final Map<ImServerEventTypeEnum, BaseCommand> MAP = new HashMap<>();

    @PostConstruct
    private void init() {
        MAP.put(getEventType(), this);
    }

    public abstract KVPair<Boolean, Object> execute(ImServerEvent message);

    public abstract ImServerEventTypeEnum getEventType();

    public static final KVPair<Boolean, Object> invoke(ImServerEventTypeEnum eventType, ImServerEvent event) {
        BaseCommand baseCommand = MAP.get(eventType);

        if (baseCommand != null) {
            LOGGER.info("收到消息： " + eventType.getEventInfo() );
            LOGGER.info("处理消息: "+ JsonUtil.toJsonString(event));
            KVPair<Boolean, Object> result = baseCommand.execute(event);
            LOGGER.info("处理消息： " + eventType.getEventInfo() +
                    "结果 :"+ result.getKey().toString()  );
            return result;
        } else {
            LOGGER.info("收到不识别的消息 " +eventType.ordinal() );
            return null;
        }
    }
}
