package com.feiniu.imServer.command;

import com.feiniu.imServer.bean.ImServer.ImServerEvent;
import com.feiniu.imServer.bean.KVPair;
import com.feiniu.imServer.constants.ImServerEventTypeEnum;
import com.feiniu.imServer.service.shunt.ShuntService;
import org.springframework.beans.factory.annotation.Autowired;

/**
 * Created by wangzhen on 2016/2/25.
 */
public class ServerCloseSessionCommand  extends  BaseCommand{

    @Autowired
    ShuntService shuntService;
    /*
    * 客服关闭窗口，结束接待
    * */
    private boolean handleServerCloseSessionEventSync(ImServerEvent message) {

        if (message.getGroupId() == null || message.getCustomerId()== null || message.getServiceId() == null){
            LOGGER.error("参数不全，客服关闭窗口，结束接待 " );
            return false;
        }

        shuntService.closeRecption( message.getServiceId(), message.getCustomerId(),true);

        return true;
    }

    @Override
    public KVPair<Boolean, Object> execute(ImServerEvent message) {
        KVPair<Boolean,Object> resultPair = new KVPair<>();
        resultPair.setKey(true);
        resultPair.setValue(null);

        if (!handleServerCloseSessionEventSync(message)){
            resultPair.setKey(false);
            resultPair.setValue("处理失败，参数不全");
            return  resultPair;
        }

        return resultPair;
    }

    @Override
    public ImServerEventTypeEnum getEventType() {
        return ImServerEventTypeEnum.EVENT_RECEPTION_END_BY_SERVICE;
    }
}
