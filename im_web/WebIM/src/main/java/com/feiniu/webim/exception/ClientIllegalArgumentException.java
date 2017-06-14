package com.feiniu.webim.exception;

/**
 * Author: morningking
 * Date: 2015/12/25
 * Contact: 243717042@qq.com
 */
public class ClientIllegalArgumentException extends RuntimeException {
    private static final long serialVersionUID = -5302028486425430868L;

    public ClientIllegalArgumentException() {
    }

    public ClientIllegalArgumentException(String message) {
        super(message);
    }

    public ClientIllegalArgumentException(Throwable cause) {
        super(cause);
    }

    public ClientIllegalArgumentException(String message, Throwable cause) {
        super(message, cause);
    }
}
