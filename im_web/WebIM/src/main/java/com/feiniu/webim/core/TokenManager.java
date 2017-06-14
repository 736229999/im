package com.feiniu.webim.core;

import com.feiniu.webim.component.EndecryptedManager;
import com.feiniu.webim.config.ModuleNo;
import com.feiniu.webim.constants.Constants;
import com.feiniu.webim.servlet.HttpAttributeKeys;
import com.feiniu.webim.util.web.ServletRequestUtil;
import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 * Created by wangchen on 2015/5/18.
 */
public class TokenManager {
    private static final Logger LOGGER = Logger.getLogger(TokenManager.class);
    private static final String TOKEN_IDENTIFIER = "token";
    private static final String IS_MOBILE = "isMobile";
    private static final EndecryptedManager ENDECRYPTED_MANAGER = EndecryptedManager.INSTANCE;

    public static String getToken(HttpServletRequest request) {
        Object token = request.getAttribute(TOKEN_IDENTIFIER);
        if (token == null) {
            return null;
        } else {
            return token.toString();
        }
    }

    public static void putToken(HttpServletRequest request) {
        String token;

        if ((token = parseToken(request)) != null) {
            request.setAttribute(TOKEN_IDENTIFIER, token);
        }
    }

    public static String parseToken(HttpServletRequest request) {
        if (Constants.isEnvTest()) {
            return ServletRequestUtil.getString(request, "sessionID");
        } else {
            String token = "";
            boolean isMobile = ServletRequestUtil.getInteger(request, "ismobile", 0) == 1;

            //是否是无线平台
            if (isMobile) {
                token = ServletRequestUtil.getString(request, "token");
            } else {
                int moduleno = ServletRequestUtil.getInteger(request, "moduleno", 0);

                switch (ModuleNo.getModuleNo(moduleno)) {
                    case MERCHANT_WEBIM:
                        token = request.getSession().getId();
                        break;
                    case CS_WEBIM:
                        for (Cookie cookie : request.getCookies()) {
                            boolean isSessionCookie = HttpAttributeKeys.WEBIM_AUTH_TOKEN_NAME.equals(cookie.getName());

                            if (isSessionCookie) {
                                String uuid = ServletRequestUtil.getString(request, HttpAttributeKeys.ATTRIBUTE_UUID);

                                if (StringUtils.isNotEmpty(uuid)) {
                                    token = cookie.getValue();
                                    request.setAttribute(HttpAttributeKeys.WEBIM_AUTH_TOKEN_NAME, token);
                                }
                                break;
                            }
                        }
                        break;
                    default:
                        break;
                }
            }

            request.setAttribute(HttpAttributeKeys.ATTRIBUTE_UUID, ServletRequestUtil.getString(request, HttpAttributeKeys.ATTRIBUTE_UUID));
            request.setAttribute(IS_MOBILE, isMobile);

            return token;
        }
    }

    public static boolean isMobile(HttpServletRequest request) {
        Boolean isMobile = (boolean) request.getAttribute(IS_MOBILE);

        return isMobile != null && isMobile;
    }

    //删除token
    public static void invalidToken(HttpServletRequest request, HttpServletResponse response) {
        injectToken(request, response, 0);
    }

    //更新token有效期
    public static void updateTokenActiveTime(HttpServletRequest request, HttpServletResponse response) {
        injectToken(request, response, HttpAttributeKeys.MAX_COOKIE_UNACTIVE_TIME);
    }

    private static void injectToken(HttpServletRequest request, HttpServletResponse response, int maxAge) {
        String authToken = (String) request.getAttribute(HttpAttributeKeys.WEBIM_AUTH_TOKEN_NAME);
        if (StringUtils.isNotEmpty(authToken)) {
            Cookie cookie = new Cookie(HttpAttributeKeys.WEBIM_AUTH_TOKEN_NAME, authToken);
            cookie.setDomain(HttpAttributeKeys.COOKIE_DOMAIN);
            cookie.setPath(HttpAttributeKeys.WEBIM_AUTH_TOKEN_PATH);
            cookie.setMaxAge(maxAge);
            cookie.setHttpOnly(true);

            response.addCookie(cookie);
        }
    }
}
