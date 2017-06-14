package com.feiniu.webim.bean.response;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.feiniu.webim.bean.domain.RecentlySeller;

import java.util.List;

/**
 * Created by wangchen on 2014/12/16.
 */
public class RecentlyContactsForBuyerResponseBean extends BaseResponseBean {
    private int ret;
    private int id;
    private int msgnum;
    private List<RecentlySeller> list;

    @JsonIgnore
    public int getRet() {
        return ret;
    }

    public void setRet(int ret) {
        this.ret = ret;
    }

    @JsonIgnore
    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public void setMsgnum(int msgnum) {
        this.msgnum = msgnum;
    }

    public List<RecentlySeller> getList() {
        return list;
    }

    public void setList(List<RecentlySeller> list) {
        this.list = list;
    }
}
