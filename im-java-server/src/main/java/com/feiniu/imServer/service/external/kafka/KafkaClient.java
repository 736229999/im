package com.feiniu.imServer.service.external.kafka;


import com.feiniu.imServer.bean.KafkaConfig;
import com.feiniu.kafka.client.ConsumerClient;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import javax.annotation.PostConstruct;
import java.util.Properties;

/**
 * Author: morningking
 * Date: 2016/1/25
 * Contact: 243717042@qq.com
 */
@Component
public class KafkaClient {
    @Autowired
    private KafkaConfig kafkaConfig;
    @Autowired
    private ConsumerClient consumerImServerClient;

    private static final Logger LOGGER = LoggerFactory.getLogger(KafkaClient.class);

    @PostConstruct
    private void initKafkaConsumer() {
        Properties clientProperties = new Properties();
        clientProperties.setProperty("zookeeper.connection.timeout.ms", kafkaConfig.getTimeoutMilis() + "");
        clientProperties.setProperty("auto.offset.reset", kafkaConfig.getOffsetReset());

        //新订单
        ConsumerClient consumerClientNewOrder = new ConsumerClient(kafkaConfig.getZookeeperConnectString(),
                kafkaConfig.getGroupIdNewOrder(), clientProperties,
                kafkaConfig.getTopicNewOrder(), kafkaConfig.getNewOrderHandler());
        consumerClientNewOrder.consume();

        //付款订单
        ConsumerClient consumerClientPayOrder = new ConsumerClient(kafkaConfig.getZookeeperConnectString(),
                kafkaConfig.getGroupIdPayOrder(), clientProperties,
                kafkaConfig.getTopicPayOrder(), kafkaConfig.getPayOrderHandler());
        consumerClientPayOrder.consume();

        //退货订单
        ConsumerClient consumerClientReturnOrder = new ConsumerClient(kafkaConfig.getZookeeperConnectString(),
                kafkaConfig.getGroupIdReturnOrder(), clientProperties,
                kafkaConfig.getTopicReturnOrder(), kafkaConfig.getReturnOrderHandler());
        consumerClientReturnOrder.consume();

        //商品评论
        ConsumerClient consumerClientGoodsComment = new ConsumerClient(kafkaConfig.getZookeeperConnectString(),
                kafkaConfig.getGroupIdGoodsComment(), clientProperties,
                kafkaConfig.getTopicGoodsComment(), kafkaConfig.getGoodsCommentHandler());
        consumerClientGoodsComment.consume();

        consumerImServerClient.consume();
    }
}

