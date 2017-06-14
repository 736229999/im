package com.feiniu.imServer.bean;

/**
 * Created by wangzhen on 2016/2/1.
 */
public class RecentConnectInfo {
    private Long id;
    //    private Long state;
    private String nickname;
    private String headImgUrl;
    private Long time;
    private String userSign;
    private String lastMsg;

    protected void cloneFromAnOther(RecentConnectInfo index) {
        id = index.getId();
        nickname = index.getNickname();
        headImgUrl = index.getHeadImgUrl();
        time = index.getTime();
        lastMsg = index.getLastMsg();
        userSign = index.getUserSign();
    }


    public String getLastMsg() {
        return lastMsg;
    }

    public void setLastMsg(String lastMsg) {
        this.lastMsg = lastMsg;
    }

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }
//
//    public Long getState() {
//        return state;
//    }
//
//    public void setState(Long state) {
//        this.state = state;
//    }


    public String getNickname() {
        return nickname;
    }

    public void setNickname(String nickname) {
        this.nickname = nickname;
    }

    public String getHeadImgUrl() {
        return headImgUrl;
    }

    public void setHeadImgUrl(String headImgUrl) {
        this.headImgUrl = headImgUrl;
    }

    public Long getTime() {
        return time;
    }

    public void setTime(Long time) {
        this.time = time;
    }

    public String getUserSign() {
        return userSign;
    }

    public void setUserSign(String userSign) {
        this.userSign = userSign;
    }
}
