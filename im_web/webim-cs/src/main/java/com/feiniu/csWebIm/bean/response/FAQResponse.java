package com.feiniu.csWebIm.bean.response;

import com.feiniu.csWebIm.bean.domain.FAQList;

/**
 * Created by wangzhen on 2015/12/23.
 */
public class FAQResponse {
    String message;
    Boolean success;
    Integer errorCode;
    FAQList data;

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }

    public Boolean getSuccess() {
        return success;
    }

    public void setSuccess(Boolean success) {
        this.success = success;
    }

    public Integer getErrorCode() {
        return errorCode;
    }

    public void setErrorCode(Integer errorCode) {
        this.errorCode = errorCode;
    }

    public FAQList getData() {
        return data;
    }

    public void setData(FAQList data) {
        this.data = data;
    }
}
