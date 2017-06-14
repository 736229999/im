package com.feiniu.fnSubAccount.bean.response;

import com.feiniu.fnSubAccount.bean.StoreInfo;

import java.util.List;

/**
 * Created by wangchen on 2015/1/13.
 */
public class StoreInfoResponse {
    private int flag;
    private List<StoreInfo> datas;

    public int getFlag() {
        return flag;
    }

    public void setFlag(int flag) {
        this.flag = flag;
    }

    public List<StoreInfo> getDatas() {
        return datas;
    }

    public void setDatas(List<StoreInfo> datas) {
        this.datas = datas;
    }
}
