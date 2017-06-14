package com.feiniu.imServer.service.external.kafka;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.web.context.ServletContextAware;
import org.zeromq.ZMQ;

import javax.servlet.ServletContext;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.TimeUnit;

/**
 * Created by zhangmin on 2016/2/23.
 */
public class ZmqPubService implements ServletContextAware {

    private static final Logger LOGGER = LoggerFactory.getLogger(ZmqPubService.class);
    private volatile ZMQ.Context context;
    private volatile ZMQ.Socket socket ;

    private volatile Thread sendingThread;
    private volatile boolean isContinue = true;
    private volatile BlockingQueue<byte []> zmqPubSendingQueue;


    public static final ZmqPubService instance = new ZmqPubService();
    private  ServletContext servletContext;

    public void initEnv()
    {
        context = ZMQ.context(1);
        socket = context.socket(ZMQ.PUB);
        socket.bind("tcp://*:5561");
        if (zmqPubSendingQueue == null) {
            zmqPubSendingQueue= new ArrayBlockingQueue<>(20);
        } else {
            zmqPubSendingQueue.clear();
        }
    }

    public void stop(){
        socket.close();
        context.term();
    }
    public void startSending() {
        sendingThread = new Thread(new Runnable() {
            @Override
            public void run() {
                while (isContinue) {
                    try {
                        byte[] zmqSendBuf = zmqPubSendingQueue.take();

                        if (!socket.send(zmqSendBuf))
                            LOGGER.error("发送zmq 消息失败");
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
        LOGGER.info("初始化发送者线程成功");
    }


    public boolean sendMessage(byte[] bytes) {
        boolean isPutSuccess = false;
        int retryTime = 0;

        while (!isPutSuccess && retryTime < 2) {
            try {
                LOGGER.info("线程[" + Thread.currentThread().getName() + "] 第" + retryTime + "次尝试将消息放入消息队列");
                isPutSuccess = zmqPubSendingQueue.offer(bytes, 1000, TimeUnit.MILLISECONDS);
                retryTime++;
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
        }

        if (isPutSuccess) {
            LOGGER.info("线程[" + Thread.currentThread().getName() + "] 在第" + retryTime + "次成功将消息放入消息队列");
        } else {
            LOGGER.info("线程[" + Thread.currentThread().getName() + "] 将消息放入消息队列失败");
        }

        return isPutSuccess;
    }

    @Override
    public void setServletContext(ServletContext servletContext) {
        this.servletContext = servletContext;
    }
}
