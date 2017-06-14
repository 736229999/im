package com.feiniu.webim.exception;

/**
 * Created by wangchen on 2015/3/12.
 */
public class ChatSessionNotExistException extends RuntimeException {
    private static final long serialVersionUID = 2741042270032528443L;

    public ChatSessionNotExistException() {
        super();
    }

    public ChatSessionNotExistException(String message) {
        super(message);
    }
}
