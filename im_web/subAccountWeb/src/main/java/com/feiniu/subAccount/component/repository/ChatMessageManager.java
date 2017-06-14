package com.feiniu.subAccount.component.repository;

import com.feiniu.subAccount.domain.ChatMessage;

import java.util.List;

/**
 * Created by wangchen on 2015/2/4.
 */
public interface ChatMessageManager {
    public List<ChatMessage> getHistoryChatMessageList(HistoryMessageListRequest historyMessageListRequest);

    public long getTotalChatMessageNumber(HistoryMessageListRequest historyMessageListRequest);
}
