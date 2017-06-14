
package com.feiniu.subAccount.base;

import java.util.HashMap;
import java.util.Map;

public class ResponseResult {
    private String message;
    private boolean success;
    private Map<String, Object> data = new HashMap<String, Object>();

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
}
