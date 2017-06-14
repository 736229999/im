package com.feiniu.webim.core;

import com.feiniu.webim.bean.domain.UserInfo;
import com.feiniu.webim.bean.web.request.LoginWebRequest;
import com.feiniu.webim.component.ChatService;
import com.feiniu.webim.config.ConfigManager;
import com.feiniu.webim.config.SystemConfig;
import com.feiniu.webim.exception.ImserverException;
import com.feiniu.webim.util.json.JSONUtil;
import com.feiniu.webim.util.web.WebUtil;
import org.apache.commons.lang.math.NumberUtils;
import org.apache.log4j.Logger;

import java.util.*;
import java.util.concurrent.ConcurrentHashMap;

/**
 * Session 管理类(单例)
 *
 * @author zb
 */
public class SessionManager {

    /**
     * 缓存实例
     */
    private static final SessionManager instance = new SessionManager();

    /**
     * 存储session,Map<Session Id,Session>
     */
    private final ConcurrentHashMap<String, Session> idTosessions = new ConcurrentHashMap<>();
    private final ConcurrentHashMap<String, Session> nameToSession = new ConcurrentHashMap<>();

    private static final Logger LOGGER = Logger.getLogger(SessionManager.class);
    /**
     * 计划任务,用于指定时间检查session是否有效
     */
    private Timer timer;
    private final long TIMER_INTERVAL_MILLIS = ConfigManager.getLongProperty(SystemConfig.TIMER_INTERVAL_MINS.toString()) * 1000;

    /**
     * 是否重新缓存(自动设置)
     */
    private volatile boolean sessionCacheDirty = false;
    private volatile List<Session> sessionCache = Collections.emptyList();

    private ChatService chatService = ChatService.getInstance();

    /**
     *
     */
    protected SessionManager() {

    }

    /**
     * 获得SessionManager实例
     *
     * @return
     */
    public static final SessionManager getInstance() {
        return instance;
    }

    public Collection<Session> getIdTosessions() {
        return new ArrayList<>(idTosessions.values());
    }

    /**
     * 添加Session
     */
    public Session addSession(Session session) {
        Session previousSession = idTosessions.putIfAbsent(session.getId(), session);
        boolean hasNotLoginedBefore = previousSession == null;

        if (hasNotLoginedBefore) {
            nameToSession.putIfAbsent(session.getUserName(), session);
            sessionCacheDirty = true;
        }

        LOGGER.info("[add-session]添加会话：" + JSONUtil.toJsonString(session));

        return hasNotLoginedBefore ? session : previousSession;
    }

    /**
     * 获得Session
     *
     * @param id Session Id
     * @return Session
     */
    public Session getSessionByID(String id) {
        return idTosessions.get(id);
    }

    public Session getSessionByName(String userName) {
        return nameToSession.get(userName);
    }

    /**
     * 移除并返回Session
     *
     * @param session Session
     */
    public void removeSession(Session session) {
        Session nsession = idTosessions.remove(session.getId());
        nameToSession.remove(session.getUserName());

        if (nsession != null) {
            LOGGER.info("[remove-session]移除会话：" + JSONUtil.toJsonString(nsession));
        }
        sessionCacheDirty = true;
    }

    public void removeAllSessions() {
        idTosessions.clear();
        nameToSession.clear();
    }

    /**
     * 重新登录所有买家
     */
    public void reloginAllUser() {
        Collection<Session> sessionList = SessionManager.getInstance().getIdTosessions();
        LoginWebRequest loginWebRequest = new LoginWebRequest();
        boolean isAllUserReloginSucc = true;

        reloginAllUserLoop:
        for (Session session : sessionList) {
            if (session != null && session.isLogin() && NumberUtils.isDigits(session.getId())) {
                UserInfo userInfo = session.getUserInfo();
                String id = session.getId();
                if (userInfo != null) {
                    loginWebRequest.setEmail(userInfo.getEmail());
                    loginWebRequest.setGuid(userInfo.getGuid());
                    loginWebRequest.setSession(session);
                    loginWebRequest.setSname(userInfo.getUsername());
                    loginWebRequest.setTelephone(userInfo.getMobile());

                    boolean reloginSucc = false;
                    reloginSingleUserUntilSuccessLoop:
                    while (!reloginSucc) {
                        //如果当前线程被打断，意味着在进行下一次重连，当前线程放弃重新登录
                        if (Thread.currentThread().isInterrupted()) {
                            LOGGER.warn("重连线程被打断，此次重连过程终止，此次线程号为：" + Thread.currentThread().getName());
                            isAllUserReloginSucc = false;
                            break reloginAllUserLoop;
                        }

                        String userInfoStr = "[id=" + id + ",name=" + userInfo.getNickname() + "]";
                        try {
                            LOGGER.info("重新向im登录用户以保持用户登录态：此次重登陆用户信息：" + userInfoStr);
                            chatService.login(loginWebRequest);
                            reloginSucc = true;
                            LOGGER.info("用户：" + userInfoStr + " 重新登录成功");
                        } catch (ImserverException e) {
                            LOGGER.error("重新登录用户" + userInfoStr + "时发生超时,忽略并进行重试", e);
                        } catch (Exception e) {
                            LOGGER.error("重新登录用户出错" + userInfoStr + ", 放弃此用户的再次重登陆尝试", e);
                            break reloginSingleUserUntilSuccessLoop;
                        }
                    }
                }
            }
        }

        if (isAllUserReloginSucc) {
            LOGGER.info("重连后，重新登录所有用户完毕");
        }
    }

    /**
     * idTosessions 长度
     *
     * @return 长度
     */
    public int getSize() {
        return idTosessions.size();
    }

    /**
     * 遍历执行Seesion并调用ApplyMethod invoke执行
     * >>添加缓存支持
     *
     * @param method
     */
    public void apply(ApplyMethod method) {
        //更新缓存
        if (sessionCacheDirty) {
            sessionCache = new ArrayList<>(idTosessions.values());
            //设置状态为fasle,防止再次更新
            sessionCacheDirty = false;
        }

        //遍历,传递给 method对象的invoke方法执行(必须从0开始遍历)
        for (Session session : sessionCache) {
            if (session != null) {
                try {
                    method.invoke(session);
                } catch (Exception e) {
                    LOGGER.warn("SessionManager apply invoke 方法执行出错:", e);
                }
            }
        }
    }

    public void start() {
        if (timer != null) {
            stop();
        }
        timer = new Timer(false);
        timer.schedule(new CheckTimerTask(), TIMER_INTERVAL_MILLIS, TIMER_INTERVAL_MILLIS);
    }

    public void stop() {
        if (timer != null) {
            timer.cancel();
            timer = null;
        }

        for (Session loginedSession : idTosessions.values()) {
            loginedSession.destroy();
        }

        idTosessions.clear();
        nameToSession.clear();
        LOGGER.info("停止会话检查线程");
    }

    /**
     * apply 调用方法接口
     *
     * @author zb
     */
    public interface ApplyMethod {
        void invoke(Session session);
    }

    /**
     * 检查Session是否有效
     *
     * @author zb
     */
    private class CheckTimerTask extends TimerTask implements ApplyMethod {
        @Override
        public void run() {
            getInstance().apply(this);
            if (LOGGER.isDebugEnabled()) {
                LOGGER.debug("[checker-timer-run],时间:" + WebUtil.dateFormat()
                                + ", idTosessions size:" + idTosessions.size() + " "
                                + ", nameTosessions size:" + nameToSession.size() + " "
                                + ", cache size: " + sessionCache.size()
                );
            }
        }

        @Override
        public void invoke(Session session) {
            //判断用户在指定节点是否未发言
            int timeNode = session.getTimedNode();
            if (timeNode > 0) {
                session.alertSpeakTime(timeNode);
            }

            //扫描那些在前台退出却在后台未登记退出的窗口
            session.doCheckChatWindowsExpiration();

            if (session.isExpired()) {
                LOGGER.info("[check-timer-remove-session]执行会话移除会话：" + session.getId());
                session.destroy();
            }
        }
    }
}
