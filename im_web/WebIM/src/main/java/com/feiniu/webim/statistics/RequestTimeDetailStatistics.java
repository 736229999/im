package com.feiniu.webim.statistics;

import com.feiniu.webim.bean.request.BaseRequestBean;
import com.feiniu.webim.util.log.Log;
import org.apache.log4j.Logger;

import javax.swing.border.TitledBorder;
import java.math.BigDecimal;
import java.math.RoundingMode;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicLong;

/**
 * Created by wangchen on 2015/1/22.
 */
public class RequestTimeDetailStatistics extends Statistics<RequestStatisticsBean>{
    private static final Logger LOGGER = Logger.getLogger(RequestTimeDetailStatistics.class);

    private ConcurrentHashMap<Integer, AtomicInteger> timedOutRequestMap
            = new ConcurrentHashMap<>();
    private ConcurrentHashMap<Integer, AtomicLong> totalConsumedTimeMap
            = new ConcurrentHashMap<>();
    ConcurrentHashMap<Integer, AtomicInteger> requestTypeStatistics =
            new ConcurrentHashMap<>();

    private static String title = "统计每种类型消息的平均响应时间和超时消息统计=====>>>>>";

    private RequestTimeDetailStatistics(){}
    public static final void init(){
        registerStatistics(new RequestTimeDetailStatistics(), RequestStatisticsBean.class);
    }

    @Override
    void putStatistics(RequestStatisticsBean object, Object[] data) {
        handleIndividualRequestStatistics(object, data);
        handleTotalRequestStatistics(object, data);
        handleTimeoutStatistics(object, data);
    }

    private void handleTimeoutStatistics(RequestStatisticsBean object, Object[] data){
        if (object.isTimedOut()) {//如果消息接收超时
            AtomicInteger timeoutCounter = timedOutRequestMap.get(object.getCmd());
            if (timeoutCounter == null) {
                synchronized (this) {
                    timeoutCounter = timedOutRequestMap.get(object.getCmd());
                    if (timeoutCounter == null) {
                        timeoutCounter = new AtomicInteger(0);
                        timedOutRequestMap.put(object.getCmd(), timeoutCounter);
                    }
                }
            }

            timeoutCounter.addAndGet(1);
        }
    }

    private void handleTotalRequestStatistics(RequestStatisticsBean object, Object[] data) {
        AtomicLong totalTimeCounter = totalConsumedTimeMap.get(object.getCmd());
        if (totalTimeCounter == null) {
            synchronized (this) {
                totalTimeCounter = totalConsumedTimeMap.get(object.getCmd());
                if (totalTimeCounter == null) {
                    totalTimeCounter = new AtomicLong(0);
                    totalConsumedTimeMap.put(object.getCmd(), totalTimeCounter);
                }
            }
        }

        totalTimeCounter.addAndGet(object.getReceivedTime() - object.getSendTime());
    }

    private void handleIndividualRequestStatistics(RequestStatisticsBean object, Object[] data) {
        AtomicInteger individualTimeCounter = requestTypeStatistics.get(object.getCmd());
        if (individualTimeCounter == null) {
            synchronized (this) {
                individualTimeCounter = requestTypeStatistics.get(object.getCmd());
                if (individualTimeCounter == null) {
                    individualTimeCounter = new AtomicInteger(0);
                    requestTypeStatistics.put(object.getCmd(), individualTimeCounter);
                }
            }
        }

        individualTimeCounter.addAndGet(1);
    }

    @Override
    public void printStatisticsBody() {
        StringBuilder builder = new StringBuilder();
        Log.debug(LOGGER, "超时消息统计：" + timedOutRequestMap);
        Log.debug(LOGGER, "消息类型数量统计：" + requestTypeStatistics);

        HashMap<Integer, BigDecimal> averageRequestTimeStatistics = new HashMap<>();
        for (Map.Entry<Integer, AtomicInteger> entry : requestTypeStatistics.entrySet()) {
            BigDecimal result = new BigDecimal(totalConsumedTimeMap.get(entry.getKey()).longValue());
            result = result.divide(new BigDecimal(entry.getValue().intValue()), 2, BigDecimal.ROUND_HALF_UP);

            averageRequestTimeStatistics.put(entry.getKey(), result);
        }
        Log.debug(LOGGER, "消息类型平均处理时长统计(ms)：" + averageRequestTimeStatistics);
    }

    @Override
    public void printStaticsHeader() {
        Log.debug(LOGGER, title);
    }
}
