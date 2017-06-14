package com.feiniu.webim.exception;

/**
 * Created by wangchen on 2015/1/7.
 */
public class BusinessException extends RuntimeException {
    private static final long serialVersionUID = -6828086916855873683L;

    public BusinessException(String message) {
        super(message);
    }

    public BusinessException(String message, Throwable cause) {
        super(message, cause);
    }

    public BusinessException(Throwable cause) {
        super(cause);
    }
}
