package com.cn.feiniu.logManagement.repository;

import com.cn.feiniu.logManagement.bean.dto.ChatMessage;

import java.util.List;

/**
 * Author: morningking
 * Date: 2016/1/25
 * Contact: 243717042@qq.com
 */
public interface ChatMessageDao {
    void saveMessageList(List<ChatMessage> chatMessages);

    void saveMessage(ChatMessage chatMessage);
}
