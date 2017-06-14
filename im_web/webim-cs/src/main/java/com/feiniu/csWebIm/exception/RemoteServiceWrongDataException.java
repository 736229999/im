package com.feiniu.csWebIm.exception;

import java.io.Serializable;

/**
 * Author: morningking
 * Date: 2016/3/14
 * Contact: 243717042@qq.com
 */
public class RemoteServiceWrongDataException extends RuntimeException implements Serializable {
    private static final long serialVersionUID = -7966598953663825278L;

    public RemoteServiceWrongDataException() {
        super();
    }

    public RemoteServiceWrongDataException(String message) {
        super(message);
    }

    public RemoteServiceWrongDataException(String message, Throwable cause) {
        super(message, cause);
    }

    public RemoteServiceWrongDataException(Throwable cause) {
        super(cause);
    }

    protected RemoteServiceWrongDataException(String message, Throwable cause, boolean enableSuppression, boolean writableStackTrace) {
        super(message, cause, enableSuppression, writableStackTrace);
    }
}
