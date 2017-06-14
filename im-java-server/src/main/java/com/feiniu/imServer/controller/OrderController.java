package com.feiniu.imServer.controller;

import com.feiniu.imServer.base.ResponseResult;
import com.feiniu.imServer.bean.request.GetReturnOrderRequest;
import com.feiniu.imServer.bean.request.NewCommentRequest;
import com.feiniu.imServer.service.external.kafka.OrderService;
import com.feiniu.imServer.util.FormatUtils;
import com.feiniu.imServer.util.MD5;
import kafka.controller.NewReplica;
import org.codehaus.jackson.JsonNode;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import java.util.Date;

/**
 * Created by zhangmin on 2016/2/17.
 */
@Controller
@RequestMapping(value = "order")
public class OrderController {

    @Autowired
    private OrderService orderService;

    @RequestMapping(value = "getOrderInfo")
    @ResponseBody
    public ResponseResult getOrderInfo() {
        ResponseResult responseResult = new ResponseResult();
        String memGuid = "66EB4CC4-A966-92EB-EAE4-17369FC89E8C";
        String ogNo = "201602CP26040991";

        responseResult.addData("orderInfo", orderService.queryOrder(memGuid, ogNo));
        return responseResult;
    }

    @RequestMapping(value = "get-return-order-info")
    @ResponseBody
    public ResponseResult getReturnOrderInfo() {
        ResponseResult responseResult = new ResponseResult();

        GetReturnOrderRequest getReturnOrderRequest = new GetReturnOrderRequest();
        getReturnOrderRequest.setMemGuid("B66BAB73-F72B-C2DD-5718-D3D08DE7245D");
        getReturnOrderRequest.setFdlSeq("");
        getReturnOrderRequest.setIsMall("2");
        getReturnOrderRequest.setOgSeq("201509CO23000014");

        orderService.getReturnedOrder(getReturnOrderRequest);
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "pay-order")
    @ResponseBody
    public ResponseResult payOrder() throws Exception {
        ResponseResult responseResult = new ResponseResult();
        String memGuid = "7CAD9F5F-5B03-E300-EC17-04DD297A3A54";
        String ogNo = "201510CP13100055";

        JsonNode jsonNode = orderService.queryOrder(memGuid, ogNo);
        orderService.sendPayOrderNotify(jsonNode);

        return responseResult;
    }

    @RequestMapping(value = "good-comment")
    @ResponseBody
    public ResponseResult goodsComment() throws Exception {
        ResponseResult responseResult = new ResponseResult();
        String memGuid = "7CAD9F5F-5B03-E300-EC17-04DD297A3A54";
        String ogNo = "201510CP13100055";
        String skuId = "100134658";

        JsonNode jsonNode = orderService.queryOrder(memGuid, ogNo);
        NewCommentRequest newCommentRequest = new NewCommentRequest();
        newCommentRequest.setBuyerName("飞牛—morningking");
        newCommentRequest.setComment("不错不错");
        newCommentRequest.setCommentDate(FormatUtils.formatTime(new Date()));
        newCommentRequest.setMemGUID(memGuid);
        newCommentRequest.setOgNo(ogNo);
        newCommentRequest.setSkuId(skuId);

        orderService.sendCommentNotify(jsonNode, newCommentRequest);

        return responseResult;
    }
}
