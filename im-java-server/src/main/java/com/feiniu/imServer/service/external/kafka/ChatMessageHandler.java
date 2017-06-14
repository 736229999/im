package com.feiniu.imServer.service.external.kafka;


import com.feiniu.imServer.bean.dto.ChatMessage;
import com.feiniu.imServer.service.im.ChatMessageLogManager;
import com.feiniu.imServer.util.JsonUtil;
import com.feiniu.kafka.client.KafkaStreamHandler;
import com.feiniu.kafka.client.exception.KafkaClientException;
import kafka.consumer.ConsumerIterator;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;

/**
 * Author: morningking
 * Date: 2016/1/22
 * Contact: 243717042@qq.com
 */
public class ChatMessageHandler implements KafkaStreamHandler {

    @Autowired
    ChatMessageLogManager chatMessageLogManager;
    private static final Logger LOGGER = LoggerFactory.getLogger(ChatMessageHandler.class);

    @Override
    public void handle(kafka.consumer.KafkaStream<byte[], byte[]> kafkaStream) throws KafkaClientException {
        ConsumerIterator<byte[], byte[]> it = kafkaStream.iterator();

        while (it.hasNext()) {
            try {
                String queuedJsonStr = new String(it.next().message());
                LOGGER.info("get chatMessage :" +queuedJsonStr);
                ChatMessage chatMessage = JsonUtil.fromJson(queuedJsonStr, ChatMessage.class);
                if (chatMessage != null) {
                    chatMessageLogManager.pushMessage(chatMessage);
                }
            } catch (Exception e) {
                LOGGER.error("处理发生错误", e);
            }
        }
    }
}
