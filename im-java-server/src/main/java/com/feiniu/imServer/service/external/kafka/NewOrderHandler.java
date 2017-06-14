package com.feiniu.imServer.service.external.kafka;

import com.feiniu.imServer.util.JsonUtil;
import com.feiniu.imServer.util.json.JsonNodeUtil;
import com.feiniu.kafka.client.KafkaStreamHandler;
import com.feiniu.kafka.client.exception.KafkaClientException;
import kafka.consumer.ConsumerIterator;
import org.apache.commons.lang.StringUtils;
import org.codehaus.jackson.JsonNode;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;

/**
 * Created by zhangmin on 2016/2/16.
 */
public class NewOrderHandler implements KafkaStreamHandler {
    @Autowired
    private OrderService orderService;

    private static final Logger LOGGER = LoggerFactory.getLogger(NewOrderHandler.class);
    //判断消息内容是否合法
    private static final int MESSAGE_SUCC = 1;

    @Override
    public void handle(kafka.consumer.KafkaStream<byte[], byte[]> kafkaStream) throws KafkaClientException {
        ConsumerIterator<byte[], byte[]> it = kafkaStream.iterator();

        while (it.hasNext()) {
            try {
                String queuedJsonStr = new String(it.next().message());
                JsonNode resultNode = JsonUtil.toNode(queuedJsonStr);

                if (resultNode.has("succ") && JsonNodeUtil.getInteger(resultNode, "succ") == MESSAGE_SUCC) {
                    String memGUID = JsonNodeUtil.getString(resultNode, "memguid");
                    String ogNo = JsonNodeUtil.getString(resultNode, "ogno");

                    if (StringUtils.isNotEmpty(memGUID) && StringUtils.isNotEmpty(ogNo)) {
                        JsonNode resultInfo = orderService.queryOrder(memGUID, ogNo);
                        if (resultInfo != null) {
                            orderService.sendNewOrderAllMerchant(resultInfo);
                        }

                        LOGGER.info("正确处理新订单：" + queuedJsonStr);
                    } else {
                        LOGGER.error("收到错误的消息：", queuedJsonStr);
                    }
                }
            } catch (Exception e) {
                LOGGER.error("处理发生错误", e);
            }
        }
    }
}
