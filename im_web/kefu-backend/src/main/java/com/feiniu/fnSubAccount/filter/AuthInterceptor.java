package com.feiniu.fnSubAccount.filter;

import com.feiniu.fnSubAccount.service.DirectorService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.servlet.HandlerInterceptor;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 * Created by wangzhen on 2015/11/30.
 */
public class AuthInterceptor implements HandlerInterceptor {
    private static final Logger LOGGER = LoggerFactory.getLogger("AuthInterceptor");

    @Autowired
    private DirectorService directorService;
    @Override
    public boolean preHandle(HttpServletRequest httpServletRequest, HttpServletResponse httpServletResponse, Object o) throws Exception {
        return  true;

//        UserSession userSession = SystemEnv.getCurrentUser();
//        HttpSession session = httpServletRequest.getSession();
//        if (userSession == null || userSession.getUserId() == null) {
//            return false;
//        }
//        Boolean sessionPassed = (Boolean)session.getAttribute("isAuthed");
//        if (sessionPassed != null && sessionPassed == true){
//            return  true;
//        }
//        //判断是否是主管
//        boolean isPassed= directorService.isIdDirector(userSession.getUserId());
//
//        if (!isPassed) {
//            LOGGER.error("权限认证失败,用户未登陆");
//        }
//        else {
//            LOGGER.error("权限认证成功.");
//            session.setAttribute("isAuthed",true);
//        }
//        return isPassed;
    }

    @Override
    public void postHandle(HttpServletRequest httpServletRequest, HttpServletResponse httpServletResponse, Object o, ModelAndView modelAndView) throws Exception {

    }

    @Override
    public void afterCompletion(HttpServletRequest httpServletRequest, HttpServletResponse httpServletResponse, Object o, Exception e) throws Exception {

    }
}
