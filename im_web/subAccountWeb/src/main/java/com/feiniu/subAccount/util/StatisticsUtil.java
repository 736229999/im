package com.feiniu.subAccount.util;

import com.feiniu.subAccount.bean.response.AttendanceDetailInfo;
import com.feiniu.subAccount.bean.response.AttendanceInfo;
import com.feiniu.subAccount.bean.response.BaseWorkLoadInfo;
import com.feiniu.subAccount.consts.SystemConfig;

import java.math.BigDecimal;
import java.util.*;

/**
 * Created by wangzhen on 2015/8/5.
 */
public class StatisticsUtil {
    //过滤一页的数据返回给用户
    public static List<Object> getOnePageInfo(List<Object> objectList,int startIndex,int endIndex){
        List<Object> resultobjectInfo = new ArrayList<>();

        if (objectList.size() < startIndex) return null;

        while (startIndex < endIndex && startIndex < objectList.size()) {
            resultobjectInfo.add(objectList.get(startIndex));
            startIndex++;
        }
        return  resultobjectInfo;
    }
    //将查询的出勤率的数据转换为界面上要显示的数据
    public static void formateAttendanceInfoToShowData(AttendanceDetailInfo attendanceDetailInfoIndex){
        attendanceDetailInfoIndex.setDate(FormatUtils.convertNumberToDate(attendanceDetailInfoIndex.getSqlAttendanceDate()));
        BigDecimal landingCount = new BigDecimal(attendanceDetailInfoIndex.getLandingCount());
        if (landingCount.intValue() != 0){
            BigDecimal firstLandingTime = new BigDecimal(attendanceDetailInfoIndex.getSqlAttendanceFirstOnlineTime());
            BigDecimal lastOnlineTime = new BigDecimal(attendanceDetailInfoIndex.getSqlAttendanceLastOnlineTime());
            BigDecimal landingTime = new BigDecimal(attendanceDetailInfoIndex.getSqlAttendanceLandingTimeSum());
            BigDecimal hungUpTime = new BigDecimal(attendanceDetailInfoIndex.getSqlAttendanceHungupTimeSum());
            BigDecimal offlineTime = new BigDecimal(attendanceDetailInfoIndex.getSqlAttendanceOfflineTimeSum());

            BigDecimal onlineRate = landingTime.subtract(hungUpTime).divide(lastOnlineTime.subtract(firstLandingTime)
                    ,3,BigDecimal.ROUND_HALF_EVEN);
            onlineRate = onlineRate.multiply(new BigDecimal(100));

            landingTime = landingTime.divide(new BigDecimal(3600), SystemConfig.DEFAULT_SCALE, BigDecimal.ROUND_HALF_EVEN);
            hungUpTime = hungUpTime.divide(new BigDecimal(3600), SystemConfig.DEFAULT_SCALE, BigDecimal.ROUND_HALF_EVEN);
            offlineTime = offlineTime.divide(new BigDecimal(3600), SystemConfig.DEFAULT_SCALE, BigDecimal.ROUND_HALF_EVEN);

            attendanceDetailInfoIndex.setFirstOnlineTime(FormatUtils.convertNumberToSecTime(firstLandingTime.intValue()));
            attendanceDetailInfoIndex.setLastOnlineTime(FormatUtils.convertNumberToSecTime(lastOnlineTime.intValue()));
            attendanceDetailInfoIndex.setLandingTimeSum(landingTime.setScale(SystemConfig.DEFAULT_SCALE).toString());
            attendanceDetailInfoIndex.setHungUpTimeSum(hungUpTime.setScale(SystemConfig.DEFAULT_SCALE).toString());
            attendanceDetailInfoIndex.setOfflineTimeSum(offlineTime.setScale(SystemConfig.DEFAULT_SCALE).toString());
            attendanceDetailInfoIndex.setOnlineRate(onlineRate.setScale(SystemConfig.DEFAULT_SCALE).toString() + "%");
        }else {
            attendanceDetailInfoIndex.setFirstOnlineTime("-");
            attendanceDetailInfoIndex.setLastOnlineTime("-");
            attendanceDetailInfoIndex.setLandingTimeSum("0.0");
            attendanceDetailInfoIndex.setHungUpTimeSum("0.0");
            attendanceDetailInfoIndex.setOfflineTimeSum("0.0");
            attendanceDetailInfoIndex.setOnlineRate("0.0%");
        }
    }
    //将查询的出勤率详情的数据转换为界面上要显示的数据
    public static void formateAttendanceInfoToShowData(AttendanceInfo attendanceInfoIndex){

        BigDecimal dayCount = new BigDecimal(attendanceInfoIndex.getAttendanceDayCount());

        if (dayCount.intValue() != 0){
            BigDecimal landingTimeSum =  new BigDecimal(attendanceInfoIndex.getSqlAttendanceLandingTimeSum());
            BigDecimal firstLandingTimeSum = new BigDecimal(attendanceInfoIndex.getSqlAttendanceFirstLandingTimeSum());
            BigDecimal endlandingTimeSum = new BigDecimal(attendanceInfoIndex.getSqlAttendanceLastOnlineTimeSum());
            BigDecimal earliestLandingTime = new BigDecimal(attendanceInfoIndex.getSqlAttendanceEarliestLandingTime());
            BigDecimal latestOnlineTime = new BigDecimal(attendanceInfoIndex.getSqlAttendanceLatestOnlineTime());

            landingTimeSum = landingTimeSum.divide(new BigDecimal(3600),SystemConfig.DEFAULT_SCALE, BigDecimal.ROUND_HALF_EVEN);

            BigDecimal landingTimeAvg =landingTimeSum.divide(dayCount,
                    SystemConfig.DEFAULT_SCALE, BigDecimal.ROUND_HALF_EVEN);

            BigDecimal firstLandingTimeAvg =firstLandingTimeSum.divide(dayCount,
                    SystemConfig.DEFAULT_SCALE, BigDecimal.ROUND_HALF_EVEN);

            BigDecimal endLandingTimeAvg =endlandingTimeSum.divide(dayCount,
                    SystemConfig.DEFAULT_SCALE, BigDecimal.ROUND_HALF_EVEN);

            attendanceInfoIndex.setAttendanceTimeSum(landingTimeSum.toString());
            attendanceInfoIndex.setAttendanceTimeAvg(landingTimeAvg.toString());
            attendanceInfoIndex.setAttendanceStartTimeAvg(FormatUtils.convertNumberToMinTime(firstLandingTimeAvg.intValue()));
            attendanceInfoIndex.setAttendanceEndTimeAvg(FormatUtils.convertNumberToMinTime(endLandingTimeAvg.intValue()));
            attendanceInfoIndex.setAttendanceEarliestTime(FormatUtils.convertNumberToSecTime(earliestLandingTime.intValue()));
            attendanceInfoIndex.setAttendanceLatestTime(FormatUtils.convertNumberToSecTime(latestOnlineTime.intValue()));
        }else {
            attendanceInfoIndex.setAttendanceTimeSum("0.0");
            attendanceInfoIndex.setAttendanceTimeAvg("0.0");
            attendanceInfoIndex.setAttendanceStartTimeAvg("-");
            attendanceInfoIndex.setAttendanceEndTimeAvg("-");
            attendanceInfoIndex.setAttendanceEarliestTime("-");
            attendanceInfoIndex.setAttendanceLatestTime("-");
        }
        return;
    }
    //未响应量，需要过滤
    public static int getUnresponseCustomerCount(String ids,String idsReception){
        String[] idList = ids.split(";");
        Map<String,Integer> idMap = new HashMap<>();
        for (String index : idList){
            if (index != null && !index.equals("null") && !index.isEmpty()) {
                if (idMap.get(index) == null){
                    idMap.put(index,1);
                }else {
                    idMap.put(index,idMap.get(index)+1);
                }
            }
        }

        String[] idListReception = idsReception.split(";");
        Map<String,Integer> idMapReception = new HashMap<>();
        for (String index : idListReception){
            if (index != null && !index.equals("null") && !index.isEmpty()) {
                if (idMapReception.get(index) == null){
                    idMapReception.put(index,1);
                }else {
                    idMapReception.put(index,idMapReception.get(index)+1);
                }
            }
        }

        Iterator<String> it = idMap.keySet().iterator();
        int result = 0;
        while(it.hasNext()){
            String key =it.next().toString();
            Integer value = idMap.get(key);
            if (idMapReception.get(key)  == null)
                result ++;
            else if (idMapReception.get(key) != value)
                result++;
        }
        return result;
    }
    public static boolean findDayInAttendanceDetailInfoList(long date ,List<AttendanceDetailInfo> attendanceDetailInfoList){

        for (AttendanceDetailInfo attendanceInfoIndex : attendanceDetailInfoList){
            if (attendanceInfoIndex.getSqlAttendanceDate() == date)
                return  true;
        }
        return  false;
    }

    public static  AttendanceDetailInfo getEmptyAttendanceDetailInfo(long date){
        AttendanceDetailInfo emptyInfo = new AttendanceDetailInfo();
        emptyInfo.setDate(FormatUtils.convertNumberToDate(date));
        emptyInfo.setFirstOnlineTime("--");
        emptyInfo.setLastOnlineTime("--");
        emptyInfo.setLandingCount(0);
        emptyInfo.setLandingTimeSum("0.0");
        emptyInfo.setHungUpTimeSum("0.0");
        emptyInfo.setOfflineTimeSum("0.0");
        emptyInfo.setOnlineRate("0.0%");
        emptyInfo.setSqlAttendanceDate(date);
        return emptyInfo;
    }

    public static  AttendanceInfo getEmptyAttendanceInfo(String name){
        AttendanceInfo emptyInfo = new AttendanceInfo();
        emptyInfo.setSubMerchantName(name);
        emptyInfo.setAttendanceDayCount(0);
        emptyInfo.setAttendanceTimeSum("0.0");
        emptyInfo.setAttendanceTimeAvg("0.0");
        emptyInfo.setAttendanceEarliestTime("--");
        emptyInfo.setAttendanceLatestTime("--");
        emptyInfo.setAttendanceEndTimeAvg("--");
        emptyInfo.setAttendanceStartTimeAvg("--");
        return emptyInfo;
    }
    //咨询量，接待量去重的函数
    public static int getCustomerCount(String ids){
        String[] idList = ids.split(";");
        Set<String> idSet = new HashSet<>();
        for (String index : idList){
            if (index != null && !index.equals("null") && !index.isEmpty())
                idSet.add(index);
        }
        return idSet.size();
    }
    //将查询出来的工作量统计的数据转换为界面上要显示的数据
    public static void formateToShowData(BaseWorkLoadInfo workLoadInfo){
        BigDecimal landingTime =  new BigDecimal(workLoadInfo.getSqlLandingTimeSum());
        landingTime = landingTime.setScale(SystemConfig.DEFAULT_SCALE);

        BigDecimal receptionTime =  new BigDecimal(workLoadInfo.getSqlReceptionTimeSum());
        receptionTime = receptionTime.setScale(SystemConfig.DEFAULT_SCALE);

        BigDecimal askingCount = new BigDecimal(workLoadInfo.getSqlAskingIdsCount());
        askingCount = askingCount.setScale(SystemConfig.DEFAULT_SCALE);

        BigDecimal responseCount = new BigDecimal(workLoadInfo.getSqlResponseIdsCount());
        responseCount = responseCount.setScale(SystemConfig.DEFAULT_SCALE);

        BigDecimal unresponseCount = new BigDecimal(workLoadInfo.getSqlUnResponseIdsCount());
        unresponseCount = unresponseCount.setScale(SystemConfig.DEFAULT_SCALE);

        BigDecimal messageCount = new BigDecimal(workLoadInfo.getSqlReponseMessageCount());
        BigDecimal sessionCount = new BigDecimal(workLoadInfo.getSqlSessionCount());
        BigDecimal sessionTime = new BigDecimal(workLoadInfo.getSqlSessionTimeSum());
        BigDecimal messageCountAvg ;
        BigDecimal sessionTimeAvg;

        if (sessionCount.intValue() != 0) {
            messageCountAvg = messageCount.divide(sessionCount,1, BigDecimal.ROUND_HALF_EVEN);
            sessionTimeAvg = sessionTime.divide(sessionCount,1, BigDecimal.ROUND_HALF_EVEN);
            sessionTimeAvg = sessionTimeAvg.divide(new BigDecimal(60),1, BigDecimal.ROUND_HALF_EVEN);
            messageCountAvg = messageCountAvg.setScale(SystemConfig.DEFAULT_SCALE);
            sessionTimeAvg = sessionTimeAvg.setScale(SystemConfig.DEFAULT_SCALE);
            workLoadInfo.setResponseMessageCountPerSessionAvg(messageCountAvg.toString());
            workLoadInfo.setTimePerSessionAvg(sessionTimeAvg.toString());
        }else {
            workLoadInfo.setResponseMessageCountPerSessionAvg("-");
            workLoadInfo.setTimePerSessionAvg("-");
        }


        BigDecimal responseTime = new BigDecimal(workLoadInfo.getSqlResponseTimeSum());
        BigDecimal responseTimes = new BigDecimal(workLoadInfo.getSqlResponseCount());

        BigDecimal firstResponseTimeAvg;
        if (responseTimes.intValue() != 0 ) {
            firstResponseTimeAvg = responseTime.divide(responseTimes,1, BigDecimal.ROUND_HALF_EVEN);
            firstResponseTimeAvg = firstResponseTimeAvg.setScale(SystemConfig.DEFAULT_SCALE);
            workLoadInfo.setFirstResponseTimeAvg(firstResponseTimeAvg.toString());
        }else {
            workLoadInfo.setFirstResponseTimeAvg("-");
        }

        landingTime = landingTime.divide(new BigDecimal(3600),SystemConfig.DEFAULT_SCALE, BigDecimal.ROUND_HALF_EVEN);
        receptionTime = receptionTime.divide(new BigDecimal(3600), SystemConfig.DEFAULT_SCALE, BigDecimal.ROUND_HALF_EVEN);
        workLoadInfo.setLandingTimeSum(landingTime.toString());
        workLoadInfo.setReceptionTimeSum(receptionTime.toString());
        workLoadInfo.setAskingCustomerCount(askingCount.toString());
        workLoadInfo.setResponseCustomerCount(responseCount.toString());
        workLoadInfo.setUnresponseCustomerCount(unresponseCount.toString());

        BigDecimal askingTimes = new BigDecimal(workLoadInfo.getSqlAskingTimes());
        askingTimes = askingTimes.setScale(SystemConfig.DEFAULT_SCALE,BigDecimal.ROUND_HALF_EVEN);

        BigDecimal responseTimesBig = new BigDecimal(workLoadInfo.getSqlResponseTimes());
        responseTimesBig = responseTimesBig.setScale(SystemConfig.DEFAULT_SCALE,BigDecimal.ROUND_HALF_EVEN);

        workLoadInfo.setAskingTimes(askingTimes.toString());
        workLoadInfo.setResponseTimes(responseTimesBig.toString());

        BigDecimal responseRate;
        if (askingTimes.intValue() != 0){
            responseRate = responseTimesBig.divide(askingTimes,3, BigDecimal.ROUND_HALF_EVEN) ;
            responseRate = responseRate.multiply(new BigDecimal(100));
            responseRate = responseRate.setScale(SystemConfig.DEFAULT_SCALE);
            workLoadInfo.setResponseRate(responseRate.toString()+"%");
        }else {
            workLoadInfo.setResponseRate("-");
        }

    }
}
