package com.feiniu.subAccount.constants;

import java.util.*;

/**
 * Created by wangchen on 2015/2/10.
 */
public final class WeekConstants {
    public static final Map<Character, String> charToContent = new HashMap<Character, String>();
    public static final List<Character> CHARACTER_LIST = new ArrayList<Character>();
    static {
        charToContent.put('1', "周一");
        charToContent.put('2', "周二");
        charToContent.put('3', "周三");
        charToContent.put('4', "周四");
        charToContent.put('5', "周五");
        charToContent.put('6', "周六");
        charToContent.put('7', "周日");

        CHARACTER_LIST.addAll(Arrays.asList('1', '2', '3', '4', '5', '6', '7'));
    }

    public static final String getRepresentDay(char indicator) {
        return charToContent.get(indicator);
    }
}
