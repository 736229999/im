package com.feiniu.webim.util.web;

import com.feiniu.webim.config.ConfigManager;
import com.feiniu.webim.config.SystemConfig;
import org.apache.commons.lang.ArrayUtils;
import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletResponse;
import java.io.UnsupportedEncodingException;
import java.net.InetAddress;
import java.net.URLDecoder;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Random;
import java.util.UUID;
import java.util.regex.Pattern;

public class WebUtil {

    private static final Logger LOGGER = Logger.getLogger(WebUtil.class);
    private static SimpleDateFormat dateFormat = null;
    private static Random rand = new Random();
    private static final Pattern UUID_PATTERN = Pattern.compile("\\w{8}-(?:\\w{4}-){3}\\w{12}");
    private static final String LOCAL_MACHINE_IP_ADDRESS;

    static {
        dateFormat = new SimpleDateFormat(ConfigManager.getProperty(SystemConfig.DATEFORMAT.toString(), "HH:mm:ss"));

        InetAddress ia = null;
        try {
            ia = ia.getLocalHost();

            LOCAL_MACHINE_IP_ADDRESS = ia.getHostAddress();
        } catch (Exception e) {
            LOGGER.error("获取本机ip出错", e);
            throw new Error("获取本机ip出错");
        }
    }

    /**
     * 使用默认格式,格式化时间
     *
     * @return 时间字符串
     */
    public static String dateFormat() {
        return dateFormat.format(new Date());
    }

    /**
     * 指定格式,格式化时间
     *
     * @param format 时间字符串
     * @return
     */
    public static String dateFormat(String format) {
        return new SimpleDateFormat(format).format(new Date());
    }

    /**
     * uuid
     */
    public static String uuid() {
        return UUID.randomUUID().toString();
    }

    public static boolean isUUID(String uuid) {
        return StringUtils.isNotEmpty(uuid) && uuid.length() == 36 && UUID_PATTERN.matcher(uuid).matches();
    }

    /**
     * currentTimeMillis
     */
    public static long now() {
        return System.currentTimeMillis();
    }

    public static int randId() {
        return rand.nextInt(10000);
    }

    public static String getNewString(String oldStr) {
        if (oldStr == null) {
            return null;
        }
        try {
            return new String(oldStr.getBytes("ISO-8859-1"), "UTF-8");
        } catch (UnsupportedEncodingException e) {
            //e.printStackTrace();
            LOGGER.warn("WebUtil,不支持的字符集");
        }
        return null;
    }

    public static String getFieldFromCookie(String fieldName, Cookie[] cookies) {
        if (StringUtils.isNotEmpty(fieldName) && ArrayUtils.isNotEmpty(cookies)) {
            for (Cookie cookie : cookies) {
                if (fieldName.equals(cookie.getName())) {
                    return cookie.getValue();
                }
            }
        }

        return null;
    }

    /**
     * 无缓存
     *
     * @param aResponse
     */
    public static void setNoCacheHeaders(HttpServletResponse aResponse) {
        // Set to expire far in the past.
        aResponse.setHeader("Expires", "Sat, 6 May 1995 12:00:00 GMT");

        // Set standard HTTP/1.1 no-cache headers.
        aResponse.setHeader("Cache-Control", "no-store, no-cache, must-revalidate");

        // Set IE extended HTTP/1.1 no-cache headers (use addHeader).
        aResponse.addHeader("Cache-Control", "post-check=0, pre-check=0");

        // Set standard HTTP/1.0 no-cache header.
        aResponse.setHeader("Pragma", "no-cache");

    }

    public static String decode(String message, String encoding) {
        try {
            return URLDecoder.decode(message, encoding);
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    public static final String getLocalIP() {
        return LOCAL_MACHINE_IP_ADDRESS;
    }
}
