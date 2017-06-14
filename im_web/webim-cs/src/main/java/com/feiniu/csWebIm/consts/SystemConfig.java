package com.feiniu.csWebIm.consts;

/**
 * Created by wangzhen on 2015/10/26.
 */
public class SystemConfig {
    public static final String MEMBER_API_SEARCH="/member_api/searchMember";
    public static final String MEMEER_LEVEL_SEARCH="/FeiniuScore/growth/mem/queryMemLevel";
    public static final String CONTACTION_TEL_SEARCH="/rest/consignee/get_list_by_mem_guid";
    public static final String FEINIU_CART_GET_CART_FOR_CUSTOMER="FeiniuCart/rest/v1/getCartForCustomer";
    public static final String HTTP_HEAD="http://";
    public static final String HTTPS_HEAD="https://";
    public static final String PIC_SIZE="_160x160.jpg";
    public static final String ITEM_HEAD="/sh/item/";
    public static final int STATISITC_STATUS_NODATA = -1;
    public static final String EMPLOYEE_INFO ="rest/backOperator/id/";
    public static final String WEBIM_AUTH_METHOD="webim/vender";
    public static final String IM_AUTH_METHOD="queryUserToken";
    public static final long TOKEN_TIMEOUT_LEN = 3600;
    public static final String TOKEN_KEY="userinfo#magicNumber#userid";
    public static final String FAQ_METHOD="/faq/get-all-showed-faq.do";

    //描述字段 个数
    public static final int DESCRIPTION_ITEM_NUMBER = 7;
    public static final String[] WEB_RIGHT_TITILES={"最近订单","咨询订单"};
    public static final String ORDER_DETAIL_URL="/order/orderDetail/";

    public static final String TOTAL_COST = "OrderQuery/customerdesk/customerDeskConsumptionAmount";
    public static final String RECENT_DEALS = "OrderQuery/customerdesk/customerDeskLastOrderDetail";
    public static final String ORDER_CONSULT ="OrderQuery/customerdesk/customerDeskOrderDetail";
    public static final String LOGISTICS_INFO ="OrderQuery/shanghai/v1/expInfo";
    public static final String PRODUCT_DETAIL ="customer_service_api/moumou_product_detail";
    public static final String SAVE_SERVICELOG ="customer_service_api/add_cs_record_for_other_platform";
    public static final String ASKING_QUESTION ="service-provider/faq-category/get-all-showed-faq-category.do";
    public static final String SAVE_QUESTIONS_TO_BACKEND ="service-provider/faq-category/update-consult-info.do?";

    public static final Long MAX_TIME=Long.valueOf("10000000000000");
    //运行时标识 值
    public static final char RUNTIME_INDICATOR = '1';
    public static final int DEFAULT_SCALE_LEN = 2;

    public static final int DEFAULT_ENCODE_SERVICE_ID = 36;
    public static final String AUTH_TOKEN_KEY= "YOWEJKRLWKFSJK";
    public static final String SESSION_KEY="sessionkey";
}
