package com.feiniu.webim.util.general;

import org.apache.commons.lang.StringUtils;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by wangchen on 2015/2/26.
 */
public class StringUtil {
    public static List<Integer> stringToIntegerList(String string, String seperator) {
        String[] stringList = string.split(seperator);
        List<Integer> integerList = new ArrayList<Integer>();

        for (String s : stringList) {
            integerList.add(Integer.parseInt(s));
        }

        return integerList;
    }

    public static List<Long> stringToLongList(String string, String seperator) {
        String[] stringList = string.split(seperator);
        List<Long> longList = new ArrayList<Long>();

        for (String s : stringList) {
            longList.add(Long.parseLong(s));
        }

        return longList;
    }

    //手机正则
    private static final String TELEPHONE_REGEXP = "^((?:13|15|18)\\d)(?:\\d{4})(\\d{4})$";

    /**
     * 隐藏手机号中间四位
     *
     * @param telphone
     * @return
     */
    public static final String makeTelephoneSecure(String telphone) {
        if (StringUtils.isNotEmpty(telphone)) {
            telphone = telphone.replaceAll(TELEPHONE_REGEXP, "$1****$2");
        }

        return telphone;
    }
}
