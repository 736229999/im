package com.feiniu.subAccount.bean.response;

/**
 * Created by wangzhen on 2015/7/22.
 */
public class WorkLoadDetailInfo extends BaseWorkLoadInfo implements Cloneable ,Comparable<WorkLoadDetailInfo>{

    private long sqlDate;

    private String date;

    @Override
    public Object clone() {
        WorkLoadDetailInfo cloneitem = (WorkLoadDetailInfo) super.clone();
        return cloneitem;
    }

    public long getSqlDate() {
        return sqlDate;
    }

    public void setSqlDate(long sqlDate) {
        this.sqlDate = sqlDate;
    }

    public String getDate() {
        return date;
    }

    public void setDate(String date) {
        this.date = date;
    }

    @Override
    public int compareTo(WorkLoadDetailInfo o) {
        return ((Long)(this.sqlDate- o.getSqlDate() )).intValue();
    }
}
