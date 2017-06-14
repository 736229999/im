package com.feiniu.subAccount.util;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by morningking on 2014/11/21.
 */
public class StringUtils {
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
