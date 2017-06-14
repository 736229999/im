package com.feiniu.csWebIm.base;

import java.util.HashMap;
import java.util.Map;

public class ResponseResult implements Cloneable {
    private String message;
    private boolean success;
    private int errorCode = 0;
    private Map<String, Object> data = new HashMap<String, Object>();
    private boolean needLogin;

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }

    public Map<String, Object> getData() {
        return data;
    }

    public void addData(String dataKey, Object value) {
        this.data.put(dataKey, value);
    }

    public boolean isSuccess() {
        return success;
    }

    public void markSuccess() {
        this.success = true;
    }

    public int getErrorCode() {
        return errorCode;
    }

    public void setErrorCode(int errorCode) {
        this.errorCode = errorCode;
    }

    public boolean isNeedLogin() {
        return needLogin;
    }

    public void setNeedLogin(boolean needLogin) {
        this.needLogin = needLogin;
    }

    @Override
    public Object clone() throws CloneNotSupportedException {
        ResponseResult responseResult = new ResponseResult();

        responseResult.setErrorCode(getErrorCode());
        responseResult.setMessage(responseResult.getMessage());
        responseResult.data = new HashMap<>(data);

        return responseResult;
    }
}
