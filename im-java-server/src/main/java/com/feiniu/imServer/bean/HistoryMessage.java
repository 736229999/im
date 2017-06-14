package com.feiniu.imServer.bean;


/**
 * Created by wangchen on 2014/12/11.
 */
public class HistoryMessage implements Comparable<HistoryMessage>{
    private int id;
    private long time;
    private String msg;
    private MessageFeatures features=new MessageFeatures() ;
    private int msgid;
    private String nickName;

    @Override
    public int compareTo(HistoryMessage o) {
        long x = this.time;
        long y = o.time;

        return (x < y) ? -1 : ((x == y) ? 0 : 1);
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public long getTime() {
        return time;
    }

    public void setTime(long time) {
        this.time = time;
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

    public int getMsgid() {
        return msgid;
    }

    public void setMsgid(int msgid) {
        this.msgid = msgid;
    }

    public String getNickName() {
        return nickName;
    }

    public void setNickName(String nickName) {
        this.nickName = nickName;
    }
}
