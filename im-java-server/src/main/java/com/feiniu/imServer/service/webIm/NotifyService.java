package com.feiniu.imServer.service.webIm;

import com.feiniu.imServer.bean.ImServer.ImServerQueueMessage;
import com.feiniu.imServer.bean.ImServer.ImServerSubAccountInfo;
import com.feiniu.imServer.service.base.ImMessageMananger;
import com.feiniu.imServer.service.shunt.KefuQueueInfoService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by wangzhen on 2016/3/1.
 */
@Service
public class NotifyService {
    private static final Logger LOGGER = LoggerFactory.getLogger(NotifyService.class);

    @Autowired
    KefuQueueInfoService kefuQueueInfoService;

    @Autowired
    ImMessageMananger imMessageMananger;
    /*
  * 通知webim分流信息有变化
  * */
    public void notifyQueueInfoMessage(Long groupId, Long customerId, Long serviceId, Long timeStamp) {
        //拼分流消息
        ImServerQueueMessage ImServerQueueMessage = new ImServerQueueMessage();
        ImServerQueueMessage.setGroupId(groupId);
        ImServerQueueMessage.setTimeStamp(timeStamp);
        ImServerQueueMessage.setLogInfo("排队数据异动，放入到缓冲队列的时间:" + System.currentTimeMillis() + "\n");
        //排队信息
        ImServerQueueMessage.setQueue(kefuQueueInfoService.getQueueInfoListInGroupId(groupId));

        //分流信息
        List<ImServerSubAccountInfo> testSubAccountList = new ArrayList<>();
        if (customerId == null || serviceId == null) {
            //没有分流信息
        } else {
            //分流成功信息
            ImServerSubAccountInfo testSubAccount = kefuQueueInfoService.getSubAccountInfo(customerId, serviceId);
            testSubAccountList.add(testSubAccount);
        }
        ImServerQueueMessage.setSubAccount(testSubAccountList);
        //发送给webim
        imMessageMananger.putEventMessage(ImServerQueueMessage);
    }


    /*
    * 通知有异动事件发生，包括异动处理结果的事件
    * */
//    public void notifyEventMessage(ImServerEvent message, int eventType, String messageInfo) {
//        //拼分流消息
//        ImServerEventQueueMessage eventMessage = new ImServerEventQueueMessage();
//        eventMessage.setGroupId(String.valueOf(message.getGroupId()));
//        eventMessage.setEventType(eventType);
//        eventMessage.setCustomerId(String.valueOf(message.getCustomerId()));
//        eventMessage.setRequestTime(String.valueOf(message.getRequestTime()));
//        eventMessage.setMessage(messageInfo);
//        eventMessage.setLogInfo(message.getLogInfo() +
//                "事件处理完成，放入到Kafak事件发送队列时间:" + System.currentTimeMillis() + "\n");
//        //发送给webim
//        imMessageMananger.putEventMessage(eventMessage);
//    }
}
