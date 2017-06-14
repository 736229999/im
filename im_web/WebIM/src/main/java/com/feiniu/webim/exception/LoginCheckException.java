package com.feiniu.webim.exception;

/**
 * Created by wangchen on 2015/4/22.
 */
public class LoginCheckException extends RuntimeException{
    private static final long serialVersionUID = 1795384137835343260L;

    public LoginCheckException(String message) {
        super(message);
    }

    public LoginCheckException(String message, Throwable cause) {
        super(message, cause);
    }

    public LoginCheckException(Throwable cause) {
        super(cause);
    }

}
