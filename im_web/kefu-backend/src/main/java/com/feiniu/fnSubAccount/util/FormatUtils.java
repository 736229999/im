package com.feiniu.fnSubAccount.util;

import com.feiniu.fnSubAccount.base.ResponseResult;
import com.feiniu.fnSubAccount.bean.*;
import com.feiniu.fnSubAccount.bean.SessionInfo.SessionInfoItemShow;
import com.feiniu.fnSubAccount.bean.request.WorkMessageListRequest;
import com.feiniu.fnSubAccount.bean.response.WorkMessageResponse;
import com.feiniu.fnSubAccount.bean.response.WorkMessageTotalInfoResponse;
import com.feiniu.fnSubAccount.consts.SystemConfig;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.math.BigDecimal;
import java.math.RoundingMode;
import java.sql.Timestamp;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.*;

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
        满意度统计
    **/
    public static final void  formatServiceAppraiseDetailStat(ServiceAppraiseDetailStat serviceAppraiseDetailStat){
        serviceAppraiseDetailStat.setAppraiseStr(SystemConfig.appraiseStatus[serviceAppraiseDetailStat.getSqlAppraise()]);
        if (serviceAppraiseDetailStat.getSqlAppraiseTime() != 0)
            serviceAppraiseDetailStat.setAppraiseTimeStr(convertNumberToDateAndTime(serviceAppraiseDetailStat.getSqlAppraiseTime()));
        else
            serviceAppraiseDetailStat.setAppraiseTimeStr("--");

        if (serviceAppraiseDetailStat.getSqlSessionStartTime() != 0)
            serviceAppraiseDetailStat.setSessionStartTimeStr(convertNumberToDateAndTime(serviceAppraiseDetailStat.getSqlSessionStartTime()));
        else
            serviceAppraiseDetailStat.setSessionStartTimeStr("--");

        if (serviceAppraiseDetailStat.getSqlSessionEndTime() != 0)
            serviceAppraiseDetailStat.setSessionEndTimeStr(convertNumberToDateAndTime(serviceAppraiseDetailStat.getSqlSessionEndTime()));
        else
            serviceAppraiseDetailStat.setSessionEndTimeStr("--");

        String comment = serviceAppraiseDetailStat.getComment();
        if (comment != null){
            int index = comment.lastIndexOf('：');
            comment = comment.substring(index+1);
        }else {
            comment = "--";
        }
        serviceAppraiseDetailStat.setComment(comment);
    }

    public static final void convertServiceAppraiseStatToAppraiseStatMap(Map<Long ,AppraiseStat> idToAppraiseStatMap,
                                                                         List<ServiceAppraiseStat> result,
                                                                         List<Long> idList){
        for (Long index:idList){
            AppraiseStat tmp = new AppraiseStat(index);

            if (!index.equals( Long.valueOf(-1)) )
                idToAppraiseStatMap.put(index,tmp);
        }

        for (ServiceAppraiseStat index : result){
            AppraiseStat tmp = idToAppraiseStatMap.get(Long.valueOf(index.getSubAccountID()));
            if (tmp == null)
                continue;
            tmp.getAppraiseCount()[index.getAppraise()] = String.valueOf(index.getCounter());
        }

    }

    public static final String formatAppraiseRate(String[] appraiseDetail,int startIndex){
        String result ;
        int totalAppraise = Integer.valueOf(appraiseDetail[startIndex+1]) +
                Integer.valueOf(appraiseDetail[startIndex+2]) +
                Integer.valueOf(appraiseDetail[startIndex+3]) +
                Integer.valueOf(appraiseDetail[startIndex+4]) +
                Integer.valueOf(appraiseDetail[startIndex+5]) ;

        int goodAppraise = Integer.valueOf(appraiseDetail[startIndex+4]) +
                Integer.valueOf(appraiseDetail[startIndex+5]) ;

        if (totalAppraise == 0)
            result ="--";
        else {
            BigDecimal goodAppraiseBig = new BigDecimal( goodAppraise);
            BigDecimal totalAppraiseBig = new BigDecimal( totalAppraise);
            BigDecimal rate = goodAppraiseBig.divide(totalAppraiseBig,SystemConfig.DEFAULT_SCALE+3,RoundingMode.HALF_EVEN).
                    multiply(new BigDecimal( 100)).setScale(SystemConfig.DEFAULT_SCALE, RoundingMode.HALF_EVEN);
            result = rate.toString()+ "%";
        }
        return  result;
    }

    public static final void formatAppraiseStatToExcel(String[][] result,int startIndex,AppraiseStat serviceAppraiseDetailStat){
        result[startIndex][0] =serviceAppraiseDetailStat.getSubMerchantName();
        result[startIndex][1] =serviceAppraiseDetailStat.getAppraiseCount()[0];
        result[startIndex][2] =serviceAppraiseDetailStat.getAppraiseCount()[5];
        result[startIndex][3] =serviceAppraiseDetailStat.getAppraiseCount()[4];
        result[startIndex][4] =serviceAppraiseDetailStat.getAppraiseCount()[3];
        result[startIndex][5] =serviceAppraiseDetailStat.getAppraiseCount()[2];
        result[startIndex][6] =serviceAppraiseDetailStat.getAppraiseCount()[1];
        result[startIndex][7] =serviceAppraiseDetailStat.getGoodRate();
    }

    public static final void formatServiceAppraiseDetailStatToExcel(String[][] result,int startIndex,ServiceAppraiseDetailStat serviceAppraiseDetailStat){
        result[startIndex][0] =serviceAppraiseDetailStat.getSubMerchantName();
        result[startIndex][1] =serviceAppraiseDetailStat.getBuyerName();
        result[startIndex][2] =serviceAppraiseDetailStat.getAppraiseStr();
        result[startIndex][3] =serviceAppraiseDetailStat.getComment();
        result[startIndex][4] =serviceAppraiseDetailStat.getAppraiseTimeStr();
        result[startIndex][5] =serviceAppraiseDetailStat.getSessionStartTimeStr();
        result[startIndex][6] =serviceAppraiseDetailStat.getSessionEndTimeStr();
    }

    public static final void formatSessionInfoItemShowToExcel(String[][] result,int startIndex,SessionInfoItemShow sessionInfoItemShow){
        result[startIndex][0] =sessionInfoItemShow.getSessionStartTimeStr();
        result[startIndex][1] =sessionInfoItemShow.getSessionEndTimeStr();
        result[startIndex][2] =sessionInfoItemShow.getCustomerNameStr();
        result[startIndex][3] =sessionInfoItemShow.getServiceNameStr();
        result[startIndex][4] =sessionInfoItemShow.getSessionLengthStr();
        result[startIndex][5] =sessionInfoItemShow.getCustomerWaitingTimeStr();
        result[startIndex][6] =sessionInfoItemShow.getMobilePortStr();
        result[startIndex][7] =sessionInfoItemShow.getPcPortStr();
    }

    /*
     * 工作量统计
     */
    public static final void  formatWorkMessageResponse(WorkMessageResponse workMessageResponse){
        workMessageResponse.setMerchantIDStr(new BigDecimal(workMessageResponse.getMerchantID()).toString());
        workMessageResponse.setSubMerchantOnlineCount(new BigDecimal(workMessageResponse.getSqlSubMerchantOnlineCount()).setScale(SystemConfig.DEFAULT_SCALE).toString());
        workMessageResponse.setSumLandingTime(new BigDecimal(workMessageResponse.getSqlSumLandingTime() / SystemConfig.SECONDS_IN_HOUR).setScale(SystemConfig.DEFAULT_SCALE, BigDecimal.ROUND_HALF_UP).toString());
        workMessageResponse.setAskingCount(new BigDecimal(workMessageResponse.getSqlAskingCount()).setScale(SystemConfig.DEFAULT_SCALE).toString());
        workMessageResponse.setReplyCount(new BigDecimal(workMessageResponse.getSqlReplyCount()).setScale(SystemConfig.DEFAULT_SCALE).toString());
        workMessageResponse.setAskingTimes(new BigDecimal(workMessageResponse.getSqlAskingTimes()).setScale(SystemConfig.DEFAULT_SCALE).toString());
        workMessageResponse.setReplyTimes(new BigDecimal(workMessageResponse.getSqlReplyTimes()).setScale(SystemConfig.DEFAULT_SCALE).toString());
       if(workMessageResponse.getSqlAskingTimes()==0){
           workMessageResponse.setReplyFrequency("--");
       }else{
           Float replyTime  =workMessageResponse.getSqlReplyTimes();
           Float askingTime = workMessageResponse.getSqlAskingTimes();
           Float rate = replyTime*100/askingTime;
           workMessageResponse.setReplyFrequency((new BigDecimal(rate).setScale(SystemConfig.DEFAULT_SCALE, BigDecimal.ROUND_HALF_UP) + "%"));
       }

        if ( !(new  BigDecimal(workMessageResponse.getSqlAvgResponseDuration()).equals(new  BigDecimal(-1.0) )))
            workMessageResponse.setAvgResponseDuration(new  BigDecimal(workMessageResponse.getSqlAvgResponseDuration()).setScale(SystemConfig.DEFAULT_SCALE, RoundingMode.HALF_UP).toString());
        else
            workMessageResponse.setAvgResponseDuration("-");

        if ( !(new  BigDecimal(workMessageResponse.getSqlResponseIn90sRate()).equals(new  BigDecimal(-1.0))))
            workMessageResponse.setResponseIn90sRate(new BigDecimal(workMessageResponse.getSqlResponseIn90sRate()).multiply(new BigDecimal(100)).setScale(SystemConfig.DEFAULT_SCALE, RoundingMode.HALF_UP).toString() + "%");
        else
            workMessageResponse.setResponseIn90sRate("-");
    }

    public static final void  formatTotalWorkMessageResponse(WorkMessageResponse workMessageResponse){
        workMessageResponse.setMerchantName("汇总");
        workMessageResponse.setMerchantIDStr("--");
        workMessageResponse.setMerchantStatus("--");
        workMessageResponse.setSubMerchantOnlineCount(new BigDecimal(workMessageResponse.getSqlSubMerchantOnlineCount()).setScale(SystemConfig.DEFAULT_SCALE).toString());
        workMessageResponse.setShopName("--");
        workMessageResponse.setMainCategoryName("--");
        workMessageResponse.setSumLandingTime(new BigDecimal(workMessageResponse.getSqlSumLandingTime() / SystemConfig.SECONDS_IN_HOUR).setScale(SystemConfig.DEFAULT_SCALE, BigDecimal.ROUND_HALF_UP).toString());
        workMessageResponse.setAskingCount(new BigDecimal(workMessageResponse.getSqlAskingCount()).setScale(SystemConfig.DEFAULT_SCALE).toString());
        workMessageResponse.setReplyCount(new BigDecimal(workMessageResponse.getSqlReplyCount()).setScale(SystemConfig.DEFAULT_SCALE).toString());
        workMessageResponse.setAskingTimes(new BigDecimal(workMessageResponse.getSqlAskingTimes()).setScale(SystemConfig.DEFAULT_SCALE).toString());
        workMessageResponse.setReplyTimes(new BigDecimal(workMessageResponse.getSqlReplyTimes()).setScale(SystemConfig.DEFAULT_SCALE).toString());
       if(workMessageResponse.getSqlAskingTimes()==0){
           workMessageResponse.setReplyFrequency("--");
       }else {
           Float replyTime  =workMessageResponse.getSqlReplyTimes();
           Float askingTime = workMessageResponse.getSqlAskingTimes();
           Float rate = replyTime*100/askingTime;
           workMessageResponse.setReplyFrequency((new BigDecimal(rate).setScale(SystemConfig.DEFAULT_SCALE, BigDecimal.ROUND_HALF_UP) + "%"));
       }

        workMessageResponse.setAvgResponseDuration("--");
        workMessageResponse.setResponseIn90sRate("--");
    }

    public static final void  formatAvgWorkMessageResponse(WorkMessageResponse workMessageResponse){
        workMessageResponse.setMerchantName("均值");
        workMessageResponse.setMerchantIDStr("--");
        workMessageResponse.setMerchantStatus("--");
        workMessageResponse.setSubMerchantOnlineCount(new BigDecimal(workMessageResponse.getSqlSubMerchantOnlineCount()).setScale(SystemConfig.DEFAULT_SCALE, BigDecimal.ROUND_HALF_UP).toString());
        workMessageResponse.setShopName("--");
        workMessageResponse.setMainCategoryName("--");
        workMessageResponse.setSumLandingTime(new BigDecimal(workMessageResponse.getSqlSumLandingTime() / SystemConfig.SECONDS_IN_HOUR).setScale(SystemConfig.DEFAULT_SCALE, BigDecimal.ROUND_HALF_UP).toString());
        workMessageResponse.setAskingCount(new BigDecimal(workMessageResponse.getSqlAskingCount()).setScale(SystemConfig.DEFAULT_SCALE, BigDecimal.ROUND_HALF_UP).toString());
        workMessageResponse.setReplyCount(new BigDecimal(workMessageResponse.getSqlReplyCount()).setScale(SystemConfig.DEFAULT_SCALE, BigDecimal.ROUND_HALF_UP).toString());
        workMessageResponse.setAskingTimes(new BigDecimal(workMessageResponse.getSqlAskingTimes()).setScale(SystemConfig.DEFAULT_SCALE, BigDecimal.ROUND_HALF_UP).toString());
        workMessageResponse.setReplyTimes(new BigDecimal(workMessageResponse.getSqlReplyTimes()).setScale(SystemConfig.DEFAULT_SCALE, BigDecimal.ROUND_HALF_UP).toString());
        workMessageResponse.setReplyFrequency("--");


        if (! new  BigDecimal(workMessageResponse.getSqlAvgResponseDuration()).equals(new BigDecimal(-1) ))
            workMessageResponse.setAvgResponseDuration(new  BigDecimal(workMessageResponse.getSqlAvgResponseDuration()).setScale(SystemConfig.DEFAULT_SCALE,BigDecimal.ROUND_HALF_UP).toString());
        else
            workMessageResponse.setAvgResponseDuration("-");

        if (! new  BigDecimal(workMessageResponse.getSqlResponseIn90sRate()).equals(new BigDecimal(-1) ))
            workMessageResponse.setResponseIn90sRate(new BigDecimal(workMessageResponse.getSqlResponseIn90sRate()).multiply(new BigDecimal(100)).setScale(SystemConfig.DEFAULT_SCALE, BigDecimal.ROUND_HALF_UP).toString() + "%");
        else
            workMessageResponse.setResponseIn90sRate("-");
    }

    public static final Map<Long,Merchant> merchantListToMap(List<Merchant> merchantInfoList){
        Map<Long,Merchant> idToMap = new HashMap<>();

        if (merchantInfoList == null)
            return idToMap;

        for (Merchant index: merchantInfoList){
            idToMap.put(index.getId(),index);
        }
        return idToMap;
    }

    public static final void  formatWorkMessageResponse(WorkMessageResponse index,Merchant indexMerchant,Integer status){
        if (index == null) return;

        if (indexMerchant != null){
            index.setMerchantName(indexMerchant.getName());
            index.setShopName(indexMerchant.getShopName());
            index.setMainCategoryName(indexMerchant.getMainCategoryName());
        }else {
            index.setMerchantName(" ");
            index.setShopName(" ");
            index.setMainCategoryName("  ");
        }

        if (status != null && (status == 0 || status == 1)){
            index.setMerchantStatus(SystemConfig.statusMap[status]);
        }else {
            index.setMerchantStatus("--");
        }

    }
    public static final void  formatWorkMessageResponseListWithMap(List<WorkMessageResponse> workMessageResponseList,
                                                                   Map<Long,Merchant>  idToMerchantInfo ,
                                                                   Map<Integer,Integer> idTostatusMap){
        for (WorkMessageResponse index : workMessageResponseList){
            Merchant indexMerchant = idToMerchantInfo.get(index.getMerchantID());
            Integer status = idTostatusMap.get(Integer.parseInt(String.valueOf(index.getMerchantID())));
            formatWorkMessageResponse(index,indexMerchant,status);
        }
    }

    public static final void  formatTotalInfo(WorkMessageTotalInfoResponse totalInfo,
                                              WorkMessageResponse totalWorkMessageResponse,
                                              WorkMessageResponse avgWworkMessageResponse
                                              )
    {
        totalWorkMessageResponse.setSqlSubMerchantOnlineCount(totalInfo.getSumOnlineCustomerCount());
        totalWorkMessageResponse.setSqlSumLandingTime(totalInfo.getSumLandingDuration());
        totalWorkMessageResponse.setSqlAskingCount(totalInfo.getSumAskingCount());
        totalWorkMessageResponse.setSqlReplyCount(totalInfo.getSumReplyCount());
        totalWorkMessageResponse.setSqlAskingTimes(totalInfo.getSumAskingTimes());
        totalWorkMessageResponse.setSqlReplyTimes(totalInfo.getSumReplyTimes());


        avgWworkMessageResponse.setSqlSubMerchantOnlineCount(totalInfo.getAvgOnlineCustomerCount());
        avgWworkMessageResponse.setSqlSumLandingTime(totalInfo.getAvgLandingDuration());
        avgWworkMessageResponse.setSqlAskingCount(totalInfo.getAvgAskingCount());
        avgWworkMessageResponse.setSqlReplyCount(totalInfo.getAvgReplyCount());
        avgWworkMessageResponse.setSqlAvgResponseDuration(totalInfo.getAvgResponseDuration());
        avgWworkMessageResponse.setSqlResponseIn90sRate(totalInfo.getAvgRate());
        avgWworkMessageResponse.setSqlAskingTimes(totalInfo.getAvgAskingTimes());
        avgWworkMessageResponse.setSqlReplyTimes(totalInfo.getAvgReplyTimes());


        FormatUtils.formatTotalWorkMessageResponse(totalWorkMessageResponse);
        FormatUtils.formatAvgWorkMessageResponse(avgWworkMessageResponse);
    }


    public static final void convertRequestToParams(WorkMessageListRequest workMessageListRequest , WorkMessageRequestParams params){
        params.setStartTime(workMessageListRequest.getStartTime());
        params.setEndTime(workMessageListRequest.getEndTime());
        params.setStartLandingDur(workMessageListRequest.getStartOnlineTime());
        params.setEndLandingDur(workMessageListRequest.getEndOnlineTime());
        params.setStartRate(workMessageListRequest.getStartResponseIn90sRate());
        params.setEndRate(workMessageListRequest.getEndResponseIn90sRate());
        params.setPageSize(workMessageListRequest.getPageSize());
        params.setCurrentPageNo(workMessageListRequest.getCurrentPageNo());
        if (workMessageListRequest.getCurrentPageNo() == 1){
            params.setStartCount(0);
        }else {
            params.setStartCount((workMessageListRequest.getCurrentPageNo() -1 )*workMessageListRequest.getPageSize()-2);
        }
        params.setEndCount(workMessageListRequest.getPageSize()*workMessageListRequest.getCurrentPageNo() -2);
    }


    public static final void convertRequestToParams(CSAccountRequest csAccountRequest , CSAccountRequestParams params){
        params.setCSName(csAccountRequest.getCSName());
        params.setCSTeam(csAccountRequest.getCSTeam());
        params.setCSUserName(csAccountRequest.getCSUserName());
        params.setCSAccountAuthor(csAccountRequest.getCSAccountAuthor());
        params.setAccountLimits(csAccountRequest.getAccountLimits());
        params.setImportUser(csAccountRequest.getImportUser());
        params.setImportTeam(csAccountRequest.getImportTeam());
        params.setAccountStates(csAccountRequest.getAccountStates());
        params.setCSPassword(csAccountRequest.getCSPassword());

        params.setPageSize(csAccountRequest.getPageSize());
        params.setCurrentPageNo(csAccountRequest.getCurrentPageNo());
        if (csAccountRequest.getCurrentPageNo() == 1){
            params.setStartCount(0);
        }else {
            params.setStartCount((csAccountRequest.getCurrentPageNo() -1 )*csAccountRequest.getPageSize()-2);
        }
        params.setEndCount(csAccountRequest.getPageSize()*csAccountRequest.getCurrentPageNo() -2);
    }
    public static final ResponseResult formatErrorResponse(ResponseResult responseResult,long checkResult){

        int checkReultInt = Integer.parseInt(String.valueOf(checkResult));

        switch (checkReultInt){
            case -2:
                responseResult.setMessage("查询的条件不匹配，请检查后重新输入");
                return responseResult;
            case -3:
                responseResult.setErrorCode(-2);
                responseResult.setMessage("没有符合的统计数据");
                return responseResult;
            case -4:
                responseResult.setMessage("输入的数据不匹配");
                return responseResult;
        }

        return responseResult;
    }

    public static final void  formatWorkMessageResponseToArray(String[][] result,int startIndex,WorkMessageResponse indexWorkMessage){
        result[startIndex][0] =indexWorkMessage.getMerchantName();
        result[startIndex][1] =indexWorkMessage.getMerchantIDStr();
        result[startIndex][2] =indexWorkMessage.getSubMerchantOnlineCount();
        result[startIndex][3] =indexWorkMessage.getShopName();
        result[startIndex][4] =indexWorkMessage.getMainCategoryName();
        result[startIndex][5] =indexWorkMessage.getSumLandingTime();
        result[startIndex][6] =indexWorkMessage.getAskingCount();
        result[startIndex][7] =indexWorkMessage.getReplyCount();
        result[startIndex][8] =indexWorkMessage.getAskingTimes();
        result[startIndex][9] =indexWorkMessage.getReplyTimes();
        result[startIndex][10] =indexWorkMessage.getReplyFrequency();
        result[startIndex][11] =indexWorkMessage.getAvgResponseDuration();
        result[startIndex][12] =indexWorkMessage.getResponseIn90sRate();
        result[startIndex][13] =indexWorkMessage.getMerchantStatus();
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

        return  String.format("%02d:%02d",hour%24,min);
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
        return  String.format("%02d:%02d:%02d",hour%24,min,sec);
    }

    //00时00分00秒
    public static String formatNumberToSecTime(int timeInNumber){
        int hour = timeInNumber / 3600;
        int min = (timeInNumber % 3600) /60;
        int sec = timeInNumber - hour * 3600 - min * 60;

        if (hour == 24 && min ==0 && sec ==0){
            hour = 23;
            min = 59;
            sec = 59;
        }
        if (hour != 0)
            return  String.format("%02d时%02d分%02d秒",hour,min,sec);
        else if (hour ==0 && min!=0)
            return  String.format("%02d分%02d秒",min,sec);
        else
            return String.format("%02d秒",sec);
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

    /*
    * SQL 语句相关
    */

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

    public static final  String  getWorkMessageSql(WorkMessageRequestParams workMessageRequestParams,boolean isNeedPage,long isNeedMerchantId){
        String sqlRate="";
        String sqlLanding="";
        String sqlPage="";
        String sqlMerchantId=" AND I_MERCHANT_ID <> -1 ";
        String sql;
        BigDecimal startRateBig = new BigDecimal(workMessageRequestParams.getStartRate());
        BigDecimal endRateBig = new BigDecimal(workMessageRequestParams.getEndRate());
        BigDecimal startLandingDurBig = new BigDecimal(workMessageRequestParams.getStartLandingDur() );
        BigDecimal endLandingDurBig = new BigDecimal(workMessageRequestParams.getEndLandingDur() );

        if (startLandingDurBig.compareTo(new BigDecimal(-1)) != 0 && endLandingDurBig.compareTo(new BigDecimal(-1))!= 0){
            sqlLanding = "         WHERE (B.LANDING_TIME >= " +startLandingDurBig.multiply(new BigDecimal(SystemConfig.SECONDS_IN_HOUR)).toString()+
                    " AND (b.LANDING_TIME <=  " +endLandingDurBig.multiply(new BigDecimal(SystemConfig.SECONDS_IN_HOUR)).toString()+
                    "  OR b.LANDING_TIME IS NULL ) "+
                    " ) " ;
        }else  if (startLandingDurBig.compareTo(new BigDecimal(-1)) != 0){
            sqlLanding = "         WHERE (B.LANDING_TIME >= " +startLandingDurBig.multiply(new BigDecimal(SystemConfig.SECONDS_IN_HOUR)).toString()+ ")";
        }else if (endLandingDurBig.compareTo(new BigDecimal(-1)) != 0){
            sqlLanding = "         WHERE (B.LANDING_TIME <=  " +endLandingDurBig.multiply(new BigDecimal(SystemConfig.SECONDS_IN_HOUR)).toString()+ "OR b.LANDING_TIME IS NULL )";
        } else {
            sqlLanding = " WHERE (1=1) ";
        }

        if (startRateBig.compareTo(new BigDecimal(-1)) != 0 && endRateBig.compareTo(new BigDecimal(-1))!= 0){
            sqlRate = "         AND (a.rate >= "+startRateBig.divide(new BigDecimal(100),SystemConfig.DEFAULT_SCALE_COUNT,RoundingMode.HALF_UP).toString()+
                    " AND ( a.rate <=" + endRateBig.divide(new BigDecimal(100),SystemConfig.DEFAULT_SCALE_COUNT,RoundingMode.HALF_UP).toString()+
                    "  OR a.rate IS NULL ) "+
                    " )";
        }else  if (startRateBig.compareTo(new BigDecimal(-1)) != 0){
            sqlRate = "      AND (a.rate >= " +startRateBig.divide(new BigDecimal(100),SystemConfig.DEFAULT_SCALE_COUNT,RoundingMode.HALF_UP).toString()+ ")";
        }else if (endRateBig.compareTo(new BigDecimal(-1)) != 0){
            sqlRate = "        AND ( a.rate <= " +endRateBig.divide(new BigDecimal(100),SystemConfig.DEFAULT_SCALE_COUNT,RoundingMode.HALF_UP).toString()+ " OR a.rate IS NULL )";
        } else {
            sqlRate = "  ";
        }

        if (isNeedPage)
            sqlPage = "   WHERE rn > "+ String.valueOf(workMessageRequestParams.getStartCount()) +" AND  rn <=  " + String.valueOf(workMessageRequestParams.getEndCount()) ;

        if (0 != isNeedMerchantId)
            sqlMerchantId += " AND I_MERCHANT_ID = " + String.valueOf(isNeedMerchantId) +"  ";

        sql =   "  SELECT PA.i_merchant_id," +
                " (CASE WHEN PA.responseCount = 0 THEN NULL ELSE PA.responseDur/ PA.responseCount END) AS RESPONSE_DUR," +
                " (case when PA.landing_time IS NULL THEN 0 ELSE PA.landing_time END) as landing_time,"  +
                " PA.RATE,"  +
                " (case when PB.customerCount IS NULL THEN 0 ELSE PB.customerCount END) as customerCount,"  +
                " (case when PB.ASKCOUNT IS NULL THEN 0 ELSE PB.ASKCOUNT END) as ASKCOUNT,"  +
                " (case when PB.replyCount IS NULL THEN 0 ELSE PB.replyCount END) as replyCount,"  +
                " (case when PB.replyTimes IS NULL THEN 0 ELSE PB.replyTimes + PB.notRelpyTimes END ) AS askingTimes,"  +
                " (case when PB.replyTimes IS NULL THEN 0 ELSE PB.replyTimes END ) AS replyTimes"  +
                "  FROM("  +
                " SELECT pdd.*"  +
                " FROM ("  +
                " SELECT  ROWNUM rn , a.*, (CASE WHEN b.LANDING_TIME IS NULL THEN 0 ELSE b.LANDING_TIME END) AS LANDING_TIME  FROM"  +
                " ("  +
                " SELECT I_MERCHANT_ID, "  +
                " SUM(I_RESPONSE_COUNT) AS responseCount, "  +
                " SUM (I_RESPONSE_DURATION) AS responseDur, "  +
                " SUM (I_RESPONSE_IN_90S_COUNT) AS responseIn90s, "  +
                " (CASE WHEN SUM (I_RESPONSE_COUNT)=0 THEN NULL ELSE SUM (I_RESPONSE_IN_90S_COUNT)  /SUM (I_RESPONSE_COUNT) END ) AS rate "  +
                "             FROM IM_SUBMERCHANT_WORKLOAD "  +
                "             WHERE I_STATISTICS_TIME >= "+String.valueOf(workMessageRequestParams.getStartTime())+
                "              AND I_STATISTICS_TIME < "+String.valueOf(workMessageRequestParams.getEndTime())+
                sqlMerchantId +
                "             GROUP BY I_MERCHANT_ID "  +
                "             ORDER BY I_MERCHANT_ID "  +
                "         )a  "  +
                "         LEFT JOIN ( "  +
                "             SELECT merchant_id,SUM(LANDING_TIME) AS LANDING_TIME "  +
                "             FROM IM_MERCHANT_LANDING_DETAIL "  +
                "             WHERE STATISTICS_DATE >= "+String.valueOf(workMessageRequestParams.getStartTime())+
                "              AND STATISTICS_DATE < "+String.valueOf(workMessageRequestParams.getEndTime())+
                "             GROUP BY merchant_ID "  +
                "             ORDER BY merchant_ID "  +
                "         ) b  "  +
                "         ON a.I_MERCHANT_ID = b.MERCHANT_ID "  +
                sqlLanding  + sqlRate +
                "         ) pdd " + sqlPage+
                " ) pa "  +
                " LEFT JOIN ( "  +
                " SELECT pab.* ,(case when paa.askcount is NULL  THEN 0 ELSE paa.askcount END) AS ASKCOUNT,  " +
                " (case when pac.replycount is NULL  THEN 0 ELSE pac.replycount END) AS  replyCount , " +
                " (case when pad.replyTimes is NULL  THEN 0 ELSE pad.replyTimes END) AS  replyTimes, " +
                " (case when pad.notRelpyTimes is NULL  THEN 0 ELSE pad.notRelpyTimes END) AS  notRelpyTimes " +
                " FROM    " +
                "(SELECT  " +
                "  i_merchant_id,  " +
                "  COUNT (DISTINCT I_SUBMERCHANT_ID) AS customerCount  " +
                " FROM  " +
                "  IM_SUBMERCHANT_ATTENDANCE   " +
                " WHERE  " +
                "  I_STATISTICS_TIME >=  " +String.valueOf(workMessageRequestParams.getStartTime())+
                " AND I_STATISTICS_TIME <   "+ String.valueOf(workMessageRequestParams.getEndTime())+
                " GROUP BY  " +
                "  i_merchant_id) pab  " +
                " LEFT JOIN (  " +
                " SELECT  " +
                "  i_merchant_id,  " +
                "  COUNT (DISTINCT I_COUSTOMER_ID) AS askcount  " +
                " FROM  " +
                "  im_RECEPTION_MAP  " +
                " WHERE  " +
                "  I_STATISITC_TIME >=  " +String.valueOf(workMessageRequestParams.getStartTime())+
                " AND I_STATISITC_TIME <   "+ String.valueOf(workMessageRequestParams.getEndTime())+
                " AND (I_REPLY <>0  OR I_NOT_REPLY<>0)  " +
                " GROUP BY  " +
                "  i_merchant_id  " +
                ") paa   " +
                "ON pab.i_merchant_id = paa.i_merchant_id  " +
                "LEFT JOIN (  " +
                " SELECT  " +
                "  i_merchant_id,  " +
                "  COUNT (DISTINCT I_COUSTOMER_ID) AS replycount  " +
                " FROM  " +
                "  im_RECEPTION_MAP  " +
                " WHERE  " +
                "  I_STATISITC_TIME >=  " +String.valueOf(workMessageRequestParams.getStartTime())+
                " AND I_STATISITC_TIME <   "+ String.valueOf(workMessageRequestParams.getEndTime())+
                " AND I_REPLY <>0    " +
                " GROUP BY  " +
                "  i_merchant_id  " +
                " ) pac  " +
                " ON pab.i_merchant_id = pac.i_merchant_id "+
                "LEFT JOIN (  " +
                " SELECT  " +
                "  i_merchant_id,  " +
                "  SUM(I_REPLY) AS replyTimes,  " +
                "  SUM(I_NOT_REPLY) AS notRelpyTimes " +
                " FROM  " +
                "  im_RECEPTION_MAP  " +
                " WHERE  " +
                "  I_STATISITC_TIME >=  " +String.valueOf(workMessageRequestParams.getStartTime())+
                " AND I_STATISITC_TIME <   "+ String.valueOf(workMessageRequestParams.getEndTime())+
                " GROUP BY  " +
                "  i_merchant_id  " +
                " ) pad  " +
                " ON pab.i_merchant_id = pad.i_merchant_id "+
                " ) PB "  +
                " ON PA.I_MERCHANT_ID = PB.I_MERCHANT_ID " ;

        return  sql;
    }



    //获取所有子商家的信息map
    public  static Map<Long, SubMerchantInfo> convertSubMerchantInfListToMap(List<SubMerchantInfo> subMerchantInfoList) {

        Map<Long, SubMerchantInfo> subMerchantInfoMap = new HashMap<Long, SubMerchantInfo>();

        for (SubMerchantInfo subMerchantInfoindex : subMerchantInfoList) {
            subMerchantInfoMap.put(subMerchantInfoindex.getMemberId(), subMerchantInfoindex);
        }
        return subMerchantInfoMap;
    }

    public  static String convertSubMerchantInfListToIdList(Map<Long, SubMerchantInfo> subMerchantInfoMap){
        String result = "";
        for (Map.Entry<Long, SubMerchantInfo> entry : subMerchantInfoMap.entrySet()) {
            result +=entry.getKey().toString() + ",";
        }
        return  result.substring(0,result.length()-1);
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
}
