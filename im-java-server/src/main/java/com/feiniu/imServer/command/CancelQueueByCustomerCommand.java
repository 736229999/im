package com.feiniu.imServer.command;

import com.feiniu.imServer.bean.ImServer.ImServerEvent;
import com.feiniu.imServer.bean.KVPair;
import com.feiniu.imServer.constants.ImServerEventTypeEnum;

/**
 * Created by wangzhen on 2016/2/25.
 */
public class CancelQueueByCustomerCommand extends BaseCommand {

    /*
   * 客户取消排队
   * */
    private boolean handleCancelWhenQueueEventSync(ImServerEvent message) {

        if (message.getGroupId() == null || message.getCustomerId() == null) {
            LOGGER.error("参数不全，客户取消排队处理 ");
            return false;
        }
        kefuQueueInfoService.removeMembernInQueue(message.getGroupId(), message.getCustomerId());

        shuntService.closeRecption(message.getServiceId(), message.getCustomerId(),true);

        if (message.getServiceId() != null && ! message.getServiceId().equals(Long.valueOf(0)))
            imService.nofityRecptionClosedByCustomer(
                    message.getCustomerId(),message.getServiceId(),message.getLoginIP());

        //通知imserver取消排队导致排队信息变化
        imService.notifyShuntDoneMessage(message.getGroupId(),
                kefuQueueInfoService.getQueueNumberInGroupId(message.getGroupId()));

        return true;
    }

    @Override
    public KVPair<Boolean, Object> execute(ImServerEvent message) {
        KVPair<Boolean, Object> resultPair = new KVPair<>();
        resultPair.setKey(true);
        resultPair.setValue(null);

        if (!handleCancelWhenQueueEventSync(message)) {
            resultPair.setKey(false);
            resultPair.setValue("处理失败，参数不全");
        }
        return resultPair;
    }

    @Override
    public ImServerEventTypeEnum getEventType() {
        return ImServerEventTypeEnum.EVENT_CANCEL_QUEUE_BY_CUSTOMER;
    }
}
