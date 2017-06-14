package com.feiniu.fnSubAccount.bean;

/**
 * Author: Du-Yong
 * Date: 2015/10/10
 * Contact: duy1102002@qq.com
 * 查询用户信息请求
 */
public class CSAccountQuery {
    private String username;    //哞哞用户名
    private String importname;  //导入人员
    private String realname;    //真实姓名
    private int status;         //账号状态
    private long begintime;      //开始时间
    private long  endtime;        //结束时间

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getImportname() {
        return importname;
    }

    public void setImportname(String importname) {
        this.importname = importname;
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

    public long getBegintime() {
        return begintime;
    }

    public void setBegintime(long begintime) {
        this.begintime = begintime;
    }

    public long getEndtime() {
        return endtime;
    }

    public void setEndtime(long endtime) {
        this.endtime = endtime;
    }
}
