package com.feiniu.imServer.consts;

/**
 * Created by wangzhen on 2016/3/4.
 */
public class ShuntConfig {
    public static final String MEMCACHE_RECPTION_LIST = "CServiceRecptionList";
    public static final String MEMCACHE_SERVICE_IN_GROUP = "CServiceListInGroup";
    public static final String MEMCACHE_INFO = "MerchantInfo";
    public static final String MEMCACHE_CS_QUEUE = "CsQueue";
    public static final String MEMCACHE_CS_QUEUE_ITEM = "CsQueueItem";
    public static final String MEMCACHE_USER_INFO = "userinfo";
    public static final String MEMCACHE_FRIEND_LIST = "friendList";
    public static final String MEMCACHE_FRIEND_GROUP_LIST = "friendGroupList";
    public static final Long  MEMCACHE_FRIEND_GROUP_ID = Long.valueOf(250);

    public static final String MEMCACHE_FEILD_CS_USERID = "csUserID";
    public static final String MEMCACHE_FEILD_GROUP_USERID = "csTeamID";
    public static final String MEMCACHE_FEILD_ONLINE_STATUS = "status";
    public static final String MEMCACHE_FEILD_LAN_IP = "lanip";

    public static final String MEMCACHE_FEILD_CSTEAMID = "csTeamID";


    public static final int MAX_MESSAGE_COUNT = 10000;
    public static final String LOCK_NAME = "#LOCK";
    public static final int LOCK_EXPIRED = 3 * 1000;
    public static final int LOCK_TIMEOUT = 3 * 1000;
    public static Long MAX_REQUEST_TIME = Long.valueOf("10000000000");
    public static Integer MAX_USER_LEVEL =Integer.valueOf(9);
    public static int THREAD_POOL_SIZE = 10;
    public static int THREAD_MAX_POOL_SIZE = 20;
    public static long THREAD_KEEP_ALIVE_TIME = 6000;
    public static Integer MAX_RECEPTION_RATE = Integer.valueOf("100000");
    public static String QUEUE_ZMQ_PORT="9999";
    public static Integer QUEUE_ZMQ_THREAD_NUMBER=1;
    public static String QUEUE_ZMQ_CHANNEL="queueinfo ";

    public static final String REDIS_CONSULT ="0";
    public static final String REDIS_RECPTION ="1";
    public static final String RECENT_INFO_CUSTOMER="recentInfo";
    public static final String LAST_MSG_CUSTOMER="lastMsg";
    public static final Integer RECENT_INFO_EXPIRE_TIME=24*60*60*7;

    public static final Integer FRIEND_SELF_LIST_TIMEOUT = 60*60;
    public static final Integer FRIEND_LIST_TIMEOUT = 3*60*60;
    public static final Integer USER_INFO_TIMEOUT = 3*60*60;
}
