package com.cn.feiniu.logManagement.core;

import com.cn.feiniu.logManagement.bean.base.BusinessConfig;
import com.cn.feiniu.logManagement.bean.dto.ChatMessage;
import com.cn.feiniu.logManagement.repository.ChatMessageDao;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;

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
public class ChatMessageLogManager {
    @Autowired
    private ChatMessageDao chatMessageDao;
    @Autowired
    private BusinessConfig businessConfig;
    private BlockingQueue<ChatMessage> chatMessageBlockingQueue;

    private static final Logger LOGGER = LoggerFactory.getLogger(ChatMessageLogManager.class);
    private volatile Thread chatMessageKeeperThread;

    @PostConstruct
    private void init() {
        chatMessageBlockingQueue = new ArrayBlockingQueue<>(businessConfig.getMaxChatMessagesInQueue());

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
                    chatMessageList.add(chatMessage);
                } catch (InterruptedException e) {
                    LOGGER.error("线程工作被打断，准备退出", e);
                    needContinue = false;
                }
            }

            chatMessageBlockingQueue.drainTo(chatMessageList);
        }

        private void transfer(List<ChatMessage> chatMessageList) {
            chatMessageDao.saveMessageList(chatMessageList);
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
