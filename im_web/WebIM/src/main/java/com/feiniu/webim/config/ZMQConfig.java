package com.feiniu.webim.config;

/**
 * Author: morningking
 * Date: 2016/3/9
 * Contact: 243717042@qq.com
 */
public enum ZMQConfig {
    /**
     * zmq排队信息发布者地址描述符
     */
    ZMQ_QUEUE_INFO_PUB_DESCRIPTOR,
    /**
     * zmq排队信息发布topic
     */
    ZMQ_QUEUE_INFO_PUB_TOPIC,
    ;

    @Override
    public String toString() {
        return super.name().toLowerCase();
    }
}
