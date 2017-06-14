package com.feiniu.subAccount.exception;

/**
 * Created by wangchen on 2015/1/15.
 */
public class HttpGetException extends Exception{
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
