package com.feiniu.csWebIm.service;

import com.feiniu.csWebIm.consts.SystemConfig;
import com.feiniu.csWebIm.service.external.WebImAuthServie;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;
import java.net.URLDecoder;

/**
 * Created by wangzhen on 2015/12/17.
 */
@Service
public class AuthService {
    private static final Logger LOGGER = LoggerFactory.getLogger("AuthService");

    @Autowired
    WebImAuthServie webImAuthServie;

    public  boolean authCookieToken(HttpServletRequest request,String serviceId){

        //从cookie中获取auth_token
        String auth_token = null;
        if (request.getCookies() == null){
            LOGGER.error("cookie认证失败:没有找到cookie,cookie为空");
            return false;
        }

        Cookie[] cookie = request.getCookies();
        for (int i = 0; i < cookie.length; i++) {
            Cookie cook = cookie[i];
            if(cook.getName().equalsIgnoreCase(SystemConfig.AUTH_TOKEN_KEY)){ //获取键
                auth_token = URLDecoder.decode(cook.getValue().toString()).replaceAll(" ", "+");
            }
        }

        if (auth_token == null) {
            LOGGER.error("cookie认证失败:cookie找到了，但是cookie里面没有auth_token(YOWEJKRLWKFSJK)字段");
            return false;
        }

        //从memcache或者imserver中获取token,不维护本地cache，否则导致生命周期不一致
        String cachedToken = webImAuthServie.getCachedToken(serviceId);

        if (auth_token.equals(cachedToken) ) {
            return true;
        }
        else{
            LOGGER.error("cookie认证失败:客服ID :"+serviceId);
            LOGGER.error("cookie认证失败:cached token :"+ cachedToken);
            LOGGER.error("cookie认证失败:params token :" + auth_token);
            LOGGER.error("cookie认证失败:验证码不匹配");
            return false;
        }
    }
}
