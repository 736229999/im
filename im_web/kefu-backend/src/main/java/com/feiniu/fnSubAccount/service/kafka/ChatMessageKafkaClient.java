package com.feiniu.fnSubAccount.service.kafka;

import com.feiniu.fnSubAccount.consts.ChatMessageConsumerConfig;
import com.feiniu.fnSubAccount.util.web.HttpUtils;
import com.feiniu.kafka.client.ConsumerClient;

import javax.annotation.PostConstruct;

/**
 * Author: morningking
 * Date: 2016/4/18 16:30
 * Contact: 243717042@qq.com
 */
public class ChatMessageKafkaClient {
    private ChatMessageConsumerConfig chatMessageConsumerConfig;
    private ConsumerClient consumerClient;

    public void setChatMessageConsumerConfig(ChatMessageConsumerConfig chatMessageConsumerConfig) {
        this.chatMessageConsumerConfig = chatMessageConsumerConfig;
    }

    @PostConstruct
    private void init() {
        String groupID = chatMessageConsumerConfig.getGroupID() + HttpUtils.getIPAddress();
        System.out.println(groupID);

        consumerClient = new ConsumerClient(chatMessageConsumerConfig.getZookeeperConnectStr(),
                groupID, chatMessageConsumerConfig.getProperties(),
                chatMessageConsumerConfig.getTopicID(), chatMessageConsumerConfig.getHandler());

        consumerClient.consume();
    }
}
