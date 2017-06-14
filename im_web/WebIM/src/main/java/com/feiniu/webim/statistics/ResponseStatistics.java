package com.feiniu.webim.statistics;

import com.feiniu.webim.bean.response.BaseResponseBean;
import com.feiniu.webim.util.json.JSONUtil;
import com.feiniu.webim.util.log.Log;
import org.apache.log4j.Logger;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * Created by wangchen on 2014/12/31.
 */
public class ResponseStatistics extends Statistics<BaseResponseBean> {
    private static final Logger LOGGER = Logger.getLogger(ResponseStatistics.class);
    ConcurrentHashMap<Integer, AtomicInteger> responseTypeStatistics =
            new ConcurrentHashMap<>();
    private static final String title = "统计消息中心传送回来的经过解析的消息=======>>>>>";

    private ResponseStatistics() {
    }

    public static final void init(){
        registerStatistics(new ResponseStatistics(), BaseResponseBean.class);
    }

    public void putStatistics(BaseResponseBean baseResponseBean, Object[] data) {
        if (baseResponseBean != null) {
            AtomicInteger counter = responseTypeStatistics.get(baseResponseBean.getCmd());
            if (counter == null) {
                synchronized (this) {
                    counter = responseTypeStatistics.get(baseResponseBean.getCmd());
                    if (counter == null) {
                        counter = new AtomicInteger(0);
                        responseTypeStatistics.put(baseResponseBean.getCmd(), counter);
                    }
                }
            }

            counter.addAndGet(1);
        }
    }

    public void printStatisticsBody() {
        int totalRequest = 0;

        for (Map.Entry<Integer, AtomicInteger> entry : responseTypeStatistics.entrySet()) {
            totalRequest += entry.getValue().intValue();
        }

        Log.debug(LOGGER, "总体接收到的消息个数: " + totalRequest);
        Log.debug(LOGGER, "各个消息类型接收的消息的数量: " + JSONUtil.toJsonString(responseTypeStatistics));
    }

    @Override
    public void printStaticsHeader() {
        Log.debug(LOGGER, title);
    }
}
