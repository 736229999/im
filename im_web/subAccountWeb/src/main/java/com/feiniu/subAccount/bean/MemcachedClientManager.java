package com.feiniu.subAccount.bean;


import net.spy.memcached.AddrUtil;
import net.spy.memcached.MemcachedClient;

import java.io.IOException;

/**
 * Created by wangzhen on 2015/10/30.
 */
public class MemcachedClientManager {

    private String str;

    public String getStr() {
        return str;
    }

    public void setStr(String str) {
        this.str = str;
    }

    public MemcachedClient getMClient() throws IOException{
        return new MemcachedClient(AddrUtil.getAddresses(str));
    }
}
