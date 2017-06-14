package com.feiniu.webim.bean.request;

import com.feiniu.webim.constants.Constants;

/**
 * Created by wangchen on 2014/12/8.
 */
public class JoinRequestBean extends BaseRequestBean implements MessengeAware {
    private String username;
    private String random_key;
    private String password;
    private int status;
    private long lanip;
    private int lanport;
    private byte login_flag;
    private String sessionID;
    private String mobilephoneno;
    private String email;

    public JoinRequestBean(){
        this.setVlink(0);
        this.setFlag(0xf4);
        this.setCmd(0x211);
        this.setFrom(Constants.SERVER_VERSION);
        this.setSeq(0);
        this.setSid(0);
        this.setDtype((byte) 1);
        this.setLen(0);
        this.setDid(0);
        this.setSid(0);
    }

    public String getUsername() {
        return username;
    }

    public JoinRequestBean setUsername(String username) {
        this.username = username;

        return this;
    }

    public String getRandom_key() {
        return random_key;
    }

    public JoinRequestBean setRandom_key(String random_key) {
        this.random_key = random_key;

        return this;
    }

    public String getPassword() {
        return password;
    }

    public JoinRequestBean setPassword(String password) {
        this.password = password;

        return this;
    }

    public int getStatus() {
        return status;
    }

    public JoinRequestBean setStatus(int status) {
        this.status = status;

        return this;
    }

    public long getLanip() {
        return lanip;
    }

    @Override
    public void setIP(long lanip) {
        this.lanip = lanip;
    }

    public int getLanport() {
        return lanport;
    }

    @Override
    public void setPort(int lanport) {
        this.lanport = lanport;
    }

    public byte getLogin_flag() {
        return login_flag;
    }

    public JoinRequestBean setLogin_flag(byte login_flag) {
        this.login_flag = login_flag;

        return this;
    }

    public String getSessionID() {
        return sessionID;
    }

    public void setSessionID(String sessionID) {
        this.sessionID = sessionID;
    }

    public String getMobilephoneno() {
        return mobilephoneno;
    }

    public void setMobilephoneno(String mobilephoneno) {
        this.mobilephoneno = mobilephoneno;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }
}
