package com.feiniu.subAccount.listener;

import com.feiniu.subAccount.util.checkSvr.SystemEnv;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import javax.servlet.ServletContext;
import javax.servlet.ServletContextEvent;
import javax.servlet.ServletContextListener;
import javax.servlet.annotation.WebListener;
import java.util.Map.Entry;
import java.util.Properties;

/**
 * webim 启动加载监听
 *
 */
@WebListener
public class InitListener implements ServletContextListener {
    private static final  Logger LOGGER = LoggerFactory.getLogger("InitListener");
    private static Properties propertiesCheckSvr;
    private static final String CHECK_SVR_FILE = "checksrv.properties";

    @Override
    public void contextInitialized(ServletContextEvent sce) {
        propertiesCheckSvr = new Properties();
        try {
            propertiesCheckSvr.load(InitListener.class.getClassLoader().getResourceAsStream(CHECK_SVR_FILE));
            SystemEnv.addProperty(propertiesCheckSvr);
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println("无法从CLASSPATH中加载配置文件:" + CHECK_SVR_FILE);
        }

        ServletContext application = sce.getServletContext();
        Properties systemProperties = SystemEnv.getProperties();
        for (Entry<Object, Object> entry : systemProperties.entrySet()) {
            String name = (String) entry.getKey();
            name = name.replace(".", "_");
            Object value = entry.getValue();
            application.setAttribute(name, value);
            LOGGER.info("Add system property to application: " + name + "=" + value);
        }
    }

    @Override
    public void contextDestroyed(ServletContextEvent sce) {
        // TODO Auto-generated method stub

    }
}
