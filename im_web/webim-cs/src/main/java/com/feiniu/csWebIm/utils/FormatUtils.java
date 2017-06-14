package com.feiniu.csWebIm.utils;

import com.feiniu.csWebIm.bean.domain.AskingInfo;
import com.feiniu.csWebIm.bean.domain.ProductData;
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
    private static final Logger logger = LoggerFactory.getLogger(FormatUtils.class);
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
        SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");
        return  format.format(date);
    }

    /*
    * SQL 语句相关
    */
    public  static String getSqlGetAskingCount(long startTime ,long endTime){

        String sql = "SELECT pqb.* ,(case when pqa.askcount is NULL  THEN 0 ELSE pqa.askcount END) AS ASKCOUNT, " +
                " (case when pqc.replycount is NULL  THEN 0 ELSE pqc.replycount END) AS  replyCount , " +
                " ( CASE WHEN pqd.notReplyCount IS NULL THEN 0 ELSE  pqd.notReplyCount END) AS NOTREPLYCOUNT FROM   " +
                "(SELECT " +
                "  i_submerchant_id, " +
                "  COUNT (DISTINCT I_SUBMERCHANT_ID) AS customerCount " +
                " FROM " +
                "  im_RECEPTION_MAP  " +
                " WHERE " +
                "  I_STATISITC_TIME >= " +String.valueOf(startTime) +
                " AND I_STATISITC_TIME < " +String.valueOf(endTime) +
                " GROUP BY " +
                "  i_submerchant_id) pqb " +
                "LEFT JOIN ( " +
                " SELECT " +
                "  i_submerchant_id, " +
                "  COUNT (DISTINCT I_COUSTOMER_ID) AS askcount " +
                " FROM " +
                "  im_RECEPTION_MAP " +
                " WHERE " +
                "  I_STATISITC_TIME >= " +String.valueOf(startTime) +
                " AND I_STATISITC_TIME < " +String.valueOf(endTime) +
                " AND (I_REPLY <>0  OR I_NOT_REPLY<>0) " +
                " GROUP BY " +
                "  i_submerchant_id " +
                ") pqa  " +
                "ON pqb.i_submerchant_id = pqa.i_submerchant_id " +
                "LEFT JOIN ( " +
                " SELECT " +
                "  i_submerchant_id, " +
                "  COUNT (DISTINCT I_COUSTOMER_ID) AS replycount " +
                " FROM " +
                "  im_RECEPTION_MAP " +
                " WHERE " +
                "  I_STATISITC_TIME >= " +String.valueOf(startTime) +
                " AND I_STATISITC_TIME < " +String.valueOf(endTime) +
                " AND I_REPLY <>0   " +
                " GROUP BY " +
                "  i_submerchant_id " +
                ") pqc " +
                "ON pqb.i_submerchant_id = pqc.i_submerchant_id " +
                "LEFT JOIN ( " +
                " SELECT " +
                "  i_submerchant_id, " +
                "  COUNT (DISTINCT I_COUSTOMER_ID) AS notreplycount " +
                " FROM " +
                "  im_RECEPTION_MAP " +
                " WHERE " +
                "  I_STATISITC_TIME >= " +String.valueOf(startTime) +
                " AND I_STATISITC_TIME < " +String.valueOf(endTime) +
                " AND I_NOT_REPLY <> 0  " +
                " GROUP BY " +
                "  i_submerchant_id " +
                ") pqd " +
                "ON pqb.i_submerchant_id = pqd.i_submerchant_id";

        return  sql;
    }

    public  static String getSqlGetAskingCountDetail(long startTime ,long endTime){

        String sql ="SELECT pab.* ,(case when paa.askcount is NULL  THEN 0 ELSE paa.askcount END) AS ASKCOUNT,  " +
                " (case when pac.replycount is NULL  THEN 0 ELSE pac.replycount END) AS  replyCount ,  " +
                " ( CASE WHEN pad.notReplyCount IS NULL THEN 0 ELSE  pad.notReplyCount END) AS NOTREPLYCOUNT  FROM    " +
                "(SELECT  " +
                "  i_submerchant_id,I_STATISITC_TIME,  " +
                "  COUNT (DISTINCT I_SUBMERCHANT_ID) AS customerCount  " +
                " FROM  " +
                "  im_RECEPTION_MAP   " +
                " WHERE  " +
                "  I_STATISITC_TIME >= " +String.valueOf(startTime) +
                " AND I_STATISITC_TIME < " +String.valueOf(endTime) +
                " GROUP BY  " +
                "  i_submerchant_id ,I_STATISITC_TIME) pab  " +
                "LEFT JOIN (  " +
                " SELECT  " +
                "  i_submerchant_id,I_STATISITC_TIME,  " +
                "  COUNT (DISTINCT I_COUSTOMER_ID) AS askcount  " +
                " FROM  " +
                "  im_RECEPTION_MAP  " +
                " WHERE  " +
                "  I_STATISITC_TIME >= " +String.valueOf(startTime) +
                " AND I_STATISITC_TIME < " +String.valueOf(endTime) +
                " AND (I_REPLY <>0  OR I_NOT_REPLY<>0)  " +
                " GROUP BY  " +
                "  i_submerchant_id,I_STATISITC_TIME  " +
                ") paa   " +
                "ON pab.i_submerchant_id = paa.i_submerchant_id AND pab.I_STATISITC_TIME = paa.I_STATISITC_TIME  " +
                "LEFT JOIN (  " +
                " SELECT  " +
                "  i_submerchant_id,I_STATISITC_TIME,  " +
                "  COUNT (DISTINCT I_COUSTOMER_ID) AS replycount  " +
                " FROM  " +
                "  im_RECEPTION_MAP  " +
                " WHERE  " +
                "  I_STATISITC_TIME >= " +String.valueOf(startTime) +
                " AND I_STATISITC_TIME < " +String.valueOf(endTime) +
                " AND I_REPLY <>0    " +
                " GROUP BY  " +
                "  i_submerchant_id,I_STATISITC_TIME  " +
                ") pac  " +
                "ON pab.i_submerchant_id = pac.i_submerchant_id AND pab.I_STATISITC_TIME = pac.I_STATISITC_TIME  " +
                "LEFT JOIN (  " +
                " SELECT  " +
                "  i_submerchant_id,I_STATISITC_TIME,  " +
                "  COUNT (DISTINCT I_COUSTOMER_ID) AS notreplycount  " +
                " FROM  " +
                "  im_RECEPTION_MAP  " +
                " WHERE  " +
                "  I_STATISITC_TIME >= " +String.valueOf(startTime) +
                " AND I_STATISITC_TIME < " +String.valueOf(endTime) +
                " AND I_NOT_REPLY <> 0   " +
                " GROUP BY  " +
                "  i_submerchant_id,I_STATISITC_TIME  " +
                ") pad  " +
                "ON pab.i_submerchant_id = pad.i_submerchant_id AND pab.I_STATISITC_TIME = pad.I_STATISITC_TIME";
        return  sql;
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

    public static AskingInfo convertToAskingInfoShow(ProductData productData){
        AskingInfo askingInfo = new AskingInfo();
        askingInfo.setTitle(productData.getITNAME());//商品名称
        askingInfo.setGoodId(productData.getID_NO());//商品ID
        askingInfo.setAddGood(productData.getKIND());//加购,主商品
        askingInfo.setSellType(productData.getDS_TYPE());//出货者类型
        askingInfo.setSellCount(productData.getQTY());//数量
        askingInfo.setSellerName(productData.getDS_NAME());//出货者名称
        askingInfo.setChecked(false);
        return askingInfo;
    }


}
