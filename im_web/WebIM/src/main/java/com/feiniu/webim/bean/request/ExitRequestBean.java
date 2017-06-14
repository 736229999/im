package com.feiniu.webim.bean.request;

/**
 * Created by wangchen on 2014/12/8.
 */
public class ExitRequestBean extends BaseRequestBean {
    private int id;
    private byte authData;

    private ExitRequestBean(){}

    public static ExitRequestBean getMessage(){
        ExitRequestBean exitRequestMessage = new ExitRequestBean();

        exitRequestMessage.setVlink(0);
        exitRequestMessage.setFlag(0xf4);
        exitRequestMessage.setFrom(1002);
        exitRequestMessage.setCmd(0x203);
        exitRequestMessage.setDtype((byte)1);
        exitRequestMessage.setDid(0);
        exitRequestMessage.setLen(0);
        exitRequestMessage.setSeq(0);

        return exitRequestMessage;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public byte getAuthData() {
        return authData;
    }

    public void setAuthData(byte authData) {
        this.authData = authData;
    }
}
