package com.feiniu.webim.bean.request;

/**
 * Created by wangchen on 2014/12/16.
 */
public class RecentylContactsForBuyerRequestBean extends BaseRequestBean{
    private int id;
    private int msgnum;

    public int getId() {
        return id;
    }

    public static RecentylContactsForBuyerRequestBean getMessage(){
        RecentylContactsForBuyerRequestBean requestMessage = new RecentylContactsForBuyerRequestBean();

        requestMessage.setVlink(0);
        requestMessage.setFlag(0xf4);
        requestMessage.setCmd(0x0618);
        requestMessage.setFrom(1002);
        requestMessage.setSeq(0);
        requestMessage.setDtype((byte) 1);
        requestMessage.setLen(0);
        requestMessage.setDid(0);
        requestMessage.setSid(0);

        return requestMessage;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getMsgnum() {
        return msgnum;
    }

    public void setMsgnum(int msgnum) {
        this.msgnum = msgnum;
    }
}
