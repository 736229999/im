package com.feiniu.webim.statistics;


import java.util.HashMap;

/**
 * Created by wangchen on 2014/12/31.
 */
@SuppressWarnings("unchecked")
public abstract class Statistics<T> {
    private static final HashMap<Class, Statistics> META_MAPPING = new HashMap<>();

    public static final Statistics getStatictics(Class clazz) {
        return META_MAPPING.get(clazz);
    }

    protected static final <E> void registerStatistics(Statistics<E> statistics, Class<E> statiticsSource) {
        META_MAPPING.put(statiticsSource, statistics);
    }

    public static final void printAllStatistics() {
        for (Statistics statistics : META_MAPPING.values()){
            statistics.printStaticsHeader();
            statistics.printStatisticsBody();
        }
    }

    public static <E> void putStaticsSource(Class clazz, E object, Object[] data) {
        getStatictics(clazz).putStatistics(object, data);
    }

    abstract void putStatistics(T object, Object[] data);

    abstract void printStatisticsBody();

    abstract void printStaticsHeader();
}
