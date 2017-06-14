package com.cn.feiniu.logManagement.exception;

import com.cn.feiniu.logManagement.bean.base.WebResultInfo;
import com.cn.feiniu.logManagement.constants.SystemConfig;
import com.cn.feiniu.logManagement.util.json.JsonUtil;
import com.cn.feiniu.logManagement.util.web.AjaxUtil;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.web.servlet.HandlerExceptionResolver;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.OutputStream;

/**
 * Author: morningking
 * Date: 2015/8/6
 * Contact: 243717042@qq.com
 */
public class CustomGlobalExceptionResolver implements
        HandlerExceptionResolver {
    private static final String SERVER_ERROR;
    private ModelAndView SERVER_ERROR_VIEW = new ModelAndView("error/server-error");
    private static final Logger LOGGER = LoggerFactory.getLogger(CustomGlobalExceptionResolver.class);

    static {
        WebResultInfo responseResult = new WebResultInfo();
        responseResult.setErrorMessage("服务器发生故障");
        SERVER_ERROR = JsonUtil.toJsonString(responseResult);
    }

    @Override
    public ModelAndView resolveException(HttpServletRequest request, HttpServletResponse response, Object handler, Exception ex) {
        LOGGER.error("发现未捕获的全局错误", ex);

        if (AjaxUtil.isAjaxRequest(request)) {
            try {
                response.setHeader("Content-Type", "application/json;charset=UTF-8");
                OutputStream outputStream = response.getOutputStream();

                outputStream.write(SERVER_ERROR.getBytes(SystemConfig.DEFAULT_ENCODING));
                outputStream.flush();
                outputStream.close();
            } catch (IOException e) {
                LOGGER.error("返回全局异常说明时发生错误", e);
            }
        } else {
            return SERVER_ERROR_VIEW;
        }

        return null;
    }

}
