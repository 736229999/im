package com.feiniu.webim.core;

import org.apache.log4j.Logger;

/**
 * 消息分发
 *
 * @author zb
 */
public class Dispatcher {
    private static final Logger LOGGER = Logger.getLogger(Dispatcher.class);

    private static Dispatcher instance = new Dispatcher();

    protected Dispatcher() {

    }

    public static Dispatcher getInstance() {
        return instance;
    }

    /**
     * 向指定session添加消息
     */
    public void unicast(String message, String gid) {
        /** gid === -1 **/
        Session to = SessionManager.getInstance().getSessionByID(gid);
        if (to == null) {
            LOGGER.warn("Miss Session,指定接收session不存在,或已失效");
        } else {
            to.getMessageManagerCenter().postMessage(message);
        }
    }
}
