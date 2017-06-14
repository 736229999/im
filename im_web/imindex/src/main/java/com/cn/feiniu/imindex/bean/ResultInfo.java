package com.cn.feiniu.imindex.bean;

import java.util.HashMap;
import java.util.Map;

/**
 * Author: morningking
 * Date: 2016/2/5
 * Contact: 243717042@qq.com
 */
public class ResultInfo {
    private boolean success;
    private String errorMessage;
    private Map<String, Object> data = new HashMap<>(1);

    public boolean isSuccess() {
        return success;
    }

    public void markSuccess() {
        this.success = true;
    }

    public String getErrorMessage() {
        return errorMessage;
    }

    public void setErrorMessage(String errorMessage) {
        this.errorMessage = errorMessage;
    }

    public Map<String, Object> getData() {
        return data;
    }

    public void addData(String key, Object value) {
        data.put(key, value);
    }
}
