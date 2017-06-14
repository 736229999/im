package com.feiniu.subAccount.service;

import com.feiniu.subAccount.component.repository.ChatMessageManager;
import com.feiniu.subAccount.component.repository.HistoryMessageListRequest;
import com.feiniu.subAccount.domain.ChatMessage;
import com.feiniu.subAccount.util.Pager;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

/**
 * Created by wangchen on 2015/2/4.
 */
@Service
public class MonitorService {
    @Autowired
    private ChatMessageManager chatMessageManager;

    @Transactional
    public Pager<ChatMessage> getHistoryMessageList(HistoryMessageListRequest historyMessageListRequest) {
        Pager<ChatMessage> pager = new Pager<ChatMessage>();

        pager.setCurrentPageNo(historyMessageListRequest.getCurrentPageNo());
        pager.setPageSize(historyMessageListRequest.getPageSize());
        pager.setResultsForCurrentPage(chatMessageManager.getHistoryChatMessageList(historyMessageListRequest));
        pager.setTotalRecordSize(chatMessageManager.getTotalChatMessageNumber(historyMessageListRequest));
        long pageNum = pager.getTotalRecordSize() / pager.getPageSize();
        pager.setTotalPageNum(pager.getTotalRecordSize() % pager.getPageSize() == 0 ? pageNum : pageNum + 1);

        return pager;
    }
}
