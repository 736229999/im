package com.feiniu.fnSubAccount.repository;

import com.feiniu.fnSubAccount.bean.ChatSession;
import com.feiniu.fnSubAccount.bean.request.GetChatSessionRequest;

import java.util.List;

/**
 * Author: morningking
 * Date: 2016/4/7 13:49
 * Contact: 243717042@qq.com
 */
public interface ChatSessionMonitorDao {
    List<ChatSession> getActiveChatSessionList(GetChatSessionRequest getChatSessionRequest);

    boolean isChatSessionClosed(int sessionID);
}
