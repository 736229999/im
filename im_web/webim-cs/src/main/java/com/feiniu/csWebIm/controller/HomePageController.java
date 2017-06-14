package com.feiniu.csWebIm.controller;

import com.feiniu.csWebIm.base.ResponseResult;
import com.feiniu.csWebIm.bean.domain.ConsultItemList;
import com.feiniu.csWebIm.bean.domain.ConsultOrderInfo;
import com.feiniu.csWebIm.bean.domain.ConsultPackageList;
import com.feiniu.csWebIm.bean.response.UserCheckInInfo;
import com.feiniu.csWebIm.bean.vo.UserOrder;
import com.feiniu.csWebIm.consts.BusinessConfig;
import com.feiniu.csWebIm.consts.SystemConfig;
import com.feiniu.csWebIm.service.external.*;
import org.apache.commons.collections.MapUtils;
import org.apache.commons.collections4.CollectionUtils;
import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import javax.servlet.http.HttpServletRequest;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Author: morningking
 * Date: 2015/11/23
 * Contact: 243717042@qq.com
 */
@Controller
@RequestMapping
public class HomePageController {
    @Autowired
    private KefuBackendService kefuBackendService;
    @Autowired
    private UserInfoService userInfoService;
    @Autowired
    private OrderService orderService;
    @Autowired
    private OrderConsultService orderConsultService;
    @Autowired
    private AppMemberService appMemberService;
    @Autowired
    private BusinessConfig businessConfig;

    @RequestMapping(value = "home-page")
    public String getHomePage(ModelMap map) {
        Map<String, Object> homePageConfig = kefuBackendService.getHomePageConfig();

        if (MapUtils.isNotEmpty(homePageConfig)) {
            map.putAll(homePageConfig);
        }

        return "home-page";
    }

    @RequestMapping(value = "get-user-order")
    @ResponseBody
    public ResponseResult getUserOrderList(HttpServletRequest request) {
        ResponseResult responseResult = new ResponseResult();
        UserCheckInInfo userCheckInInfo = userInfoService.checkUserInfoFromMember(request.getCookies());

        List<UserOrder> userOrderList = orderService.getOrderListRecentMonth(userCheckInInfo.getGuid());

        responseResult.addData("userOrderList", userOrderList);
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "get-user-order-list-from-app")
    @ResponseBody
    public ResponseResult getUserOrderListFromApp(String token) {
        ResponseResult responseResult = new ResponseResult();
        UserCheckInInfo userCheckInInfo = appMemberService.getUserInfo(token);
        List<UserOrder> userOrderList = orderService.getOrderListRecentMonth(userCheckInInfo.getGuid());

        responseResult.addData("list", userOrderList);
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "get-user-order-from-app")
    @ResponseBody
    public ResponseResult getUserOrderListFromApp(String token, String orderSeq) {
        ResponseResult responseResult = new ResponseResult();
        UserCheckInInfo userCheckInInfo = appMemberService.getUserInfo(token);
        ConsultOrderInfo consultOrderInfo = orderConsultService.getOrderConsult(orderSeq, userCheckInInfo.getGuid());

        Map<String, Object> orderInfo = new HashMap<>(4);
        orderInfo.put("seq", consultOrderInfo.getOrderDetail().getOgSeq());
        orderInfo.put("price", consultOrderInfo.getOrderDetail().getOrderPrice());

        if (CollectionUtils.isNotEmpty(consultOrderInfo.getPackageList())) {
            ConsultPackageList packageItem = consultOrderInfo.getPackageList().get(0);
            List<ConsultItemList> consultItemList = packageItem.getItemList();
            if (CollectionUtils.isNotEmpty(consultItemList)) {
                ConsultItemList consultItem = consultItemList.get(0);
                String picUrl = consultItem.getPicUrl();
                int supplierType = packageItem.getType();

                if (supplierType == 1) {
                    picUrl = businessConfig.getFeiniuSelfSupportImgBaseUrl() + picUrl;
                } else {
                    picUrl = picUrl.replaceFirst("^http://wh-image01\\.fn\\.com(:80)?/", businessConfig.getRandomUrlBase());
                    picUrl = picUrl.replaceFirst("^http://10\\.211\\.64\\.68(:80)?/", businessConfig.getRandomUrlBase());
                    picUrl = picUrl.indexOf("://") > 0 ? picUrl : businessConfig.getRandomUrlBase() + picUrl;
                }

                picUrl = picUrl.replaceFirst("\\.[a-zA-Z]+$", businessConfig.getOrderPicSize() + "$0");
                picUrl = picUrl.replaceFirst("\\.[\\w_]+\\.([a-zA-Z]+)$", businessConfig.getOrderPicSize() + "$1");
                orderInfo.put("picURL", picUrl);

                String strPicSource = consultItem.getSourceUrl();
                if (strPicSource.indexOf(SystemConfig.HTTP_HEAD) != -1) {
                    orderInfo.put("url", strPicSource);
                } else {
                    orderInfo.put("url", businessConfig.getGoodsItemURL() + strPicSource);
                }
                orderInfo.put("title", consultItem.getProductName());
            }
        }

        responseResult.addData("order", orderInfo);
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "check-order-seq")
    @ResponseBody
    public ResponseResult checkOrderIsUnderUser(String orderSeq, HttpServletRequest request) {
        ResponseResult responseResult = new ResponseResult();

        if (StringUtils.isNotEmpty(orderSeq)) {
            UserCheckInInfo userCheckInInfo = userInfoService.checkUserInfoFromMember(request.getCookies());
            boolean isLegal = orderService.isOrderSeqUnderUser(userCheckInInfo.getGuid(), orderSeq);

            responseResult.addData("isLegal", isLegal);
            responseResult.markSuccess();
        }

        return responseResult;
    }

    @RequestMapping(value = "update-quick-phrase")
    @ResponseBody
    public ResponseResult updateQuickPhrase(String questionId, HttpServletRequest httpRequest) {
        ResponseResult responseResult = new ResponseResult();

        if (questionId == null)
            return responseResult;

        kefuBackendService.updateFAQCount(questionId);

        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "get-touch-board-config-list")
    @ResponseBody
    public ResponseResult getTouchBoardConfigList() {
        ResponseResult responseResult = new ResponseResult();

        responseResult.addData("list", kefuBackendService.getTouchBoardList());
        responseResult.markSuccess();
        return responseResult;
    }
}
