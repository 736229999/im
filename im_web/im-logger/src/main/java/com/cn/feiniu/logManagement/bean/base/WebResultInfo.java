package com.cn.feiniu.logManagement.bean.base;

import java.util.HashMap;
import java.util.Map;

/**
 * Author: morningking
 * Date: 2015/8/5
 * Contact: 243717042@qq.com
 */
public class WebResultInfo {
    private boolean success;
    private Map<String, Object> data = new HashMap<>();
    private String errorMessage;

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
}
