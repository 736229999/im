package com.feiniu.webim.bean.request;

/**
 * Created by wangchen on 2014/12/8.
 */
public class UserInfoRequestBean extends BaseRequestBean {
    private int fid;

    private UserInfoRequestBean(){}

    public static UserInfoRequestBean getMessage(){
        UserInfoRequestBean retrieveUserInfoRequestMessage = new UserInfoRequestBean();

        retrieveUserInfoRequestMessage.setFlag(0xf4);
        retrieveUserInfoRequestMessage.setFrom(1004);
        retrieveUserInfoRequestMessage.setCmd(0x0415);
        retrieveUserInfoRequestMessage.setDid(0);
        retrieveUserInfoRequestMessage.setDtype((byte) 1);
        retrieveUserInfoRequestMessage.setLen(0);

        return retrieveUserInfoRequestMessage;
    }

    public int getFid() {
        return fid;
    }

    public void setFid(int fid) {
        this.fid = fid;
    }
}
