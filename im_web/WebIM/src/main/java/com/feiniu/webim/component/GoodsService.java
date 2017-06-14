package com.feiniu.webim.component;

import com.fasterxml.jackson.databind.JsonNode;
import com.feiniu.webim.bean.domain.GoodsAsking;
import com.feiniu.webim.bean.domain.GoodsInfo;
import com.feiniu.webim.bean.web.request.GetGoodsRequest;
import com.feiniu.webim.config.ConfigManager;
import com.feiniu.webim.exception.BusinessException;
import com.feiniu.webim.util.json.JSONUtil;
import com.feiniu.webim.util.json.JsonNodeUtil;
import com.feiniu.webim.util.web.HttpClientUtils;
import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;

import java.io.IOException;
import java.util.*;

/**
 * Created by zhangmin on 2015/12/8.
 */
public class GoodsService {

    public static final GoodsService instance = new GoodsService();

    //商品图片地址转换规则
    private static final ConvertService CONVERT_SERVICE = ConvertService.CONVERT_SERVICE;
    private static final List<ConvertService.ConvertRule> CONVERT_GOODS_PIC_URL =
            Arrays.asList(ConvertService.ConvertRule.MEKE_HEAD_IMG_ABSOLUTE, ConvertService.ConvertRule.TRANSFORM_OLD_MERCHANT_PHOTO_FROM_EXTERNAL);
    private static final String PRODUCT_SERVICE_URL = ConfigManager.getProperty("service_product");
    private static final String PRODUCT_ITEM_BASE_URL = ConfigManager.getProperty("product_item_base_url");
    private static final String PRODUCT_COMMENT_URL = ConfigManager.getProperty("product_comment_url");
    private static final String GOODS_SERACH_URL = ConfigManager.getProperty("service_goods_search");
    private static final String GOODS_GET_PRICE_URL = ConfigManager.getProperty("service_goods_get_price");
    private static final Logger LOGGER = Logger.getLogger(ProductService.class);
    private static final String SUCCSS_CODE = "200";


    public List<GoodsInfo> searchGoodsInfoForMerchant(GetGoodsRequest getGoodsRequest) {

        String response;
        List<GoodsInfo> goodsInfoArrayList = new ArrayList<>();
        Map<String, Object> paramMap = new HashMap<>();
        paramMap.put("shopid", getGoodsRequest.getMerchantId());
        //返回是否主商品字段
        paramMap.put("fl", "IS_MAIN_SKU");
        paramMap.put("q", getGoodsRequest.getKeyWord());
        paramMap.put("resnum", getGoodsRequest.getPageRows());
        paramMap.put("pn", getGoodsRequest.getCurPage());
        paramMap.put("sort", getGoodsRequest.getSort());
        paramMap.put("asc", getGoodsRequest.getAsc());

        //销量排序
        boolean isSortBySold = false;
        if (!StringUtils.isEmpty(getGoodsRequest.getSort()) && getGoodsRequest.getSort().equals("sm_soldqty")) {
            isSortBySold = true;
        }
        try {
            response = HttpClientUtils.excutePost(GOODS_SERACH_URL, paramMap);
            JsonNode jsonNode = JSONUtil.toNode(response);
            if (jsonNode != null) {
                JsonNode listGroups = jsonNode.get("grouped").get("NEW_SPEC_SEQ").get("groups");
                if (listGroups != null) {
                    Iterator<JsonNode> iterator = listGroups.iterator();
                    while (iterator.hasNext()) {
                        JsonNode docs = iterator.next().get("doclist").get("docs");
                        Iterator<JsonNode> iterator1 = docs.iterator();
                        while (iterator1.hasNext()) {
                            JsonNode docNode = iterator1.next();
                            //只返回主商品
                            if (JsonNodeUtil.getString(docNode, "IS_MAIN_SKU").equals("1")) {
                                GoodsInfo goodsInfo = JsonNodeUtil.parseBean(docNode.toString(), GoodsInfo.class);
                                goodsInfo.setSM_PIC(CONVERT_SERVICE.doConvert(goodsInfo.getSM_PIC(), CONVERT_GOODS_PIC_URL));
                                goodsInfoArrayList.add(goodsInfo);
                                //只有一个主商品，找到就break
                                break;
                            }
                        }

                    }
                }
            } else {
                throw new BusinessException("商品-商品搜索接口返回错误数据: " + response);
            }
        } catch (IOException e) {
            LOGGER.error("商品-商品搜索接口发生错误: " + JSONUtil.toJsonString(getGoodsRequest.getMerchantId()), e);
            throw new RuntimeException(e);
        }
        //批量获取飞牛价
        if (goodsInfoArrayList.size() > 0) {
            goodsInfoArrayList = attachGoodsFnPriceForMerchant(goodsInfoArrayList, isSortBySold);
        }
        return goodsInfoArrayList;

    }

    //获取飞牛价
    private List<GoodsInfo> attachGoodsFnPriceForMerchant(List<GoodsInfo> goodsInfoList, boolean isSortBySold) {

        //skuid拼接
        StringBuilder stringBuilderParams = new StringBuilder("");
        for (GoodsInfo goodsInfo : goodsInfoList) {
            stringBuilderParams.append(goodsInfo.getSM_SEQ() + ",");
        }
        stringBuilderParams.setLength(stringBuilderParams.length() > 1 ? (stringBuilderParams.length() - 1) : 0);

        //批量获取飞牛价和库存
        Map<Long, GoodsInfo> goodsInfoMap = new HashMap<>();
        List<Long> skuIdList = new ArrayList<>();
        //拼接skuid参数
        for (GoodsInfo goodsInfo : goodsInfoList) {
            skuIdList.add(goodsInfo.getSM_SEQ());
            goodsInfoMap.put(goodsInfo.getSM_SEQ(), goodsInfo);
        }
        List<GoodsAsking> goodsAskingList = getGoodsAskingInfo(skuIdList);
        for (GoodsAsking goodsAsking : goodsAskingList) {
            Double promotionPrice = goodsAsking.getSalePrice();
            goodsInfoMap.get(goodsAsking.getSkuId()).setFnPrice(promotionPrice);
            goodsInfoMap.get(goodsAsking.getSkuId()).setNum(goodsAsking.getNum());
            goodsInfoMap.get(goodsAsking.getSkuId()).setSold(goodsAsking.getSold());
        }
        if (isSortBySold) {
            Collections.sort(goodsInfoList, new Comparator<GoodsInfo>() {
                public int compare(GoodsInfo goodsInfo0, GoodsInfo goodsInfo1) {
                    return goodsInfo1.getSold() - goodsInfo0.getSold();
                }
            });

        }

        Iterator<GoodsInfo> it = goodsInfoList.iterator();
        while (it.hasNext()) {
            GoodsInfo goodsInfo = it.next();
            //get_price接口没查到，剔除掉
            if (goodsInfo.getFnPrice() == null || goodsInfo.getNum() == null || goodsInfo.getSold() == null) {
                it.remove();
            }
            //热销商品，如果销量为0，则不显示
            if (isSortBySold && goodsInfo.getSold().equals(0)) {
                it.remove();
            }
        }
        return goodsInfoList;
    }

    //获取正在咨询的商品(sku）的信息
    public List<GoodsAsking> getGoodsAskingInfo(List<Long> skuIdList) {
        //skuid拼接
        StringBuilder stringBuilderParams = new StringBuilder("");
        for (Long skuId : skuIdList) {
            stringBuilderParams.append(skuId + ",");
        }
        stringBuilderParams.setLength(stringBuilderParams.length() > 1 ? (stringBuilderParams.length() - 1) : 0);

        //获取正在咨询的商品信息
        String response;
        Map<String, Object> paramMap = new HashMap<>();
        paramMap.put("params", stringBuilderParams);
        List<GoodsAsking> goodsAskingList = new ArrayList<>();
        try {
            response = HttpClientUtils.excutePost(GOODS_GET_PRICE_URL, paramMap);
            JsonNode jsonNodeResponse = JSONUtil.toNode(response);
            if (jsonNodeResponse != null && SUCCSS_CODE.equals(JsonNodeUtil.getString(jsonNodeResponse, "code"))
                    && jsonNodeResponse.hasNonNull("body")) {
                JsonNode jsonNodeBody = jsonNodeResponse.get("body");
                Iterator<JsonNode> jsonNodeIterator = jsonNodeBody.iterator();
                while (jsonNodeIterator.hasNext()) {
                    GoodsAsking goodsAsking = new GoodsAsking();
                    JsonNode jsonNodeDetail = jsonNodeIterator.next();
                    goodsAsking = JsonNodeUtil.parseBean(jsonNodeDetail.toString(), GoodsAsking.class);
                    goodsAsking.setPic01(CONVERT_SERVICE.doConvert(goodsAsking.getPic01(), CONVERT_GOODS_PIC_URL));
                    goodsAsking.setProductURL(PRODUCT_ITEM_BASE_URL + goodsAsking.getSkuId());
                    goodsAskingList.add(goodsAsking);
                }
            } else {
                throw new BusinessException("商品-飞牛价查询接口返回错误数据: " + response);
            }
        } catch (IOException e) {
            LOGGER.error("商品-飞牛价查询发生错误: " + JSONUtil.toJsonString(skuIdList), e);
            throw new RuntimeException(e);
        }
        return goodsAskingList;
    }


}
