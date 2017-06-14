package com.feiniu.fnSubAccount.util;

import org.springframework.web.context.support.WebApplicationContextUtils;

import javax.servlet.ServletContext;
import javax.servlet.http.HttpServletRequest;

/**
 * Created by morningking on 2014/12/14.
 */
public class WebUtil {
    public static final String getMetaPath(HttpServletRequest request) {
        String requestURL = request.getRequestURL().toString();
        int firstOccuranceOfSlash = requestURL.indexOf('/', 7) + 1;
        String metaPath = requestURL.substring(0, requestURL.indexOf('/', firstOccuranceOfSlash));

        return metaPath;
    }

    public static final <T> T getBean(Class<T> clazz, ServletContext context) {
        return WebApplicationContextUtils.getWebApplicationContext(context).getBean(clazz);
    }
}
