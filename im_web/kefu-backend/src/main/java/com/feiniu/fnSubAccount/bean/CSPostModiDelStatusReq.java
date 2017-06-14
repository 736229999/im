package com.feiniu.fnSubAccount.bean;

import java.util.List;

/**
 * Author: Du-Yong
 * Date: 2015/10/11
 * Contact: duy1102002@qq.com
 */
public class CSPostModiDelStatusReq {
    List< CSModiDelinf> csstatus;
    public class CSModiDelinf{
        private String username;           //哞哞登录名
        private int status;             //状态请求  0 启用 1 停用 2 删除

        public String getUsername() {
            return username;
        }

        public void setUsername(String username) {
            this.username = username;
        }

        public int getStatus() {
            return status;
        }

        public void setStatus(int status) {
            this.status = status;
        }
    }

    public List<CSModiDelinf> getCsstatus() {
        return csstatus;
    }

    public void setCsstatus(List<CSModiDelinf> csstatus) {
        this.csstatus = csstatus;
    }
}
