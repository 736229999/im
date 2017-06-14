package com.cn.feiniu.logManagement.bean.dto;

/**
 * Author: morningking
 * Date: 2016/1/25
 * Contact: 243717042@qq.com
 */
public class ChatMessage {
    private String data;
    private int data_type;
    private int fontColor;
    private String fontName;
    private int fontSize;
    private int fontStyle;
    private int from_id;
    private String from_nickname;
    private int from_terminal;
    private int logintime;
    private int recv_flag;
    private int recv_time;
    private int send_time;
    private int to_id;
    private int to_terminal;
    private int ver;
    private String uuid;

    public String getData() {
        return data;
    }

    public void setData(String data) {
        this.data = data;
    }

    public int getData_type() {
        return data_type;
    }

    public void setData_type(int data_type) {
        this.data_type = data_type;
    }

    public int getFontColor() {
        return fontColor;
    }

    public void setFontColor(int fontColor) {
        this.fontColor = fontColor;
    }

    public String getFontName() {
        return fontName;
    }

    public void setFontName(String fontName) {
        this.fontName = fontName;
    }

    public int getFontSize() {
        return fontSize;
    }

    public void setFontSize(int fontSize) {
        this.fontSize = fontSize;
    }

    public int getFontStyle() {
        return fontStyle;
    }

    public void setFontStyle(int fontStyle) {
        this.fontStyle = fontStyle;
    }

    public int getFrom_id() {
        return from_id;
    }

    public void setFrom_id(int from_id) {
        this.from_id = from_id;
    }

    public String getFrom_nickname() {
        return from_nickname;
    }

    public void setFrom_nickname(String from_nickname) {
        this.from_nickname = from_nickname;
    }

    public int getFrom_terminal() {
        return from_terminal;
    }

    public void setFrom_terminal(int from_terminal) {
        this.from_terminal = from_terminal;
    }

    public int getLogintime() {
        return logintime;
    }

    public void setLogintime(int logintime) {
        this.logintime = logintime;
    }

    public int getRecv_flag() {
        return recv_flag;
    }

    public void setRecv_flag(int recv_flag) {
        this.recv_flag = recv_flag;
    }

    public int getRecv_time() {
        return recv_time;
    }

    public void setRecv_time(int recv_time) {
        this.recv_time = recv_time;
    }

    public int getSend_time() {
        return send_time;
    }

    public void setSend_time(int send_time) {
        this.send_time = send_time;
    }

    public int getTo_id() {
        return to_id;
    }

    public void setTo_id(int to_id) {
        this.to_id = to_id;
    }

    public int getTo_terminal() {
        return to_terminal;
    }

    public void setTo_terminal(int to_terminal) {
        this.to_terminal = to_terminal;
    }

    public int getVer() {
        return ver;
    }

    public void setVer(int ver) {
        this.ver = ver;
    }

    public String getUuid() {
        return uuid;
    }

    public void setUuid(String uuid) {
        this.uuid = uuid;
    }
}
