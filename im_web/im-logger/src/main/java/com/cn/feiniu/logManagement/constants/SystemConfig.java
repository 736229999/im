package com.cn.feiniu.logManagement.constants;

/**
 * Author: morningking
 * Date: 2015/8/3
 * Contact: 243717042@qq.com
 */
public class SystemConfig {
    /**
     * 记录子账号动作日志时，列之间分隔符 默认值
     */
    public static final String SUB_MERCHANT_ACTION_LOG_FIELD_SEPARATOR = " ";

    /**
     * 系统行分隔符
     */
    public static final String SYSTEM_LINE_SEPARATOR = System.getProperty("line.separator");

    /**
     * 默认编码字符集
     */
    public static final String DEFAULT_ENCODING = "utf-8";

    /**
     * java当前时间毫秒数 和 unix时间戳[秒数] 之间转换因子
     */
    public static final long UNIX_TIME_TRANSFORM_FACTOR = 1000l;
}
