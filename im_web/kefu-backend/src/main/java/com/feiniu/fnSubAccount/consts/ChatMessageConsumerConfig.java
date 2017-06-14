package com.feiniu.fnSubAccount.consts;

import com.feiniu.kafka.client.KafkaStreamHandler;

import java.util.Properties;

/**
 * Author: morningking
 * Date: 2016/4/18 16:35
 * Contact: 243717042@qq.com
 */
public class ChatMessageConsumerConfig {
    private String zookeeperConnectStr;
    private String groupID;
    private String topicID;
    private KafkaStreamHandler handler;
    private Properties properties;

    public String getZookeeperConnectStr() {
        return zookeeperConnectStr;
    }

    public void setZookeeperConnectStr(String zookeeperConnectStr) {
        this.zookeeperConnectStr = zookeeperConnectStr;
    }

    public String getGroupID() {
        return groupID;
    }

    public void setGroupID(String groupID) {
        this.groupID = groupID;
    }

    public String getTopicID() {
        return topicID;
    }

    public void setTopicID(String topicID) {
        this.topicID = topicID;
    }

    public KafkaStreamHandler getHandler() {
        return handler;
    }

    public void setHandler(KafkaStreamHandler handler) {
        this.handler = handler;
    }

    public Properties getProperties() {
        return properties;
    }

    public void setProperties(Properties properties) {
        this.properties = properties;
    }
}
