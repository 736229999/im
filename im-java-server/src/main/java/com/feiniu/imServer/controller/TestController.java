package com.feiniu.imServer.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;

/**
 * Created by wangzhen on 2016/2/17.
 */
@Controller
@RequestMapping(value = "test")
public class TestController {
//    @Autowired
//    CacheClientService cacheClientService;
//    @Autowired
//    RedisCacheClient redisCacheClient;
//    @Autowired
//    WebimMessageProducerService webimMessageProducerService;
//    @Autowired
//    ShuntService shuntService;
//    @Autowired
//    KefuQueueInfoService kefuQueueInfoService;
//
//
//    private volatile boolean isContinue = true;
//    private volatile Thread receivingThread = null;
//
//    private static final Logger LOGGER = LoggerFactory.getLogger(TestController.class);
//
//
//    @RequestMapping(value = "test-sending-thread-stop")
//    @ResponseBody
//    public ResponseResult testSendingThreadStop() {
//        if (receivingThread != null) {
//            receivingThread.interrupt();
//        }
//        return null;
//    }
//
//    @RequestMapping(value = "test-sending-thread-start")
//    @ResponseBody
//    public ResponseResult testSendingThreadStart() {
//        if (receivingThread != null) return null;
//
//        receivingThread = new Thread(new Runnable() {
//            @Override
//            public void run() {
//                while (isContinue) {
//                    try {
//                        testSendKafkaMessage(Long.valueOf(111111), Long.valueOf(11), Long.valueOf("111111"),
//                               ImServerEventTypeEnum.EVENT_SERVICE_ON_LINE.getId()
//                                , 2, System.currentTimeMillis());
//                        Thread.sleep(10000);
//                    } catch (Exception e) {
//                        LOGGER.error("处理消息失败", e);
//                    }
//                }
//            }
//        }, "转发线程");
//
//        receivingThread.start();
//        LOGGER.info("初始化转发线程成功");
//
//        return null;
//    }
//
//    //测试发送数据异动kafka消息
//    @RequestMapping(value = "test-send-kafka-message")
//    @ResponseBody
//    public ResponseResult testSendKafkaMessage(Long customerId, Long groupId, Long serviceId, Integer enent_type, Integer userLevel, Long requestTime) {
//        try {
//            ImServerEvent testMessage = new ImServerEvent();
//            testMessage.setCustomerId(customerId);
//            testMessage.setEventType(enent_type);
//            testMessage.setGroupId(groupId);
//            testMessage.setServiceId(serviceId);
//            testMessage.setCustomerId(customerId);
//            testMessage.setRequestTime(requestTime);
//            testMessage.setUserLevel(userLevel);
//            testMessage.setLogInfo("发送模拟消息时间: " + System.currentTimeMillis() + "\n");
//            webimMessageProducerService.sendImMessage(JsonUtil.toJsonString(testMessage));
//            LOGGER.error("send test message done");
//        } catch (Exception e) {
//
//            LOGGER.error("send kafka message ERROR");
//        }
//
//        return null;
//    }
//
//    //测试获取排队消息
//    @RequestMapping(value = "test-get-queue-info")
//    @ResponseBody
//    public ResponseResult testGetQueueInfo(Long groupId) {
//        if (groupId == null)
//            return null;
//
//
//        ResponseResult responseResult = new ResponseResult();
//        Set<String> infoSet = kefuQueueInfoService.getQueueInfoInGroupId(groupId);
//        responseResult.markSuccess();
//        responseResult.addData("info", infoSet);
//
//        return responseResult;
//    }
//
//    public void addUserInfo(String userid, String csId, String groupId, String receptionLimit) {
////        //20000001 的用户上下文，被111111客服接待
////        redisCacheClient.del("userinfo#3333#" + userid);
////        CachedUserInfo userInfo = new CachedUserInfo();
////        userInfo.setId(userid);
////        userInfo.setUsername(userid);
////        userInfo.setCsTeamID(groupId);
////        userInfo.setCsUserID(csId);
////        userInfo.seteMail("  ");
////        userInfo.setHeadURL("");
////        userInfo.setFlow("0");
////        userInfo.setLanip(" ");
////        userInfo.setNickname(userid);
////        userInfo.setPhone(" ");
////        userInfo.setUserSign(" ");
////        userInfo.setStatus("1");
////        userInfo.setReceptionLimit(receptionLimit);
////        try {
////            Map result = BeanToMapUtil.convertBean(userInfo);
////            Iterator it = result.entrySet().iterator();
////            while (it.hasNext()) {
////                Map.Entry entry = (Map.Entry) it.next();
////                Object key = entry.getKey();
////                Object value = entry.getValue();
////                if (value != null)
////                    redisCacheClient.hset("userinfo#3333#" + userid, key.toString(), value.toString());
////                else
////                    redisCacheClient.hset("userinfo#3333#" + userid, key.toString(), "--");
////            }
////        } catch (Exception e) {
////        }
//    }
//
//    //测试数据
//    @RequestMapping(value = "test-insert-redis-test-data")
//    @ResponseBody
//    public ResponseResult testGetAllServiceInfo(String groupId) {
//
//        //等待队列 20000001 20000002  0000003 属于11组
//        redisCacheClient.del("CsQueue#3333#11");
//        redisCacheClient.zadd("CsQueue#3333#11", 20000004, "20000004");
//        redisCacheClient.zadd("CsQueue#3333#11", 20000005, "20000005");
//        redisCacheClient.zadd("CsQueue#3333#11", 20000006, "20000006");
//
//        addUserInfo("20000001", "111111", "11", "2");
//        addUserInfo("20000002", "111111", "11", "2");
//        addUserInfo("20000003", "222222", "11", "2");
//        addUserInfo("20000004", "", "", "2");
//        addUserInfo("20000005", "", "", "2");
//        addUserInfo("20000006", "", "", "2");
//
//        addUserInfo("111111", "", "", "2");
//        addUserInfo("222222", "", "", "2");
//
//        //11组内成员：111111 222222
//        redisCacheClient.del("CServiceListInGroup#3333#11");
//        redisCacheClient.lpush("CServiceListInGroup#3333#11", "111111");
//        redisCacheClient.lpush("CServiceListInGroup#3333#11", "222222");
//
//        //接待列表，111111 ,接待客户20000001 20000002
//        redisCacheClient.del("CServiceRecptionList#3333#111111");
//        redisCacheClient.hset("CServiceRecptionList#3333#111111", "20000001", "1");
//        redisCacheClient.hset("CServiceRecptionList#3333#111111", "20000002", "0");
//
//        //不需要单独的客服上下文
//        //222222客服上下文信息
//        redisCacheClient.del("CServiceInfo#3333#222222");
//        //111111客服上下文信息
//        redisCacheClient.del("CServiceInfo#3333#111111");
//
//        //接待列表，222222 ,接待客户20000003
//        redisCacheClient.del("CServiceRecptionList#3333#222222");
//        redisCacheClient.hset("CServiceRecptionList#3333#222222", "20000003", "1");
//
//        return null;
//    }
//
//    //测试一个组内所有客服的接待情况
//    @RequestMapping(value = "test-service-info-in-group")
//    @ResponseBody
//    public ResponseResult testGetAllServiceInfoInGroup(Long groupId) {
//        if (groupId == null)
//            return null;
//
//        ResponseResult responseResult = new ResponseResult();
//        List infoList = kefuQueueInfoService.getServiceInfoListInGroupId(groupId, false);
//        Map<String, String> recptionList1 = redisCacheClient.hgetAll("CServiceRecptionList#3333#111111");
//        Map<String, String> recptionList2 = redisCacheClient.hgetAll("CServiceRecptionList#3333#222222");
//        responseResult.markSuccess();
//        responseResult.addData("info", infoList);
//        responseResult.addData("recptionList1", recptionList1);
//        responseResult.addData("recptionList2", recptionList2);
//        return responseResult;
//    }
//
//    /*
//    *           【队列/组内客服         组内客服还可以接待       组内客服接待已经满了
//    *           队列为空                    1                       2
//    *           队列不为空                  -                      3
//    *           】
//    *           *
//    *           【需要排队的客户没有在排队也没有被接待   1,2,3
//    *           需要排队客户已经在排队      4,5,6
//    *           需要排队客户已经在接待      7,8,9
//    *           】
//    * */
//    //排队测试，场景:
//    @RequestMapping(value = "reset-stat")
//    @ResponseBody
//    public ResponseResult resetStatRedisData(String statsNumber) {
//        ResponseResult responseResult = new ResponseResult();
//        //等待队列 20000001 20000002  0000003 属于11组
//        redisCacheClient.del("CsQueue#3333#11");
//        if (statsNumber.equals("3") || statsNumber.equals("6") || statsNumber.equals("9")) {
//            redisCacheClient.zadd("CsQueue#3333#11", 20000005, "20000005");
//        }
//
//        addUserInfo("20000001", "111111", "11", "2");
//        addUserInfo("20000002", "111111", "11", "2");
//        addUserInfo("20000003", "222222", "11", "2");
//        addUserInfo("20000004", "", "", "2");
//        addUserInfo("20000005", "", "", "2");
//        addUserInfo("20000006", "", "", "2");
//        addUserInfo("111111", "", "", "2");
//        addUserInfo("222222", "", "", "2");
//
//        if (statsNumber.equals("2") || statsNumber.equals("3") || statsNumber.equals("5")
//                || statsNumber.equals("6") || statsNumber.equals("8") || statsNumber.equals("9")) {
//            //接待列表，111111 ,接待客户20000001 20000002
//            redisCacheClient.del("CServiceRecptionList#3333#111111");
//            redisCacheClient.hset("CServiceRecptionList#3333#111111", "20000001", "1");
//            redisCacheClient.hset("CServiceRecptionList#3333#111111", "20000002", "0");
//
//            //接待列表，222222 ,接待客户20000003
//            redisCacheClient.del("CServiceRecptionList#3333#222222");
//            redisCacheClient.hset("CServiceRecptionList#3333#222222", "20000003", "1");
//            redisCacheClient.hset("CServiceRecptionList#3333#222222", "20000004", "0");
//        } else {
//            //接待列表，111111 ,接待客户20000001 20000002
//            redisCacheClient.del("CServiceRecptionList#3333#111111");
//            redisCacheClient.hset("CServiceRecptionList#3333#111111", "20000001", "1");
//
//            //接待列表，222222 ,接待客户20000003
//            redisCacheClient.del("CServiceRecptionList#3333#222222");
//            redisCacheClient.hset("CServiceRecptionList#3333#222222", "20000003", "1");
//            redisCacheClient.hset("CServiceRecptionList#3333#222222", "20000004", "0");
//        }
//        //11组内成员：111111 222222
//        redisCacheClient.del("CServiceListInGroup#3333#11");
//        redisCacheClient.lpush("CServiceListInGroup#3333#11", "111111");
//        redisCacheClient.lpush("CServiceListInGroup#3333#11", "222222");
//
//        //不需要单独的客服上下文
//        //222222客服上下文信息
//        redisCacheClient.del("CServiceInfo#3333#222222");
//        //111111客服上下文信息
//        redisCacheClient.del("CServiceInfo#3333#111111");
//
//        responseResult.markSuccess();
//        return responseResult;
//    }
//
//
//    //分流测试,
//    @RequestMapping(value = "reset-shunt")
//    @ResponseBody
//    public ResponseResult resetShuntRedisData(String statsNumber) {
//        ResponseResult responseResult = new ResponseResult();
//        //等待队列 20000001 20000002  0000003 属于11组
//        redisCacheClient.del("CsQueue#3333#11");
//        if (statsNumber.equals("3") || statsNumber.equals("6") || statsNumber.equals("9")) {
//            redisCacheClient.zadd("CsQueue#3333#11", 20000005, "20000005");
//        }
//
//        addUserInfo("20000001", "111111", "11", "2");
//        addUserInfo("20000002", "111111", "11", "2");
//        addUserInfo("20000003", "222222", "11", "2");
//        addUserInfo("20000004", "", "", "2");
//        addUserInfo("20000005", "", "", "2");
//        addUserInfo("20000006", "", "", "2");
//        addUserInfo("111111", "", "", "2");
//        addUserInfo("222222", "", "", "2");
//
//        if (statsNumber.equals("1")) {
//            //接待列表，111111 ,接待客户20000001 20000002
//            redisCacheClient.del("CServiceRecptionList#3333#111111");
//            redisCacheClient.hset("CServiceRecptionList#3333#111111", "20000001", "1");
//            redisCacheClient.hset("CServiceRecptionList#3333#111111", "20000002", "0");
//
//            //接待列表，222222 ,接待客户20000003
//            redisCacheClient.del("CServiceRecptionList#3333#222222");
//            redisCacheClient.hset("CServiceRecptionList#3333#222222", "20000003", "1");
//            redisCacheClient.hset("CServiceRecptionList#3333#222222", "20000004", "0");
//        } else if (statsNumber.equals("2")) {
//            //接待列表，111111 ,接待客户20000001 20000002
//            redisCacheClient.del("CServiceRecptionList#3333#111111");
//            redisCacheClient.hset("CServiceRecptionList#3333#111111", "20000001", "1");
//
//            //接待列表，222222 ,接待客户20000003
//            redisCacheClient.del("CServiceRecptionList#3333#222222");
//            redisCacheClient.hset("CServiceRecptionList#3333#222222", "20000003", "1");
//            redisCacheClient.hset("CServiceRecptionList#3333#222222", "20000004", "0");
//        } else if (statsNumber.equals("3")) {
//            redisCacheClient.del("CServiceRecptionList#3333#111111");
//            redisCacheClient.del("CServiceRecptionList#3333#222222");
//        }
//        //11组内成员：111111 222222
//        redisCacheClient.del("CServiceListInGroup#3333#11");
//
//        if (!statsNumber.equals("3")) {
//            redisCacheClient.lpush("CServiceListInGroup#3333#11", "111111");
//            redisCacheClient.lpush("CServiceListInGroup#3333#11", "222222");
//        }
//
//        //不需要单独的客服上下文
//        //222222客服上下文信息
//        redisCacheClient.del("CServiceInfo#3333#222222");
//        //111111客服上下文信息
//        redisCacheClient.del("CServiceInfo#3333#111111");
//
//        responseResult.markSuccess();
//        return responseResult;
//    }
//
//    //分流测试,
//    @RequestMapping(value = "reset-invit")
//    @ResponseBody
//    public ResponseResult resetinvitRedisData(String statsNumber) {
//        ResponseResult responseResult = new ResponseResult();
//        //等待队列 20000001 20000002  0000003 属于11组
//        redisCacheClient.del("CsQueue#3333#11");
//        if (statsNumber.equals("1") || statsNumber.equals("2")) {
//            redisCacheClient.zadd("CsQueue#3333#11", 20000005, "20000005");
//        }
//
//        addUserInfo("20000001", "111111", "11", "2");
//        addUserInfo("20000002", "111111", "11", "2");
//        addUserInfo("20000003", "222222", "11", "2");
//        addUserInfo("20000004", "", "", "2");
//        addUserInfo("20000005", "", "", "2");
//        addUserInfo("20000006", "", "", "2");
//        addUserInfo("111111", "", "", "2");
//        addUserInfo("222222", "", "", "2");
//
//        if (statsNumber.equals("1") || statsNumber.equals("3")) {
//            //接待列表，111111 ,接待客户20000001 20000002
//            redisCacheClient.del("CServiceRecptionList#3333#111111");
//            redisCacheClient.hset("CServiceRecptionList#3333#111111", "20000001", "1");
//            redisCacheClient.hset("CServiceRecptionList#3333#111111", "20000002", "0");
//
//            //接待列表，222222 ,接待客户20000003
//            redisCacheClient.del("CServiceRecptionList#3333#222222");
//            redisCacheClient.hset("CServiceRecptionList#3333#222222", "20000003", "1");
//            redisCacheClient.hset("CServiceRecptionList#3333#222222", "20000004", "0");
//        } else if (statsNumber.equals("2") || statsNumber.equals("4")) {
//            //接待列表，111111 ,接待客户20000001 20000002
//            redisCacheClient.del("CServiceRecptionList#3333#111111");
//            redisCacheClient.hset("CServiceRecptionList#3333#111111", "20000001", "1");
//
//            //接待列表，222222 ,接待客户20000003
//            redisCacheClient.del("CServiceRecptionList#3333#222222");
//            redisCacheClient.hset("CServiceRecptionList#3333#222222", "20000003", "1");
//            redisCacheClient.hset("CServiceRecptionList#3333#222222", "20000004", "0");
//        }
//        //11组内成员：111111 222222
//        redisCacheClient.del("CServiceListInGroup#3333#11");
//
//        redisCacheClient.lpush("CServiceListInGroup#3333#11", "111111");
//        redisCacheClient.lpush("CServiceListInGroup#3333#11", "222222");
//
//        //不需要单独的客服上下文
//        //222222客服上下文信息
//        redisCacheClient.del("CServiceInfo#3333#222222");
//        //111111客服上下文信息
//        redisCacheClient.del("CServiceInfo#3333#111111");
//
//        responseResult.markSuccess();
//        return responseResult;
//    }
//
//    //分流测试,
//    @RequestMapping(value = "reset-online")
//    @ResponseBody
//    public ResponseResult resetOnlineRedisData(String statsNumber) {
//        ResponseResult responseResult = new ResponseResult();
//        //等待队列 20000001 20000002  0000003 属于11组
//        redisCacheClient.del("CsQueue#3333#11");
//        if (statsNumber.equals("1") || statsNumber.equals("2")) {
//            redisCacheClient.zadd("CsQueue#3333#11", 20000005, "20000005");
//            redisCacheClient.zadd("CsQueue#3333#11", 20000006, "20000006");
//        }
//
//        addUserInfo("20000001", "111111", "11", "2");
//        addUserInfo("20000002", "111111", "11", "2");
//        addUserInfo("20000003", "222222", "11", "2");
//        addUserInfo("20000004", "", "", "2");
//        addUserInfo("20000005", "", "", "2");
//        addUserInfo("20000006", "", "", "2");
//        addUserInfo("111111", "", "", "2");
//        addUserInfo("222222", "", "", "2");
//
//        redisCacheClient.del("CServiceRecptionList#3333#111111");
//        redisCacheClient.del("CServiceRecptionList#3333#222222");
//        if (statsNumber.equals("1") || statsNumber.equals("3")) {
//            //接待列表，111111 ,接待客户20000001 20000002
//            redisCacheClient.hset("CServiceRecptionList#3333#111111", "20000001", "1");
//            redisCacheClient.hset("CServiceRecptionList#3333#111111", "20000002", "0");
//
//        }
//        //11组内成员：111111 222222
//        redisCacheClient.del("CServiceListInGroup#3333#11");
//        redisCacheClient.lpush("CServiceListInGroup#3333#11", "111111");
//        redisCacheClient.lpush("CServiceListInGroup#3333#11", "222222");
//        //不需要单独的客服上下文
//        //222222客服上下文信息
//        redisCacheClient.del("CServiceInfo#3333#222222");
//        //111111客服上下文信息
//        redisCacheClient.del("CServiceInfo#3333#111111");
//
//        responseResult.markSuccess();
//        return responseResult;
//    }
//
//    //排队分流，场景:
//    @RequestMapping(value = "test-shunt")
//    @ResponseBody
//    public ResponseResult testShunt(String statsNumber) {
//        ResponseResult responseResult = new ResponseResult();
//        String queueId = "";
//
//        if (statsNumber.equals("1")) {
//            responseResult.markSuccess();
//            responseResult.addData("id", "20000006");
//            queueId = "20000006";
//        } else if (statsNumber.equals("2")) {
//            responseResult.addData("id", "20000005");
//            queueId = "20000005";
//        } else if (statsNumber.equals("3")) {
//            responseResult.addData("id", "20000006");
//            queueId = "20000004";
//        }
//
//        testSendKafkaMessage(Long.valueOf(queueId), Long.valueOf(11), Long.valueOf(0),
//                ShuntConfig.SHUNT_EVENT_TYPE.EVENT_QUEUE_REQUESET_BY_CUSTOMER.ordinal(), 2, System.currentTimeMillis());
//
//
//        responseResult.markSuccess();
//        return responseResult;
//    }
//
//    //排队分流，场景:
//    @RequestMapping(value = "test-stats")
//    @ResponseBody
//    public ResponseResult testStats(String statsNumber) {
//        ResponseResult responseResult = new ResponseResult();
//        String queueId = "";
//
//        if (statsNumber.equals("1") || statsNumber.equals("2") || statsNumber.equals("3")) {
//            responseResult.markSuccess();
//            responseResult.addData("id", "20000006");
//            queueId = "20000006";
//        } else if (statsNumber.equals("4") || statsNumber.equals("5") || statsNumber.equals("6")) {
//            responseResult.addData("id", "20000005");
//            queueId = "20000005";
//        } else if (statsNumber.equals("7") || statsNumber.equals("8") || statsNumber.equals("9")) {
//            responseResult.addData("id", "20000004");
//            queueId = "20000004";
//        }
//
//        testSendKafkaMessage(Long.valueOf(queueId), Long.valueOf(11), Long.valueOf(0),
//                ShuntConfig.SHUNT_EVENT_TYPE.EVENT_QUEUE_REQUESET_BY_CUSTOMER.ordinal(), 2, System.currentTimeMillis());
//
//
//        responseResult.markSuccess();
//        return responseResult;
//    }
//
//
//    //排队分流，场景:
//    @RequestMapping(value = "test-invit")
//    @ResponseBody
//    public ResponseResult testInvit() {
//        ResponseResult responseResult = new ResponseResult();
//
//        responseResult.addData("id", "111111");
//
//        testSendKafkaMessage(Long.valueOf(0), Long.valueOf(11), Long.valueOf("111111"),
//                ShuntConfig.SHUNT_EVENT_TYPE.EVENT_INVITE_SESSION_REQUSET.ordinal(), 2, System.currentTimeMillis());
//
//
//        responseResult.markSuccess();
//        return responseResult;
//    }
//
//    //排队分流，场景:
//    @RequestMapping(value = "test-offline")
//    @ResponseBody
//    public ResponseResult testOffLine(String statsNumber) {
//        ResponseResult responseResult = new ResponseResult();
//
//        String serviceId = "";
//        if (statsNumber.equals("1") || statsNumber.equals("2")) {
//            serviceId = "111111";
//        } else {
//            serviceId = "222222";
//        }
//
//        responseResult.addData("id", "111111");
//
//        testSendKafkaMessage(Long.valueOf(serviceId), Long.valueOf(11), Long.valueOf("111111"),
//                ShuntConfig.SHUNT_EVENT_TYPE.EVENT_SERVICE_OFF_LINE.ordinal(), 2, System.currentTimeMillis());
//
//
//        responseResult.markSuccess();
//        return responseResult;
//    }
//
//
//    //排队分流，场景:
//    @RequestMapping(value = "test-online")
//    @ResponseBody
//    public ResponseResult testOnLine(String statsNumber) {
//        ResponseResult responseResult = new ResponseResult();
//
//        String serviceId = "";
//        if (statsNumber.equals("1") || statsNumber.equals("2")) {
//            serviceId = "222222";
//        } else {
//            serviceId = "111111";
//        }
//
//        responseResult.addData("id", "111111");
//
//        testSendKafkaMessage(Long.valueOf(serviceId), Long.valueOf(11), Long.valueOf("111111"),
//                ShuntConfig.SHUNT_EVENT_TYPE.EVENT_SERVICE_ON_LINE.ordinal(), 2, System.currentTimeMillis());
//
//        responseResult.markSuccess();
//        return responseResult;
//    }
}
