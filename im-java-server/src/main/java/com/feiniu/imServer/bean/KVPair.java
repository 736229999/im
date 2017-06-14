package com.feiniu.imServer.bean;

/**
 * Author: wangchen
 * Date: 2015/6/29
 * Contact: 243717042@qq.com
 */
public class KVPair<K, V> {
    private K key;
    private V value;

    public KVPair() {
        this(null, null);
    }

    public KVPair(K key, V value) {
        this.key = key;
        this.value = value;
    }

    public K getKey() {
        return key;
    }

    public void setKey(K key) {
        this.key = key;
    }

    public V getValue() {
        return value;
    }

    public void setValue(V value) {
        this.value = value;
    }
}
