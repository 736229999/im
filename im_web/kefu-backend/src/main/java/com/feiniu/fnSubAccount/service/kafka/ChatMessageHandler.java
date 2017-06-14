package com.feiniu.fnSubAccount.service.kafka;

import com.feiniu.fnSubAccount.controller.websocket.ChatSessionManager;
import com.feiniu.fnSubAccount.util.json.JsonNodeUtil;
import com.feiniu.fnSubAccount.util.json.JsonUtil;
import com.feiniu.kafka.client.KafkaStreamHandler;
import com.feiniu.kafka.client.exception.KafkaClientException;
import kafka.consumer.ConsumerIterator;
import org.codehaus.jackson.JsonNode;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;

import java.util.Arrays;

/**
 * Author: morningking
 * Date: 2016/4/11 15:14
 * Contact: 243717042@qq.com
 */
public class ChatMessageHandler implements KafkaStreamHandler {
    @Autowired
    private ChatSessionManager chatSessionManager;
    private static final Logger LOGGER = LoggerFactory.getLogger(ChatMessageHandler.class);

    @Override
    public void handle(kafka.consumer.KafkaStream<byte[], byte[]> kafkaStream) throws KafkaClientException {
        ConsumerIterator<byte[], byte[]> it = kafkaStream.iterator();

        while (it.hasNext()) {
            try {
                String queuedJsonStr = new String(it.next().message());
                LOGGER.info("get chatMessage :" + queuedJsonStr);
                JsonNode messageNode = JsonUtil.toNode(queuedJsonStr);

                if (messageNode != null) {
                    long fromID = JsonNodeUtil.getLong(messageNode, "from_id");
                    long toID = JsonNodeUtil.getLong(messageNode, "to_id");

                    chatSessionManager.sendMessageToUser(Arrays.asList(fromID, toID), queuedJsonStr);
                }
            } catch (Exception e) {
                LOGGER.error("转发聊天消息失败", e);
            }
        }
    }
}
