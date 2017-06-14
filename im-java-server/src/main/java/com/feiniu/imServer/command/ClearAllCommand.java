package com.feiniu.imServer.command;

import com.feiniu.imServer.bean.ImServer.ImServerEvent;
import com.feiniu.imServer.bean.KVPair;
import com.feiniu.imServer.constants.ImServerEventTypeEnum;

/**
 * Created by wangzhen on 2016/2/25.
 */
public class ClearAllCommand  extends BaseCommand{
    /*
    * 清空所有数据
    * */
    public boolean  handleClearAllSync() {
        //kefuQueueInfoService.removeAllQueues();
        kefuQueueInfoService.clearAllRecptionRelation();
        kefuQueueInfoService.flushAllGroupMemToRedis();
        return  true;
    }

    @Override
    public KVPair<Boolean, Object> execute(ImServerEvent message) {
        KVPair<Boolean,Object> resultPair = new KVPair<>();
        resultPair.setKey(true);
        resultPair.setValue(null);

        if (!handleClearAllSync()){
            resultPair.setKey(false);
            resultPair.setValue("处理失败，参数不全");
        }
        return resultPair;
    }

    @Override
    public ImServerEventTypeEnum getEventType() {
        return ImServerEventTypeEnum.EVENT_CLEAR_ALL;
    }
}
