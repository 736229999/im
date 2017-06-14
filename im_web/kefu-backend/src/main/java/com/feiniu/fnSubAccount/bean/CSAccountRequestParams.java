package com.feiniu.fnSubAccount.bean;

/**
 * Author: duyong
 * Date: 2015/9/29
 * Contact:
 */
public class CSAccountRequestParams {
    String CSName;           //哞哞登录名
    String CSAccountAuthor;     //哞哞账号权限
    String ImportUser;       //导入人员
    String ImportTime;       //导入时间
    String ImportTeam;       //导入人员组织
    String CSUserName;       //账号所属人员
    String CSTeam;           //账号所属组织
    String CSPassword;      //账号密码
    int AccountStates;       //状态
    int AccountLimits;       //接待上限
    int AccountOperatorable;//操作
    int startCount;
    int endCount;
    int pageSize;
    int currentPageNo;

    public String getCSPassword() {
        return CSPassword;
    }

    public void setCSPassword(String CSPassword) {
        this.CSPassword = CSPassword;
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



    public String getCSName() {
        return CSName;
    }
    public void setCSName(String CSName) {
        this.CSName = CSName;
    }
}
