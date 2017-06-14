package com.feiniu.webim.config;

import com.feiniu.webim.util.checkSvr.SystemEnv;
import com.feiniu.webim.util.general.FormatUtils;
import org.apache.log4j.Logger;

import java.io.File;
import java.io.FileInputStream;
import java.util.Date;
import java.util.Properties;

/**
 * 全局配置管理
 *
 * @author zb
 */
public class ConfigManager {
    public static final Logger LOGGER = Logger.getLogger(ConfigManager.class);
    private static final String PROPERTIES_FILE = "webim.xml";
    private static final String CHECK_SVR_FILE = "checksrv.properties";
    private static Properties properties;
    private static Properties propertiesCheckSvr;
    /**
     * 加载配置文件
     *
     * @param webPath 文件加载路径
     */
    public static void load(String webPath) {
        properties = new Properties();
        propertiesCheckSvr = new Properties();
        try {
            XMLConfig xmlConfig = new XMLConfig();
            xmlConfig.load(ConfigManager.class.getClassLoader().getResourceAsStream(PROPERTIES_FILE));

            properties.load(new FileInputStream(xmlConfig.getProperty("preview_properties")));

            propertiesCheckSvr.load(ConfigManager.class.getClassLoader().getResourceAsStream(CHECK_SVR_FILE));

            SystemEnv.addProperty(propertiesCheckSvr);

            if (properties.containsKey("webPath")) {
                webPath = properties.getProperty("webPath");
            }

            properties.setProperty("webPath", webPath + File.separator);

        } catch (Exception e) {
            e.printStackTrace();
            LOGGER.info("loading checksrv.properties failed==============");
            //这里的错误处理(Log尚未初始化....)
            System.out.println("无法从CLASSPATH中加载配置文件:" + PROPERTIES_FILE);
        }
    }

    /**
     * 配置参数值为null,返回默认
     *
     * @param name       参数名
     * @param valDefault 参数值
     * @return 参数值/默认值
     */
    public static String getProperty(String name, String valDefault) {
        String val = properties.getProperty(name);
        return (val == null) ? valDefault : val;
    }

    /**
     * 根据name获取配置参数
     *
     * @param name 参数名
     * @return 参数值
     */
    public static String getProperty(String name) {
        String value = properties.getProperty(name);

        if (value == null) {
            System.err.println(FormatUtils.formatTime(new Date()) + "：!!!!!!!!!未知 属性: " + name);
            throw new Error("未知 属性: " + name);
        }

        return value;
    }

    /**
     * 返回long类型配置参数
     *
     * @param name 参数名
     * @return 参数值
     */
    public static Long getLongProperty(String name) {
        String value = getProperty(name);
        try {
            return Long.parseLong(value);
        } catch (Throwable t) {
            throw new Error("非法 属性 value: " + name + " val=" + value);
        }
    }

    public static Integer getIntProperty(String name) {
        String value = getProperty(name);

        try {
            return Integer.parseInt(value);
        } catch (Throwable t) {
            throw new Error("非法 属性 value: " + name + " val=" + value);
        }
    }
}
