package com.feiniu.fnSubAccount.bean;

/**
 * Author: morningking
 * Date: 2015/9/29
 * Contact: 243717042@qq.com
 */
public class CSAccountRequest {
    private String CSName;           //哞哞登录名
    private String CSAccountAuthor;     //哞哞账号权限
    private String ImportUser;       //导入人员
    private String ImportTime;       //导入时间
    private String ImportTeam;       //导入人员组织
    private String CSUserName;       //账号所属人员
    private String CSTeam;           //账号所属组织
    private String CSPassword;      //账号密码
    private String Starttime;
    private String Endtime;
    private String rows;        //请求修改的用户名列表
    private int status;         //请求类型
    private int AccountStates;       //状态
    private int AccountLimits;       //接待上限
    private int AccountOperatorable;//操作
    private int startCount;
    private int endCount;
    private int pageSize;
    private int currentPageNo;

    public String getRows() {
        return rows;
    }

    public void setRows(String rows) {
        this.rows = rows;
    }

    public int getStatus() {
        return status;
    }

    public void setStatus(int status) {
        this.status = status;
    }

    public String getStarttime() {
        return Starttime;
    }

    public void setStarttime(String starttime) {
        Starttime = starttime;
    }

    public String getEndtime() {
        return Endtime;
    }

    public void setEndtime(String endtime) {
        Endtime = endtime;
    }

    public String getCSName() {
        return CSName;
    }

    public void setCSName(String CSName) {
        this.CSName = CSName;
    }

    public String getCSAccountAuthor() {
        return CSAccountAuthor;
    }

    public void setCSAccountAuthor(String CSAccountAuthor) {
        this.CSAccountAuthor = CSAccountAuthor;
    }

    public String getImportUser() {
        return ImportUser;
    }

    public void setImportUser(String importUser) {
        ImportUser = importUser;
    }

    public String getImportTime() {
        return ImportTime;
    }

    public void setImportTime(String importTime) {
        ImportTime = importTime;
    }

    public String getImportTeam() {
        return ImportTeam;
    }

    public void setImportTeam(String importTeam) {
        ImportTeam = importTeam;
    }

    public String getCSUserName() {
        return CSUserName;
    }

    public void setCSUserName(String CSUserName) {
        this.CSUserName = CSUserName;
    }

    public String getCSTeam() {
        return CSTeam;
    }

    public void setCSTeam(String CSTeam) {
        this.CSTeam = CSTeam;
    }

    public String getCSPassword() {
        return CSPassword;
    }

    public void setCSPassword(String CSPassword) {
        this.CSPassword = CSPassword;
    }

    public int getAccountStates() {
        return AccountStates;
    }

    public void setAccountStates(int accountStates) {
        AccountStates = accountStates;
    }

    public int getAccountLimits() {
        return AccountLimits;
    }

    public void setAccountLimits(int accountLimits) {
        AccountLimits = accountLimits;
    }

    public int getAccountOperatorable() {
        return AccountOperatorable;
    }

    public void setAccountOperatorable(int accountOperatorable) {
        AccountOperatorable = accountOperatorable;
    }

    public int getStartCount() {
        return startCount;
    }

    public void setStartCount(int startCount) {
        this.startCount = startCount;
    }

    public int getEndCount() {
        return endCount;
    }

    public void setEndCount(int endCount) {
        this.endCount = endCount;
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
