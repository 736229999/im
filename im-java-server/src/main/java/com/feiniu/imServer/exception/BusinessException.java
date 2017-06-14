package com.feiniu.imServer.exception;

/**
 * Created by wangchen on 2015/1/7.
 */
public class BusinessException extends RuntimeException {
    private static final long serialVersionUID = 5206669037090970136L;

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
