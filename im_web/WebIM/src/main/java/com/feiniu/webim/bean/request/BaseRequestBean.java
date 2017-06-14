package com.feiniu.webim.bean.request;

import com.fasterxml.jackson.annotation.JsonIgnoreType;

/**
 * Created by wangchen on 2014/12/8.
 * 公共请求消息数据结构
 */
@JsonIgnoreType
public class BaseRequestBean{
    int vlink;
    int flag;  //1  MINI139包标识 0xF0服务器主动发包，0xF1服务器应答  0xF2客户端请求 0xF3客户端应答
    int from;  //2  来源,对客户端来说是版本号
    int cmd;   //3 命令号
    int seq;   //4 命令序列号
    int sid;   //5 源ID (详细说明见协议文档)
    byte dtype;// 6 目标类型 (0x01 用户，0x02 家族，0x03 中转会话成员)
    int did;   //7 目标ID(详细说明见协议文档)
    int len;  //8 包体长度
    long deviceid;

    public int getVlink() {
        return vlink;
    }

    public void setVlink(int vlink) {
        this.vlink = vlink;
    }

    public int getFlag() {
        return flag;
    }

    public void setFlag(int flag) {
        this.flag = flag;
    }

    public int getFrom() {
        return from;
    }

    public void setFrom(int from) {
        this.from = from;
    }

    public int getCmd() {
        return cmd;
    }

    public void setCmd(int cmd) {
        this.cmd = cmd;
    }

    public int getSeq() {
        return seq;
    }

    public void setSeq(int seq) {
        this.seq = seq;
    }

    public int getSid() {
        return sid;
    }

    public void setSid(int sid) {
        this.sid = sid;
    }

    public byte getDtype() {
        return dtype;
    }

    public void setDtype(byte dtype) {
        this.dtype = dtype;
    }

    public int getDid() {
        return did;
    }

    public void setDid(int did) {
        this.did = did;
    }

    public int getLen() {
        return len;
    }

    public void setLen(int len) {
        this.len = len;
    }

    public long getDeviceid() {
        return deviceid;
    }

    public void setDeviceid(long deviceid) {
        this.deviceid = deviceid;
    }
}
