package com.feiniu.webim.controller;

import com.feiniu.webim.bean.domain.GoodsInfo;
import com.feiniu.webim.bean.domain.ResultInfo;
import com.feiniu.webim.bean.domain.UserInfo;
import com.feiniu.webim.bean.request.OrderMallRequest;
import com.feiniu.webim.bean.request.OrderRemarkRequest;
import com.feiniu.webim.bean.web.request.GetGoodsRequest;
import com.feiniu.webim.component.EndecryptedManager;
import com.feiniu.webim.component.GoodsService;
import com.feiniu.webim.component.OrderService;
import com.feiniu.webim.component.UserService;
import com.feiniu.webim.config.VenderInfoType;
import com.feiniu.webim.constants.ErrorCode;
import com.feiniu.webim.core.ResponseContentGenerator;
import com.feiniu.webim.core.WebAction;
import com.feiniu.webim.exception.WrongRequestParamException;
import com.feiniu.webim.util.general.StringUtil;
import com.feiniu.webim.util.web.ServletRequestUtil;
import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;

import javax.servlet.http.HttpServletRequest;
import java.util.List;

/**
 * Created by zhangmin on 2016/1/8.
 */
public class ClientRightPageController {
    public static final ClientRightPageController instance = new ClientRightPageController();

    private ClientRightPageController() {
    }

    private OrderService orderService = OrderService.instance;
    private GoodsService goodsService = GoodsService.instance;
    private UserService userService = UserService.instance;

    private static final Logger LOGGER = Logger.getLogger(VenderController.class);

    public void handleRequest(WebAction webAction) {
        try {
            webAction.startAction();
            doRequest(webAction);
            webAction.printResult();
        } catch (RuntimeException e) {
            ResponseContentGenerator generator = webAction.getResponseContentGenerator();
            if (e instanceof WrongRequestParamException) {
                ResultInfo resultInfo = new ResultInfo();
                resultInfo.setErrorMessage(e.getMessage());

                generator.writeObject(resultInfo);
            } else {
                generator.printServerError(null);
            }

            LOGGER.error("处理请求出错：", e);
        } finally {
            webAction.finishAction();
        }
    }

    private void doRequest(WebAction webAction) {
        HttpServletRequest request = webAction.getRequest();
        String type = ServletRequestUtil.getString(request, "VENDER_INFO_TYPE");
        VenderInfoType venderInfoType = VenderInfoType.getVenderInfoType(type);

        switch (venderInfoType) {
            //获取店铺下所有商品列表
            case GET_SHOP_GOODS_LIST:
                getGoodsInfo(webAction);
                break;
            //获取咨询的买家信息
            case GET_BUYER_INFO:
                getBuyerInfo(webAction);
                break;
            //获取订单信息
            case GET_ORDER_INFO:
                getOrderMall(webAction);
                break;
            case REMARK_ORDER:
                remarkOrder(webAction);
                break;
            default:
                throw new WrongRequestParamException("接口不存在");
        }
    }

    private void remarkOrder(WebAction webAction) {
        HttpServletRequest request = webAction.getRequest();
        OrderRemarkRequest orderRemarkRequest = ServletRequestUtil.getObject(request, OrderRemarkRequest.class);
        ResultInfo resultInfo = new ResultInfo();
        if (orderService.remarkOrder(orderRemarkRequest)) {
            resultInfo.markSuccess();
        }
        webAction.setResultInfo(resultInfo);
    }

    //订单查询  参数会包括memGuid
    private void getOrderMall(WebAction webAction) {
        HttpServletRequest request = webAction.getRequest();
        OrderMallRequest orderMallRequest = ServletRequestUtil.getObject(request, OrderMallRequest.class);
        ResultInfo resultInfo = new ResultInfo();

        if (orderMallRequest.getMerchantId().equals(0)) {
            resultInfo.setErrorCode(ErrorCode.ERRORCODE_NOT_LEGAL);
            resultInfo.setErrorMessage("参数错误");
        } else {
            //uuid解密后的形式为：memguid(36个字符),userid
            String customerToken = ServletRequestUtil.getString(request, "customerToken");
            String memGuid = EndecryptedManager.INSTANCE.decryptedWithURLDecode(customerToken).substring(0, 36);
            orderMallRequest.setMemGuid(memGuid);

            resultInfo.addData("orderQueryResult", orderService.QueryOrderByCondition(orderMallRequest));
            resultInfo.markSuccess();
        }

        webAction.setResultInfo(resultInfo);
    }

    //获取买家基本信息以及会员等级  参数会包括memGuid
    private void getBuyerInfo(WebAction webAction) {
        HttpServletRequest request = webAction.getRequest();

        //uuid解密后的形式为：memguid,userid
        String customerToken = ServletRequestUtil.getString(request, "customerToken");
        String memGuid = EndecryptedManager.INSTANCE.decryptedWithURLDecode(customerToken).substring(0, 36);
        UserInfo userInfo = userService.getUserInfo(memGuid);
        if (userInfo != null && StringUtils.isNotEmpty(userInfo.getUsername())) {
            userInfo.setNickname(StringUtil.makeTelephoneSecure(userInfo.getNickname()));
        }

        ResultInfo resultInfo = new ResultInfo();
        resultInfo.addData("userInfo", userInfo);
        resultInfo.markSuccess();
        webAction.setResultInfo(resultInfo);
    }

    //搜索接口获取店铺商品
    private void getGoodsInfo(WebAction webAction) {
        HttpServletRequest request = webAction.getRequest();

        GetGoodsRequest getGoodsRequest = ServletRequestUtil.getObject(request, GetGoodsRequest.class);

        ResultInfo resultInfo = new ResultInfo();
        if (getGoodsRequest.getMerchantId().equals(0)) {
            resultInfo.setErrorCode(ErrorCode.ERRORCODE_NOT_LEGAL);
            resultInfo.setErrorMessage("参数错误");
        } else {
            List<GoodsInfo> goodsSearchInfoList = goodsService.searchGoodsInfoForMerchant(getGoodsRequest);
            resultInfo.markSuccess();
            resultInfo.addData("goodsInfo", goodsSearchInfoList);
        }


        webAction.setResultInfo(resultInfo);
    }


}
