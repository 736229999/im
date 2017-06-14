package com.feiniu.fnSubAccount.controller.websocket;

import com.feiniu.fnSubAccount.constants.SessionAttributes;
import com.feiniu.fnSubAccount.util.JsonUtil;
import org.codehaus.jackson.JsonNode;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.socket.*;

import java.util.*;

/**
 * Author: morningking
 * Date: 2016/4/6 11:38
 * Contact: 243717042@qq.com
 */
public class SystemWebSocketHandler implements WebSocketHandler {
    @Autowired
    private ChatSessionManager chatSessionManager;

    private static final Logger logger = LoggerFactory.getLogger(SystemWebSocketHandler.class);

    @Override
    public void afterConnectionEstablished(WebSocketSession session) throws Exception {
        Long userID = (Long) session.getAttributes().get(SessionAttributes.MONITOR_ID);
        if (userID != null) {
            session.getAttributes().put("idList", Collections.synchronizedSet(new HashSet<>(1)));

            logger.info("User[" + userID + "] connect to the websocket success......");
        }
    }

    @Override
    public void handleMessage(WebSocketSession session, WebSocketMessage<?> message) throws Exception {
        Long userID = (Long) session.getAttributes().get(SessionAttributes.MONITOR_ID);
        if (message.getPayload() != null) {
            String payload = message.getPayload().toString();
            JsonNode jsonNode = JsonUtil.toNode(payload);

            if (jsonNode != null) {
                Iterator<JsonNode> idListIterator = jsonNode.get("idList").iterator();
                List<Long> idList = new ArrayList<>(1);

                while (idListIterator.hasNext()) {
                    idList.add(idListIterator.next().asLong());
                }

                chatSessionManager.addPair(session, idList);
                ((Set) (session.getAttributes().get("idList"))).addAll(idList);
                logger.info("Monitor [" + userID + "] subscribe messages from " + JsonUtil.toJsonString(idList));
            } else {
                logger.info("Receive heartbeat message from Monitor [" + userID + "] at " + System.currentTimeMillis());
            }
        }
    }

    @Override
    public void handleTransportError(WebSocketSession session, Throwable exception) throws Exception {
        if (session.isOpen()) {
            session.close();
        }
        logger.info("websocket connection closed......");
    }

    @Override
    public void afterConnectionClosed(WebSocketSession session, CloseStatus closeStatus) throws Exception {
        logger.info("websocket connection closed......");
        Set<Long> idList = (Set<Long>) session.getAttributes().get("idList");

        chatSessionManager.removePair(idList, session);

        Long userID = (Long) session.getAttributes().get(SessionAttributes.MONITOR_ID);
        if (userID != null) {
            logger.info("User[" + userID + "] close the websocket");
        }
    }

    @Override
    public boolean supportsPartialMessages() {
        return false;
    }
}
