package com.feiniu.fnSubAccount.domain;

import org.codehaus.jackson.annotate.JsonIgnore;

import javax.persistence.*;

/**
 * Created by wangchen on 2015/2/4.
 */
@Entity
@Table(name = "IM_WEBDATA")
public class ChatMessage {
    @Id
    @Column(name = "msgid", nullable = false)
    private long messageID;

    @OneToOne
    @JoinColumn(name = "fromid", nullable = false)
    private User fromUser;

    @OneToOne
    @JoinColumn(name = "TOID", nullable = false)
    private User toUser;

    @Column(name = "DATATYPE", nullable = false)
    @JsonIgnore
    private int dataType;

    @Column(name = "VERSION", nullable = false)
    @JsonIgnore
    private int version;

    @Column(name = "SENDTIME", nullable = false)
    private int sendTime;

    @Column(name = "RECVFLAG", nullable = false)
    @JsonIgnore
    private int recvFlag;

    @Column(name = "fontsize", nullable = false)
    private int fontSize;

    @Column(name = "FONTCOLOR", nullable = false)
    private int fontColor;

    @Column(name = "FONTSTYLE", nullable = false)
    private int fontStyle;

    @Column(name = "FONTNAME", nullable = false)
    private String fontName;

    @Column(name = "FROMNICKNAME", nullable = false)
    private String fromNickName;

    @Column(name = "MSG", nullable = false)
    private String msg;

    @Column(name = "LOGINTIME", nullable = true)
    private long loginTime;

    public long getMessageID() {
        return messageID;
    }

    public void setMessageID(long messageID) {
        this.messageID = messageID;
    }

    public User getFromUser() {
        return fromUser;
    }

    public void setFromUser(User fromUser) {
        this.fromUser = fromUser;
    }

    public User getToUser() {
        return toUser;
    }

    public void setToUser(User toUser) {
        this.toUser = toUser;
    }

    public int getDataType() {
        return dataType;
    }

    public void setDataType(int dataType) {
        this.dataType = dataType;
    }

    public int getVersion() {
        return version;
    }

    public void setVersion(int version) {
        this.version = version;
    }

    public int getSendTime() {
        return sendTime;
    }

    public void setSendTime(int sendTime) {
        this.sendTime = sendTime;
    }

    public int getRecvFlag() {
        return recvFlag;
    }

    public void setRecvFlag(int recvFlag) {
        this.recvFlag = recvFlag;
    }

    public int getFontSize() {
        return fontSize;
    }

    public void setFontSize(int fontSize) {
        this.fontSize = fontSize;
    }

    public int getFontColor() {
        return fontColor;
    }

    public void setFontColor(int fontColor) {
        this.fontColor = fontColor;
    }

    public int getFontStyle() {
        return fontStyle;
    }

    public void setFontStyle(int fontStyle) {
        this.fontStyle = fontStyle;
    }

    public String getFontName() {
        return fontName;
    }

    public void setFontName(String fontName) {
        this.fontName = fontName;
    }

    public String getFromNickName() {
        return fromNickName;
    }

    public void setFromNickName(String fromNickName) {
        this.fromNickName = fromNickName;
    }

    public String getMsg() {
        return msg;
    }

    public void setMsg(String msg) {
        this.msg = msg;
    }

    public long getLoginTime() {
        return loginTime;
    }

    public void setLoginTime(long loginTime) {
        this.loginTime = loginTime;
    }
}
