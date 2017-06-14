package com.feiniu.fnSubAccount.bean.response;

import com.feiniu.fnSubAccount.bean.CSBackendInfo;

import java.util.List;

/**
 * Created by wangzhen on 2015/10/12.
 */
public class BackOperatersByOrganIdsResponse {
    private List<CSBackendInfo> data;
    private int code;

    public List<CSBackendInfo> getData() {
        return data;
    }

    public void setData(List<CSBackendInfo> data) {
        this.data = data;
    }

    public int getCode() {
        return code;
    }

    public void setCode(int code) {
        this.code = code;
    }
}
