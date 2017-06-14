package com.feiniu.fnSubAccount.consts;

/**
 * Created by morningking on 2014/11/24.
 */
public class SystemConfig {
    public static final String DEFAULT_ENCODING = "utf-8";
    public static final int DEFAULT_SCALE = 1;
    public static final int DEFAULT_SCALE_COUNT = 2;
    public static final int STATISITC_TYPE_BACKEND = 1;
    public static final int STATISITC_TYPE_SUB_MERCHANT = 2;
    public static final int STATISITC_STATUS_NODATA = -1;
    public static final int STATISITC_STATUS_NOT_STATISITCED = 0;
    public static final int STATISITC_STATUS_UPDATED = 1;
    public static final int STATISITC_STATUS_FAILED = 2;
    public static final int STATISITC_STATUS_UPDATING = 3;

    public static final int MAX_GET_STATUS_ID_COUNT =10;
    public static final int SECONDS_IN_HOUR =3600;
    public static final String statusMap[]={"已关闭","已显示","错误"};

    public static final String appraiseStatus[]={"未评估","非常不满意","不满意","一般","满意","非常满意"};
    public static final int STATISITC_ONE_DAY_SECOUNDS=86400;
    public static final int STATISITC_ONE_DAY_TEN_SECOUNDS=8640;


    public static final int STATISITC_ITEM_COUNT=14;
    public static final int STATISITC_MAX_ITEM_PER_PAGE=50;

    public static final int STATISITC_MAX_STATISITC_WORKLOAD_MERCHANT_NUM = 50;
    public static final int STATISITC_MAX_STATISITC_WORKLOAD_SUB_MRCHANT_NUM = 200;

    public static final int STATISITC_MSECOND_TO_SECOND=1000;

    public static final int ATTENDANCE_INFO = 0;
    public static final int ATTENDANCE_DETAIL_INFO=1;
    public static final int WORKLOAD_INFO=2;
    public static final int WORKLOAD_DETAIL_INFO=3;
    public static final String EXPROT_FILE_NAME[] ={"出勤率统计报表","出勤率明细统计报表","工作量统计报表","工作量明细统计报表"};


    public static final int MAX_PAGE_ITEM_SIZE = 99999999;
    public static final long MAX_END_TIME = 2000000000;
    public static final String CUSTOMER_SERVICE_MERCHANT_ID = "-1";

    public static final String ORGAN_ID = "$organId";
    public static final String GET_ORGAN_URL = "/rest/organ/$organId/ids";
    public static final String HTTP_URL= "http://";
    public static final String GET_BACKOPERATERS_URL= "/rest/backOperator/getBackOperatersByOrganIds";

    //设计时标识 值
    public static final char DESIGN_TIME_INDICATOR = '0';
    //运行时标识 值
    public static final char RUNTIME_INDICATOR = '1';

    //描述字段 个数
    public static final int DESCRIPTION_ITEM_NUMBER = 7;
    public static final String GET_REAL_ID="/rest/backOperator/id/";
    public static final String GET_REAL_NAME="/rest/backOperator/realname/";
    public static final String GET_USER_NAME="/rest/backOperator/username/";

    public static final String BACKEND_GET_OPERATOR_METHOD[]={
            GET_REAL_ID,GET_REAL_NAME,GET_USER_NAME
    };

    public static final String MEMCACHE_SERVICE_INFO= "CServiceInfo";
    public static final String MEMCACHE_SERVICE_LIST="CServiceList";
    public static final String MEMCACHE_SERVICE_IN_GROUP = "CServiceListInGroup";
    public static final String MEMCACHE_CS_QUEUE="CsQueue";
    public static final String QUICK_PHRASE_TITILE ="快捷编码";

    public static final String QUEUE_NUMBER_TAG="teamID";
    public static final String SUB_MERCHANT_KEY="SubMerchntInfoList";
    public static final Integer FRIEND_LIST_TIMEOUT = 3*60*60;

    public static final String USER_LEVEL_DESC[]={"普通会员","银卡会员","金卡会员","白金卡会员"};
}
