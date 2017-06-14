package com.feiniu.fnSubAccount.bean;

import java.util.List;

/**
 * Author: Du-Yong
 * Date: 2015/10/9
 * Contact: duy1102002@qq.com
 */
public class CSModiAddUserReq {
    private String importname;              //导入人员
    private Long importCsId;
    List< CSModiAdduserinf> userinfo;

    public class CSModiAdduserinf{
        private String username;           //哞哞登录名
        private int permission;             //主管:0 员工:1
        private int receptionlimit;         //接待上限

        public String getUsername() {
            return username;
        }

        public void setUsername(String username) {
            this.username = username;
        }

        public int getPermission() {
            return permission;
        }

        public void setPermission(int permission) {
            this.permission = permission;
        }

        public int getReceptionlimit() {
            return receptionlimit;
        }

        public void setReceptionlimit(int receptionlimit) {
            this.receptionlimit = receptionlimit;
        }
    }

    public String getImportname() {
        return importname;
    }

    public void setImportname(String importname) {
        this.importname = importname;
    }

    public Long getImportCsId() {
        return importCsId;
    }

    public void setImportCsId(Long importCsId) {
        this.importCsId = importCsId;
    }

    public List<CSModiAdduserinf> getUserinfo() {
        return userinfo;
    }

    public void setUserinfo(List<CSModiAdduserinf> userinfo) {
        this.userinfo = userinfo;
    }
}
