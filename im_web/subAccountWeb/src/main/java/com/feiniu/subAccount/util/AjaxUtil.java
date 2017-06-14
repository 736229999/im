package com.feiniu.subAccount.util;

import javax.servlet.http.HttpServletRequest;

/**
 * Created by morningking on 2015/1/6.
 */
public class AjaxUtil {
    public static final boolean isAjaxRequest(HttpServletRequest request) {
        boolean result = !(request.getHeader("accept") != null
                && request.getHeader("accept").indexOf("application/json") > -1 || (request
                .getHeader("X-Requested-With") != null && request
                .getHeader("X-Requested-With").indexOf("XMLHttpRequest") > -1));

        return !result;
    }
}
