package com.feiniu.webim.bean.request;

/**
 * Created by wangyixiong on 2015/11/11.
 */
public class RecentyMobileContactsForBuyerRequestBean extends BaseRequestBean{
    private int id;
    private int msgnum;

    public void setId(int id) {
        this.id = id;
    }

    public int getId() {
        return id;
    }

    public int getMsgnum() {
        return msgnum;
    }

    public void setMsgnum(int msgnum) {
        this.msgnum = msgnum;
    }

    public static RecentyMobileContactsForBuyerRequestBean getMessage(){
        RecentyMobileContactsForBuyerRequestBean requestMessage = new RecentyMobileContactsForBuyerRequestBean();

        requestMessage.setVlink(0);
        requestMessage.setFlag(0xf4);
        requestMessage.setCmd(0x0965);
        requestMessage.setFrom(1002);
        requestMessage.setSeq(0);
        requestMessage.setDtype((byte) 1);
        requestMessage.setLen(0);
        requestMessage.setDid(0);
        requestMessage.setSid(0);

        return requestMessage;
    }
}
