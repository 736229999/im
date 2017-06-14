package com.feiniu.webim.servlet;

import com.feiniu.webim.bean.domain.ResultInfo;
import com.feiniu.webim.config.EventType;
import com.feiniu.webim.config.RequestParams;
import com.feiniu.webim.constants.ErrorCode;
import com.feiniu.webim.controller.ChatController;
import com.feiniu.webim.core.*;
import com.feiniu.webim.util.web.ServletRequestUtil;
import org.apache.log4j.Logger;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;

@WebServlet(value = "/im", loadOnStartup = 0)
public class WebIMServlet extends HttpServlet {

    private static final long serialVersionUID = -8846135815538952717L;
    private ChatController chatController = ChatController.getInstance();
    private static final Logger LOGGER = Logger.getLogger(WebIMServlet.class);

    /**
     * Constructor of the object.
     */
    public WebIMServlet() {
        super();
        initSession();
    }

    public synchronized void initSession() {
        LOGGER.info("session初始化：清空");
//        SessionManager.getInstance().removeAllSessions();
        SessionManager.getInstance().reloginAllUser();
    }

    /**
     * Destruction of the servlet. <br>
     */
    public void destroy() {
        super.destroy();
    }

    //处理  GET/POST 请求
    public void processRequest(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        ResponseContentGenerator generator = new ResponseContentGenerator(response);

        String event = request.getParameter(RequestParams.EVENT.toString());
        String callback = request.getParameter(RequestParams.CALLBACK.toString());

        if (event == null) {
            generator.start();
            generator.printClientError("请指定事件类型", callback);
            generator.close();
            return;
        }

        Session session = null;
        //新的用户,创建新的Session,并添加到SessionManager
        if (EventType.INIT_CHAT_ENV.toString().equalsIgnoreCase(event)
                || EventType.INIT_EMBED_ENV.toString().equalsIgnoreCase(event)) {
            //如果是 初始化 聊天环境/布点环境，那么设置session为null，以便后续判断
            //用户是否在webim有登录记录
        } else {
            String sid = request.getParameter(RequestParams.SID.toString());
            if (sid == null) {
                generator.start();
                generator.printClientError("未指定sid", callback);
                generator.close();
                return;
            }

            session = SessionManager.getInstance().getSessionByID(sid);
            if (session == null) {
                //Todo:临时解决方案，以后商家后台需要调用venderController另外一个接口
                if (!EventType.GET_SUB_MERCHANT_ONLINE_INFO.toString().equalsIgnoreCase(event)
                        && !EventType.GET_MERCHANT_ONLINE_INFO.toString().equalsIgnoreCase(event)) {
                    LOGGER.error("[session-id-invalid]Session不存在或者已经失效, id:" + sid);

                    ResultInfo resultInfo = new ResultInfo();
                    resultInfo.setErrorMessage("您已很长时间没有发言，请重新刷新页面登录以开启会话");
                    resultInfo.setErrorCode(ErrorCode.ERRORCODE_CHAT_SESSION_NOT_EXIST);

                    generator.start();
                    generator.printClientError(resultInfo, callback);
                    generator.close();

                    return;
                }
            } else if (!"-1".equals(sid)) {
                String token = TokenManager.parseToken(request);
                String uuid = ServletRequestUtil.getString(request, "uuid", "");

                if (!session.isTokenLegal(token) || !session.updateChatWindowActiveTime(uuid)) {
                    LOGGER.info("User " + session.getUserName() + " with token: " +
                            token + "; uuid: " + uuid + " is intercepted as illegal access.");

                    ResultInfo resultInfo = new ResultInfo();
                    resultInfo.setErrorMessage("非法访问");
                    resultInfo.setErrorCode(ErrorCode.ERRORCODE_NOT_LEGAL);

                    generator.start();
                    generator.printClientError(resultInfo, callback);
                    generator.close();
                    return;
                }
            }
        }

        WebAction webAction = new WebAction();

        TokenManager.putToken(request);
        webAction.setRequest(request);
        webAction.setResponse(response);
        webAction.setSession(session);
        webAction.setResponseContentGenerator(generator);

        chatController.handleRequest(webAction);
    }

    public void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        processRequest(request, response);
    }

    public void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        processRequest(request, response);
    }

    /**
     * Initialization of the servlet. <br>
     *
     * @throws javax.servlet.ServletException if an error occurs
     */
    public void init() throws ServletException {
        getServletContext().setAttribute("webimServlet", this);
    }

}
