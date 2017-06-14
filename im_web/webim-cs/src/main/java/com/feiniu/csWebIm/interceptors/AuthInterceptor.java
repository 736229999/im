package com.feiniu.csWebIm.interceptors;

import com.feiniu.csWebIm.bean.request.ServiceLogRequset;
import com.feiniu.csWebIm.service.AuthService;
import com.feiniu.csWebIm.utils.UrlDecoderUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.servlet.HandlerInterceptor;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 * Created by wangzhen on 2015/11/3.
 */
public class AuthInterceptor implements HandlerInterceptor {

    private static final Logger LOGGER = LoggerFactory.getLogger("AuthInterceptor");

    @Autowired
    AuthService authService;

    @Override
    public boolean preHandle(HttpServletRequest request,
                             HttpServletResponse response, Object handler) throws Exception {

        ServiceLogRequset serviceLogRequset = new ServiceLogRequset();
        if(!UrlDecoderUtils.decodeUrl(serviceLogRequset, request)){
            return  false;
        }

        String serviceId = serviceLogRequset.getCustomerServiceId();
        if (serviceId == null || serviceId == "") {
            return  false;
        }

        if (authService.authCookieToken(request,serviceId) ) {
            request.setAttribute("serviceLogRequset",serviceLogRequset );
            return true;
        } else{
            return false;
        }
    }

    @Override
    public void postHandle(HttpServletRequest request,
                           HttpServletResponse response, Object handler,
                           ModelAndView modelAndView) throws Exception {

    }

    @Override
    public void afterCompletion(HttpServletRequest request,
                                HttpServletResponse response, Object handler, Exception ex)
            throws Exception {

    }
}
