package com.feiniu.webim.config;

/**
 * Created by wangchen on 2014/12/16.
 */
public enum SystemConfig {
    /**
     * session_timeout_mins 配置会话失效时间(分钟)
     */
    SESSION_TIMEOUT_MINS,

    //聊天窗口失效时间
    CHAT_WINDOW_NON_ACTIVE_TORELANT_MINIS,

    /**
     * timer_intercal_mins 配置检查
     */
    TIMER_INTERVAL_MINS,

    /**
     * 心跳间隔时间
     */
    HEART_BEAT_INTERVAL_MILLS,

    /**
     * udp_server_host
     */
    SERVER_HOST,

    /**
     * udp_server_host
     */
    SERVER_PORT,

    /**服务器版本**/
    SERVER_VERSION ,

    /**
     * 发送队列最大容量
     */
    MESSENGER_SENDING_QUEUE_CAPACITY,
    MESSENGER_RECEIVING_QUEUE_CAPACITY,
    MESSENGER_SENDING_QUEUE_PUT_TIMEOUT,
    MESSENGER_SENDING_QUEUE_PUT_MAX_RETRY_TIME,
    /**
     * chat_message_maxwaittime_millis 聊天新消息等待时间
     */
    CHAT_MESSAGE_MAXWAITTIME_MILLIS,
    /**
     * general_message_maxwaittime_millis 非聊天新消息等待时间
     */
    GENERAL_MESSAGE_MAXWAITTIME_MILLIS,
    /**
     * 消息队列默认最大堆积数目
     */
    MAX_MESSAGE_IN_QUEUE,


    /**
     * dateformat,消息时间格式
     */
    DATEFORMAT,

    /**
     * queue_capacity,消息队列容量
     */
    QUEUE_CAPACITY,

    /**
     * number_of_history_list_fetch, 每次获取历史消息的条数
     */
    NUMBER_OF_HISTORY_LIST_FETCH,

    FN_LOGIN,

    WEBIM_DATA_GET_SESSION_TIMEOUT_MILL_SECONDS,

    /**webim环境变量**/
    CURRENT_ENV,

    /**
     * 最大容忍买家 不发言时间
     */
    MAX_NON_SPEAK_TOLERANT_TIME,

    /**
     * 推送不发言提示 时间节点
     */
    ALERT_SPEAK_TIME_NODE_LIST,


    ;

    @Override
    public String toString() {
        return super.name().toLowerCase();
    }
}
