package com.feiniu.subAccount.util;

import com.feiniu.subAccount.constants.SystemConfig;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.sql.Timestamp;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Created by Dell on 14-9-30.
 */
public class FormatUtils {
    private static final Logger logger = LoggerFactory.getLogger(JsonUtil.class);
    public static final SimpleDateFormat TIME_FORMAT = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    public static final SimpleDateFormat DATE_FORMAT = new SimpleDateFormat("yyyy-MM-dd");
    public static final SimpleDateFormat HTML_TIME_FORMAT = new SimpleDateFormat("yyyy年MM月dd日-HH:mm:ss");

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

    public static final String formatHTMLTime(Timestamp timestamp) {
        if (timestamp == null) {
            return "";
        }else {
            return HTML_TIME_FORMAT.format(timestamp).replace("-", "<br>");
        }
    }

    //将秒转为日期 1999-00-00
    public static String convertNumberToDate(long timeInNumber){
        Date date = new Date(timeInNumber *1000);
        SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd");
        return  format.format(date);
    }
    //将秒转为时间分钟 00:00
    public static String convertNumberToMinTime(int timeInNumber){
        int hour = timeInNumber / 3600;
        int min = (timeInNumber % 3600) /60;

        if (hour == 24 && min ==0){
            hour = 23;
            min = 59;
        }

        return  String.format("%02d:%02d",hour,min);
    }
    //将秒转为时间：00:00:00
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


    public  static String getSqlGetAskingCount(long startTime ,long endTime){

        String sql = "SELECT pqb.* ,(case when pqa.askcount is NULL  THEN 0 ELSE pqa.askcount END) AS ASKCOUNT, " +
                " (case when pqc.replycount is NULL  THEN 0 ELSE pqc.replycount END) AS  replyCount , " +
                " ( CASE WHEN pqd.notReplyCount IS NULL THEN 0 ELSE  pqd.notReplyCount END) AS NOTREPLYCOUNT ," +
                " ( CASE WHEN pqe.replyTimes IS NULL THEN 0 ELSE  pqe.replyTimes END) AS replyTimes ," +
                " ( CASE WHEN pqe.notRelpyTimes IS NULL THEN 0 ELSE  pqe.notRelpyTimes END) AS notreplyTimes " +
                " FROM   " +
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
                " ) pqd " +
                " ON pqb.i_submerchant_id = pqd.i_submerchant_id "+
                " LEFT JOIN ( " +
                " SELECT " +
                "  i_submerchant_id, " +
                "  SUM(I_REPLY) AS replyTimes,  " +
                "  SUM(I_NOT_REPLY) AS notRelpyTimes " +
                " FROM " +
                "  im_RECEPTION_MAP " +
                " WHERE " +
                "  I_STATISITC_TIME >= " +String.valueOf(startTime) +
                " AND I_STATISITC_TIME < " +String.valueOf(endTime) +
                " GROUP BY " +
                "  i_submerchant_id " +
                ") pqe " +
                "ON pqb.i_submerchant_id = pqe.i_submerchant_id";

        return  sql;
    }

    public  static String getSqlGetAskingCountDetail(long startTime ,long endTime){

        String sql ="SELECT pab.* ,(case when paa.askcount is NULL  THEN 0 ELSE paa.askcount END) AS ASKCOUNT,  " +
                " (case when pac.replycount is NULL  THEN 0 ELSE pac.replycount END) AS  replyCount ,  " +
                " ( CASE WHEN pad.notReplyCount IS NULL THEN 0 ELSE  pad.notReplyCount END) AS NOTREPLYCOUNT ," +
                " ( CASE WHEN pae.replyTimes IS NULL THEN 0 ELSE  pae.replyTimes END) AS replyTimes ," +
                " ( CASE WHEN pae.notRelpyTimes IS NULL THEN 0 ELSE  pae.notRelpyTimes END) AS notreplyTimes " +
                " FROM    " +
                " (SELECT  " +
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
                " ) pad  " +
                " ON pab.i_submerchant_id = pad.i_submerchant_id AND pab.I_STATISITC_TIME = pad.I_STATISITC_TIME "+
                " LEFT JOIN (  " +
                " SELECT  " +
                "  i_submerchant_id,I_STATISITC_TIME,  " +
                "  SUM(I_REPLY) AS replyTimes,  " +
                "  SUM(I_NOT_REPLY) AS notRelpyTimes " +
                " FROM  " +
                "  im_RECEPTION_MAP  " +
                " WHERE  " +
                "  I_STATISITC_TIME >= " +String.valueOf(startTime) +
                " AND I_STATISITC_TIME < " +String.valueOf(endTime) +
                " GROUP BY  " +
                "  i_submerchant_id,I_STATISITC_TIME  " +
                ") pae  " +
                "ON pab.i_submerchant_id = pae.i_submerchant_id AND pab.I_STATISITC_TIME = pae.I_STATISITC_TIME";

        return  sql;
    }

    public  static String getSqlGetAskingCountDetailTotal(long startTime ,long endTime,long subMerchantId){

        String sql = "SELECT " +
                "   (case when PA.i_submerchant_id is NULL then 0 ELSE PA.i_submerchant_id end), " +
                "  (case when PA.landingtime is NULL then 0 ELSE PA.landingtime end), " +
                "  (case when pb.askcount is NULL then 0 ELSE pb.askcount end), " +
                "  (case when pc.replyCount is NULL then 0 ELSE pc.replyCount end) "+
                " FROM " +
                "  ( " +
                "    SELECT " +
                "      i_submerchant_id, " +
                "      SUM (I_TOTAL_ONLINE_DURATION) AS landingtime " +
                "    FROM " +
                "      IM_SUBMERCHANT_ATTENDANCE " +
                "    WHERE " +
                "      i_submerchant_id =  " + String.valueOf(subMerchantId) +
                "    AND I_STATISTICS_TIME >= " + String.valueOf(startTime) +
                "    AND I_STATISTICS_TIME <  " + String.valueOf(endTime) +
                "    GROUP BY " +
                "      i_submerchant_id " +
                "  ) PA " +
                "LEFT JOIN ( " +
                "  SELECT " +
                "    i_submerchant_id, " +
                "    COUNT (DISTINCT I_COUSTOMER_ID) AS askcount " +
                "  FROM " +
                "    im_RECEPTION_MAP " +
                "  WHERE " +
                "      i_submerchant_id =  " + String.valueOf(subMerchantId) +
                "    AND I_STATISITC_TIME >= " + String.valueOf(startTime) +
                "    AND I_STATISITC_TIME <  " + String.valueOf(endTime) +
                "  AND (I_REPLY <> 0 OR I_NOT_REPLY <> 0) " +
                "  GROUP BY " +
                "    i_submerchant_id " +
                ") PB ON PB.i_submerchant_id = PA.i_submerchant_id " +
                "LEFT JOIN ( " +
                "  SELECT " +
                "    i_submerchant_id, " +
                "    COUNT (DISTINCT I_COUSTOMER_ID) AS replyCount " +
                "  FROM " +
                "    im_RECEPTION_MAP " +
                "  WHERE " +
                "      i_submerchant_id =  " + String.valueOf(subMerchantId) +
                "    AND I_STATISITC_TIME >= " + String.valueOf(startTime) +
                "    AND I_STATISITC_TIME <  " + String.valueOf(endTime) +
                "  AND (I_REPLY <> 0) " +
                "  GROUP BY " +
                "    i_submerchant_id " +
                ") PC ON PC.i_submerchant_id = PA.i_submerchant_id";

        return  sql;
    }

    public static String  getSqlGetAskingCountTotal(long startTime ,long endTime,long merchantId){

        String sql = "SELECT " +
                "   (case when PA.MERCHANT_ID is NULL then 0 ELSE PA.MERCHANT_ID end), " +
                "  (case when PA.LANDING_TIME is NULL then 0 ELSE PA.LANDING_TIME end), " +
                "  (case when pb.askcount is NULL then 0 ELSE pb.askcount end), " +
                "  (case when pc.replyCount is NULL then 0 ELSE pc.replyCount end) "+
                " FROM " +
                "  ( " +
                "             SELECT merchant_id,SUM(LANDING_TIME) AS LANDING_TIME "  +
                "             FROM IM_MERCHANT_LANDING_DETAIL "  +
                "             WHERE STATISTICS_DATE >= "+String.valueOf(startTime)+
                "              AND STATISTICS_DATE < "+String.valueOf(endTime)+
                "              AND merchant_id = "+ String.valueOf(merchantId)+
                "             GROUP BY merchant_ID "  +
                "             ORDER BY merchant_ID "  +
                "  ) PA " +
                "LEFT JOIN ( " +
                "  SELECT " +
                "    I_MERCHANT_ID, " +
                "    COUNT (DISTINCT I_COUSTOMER_ID) AS askcount " +
                "  FROM " +
                "    im_RECEPTION_MAP " +
                "  WHERE " +
                "      I_MERCHANT_ID =  " + String.valueOf(merchantId) +
                "    AND I_STATISITC_TIME >= " + String.valueOf(startTime) +
                "    AND I_STATISITC_TIME <  " + String.valueOf(endTime) +
                "  AND (I_REPLY <> 0 OR I_NOT_REPLY <> 0) " +
                "  GROUP BY " +
                "    i_merchant_id " +
                ") PB ON PB.I_MERCHANT_ID = PA.MERCHANT_ID " +
                "LEFT JOIN ( " +
                "  SELECT " +
                "    I_MERCHANT_ID, " +
                "    COUNT (DISTINCT I_COUSTOMER_ID) AS replyCount " +
                "  FROM " +
                "    im_RECEPTION_MAP " +
                "  WHERE " +
                "      I_MERCHANT_ID =  " + String.valueOf(merchantId) +
                "    AND I_STATISITC_TIME >= " + String.valueOf(startTime) +
                "    AND I_STATISITC_TIME <  " + String.valueOf(endTime) +
                "  AND (I_REPLY <> 0) " +
                "  GROUP BY " +
                "    i_merchant_id " +
                ") PC ON PC.I_MERCHANT_ID = PA.MERCHANT_ID";

        return  sql;
    }

    public static String formatPhoneStr(String phoneNumber){

        if(phoneNumber == null)
            return null;
        //移除空格
        String result = phoneNumber.replaceAll(" ", "");
        //如果是默认值返回null
        if (SystemConfig.EMPTY_PHONE.equals(result))
            return  null;

        return result;
    }
}
