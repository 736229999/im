package com.feiniu.csWebIm.exception;

import com.feiniu.csWebIm.base.ResponseResult;
import com.feiniu.csWebIm.utils.json.JsonUtil;
import com.feiniu.csWebIm.utils.web.AjaxUtil;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.web.servlet.HandlerExceptionResolver;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;

/**
 * Author: morningking
 * Date: 2015/11/24
 * Contact: 243717042@qq.com
 */
public class GlobalExceptionHandler implements
        HandlerExceptionResolver {
    private static final String SERVER_ERROR;
    private static final ResponseResult SERVER_ERROR_INFO;
    private static final Logger LOGGER = LoggerFactory.getLogger(GlobalExceptionHandler.class);

    static {
        SERVER_ERROR_INFO = new ResponseResult();
        SERVER_ERROR_INFO.setMessage("系统发生故障");
        SERVER_ERROR = JsonUtil.toJsonString(SERVER_ERROR_INFO);
    }

    @Override
    public ModelAndView resolveException(HttpServletRequest request, HttpServletResponse response, Object handler, Exception ex) {
        LOGGER.error("发现未捕获的全局错误", ex);

        if (AjaxUtil.isAjaxRequest(request)) {
            response.setContentType("application/json;charset=UTF-8");
            try {
                PrintWriter writer = response.getWriter();
                String result = SERVER_ERROR;

                if (ex instanceof NonLoginFromMemberException) {
                    try {
                        ResponseResult currentResultInfo = (ResponseResult) (SERVER_ERROR_INFO.clone());
                        currentResultInfo.setNeedLogin(true);
                        currentResultInfo.setMessage(ex.getMessage());

                        result = JsonUtil.toJsonString(currentResultInfo);
                    } catch (CloneNotSupportedException e) {
                        //omit
                    }
                }

                writer.write(result);
                writer.flush();
            } catch (IOException e) {
                LOGGER.error("返回全局异常说明时发生错误", e);
            }
        } else {
            return new ModelAndView("error/500");
        }

        return null;
    }

}
