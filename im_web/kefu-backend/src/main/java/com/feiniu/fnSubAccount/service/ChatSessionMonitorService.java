package com.feiniu.fnSubAccount.service;

import com.feiniu.fnSubAccount.bean.ChatSession;
import com.feiniu.fnSubAccount.bean.request.GetChatSessionRequest;
import com.feiniu.fnSubAccount.repository.ChatSessionMonitorDao;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;

/**
 * Author: morningking
 * Date: 2016/4/8 8:52
 * Contact: 243717042@qq.com
 */
@Service
@Transactional
public class ChatSessionMonitorService {
    @Autowired
    private ChatSessionMonitorDao chatSessionMonitorDao;

    //获取活动[未关闭的]会话列表
    public List<ChatSession> getActiveChatSessionList(GetChatSessionRequest request) {
        return chatSessionMonitorDao.getActiveChatSessionList(request);
    }

    //当前指示的会话是否被关掉
    public boolean isChatSessionClosed(int sessionID) {
        return chatSessionMonitorDao.isChatSessionClosed(sessionID);
    }
}
