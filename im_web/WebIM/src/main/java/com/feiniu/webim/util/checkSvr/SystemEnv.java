package com.feiniu.webim.util.checkSvr;

import org.apache.log4j.Logger;

import java.util.Properties;


public class SystemEnv {

    public static final Logger LOGGER = Logger.getLogger(SystemEnv.class);

    private static Properties systemProperties = new Properties();

    public static void addProperty(Properties properties) {
        if (properties != null) {
            for (Object key : properties.keySet()) {
                if (key != null) {
                    if (systemProperties.keySet().contains(key)) {
                        LOGGER.error("系统Property配置项 {} 重复");
                    }
                    systemProperties.put(key, properties.get(key));
                }
            }
        }
    }

    public static String getProperty(String key) {
        return systemProperties.getProperty(key);
    }

    public static Properties getProperties() {
        return systemProperties;
    }
}
