package com.feiniu.webim.bean.request;

/**
 * Created by wangchen on 2014/12/11.
 */
public class HistoryListRequestBean extends BaseRequestBean {
    private int sid;
    private int fid;  //isMerchant为0时：此字段标识子账号id；为1时，此字段标识商家id
    private int msgnum;
    private long time;
    private int msgid;
    private String token;
    private String uuid;
    private Integer isMerchant = 0;

    public HistoryListRequestBean() {
        this.setVlink(0);
        this.setFlag(0xf4);
        this.setCmd(0x0616);
        this.setFrom(1002);
        this.setSeq(0);
        this.setDtype((byte) 0);
        this.setLen(0);
        this.setDid(0);
    }

    @Override
    public int getSid() {
        return sid;
    }

    @Override
    public void setSid(int sid) {
        this.sid = sid;
    }

    public int getFid() {
        return fid;
    }

    public void setFid(int fid) {
        this.fid = fid;
    }

    public int getMsgnum() {
        return msgnum;
    }

    public void setMsgnum(int msgnum) {
        this.msgnum = msgnum;
    }

    public long getTime() {
        return time;
    }

    public void setTime(long time) {
        this.time = time;
    }

    public int getMsgid() {
        return msgid;
    }

    public void setMsgid(int msgid) {
        this.msgid = msgid;
    }

    public String getToken() {
        return token;
    }

    public void setToken(String token) {
        this.token = token;
    }

    public String getUuid() {
        return uuid;
    }

    public void setUuid(String uuid) {
        this.uuid = uuid;
    }

    public Integer getIsMerchant() {
        return isMerchant;
    }

    public void setIsMerchant(Integer isMerchant) {
        this.isMerchant = isMerchant;
    }
}
