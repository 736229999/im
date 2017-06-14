package com.cn.feiniu.logManagement.util.web;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.net.InetAddress;

public class WebUtil {

    private static final Logger LOGGER = LoggerFactory.getLogger(WebUtil.class);
    private static final String LOCAL_MACHINE_IP_ADDRESS;

    static {
        InetAddress ia = null;
        try {
            ia = ia.getLocalHost();

            LOCAL_MACHINE_IP_ADDRESS = ia.getHostAddress();
        } catch (Exception e) {
            LOGGER.error("获取本机ip出错", e);
            throw new Error("获取本机ip出错");
        }
    }

    public static final String getLocalIP() {
        return LOCAL_MACHINE_IP_ADDRESS;
    }
}
