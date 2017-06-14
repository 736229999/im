package com.feiniu.csWebIm.interceptors;

import com.feiniu.csWebIm.bean.request.ServiceLogRequset;
import com.feiniu.csWebIm.utils.UrlDecoderUtils;
import org.springframework.web.bind.ServletRequestUtils;
import org.springframework.web.servlet.HandlerInterceptor;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 * Created by wangzhen on 2015/12/14.
 */
public class ServiceLogInterceptor implements HandlerInterceptor {


    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse httpServletResponse, Object o) throws Exception {
        ServiceLogRequset serviceLogRequset = new ServiceLogRequset();
        String customerToken = ServletRequestUtils.getStringParameter(request, "customerId", "");
        String goodsAskingType = ServletRequestUtils.getStringParameter(request, "goodsAskingType", "");
        String orderId = ServletRequestUtils.getStringParameter(request, "orId", "");

        UrlDecoderUtils.decodeUrl(serviceLogRequset,customerToken,goodsAskingType,"");

        serviceLogRequset.setOrId(orderId);
        request.setAttribute("ServiceLogRequest",serviceLogRequset);
        return true;
    }

    @Override
    public void postHandle(HttpServletRequest httpServletRequest, HttpServletResponse httpServletResponse, Object o, ModelAndView modelAndView) throws Exception {

    }

    @Override
    public void afterCompletion(HttpServletRequest httpServletRequest, HttpServletResponse httpServletResponse, Object o, Exception e) throws Exception {

    }
}
