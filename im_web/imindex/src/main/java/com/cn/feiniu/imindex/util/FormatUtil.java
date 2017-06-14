package com.cn.feiniu.imindex.util;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.sql.Timestamp;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Author: morningking
 * Date: 2016/2/14
 * Contact: 243717042@qq.com
 */
public class FormatUtil {
    private static final Logger LOGGER = LoggerFactory.getLogger(FormatUtil.class);
    public static final SimpleDateFormat TIME_FORMAT = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    public static final SimpleDateFormat DATE_FORMAT = new SimpleDateFormat("yyyy-MM-dd");
    public static final SimpleDateFormat CHINA_TIME_FORMAT = new SimpleDateFormat("yyyy年MM月dd日HH时mm分ss秒");

    /**
     * 格式化为 中文日期显示方式
     *
     * @param timestamp
     * @return
     */
    public static final String formatChinaTime(Timestamp timestamp) {
        if (timestamp == null) {
            return "";
        } else {
            return CHINA_TIME_FORMAT.format(timestamp);
        }
    }

    // 1999-00-00
    public static String convertNumberToDate(long timeInNumber) {
        Date date = new Date(timeInNumber * 1000);
        SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd");
        return format.format(date);
    }

    // 00:00
    public static String convertNumberToMinTime(int timeInNumber) {
        int hour = timeInNumber / 3600;
        int min = (timeInNumber % 3600) / 60;

        if (hour == 24 && min == 0) {
            hour = 23;
            min = 59;
        }

        return String.format("%02d:%02d", hour, min);
    }

    //00:00:00
    public static String convertNumberToSecTime(int timeInNumber) {
        int hour = timeInNumber / 3600;
        int min = (timeInNumber % 3600) / 60;
        int sec = timeInNumber - hour * 3600 - min * 60;

        if (hour == 24 && min == 0 && sec == 0) {
            hour = 23;
            min = 59;
            sec = 59;
        }
        return String.format("%02d:%02d:%02d", hour, min, sec);
    }

    public static final String formatDate(Date date) {
        return DATE_FORMAT.format(date);
    }

    public static final String formatTime(Date date) {
        return TIME_FORMAT.format(date);
    }

    public static final String formatTime(Timestamp timestamp) {
        return TIME_FORMAT.format(timestamp);
    }

    public static final Date formatStringDate(String dateString) {
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        try {
            return sdf.parse(dateString);
        } catch (ParseException e) {
            LOGGER.error("formatStringDate error : ", e);
        }
        return null;
    }

    public static String convertNumberToDateAndTime(long timeInNumber) {
        Date date = new Date(timeInNumber * 1000);
        SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        return format.format(date);
    }
}
