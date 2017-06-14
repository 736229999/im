package com.feiniu.imServer.command;

import com.feiniu.imServer.bean.ImServer.ImServerEvent;
import com.feiniu.imServer.bean.KVPair;
import com.feiniu.imServer.bean.QueueResponse;
import com.feiniu.imServer.bean.Shunt.QueueItem;
import com.feiniu.imServer.constants.ImServerEventTypeEnum;

/**
 * Created by wangzhen on 2016/2/25.
 */
public class QueueRequestCommand  extends BaseCommand{
    /*
  * 客户申请排队
  * */
    public Long handleCustomerQueueRequestSync(ImServerEvent message) {
        if (message.getGroupId() == null || message.getCustomerId()== null
                || message.getUserLevel() == null || message.getRequestTime() == null){
            LOGGER.error("参数不全，客户申请排队 ");
            return null;
        }
        if (message.getFromDevice() == null) {
            message.setFromDevice(0);
        }
        if (message.getFromPage() == null) {
            message.setFromPage(-1);
        }
        QueueItem queueItem = new QueueItem();
        queueItem.setCustomerId(message.getCustomerId());
        queueItem.setFromDevice(message.getFromDevice());
        queueItem.setFromPage(message.getFromPage());
        queueItem.setWaitingTime(System.currentTimeMillis()/1000);
        queueItem.setCustomerLevel(message.getUserLevel());
        Long rank = shuntService.addToQueue(queueItem, message.getGroupId(),
                message.getUserLevel(), System.currentTimeMillis()/1000);

        return rank;
    }

    private Integer CalcQueueLength(int QueueOrigin) {
        if (QueueOrigin == 0)
            return -1;

        if (QueueOrigin < (4* 4)) {
            return (--QueueOrigin / 4) + 1;
        } else {
            return QueueOrigin - (4 * 4) + 4;
        }
    }

    @Override
    public KVPair<Boolean, Object> execute(ImServerEvent message) {
        KVPair<Boolean,Object> resultPair = new KVPair<>();
        resultPair.setKey(true);
        resultPair.setValue(null);

        Long queueLength =handleCustomerQueueRequestSync(message);
        if (queueLength< 0){
            resultPair.setKey(false);
            resultPair.setValue(queueLength);
        }else {
            QueueResponse queueResponse = new QueueResponse();
            queueResponse.setQueueLength(CalcQueueLength(queueLength.intValue()));
            resultPair.setValue(queueResponse);
        }

        return resultPair;
    }

    @Override
    public ImServerEventTypeEnum getEventType() {
        return ImServerEventTypeEnum.EVENT_QUEUE_REQUESET_BY_CUSTOMER;
    }
}
