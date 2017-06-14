package com.feiniu.webim.bean.domain;

/**
 * Created by wangchen on 2014/12/16.
 */
public class RecentlySeller implements Comparable<RecentlySeller> {
    //正在联系人取值
    public static final int IS_CURRENT_CHATTING_SELLER = 1;
    //最近联系人取值
    public static final int IS_RECENTLY_SELLER = 0;

    private int id;
    private String nickname;
    private int state;
    private int time;
    private int tag;
    private long skuID;
    private String headImgUrl;
    private String usersign = "";

    @Override
    public int compareTo(RecentlySeller o) {
        if (o == null) {
            throw new NullPointerException();
        }

        return o.state != this.state ?
                o.state - this.state :
                this.time != o.time ?
                        this.time - o.time :
                        0;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        RecentlySeller seller = (RecentlySeller) o;

        return id == seller.id;

    }

    @Override
    public int hashCode() {
        return id;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getNickname() {
        return nickname;
    }

    public void setNickname(String nickname) {
        this.nickname = nickname;
    }

    public int getState() {
        return state;
    }

    public void setState(int state) {
        this.state = state;
    }

    public int getTime() {
        return time;
    }

    public void setTime(int time) {
        this.time = time;
    }

    public int getTag() {
        return tag;
    }

    public void setTag(int tag) {
        this.tag = tag;
    }

    public long getSkuID() {
        return skuID;
    }

    public void setSkuID(long skuID) {
        this.skuID = skuID;
    }

    public String getHeadImgUrl() {
        return headImgUrl;
    }

    public void setHeadImgUrl(String headImgUrl) {
        this.headImgUrl = headImgUrl;
    }

    public String getUsersign() {
        return usersign;
    }

    public void setUsersign(String usersign) {
        this.usersign = usersign;
    }
}
