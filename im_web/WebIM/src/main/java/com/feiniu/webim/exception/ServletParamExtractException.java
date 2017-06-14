package com.feiniu.webim.exception;

/**
 * Created by wangchen on 2015/3/11.
 */
public class ServletParamExtractException extends ClientIllegalArgumentException {
    private static final long serialVersionUID = -5527294040132922232L;

    public ServletParamExtractException() {
        super();
    }

    public ServletParamExtractException(String message) {
        super(message);
    }

    public ServletParamExtractException(String message, Throwable cause) {
        super(message, cause);
    }
}
