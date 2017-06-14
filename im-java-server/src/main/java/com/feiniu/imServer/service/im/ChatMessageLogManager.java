package com.feiniu.imServer.service.im;


import com.feiniu.imServer.bean.dto.ChatMessage;
import com.feiniu.imServer.constants.BusinessConfig;
import com.feiniu.imServer.consts.SystemConfig;
import com.feiniu.imServer.repository.ChatMessageDao;
import com.feiniu.imServer.service.userinfo.UserHotInfoService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import javax.annotation.PostConstruct;
import javax.annotation.PreDestroy;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

/**
 * Author: morningking
 * Date: 2016/1/26
 * Contact: 243717042@qq.com
 */
@Service
public class ChatMessageLogManager {
    @Autowired
    private ChatMessageDao chatMessageDao;
    @Autowired
    private BusinessConfig businessConfig;
    @Autowired
    private UserHotInfoService userHotInfoService;

    private BlockingQueue<ChatMessage> chatMessageBlockingQueue;

    private static final Logger LOGGER = LoggerFactory.getLogger(ChatMessageLogManager.class);
    private volatile Thread chatMessageKeeperThread;

    @PostConstruct
    private void init() {
        chatMessageBlockingQueue = new ArrayBlockingQueue<>(1000);

        //开启后台保存线程
        chatMessageKeeperThread = new Thread(new ChatMessageKeeper(), "chat-message-keeper");
        chatMessageKeeperThread.start();
    }

    private class ChatMessageKeeper implements Runnable {
        @Override
        public void run() {
            List<ChatMessage> chatMessageList = new ArrayList<>(1);
            boolean needContinue = true;

            while (needContinue) {
                try {
                    ChatMessage chatMessage = chatMessageBlockingQueue.take();
                    transfer(chatMessage);
                } catch (InterruptedException e) {
                    LOGGER.error("线程工作被打断，准备退出", e);
                    needContinue = false;
                }
            }

            chatMessageBlockingQueue.drainTo(chatMessageList);
        }

        private void transfer(ChatMessage chatMessage) {
            try {
                //商城客服发给客户
                if (chatMessage.getFrom_id() <= SystemConfig.MAX_SERVICE_ID
                        && chatMessage.getTo_id() >= SystemConfig.MAX_SERVICE_ID){
                    LOGGER.info("get chatMessage from service to client :");
                    userHotInfoService.setConsultToReception(
                            Long.valueOf(chatMessage.getTo_id()),
                            Long.valueOf(chatMessage.getFrom_id()));

                    userHotInfoService.putRecentInfo(
                            Long.valueOf(chatMessage.getTo_id()),
                            Long.valueOf(chatMessage.getFrom_id()),
                            chatMessage
                    );
                }else if (chatMessage.getFrom_id() >= SystemConfig.MIN_SELF_SERVICE_ID
                        && chatMessage.getTo_id() >= SystemConfig.MAX_SERVICE_ID){
                    //自营客服发给客户
                    LOGGER.info("get chatMessage from service to client :");

                    userHotInfoService.setConsultToReception(
                            Long.valueOf(chatMessage.getTo_id()),
                            Long.valueOf(chatMessage.getFrom_id()));
                }
                else if(chatMessage.getFrom_id() >= SystemConfig.MAX_SERVICE_ID
                        && chatMessage.getTo_id() <= SystemConfig.MAX_SERVICE_ID){
                    LOGGER.info("get chatMessage from client to service :",chatMessage);
                    //客户发给商城客服
                    userHotInfoService.putRecentInfo(
                            Long.valueOf(chatMessage.getFrom_id()),
                            Long.valueOf(chatMessage.getTo_id()),
                            chatMessage
                    );
                }
                chatMessageDao.saveMessage(chatMessage);
            }catch (Exception e){
                LOGGER.error("聊天消息处理函数异常 :", e);
            }

        }
    }

    public void pushMessage(ChatMessage chatMessage) throws InterruptedException {
        chatMessageBlockingQueue.put(chatMessage);
    }

    @PreDestroy
    private void destroy() {
        chatMessageKeeperThread.interrupt();
    }
}
