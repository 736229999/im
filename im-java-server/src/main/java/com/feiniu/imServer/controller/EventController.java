package com.feiniu.imServer.controller;

import com.feiniu.imServer.base.ResponseResult;
import com.feiniu.imServer.bean.ImServer.ImServerEvent;
import com.feiniu.imServer.bean.KVPair;
import com.feiniu.imServer.bean.QueueResponse;
import com.feiniu.imServer.bean.ShuntRequestPrincipal;
import com.feiniu.imServer.service.base.ImMessageHandleSyncService;
import com.feiniu.imServer.service.shunt.ShuntPrinciaplService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

/**
 * Created by wangzhen on 2016/1/28.
 */
@Controller
@RequestMapping(value = "event")
public class EventController {
    private static final Logger LOGGER = LoggerFactory.getLogger(EventController.class);


    @Autowired
    ShuntPrinciaplService shuntPrinciaplService;
    @Autowired
    ImMessageHandleSyncService imMessageHandleSyncService;

    //收到http数据异动消息
    @RequestMapping(value = "im-push-event")
    @ResponseBody
    public ResponseResult httpReceiveEvent(ImServerEvent receviedMessage) {
        ResponseResult result = new ResponseResult();
        receviedMessage.setLogInfo(receviedMessage.getLogInfo() +
                "http 收到http数据异动消息的时间: " + System.currentTimeMillis() + "\n");
        LOGGER.info("http convert to class" + receviedMessage.toString());
//        imMessageMananger.receiveImMessage(receviedMessage);
        result.markSuccess();
        return result;
    }

    //收到排队请求，给webim调用，排队申请，屏蔽event_type
    @RequestMapping(value = "queue-request-event-sync")
    @ResponseBody
    public ResponseResult httpAddQueueSync(ShuntRequestPrincipal shuntRequestPrincipal) {
        if (shuntRequestPrincipal.getUserLevel() == null)
            shuntRequestPrincipal.setUserLevel(2);

        if (shuntRequestPrincipal.getType() == 2){
            //2.自营分流
            ImServerEvent imServerEvent = new ImServerEvent();
            imServerEvent.setEventType(6);
            imServerEvent.setGroupId(shuntRequestPrincipal.getGroupId());
            imServerEvent.setCustomerId(shuntRequestPrincipal.getCustomerId());
            imServerEvent.setUserLevel(shuntRequestPrincipal.getUserLevel());
            imServerEvent.setRequestTime(shuntRequestPrincipal.getRequestTime());
            imServerEvent.setFromDevice(shuntRequestPrincipal.getFromDevice());
            imServerEvent.setFromPage(shuntRequestPrincipal.getFromPage());
            return httpReceiveEventSync(imServerEvent);
        }else {
            //1.组内分流，0.全局分流
            if (shuntRequestPrincipal.getType() == 1)
                ;
            else if ((shuntRequestPrincipal.getType() == 0))
                shuntRequestPrincipal.setGroupId(Long.valueOf(0));
            else
                return null;

            return shuntPrinciaplService.shuntRequestPrinciapl(shuntRequestPrincipal);
        }
    }

    //收到http数据异动消息
    @RequestMapping(value = "im-push-event-sync")
    @ResponseBody
    public ResponseResult httpReceiveEventSync(ImServerEvent receviedMessage) {
        ResponseResult result = new ResponseResult();
        receviedMessage.setLogInfo(receviedMessage.getLogInfo() +
                "http 收到http数据异动同步消息的时间: " + System.currentTimeMillis() + "\n");

        KVPair<Boolean,Object> resultPair=imMessageHandleSyncService.handleImMessageSync(receviedMessage);
        result.markSuccess();
        if (!resultPair.getKey()){
            if (resultPair.getValue().equals(Long.valueOf(-1))){
                result.setMessage("申请排队失败，没有客服在线");
                QueueResponse queueResponse = new QueueResponse();
                queueResponse.setQueueLength(-1);
                result.addData("queueResponse", queueResponse);
            }else if (resultPair.getValue().equals(Long.valueOf(-2))){
                result.setMessage("申请排队失败，该客服已经在被接待");
                QueueResponse queueResponse = new QueueResponse();
                queueResponse.setQueueLength(-2);
                result.addData("queueResponse", queueResponse);
            }else if (resultPair.getValue().equals(Long.valueOf(-3))){
                result.setMessage("排队过程中，客户取消了排队");
                QueueResponse queueResponse = new QueueResponse();
                queueResponse.setQueueLength(-3);
                result.addData("queueResponse", queueResponse);
                return result;
            }else{
                result.setMessage(resultPair.getValue().toString());
            }
            return  result;
        }else {
            result.addData("queueResponse", resultPair.getValue());
        }

        return result;
    }

}
