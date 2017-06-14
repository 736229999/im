package com.feiniu.imServer.command;

import com.feiniu.imServer.bean.ImServer.ImServerEvent;
import com.feiniu.imServer.bean.KVPair;
import com.feiniu.imServer.constants.ImServerEventTypeEnum;

/**
 * Created by wangzhen on 2016/2/25.
 */
public class CancelReceptionByCustomerCommand extends BaseCommand {
    /*
 * 客户取消排队
 * */
    private boolean handleCancelWhenReceptionEventSync(ImServerEvent message) {

        if (message.getGroupId() == null || message.getCustomerId()== null){
            LOGGER.error("参数不全，客户取消排队处理 " );
            return false;
        }
        kefuQueueInfoService.removeMembernInQueue(message.getGroupId(), message.getCustomerId());
        KVPair<Long,Long> receptionServiceIds = imService.getRecptionServiceIds(message.getCustomerId());

        if (receptionServiceIds != null ){
            shuntService.closeRecption( receptionServiceIds.getKey(), message.getCustomerId(),false);
        }

        return true;
    }

    @Override
    public KVPair<Boolean, Object> execute(ImServerEvent message) {
        KVPair<Boolean,Object> resultPair = new KVPair<>();
        resultPair.setKey(true);
        resultPair.setValue(null);

        if (!handleCancelWhenReceptionEventSync(message)){
            resultPair.setKey(false);
            resultPair.setValue("处理失败，参数不全");
        }
        return resultPair;
    }

    @Override
    public ImServerEventTypeEnum getEventType() {
        return ImServerEventTypeEnum.EVENT_CANCEL_RECEPTION_BY_CUSTOMER;
    }
}
