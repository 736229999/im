package com.feiniu.fnSubAccount.bean;

/**
 * Author: Du-Yong
 * Date: 2015/10/11
 * Contact: duy1102002@qq.com
 * 从imserver返回的数据
 */
public class CSAccountQueryReply {
    private String username;
    private int permission;
    private String importname;
    private long importtime;
    private String realname;
    private int status;
    private int receptionlimit;
    private int pageSize;
    private int currentPageNo;

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

    public String getImportname() {
        return importname;
    }

    public void setImportname(String importname) {
        this.importname = importname;
    }

    public long getImporttime() {
        return importtime;
    }

    public void setImporttime(long importtime) {
        this.importtime = importtime;
    }

    public String getRealname() {
        return realname;
    }

    public void setRealname(String realname) {
        this.realname = realname;
    }

    public int getStatus() {
        return status;
    }

    public void setStatus(int status) {
        this.status = status;
    }

    public int getReceptionlimit() {
        return receptionlimit;
    }

    public void setReceptionlimit(int receptionlimit) {
        this.receptionlimit = receptionlimit;
    }

    public int getPageSize() {
        return pageSize;
    }

    public void setPageSize(int pageSize) {
        this.pageSize = pageSize;
    }

    public int getCurrentPageNo() {
        return currentPageNo;
    }

    public void setCurrentPageNo(int currentPageNo) {
        this.currentPageNo = currentPageNo;
    }
}
