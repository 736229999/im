package com.feiniu.imServer.bean;

/**
 * Created by wangzhen on 2016/2/3.
 */
public class RecentConnectInfoShow extends RecentConnectInfo {
    private Integer state;
    //webim接口需要小写的usersign
    private String usersign;

    public RecentConnectInfoShow(RecentConnectInfo pbase) {
        cloneFromAnOther(pbase);
        usersign = pbase.getUserSign();
    }

    public String getUsersign() {
        return usersign;
    }

    public void setUsersign(String usersign) {
        this.usersign = usersign;
    }

    public Integer getState() {
        return state;
    }

    public void setState(Integer state) {
        this.state = state;
    }
}
