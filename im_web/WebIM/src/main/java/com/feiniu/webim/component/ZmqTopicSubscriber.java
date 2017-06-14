package com.feiniu.webim.component;

import com.fasterxml.jackson.databind.JsonNode;
import com.feiniu.webim.bean.domain.MerchantSubAccountInfo;
import com.feiniu.webim.core.Session;
import com.feiniu.webim.core.SessionManager;
import com.feiniu.webim.util.json.JSONUtil;
import com.feiniu.webim.util.json.JsonNodeUtil;
import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.zeromq.ZMQ;

import java.util.Iterator;

/**
 * Author: morningking
 * Date: 2016/2/29
 * Contact: 243717042@qq.com
 */
public class ZmqTopicSubscriber {
    private volatile ZMQ.Context context;
    private volatile ZMQ.Socket subscriber;
    private static final Logger logger = Logger.getLogger(ZmqTopicSubscriber.class);
    private final SessionManager SESSION_MANAGER = SessionManager.getInstance();
    private volatile boolean isStop = true;
    private volatile int subscriberNo;

    private ZmqTopicSubscriber() {
    }

    public static ZmqTopicSubscriber getSubscriber(String pubDescriptor, String topic) {
        // Prepare our context and subscriber
        ZMQ.Context context = ZMQ.context(1);
        ZMQ.Socket subscriber = context.socket(ZMQ.SUB);

        subscriber.connect(pubDescriptor);
        subscriber.subscribe(topic.getBytes(ZMQ.CHARSET));

        ZmqTopicSubscriber zmqTopicSubscriber = new ZmqTopicSubscriber();
        zmqTopicSubscriber.context = context;
        zmqTopicSubscriber.subscriber = subscriber;

        return zmqTopicSubscriber;
    }

    public synchronized void setSubscriberNo(int subscriberNo) {
        this.subscriberNo = subscriberNo;
    }

    public void doWork() {

        while (isStop) {
            // Read envelope with address
            String address = subscriber.recvStr();
            // Read message contents
            String contents = subscriber.recvStr();

            try {
                logger.info("zmq-subscriber-" + subscriberNo + " 收到zmq-push消息：" + contents);

                if (StringUtils.isNotEmpty(contents)) {
                    JsonNode eachGroupQueueInfo = JSONUtil.toNode(contents);
                    int groupID = JsonNodeUtil.getInteger(eachGroupQueueInfo, "groupId");

                    //更新那些还是处于排队中的用户的排队信息
                    JsonNode stillInQueuedCustomersNode = eachGroupQueueInfo.get("queue");
                    if (!stillInQueuedCustomersNode.isNull() && stillInQueuedCustomersNode.isArray()) {
                        Iterator<JsonNode> queueNodeIterator = stillInQueuedCustomersNode.iterator();

                        while (queueNodeIterator.hasNext()) {
                            JsonNode queueInfo = queueNodeIterator.next();
                            int userID = JsonNodeUtil.getInteger(queueInfo, "id");
                            int queueLength = JsonNodeUtil.getInteger(queueInfo, "pos");

                            Session session = SESSION_MANAGER.getSessionByID(userID + "");
                            if (session != null) {
                                session.updateQueueInfoFromIm(groupID, queueLength, null);
                            }
                        }
                    }

                    //更新那些已经可以被服务的买家信息
                    JsonNode nonNeedQueuedCustomersNode = eachGroupQueueInfo.get("subAccount");
                    if (!nonNeedQueuedCustomersNode.isNull() && nonNeedQueuedCustomersNode.isArray()) {
                        Iterator<JsonNode> queueNodeIterator = nonNeedQueuedCustomersNode.iterator();

                        while (queueNodeIterator.hasNext()) {
                            JsonNode queueInfo = queueNodeIterator.next();
                            int userID = JsonNodeUtil.getInteger(queueInfo, "buyerId");

                            Session session = SESSION_MANAGER.getSessionByID(userID + "");
                            if (session != null) {
                                MerchantSubAccountInfo merchantSubAccountInfo = new MerchantSubAccountInfo();
                                merchantSubAccountInfo.setOnline(true);
                                merchantSubAccountInfo.setHeadImgUrl(JsonNodeUtil.getString(queueInfo, "faceUrl"));
                                merchantSubAccountInfo.setId(JsonNodeUtil.getInteger(queueInfo, "merchantId"));
                                merchantSubAccountInfo.setQueueLength(0);
                                merchantSubAccountInfo.setUsersign(JsonNodeUtil.getString(queueInfo, "userSign"));
                                merchantSubAccountInfo.setUsername(JsonNodeUtil.getString(queueInfo, "userName"));
                                merchantSubAccountInfo.setTruename(JsonNodeUtil.getString(queueInfo, "nickName"));

                                session.updateQueueInfoFromIm(groupID, 0, merchantSubAccountInfo);
                            }
                        }
                    }
                }

            } catch (RuntimeException e) {
                logger.error("处理推送消息失败：", e);
            }
        }
    }

    public void close() {
        isStop = false;
        subscriber.close();
        context.term();
    }
}
