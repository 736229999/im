package com.feiniu.webim.exception;

/**
 * Created by wangchen on 2015/1/15.
 */
public class HttpGetException extends Exception{
    private static final long serialVersionUID = 6766136845346197749L;

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
