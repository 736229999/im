package com.feiniu.imServer.command;

import com.feiniu.imServer.bean.ImServer.ImServerEvent;
import com.feiniu.imServer.bean.KVPair;
import com.feiniu.imServer.constants.ImServerEventTypeEnum;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by wangzhen on 2016/2/25.
 */
public class ServiceOnlineCommand  extends  BaseCommand{
    /*
    * 客服上线
    * */
    public Map<Long,Long> handleServerOnlineEventSync(ImServerEvent message) {
        if (message.getServiceId() == null){
            LOGGER.error("参数不全，客服上线处理 " );
            return null;
        }
        Map<Long,Long> resultMap = new HashMap<>();
//        imService.setUserOnlineStatus(message.getServiceId(), 1);
        //用户异常下线可能导致redis脏数据
        kefuQueueInfoService.remAllRecptionRelationByServiceId(message.getServiceId(), false);

        List<Long> groupIdList = csInfoDao.getAllGroupsOfCsId(message.getServiceId().toString());

        if (groupIdList == null) {
            return null;
        }
        for (Long groupIdIndex : groupIdList) {
            shuntService.startShuntTask(groupIdIndex);
            resultMap.put(groupIdIndex,kefuQueueInfoService.getQueueNumberInGroupId(groupIdIndex));
        }

        return resultMap;
    }

    @Override
    public KVPair<Boolean, Object> execute(ImServerEvent message) {
        KVPair<Boolean,Object> resultPair = new KVPair<>();
        resultPair.setKey(true);

        Map<Long,Long> queueLengthMap = handleServerOnlineEventSync(message);
        if ( queueLengthMap == null){
            resultPair.setKey(false);
            resultPair.setValue("处理失败，参数不全");
        }
        resultPair.setValue(queueLengthMap);
        return resultPair;
    }

    @Override
    public ImServerEventTypeEnum getEventType() {
        return ImServerEventTypeEnum.EVENT_SERVICE_ON_LINE;
    }
}
