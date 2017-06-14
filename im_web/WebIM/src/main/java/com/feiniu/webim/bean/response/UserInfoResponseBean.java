package com.feiniu.webim.bean.response;

import com.feiniu.webim.bean.domain.UserInfo;

/**
 * Created by wangchen on 2014/12/8.
 */
public class UserInfoResponseBean extends BaseResponseBean {
    private UserInfo userInfo;

    public UserInfo getUserInfo() {
        return userInfo;
    }

    public void setUserInfo(UserInfo userInfo) {
        this.userInfo = userInfo;
    }
}
