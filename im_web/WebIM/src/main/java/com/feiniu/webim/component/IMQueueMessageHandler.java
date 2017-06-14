package com.feiniu.webim.component;

import com.fasterxml.jackson.databind.JsonNode;
import com.feiniu.kafka.client.KafkaStreamHandler;
import com.feiniu.kafka.client.exception.KafkaClientException;
import com.feiniu.webim.bean.domain.MerchantSubAccountInfo;
import com.feiniu.webim.core.Session;
import com.feiniu.webim.core.SessionManager;
import com.feiniu.webim.util.json.JSONUtil;
import com.feiniu.webim.util.json.JsonNodeUtil;
import kafka.consumer.ConsumerIterator;
import kafka.consumer.KafkaStream;
import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;

import java.util.Iterator;

/**
 * Author: morningking
 * Date: 2015/12/18
 * Contact: 243717042@qq.com
 */
public class IMQueueMessageHandler implements KafkaStreamHandler {
    private final SessionManager SESSION_MANAGER = SessionManager.getInstance();
    private static final Logger LOGGER = Logger.getLogger(IMQueueMessageHandler.class);

    @Override
    public void handle(KafkaStream<byte[], byte[]> kafkaStream) throws KafkaClientException {
        ConsumerIterator<byte[], byte[]> it = kafkaStream.iterator();
        while (it.hasNext()) {
            try {
                String queuedJsonStr = new String(it.next().message());
                LOGGER.debug("收到im-push消息：" + queuedJsonStr);

                if (StringUtils.isNotEmpty(queuedJsonStr)) {
                    JsonNode eachGroupQueueInfo = JSONUtil.toNode(queuedJsonStr);
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
                LOGGER.error("处理推送消息失败：", e);
            }
        }
    }
}
