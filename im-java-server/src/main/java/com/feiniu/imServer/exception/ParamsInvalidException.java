package com.feiniu.imServer.exception;

/**
 * Created by wangchen on 2015/2/10.
 */
public class ParamsInvalidException extends RuntimeException {
    private static final long serialVersionUID = -7768756581835965220L;

    public ParamsInvalidException(String message) {
        super(message);
    }

    public ParamsInvalidException(String message, Throwable cause) {
        super(message, cause);
    }

    public ParamsInvalidException(Throwable cause) {
        super(cause);
    }
}
