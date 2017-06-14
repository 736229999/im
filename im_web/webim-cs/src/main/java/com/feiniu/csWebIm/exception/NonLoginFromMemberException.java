package com.feiniu.csWebIm.exception;

/**
 * Author: morningking
 * Date: 2015/11/24
 * Contact: 243717042@qq.com
 */
public class NonLoginFromMemberException extends RuntimeException{
    private static final long serialVersionUID = -1004201296292499876L;


    public NonLoginFromMemberException(String message) {
        super(message);
    }

    public NonLoginFromMemberException(String message, Throwable cause) {
        super(message, cause);
    }

    public NonLoginFromMemberException(Throwable cause) {
        super(cause);
    }
}
