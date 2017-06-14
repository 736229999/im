package com.cn.feiniu.logManagement.core;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Author: morningking
 * Date: 2015/7/31
 * Contact: 243717042@qq.com
 */
public class LogFileNamePatternManager {
    public enum LogTypeEnum {
        SUB_MERCHANT_ACTION_LOG_PATTERN("submerchant-action-log-%d{yyyy-MM-dd}.log"),;

        LogTypeEnum(String pattern) {
            this.logTypePattern = pattern;
        }

        public String getLogTypePattern() {
            return logTypePattern;
        }

        private String logTypePattern;
    }

    public static final Pattern TIME_PATTERN = Pattern.compile("([%]d[{]([^}]+)[}])");
    public static final Map<String, SimpleDateFormat> FORMAT_MAP = new HashMap<>(1);

    public String getLogFileName(LogTypeEnum logTypeEnum, Map<String, Object> params) {
        if (logTypeEnum == null) {
            throw new IllegalArgumentException("日志类型为空");
        }

        switch (logTypeEnum) {
            case SUB_MERCHANT_ACTION_LOG_PATTERN:
                String logType = logTypeEnum.getLogTypePattern();
                Matcher matcher = TIME_PATTERN.matcher(logType);
                Date baseDate = new Date();
                if (params.containsKey("date")) {
                    baseDate = (Date) params.get("date");
                }

                if (matcher.find()) {
                    String patternType = matcher.group(1);
                    String pattern = matcher.group(2);

                    FORMAT_MAP.put(patternType, new SimpleDateFormat(pattern));
                }

                for (Map.Entry<String, SimpleDateFormat> entry : FORMAT_MAP.entrySet()) {
                    logType = logType.replace(entry.getKey(), entry.getValue().format(baseDate));
                }

                return logType;
            default:
                return null;
        }
    }

    public static void main(String[] args) {
    }
}
