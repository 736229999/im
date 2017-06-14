package com.feiniu.fnSubAccount.bean;

//import com.sun.org.apache.xpath.internal.operations.String;

/**
 * Author: morningking
 * Date: 2015/9/29
 * Contact: 243717042@qq.com
 */
public class CSAccountResponse {
    private String moumouId;          //哞哞ID ，不能为空
    private String opratorId;          //csId
    private String moumouUserName;           //哞哞登录名
    private String moumouUserLevel;     //哞哞账号权限
    private String importName;       //导入人员
    private String importTime;       //导入时间
    private String importUserTeam;       //导入人员组织
    private String importCsId;
    private String operatorRealName;       //账号所属人员
    private String operatorOrganName;           //账号所属组织
    private String accountStates;       //状态
    private int accountLimits;       //接待上限


    public String getImportCsId() {
        return importCsId;
    }

    public void setImportCsId(String importCsId) {
        this.importCsId = importCsId;
    }

    public String getMoumouId() {
        return moumouId;
    }

    public void setMoumouId(String moumouId) {
        this.moumouId = moumouId;
    }

    public String getOpratorId() {
        return opratorId;
    }

    public void setOpratorId(String opratorId) {
        this.opratorId = opratorId;
    }

    public String getMoumouUserName() {
        return moumouUserName;
    }

    public void setMoumouUserName(String moumouUserName) {
        this.moumouUserName = moumouUserName;
    }

    public String getMoumouUserLevel() {
        return moumouUserLevel;
    }

    public void setMoumouUserLevel(String moumouUserLevel) {
        this.moumouUserLevel = moumouUserLevel;
    }

    public String getImportName() {
        return importName;
    }

    public void setImportName(String importName) {
        this.importName = importName;
    }

    public String getImportTime() {
        return importTime;
    }

    public void setImportTime(String importTime) {
        this.importTime = importTime;
    }

    public String getImportUserTeam() {
        return importUserTeam;
    }

    public void setImportUserTeam(String importUserTeam) {
        this.importUserTeam = importUserTeam;
    }

    public String getOperatorRealName() {
        return operatorRealName;
    }

    public void setOperatorRealName(String operatorRealName) {
        this.operatorRealName = operatorRealName;
    }

    public String getOperatorOrganName() {
        return operatorOrganName;
    }

    public void setOperatorOrganName(String operatorOrganName) {
        this.operatorOrganName = operatorOrganName;
    }

    public String getAccountStates() {
        return accountStates;
    }

    public void setAccountStates(String accountStates) {
        this.accountStates = accountStates;
    }

    public int getAccountLimits() {
        return accountLimits;
    }

    public void setAccountLimits(int accountLimits) {
        this.accountLimits = accountLimits;
    }
}
