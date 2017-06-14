package com.feiniu.webim.bean.web.request;

import com.feiniu.webim.core.Session;

/**
 * Created by wangchen on 2015/3/10.
 */
public class GetSubMerchantWebRequest extends BaseWebRequest {
    private Integer pid = 0;
    private Integer sid = 0;
    private Integer diverse_type = 0;
    private Integer sub_merchant_id = -1;
    private Integer group_id = 0;
    private String nickName;
    private String userLevel;
    private Integer fromDeviceType;
    private Integer fromPageType;

    public GetSubMerchantWebRequest() {
        super();
    }

    public GetSubMerchantWebRequest(Session session) {
        super(session);
    }

    public Integer getPid() {
        return pid;
    }

    public void setPid(Integer pid) {
        this.pid = pid;
    }

    public Integer getSid() {
        return sid;
    }

    public void setSid(Integer sid) {
        this.sid = sid;
    }

    public Integer getDiverse_type() {
        return diverse_type;
    }

    public void setDiverse_type(Integer diverse_type) {
        this.diverse_type = diverse_type;
    }

    public Integer getSub_merchant_id() {
        return sub_merchant_id;
    }

    public void setSub_merchant_id(Integer sub_merchant_id) {
        this.sub_merchant_id = sub_merchant_id;
    }

    public Integer getGroup_id() {
        return group_id;
    }

    public void setGroup_id(Integer group_id) {
        this.group_id = group_id;
    }

    public String getNickName() {
        return nickName;
    }

    public void setNickName(String nickName) {
        this.nickName = nickName;
    }

    public String getUserLevel() {
        return userLevel;
    }

    public void setUserLevel(String userLevel) {
        this.userLevel = userLevel;
    }

    public Integer getFromDeviceType() {
        return fromDeviceType;
    }

    public void setFromDeviceType(Integer fromDeviceType) {
        this.fromDeviceType = fromDeviceType;
    }

    public Integer getFromPageType() {
        return fromPageType;
    }

    public void setFromPageType(Integer fromPageType) {
        this.fromPageType = fromPageType;
    }
}
