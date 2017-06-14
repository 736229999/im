package com.feiniu.webim.bean.domain;

import com.feiniu.webim.constants.ErrorCode;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by wangchen on 2014/12/16.
 */
public class ResultInfo {
    private boolean success;
    private Map<String, Object> data = new HashMap<>();
    private String errorMessage;
    private boolean needLogin = false;//是否需要重新登录
    private Integer errorCode = ErrorCode.ERRORCODE_NORMAL;//错误码

    public boolean isSuccess() {
        return success;
    }

    public void markSuccess() {
        this.success = true;
    }

    public Map<String, Object> getData() {
        return data;
    }

    public void addData(String key, Object data) {
        this.data.put(key, data);
    }

    public String getErrorMessage() {
        return errorMessage;
    }

    public void setErrorMessage(String errorMessage) {
        this.errorMessage = errorMessage;
    }

    public boolean isNeedLogin() {
        return needLogin;
    }

    public void setNeedLogin(boolean needLogin) {
        this.needLogin = needLogin;
    }

    public Integer getErrorCode() {
        return errorCode;
    }

    public void setErrorCode(Integer errorCode) {
        this.errorCode = errorCode;
    }
}
