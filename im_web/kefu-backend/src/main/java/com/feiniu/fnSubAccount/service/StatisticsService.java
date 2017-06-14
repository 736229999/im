package com.feiniu.fnSubAccount.service;

/**
 * Created by wangzhen on 2015/7/22.
 */

import com.feiniu.fnSubAccount.bean.*;
import com.feiniu.fnSubAccount.bean.request.AttendanceRequest;
import com.feiniu.fnSubAccount.consts.SystemConfig;
import com.feiniu.fnSubAccount.repository.AttendanceMessageDao;
import com.feiniu.fnSubAccount.repository.StatisticStatusDao;
import com.feiniu.fnSubAccount.repository.WorkLoadMessageDao;
import com.feiniu.fnSubAccount.service.external.MerchantService;
import com.feiniu.fnSubAccount.util.FormatUtils;
import com.feiniu.fnSubAccount.util.StatisticsUtil;
import com.feiniu.fnSubAccount.util.web.HttpUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.math.BigDecimal;
import java.util.*;

/**
 * Created by wangzhen on 2015/7/22.
 */
@SuppressWarnings({ "unused","unchecked", "rawtypes" })
@Service
public class StatisticsService {
    @Autowired
    private StatisticStatusDao statisticStatusDao;

    @Autowired
    private WorkLoadMessageDao workLoadMessageDao;

    @Autowired
    private AttendanceMessageDao attendanceMessageDao;

    @Autowired
    private MerchantService merchantService;

    @Autowired
    private DirectorService directorService;

    private static final Logger LOGGER = LoggerFactory.getLogger(StatisticsService.class);

    /*
    * 定时任务的相关入口函数
    */
    //插入一行数据到IM_STATISITS_STATUS表,给定时任务调用@Transactional
    @Transactional
    public void insertBuildStatus() {
        Long currentTime = FormatUtils.getTodayTimeInSeconds();

        String ipAddress = "0.0.0.0";
        if (0 == statisticStatusDao.insertIntoStatus(currentTime, SystemConfig.STATISITC_STATUS_NOT_STATISITCED, ipAddress)) {
            LOGGER.info(currentTime.toString() + ":插入统计状态到统计状态表失败");
            return;
        }
    }

    //为了方便手动跑批任务独立出来的函数
    @Transactional
    public void doUpdateDataBaseBuild(Long currentTime, Long tomorrowTime) {
        String ipAddress = HttpUtils.getIPAddress();
        try {
            //获取所有商家信息
            List<Long> merchantIdsList = merchantService.getAllMerchantIds();

            if (merchantIdsList.isEmpty()) {
                LOGGER.info((new Date()).toString() + "调用接口获取商家信息失败");
                return;
            }
            //统计临时表数据
            workLoadMessageDao.updateReplyDetail(currentTime, tomorrowTime);

            int indexMerchant = 0;
            String merchantIDs = "";

            List<SubMerchantInfo> subMerchantInfoList = new ArrayList<>();
            //限制一次获取的商家信息的数量，防止数据太长，调用失败
            for (Long merchantID : merchantIdsList) {
                indexMerchant++;
                merchantIDs += String.valueOf(merchantID) + ",";
                if (indexMerchant >= SystemConfig.STATISITC_MAX_STATISITC_WORKLOAD_MERCHANT_NUM) {
                    subMerchantInfoList = merchantService.retrieveSubMerchantInfo(merchantIDs);
                    doStatisticsWorkload(currentTime, tomorrowTime, subMerchantInfoList);
                    indexMerchant = 0;
                    merchantIDs = "";
                }
            }
            if (indexMerchant != 0) {
                subMerchantInfoList = merchantService.retrieveSubMerchantInfo(merchantIDs);
                doStatisticsWorkload(currentTime, tomorrowTime, subMerchantInfoList);
            }

        } catch (Exception e) {

            updateBuildStatus(currentTime, SystemConfig.STATISITC_STATUS_FAILED, ipAddress);
            LOGGER.error((new Date()).toString() + "<=====:定时统计任务异常", e);
            return;
        }
    }

    //定时统计任务
    @Transactional
    public void updateDataBaseBuild() {
        Long currentTime = FormatUtils.getTodayTimeInSeconds();
        Long tomorrowTime = FormatUtils.getTomorrowTimeInSeconds();

        String ipAddress = HttpUtils.getIPAddress();

        LOGGER.info((new Date()).toString() + "=====>定时统计数据开始");
        //判断今天的定时任务是否已经被执行过。
        if (0 > updateBuildStatus(currentTime, SystemConfig.STATISITC_STATUS_UPDATING, ipAddress)) {
            LOGGER.info((new Date()).toString() + "<=====:今天的数据已经更新过，不需要再更新");
            return;
        }
        //刷新数据
        doUpdateDataBaseBuild(currentTime, tomorrowTime);
        //更新数据后，重新刷新状态表
        updateBuildStatus(currentTime, SystemConfig.STATISITC_STATUS_UPDATED, ipAddress);
        LOGGER.info((new Date()).toString() + "<=====定时统计任务结束");
    }

    /*
    * excel导出相关入口函数
    */
    //excel导出的时候，获取标题栏的函数
    public String[] getAttendanceTitileStringByRequest(AttendanceRequest attendanceRequest) {
        String[] result = null;
        switch (attendanceRequest.getSearchType()) {
            case SystemConfig.ATTENDANCE_INFO: {
                result = new String[8];
                result[0] = "哞哞客服";
                result[1] = "上班天数";
                result[2] = "累计登陆总时长(小时)";
                result[3] = "日均登录时长（小时）";
                result[4] = "每天首次上线时间均值";
                result[5] = "每天最后离线时间均值";
                result[6] = "最早上线时间";
                result[7] = "最晚下线时间";
                break;
            }
            case SystemConfig.ATTENDANCE_DETAIL_INFO: {
                result = new String[8];
                result[0] = "日期";
                result[1] = "当天首次上线时间";
                result[2] = "当天最后在线时间";
                result[3] = "登录次数";
                result[4] = "当天累计登录总时长（小时）";
                result[5] = "当天累计挂起总时长（小时）";
                result[6] = "当天累计离线总时长（小时）";
                result[7] = "在线率";
                break;
            }
            case SystemConfig.WORKLOAD_INFO: {
                result = new String[12];
                result[0] = "哞哞客服";
                result[1] = "分组名称";
                result[2] = "累计登录总时长（小时）";
                result[3] = "累计接待总时长（小时）";
                result[4] = "咨询量（人）";
                result[5] = "接待量（人）";
                result[6] = "咨询次数";
                result[7] = "接待次数";
                result[8] = "回复率";
                result[9] = "平均回复消息数";
                result[10] = "会话平均时长（分钟）";
                result[11] = "平均首次回复响应时间（秒）";
                break;
            }
            case SystemConfig.WORKLOAD_DETAIL_INFO: {
                result = new String[11];
                result[0] = "日期";
                result[1] = "累计登录总时长（小时）";
                result[2] = "累计接待总时长（小时）";
                result[3] = "咨询量（人）";
                result[4] = "接待量（人）";
                result[5] = "咨询次数";
                result[6] = "接待次数";
                result[7] = "回复率";
                result[8] = "平均回复消息数";
                result[9] = "会话平均时长（分钟）";
                result[10] = "平均首次回复响应时间（秒）";
                break;
            }
            default:
                break;
        }
        return result;
    }

    //excel导出，获取所有数据
    @Transactional
    public String[][] getAttendanceArrayStringByRequest(AttendanceRequest attendanceRequest) {
        String[][] result = null;
        switch (attendanceRequest.getSearchType()) {
            case SystemConfig.ATTENDANCE_INFO: {
                List<AttendanceInfo> attendanceInfos = getAttendanceListByTimeAndMerchantId(attendanceRequest.getStartTime(),
                        attendanceRequest.getEndTime(), attendanceRequest.getMerchantID(),attendanceRequest.getDirectorID());
                int indexNum = 0;
                result = new String[attendanceInfos.size()][8];

                for (AttendanceInfo index : attendanceInfos) {
                    result[indexNum][0] = index.getSubMerchantName();
                    result[indexNum][1] = String.valueOf(index.getAttendanceDayCount());
                    result[indexNum][2] = index.getAttendanceTimeSum();
                    result[indexNum][3] = index.getAttendanceTimeAvg();
                    result[indexNum][4] = index.getAttendanceStartTimeAvg();
                    result[indexNum][5] = index.getAttendanceEndTimeAvg();
                    result[indexNum][6] = index.getAttendanceEarliestTime();
                    result[indexNum][7] = index.getAttendanceLatestTime();
                    indexNum++;
                }
                break;
            }
            case SystemConfig.ATTENDANCE_DETAIL_INFO: {
                List<AttendanceDetailInfo> attendanceInfos = getAttendanceDetailByTimeAndSubmerchantId(attendanceRequest.getStartTime(),
                        attendanceRequest.getEndTime(), attendanceRequest.getSubMerchantID());
                int indexNum = 0;
                result = new String[attendanceInfos.size()][8];

                for (AttendanceDetailInfo index : attendanceInfos) {
                    result[indexNum][0] = index.getDate();
                    result[indexNum][1] = index.getFirstOnlineTime();
                    result[indexNum][2] = index.getLastOnlineTime();
                    result[indexNum][3] = String.valueOf(index.getLandingCount());
                    result[indexNum][4] = index.getLandingTimeSum();
                    result[indexNum][5] = index.getHungUpTimeSum();
                    result[indexNum][6] = index.getOfflineTimeSum();
                    result[indexNum][7] = index.getOnlineRate();
                    indexNum++;
                }
                break;
            }
            case SystemConfig.WORKLOAD_INFO: {
                List<WorkLoadInfo> attendanceInfos = (List) getWorkLoadListByTimeAndMerchantId(attendanceRequest.getStartTime(), attendanceRequest.getEndTime(),
                        attendanceRequest.getMerchantID(), attendanceRequest.getCurrentPageNo() == 1,attendanceRequest.getDirectorID());
                int indexNum = 0;
                result = new String[attendanceInfos.size()][12];

                for (WorkLoadInfo index : attendanceInfos) {
                    result[indexNum][0] = index.getSubMerchantName();
                    result[indexNum][1] = index.getSubMerchantGroup();
                    result[indexNum][2] = index.getLandingTimeSum();
                    result[indexNum][3] = index.getReceptionTimeSum();
                    result[indexNum][4] = index.getAskingCustomerCount();
                    result[indexNum][5] = index.getResponseCustomerCount();
                    result[indexNum][6] = index.getAskingTimes();
                    result[indexNum][7] = index.getResponseTimes();
                    result[indexNum][8] = index.getResponseRate();
                    result[indexNum][9] = index.getResponseMessageCountPerSessionAvg();
                    result[indexNum][10] = index.getTimePerSessionAvg();
                    result[indexNum][11] = index.getFirstResponseTimeAvg();
                    indexNum++;
                }
                break;
            }
            case SystemConfig.WORKLOAD_DETAIL_INFO: {
                List<WorkLoadDetailInfo> attendanceInfos = getWorkLoadDetailByTimeAndSubmerchantId(attendanceRequest.getStartTime(), attendanceRequest.getEndTime(),
                        attendanceRequest.getSubMerchantID(), attendanceRequest.getCurrentPageNo() == 1);
                int indexNum = 0;
                result = new String[attendanceInfos.size()][11];

                for (WorkLoadDetailInfo index : attendanceInfos) {
                    result[indexNum][0] = index.getDate();
                    result[indexNum][1] = index.getLandingTimeSum();
                    result[indexNum][2] = index.getReceptionTimeSum();
                    result[indexNum][3] = index.getAskingCustomerCount();
                    result[indexNum][4] = index.getResponseCustomerCount();
                    result[indexNum][5] = index.getAskingTimes();
                    result[indexNum][6] = index.getResponseTimes();
                    result[indexNum][7] = index.getResponseRate();
                    result[indexNum][8] = index.getResponseMessageCountPerSessionAvg();
                    result[indexNum][9] = index.getTimePerSessionAvg();
                    result[indexNum][10] = index.getFirstResponseTimeAvg();
                    indexNum++;
                }
                break;
            }
            default:
                break;
        }
        return result;
    }

    /*
    * 数据查询相关入口
    */
    //查询的入口函数
    @Transactional
    public List<Object> getAttendanceListByRequest(AttendanceRequest attendanceRequest) {
        switch (attendanceRequest.getSearchType()) {
            case SystemConfig.ATTENDANCE_INFO: {
                return (List) getAttendanceListByTimeAndMerchantId(attendanceRequest.getStartTime(),
                        attendanceRequest.getEndTime(), attendanceRequest.getMerchantID(),attendanceRequest.getDirectorID());
            }
            case SystemConfig.ATTENDANCE_DETAIL_INFO: {
                return (List) getAttendanceDetailByTimeAndSubmerchantId(attendanceRequest.getStartTime(),
                        attendanceRequest.getEndTime(), attendanceRequest.getSubMerchantID());
            }
            case SystemConfig.WORKLOAD_INFO: {
                return (List) getWorkLoadListByTimeAndMerchantId(attendanceRequest.getStartTime(), attendanceRequest.getEndTime(),
                        attendanceRequest.getMerchantID(), attendanceRequest.getCurrentPageNo() == 1,attendanceRequest.getDirectorID());
            }
            case SystemConfig.WORKLOAD_DETAIL_INFO: {
                return (List) getWorkLoadDetailByTimeAndSubmerchantId(attendanceRequest.getStartTime(), attendanceRequest.getEndTime(),
                        attendanceRequest.getSubMerchantID(), attendanceRequest.getCurrentPageNo() == 1);
            }
            default:
                break;
        }
        return null;
    }

    /*
    * 数据查询相关实现
    */
    //查询出勤率的数据
    @Transactional
    private List<AttendanceInfo> getAttendanceListByTimeAndMerchantId(long startTime, long endTime, String merchantID,String directorID) {
        List<SubMerchantInfo> subMerchantInfoList =
                directorService.getSubMerchantInfoListByDirectorId();

        if(subMerchantInfoList.size() ==0){
            return new ArrayList<>();
        }

        Map<Long, SubMerchantInfo> subMerchantInfoMap = FormatUtils.convertSubMerchantInfListToMap(subMerchantInfoList);

        String subMerchantIDlist = FormatUtils.convertSubMerchantInfListToIdList(subMerchantInfoMap);

        List<AttendanceInfo> resultInfo = attendanceMessageDao.getAttendanceListByTime(startTime, endTime, merchantID,subMerchantIDlist);

        for (AttendanceInfo attendanceInfoIndex : resultInfo) {
            StatisticsUtil.formateAttendanceInfoToShowData(attendanceInfoIndex);
            SubMerchantInfo resultIndex = subMerchantInfoMap.get(attendanceInfoIndex.getSubMerchantId());
            if (resultIndex != null) {
                attendanceInfoIndex.setSubMerchantName(resultIndex.getUsername());
                subMerchantInfoMap.remove(attendanceInfoIndex.getSubMerchantId());
            } else {
                attendanceInfoIndex.setSubMerchantName("");
            }
        }

        Iterator it=subMerchantInfoMap.keySet().iterator();
        while(it.hasNext()){
            Long key=(Long)it.next();
            SubMerchantInfo value = subMerchantInfoMap.get(key);
            AttendanceInfo emptyInfo = StatisticsUtil.getEmptyAttendanceInfo(value.getUsername());
            resultInfo.add(emptyInfo);
        }

        return resultInfo;
    }

    //查询出勤率详情的数据
    @Transactional
    private List<AttendanceDetailInfo> getAttendanceDetailByTimeAndSubmerchantId(long startTime, long endTime, String subMerchantID) {

        List<AttendanceDetailInfo> resultInfo = attendanceMessageDao.getAttendanceDetailListByTime(startTime, endTime, subMerchantID);

        for (AttendanceDetailInfo attendanceInfoIndex : resultInfo) {
            StatisticsUtil.formateAttendanceInfoToShowData(attendanceInfoIndex);
        }

        while (startTime < endTime){
            if (!StatisticsUtil.findDayInAttendanceDetailInfoList(startTime,resultInfo)){
                AttendanceDetailInfo emptyInfo = StatisticsUtil.getEmptyAttendanceDetailInfo(startTime);
                resultInfo.add(emptyInfo);
            }
            startTime += SystemConfig.STATISITC_ONE_DAY_SECOUNDS;
        }
        Collections.sort(resultInfo);
        return resultInfo;
    }

    //工作量统计查询函数
    @Transactional
    private List<WorkLoadInfo> getWorkLoadListByTimeAndMerchantId(long startTime, long endTime, String merchantID, boolean needTotalInfo,String directorID) {
        List<SubMerchantInfo> subMerchantInfoList =
                directorService.getSubMerchantInfoListByDirectorId();

        if(subMerchantInfoList.size() ==0){
            return  new ArrayList<>();
        }

        Map<Long, SubMerchantInfo> subMerchantInfoMap = FormatUtils.convertSubMerchantInfListToMap(subMerchantInfoList);

        String subMerchantIDlist = FormatUtils.convertSubMerchantInfListToIdList(subMerchantInfoMap);

        List<WorkLoadInfo> resultInfo = workLoadMessageDao.getWorkLoadListByTime(startTime, endTime, merchantID,subMerchantIDlist);

        if (resultInfo == null || resultInfo.size() == 0)
            return resultInfo;

        for (WorkLoadInfo workloadindex : resultInfo) {
            StatisticsUtil.formateToShowData(workloadindex);
            if (subMerchantInfoMap.get(workloadindex.getSubMerchantId()) != null)
                workloadindex.setSubMerchantName(subMerchantInfoMap.get(workloadindex.getSubMerchantId()).getUsername());
            else
                workloadindex.setSubMerchantName("--");
        }

        WorkLoadInfo totalInfo = new WorkLoadInfo();
        WorkLoadInfo avgInfo = new WorkLoadInfo();

        if (needTotalInfo) {
            List<BaseWorkLoadInfo> totalInfoList = getTotalWorkLoadInfo((List) resultInfo, 1,startTime,endTime,merchantID);
            ((WorkLoadInfo) totalInfoList.get(0)).setSubMerchantName("汇总");
            ((WorkLoadInfo) totalInfoList.get(0)).setSubMerchantGroup("--");
            ((WorkLoadInfo) totalInfoList.get(1)).setSubMerchantName("均值");
            ((WorkLoadInfo) totalInfoList.get(1)).setSubMerchantGroup("--");
            resultInfo.addAll(0, (List) totalInfoList);
        } else {
            resultInfo.add(0, totalInfo);
            resultInfo.add(1, avgInfo);
        }
        return resultInfo;
    }

    //工作量统计汇总信息生成的函数
    @Transactional
    private List<BaseWorkLoadInfo> getTotalWorkLoadInfo(List<BaseWorkLoadInfo> listData, int type,long startTime, long endTime, String merchantId) {
        BaseWorkLoadInfo totalInfo;
        BaseWorkLoadInfo avgInfo;
        int sessionNotNullCount = 0;
        int responseNotNullCount = 0;
        if (type == 0) {
            totalInfo = new WorkLoadDetailInfo();
            totalInfo.initWithZero();
            avgInfo = new WorkLoadDetailInfo();
            avgInfo.initWithZero();
        } else {
            totalInfo = new WorkLoadInfo();
            totalInfo.initWithZero();
            avgInfo = new WorkLoadInfo();
            avgInfo.initWithZero();
        }

        for (BaseWorkLoadInfo workloadindex : listData) {
            if (!workloadindex.getResponseMessageCountPerSessionAvg().equals("-"))
                sessionNotNullCount++;
            if (!workloadindex.getTimePerSessionAvg().equals("-"))
                responseNotNullCount++;
            totalInfo.add(workloadindex);
        }

        avgInfo = (BaseWorkLoadInfo) totalInfo.clone();

        BigDecimal askingCount = new BigDecimal(totalInfo.getAskingCustomerCount());
        BigDecimal responseCount = new BigDecimal(totalInfo.getResponseCustomerCount());
        BigDecimal messageCountTotal = new BigDecimal(totalInfo.getResponseMessageCountPerSessionAvg());
        BigDecimal sessionTimeTotal = new BigDecimal(totalInfo.getTimePerSessionAvg());
        BigDecimal responseTimeTotal = new BigDecimal(totalInfo.getFirstResponseTimeAvg());
        if (askingCount.intValue() != 0) {
            BigDecimal responseRate;
            responseRate = responseCount.divide(askingCount, 3, BigDecimal.ROUND_HALF_EVEN);
            responseRate = responseRate.multiply(new BigDecimal(100));
            responseRate = responseRate.setScale(SystemConfig.DEFAULT_SCALE);
            totalInfo.setResponseRate(responseRate.toString() + "%");
        } else {
            totalInfo.setResponseRate("-");
        }

        totalInfo.setResponseRate("--");
        totalInfo.setResponseMessageCountPerSessionAvg("--");
        totalInfo.setTimePerSessionAvg("--");
        totalInfo.setFirstResponseTimeAvg("--");

        avgInfo.div(listData.size());
        askingCount = new BigDecimal(avgInfo.getAskingTimes());
        responseCount = new BigDecimal(avgInfo.getResponseTimes());

        if (askingCount.intValue() != 0) {
            BigDecimal responseRate;
            responseRate = responseCount.divide(askingCount, 3, BigDecimal.ROUND_HALF_EVEN);
            responseRate = responseRate.multiply(new BigDecimal(100));
            responseRate = responseRate.setScale(SystemConfig.DEFAULT_SCALE);
            avgInfo.setResponseRate(responseRate.toString() + "%");
        } else {
            avgInfo.setResponseRate("-");
        }

        if (sessionNotNullCount != 0) {
            BigDecimal messageCountAvg = messageCountTotal.divide(new BigDecimal(sessionNotNullCount), 1, BigDecimal.ROUND_HALF_EVEN);
            messageCountAvg = messageCountAvg.setScale(SystemConfig.DEFAULT_SCALE);

            BigDecimal sessionTimeAvg = sessionTimeTotal.divide(new BigDecimal(sessionNotNullCount), 1, BigDecimal.ROUND_HALF_EVEN);
            sessionTimeAvg = sessionTimeAvg.setScale(SystemConfig.DEFAULT_SCALE);

            avgInfo.setResponseMessageCountPerSessionAvg(messageCountAvg.toString());
            avgInfo.setTimePerSessionAvg(sessionTimeAvg.toString());
        } else {
            avgInfo.setResponseMessageCountPerSessionAvg("-");
            avgInfo.setTimePerSessionAvg("-");
        }

        if (responseNotNullCount != 0) {
            BigDecimal firstResponseTimeAvg = responseTimeTotal.divide(new BigDecimal(sessionNotNullCount), 1, BigDecimal.ROUND_HALF_EVEN);
            firstResponseTimeAvg = firstResponseTimeAvg.setScale(SystemConfig.DEFAULT_SCALE);
            avgInfo.setFirstResponseTimeAvg(firstResponseTimeAvg.toString());
        } else {
            avgInfo.setFirstResponseTimeAvg("-");
        }
        BaseWorkLoadInfo workLoadInfo ;
        if (type != 0) {
            workLoadInfo = workLoadMessageDao.getTotalWorkLoadListByTime(startTime, endTime, merchantId);
        } else {
            workLoadInfo = workLoadMessageDao.getTotalWorkLoadListDetailByTime(startTime, endTime, merchantId);
        }

        StatisticsUtil.formateToShowData(workLoadInfo);
        totalInfo.setLandingTimeSum(workLoadInfo.getLandingTimeSum());
        totalInfo.setAskingCustomerCount(workLoadInfo.getAskingCustomerCount());
        totalInfo.setResponseCustomerCount(workLoadInfo.getResponseCustomerCount());
        List<BaseWorkLoadInfo> resultList = new ArrayList<>();
        resultList.add(totalInfo);
        resultList.add(avgInfo);

        return resultList;
    }

    //工作量详情统计查询函数
    @Transactional
    private List<WorkLoadDetailInfo> getWorkLoadDetailByTimeAndSubmerchantId(long startTime, long endTime, String subMerchantID, boolean needTotalInfo) {

        List<WorkLoadDetailInfo> resultInfo = workLoadMessageDao.getWorkLoadDetailByTime(startTime, endTime, subMerchantID);

        if (resultInfo == null || resultInfo.size() == 0)
            return resultInfo;

        for (WorkLoadDetailInfo workloadindex : resultInfo) {
            StatisticsUtil.formateToShowData(workloadindex);

            workloadindex.setDate(FormatUtils.convertNumberToDate(workloadindex.getSqlDate()));
        }

        WorkLoadDetailInfo totalInfo = new WorkLoadDetailInfo();
        WorkLoadDetailInfo avgInfo = new WorkLoadDetailInfo();

        if (needTotalInfo) {
            List<BaseWorkLoadInfo> totalInfoList = getTotalWorkLoadInfo((List) resultInfo, 0,startTime,endTime,subMerchantID);
            ((WorkLoadDetailInfo) totalInfoList.get(0)).setDate("汇总");
            ((WorkLoadDetailInfo) totalInfoList.get(1)).setDate("均值");
            resultInfo.addAll(0, (List) totalInfoList);
        } else {
            resultInfo.add(0, totalInfo);
            resultInfo.add(1, avgInfo);
        }
        Collections.sort(resultInfo);
        return resultInfo;
    }

    /*
    * 工作量统计的相关实现
    */
    //工作量统计的具体是实现函数
    @Transactional
    private void doStatisticsWorkload(long currentTime, long tomorrowTime, List<SubMerchantInfo> subMerchantInfoList) {
        int indexsubMerchant = 0;
        String merchantIDs = "";
        String subMerchantIds = "";

        for (SubMerchantInfo subMerchantInfo : subMerchantInfoList) {
            merchantIDs += String.valueOf(subMerchantInfo.getMerchantId()) + ";";
            subMerchantIds += String.valueOf(subMerchantInfo.getMemberId()) + ";";
            indexsubMerchant++;
            if (indexsubMerchant >= SystemConfig.STATISITC_MAX_STATISITC_WORKLOAD_SUB_MRCHANT_NUM) {
                workLoadMessageDao.updateWorkLoadList(currentTime, tomorrowTime, subMerchantIds, merchantIDs, currentTime);
                subMerchantIds = subMerchantIds.substring(0, subMerchantIds.length() - 1);
                subMerchantIds = subMerchantIds.replace(";", ",");
                List<ReceptionInfo> receptionInfos = workLoadMessageDao.getReceptionInfo(subMerchantIds, currentTime);
                updateRecptionTime(receptionInfos, currentTime, tomorrowTime);
                indexsubMerchant = 0;
                merchantIDs = "";
                subMerchantIds = "";
            }
        }
        if (indexsubMerchant != 0) {
            workLoadMessageDao.updateWorkLoadList(currentTime, tomorrowTime, subMerchantIds, merchantIDs, currentTime);
            subMerchantIds = subMerchantIds.substring(0, subMerchantIds.length() - 1);
            subMerchantIds = subMerchantIds.replace(";", ",");
            List<ReceptionInfo> receptionInfos = workLoadMessageDao.getReceptionInfo(subMerchantIds.replace(";", ","), currentTime);
            updateRecptionTime(receptionInfos, currentTime, tomorrowTime);
        }
    }

    //更新接待时间，因为接待时间需要去重，没办法在存储过程中写。
    @Transactional
    private void updateRecptionTime(List<ReceptionInfo> receptionInfos, long currentTime, long tomorrowTime) {
        Map<Long, List<ReceptionInfo>> receptionInfosMap = new HashMap<>();
        for (ReceptionInfo index : receptionInfos) {
            if (receptionInfosMap.get(index.getSubMerchantId()) == null) {
                List<ReceptionInfo> newRecInfoList = new ArrayList<>();
                receptionInfosMap.put(index.getSubMerchantId(), newRecInfoList);
            }
            receptionInfosMap.get(index.getSubMerchantId()).add(index);
        }

        Iterator it = receptionInfosMap.keySet().iterator();
        while (it.hasNext()) {
            long subMerchantID = (long) it.next();
            List<ReceptionInfo> receptionInfoListIndex = receptionInfosMap.get(subMerchantID);
            long receptionTime = getReceptionTime(receptionInfoListIndex, currentTime);
            workLoadMessageDao.setReceptionTime(receptionTime, currentTime, subMerchantID);
        }
    }

    @Transactional
    private long getReceptionTime(List<ReceptionInfo> receptionInfoListIndex, long currentTime) {
        int EveryTensecounds = 24 * 60 * 6;

        byte CorlorMap[] = new byte[EveryTensecounds];

        for (ReceptionInfo index : receptionInfoListIndex) {
            int startTime = (int) (index.getReplyStart() - currentTime) / 10;
            int endTime = (int) (index.getReplyEnd() - currentTime) / 10;

            while (startTime < endTime) {
                CorlorMap[startTime++] = 1;
            }
        }

        long totalDur = 0;
        for (int i = 0; i < CorlorMap.length; i++)
            totalDur += CorlorMap[i];

        return totalDur * 10;
    }

    //更新统计状态表
    @Transactional
    private int updateBuildStatus(Long currentTime, int statisitcStatus, String ipAddress) {
        Integer status = SystemConfig.STATISITC_STATUS_NODATA;
        Integer retrytime = 0;
        //锁定要统计的行，锁定有不允许其他任务去锁定改行
        Map<String, Object> returnParams = statisticStatusDao.lockUpdateStatus(currentTime);
        if (returnParams == null) {
            LOGGER.error("锁定统计状态表失败");
            return -1;
        }
        status = Integer.valueOf(returnParams.get("status").toString());
        retrytime = Integer.valueOf(returnParams.get("retrytime").toString());

        if (status == SystemConfig.STATISITC_STATUS_NODATA) {
            //还没有当天的数据项，插入数据项
            statisticStatusDao.insertIntoStatus(currentTime, statisitcStatus, ipAddress);
            return 0;
        } else if (status == SystemConfig.STATISITC_STATUS_UPDATED) {
            //如果当前的统计状态为已经更新，返回错误
            return -1;
        } else if (status == SystemConfig.STATISITC_STATUS_UPDATING) {
            //如果当前的状态为正在更新，允许更新状态为已更新或者更新失败
            if (statisitcStatus != SystemConfig.STATISITC_STATUS_UPDATED
                    && statisitcStatus != SystemConfig.STATISITC_STATUS_FAILED)
                return -1;
        } else if (status == SystemConfig.STATISITC_STATUS_FAILED || status == SystemConfig.STATISITC_STATUS_NOT_STATISITCED) {
            //如果当前状态为更新失败或者没有更新，允许执行更新函数
        } else {
            //出错
            return -1;
        }

        retrytime++;
        return statisticStatusDao.updateStatus(currentTime, statisitcStatus, ipAddress, retrytime.toString());
    }


    //删除所有的统计记录
    @Transactional
    public void clearAll() {
        statisticStatusDao.clearAll();
    }
}
