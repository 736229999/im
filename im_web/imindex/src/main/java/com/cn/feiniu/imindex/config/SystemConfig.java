package com.cn.feiniu.imindex.config;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Component;

import java.util.Properties;

/**
 * Created by wangchen on 2015/6/25.
 */
@Component
public class SystemConfig {
    public volatile static String VERSION;
    public volatile static String SIZE;
    public volatile static String UPDATE_DATE;
    public volatile static String FILE_NAME;

    @Value("#{appSettings}")
    private void setProperties(Properties properties) {
        init(properties);
    }

    private static void init(Properties properties) {
        System.out.println("hre");
        VERSION = properties.getProperty("app.version");
        SIZE = properties.getProperty("app.size");
        UPDATE_DATE = properties.getProperty("app.update_date");
        FILE_NAME = properties.getProperty("app.file_name");
    }
}
