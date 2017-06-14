package com.feiniu.imServer.bean;

import com.feiniu.kafka.client.KafkaStreamHandler;

/**
 * Author: morningking
 * Date: 2016/1/22
 * Contact: 243717042@qq.com
 */
public class KafkaConfig {
    private String groupIdNewOrder;
    private String groupIdPayOrder;
    private String groupIdReturnOrder;
    private String groupIdGoodsComment;
    private String topicNewOrder;
    private String topicPayOrder;
    private String topicReturnOrder;
    private String topicGoodsComment;
    private String zookeeperConnectString;
    private int timeoutMilis;
    private String offsetReset;
    private KafkaStreamHandler kafkaStreamHandler;
    private KafkaStreamHandler newOrderHandler;
    private KafkaStreamHandler payOrderHandler;
    private KafkaStreamHandler returnOrderHandler;
    private KafkaStreamHandler goodsCommentHandler;

    public String getGroupIdNewOrder() {
        return groupIdNewOrder;
    }

    public void setGroupIdNewOrder(String groupIdNewOrder) {
        this.groupIdNewOrder = groupIdNewOrder;
    }

    public String getGroupIdPayOrder() {
        return groupIdPayOrder;
    }

    public void setGroupIdPayOrder(String groupIdPayOrder) {
        this.groupIdPayOrder = groupIdPayOrder;
    }

    public String getGroupIdReturnOrder() {
        return groupIdReturnOrder;
    }

    public void setGroupIdReturnOrder(String groupIdReturnOrder) {
        this.groupIdReturnOrder = groupIdReturnOrder;
    }

    public String getGroupIdGoodsComment() {
        return groupIdGoodsComment;
    }

    public void setGroupIdGoodsComment(String groupIdGoodsComment) {
        this.groupIdGoodsComment = groupIdGoodsComment;
    }

    public String getTopicNewOrder() {
        return topicNewOrder;
    }

    public void setTopicNewOrder(String topicNewOrder) {
        this.topicNewOrder = topicNewOrder;
    }

    public String getTopicPayOrder() {
        return topicPayOrder;
    }

    public void setTopicPayOrder(String topicPayOrder) {
        this.topicPayOrder = topicPayOrder;
    }

    public String getTopicReturnOrder() {
        return topicReturnOrder;
    }

    public void setTopicReturnOrder(String topicReturnOrder) {
        this.topicReturnOrder = topicReturnOrder;
    }

    public String getTopicGoodsComment() {
        return topicGoodsComment;
    }

    public void setTopicGoodsComment(String topicGoodsComment) {
        this.topicGoodsComment = topicGoodsComment;
    }

    public String getZookeeperConnectString() {
        return zookeeperConnectString;
    }

    public void setZookeeperConnectString(String zookeeperConnectString) {
        this.zookeeperConnectString = zookeeperConnectString;
    }

    public int getTimeoutMilis() {
        return timeoutMilis;
    }

    public void setTimeoutMilis(int timeoutMilis) {
        this.timeoutMilis = timeoutMilis;
    }

    public String getOffsetReset() {
        return offsetReset;
    }

    public void setOffsetReset(String offsetReset) {
        this.offsetReset = offsetReset;
    }

    public KafkaStreamHandler getKafkaStreamHandler() {
        return kafkaStreamHandler;
    }

    public void setKafkaStreamHandler(KafkaStreamHandler kafkaStreamHandler) {
        this.kafkaStreamHandler = kafkaStreamHandler;
    }

    public KafkaStreamHandler getNewOrderHandler() {
        return newOrderHandler;
    }

    public void setNewOrderHandler(KafkaStreamHandler newOrderHandler) {
        this.newOrderHandler = newOrderHandler;
    }

    public KafkaStreamHandler getPayOrderHandler() {
        return payOrderHandler;
    }

    public void setPayOrderHandler(KafkaStreamHandler payOrderHandler) {
        this.payOrderHandler = payOrderHandler;
    }

    public KafkaStreamHandler getReturnOrderHandler() {
        return returnOrderHandler;
    }

    public void setReturnOrderHandler(KafkaStreamHandler returnOrderHandler) {
        this.returnOrderHandler = returnOrderHandler;
    }

    public KafkaStreamHandler getGoodsCommentHandler() {
        return goodsCommentHandler;
    }

    public void setGoodsCommentHandler(KafkaStreamHandler goodsCommentHandler) {
        this.goodsCommentHandler = goodsCommentHandler;
    }
}
