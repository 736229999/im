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
public class PayOrderHandler implements KafkaStreamHandler {
    private static final Logger LOGGER = LoggerFactory.getLogger(PayOrderHandler.class);
    @Autowired
    private OrderService orderService;

    @Override
    public void handle(kafka.consumer.KafkaStream<byte[], byte[]> kafkaStream) throws KafkaClientException {
        ConsumerIterator<byte[], byte[]> it = kafkaStream.iterator();

        while (it.hasNext()) {
            try {
                String queuedJsonStr = new String(it.next().message());
                JsonNode resultNode = JsonUtil.toNode(queuedJsonStr);
                String memGUID = JsonNodeUtil.getString(resultNode, "MEM_GUID");
                String ogNo = JsonNodeUtil.getString(resultNode, "OG_NO");

                if (StringUtils.isNotEmpty(memGUID) && StringUtils.isNotEmpty(ogNo)) {
                    JsonNode resultInfo = orderService.queryOrder(memGUID, ogNo);
                    if (resultInfo != null) {
                        orderService.sendPayOrderNotify(resultInfo);
                    }

                    LOGGER.info("正确处理订单支付提醒：" + queuedJsonStr);
                } else {
                    LOGGER.error("收到错误的消息：", queuedJsonStr);
                }

            } catch (Exception e) {
                LOGGER.error("处理支付订单推送消息发生错误", e);
            }
        }
    }
}
