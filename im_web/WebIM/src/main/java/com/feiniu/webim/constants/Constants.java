package com.feiniu.webim.constants;

import com.feiniu.webim.config.ConfigManager;
import com.feiniu.webim.config.SystemConfig;
import com.feiniu.webim.util.general.StringUtil;

import java.util.Collections;
import java.util.List;

/**
 * Created by wangchen on 2014/12/5.
 */
public class Constants {
    /**
     * 客户端消息编码
     */
    public static final String PC_MESSAGE_ENCODING = "gbk";

    /**
     * web端消息编码
     */
    public static final String WEB_MESSAGE_ENCODING = "utf-8";

    /**
     * 单字节存储编码
     */
    public static final String GENERAL_ENCODING = "ISO-8859-1";

    public static final int MSG_TIME_TRANSFORM = 1000;

    public static final int NUMBER_OF_RECENTLY_SELLER_FETCH = 10;

    public static final String NIL_RESPONSE = "{}";

    public static final String NIL_LIST_RESPONSE = "[]";

    public static String MD5_KEY = "mallorder.feiniu.com_dsfsafsdfwefew";

    public static final String DEFAULT_WORD_SEPARATOR = ";";

    public static final int NUM_OF_MESSAGE_PER_PAGE = 25;

    //最大容忍不允许发言时间(秒)
    public static final int MAX_NON_SPEAK_TOLERANT_TIME;

    //推送不发言提示 时间节点
    public static final List<Integer> ALERT_SPEAK_NODE_LIST;

    public static final String UPLOAD_PHOTO_CATEGORY = "webim上传图片";

    //发送消息时，最多允许字数
    public static final int MAX_MESSAGE_CONTENT_LENGTH;
    /**
     * 当前环境标识 0：正式环境，1：压测环境
     **/
    public static final int CURRENT_ENV;

    public static final int SERVER_VERSION;

    /**
     * 自营客服 最小id
     */
    private static final int CS_KEFU_BOUNDARY = 200000000;

    public static final boolean isEnvTest() {
        return CURRENT_ENV == 1;
    }

    /**
     * im-server初始化时间
     */
    public volatile static Integer TIME_DELTA_BETWEEN_WEBIM_AND_IM = 0;

    static {
        SERVER_VERSION = ConfigManager.getIntProperty(SystemConfig.SERVER_VERSION.toString());
        CURRENT_ENV = Integer.parseInt(ConfigManager.getProperty(SystemConfig.CURRENT_ENV.toString(), "0"));
        MAX_NON_SPEAK_TOLERANT_TIME = Integer.parseInt(ConfigManager.getProperty(SystemConfig.MAX_NON_SPEAK_TOLERANT_TIME.toString(), "10"));

        String alertSpeakNodes = ConfigManager.getProperty(SystemConfig.ALERT_SPEAK_TIME_NODE_LIST.toString());
        ALERT_SPEAK_NODE_LIST = Collections.unmodifiableList(StringUtil.stringToIntegerList(alertSpeakNodes, ","));
        MAX_MESSAGE_CONTENT_LENGTH = Integer.parseInt(ConfigManager.getProperty("max_message_content_length", "500"));
    }

    //是否是自营客服
    public static final boolean isKefuFromCS(int id) {
        return id > CS_KEFU_BOUNDARY;
    }

    //没有客服在线时的文案
    public static final String TIP_FOR_NO_SUBACCOUNT_ONLINE = "亲爱的顾客，您可于9:00—22:00间访问在线客服或拨打400-920-6565热线，感谢您的支持。";
}
