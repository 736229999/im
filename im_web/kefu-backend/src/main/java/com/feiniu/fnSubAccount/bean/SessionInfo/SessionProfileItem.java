package com.feiniu.fnSubAccount.bean.SessionInfo;

/**
 * Created by wangzhen on 2016/4/6.
 */
public class SessionProfileItem {
    String title;
    Integer typeSql;
    Integer count;

    public Integer getTypeSql() {
        return typeSql;
    }

    public void setTypeSql(Integer typeSql) {
        this.typeSql = typeSql;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public Integer getCount() {
        return count;
    }

    public void setCount(Integer count) {
        this.count = count;
    }
}
