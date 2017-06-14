package com.feiniu.subAccount.consts;

/**
 * Created by morningking on 2014/11/24.
 */
public class SystemConfig {
    public static final String DEFAULT_ENCODING = "utf-8";
    public static final int DEFAULT_SCALE = 1;
    public static final int STATISITC_TYPE_SUB_MERCHANT = 2;
    public static final int STATISITC_STATUS_NODATA = -1;
    public static final int STATISITC_STATUS_NOT_STATISITCED = 0;
    public static final int STATISITC_STATUS_UPDATED = 1;
    public static final int STATISITC_STATUS_FAILED = 2;
    public static final int STATISITC_STATUS_UPDATING = 3;

    public static final int STATISITC_MAX_STATISITC_WORKLOAD_MERCHANT_NUM = 50;
    public static final int STATISITC_MAX_STATISITC_WORKLOAD_SUB_MRCHANT_NUM = 200;

    public static final int STATISITC_MSECOND_TO_SECOND=1000;
    public static final int STATISITC_ONE_DAY_SECOUNDS=86400;

    public static final int ATTENDANCE_INFO = 0;
    public static final int ATTENDANCE_DETAIL_INFO=1;
    public static final int WORKLOAD_INFO=2;
    public static final int WORKLOAD_DETAIL_INFO=3;

    public static final String MEMCACHE_INFO = "MerchantInfo";
    public static final String SUB_MERCHANT_KEY="SubMerchntInfoList";
    public static final Integer FRIEND_LIST_TIMEOUT = 3*60*60;
}
