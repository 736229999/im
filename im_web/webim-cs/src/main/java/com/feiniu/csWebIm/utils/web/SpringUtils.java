package com.feiniu.csWebIm.utils.web;

import org.springframework.web.context.support.WebApplicationContextUtils;

import javax.servlet.ServletContext;

/**
 * Author: morningking
 * Date: 2015/12/23
 * Contact: 243717042@qq.com
 */
public class SpringUtils {
    public static final <T> T getBean(ServletContext context, Class<T> clazz) {
        return WebApplicationContextUtils.getWebApplicationContext(context).getBean(clazz);
    }
}
