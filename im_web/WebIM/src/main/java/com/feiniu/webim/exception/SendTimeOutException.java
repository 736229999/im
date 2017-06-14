package com.feiniu.webim.exception;

/**
 * Created by wangchen on 2015/1/20.
 */
public class SendTimeOutException extends RuntimeException{
    private static final long serialVersionUID = 8514642679414467830L;

    public SendTimeOutException(String message) {
        super(message);
    }

    public SendTimeOutException(String message, Throwable cause) {
        super(message, cause);
    }

    public SendTimeOutException(Throwable cause) {
        super(cause);
    }
}
