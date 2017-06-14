package com.feiniu.imServer.service.kafka;

import org.springframework.stereotype.Service;

/**
 * Created by wangzhen on 2015/12/29.
 */
@Service
public class WebimMessageProducerService {
//    private static final Logger LOGGER = LoggerFactory.getLogger(WebimMessageProducerService.class);
//    @Autowired
//    BusinessConfig businessConfig;
//    //发送给webim 排队消息的producer
//    @Autowired
//    ProducerClient producerWebImClient;
//    @Autowired
//    ConsumerClient consumerImServerClient;
//    private Random random = new Random();
//    private String webImTopic = null;
//    private String imTopic = null;
////    private String webEventTopic = null;
//
//    @PostConstruct
//    private void init() throws IOException {
//        webImTopic = businessConfig.getTopicWebImShunt();
//        imTopic = businessConfig.getTopicImServerShunt();
////        webEventTopic = businessConfig.getTopicEventShunt();
//        consumerImServerClient.consume();
//    }
//
//    @PreDestroy
//    public void destroy() throws IOException {
//        producerWebImClient.close();
//    }
//
//    private void doSend(String topic, Object object) {
//        try {
//            producerWebImClient.sendMessage(topic, String.valueOf(random.nextInt()), object);
//        } catch (Exception e) {
//            LOGGER.error("向kafka发送消息失败" + topic, e);
//        }
//        return;
//    }
//
////    public void sendEventMessage(Object object) {
////        doSend(webEventTopic, object);
////    }
//
//    public void sendWebMessage(Object object) {
//        doSend(webImTopic, object);
//    }
//
//
//    //Im code
//    public void sendImMessage(Object object) {
//        doSend(imTopic, object);
//    }
//    //
}
