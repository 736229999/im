package com.feiniu.subAccount.constants;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by wangchen on 2015/2/12.
 */
public class ServiceAppraiseConstants {
    public static final Map<Integer, String> charToContent = new HashMap<Integer, String>();
    static {
        charToContent.put(0, "未评估");
        charToContent.put(1, "非常不满意");
        charToContent.put(2, "不满意");
        charToContent.put(3, "一般");
        charToContent.put(4, "满意");
        charToContent.put(5, "非常满意");
    }

}
