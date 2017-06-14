package com.feiniu.imServer.constants;

import java.util.HashMap;

/**
 * Created by Dell on 14-10-9.
 */
public final class ResponseCodes {
    private static final HashMap<String, Integer> errorMessageToCode = new HashMap<String, Integer>();
    private static final int DEFAULT_ERROR_CODES = -1;

    static {
        put(ResponseMessages.REQUEST_SUCCESS, 200);
        put(ResponseMessages.SERVER_ERROR, 500);
        put(ResponseMessages.CLIENT_ERROR, 400);
    }

    private static void put(String errorMessage, Integer errorCode) {
        errorMessageToCode.put(errorMessage, errorCode);
    }

    public static Integer getStatusCode(String errorMessage) {
        if (errorMessage != null && errorMessageToCode.containsKey(errorMessage)) {
            return errorMessageToCode.get(errorMessage);
        } else {
            return DEFAULT_ERROR_CODES;
        }
    }
}
