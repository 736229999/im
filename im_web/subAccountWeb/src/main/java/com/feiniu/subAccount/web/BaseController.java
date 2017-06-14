package com.feiniu.subAccount.web;

import com.feiniu.core.base.bean.Auth;
import com.feiniu.core.web.util.CoreWebConstant;
import com.feiniu.core.web.util.CookieUtil;

import javax.servlet.http.HttpServletRequest;

/**
 * Created by wangchen on 2015/2/12.
 */
public abstract class BaseController {
    protected String getAuthToken(HttpServletRequest request) {
        return CookieUtil.getCookieValue(request, "AUTH_TOKEN");
    }

    protected Auth getAuth(HttpServletRequest request) {
        Auth result = (Auth) request.getAttribute(CoreWebConstant.AUTH_ATTRNAME);
        return result;
    }
}
