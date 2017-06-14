package com.feiniu.fnSubAccount.controller.websocket;

import com.feiniu.fnSubAccount.constants.SessionAttributes;
import org.apache.commons.collections.CollectionUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Service;
import org.springframework.web.socket.TextMessage;
import org.springframework.web.socket.WebSocketSession;

import java.io.IOException;
import java.util.Collection;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

/**
 * Author: morningking
 * Date: 2016/4/11 14:15
 * Contact: 243717042@qq.com
 */
@Service
public class ChatSessionManager {
    private ConcurrentHashMap<Long, Set<WebSocketSession>> map = new ConcurrentHashMap<>(1);
    public static final Logger LOGGER = LoggerFactory.getLogger(ChatSessionManager.class);

    public synchronized void addPair(WebSocketSession session, Collection<Long> idList) {
        for (Long id : idList) {
            Set<WebSocketSession> toPut = new HashSet<>(1);
            Set<WebSocketSession> sessionList = map.putIfAbsent(id, toPut);
            sessionList = sessionList == null ? toPut : sessionList;

            sessionList.add(session);
        }
    }

    public synchronized void removePair(Set<Long> idSet, WebSocketSession session) {
        for (Long id : idSet) {
            Set<WebSocketSession> sessionSet = map.get(id);

            if (CollectionUtils.isNotEmpty(sessionSet)) {
                sessionSet.remove(session);

                if (sessionSet.isEmpty()) {
                    map.remove(id);
                }
            }
        }
    }

    /**
     * 给某个用户发送消息
     *
     * @param message
     */
    public void sendMessageToUser(List<Long> idList, String message) {
        Set<WebSocketSession> sessionSet = new HashSet<>(1);
        for (Long id : idList) {
            Set<WebSocketSession> currentSessionSet = map.get(id);

            if (CollectionUtils.isNotEmpty(currentSessionSet)) {
                for (WebSocketSession session : currentSessionSet) {
                    sessionSet.add(session);
                }
            }
        }

        for (WebSocketSession session : sessionSet) {
            try {
                Object monitorID = session.getAttributes().get(SessionAttributes.MONITOR_ID);

                if (session.isOpen()) {
                    session.sendMessage(new TextMessage(message));
                    LOGGER.debug("Send Message " + message + " to session[UserID=" + monitorID + "]");
                } else {
                    LOGGER.debug("Session[UserID=" + monitorID + "] is closed already!");
                }
            } catch (IOException e) {
                LOGGER.error("发送消息发生错误", e);
            }
        }
    }
}
