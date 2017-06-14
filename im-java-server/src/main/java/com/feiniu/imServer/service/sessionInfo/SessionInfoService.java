package com.feiniu.imServer.service.sessionInfo;

import com.feiniu.imServer.bean.Shunt.SessionInfoItem;
import com.feiniu.imServer.repository.SessionInfoDao;
import com.feiniu.imServer.util.JsonUtil;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import javax.annotation.PostConstruct;
import javax.annotation.PreDestroy;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

/**
 * Created by wangzhen on 2016/4/7.
 */
@Service
public class SessionInfoService {
    private static final Logger LOGGER = LoggerFactory.getLogger(SessionInfoService.class);
    private volatile BlockingQueue<SessionInfoItem> sessionInfoItemBlockingQueue;
    private volatile Thread sendingThread;
    private volatile boolean isContinue = true;

    @Autowired
    SessionInfoDao sessionInfoDao;
    @PostConstruct
    public void initEnv()
    {
        if (sessionInfoItemBlockingQueue == null) {
            sessionInfoItemBlockingQueue= new ArrayBlockingQueue<>(40);
        } else {
            sessionInfoItemBlockingQueue.clear();
        }
        startSending();
    }

    @PreDestroy
    public void stop(){
    }

    public void startSending() {
        sendingThread = new Thread(new Runnable() {
            @Override
            public void run() {
                while (isContinue) {
                    try {
                        SessionInfoItem sessionInfoItem = sessionInfoItemBlockingQueue.take();
                        if (sessionInfoItem.getEventType() == 0){
                            sessionInfoDao.putSessionInfo(sessionInfoItem);
                        }else if (sessionInfoItem.getEventType() == 1){
                            sessionInfoDao.updateSessionInfoEndTime(sessionInfoItem);
                        }else if (sessionInfoItem.getEventType() == 2){
                            sessionInfoDao.transSessionInfo(sessionInfoItem);
                        }else {
                            LOGGER.error("会话事件处理:收到未知的类型");
                        }
                        LOGGER.info("会话事件处理:" + JsonUtil.toJsonString(sessionInfoItem));
                    } catch (InterruptedException e) {
                        LOGGER.warn("会话处理线程读取队列时被打断");
                        throw new RuntimeException();
                    } catch (RuntimeException e) {
                        LOGGER.error("会话处理线程出现异常",e);
                    }
                }
            }
        }, "发送线程");

        sendingThread.start();
        LOGGER.info("初始化session 处理队列成功");
    }

    public void putEventMessage(SessionInfoItem message) {
        try {
            boolean isPutSuccess = sessionInfoItemBlockingQueue.offer(message);

            LOGGER.info("{会话处理线程}会话事件：" + message
                    + "，是否成功放入会话事件处理队列：" + (isPutSuccess ? "是" : "否"));

        } catch (Exception e) {
            LOGGER.error("{会话处理线程}将事件处理数据放入会话处理消息队列失败：" +
                    JsonUtil.toJsonString(message), e);
        }
    }
}
