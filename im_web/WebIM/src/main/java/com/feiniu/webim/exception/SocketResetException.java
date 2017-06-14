package com.feiniu.webim.exception;

/**
 * Created by wangchen on 2015/1/21.
 */
public class SocketResetException extends RuntimeException{
    private static final long serialVersionUID = 2110918829500225875L;

    public SocketResetException(String message) {
        super(message);
    }

    public SocketResetException(String message, Throwable cause) {
        super(message, cause);
    }

    public SocketResetException(Throwable cause) {
        super(cause);
    }

}
