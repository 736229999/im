package com.feiniu.webim.util.web;

import javax.servlet.ServletContext;

/**
 * Created by wangchen on 2014/12/31.
 */
public class ServletContextUtils {
    public static final Object getAttribute(ServletContext context, String attributeName) {
        return context.getAttribute(attributeName);
    }

    public static final void putAttribute(ServletContext context, String attributeName, Object value) {
        context.setAttribute(attributeName, value);
    }
}
