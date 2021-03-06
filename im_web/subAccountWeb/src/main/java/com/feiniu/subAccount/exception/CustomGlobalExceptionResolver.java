package com.feiniu.subAccount.exception;

import com.feiniu.subAccount.base.ResponseResult;
import com.feiniu.subAccount.constants.ResponseMessages;
import com.feiniu.subAccount.constants.SystemConfig;
import com.feiniu.subAccount.util.AjaxUtil;
import com.feiniu.subAccount.util.JsonUtil;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.web.servlet.HandlerExceptionResolver;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.OutputStream;

/**
 * Created by wangchen on 2015/1/6.
 */
public class CustomGlobalExceptionResolver implements
        HandlerExceptionResolver {
    private static final String SERVER_ERROR;
    private ModelAndView SERVER_ERROR_VIEW = new ModelAndView("error/server-error");
    private static final Logger LOGGER = LoggerFactory.getLogger(CustomGlobalExceptionResolver.class);

    static {
        ResponseResult responseResult = new ResponseResult();
        responseResult.setMessage(ResponseMessages.SERVER_ERROR);
        SERVER_ERROR = JsonUtil.toJsonString(responseResult);
    }

    @Override
    public ModelAndView resolveException(HttpServletRequest request, HttpServletResponse response, Object handler, Exception ex) {
        LOGGER.error("发现未捕获的全局错误", ex);

        if (AjaxUtil.isAjaxRequest(request)) {
            try {
                response.setHeader("Content-Type","application/json;charset=UTF-8");
                OutputStream outputStream = response.getOutputStream();

                if (ex instanceof BusinessException) {
                    ResponseResult responseResult = new ResponseResult();
                    responseResult.setMessage(ex.getMessage());
                    outputStream.write(JsonUtil.toJsonString(responseResult).getBytes(SystemConfig.DEFAULT_ENCODING));
                } else {
                    outputStream.write(SERVER_ERROR.getBytes(SystemConfig.DEFAULT_ENCODING));
                }

                outputStream.flush();
                outputStream.close();
            } catch (IOException e) {
                LOGGER.error("返回全局异常说明时发生错误", e);
            }
        }else {
            throw new RuntimeException(ex);
        }

        return null;
    }

}
