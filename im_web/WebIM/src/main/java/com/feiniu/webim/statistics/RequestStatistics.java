package com.feiniu.webim.statistics;

import com.feiniu.webim.bean.request.BaseRequestBean;
import com.feiniu.webim.util.json.JSONUtil;
import com.feiniu.webim.util.log.Log;
import org.apache.log4j.Logger;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * Created by wangchen on 2014/12/31.
 */
public class RequestStatistics extends Statistics<BaseRequestBean> {
    Logger logger = Logger.getLogger(RequestStatistics.class);
    ConcurrentHashMap<Integer, AtomicInteger> requestTypeStatistics =
            new ConcurrentHashMap<>();
    private static String title = "统计发送给消息中心的请求====>>>>";

    private RequestStatistics(){}

    public static final void init(){
        registerStatistics(new RequestStatistics(), BaseRequestBean.class);
    }

    public void putStatistics(BaseRequestBean baseRequestBean, Object[] data) {
        AtomicInteger counter = requestTypeStatistics.get(baseRequestBean.getCmd());

        if (counter == null) {
            synchronized (this) {
                counter = requestTypeStatistics.get(baseRequestBean.getCmd());
                if (counter == null) {
                    counter = new AtomicInteger(0);
                    requestTypeStatistics.put(baseRequestBean.getCmd(), counter);
                }
            }
        }

        counter.addAndGet(1);
    }

    public void printStatisticsBody() {
        int totalRequest = 0;

        for (Map.Entry<Integer, AtomicInteger> entry : requestTypeStatistics.entrySet()) {
            totalRequest += entry.getValue().intValue();
        }

        logger.debug("总体发出去的消息的数量: " + totalRequest);
        logger.debug("每种类型发出去的消息数量: " + JSONUtil.toJsonString(requestTypeStatistics));
    }

    @Override
    public void printStaticsHeader() {
        Log.debug(logger, title);
    }
}
