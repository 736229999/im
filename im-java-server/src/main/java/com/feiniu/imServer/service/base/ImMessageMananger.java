package com.feiniu.imServer.service.base;

import com.feiniu.imServer.bean.ImServer.ImServerQueueMessage;
import com.feiniu.imServer.consts.ShuntConfig;
import com.feiniu.imServer.util.JsonUtil;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Service;
import org.zeromq.ZMQ;

import javax.annotation.PostConstruct;
import javax.annotation.PreDestroy;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

/**
 * Created by wangzhen on 2015/12/30.
 */
@Service
@Scope("singleton")
public class ImMessageMananger {
    private static final Logger LOGGER = LoggerFactory.getLogger(ImMessageMananger.class);

    private volatile ZMQ.Context context;
    private volatile ZMQ.Socket publisher ;

    private volatile Thread sendingThread;
    private volatile boolean isContinue = true;
    private volatile BlockingQueue<ImServerQueueMessage> zmqPubSendingQueue;


    @PostConstruct
    public void initEnv()
    {
        context = ZMQ.context(ShuntConfig.QUEUE_ZMQ_THREAD_NUMBER);
        publisher = context.socket(ZMQ.PUB);
        LOGGER.info("初始化zmq===");
        publisher.bind("tcp://*:"+ ShuntConfig.QUEUE_ZMQ_PORT);
        if (zmqPubSendingQueue == null) {
            zmqPubSendingQueue= new ArrayBlockingQueue<>(40);
        } else {
            zmqPubSendingQueue.clear();
        }
        startSending();
    }

    @PreDestroy
    public void stop(){
        if (sendingThread != null) {
            sendingThread.interrupt();
        }

        LOGGER.info("zmq反初始化");
        publisher.close();
        context.term();
    }

    public void startSending() {
        sendingThread = new Thread(new Runnable() {
            @Override
            public void run() {
                while (isContinue) {
                    try {
                        ImServerQueueMessage ImServerQueueMessage = zmqPubSendingQueue.take();
                        if (!publisher.sendMore(ShuntConfig.QUEUE_ZMQ_CHANNEL))
                            LOGGER.error("发送zmq Channel 消息失败");
                        if (!publisher.send( JsonUtil.toJsonString(ImServerQueueMessage)))
                            LOGGER.error("发送zmq 消息失败 " );
                        LOGGER.info("推送zmq消息:" + JsonUtil.toJsonString(ImServerQueueMessage));
                    } catch (InterruptedException e) {
                        LOGGER.warn("发送线程读取队列时被打断");
                        throw new RuntimeException();
                    } catch (RuntimeException e) {
                        LOGGER.error("发送消息出现异常",e);
                    }
                }
            }
        }, "发送线程");

        sendingThread.start();
        LOGGER.info("初始化zmq 分流信息发送线程成功");
    }

    public void putEventMessage(ImServerQueueMessage message) {
        try {
            boolean isPutSuccess = zmqPubSendingQueue.offer(message);

            LOGGER.info("{事件信息发送线程}分流成功消息：" + message
                    + "，是否成功放入分流成功消息队列：" + (isPutSuccess ? "是" : "否"));

        } catch (Exception e) {
            LOGGER.error("{事件信息发送线程}将分流成功消息数据放入转发消息队列失败：" +
                    JsonUtil.toJsonString(message), e);
        }
    }

}
