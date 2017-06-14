package com.cn.feiniu.logManagement.bean.base;

import com.feiniu.kafka.client.KafkaStreamHandler;

/**
 * Author: morningking
 * Date: 2016/1/22
 * Contact: 243717042@qq.com
 */
public class KafkaConfig {
    private String groupId;
    private String topicName;
    private String zookeeperConnectString;
    private int timeoutMilis;
    private String offsetReset;
    private KafkaStreamHandler kafkaStreamHandler;

    public String getGroupId() {
        return groupId;
    }

    public void setGroupId(String groupId) {
        this.groupId = groupId;
    }

    public String getTopicName() {
        return topicName;
    }

    public void setTopicName(String topicName) {
        this.topicName = topicName;
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
}
