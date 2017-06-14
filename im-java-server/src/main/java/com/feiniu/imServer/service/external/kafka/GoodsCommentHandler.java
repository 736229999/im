package com.feiniu.imServer.service.external.kafka;

import com.feiniu.imServer.bean.request.NewCommentRequest;
import com.feiniu.imServer.util.FormatUtils;
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

import java.util.Date;

/**
 * Created by zhangmin on 2016/2/16.
 */
public class GoodsCommentHandler implements KafkaStreamHandler {
    @Autowired
    private OrderService orderService;

    private static final Logger LOGGER = LoggerFactory.getLogger(GoodsCommentHandler.class);

    @Override
    public void handle(kafka.consumer.KafkaStream<byte[], byte[]> kafkaStream) throws KafkaClientException {
        ConsumerIterator<byte[], byte[]> it = kafkaStream.iterator();

        while (it.hasNext()) {
            try {
                String queuedJsonStr = new String(it.next().message());
                JsonNode resultNode = JsonUtil.toNode(queuedJsonStr);
                String memGUID = JsonNodeUtil.getString(resultNode, "buyer_id");
                String ogNo = JsonNodeUtil.getString(resultNode, "order_id");

                if (StringUtils.isNotEmpty(memGUID) && StringUtils.isNotEmpty(ogNo)) {
                    JsonNode resultInfo = orderService.queryOrder(memGUID, ogNo);
                    if (resultInfo != null) {
                        NewCommentRequest newCommentRequest = new NewCommentRequest();
                        newCommentRequest.setBuyerName(JsonNodeUtil.getString(resultNode, "buyer_name"));
                        newCommentRequest.setComment(JsonNodeUtil.getString(resultNode, "evaluate_context"));
                        newCommentRequest.setCommentDate(FormatUtils.formatDate(new Date()));
                        newCommentRequest.setSkuId(JsonNodeUtil.getString(resultNode, "skuId"));
                        newCommentRequest.setMemGUID(memGUID);
                        newCommentRequest.setOgNo(ogNo);

                        orderService.sendCommentNotify(resultInfo, newCommentRequest);
                    }

                    LOGGER.info("正确处理订单评论消息：" + queuedJsonStr);
                } else {
                    LOGGER.error("收到错误的消息：", queuedJsonStr);
                }

            } catch (Exception e) {
                LOGGER.error("处理订单评论推送消息发生错误", e);
            }
        }
    }
}
