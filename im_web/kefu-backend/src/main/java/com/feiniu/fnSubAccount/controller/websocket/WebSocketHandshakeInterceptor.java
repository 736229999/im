package com.feiniu.fnSubAccount.controller.websocket;

import com.feiniu.common.core.SystemEnv;
import com.feiniu.common.core.UserSession;
import com.feiniu.fnSubAccount.constants.SessionAttributes;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.http.server.ServerHttpRequest;
import org.springframework.http.server.ServerHttpResponse;
import org.springframework.http.server.ServletServerHttpRequest;
import org.springframework.web.socket.WebSocketHandler;
import org.springframework.web.socket.server.HandshakeInterceptor;

import javax.servlet.http.HttpSession;
import java.util.Map;

/**
 * Author: morningking
 * Date: 2016/4/6 11:27
 * Contact: 243717042@qq.com
 */
public class WebSocketHandshakeInterceptor implements HandshakeInterceptor {
    private static final Logger logger = LoggerFactory.getLogger(HandshakeInterceptor.class);

    @Override
    public boolean beforeHandshake(ServerHttpRequest request, ServerHttpResponse response, WebSocketHandler wsHandler,
                                   Map<String, Object> attributes) throws Exception {
        if (request instanceof ServletServerHttpRequest) {
            ServletServerHttpRequest servletRequest = (ServletServerHttpRequest) request;
            HttpSession session = servletRequest.getServletRequest().getSession(false);
            if (session != null) {
                UserSession userSession = SystemEnv.getCurrentUser();
                logger.info("Interceptored UserID:[" + userSession.getUserId()
                        + "], UserName:[" + userSession.getTrueName() + "] connected to server");
                attributes.put(SessionAttributes.MONITOR_ID, SystemEnv.getCurrentUser().getUserId());
            }
        }

        return true;
    }

    @Override
    public void afterHandshake(ServerHttpRequest request, ServerHttpResponse response, WebSocketHandler wsHandler, Exception exception) {

    }
}
