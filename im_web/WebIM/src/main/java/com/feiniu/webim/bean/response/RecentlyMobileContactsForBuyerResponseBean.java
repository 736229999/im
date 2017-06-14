package com.feiniu.webim.bean.response;

import com.feiniu.webim.bean.domain.RecentlyMobileSeller;

import java.util.List;

/**
 * Created by wangyixiong on 2015/11/11.
 */
public class RecentlyMobileContactsForBuyerResponseBean extends BaseResponseBean {
    private int ret;
    private int id;
    private int msgnum;
    private List<RecentlyMobileSeller> list;

    public int getRet() {
        return ret;
    }

    public void setRet(int ret) {
        this.ret = ret;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getMsgnum() {
        return msgnum;
    }

    public void setMsgnum(int msgnum) {
        this.msgnum = msgnum;
    }

    public List<RecentlyMobileSeller> getList() {
        return list;
    }

    public void setList(List<RecentlyMobileSeller> list) {
        this.list = list;
    }
}
