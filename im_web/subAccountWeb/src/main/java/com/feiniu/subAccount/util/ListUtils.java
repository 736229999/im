package com.feiniu.subAccount.util;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by wangchen on 2015/2/6.
 */
public class ListUtils {
    public static <T extends Number> List<T> transformElementToNumber(List list, Class<T> clazz) {
        List<T> transformedList = new ArrayList<T>();

        for (Object object : list) {
            if (clazz == Integer.class) {
                transformedList.add((T)((Integer)Integer.parseInt(object.toString())));
            } else if (clazz == Float.class) {
                transformedList.add((T)((Float)Float.parseFloat(object.toString())));
            } else if (clazz == Byte.class){
                transformedList.add((T)((Byte)Byte.parseByte(object.toString())));
            } else if (clazz == Long.class) {
                transformedList.add((T)(Long)Long.parseLong(object.toString()));
            }
        }

        return transformedList;
    }
}
