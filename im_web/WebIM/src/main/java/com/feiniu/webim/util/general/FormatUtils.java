package com.feiniu.webim.util.general;

import java.sql.Timestamp;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Created by Dell on 14-9-30.
 */
public class FormatUtils {
    public static final SimpleDateFormat TIME_FORMAT = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    public static final SimpleDateFormat DATE_FORMAT = new SimpleDateFormat("yyyy-MM-dd");
    public static final SimpleDateFormat HTML_TIME_FORMAT = new SimpleDateFormat("yyyy年MM月dd日-HH:mm:ss");
    public static final SimpleDateFormat GENERAL_DATE_FORMAT = new SimpleDateFormat("yyyyMMdd");

    public static final String formatDate(Date date) {
        return DATE_FORMAT.format(date);
    }

    public static final String formatTime(Date date) {
        return TIME_FORMAT.format(date);
    }

    public static final String formatTime(Timestamp timestamp) {
        return TIME_FORMAT.format(timestamp);
    }

    public static final String formatHTMLTime(Timestamp timestamp) {
        if (timestamp == null) {
            return "";
        }else {
            return HTML_TIME_FORMAT.format(timestamp).replace("-", "<br>");
        }
    }

    public static final String formatGeneralTime(Date date) {
        return GENERAL_DATE_FORMAT.format(date);
    }

}
