package com.feiniu.webim.bean.request;

import com.feiniu.webim.bean.domain.MessageFeatures;
import com.feiniu.webim.constants.Constants;

/**
 * Created by wangchen on 2014/12/8.
 */
public class PutMessageRequestBean extends BaseRequestBean {
    private int sender;
    private int recver;
    private int type;
    private String msg;
    private MessageFeatures features;
    private String nickname;
    private String toUserName;
    private String storeName;
    private long time;
    private Integer version = 0;

    public PutMessageRequestBean(){
        this.setVlink(0);
        this.setFrom(1002);
        this.setFlag(0xf4);
        this.setCmd(0x60b);
        this.setDtype((byte) 1);
        this.setLen(0);
    }

    public int getSender() {
        return sender;
    }

    public void setSender(int sender) {
        this.sender = sender;
    }

    public int getRecver() {
        return recver;
    }

    public void setRecver(int recver) {
        this.recver = recver;
    }

    public int getType() {
        return type;
    }

    public void setType(int type) {
        this.type = type;
    }

    public String getMsg() {
        return msg;
    }

    public void setMsg(String msg) {
        this.msg = msg;
    }

    public MessageFeatures getFeatures() {
        return features;
    }

    public void setFeatures(MessageFeatures features) {
        this.features = features;
    }

    public String getNickname() {
        return nickname;
    }

    public void setNickname(String nickname) {
        this.nickname = nickname;
    }

    public long getTime() {
        return time / Constants.MSG_TIME_TRANSFORM;
    }

    public void setTime(long time) {
        this.time = time;
    }

    public int getVersion() {
        return version;
    }

    public void setVersion(int version) {
        this.version = version;
    }
}
