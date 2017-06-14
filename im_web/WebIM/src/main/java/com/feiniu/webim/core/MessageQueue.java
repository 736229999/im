package com.feiniu.webim.core;

import com.feiniu.webim.bean.response.BaseResponseBean;
import com.feiniu.webim.config.ConfigManager;
import com.feiniu.webim.config.SystemConfig;
import org.apache.log4j.Logger;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.TimeUnit;

/**
 * 阻塞消息队列(仅是对LinkedBlockingQueue的简单封装)
 *
 * @author zb
 */
public class MessageQueue {

    /**
     * 无新消息等待时间
     */
    private static final long CHAT_MESSAGE_MAXWAITTIME_MILLIS = ConfigManager.getLongProperty(SystemConfig.CHAT_MESSAGE_MAXWAITTIME_MILLIS.toString());
    private static final long GENERAL_MESSAGE_MAXWAITTIME_MILLIS = ConfigManager.getLongProperty(SystemConfig.GENERAL_MESSAGE_MAXWAITTIME_MILLIS.toString());
    private static final int MAX_MESSAGE_IN_QUEUE = ConfigManager.getIntProperty(SystemConfig.MAX_MESSAGE_IN_QUEUE.toString());
    private static final Logger LOGGER = Logger.getLogger(MessageQueue.class);

    private ArrayBlockingQueue<BaseResponseBean> queue;

    public MessageQueue() {
        queue = new ArrayBlockingQueue<>(MAX_MESSAGE_IN_QUEUE);
    }

    public boolean add(BaseResponseBean m) {
        return queue.offer(m);
    }

    public BaseResponseBean pollChatMessage() {
        BaseResponseBean m = null;

        try {
            m = queue.poll(CHAT_MESSAGE_MAXWAITTIME_MILLIS, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            LOGGER.warn("MessageQueue,poll异常:", e);
        }

        return m;
    }

    public BaseResponseBean pollGeneralMessage() {
        BaseResponseBean m = null;

        try {
            m = queue.poll(GENERAL_MESSAGE_MAXWAITTIME_MILLIS, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            LOGGER.warn("MessageQueue,poll异常:", e);
        }

        return m;
    }

    /**
     * 获取并清空消息队列中 所有消息
     *
     * @return List
     */
    public List<BaseResponseBean> pollAllChatMessage() {
        List<BaseResponseBean> list = new ArrayList<>(1);

        BaseResponseBean message = pollChatMessage();

        if (message != null) {
            list.add(message);
        }

        queue.drainTo(list);

        return list;
    }

    public List<BaseResponseBean> pollAllHistoryMessage() {
        List<BaseResponseBean> list = new ArrayList<>();

        BaseResponseBean message = pollGeneralMessage();

        if (message != null) {
            list.add(message);
        }

        queue.drainTo(list);

        return list;
    }

    public void destroy() {
        queue.clear();
    }
}