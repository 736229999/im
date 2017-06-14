package com.feiniu.fnSubAccount.repository;

import com.feiniu.fnSubAccount.bean.request.GetHistoryMessageListWithSessionRequest;
import com.feiniu.fnSubAccount.bean.request.HistoryMessageListRequest;
import com.feiniu.fnSubAccount.domain.ChatMessage;

import java.util.List;

/**
 * Created by wangchen on 2015/2/4.
 */
public interface ChatMessageDao {
    //所有客服与买家的聊天记录
    List<ChatMessage> getHistoryChatMessageList(HistoryMessageListRequest historyMessageListRequest);

    List<ChatMessage> getHistoryChatMessageListWithinSession(GetHistoryMessageListWithSessionRequest request);

    long getTotalChatMessageNumber(HistoryMessageListRequest historyMessageListRequest);

    //根据ID列表来获取聊天记录
    List<ChatMessage> getHistoryChatMessageListByIdList(HistoryMessageListRequest historyMessageListRequest);

    long getTotalChatMessageNumberByIdList(HistoryMessageListRequest historyMessageListRequest);


    List<ChatMessage> getHistoryChatMessageListByid(long sessionId,long subMerchantId,
                                                           int startRow,int endRow);

    public long getHistoryChatMessageCountByid(long sessionId,long subMerchantId);

    List<ChatMessage> getCsHistoryChatMessageListBySession(long sessionId,int startRow,int endRow);

    long getCsgetCsHistoryChatMessageLenBySession(long sessionId);
}
