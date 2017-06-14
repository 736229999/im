package com.feiniu.imServer.util;


import com.feiniu.imServer.bean.CachedUserInfo;
import com.feiniu.imServer.consts.SystemConfig;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.sql.Timestamp;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

/**
 * Created by Dell on 14-9-30.
 */
public class FormatUtils {
    private static final Logger logger = LoggerFactory.getLogger(JsonUtil.class);
    public static final SimpleDateFormat TIME_FORMAT = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    public static final SimpleDateFormat DATE_FORMAT = new SimpleDateFormat("yyyy-MM-dd");
    public static final SimpleDateFormat HTML_TIME_FORMAT = new SimpleDateFormat("yyyy年MM月dd日-HH:mm:ss");


    public static final String formatHTMLTime(Timestamp timestamp) {
        if (timestamp == null) {
            return "";
        }else {
            return HTML_TIME_FORMAT.format(timestamp).replace("-", "<br>");
        }
    }

    /*
     * 时间相关
     */
    // 1999-00-00
    public static String convertNumberToDate(long timeInNumber){
        Date date = new Date(timeInNumber *1000);
        SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd");
        return  format.format(date);
    }
    // 00:00
    public static String convertNumberToMinTime(int timeInNumber){
        int hour = timeInNumber / 3600;
        int min = (timeInNumber % 3600) /60;

        if (hour == 24 && min ==0){
            hour = 23;
            min = 59;
        }

        return  String.format("%02d:%02d",hour,min);
    }
    //00:00:00
    public static String convertNumberToSecTime(int timeInNumber){
        int hour = timeInNumber / 3600;
        int min = (timeInNumber % 3600) /60;
        int sec = timeInNumber - hour * 3600 - min * 60;

        if (hour == 24 && min ==0 && sec ==0){
            hour = 23;
            min = 59;
            sec = 59;
        }
        return  String.format("%02d:%02d:%02d",hour,min,sec);
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

    public static final Date formatStringDate(String dateString){
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        try {
            return  sdf.parse(dateString);
        } catch (ParseException e) {
            logger.error("formatStringDate error : ",e);
        }
        return  null;
    }

    public static String convertNumberToDateAndTime(long timeInNumber){
        Date date = new Date(timeInNumber *1000);
        SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        return  format.format(date);
    }

    public  static long getTodayTimeInSeconds(){
        Calendar calendar = Calendar.getInstance();
        calendar.set(Calendar.HOUR_OF_DAY, 0);
        calendar.set(Calendar.MINUTE, 0);
        calendar.set(Calendar.SECOND, 0);
        return calendar.getTimeInMillis()/ 1000;
    }

    public  static  long getTomorrowTimeInSeconds(){
        Calendar calendar = Calendar.getInstance();
        calendar.set(Calendar.HOUR_OF_DAY, 0);
        calendar.set(Calendar.MINUTE, 0);
        calendar.set(Calendar.SECOND, 0);
        calendar.add(Calendar.DATE, 1);
        return calendar.getTimeInMillis()/ 1000;
    }

    public static long getMinStartTime(String startTime){
        SimpleDateFormat format =   new SimpleDateFormat( "yyyy-MM-dd" );
        if(startTime == null || startTime == "")
            return 0;
        else {
            try {
                return  format.parse(startTime).getTime()/1000;
            }catch (Exception e){
                return  0;
            }
        }
    }

    public static long getMaxEndTime(String endTime){
        SimpleDateFormat format =   new SimpleDateFormat( "yyyy-MM-dd" );
        if(endTime == null || endTime == "")
            return SystemConfig.MAX_END_TIME;
        else {
            try {
                return  format.parse(endTime).getTime()/1000+ SystemConfig.STATISITC_ONE_DAY_SECOUNDS;
            }catch (Exception e){
                return  SystemConfig.MAX_END_TIME;
            }
        }
    }


    public static boolean isStatusOnline(Integer status){
        if (status == -1 || status == 19){
            return false;
        }

        return true;
    }

    public  static boolean isServiceCanShunt(CachedUserInfo cachedUserInfo){
        Integer status = Integer.valueOf(cachedUserInfo.getStatus());
        Integer flow = Integer.valueOf(cachedUserInfo.getFlow());

        if ( flow != 0 ||status == -1 || status == 19 || status == 1 || status == 2){
            return false;
        }
        return true;
    }

    public static long ipToLong(String strIp){
        long[] ip = new long[4];
        //先找到IP地址字符串中.的位置
        int position1 = strIp.indexOf(".");
        int position2 = strIp.indexOf(".", position1 + 1);
        int position3 = strIp.indexOf(".", position2 + 1);
        //将每个.之间的字符串转换成整型
        ip[0] = Long.parseLong(strIp.substring(0, position1));
        ip[1] = Long.parseLong(strIp.substring(position1+1, position2));
        ip[2] = Long.parseLong(strIp.substring(position2+1, position3));
        ip[3] = Long.parseLong(strIp.substring(position3+1));
        return (ip[3] << 24) + (ip[2] << 16) + (ip[1] << 8) + ip[0];
    }

    private static byte[] longToByte(long i) {
        byte[] bytes = new byte[4];

        bytes[0] = (byte) (0xff & i);
        bytes[1] = (byte) ((0xff00 & i) >> 8);
        bytes[2] = (byte) ((0xff0000 & i) >> 16);
        bytes[3] = (byte) ((0xff000000 & i) >> 24);

        return bytes;
    }

    public static String longToIP(long longIp) {
        byte[] bytes = longToByte(longIp);
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < 4; i++) {
            sb.append(bytes[i] & 0xFF);
            if (i < 3) {
                sb.append(".");
            }
        }
        return sb.toString();
    }

}
