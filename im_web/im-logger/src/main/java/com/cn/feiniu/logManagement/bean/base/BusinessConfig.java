package com.cn.feiniu.logManagement.bean.base;

/**
 * Author: morningking
 * Date: 2015/7/22
 * Contact: 243717042@qq.com
 */
public class BusinessConfig {
    private int maxRequestInMemory;
    private String submerchantActionLogFilePath;
    private int maxChatMessagesInQueue;

    public int getMaxRequestInMemory() {
        return maxRequestInMemory;
    }

    public void setMaxRequestInMemory(int maxRequestInMemory) {
        this.maxRequestInMemory = maxRequestInMemory;
    }

    public String getLogFilePath() {
        return submerchantActionLogFilePath;
    }

    public void setLogFilePath(String logFilePath) {
        this.submerchantActionLogFilePath = logFilePath;
    }

    public int getMaxChatMessagesInQueue() {
        return maxChatMessagesInQueue;
    }

    public void setMaxChatMessagesInQueue(int maxChatMessagesInQueue) {
        this.maxChatMessagesInQueue = maxChatMessagesInQueue;
    }
}
