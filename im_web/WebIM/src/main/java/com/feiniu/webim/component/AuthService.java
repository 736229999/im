package com.feiniu.webim.component;

import com.feiniu.webim.config.ConfigManager;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
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
    public static final String TOKEN_KEY="userinfo#magicNumber#userid";
    public static final String SESSION_KEY = "sessionkey";
    public static final String AUTH_TOKEN_KEY= "YOWEJKRLWKFSJK";
    public static final String magicNumber = ConfigManager.getProperty("feiniu.magicNumber");
    public static final AuthService instance = new AuthService();
    CacheClientService cacheClientService = CacheClientService.instance;
    //校验cookie中的token
    public boolean authCookieToken(HttpServletRequest request,String serviceId){
        //口子
        if(serviceId == null)
            return false;
        //从cookie中获取auth_token
        String cookieToken= null;
        if (request.getCookies() == null){
            LOGGER.error("cookie认证失败:没有找到cookie,cookie为空");
            return false;
        }
        for(Cookie cookie: request.getCookies())
        {
            if(cookie.getName().equalsIgnoreCase(AUTH_TOKEN_KEY)){ //获取键
                cookieToken = URLDecoder.decode(cookie.getValue().toString()).replaceAll(" ", "+");
                break;
            }
        }

        if ( cookieToken == null) {
            LOGGER.error("cookie认证失败:cookie找到了，但是cookie里面没有auth_token(YOWEJKRLWKFSJK)字段");
            return false;
        }

        //从memcache或者imserver中获取token,不维护本地cache，否则导致生命周期不一致
        String authKey = TOKEN_KEY.replace("userid", serviceId).replace("magicNumber", magicNumber);

        String sessionKey = null;
        try {
            sessionKey = cacheClientService.getHFieldValue(authKey, SESSION_KEY);
        } catch (Exception e) {
            e.printStackTrace();
        }
        if (sessionKey == null) {
            LOGGER.error("权限认证失败， auth_key :" + authKey);
            LOGGER.error("权限认证失败，memcache和imserver中没找到token");
            return false;
        }

        //cacheToken与cookieToken比较
        if (sessionKey.equals(cookieToken )) {
            return true;
        }
        else{
            LOGGER.error("cookie认证失败:客服ID :"+serviceId);
            LOGGER.error("cookie认证失败:cached token :"+ sessionKey);
            LOGGER.error("cookie认证失败:params token :" + cookieToken );
            LOGGER.error("cookie认证失败:验证码不匹配");
            return false;
        }
    }

}
