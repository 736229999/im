package com.feiniu.imServer.command;

import com.feiniu.imServer.bean.ImServer.ImServerEvent;
import com.feiniu.imServer.bean.KVPair;
import com.feiniu.imServer.constants.ImServerEventTypeEnum;

/**
 * Created by wangzhen on 2016/2/25.
 */
public class CatcherSessionCommand extends BaseCommand{
    /*
       * 客服接手会话申请
       * */
    public boolean handleCustomerCatcherRequestSync(ImServerEvent message) {
        if (message.getGroupId() == null || message.getServiceId()== null ||
                message.getToServiceId() == null || message.getToServiceGroup()== null){
            LOGGER.error("参数不全，客服接手会话申请 " );
            return false;
        }
        if (shuntService.transfer(message.getGroupId(),
                message.getServiceId(),
                message.getCustomerId(),
                message.getToServiceId(),
                message.getToServiceGroup()
        )) {
            return  true;
        } else {
            return false;
        }
    }

    @Override
    public KVPair<Boolean, Object> execute(ImServerEvent message) {
        KVPair<Boolean,Object> resultPair = new KVPair<>();
        resultPair.setKey(true);
        resultPair.setValue(null);

        if (!handleCustomerCatcherRequestSync(message)){
            resultPair.setKey(false);
            resultPair.setValue("处理失败，参数不全");
        }
        return resultPair;
    }

    @Override
    public ImServerEventTypeEnum getEventType() {
        return ImServerEventTypeEnum.EVENT_CATCHER_SESSION_REQUSET;
    }
}
