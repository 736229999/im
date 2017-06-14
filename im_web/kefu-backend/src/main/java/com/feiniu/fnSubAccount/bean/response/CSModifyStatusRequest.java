package com.feiniu.fnSubAccount.bean.response;

/**
 * Created by wangzhen on 2015/11/23.
 */
public class CSModifyStatusRequest {
    String ids;
    Integer status;

    public String getIds() {
        return ids;
    }

    public void setIds(String ids) {
        this.ids = ids;
    }

    public Integer getStatus() {
        return status;
    }

    public void setStatus(Integer status) {
        this.status = status;
    }
}
