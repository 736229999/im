package com.feiniu.webim.exception;

/**
 * Created by wangyixiong on 2015/12/17.
 * imserver异常
 */
public class ImserverException extends RuntimeException {
    private static final long serialVersionUID = -7933537044377317924L;

    public ImserverException() {
        super();
    }
    public ImserverException(String message) {
        super(message);
    }
    public ImserverException(Throwable cause) {
        super(cause);
    }
}
