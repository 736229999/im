package com.feiniu.webim.core;

import com.feiniu.webim.bean.domain.ResultInfo;
import com.feiniu.webim.constants.Constants;
import com.feiniu.webim.util.json.JSONUtil;
import com.feiniu.webim.util.web.WebUtil;
import org.apache.commons.lang.StringUtils;

import javax.servlet.http.HttpServletResponse;
import java.io.PrintWriter;

/**
 * 将消息输出到客户端
 *
 * @author zb
 */
public class ResponseContentGenerator {
    private static final String PAGE_ENCODING = Constants.WEB_MESSAGE_ENCODING;
    private static final String GENERAL_SERVER_ERROR;
    private static final String COMMON_SESSION_INIT_ERROR;

    static {
        ResultInfo resultInfo = new ResultInfo();
        resultInfo.setErrorMessage("服务器发生故障");

        GENERAL_SERVER_ERROR = JSONUtil.toJsonString(resultInfo);

        resultInfo.setErrorMessage("常驻用户登录失败");
        COMMON_SESSION_INIT_ERROR = JSONUtil.toJsonString(resultInfo);
    }

    private HttpServletResponse response;
    private PrintWriter writer;

    public ResponseContentGenerator(HttpServletResponse response) {
        this.response = response;

        response.setStatus(HttpServletResponse.SC_OK);
        response.setContentType("text/html;charset=UTF-8");
        WebUtil.setNoCacheHeaders(response);
    }

    /**
     * 响应成功，打印结果-------200
     */
    public void writeString(String object) {
        writer.print(object);
        writer.flush();
    }

    public void writeObject(Object object) {
        writer.print(JSONUtil.toJsonString(object));
        writer.flush();
    }

    public void writeJsonPString(String object, String callbackName) {
        writer.print(callbackName + "(");
        writer.print(object + ");");
        writer.flush();
    }

    public void writeJsonPObject(Object object, String callbackName) {
        writer.print(callbackName + "(");
        writer.print(JSONUtil.toJsonString(object));
        writer.print(");");
        writer.flush();
    }

    /**
     * 客户端非法请求 ------ 4xx
     */
    public void printClientError(String responseBody, String callback) {
        ResultInfo resultInfo = new ResultInfo();
        resultInfo.setErrorMessage(responseBody);

        if (callback != null && callback.length() > 1) {
            writeJsonPObject(resultInfo, callback);
        } else {
            writeObject(resultInfo);
        }
    }

    public void printClientError(ResultInfo resultInfo, String callback) {
        if (callback != null && callback.length() > 1) {
            writeJsonPObject(resultInfo, callback);
        } else {
            writeObject(resultInfo);
        }
    }

    /**
     * 服务端出现错误 ------ 5xx
     */
    public void printServerError(String callback) {
        if (StringUtils.isEmpty(callback)) {
            writeString(GENERAL_SERVER_ERROR);
        } else {
            writeJsonPString(GENERAL_SERVER_ERROR, callback);
        }
    }

    public void printCommonSessionInitError(String callback) {
        if (StringUtils.isEmpty(callback)) {
            writeString(COMMON_SESSION_INIT_ERROR);
        } else {
            writeJsonPString(COMMON_SESSION_INIT_ERROR, callback);
        }
    }

    public void start() {
        try {
            writer = response.getWriter();
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    public void close() {
        if (writer != null) {
            writer.flush();

            writer.close();
            writer = null;
        }
    }

}
