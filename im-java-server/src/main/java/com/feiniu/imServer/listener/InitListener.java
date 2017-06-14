package com.feiniu.imServer.listener;


import com.feiniu.imServer.service.external.kafka.ZmqPubService;
import com.feiniu.imServer.util.checkSvr.SystemEnv;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import javax.servlet.ServletContext;
import javax.servlet.ServletContextEvent;
import javax.servlet.ServletContextListener;
import java.util.Map.Entry;
import java.util.Properties;

public class InitListener implements ServletContextListener {
    private static final  Logger LOGGER = LoggerFactory.getLogger("InitListener");
    private static final String CHECK_SVR_FILE = "checksrv.properties";

    @Override
    public void contextInitialized(ServletContextEvent sce) {
        Properties propertiesCheckSvr = new Properties();
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

       try {
            ZmqPubService.instance.initEnv();
            ZmqPubService.instance.startSending();
        } catch (Exception e) {
           LOGGER.error("zmq启动失败", e);
        }

    }

    @Override
    public void contextDestroyed(ServletContextEvent sce) {
        // TODO Auto-generated method stub

        ZmqPubService.instance.stop();
    }
}