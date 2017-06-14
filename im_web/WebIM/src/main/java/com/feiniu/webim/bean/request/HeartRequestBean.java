package com.feiniu.webim.bean.request;

/**
 * Created by wangchen on 2014/12/31.
 */
public class HeartRequestBean extends BaseRequestBean implements MessengeAware {
    private long ip;
    private int port;
    private int id;

    public long getIp() {
        return ip;
    }

    public void setIP(long ip) {
        this.ip = ip;
    }

    public int getPort() {
        return port;
    }

    public void setPort(int port) {
        this.port = port;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public static final HeartRequestBean getMessage(){
        HeartRequestBean heartRequestBean = new HeartRequestBean();
        heartRequestBean.setCmd(769);
        heartRequestBean.setFlag(244);
        heartRequestBean.setVlink(0);
        heartRequestBean.setFrom(1001);
        heartRequestBean.setDtype((byte)1);

        return heartRequestBean;
    }
}
