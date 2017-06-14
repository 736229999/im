package com.feiniu.webim.listener;


import com.feiniu.kafka.client.ConsumerClient;
import com.feiniu.webim.bean.domain.KafkaConfig;
import com.feiniu.webim.communication.CommunicationManager;
import com.feiniu.webim.component.ZmqTopicSubscriber;
import com.feiniu.webim.config.ConfigManager;
import com.feiniu.webim.config.ZMQConfig;
import com.feiniu.webim.core.SessionManager;
import com.feiniu.webim.statistics.RequestStatistics;
import com.feiniu.webim.statistics.RequestTimeDetailStatistics;
import com.feiniu.webim.statistics.ResponseStatistics;
import com.feiniu.webim.util.checkSvr.SystemEnv;
import com.feiniu.webim.util.log.Log;
import com.feiniu.webim.util.web.WebUtil;
import org.apache.log4j.Logger;
import org.springframework.context.ApplicationContext;
import org.springframework.context.ConfigurableApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;

import javax.servlet.ServletContext;
import javax.servlet.ServletContextEvent;
import javax.servlet.ServletContextListener;
import javax.servlet.annotation.WebListener;
import java.util.*;
import java.util.concurrent.TimeUnit;

/**
 * webim 启动加载监听
 *
 * @author zb
 */
@WebListener
public class InitListener implements ServletContextListener {
    private ApplicationContext SPRING_CONTEXT;
    private final List<ZmqTopicSubscriber> subscriberList = Collections.synchronizedList(new ArrayList<ZmqTopicSubscriber>(2));
    private static final Logger LOGGER = Logger.getLogger(InitListener.class);

    @Override
    public void contextInitialized(ServletContextEvent sce) {
        try {
            String webPath = sce.getServletContext().getRealPath("/") + "WEB-INF/classes";
            //从classpath下加载配置文件
            ConfigManager.load(webPath);

            SessionManager sessionManager = SessionManager.getInstance();
            CommunicationManager communicationManager = CommunicationManager.instance;

            initCheckSrv(sce);
            Log.init();
            initStatistics();

            sessionManager.start();
            communicationManager.setServletContext(sce.getServletContext());
            communicationManager.initCommunicationEnv();
            System.out.println("webim 启动,webPath:" + webPath);
        } catch (RuntimeException e) {
            e.printStackTrace();
            throw new Error("启动失败");
        }

        try {
//            initKafka();
        } catch (RuntimeException e) {
            e.printStackTrace();
        }

        try {
            initZMQ();
        } catch (RuntimeException e) {
            e.printStackTrace();
        }
    }

    private void initCheckSrv(ServletContextEvent sce) {
        ServletContext application = sce.getServletContext();
        Properties systemProperties = SystemEnv.getProperties();
        for (Map.Entry<Object, Object> entry : systemProperties.entrySet()) {
            String name = (String) entry.getKey();
            name = name.replace(".", "_");
            Object value = entry.getValue();
            application.setAttribute(name, value);
            System.out.println("Add system property to application: " + name + "=" + value);
        }
    }

    private void initStatistics() {
        RequestStatistics.init();
        ResponseStatistics.init();
        RequestTimeDetailStatistics.init();
    }

    private void initKafka() {
        SPRING_CONTEXT = new ClassPathXmlApplicationContext("/spring/application-context.xml");
        KafkaConfig kafkaConfig = SPRING_CONTEXT.getBean(KafkaConfig.class);
        Properties clientProperties = new Properties();
        clientProperties.setProperty("zookeeper.connection.timeout.ms", kafkaConfig.getTimeoutMilis() + "");
        clientProperties.setProperty("auto.offset.reset", kafkaConfig.getOffsetReset());

        String consumerGroup = kafkaConfig.getGroupId() + "_" + WebUtil.getLocalIP();
        System.out.println("Kafka Consumer GroupName Set To: " + consumerGroup);

        ConsumerClient consumerClient =
                new ConsumerClient(kafkaConfig.getZookeeperConnectString(), consumerGroup, clientProperties,
                        kafkaConfig.getTopicName(), kafkaConfig.getKafkaStreamHandler());

        consumerClient.consume();
    }

    private void closeKafka() {
        ((ConfigurableApplicationContext) SPRING_CONTEXT).close();
    }

    private void initZMQ() {
        String[] pubDescriptorArray = ConfigManager.getProperty(ZMQConfig.ZMQ_QUEUE_INFO_PUB_DESCRIPTOR.toString()).split(",");
        String topic = ConfigManager.getProperty(ZMQConfig.ZMQ_QUEUE_INFO_PUB_TOPIC.toString());

        for (int i = 0; i < pubDescriptorArray.length; i++) {
            String pubDescriptor = pubDescriptorArray[i];

            final ZmqTopicSubscriber subscriber = ZmqTopicSubscriber.getSubscriber(pubDescriptor, topic);
            subscriber.setSubscriberNo(i);
            subscriberList.add(subscriber);

            new Thread(new Runnable() {
                @Override
                public void run() {
                    subscriber.doWork();
                }
            }, "zmq-consumer-" + i).start();
        }
    }

    private void closeZMQ() {
        LOGGER.info("close zmq now...");

        for (ZmqTopicSubscriber subscriber : subscriberList) {
            subscriber.close();
        }
    }

    @Override
    public void contextDestroyed(ServletContextEvent sce) {
        SessionManager.getInstance().stop();

        try {
            TimeUnit.SECONDS.sleep(2);
        } catch (Exception e) {
            //omit exception
        }

        CommunicationManager.instance.stop();
        closeKafka();
        closeZMQ();

        System.out.println("webim 停止");
    }
}
