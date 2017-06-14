package com.cn.feiniu.logManagement.service.internal;

import com.cn.feiniu.logManagement.bean.base.BusinessConfig;
import com.cn.feiniu.logManagement.bean.dto.SubMerchantAttendanceDetail;
import com.cn.feiniu.logManagement.bean.dto.SubMerchantAttendanceDetailKey;
import com.cn.feiniu.logManagement.bean.web.hessian.req.SubMerchantAction;
import com.cn.feiniu.logManagement.constants.SubMerchantActionType;
import com.cn.feiniu.logManagement.constants.SystemConfig;
import com.cn.feiniu.logManagement.core.LogFileNamePatternManager;
import com.cn.feiniu.logManagement.helper.SubMerchantActionLogComparator;
import com.cn.feiniu.logManagement.repository.SubMerchantActionStatisticsDao;
import org.apache.commons.io.FileUtils;
import org.apache.commons.lang.time.DateUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.io.File;
import java.util.*;

/**
 * Author: morningking
 * Date: 2015/7/31
 * Contact: 243717042@qq.com
 */
@Service
public class SubMerchantActionStatisticsService {
    @Autowired
    private SubMerchantActionStatisticsDao subMerchantActionStatisticsDao;
    @Autowired
    private LogFileNamePatternManager logFileNamePatternManager;
    @Autowired
    private BusinessConfig businessConfig;

    @Transactional
    public void persistStatistics(List<SubMerchantAttendanceDetail> subMerchantAttendanceDetailList) {
        subMerchantActionStatisticsDao.saveActionStatisticsList(subMerchantAttendanceDetailList);
    }

    public List<SubMerchantAttendanceDetail> countFromLog(Date statisticsDate) throws Exception {
        /** 第一步：读日志，转换为业务对象 */
        HashMap<String, Object> params = new HashMap<>(1);
        params.put("date", statisticsDate);
        List<SubMerchantAction> subMerchantActionList = parseFile(params);

        /** 第二步：业务排序 */
        Collections.sort(subMerchantActionList, new SubMerchantActionLogComparator());

        /** 第三步：统计分析 */
        return analyzeData(subMerchantActionList, statisticsDate);
    }

    private List<SubMerchantAction> parseFile(Map<String, Object> params) throws Exception {
        String logFileName = businessConfig.getLogFilePath() + logFileNamePatternManager.getLogFileName(
                LogFileNamePatternManager.LogTypeEnum.SUB_MERCHANT_ACTION_LOG_PATTERN, params);
        List<String> logs = FileUtils.readLines(new File(logFileName));
        List<SubMerchantAction> subMerchantActionList = new ArrayList<>(logs.size());

        for (String line : logs) {
            String[] fields = line.split(SystemConfig.SUB_MERCHANT_ACTION_LOG_FIELD_SEPARATOR);

            SubMerchantAction subMerchantAction = new SubMerchantAction();
            subMerchantAction.setMerchantID(Integer.parseInt(fields[0]));
            subMerchantAction.setSubMerchantID(Long.parseLong(fields[1]));
            subMerchantAction.setActionType(Byte.parseByte(fields[2]));
            subMerchantAction.setActionTime(Integer.parseInt(fields[3]));

            subMerchantActionList.add(subMerchantAction);
        }

        return subMerchantActionList;
    }

    private List<SubMerchantAttendanceDetail> analyzeData(List<SubMerchantAction> subMerchantActionList, Date statisticsDate) {
        List<SubMerchantAttendanceDetail> subMerchantAttendanceDetailList = new ArrayList<>(1);
        TreeMap<Integer, Integer> logInOutPair = new TreeMap<>();
        TreeMap<Integer, Integer> hangStartOverPair = new TreeMap<>();
        TreeMap<Integer, Integer> leaveStartOverPair = new TreeMap<>();

        int currentMerchantID = -2;
        long currentSubMerchantID = -1L;
        int currentLoginTime = -1;
        int currentHangStartTime = -1;
        int currentLeaveStartTime = -1;

        for (SubMerchantAction subMerchantAction : subMerchantActionList) {
            if (currentSubMerchantID != subMerchantAction.getSubMerchantID()) {
                if (currentSubMerchantID != -1L) {
                    subMerchantAttendanceDetailList.add(
                            getNewDetail(
                                    logInOutPair, hangStartOverPair, leaveStartOverPair,
                                    currentMerchantID, currentSubMerchantID, statisticsDate
                            )
                    );
                }

                logInOutPair.clear();
                hangStartOverPair.clear();
                leaveStartOverPair.clear();

                currentLoginTime = -1;
                currentHangStartTime = -1;
                currentSubMerchantID = subMerchantAction.getSubMerchantID();
                currentMerchantID = subMerchantAction.getMerchantID();
            }

            SubMerchantActionType subMerchantActionType = SubMerchantActionType.getType(subMerchantAction.getActionType());
            if (subMerchantActionType != null) {
                switch (subMerchantActionType) {
                    case TYPE_LOGIN:
                        currentLoginTime = subMerchantAction.getActionTime();
                        logInOutPair.put(currentLoginTime, -1);
                        break;
                    case TYPE_LOGOUT:
                        if (currentLoginTime != -1) {
                            logInOutPair.put(currentLoginTime, subMerchantAction.getActionTime());
                        }
                        break;
                    case TYPE_HANG_START:
                        currentHangStartTime = subMerchantAction.getActionTime();
                        hangStartOverPair.put(currentHangStartTime, -1);
                        break;
                    case TYPE_HANG_OVER:
                        if (currentHangStartTime != -1) {
                            hangStartOverPair.put(currentHangStartTime, subMerchantAction.getActionTime());
                        }
                        break;
                    case TYPE_LEAVE_START:
                        currentLeaveStartTime = subMerchantAction.getActionTime();
                        leaveStartOverPair.put(currentLeaveStartTime, -1);
                        break;
                    case TYPE_LEAVE_END:
                        if (currentLeaveStartTime != -1) {
                            leaveStartOverPair.put(currentLeaveStartTime, subMerchantAction.getActionTime());
                        }
                        break;
                    default:
                        break;
                }
            }
        }

        if (currentMerchantID != -2) {
            subMerchantAttendanceDetailList.add(
                    getNewDetail(logInOutPair, hangStartOverPair, leaveStartOverPair, currentMerchantID, currentSubMerchantID, statisticsDate));
        }

        return subMerchantAttendanceDetailList;
    }

    private SubMerchantAttendanceDetail getNewDetail(TreeMap<Integer, Integer> logInOutPair, TreeMap<Integer, Integer> hangStartOverPair,
                                                     TreeMap<Integer, Integer> leaveStartOverPair,
                                                     int currentMerchantID, long currentSubMerchantID, Date statisticsDate) {
        Integer NIL_VALUE = -1;
        SubMerchantAttendanceDetail subMerchantAttendanceDetail = new SubMerchantAttendanceDetail();

        SubMerchantAttendanceDetailKey subMerchantAttendanceDetailKey = new SubMerchantAttendanceDetailKey();
        subMerchantAttendanceDetailKey.setMerchantID(currentMerchantID);
        subMerchantAttendanceDetailKey.setSubMerchantID(currentSubMerchantID);
        subMerchantAttendanceDetailKey.setStatisticsTime((int) (statisticsDate.getTime() / 1000));
        subMerchantAttendanceDetail.setSubMerchantAttendanceDetailKey(subMerchantAttendanceDetailKey);

        /** 处理登入登出 */
        if (logInOutPair.size() > 0) {
            /** 处理当天未退出的情况 **/
            if (NIL_VALUE.equals(logInOutPair.lastEntry().getValue())) {
                logInOutPair.put(logInOutPair.lastKey(), (int) (DateUtils.addDays(statisticsDate, 1).getTime() / 1000));
            }

            subMerchantAttendanceDetail.setFirstOnlineTime(logInOutPair.firstKey());
            subMerchantAttendanceDetail.setLoginCount(logInOutPair.size());
            subMerchantAttendanceDetail.setLastOnlineTime(logInOutPair.lastEntry().getValue());

            Iterator<Map.Entry<Integer, Integer>> entryIterator = logInOutPair.entrySet().iterator();
            int totalOnlineDuration = 0;

            while (entryIterator.hasNext()) {
                Map.Entry<Integer, Integer> entry = entryIterator.next();

                if (entry.getValue() > entry.getKey()) {
                    totalOnlineDuration += (entry.getValue() - entry.getKey());
                }
            }

            subMerchantAttendanceDetail.setTotalOnlineDuration(totalOnlineDuration);
        }

        /** 处理 挂起 */
        if (hangStartOverPair.size() > 0) {
            /** 处理当天未挂起结束的情况 **/
            if (NIL_VALUE.equals(hangStartOverPair.lastEntry().getValue())) {
                hangStartOverPair.put(hangStartOverPair.lastKey(), (int) (DateUtils.addDays(statisticsDate, 1).getTime() / 1000));
            }

            Iterator<Map.Entry<Integer, Integer>> entryIterator = hangStartOverPair.entrySet().iterator();
            int totalHangDuration = 0;

            while (entryIterator.hasNext()) {
                Map.Entry<Integer, Integer> entry = entryIterator.next();

                if (entry.getValue() > entry.getKey()) {
                    totalHangDuration += (entry.getValue() - entry.getKey());
                }
            }

            subMerchantAttendanceDetail.setTotalHangingDuration(totalHangDuration);
        }

        //处理离开事件相关统计
        if (leaveStartOverPair.size() > 0) {
            /** 处理当天未挂起结束的情况 **/
            if (NIL_VALUE.equals(leaveStartOverPair.lastEntry().getValue())) {
                leaveStartOverPair.put(leaveStartOverPair.lastKey(), (int) (DateUtils.addDays(statisticsDate, 1).getTime() / 1000));
            }

            Iterator<Map.Entry<Integer, Integer>> entryIterator = leaveStartOverPair.entrySet().iterator();
            int totalLeaveDuration = 0;

            while (entryIterator.hasNext()) {
                Map.Entry<Integer, Integer> entry = entryIterator.next();

                if (entry.getValue() > entry.getKey()) {
                    totalLeaveDuration += (entry.getValue() - entry.getKey());
                }
            }

            subMerchantAttendanceDetail.setTotalLeaveDuration(totalLeaveDuration);
            subMerchantAttendanceDetail.setTotalLeaveNum(leaveStartOverPair.size());
        }

        subMerchantAttendanceDetail.setTotalOfflineDuration(
                subMerchantAttendanceDetail.getLastOnlineTime() - subMerchantAttendanceDetail.getFirstOnlineTime()
                        - subMerchantAttendanceDetail.getTotalOnlineDuration());

        return subMerchantAttendanceDetail;
    }
}
