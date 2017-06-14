package com.feiniu.imServer.bean.ImServer;

/**
 * Created by wangzhen on 2015/12/30.
 */
public class ImServerSubAccountInfo {
    private long BuyerId;
    private String Email;
    private String FaceUrl;
    private Long MerchantId;
    private String Mobile;
    private String NickName;
    private String UserName;
    private String UserSign;

    public long getBuyerId() {
        return BuyerId;
    }

    public void setBuyerId(long buyerId) {
        BuyerId = buyerId;
    }

    public String getEmail() {
        return Email;
    }

    public void setEmail(String email) {
        Email = email;
    }

    public String getFaceUrl() {
        return FaceUrl;
    }

    public void setFaceUrl(String faceUrl) {
        FaceUrl = faceUrl;
    }

    public Long getMerchantId() {
        return MerchantId;
    }

    public void setMerchantId(Long merchantId) {
        MerchantId = merchantId;
    }

    public String getMobile() {
        return Mobile;
    }

    public void setMobile(String mobile) {
        Mobile = mobile;
    }

    public String getNickName() {
        return NickName;
    }

    public void setNickName(String nickName) {
        NickName = nickName;
    }

    public String getUserName() {
        return UserName;
    }

    public void setUserName(String userName) {
        UserName = userName;
    }

    public String getUserSign() {
        return UserSign;
    }

    public void setUserSign(String userSign) {
        UserSign = userSign;
    }
}
