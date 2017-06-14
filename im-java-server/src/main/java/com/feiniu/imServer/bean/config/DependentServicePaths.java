package com.feiniu.imServer.bean.config;

/**
 * 本项目依赖的外部服务的主机地址
 * Author: morningking
 * Date: 2016/2/25
 * Contact: 243717042@qq.com
 */

public class DependentServicePaths {
    private String queryReturnOrderServicePath;
    private String queryNewOrderServicePath;
    private String queryUserInfoServicePath;

    public String getOrderReturnServicePath() {
        return queryReturnOrderServicePath;
    }

    public void setOrderReturnServicePath(String orderReturnServicePath) {
        this.queryReturnOrderServicePath = orderReturnServicePath;
    }

    public String getQueryNewOrderServicePath() {
        return queryNewOrderServicePath;
    }

    public void setQueryNewOrderServicePath(String queryNewOrderServicePath) {
        this.queryNewOrderServicePath = queryNewOrderServicePath;
    }

    public String getQueryUserInfoServicePath() {
        return queryUserInfoServicePath;
    }

    public void setQueryUserInfoServicePath(String queryUserInfoServicePath) {
        this.queryUserInfoServicePath = queryUserInfoServicePath;
    }
}
