package com.feiniu.imServer.command;

import com.feiniu.imServer.bean.ImServer.ImServerEvent;
import com.feiniu.imServer.bean.KVPair;
import com.feiniu.imServer.constants.ImServerEventTypeEnum;

/**
 * Created by wangzhen on 2016/2/25.
 */
public class ServiceOfflineCommand extends  BaseCommand{
    /*
   * 客服下线处理函数
   * */
    public boolean handleServerOffLineEventSync(ImServerEvent message) {
        if (message.getServiceId() == null){
            LOGGER.error("参数不全，客服下线处理，没有设置客服ID");
            return false;
        }

        boolean resultBooolen = kefuQueueInfoService.remAllRecptionRelationByServiceId(message.getServiceId(), true);

        return resultBooolen;
    }

    @Override
    public KVPair<Boolean, Object> execute(ImServerEvent message) {
        KVPair<Boolean,Object> resultPair = new KVPair<>();
        resultPair.setKey(true);
        resultPair.setValue(null);

        if (!handleServerOffLineEventSync(message)){
            resultPair.setKey(false);
            resultPair.setValue("处理失败，参数不全");
        }
        return resultPair;
    }

    @Override
    public ImServerEventTypeEnum getEventType() {
        return ImServerEventTypeEnum.EVENT_SERVICE_OFF_LINE;
    }
}
