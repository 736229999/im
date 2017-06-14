package com.feiniu.webim.core;

import com.feiniu.webim.bean.domain.ResultInfo;
import com.feiniu.webim.config.RequestParams;
import com.feiniu.webim.servlet.HttpAttributeKeys;
import com.feiniu.webim.util.web.ServletRequestUtil;
import com.feiniu.webim.util.web.WebUtil;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 * Created by wangchen on 2014/12/8.
 */
public class WebAction {
    private Session session;
    private boolean needRefreshSession;
    private boolean needRefreshToken = true;
    private HttpServletRequest request;
    private HttpServletResponse response;
    private ResponseContentGenerator responseContentGenerator;
    private ResultInfo resultInfo;

    public Session getSession() {
        return session;
    }

    public void setSession(Session session) {
        this.session = session;
    }

    public HttpServletRequest getRequest() {
        return request;
    }

    public void setRequest(HttpServletRequest request) {
        this.request = request;
    }

    public HttpServletResponse getResponse() {
        return response;
    }

    public void setResponse(HttpServletResponse response) {
        this.response = response;
    }

    public void setResponseContentGenerator(ResponseContentGenerator responseContentGenerator) {
        this.responseContentGenerator = responseContentGenerator;
    }

    public ResponseContentGenerator getResponseContentGenerator() {
        return responseContentGenerator;
    }

    public ResultInfo getResultInfo() {
        return resultInfo;
    }

    public void setResultInfo(ResultInfo resultInfo) {
        this.resultInfo = resultInfo;
    }

    public boolean isNeedRefreshSession() {
        return needRefreshSession;
    }

    public void setNeedRefreshSession(boolean needRefreshSession) {
        this.needRefreshSession = needRefreshSession;
    }

    public void setNeedRefreshToken(boolean needRefreshToken) {
        this.needRefreshToken = needRefreshToken;
    }

    public void startAction() {
        updateChatWindowInfo();

        if (needRefreshSession && session != null) {
            session.updateTime();
        }

        responseContentGenerator.start();

        String callback = ServletRequestUtil.getString(request, RequestParams.CALLBACK.toString());
        if (callback != null && callback != "") {
            responseContentGenerator.writeString(callback + "(");
        }
    }

    public void printResult() {
        String uuid = (String) request.getAttribute(HttpAttributeKeys.ATTRIBUTE_UUID);

        if (session != null && uuid != null && session.isUuidFromCS(uuid)) {
            if (needRefreshToken) {
                TokenManager.updateTokenActiveTime(request, response);
            } else {
                TokenManager.invalidToken(request, response);
            }
        }

        responseContentGenerator.writeObject(resultInfo);
    }

    public void finishAction() {
        updateChatWindowInfo();

        if (needRefreshSession && session != null) {
            session.updateTime();
        }

        String callback = ServletRequestUtil.getString(request, RequestParams.CALLBACK.toString());
        if (callback != null && callback != "") {
            responseContentGenerator.writeString(");");
        }

        responseContentGenerator.close();
    }

    private void updateChatWindowInfo() {
        String uuid = (String) request.getAttribute(HttpAttributeKeys.ATTRIBUTE_UUID);
        if (WebUtil.isUUID(uuid)) {
            session.updateChatWindowActiveTime(uuid);
        }
    }
}
