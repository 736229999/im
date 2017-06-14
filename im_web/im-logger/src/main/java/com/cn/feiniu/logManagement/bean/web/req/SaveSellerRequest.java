package com.cn.feiniu.logManagement.bean.web.req;

/**
 * Created by zhangmin on 2016/2/29.
 */
public class SaveSellerRequest {
    private Integer userId;
    private String userName;
    private String password;
    private String trueName;

    public Integer getUserId() {
        return userId;
    }

    public void setUserId(Integer userId) {
        this.userId = userId;
    }

    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public String getTrueName() {
        return trueName;
    }

    public void setTrueName(String trueName) {
        this.trueName = trueName;
    }
}
