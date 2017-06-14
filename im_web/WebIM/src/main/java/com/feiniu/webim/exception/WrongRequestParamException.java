package com.feiniu.webim.exception;

/**
 * Created by wangchen on 2015/4/22.
 */
public class WrongRequestParamException extends RuntimeException{
    private static final long serialVersionUID = -1992606819479903032L;

    public WrongRequestParamException(String message) {
        super(message);
    }

    public WrongRequestParamException(String message, Throwable cause) {
        super(message, cause);
    }

    public WrongRequestParamException(Throwable cause) {
        super(cause);
    }

}
