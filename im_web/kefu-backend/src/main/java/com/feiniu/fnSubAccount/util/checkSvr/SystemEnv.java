package com.feiniu.fnSubAccount.util.checkSvr;



import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.Properties;


public class SystemEnv {

    private static final  Logger LOGGER = LoggerFactory.getLogger("SystemEnv");

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