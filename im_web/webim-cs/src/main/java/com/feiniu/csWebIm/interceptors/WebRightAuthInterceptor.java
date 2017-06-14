package com.feiniu.csWebIm.interceptors;

import com.feiniu.csWebIm.service.external.WebImAuthServie;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.ServletRequestUtils;
import org.springframework.web.servlet.HandlerInterceptor;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

/**
 * Created by wangzhen on 2015/11/26.
 */
public class WebRightAuthInterceptor implements HandlerInterceptor {

    private static final Logger LOGGER = LoggerFactory.getLogger("AuthInterceptor");

    @Autowired
    WebImAuthServie webImAuthServie;

    @Override
    public boolean preHandle(HttpServletRequest httpServletRequest, HttpServletResponse httpServletResponse, Object o) throws Exception {

        String sid = ServletRequestUtils.getStringParameter(httpServletRequest, "tokenKey", null);
        String token =ServletRequestUtils.getStringParameter(httpServletRequest, "tokenValue", null);

        //如果参数中没有数据，认证失败
        if (sid == null || token ==null){
            LOGGER.error("web右侧页权限认证失败，参数中没有token");
            return false;
        }

        String customerId = webImAuthServie.authWithSid(sid, token);
        if (customerId == null) {
            LOGGER.error("web右侧页权限认证失败,用户未登陆");
            return false;
        }
        else {
            LOGGER.error("web右侧页权限认证成功,刷新右侧页");
            HttpSession session = httpServletRequest.getSession();
            session.setAttribute("customerId",customerId);
        }
        return true;
    }

    @Override
    public void postHandle(HttpServletRequest httpServletRequest, HttpServletResponse httpServletResponse, Object o, ModelAndView modelAndView) throws Exception {

    }

    @Override
    public void afterCompletion(HttpServletRequest httpServletRequest, HttpServletResponse httpServletResponse, Object o, Exception e) throws Exception {

    }
}
