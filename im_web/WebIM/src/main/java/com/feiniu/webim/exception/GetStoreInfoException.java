package com.feiniu.webim.exception;

/**
 * Created by wangyixiong on 2015/12/17.
 * 获取商家信息异常
 */
public class GetStoreInfoException extends RuntimeException {
    private static final long serialVersionUID = -4733650490357390366L;

    public GetStoreInfoException() {
        super();
    }
    public GetStoreInfoException(String message) {
        super(message);
    }
    public GetStoreInfoException(Throwable cause) {
        super(cause);
    }
}
