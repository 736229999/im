package com.feiniu.imServer.service.external.kafka;

import com.feiniu.imServer.bean.request.GetReturnOrderRequest;
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
public class ReturnOrderHandler implements KafkaStreamHandler {
    @Autowired
    private OrderService orderService;

    private static final Logger LOGGER = LoggerFactory.getLogger(ReturnOrderHandler.class);

    @Override
    public void handle(kafka.consumer.KafkaStream<byte[], byte[]> kafkaStream) throws KafkaClientException {
        ConsumerIterator<byte[], byte[]> it = kafkaStream.iterator();

        while (it.hasNext()) {
            try {
                String queuedJsonStr = new String(it.next().message());
                JsonNode resultNode = JsonUtil.toNode(queuedJsonStr);

                GetReturnOrderRequest getReturnOrderRequest = new GetReturnOrderRequest();
                getReturnOrderRequest.setMemGuid(JsonNodeUtil.getString(resultNode, "memGuid"));
                getReturnOrderRequest.setOgSeq(JsonNodeUtil.getString(resultNode, "ogSeq"));
                getReturnOrderRequest.setIsMall("2");

                JsonNode resultInfo = orderService.getReturnedOrder(getReturnOrderRequest);
                if (resultInfo != null) {
                    orderService.sendReturnOrderInfo(resultInfo, getReturnOrderRequest);
                }

                LOGGER.info("正确处理退单：" + queuedJsonStr);
            } catch (Exception e) {
                LOGGER.error("处理发生错误", e);
            }
        }
    }
}
