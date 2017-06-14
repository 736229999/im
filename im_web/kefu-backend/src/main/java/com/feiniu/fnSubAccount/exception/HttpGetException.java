package com.feiniu.fnSubAccount.exception;

/**
 * Created by wangchen on 2015/1/15.
 */
public class HttpGetException extends Exception{
    private static final long serialVersionUID = -2275518216537518568L;

    public HttpGetException(String message) {
        super(message);
    }

    public HttpGetException(String message, Throwable cause) {
        super(message, cause);
    }

    public HttpGetException(Throwable cause) {
        super(cause);
    }
}
