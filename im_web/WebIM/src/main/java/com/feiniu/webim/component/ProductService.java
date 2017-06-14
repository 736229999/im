package com.feiniu.webim.component;

import com.fasterxml.jackson.databind.JsonNode;
import com.feiniu.webim.bean.domain.MerchantProductInfo;
import com.feiniu.webim.bean.domain.ProductInfo;
import com.feiniu.webim.bean.web.request.GetProductListRequest;
import com.feiniu.webim.config.ConfigManager;
import com.feiniu.webim.exception.BusinessException;
import com.feiniu.webim.exception.HttpGetException;
import com.feiniu.webim.util.json.JSONUtil;
import com.feiniu.webim.util.json.JsonNodeUtil;
import com.feiniu.webim.util.web.HttpClientUtils;
import org.apache.commons.collections4.CollectionUtils;
import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;

import java.io.IOException;
import java.util.*;

/**
 * Created by wangchen on 2015/1/14.
 */
public class ProductService {
    private ProductService() {
    }

    public static final ProductService instance = new ProductService();

    private static final String PRODUCT_SERVICE_URL = ConfigManager.getProperty("service_product");
    private static final String PRODUCT_ITEM_BASE_URL = ConfigManager.getProperty("product_item_base_url");
    private static final String PRODUCT_COMMENT_URL = ConfigManager.getProperty("product_comment_url");
    private static final Logger LOGGER = Logger.getLogger(ProductService.class);
    private static final String SUCCSS_CODE = "200";

    public ProductInfo retrieveProductInfo(String skuID) {
        String response;
        ProductInfo productInfo;

        try {
            response = HttpClientUtils.executeGet(PRODUCT_SERVICE_URL, "/goodsinfo_api/get_moumouattribute_by_skuid/" + skuID + ".html",
                    Collections.EMPTY_MAP);
            JsonNode jsonNode = JSONUtil.toNode(response);

            if (jsonNode != null && (jsonNode = jsonNode.get("datas")) != null
                    && SUCCSS_CODE.equals(JsonNodeUtil.getString(jsonNode, "code")) && jsonNode.hasNonNull("body")) {
                productInfo = JsonNodeUtil.injectBean(jsonNode.get("body").toString(), ProductInfo.class);
                if (productInfo != null) {
                    productInfo.setProductURL(PRODUCT_ITEM_BASE_URL + skuID);
                    productInfo.setCommentURL(PRODUCT_COMMENT_URL + skuID);
                }
            } else {
                throw new BusinessException("商品-获取商品接口返回错误数据：" + response);
            }
        } catch (HttpGetException e) {
            LOGGER.error("商品-获取商品相关信息发生错误");
            throw new RuntimeException(e);
        }

        return productInfo;
    }

    public List<MerchantProductInfo> retrieveProductInfoForMerchant(GetProductListRequest getProductListRequest) {
        String response;
        List<MerchantProductInfo> productInfoList = new ArrayList<>();
        Map<String, Object> paramMap = new HashMap<>();
        paramMap.put("param", JSONUtil.toJsonString(getProductListRequest));

        try {
            response = HttpClientUtils.excutePost("http://" + PRODUCT_SERVICE_URL + "/rest/mallGoods/get_mall_goods_info.html", paramMap);
            JsonNode jsonNode = JSONUtil.toNode(response);

            if (jsonNode != null && (jsonNode = jsonNode.get("datas")) != null
                    && SUCCSS_CODE.equals(JsonNodeUtil.getString(jsonNode, "code")) && jsonNode.hasNonNull("body")) {
                JsonNode listNode = jsonNode.get("body").get("list");

                if (listNode != null) {
                    Iterator<JsonNode> iterator = listNode.iterator();
                    while (iterator.hasNext()) {
                        JsonNode currentProductNode = iterator.next();

                        productInfoList.add(JsonNodeUtil.parseBean(currentProductNode.toString(), MerchantProductInfo.class));
                    }
                }
            } else {
                throw new BusinessException("商品-查询商品接口返回错误数据: " + response);
            }
        } catch (IOException e) {
            LOGGER.error("商品-查询商品相关信息发生错误: " + JSONUtil.toJsonString(getProductListRequest), e);
            throw new RuntimeException(e);
        }

        return productInfoList;
    }

    public String getProductUrl(String goodsId) {
        String response;
        String skuID;

        try {
            response = HttpClientUtils.executeGet(PRODUCT_SERVICE_URL, "/detailpage_api/get_cheapest_skuid_by_goodsid/" + goodsId + ".html",
                    Collections.EMPTY_MAP);
            JsonNode jsonNode = JSONUtil.toNode(response);

            if (jsonNode != null && SUCCSS_CODE.equals(JsonNodeUtil.getString(jsonNode, "code")) && jsonNode.hasNonNull("body")) {
                skuID = jsonNode.get("body").get("cheapest").asLong() + "";
                skuID = PRODUCT_ITEM_BASE_URL + skuID;
            } else {
                throw new BusinessException("商品-获取商品接口返回错误数据：" + response);
            }
        } catch (HttpGetException e) {
            LOGGER.error("商品-获取商品相关信息发生错误");
            throw new RuntimeException(e);
        }

        return skuID;

    }

    public List<ProductInfo> batchRetrieveProductInfo(List<String> skuIDList) {
        if (CollectionUtils.isEmpty(skuIDList)) {
            throw new RuntimeException("商品-批量获取商品相关信息发生错误");
        }

        try {
            Map<String, Object> paramsMap = new HashMap<>(1);
            paramsMap.put("params", StringUtils.join(skuIDList, ","));

            String response = HttpClientUtils.executeGet(PRODUCT_SERVICE_URL, "/cp_goodsinfo_api/get_sku_batch.html", paramsMap);
            JsonNode jsonNode = JSONUtil.toNode(response);

            if (jsonNode != null && SUCCSS_CODE.equals(JsonNodeUtil.getString(jsonNode, "code")) && jsonNode.hasNonNull("body")) {
                JsonNode listNode = jsonNode.get("body");
                List<ProductInfo> productInfoList = new ArrayList<>(skuIDList.size());

                if (listNode != null) {
                    Iterator<JsonNode> iterator = listNode.iterator();
                    Map<String, ProductInfo> productInfoMap = new HashMap<>(skuIDList.size());

                    while (iterator.hasNext()) {
                        JsonNode currentProductNode = iterator.next();
                        String skuID = JsonNodeUtil.getString(currentProductNode, "SKU");

                        ProductInfo productInfo = new ProductInfo();
                        productInfo.setTitle(JsonNodeUtil.getString(currentProductNode, "ITNAME"));
                        productInfo.setDefaultPic(JsonNodeUtil.getString(currentProductNode, "SM_PIC"));
                        productInfo.setPrice(JsonNodeUtil.getDouble(currentProductNode, "SM_PRICE"));
                        productInfo.setProductURL(PRODUCT_ITEM_BASE_URL + skuID);
                        productInfo.setCommentURL(PRODUCT_COMMENT_URL + skuID);
                        productInfoMap.put(skuID, productInfo);
                    }

                    for (String skuID : skuIDList) {
                        ProductInfo productInfo = productInfoMap.get(skuID);
                        if (productInfo != null) {
                            productInfoList.add(productInfo);
                        }
                    }
                }

                return productInfoList;
            } else {
                throw new BusinessException("商品-批量获取商品相关信息发生错误：" + response);
            }
        } catch (HttpGetException e) {
            LOGGER.error("商品-批量获取商品相关信息发生错误");
            throw new RuntimeException(e);
        }
    }

    public void retrieveProductSeriesProperty(ProductInfo productInfo) {
        String response;

        try {
            response = HttpClientUtils.executeGet(PRODUCT_SERVICE_URL, "/moumou/is_specType/" + productInfo.getGoodsId() + ".html",
                    Collections.EMPTY_MAP);
            JsonNode jsonNode = JSONUtil.toNode(response);

            if (jsonNode != null && SUCCSS_CODE.equals(JsonNodeUtil.getString(jsonNode, "code")) && jsonNode.hasNonNull("body")) {
                productInfo.setIsSeries(jsonNode.get("body").asBoolean());
            } else {
                throw new BusinessException("商品-获取商品是哦福是接口返回错误数据：" + response);
            }
        } catch (HttpGetException e) {
            LOGGER.error("商品-获取商品相关信息发生错误");
            throw new RuntimeException(e);
        }

    }
}
