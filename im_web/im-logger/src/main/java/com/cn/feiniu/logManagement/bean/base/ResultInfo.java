package com.cn.feiniu.logManagement.bean.base;

import java.io.Serializable;

/**
 * Author: morningking
 * Date: 2015/7/15
 * Contact: 243717042@qq.com
 */
public class ResultInfo implements Serializable {

    private static final long serialVersionUID = 8877444575208914183L;

    private boolean success;
    private String errorMessage;

    public boolean isSuccess() {
        return success;
    }

    public void markSuccess() {
        this.success = true;
    }

    public void setSuccess(boolean success) {
        this.success = success;
    }

    public String getErrorMessage() {
        return errorMessage;
    }

    public void setErrorMessage(String errorMessage) {
        this.errorMessage = errorMessage;
    }

}
