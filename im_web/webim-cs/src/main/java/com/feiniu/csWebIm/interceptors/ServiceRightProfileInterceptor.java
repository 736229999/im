package com.feiniu.csWebIm.interceptors;

import com.feiniu.csWebIm.service.AuthService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.ServletRequestUtils;
import org.springframework.web.servlet.HandlerInterceptor;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 * Created by wangzhen on 2015/12/16.
 */
public class ServiceRightProfileInterceptor implements HandlerInterceptor {

    private static final Logger LOGGER = LoggerFactory.getLogger("ServiceRightProfileInterceptor");

    @Autowired
    AuthService authService;

    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse httpServletResponse, Object o) throws Exception {
        String serviceId = ServletRequestUtils.getStringParameter(request, "customerServiceId",null);

        if (serviceId == null) {
            LOGGER.error("客服右侧页页权限认证失败:没有参数 customerServiceId");
            return false;
        }

        return  authService.authCookieToken(request,serviceId);

    }

    @Override
    public void postHandle(HttpServletRequest httpServletRequest, HttpServletResponse httpServletResponse, Object o, ModelAndView modelAndView) throws Exception {

    }

    @Override
    public void afterCompletion(HttpServletRequest httpServletRequest, HttpServletResponse httpServletResponse, Object o, Exception e) throws Exception {

    }
}
