package com.feiniu.webim.controller;

import com.feiniu.webim.bean.domain.*;
import com.feiniu.webim.bean.web.request.GetProductListRequest;
import com.feiniu.webim.component.*;
import com.feiniu.webim.config.RequestParams;
import com.feiniu.webim.config.VenderInfoType;
import com.feiniu.webim.constants.Constants;
import com.feiniu.webim.core.ResponseContentGenerator;
import com.feiniu.webim.core.Session;
import com.feiniu.webim.core.SessionManager;
import com.feiniu.webim.core.WebAction;
import com.feiniu.webim.exception.WrongRequestParamException;
import com.feiniu.webim.util.general.StringUtil;
import com.feiniu.webim.util.web.ServletRequestUtil;
import org.apache.commons.collections4.CollectionUtils;
import org.apache.commons.collections4.MapUtils;
import org.apache.commons.lang.StringUtils;
import org.apache.commons.lang.math.NumberUtils;
import org.apache.log4j.Logger;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;
import java.util.*;

/**
 * Created by wangchen on 2015/1/14.
 */
public class VenderController {
    public static final VenderController instance = new VenderController();
    private static Integer time_delta_local_cache;

    private VenderController() {
    }

    private MerchantService merchantService = MerchantService.instance;
    private StoreService storeService = StoreService.instance;
    private ProductService productService = ProductService.instance;
    private KefuService kefuService = KefuService.instance;
    private CommentService commentService = CommentService.instance;
    private OrderService orderService = OrderService.instance;
    private ImService imService = ImService.IM_SERVICE;
    private ImApiService imApiService = ImApiService.INSTANCE;
    private FeiniuItemService feiniuItemService = FeiniuItemService.instance;
    private EndecryptedManager endecryptedManager = EndecryptedManager.INSTANCE;
    private ConvertService CONVERT_SERVICE = ConvertService.CONVERT_SERVICE;
    private GoodsService goodsService = GoodsService.instance;

    //当返回消息给端时，使用的相关规则
    private static final List<ConvertService.ConvertRule> CONVERT_HEAD_IMG_RELATIVE =
            Arrays.asList(ConvertService.ConvertRule.MEKE_HEAD_IMG_ABSOLUTE,
                    ConvertService.ConvertRule.TRANSFORM_OLD_MERCHANT_PHOTO_FROM_EXTERNAL);

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
            case GET_STORE_INFO:
                getStoreInfo(webAction);
                break;
            case GET_STORE_NAME:
                getStoreName(webAction);
                break;
            case GET_MERCHANT_INFO:
                getMerchantInfo(webAction);
                break;
            //获取商品信息(单个)
            case GET_PRODUCT_INFO:
                getProductInfo(webAction);
                break;
            case GET_FEINIU_ITEM_INFO:
                getFeiniuItemInfo(webAction);
                break;
            //店铺下商品列表(旧的)
            case GET_PRODUCT_INFO_FOR_MERCHANT:
                getProductInfoForMerchant(webAction);
                break;
            case GET_PRODUCT_ITEM_URL_BY_GOOD_ID:
                getProductItemUrlByGoodId(webAction);
                break;
            case GET_STORE_RELATED_INFO_FOR_PC:
                getStoreRelatedInfoForPC(webAction);
                break;
            case GET_STORE_LIGHT_CONFIG:
                getStoreLightConfig(webAction);
                break;
            case GET_STORE_COMMENT_INFO:
                getStoreCommentInfo(webAction);
                break;
            //旧的订单查询
            case GET_USER_RELATED_STORE_ORDER_INFO:
                getUserRelatedStoreOrderInfo(webAction);
                break;
            case GET_RECOMMEND_PRODUCT_LIST:
                getRecommendProductList(webAction);
                break;
            case GET_MERCHANT_ONLINE_INFO:
                getMerchantOnlineInfo(webAction);
                break;
            case GET_SUB_MERCHANT_ONLINE_INFO:
                getServiceGroupOnlineInfo(webAction);
                break;
            case SYNC_TIME:
                syncTime(webAction);
                break;
            case CHECK_SESSION_TOKEN_LEGALITY:
                checkSessionTokenLegality(webAction);
                break;
            case ADD_COOKIE:
                addCookie(webAction);
                break;
            //获取飞牛内部员工信息
            case GET_STAFF_INFO:
                getFeiniuStaffInfo(webAction);
                break;
            case QUERY_USER_LOGIN_INFO:
                queryUserLoginInfo(webAction);
                break;
            //获取主子账号
            case GET_MAIN_SUB_MERCHANT_ID:
                getMainSubMerchntId(webAction);
                break;
            //获取子账号对应的商家账号
            case GET_MERCHANT_IDS_BY_MEMBER_IDS:
                getMerchantIDsByMemberIDs(webAction);
                break;
            default:
                throw new WrongRequestParamException("接口不存在");
        }
    }

    private void addCookie(WebAction webAction) {
        HttpServletRequest request = webAction.getRequest();
        String key = ServletRequestUtil.getString(request, "key");
        String value = ServletRequestUtil.getString(request, "value");
        Cookie cookie = new Cookie(key, value);
        webAction.getResponse().addCookie(cookie);
    }

    private void getProductInfo(WebAction webAction) {
        HttpServletRequest request = webAction.getRequest();
        String skuID = ServletRequestUtil.getString(request, RequestParams.PRODUCT_ID.toString());

        if (StringUtils.isNotEmpty(skuID) && NumberUtils.isDigits(skuID)) {
//            ProductInfo productInfo = productService.retrieveProductInfo(skuID);
//            productInfo.setProductURL(CONVERT_SERVICE.doConvert(productInfo.getProductURL(), CONVERT_HEAD_IMG_RELATIVE));
//            productInfo.setDefaultPic(CONVERT_SERVICE.doConvert(productInfo.getDefaultPic(), CONVERT_HEAD_IMG_RELATIVE));

            List<Long> skuList = new ArrayList<>();
            skuList.add(NumberUtils.createLong(skuID));
            GoodsAsking goodsAsking = new GoodsAsking();
            goodsAsking = goodsService.getGoodsAskingInfo(skuList).get(0);
            goodsAsking.setCommentCount(commentService.getCommentCount(goodsAsking.getGoodsId()));
//                productService.retrieveProductSeriesProperty(productInfo);
//                productInfo.setCommentCount(commentService.getCommentCount(productInfo.getGoodsId()));
//            } catch (RuntimeException e) {
//                omit exception
//            }

            ResultInfo resultInfo = new ResultInfo();
//            resultInfo.addData("productInfo", productInfo);
            resultInfo.addData("goodsAsking", goodsAsking);
            resultInfo.markSuccess();

            webAction.setResultInfo(resultInfo);
        } else {
            throw new WrongRequestParamException("输入参数有误：" + RequestParams.PRODUCT_ID + "为空");
        }
    }

    private void getProductInfoForMerchant(WebAction webAction) {
        HttpServletRequest request = webAction.getRequest();
        GetProductListRequest getProductListRequest = ServletRequestUtil.getObject(request, GetProductListRequest.class);

        List<MerchantProductInfo> merchantProductInfoList = productService.retrieveProductInfoForMerchant(getProductListRequest);
        for (MerchantProductInfo merchantProductInfo : merchantProductInfoList) {
            merchantProductInfo.setPic01(CONVERT_SERVICE.doConvert(merchantProductInfo.getPic01(), CONVERT_HEAD_IMG_RELATIVE));
        }

        ResultInfo resultInfo = new ResultInfo();
        resultInfo.addData("productInfoList", merchantProductInfoList);
        resultInfo.markSuccess();

        webAction.setResultInfo(resultInfo);
    }

    //获取子账号对应的商家账号
    private void getMerchantIDsByMemberIDs(WebAction webAction) {
        HttpServletRequest request = webAction.getRequest();
        Integer subMerchantID = ServletRequestUtil.getInteger(request, RequestParams.SUB_MERCHANT_ID.toString());
        if (subMerchantID <= 0) {
            throw new WrongRequestParamException("输入参数有误：" + RequestParams.SUB_MERCHANT_ID + "不合法");
        }
        Map<String, Integer> merchantIDMap = merchantService.retriveMerchantIDsByMemberIDs(Arrays.asList(subMerchantID));
        if (MapUtils.isEmpty(merchantIDMap)) {
            throw new WrongRequestParamException("参数有误，找不到指定的信息");
        }
        String merchantID = merchantIDMap.entrySet().iterator().next().getValue() + "";

        if (StringUtils.isNotEmpty(merchantID) && NumberUtils.isNumber(merchantID)) {
            ResultInfo resultInfo = new ResultInfo();
            resultInfo.addData("merchantID", merchantID);
            resultInfo.markSuccess();
            webAction.setResultInfo(resultInfo);
        } else {
            throw new WrongRequestParamException("输入参数有误：" + RequestParams.SUB_MERCHANT_ID + "为空");
        }
    }

    private void getMerchantInfo(WebAction webAction) {
        HttpServletRequest request = webAction.getRequest();
        String merchantID = ServletRequestUtil.getString(request, RequestParams.MERCHANT_ID.toString());

        if (StringUtils.isNotEmpty(merchantID) && NumberUtils.isNumber(merchantID)) {
            MerchantInfo merchantInfo = merchantService.retrieveMerchantInfo(merchantID);
            ResultInfo resultInfo = new ResultInfo();
            resultInfo.addData("merchantInfo", merchantInfo);
            resultInfo.markSuccess();

            webAction.setResultInfo(resultInfo);
        } else {
            throw new WrongRequestParamException("输入参数有误：" + RequestParams.MERCHANT_ID + "为空");
        }
    }

    private void getFeiniuStaffInfo(WebAction webAction) {
        HttpServletRequest request = webAction.getRequest();
        String staffID = ServletRequestUtil.getString(request, RequestParams.STAFF_ID.toString());
        if (StringUtils.isNotEmpty(staffID) && NumberUtils.isNumber(staffID)) {
            FeiniuStaffInfo staffInfo = imService.getInnerFeiniuStaffInfo(Long.parseLong(staffID));

            ResultInfo resultInfo = new ResultInfo();
            resultInfo.addData("staffInfo", staffInfo);
            resultInfo.markSuccess();

            webAction.setResultInfo(resultInfo);
        } else {
            throw new WrongRequestParamException("输入参数有误：" + RequestParams.STAFF_ID + "为空");
        }

    }

    //根据商家ID，获取主子账号
    private void getMainSubMerchntId(WebAction webAction) {
        HttpServletRequest request = webAction.getRequest();
        String merchantId = ServletRequestUtil.getString(request, "merchantId");
        if (StringUtils.isNotEmpty(merchantId) && NumberUtils.isNumber(merchantId)) {
            Long mainSubMerchntId = merchantService.retrieveMainSubMerchantId(Long.parseLong(merchantId));

            ResultInfo resultInfo = new ResultInfo();
            resultInfo.addData("mainSubMerchntId", mainSubMerchntId);
            resultInfo.markSuccess();

            webAction.setResultInfo(resultInfo);
        } else {
            throw new WrongRequestParamException("输入参数有误：" + merchantId + "为空");
        }

    }

    private void getStoreInfo(WebAction webAction) {
        HttpServletRequest request = webAction.getRequest();
        String merchantID = ServletRequestUtil.getString(request, RequestParams.MERCHANT_ID.toString());

        if (StringUtils.isNotEmpty(merchantID) && NumberUtils.isNumber(merchantID)) {
            StoreInfo storeInfo = storeService.retrieveStoreInfo(merchantID);
            storeInfo.setStoreLogoUrl(CONVERT_SERVICE.doConvert(storeInfo.getStoreLogoUrl(), CONVERT_HEAD_IMG_RELATIVE));

            ResultInfo resultInfo = new ResultInfo();
            resultInfo.addData("storeInfo", storeInfo);
            resultInfo.markSuccess();

            webAction.setResultInfo(resultInfo);
        } else {
            throw new WrongRequestParamException("输入参数有误：" + RequestParams.MERCHANT_ID + "为空");
        }
    }

    private void getFeiniuItemInfo(WebAction webAction) {
        HttpServletRequest request = webAction.getRequest();
        String smSeq = ServletRequestUtil.getString(request, RequestParams.SM_SEQ.toString());

        if (StringUtils.isNotEmpty(smSeq)) {
            FeiniuItemInfo feiniuItemInfo = feiniuItemService.getItemInfo(smSeq);

            ResultInfo resultInfo = new ResultInfo();
            resultInfo.addData("feiniuItemInfo", feiniuItemInfo);
            resultInfo.markSuccess();

            webAction.setResultInfo(resultInfo);
        } else {
            throw new WrongRequestParamException("输入参数有误：" + RequestParams.SM_SEQ + "为空");
        }
    }

    private void getStoreRelatedInfoForPC(WebAction webAction) {
        HttpServletRequest request = webAction.getRequest();
        Integer subMerchantID = ServletRequestUtil.getInteger(request, RequestParams.SUB_MERCHANT_ID.toString());
        if (subMerchantID <= 0) {
            throw new WrongRequestParamException("输入参数有误：" + RequestParams.SUB_MERCHANT_ID + "不合法");
        }

        Map<String, Integer> merchantIDMap = merchantService.retriveMerchantIDsByMemberIDs(Arrays.asList(subMerchantID));
        if (MapUtils.isEmpty(merchantIDMap)) {
            throw new WrongRequestParamException("参数有误，找不到指定的信息");
        }

        String merchantID = merchantIDMap.entrySet().iterator().next().getValue() + "";
        MerchantInfo merchantInfo = merchantService.retrieveMerchantInfo(merchantID);
        StoreInfo storeInfo = storeService.retrieveStoreInfo(merchantID);
        storeInfo.setStoreLogoUrl(CONVERT_SERVICE.doConvert(storeInfo.getStoreLogoUrl(), CONVERT_HEAD_IMG_RELATIVE));

        ResultInfo resultInfo = new ResultInfo();
        resultInfo.addData("merchantInfo", merchantInfo);
        resultInfo.addData("storeInfo", storeInfo);
        resultInfo.addData("storeCommentInfo", commentService.getScoreInfo(Long.parseLong(merchantID)));

        resultInfo.markSuccess();
        webAction.setResultInfo(resultInfo);
    }

    private void getStoreLightConfig(WebAction webAction) {
        ResultInfo resultInfo = new ResultInfo();
        String merchantIDs = ServletRequestUtil.getString(webAction.getRequest(), RequestParams.MERCHANTIDS.toString());
        List<Long> merchantIDList = new ArrayList<>();
        for (String merchantID : merchantIDs.split(",")) {
            merchantIDList.add(Long.parseLong(merchantID));
        }

        if (CollectionUtils.isNotEmpty(merchantIDList)) {
            long merchantID = merchantIDList.get(0);
            Map<String, Object> storeLightConfig = kefuService.getStoreLightConfig(merchantID);
            if (MapUtils.isNotEmpty(storeLightConfig)) {
                for (Map.Entry<String, Object> entry : storeLightConfig.entrySet()) {
                    resultInfo.addData(entry.getKey(), entry.getValue());
                }
            }
        }

        webAction.setResultInfo(resultInfo);
        resultInfo.markSuccess();
    }

    private void getStoreCommentInfo(WebAction webAction) {
        HttpServletRequest request = webAction.getRequest();
        long merchantID = ServletRequestUtil.getLong(request, RequestParams.MERCHANT_ID.toString());

        if (merchantID > 0) {
            ResultInfo resultInfo = new ResultInfo();
            resultInfo.addData("storeCommentInfo", commentService.getScoreInfo(merchantID));
            resultInfo.markSuccess();

            webAction.setResultInfo(resultInfo);
        } else {
            throw new WrongRequestParamException("输入参数有误：" + RequestParams.MERCHANT_ID + "不合法");
        }
    }

    private void getUserRelatedStoreOrderInfo(WebAction webAction) {
        HttpServletRequest request = webAction.getRequest();
        long merchantID = ServletRequestUtil.getLong(request, RequestParams.MERCHANT_ID.toString(), -1);
        String userID = ServletRequestUtil.getString(request, RequestParams.USER_GID.toString());

        if (merchantID > 0 && StringUtils.isNotEmpty(userID)) {
//            userID = userService.resolveToken(userID);

            ResultInfo resultInfo = new ResultInfo();
            UserRelatedOrderInfo userRelatedOrderInfo = orderService.getOrderInfoForUser(userID, merchantID);
            List<String> picUrlList = userRelatedOrderInfo.getRecentlyTradePicUrlList();

            if (CollectionUtils.isNotEmpty(picUrlList)) {
                for (int i = 0; i < picUrlList.size(); i++) {
                    picUrlList.set(i, CONVERT_SERVICE.doConvert(picUrlList.get(i), CONVERT_HEAD_IMG_RELATIVE));
                }
            }

            resultInfo.addData("orderInfo", userRelatedOrderInfo);
            resultInfo.markSuccess();

            webAction.setResultInfo(resultInfo);
        } else {
            throw new WrongRequestParamException("输入参数有误：参数缺失");
        }
    }

    private void getProductItemUrlByGoodId(WebAction webAction) {
        HttpServletRequest request = webAction.getRequest();
        String goodId = ServletRequestUtil.getString(request, "goodId");

        if (StringUtils.isNotEmpty(goodId)) {
            ResultInfo resultInfo = new ResultInfo();
            resultInfo.addData("goodItemUrl", productService.getProductUrl(goodId));
            resultInfo.markSuccess();

            webAction.setResultInfo(resultInfo);
        } else {
            throw new WrongRequestParamException("输入参数有误：参数缺失");
        }

    }

    private void getRecommendProductList(WebAction webAction) {
        HttpServletRequest request = webAction.getRequest();
        String merchantID = ServletRequestUtil.getString(request, RequestParams.MERCHANT_ID.toString());

        if (StringUtils.isNotEmpty(merchantID) && NumberUtils.isDigits(merchantID)) {
            ResultInfo resultInfo = new ResultInfo();

            List<String> skuIDList = storeService.getRecommendProductList(merchantID);
            List<ProductInfo> productInfoList = Collections.emptyList();

            if (CollectionUtils.isNotEmpty(skuIDList)) {
                productInfoList = productService.batchRetrieveProductInfo(skuIDList);
                for (ProductInfo productInfo : productInfoList) {
                    productInfo.setProductURL(CONVERT_SERVICE.doConvert(productInfo.getProductURL(), CONVERT_HEAD_IMG_RELATIVE));
                    productInfo.setDefaultPic(CONVERT_SERVICE.doConvert(productInfo.getDefaultPic(), CONVERT_HEAD_IMG_RELATIVE));
                }
            }

            resultInfo.addData("recommendProductList", productInfoList);
            resultInfo.markSuccess();

            webAction.setResultInfo(resultInfo);
        } else {
            throw new WrongRequestParamException("输入参数有误：参数不合法");
        }
    }

    private void getMerchantOnlineInfo(WebAction webAction) {
        HttpServletRequest request = webAction.getRequest();
        String merchantIDs = ServletRequestUtil.getString(request, RequestParams.MERCHANTIDS.toString(), "");
        List<Integer> merchantIDList = StringUtil.stringToIntegerList(merchantIDs, ",");

        if (CollectionUtils.isNotEmpty(merchantIDList)) {
            ResultInfo resultInfo = new ResultInfo();

            List<MerchantOnlineInfo> merchantOnlineInfoList =
                    imApiService.getMerchantOnlineInfo(merchantIDList);

            resultInfo.addData("merchantOnlineInfo", merchantOnlineInfoList);
            resultInfo.markSuccess();

            webAction.setResultInfo(resultInfo);
        } else {
            throw new WrongRequestParamException("输入参数有误：参数不合法");
        }
    }

    private void getServiceGroupOnlineInfo(WebAction webAction) {
        HttpServletRequest request = webAction.getRequest();
        String groupIDs = ServletRequestUtil.getString(request, RequestParams.GROUP_IDS.toString(), "");
        List<Long> groupIDList = StringUtil.stringToLongList(groupIDs, ",");

        if (CollectionUtils.isEmpty(groupIDList)) {
            throw new WrongRequestParamException("输入参数不合法");
        }

        Map<Long, Set<Long>> groupToSubAccountList = kefuService.getSubAccountListUnderGroupList(groupIDList);
        List<Long> subMerchantIDList = new ArrayList<>(1);
        Map<Long, Long> subAccountToGroup = new HashMap<>(groupIDList.size());

        if (MapUtils.isNotEmpty(groupToSubAccountList)) {
            for (Map.Entry<Long, Set<Long>> entry : groupToSubAccountList.entrySet()) {
                for (Long subAccountID : entry.getValue()) {
                    subAccountToGroup.put(subAccountID, entry.getKey());
                }
                subMerchantIDList.addAll(entry.getValue());
            }
        } else {
            throw new WrongRequestParamException("分组下面并不存在任何子账号");
        }

        if (CollectionUtils.isNotEmpty(subMerchantIDList)) {
            ResultInfo resultInfo = new ResultInfo();

            List<MerchantOnlineInfo> subMerchantOnlineInfoList =
                    imApiService.getSubMerchantOnlineInfo(subMerchantIDList);
            Map<Long, MerchantOnlineInfo> serviceGroupOnlineInfoMap = new HashMap<>(groupIDList.size());

            for (Long groupID : groupIDList) {
                MerchantOnlineInfo groupOnlineInfo = new MerchantOnlineInfo();
                groupOnlineInfo.setId(groupID);
                serviceGroupOnlineInfoMap.put(groupID, groupOnlineInfo);
            }

            if (CollectionUtils.isNotEmpty(subMerchantOnlineInfoList)) {
                for (MerchantOnlineInfo subMerchantOnlineInfo : subMerchantOnlineInfoList) {
                    Long groupID = subAccountToGroup.get(subMerchantOnlineInfo.getId());
                    MerchantOnlineInfo serviceGroupOnlineInfo;

                    if (groupID != null && (serviceGroupOnlineInfo = serviceGroupOnlineInfoMap.get(groupID)) != null) {
                        if (!serviceGroupOnlineInfo.isOnline()) {
                            serviceGroupOnlineInfo.setOnline(subMerchantOnlineInfo.isOnline());
                        }
                    }
                }
            }

            resultInfo.addData("serviceGroupOnlineInfo", serviceGroupOnlineInfoMap.values());
            resultInfo.markSuccess();

            webAction.setResultInfo(resultInfo);
        } else {
            throw new WrongRequestParamException("输入参数有误：参数不合法");
        }
    }

    private void checkSessionTokenLegality(WebAction webAction) {
        HttpServletRequest request = webAction.getRequest();
        String encryptedSid = ServletRequestUtil.getString(request, "sid");
        String encryptedToken = ServletRequestUtil.getString(request, "token");

        if (StringUtils.isNotEmpty(encryptedSid) && StringUtils.isNotEmpty(encryptedToken)) {
            String sid = endecryptedManager.decryptedWithURLDecode(encryptedSid);

            if (StringUtils.isNotEmpty(sid)) {
                sid = sid.split(",")[1];
                Session session = SessionManager.getInstance().getSessionByID(sid);

                if (session != null) {
                    String guid = endecryptedManager.decryptedWithURLDecode(encryptedToken);

                    if (guid != null && session.isGUIDLegal((guid = guid.substring(0, 36)))) {
                        ResultInfo resultInfo = new ResultInfo();

                        resultInfo.addData("guid", guid);
                        resultInfo.markSuccess();

                        webAction.setResultInfo(resultInfo);
                    } else {
                        throw new WrongRequestParamException("无效token");
                    }
                } else {
                    throw new WrongRequestParamException("用户未登录");
                }
            } else {
                throw new WrongRequestParamException("无效sid");
            }
        } else {
            throw new WrongRequestParamException("字段不能为空");
        }
    }

    private void syncTime(WebAction webAction) {
        checkTimeDelta();

        ResultInfo resultInfo = new ResultInfo();
        int currentTime = (int) (System.currentTimeMillis() / 1000);

        resultInfo.addData("serverTime", (time_delta_local_cache + currentTime));
        resultInfo.addData("maxMessageContentLength", Constants.MAX_MESSAGE_CONTENT_LENGTH);
        resultInfo.markSuccess();

        webAction.setResultInfo(resultInfo);
    }

    private void checkTimeDelta() {
        if (time_delta_local_cache == null) {
            time_delta_local_cache = Constants.TIME_DELTA_BETWEEN_WEBIM_AND_IM;
        }
    }

    private void queryUserLoginInfo(WebAction webAction) {
        HttpServletRequest request = webAction.getRequest();
        String userIDListStr = ServletRequestUtil.getString(request, "userIDList");
        if (StringUtils.isEmpty(userIDListStr)) {
            throw new WrongRequestParamException("userIDList字段为空");
        }

        List<Integer> userIDList = StringUtil.stringToIntegerList(userIDListStr, ",");
        Map<Integer, Boolean> isUserLoginedMap = new HashMap<>(userIDList.size());
        SessionManager sessionManager = SessionManager.getInstance();

        for (Integer userID : userIDList) {
            isUserLoginedMap.put(userID, sessionManager.getSessionByID(userID + "") != null);
        }

        ResultInfo resultInfo = new ResultInfo();
        resultInfo.addData("userLoginInfo", isUserLoginedMap);
        resultInfo.markSuccess();
        webAction.setResultInfo(resultInfo);
    }

    private void getStoreName(WebAction webAction) {
        HttpServletRequest request = webAction.getRequest();
        String merchantID = ServletRequestUtil.getString(request, RequestParams.MERCHANT_ID.toString());

        if (StringUtils.isNotEmpty(merchantID) && NumberUtils.isNumber(merchantID)) {

            ResultInfo resultInfo = new ResultInfo();
            resultInfo.addData("storeName", merchantService.retrieveStoreName(merchantID));
            resultInfo.markSuccess();

            webAction.setResultInfo(resultInfo);
        } else {
            throw new WrongRequestParamException("输入参数有误：" + RequestParams.MERCHANT_ID + "为空");
        }
    }
}
