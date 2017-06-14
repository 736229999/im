package com.cn.feiniu.logManagement.util.general;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by morningking on 2015/2/26.
 */
public class StringUtil {
    public static List<Integer> stringToIntegerList(String string, String seperator){
        String[] stringList = string.split(seperator);
        List<Integer> integerList = new ArrayList<Integer>();

        for(String s : stringList){
            integerList.add(Integer.parseInt(s));
        }

        return integerList;
    }

    public static List<Long> stringToLongList(String string, String seperator){
        String[] stringList = string.split(seperator);
        List<Long> longList = new ArrayList<Long>();

        for(String s : stringList){
            longList.add(Long.parseLong(s));
        }

        return longList;
    }

}
