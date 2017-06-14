package com.feiniu.csWebIm.bean.domain;
/**
 * Created by chenwuming on 2015/10/30.
 */
public class EmployeeInfo
{
    private String employeeName="--";//员工信息
    private String emailInfo="--"; //邮件
    private String cellPhone="--"; //移动电话
    private String departmentInfo="--";//部门组织

    public String getEmployeeName() {
        return employeeName;
    }

    public void setEmployeeName(String employeeName) {
        this.employeeName = employeeName;
    }

    public String getEmailInfo() {
        return emailInfo;
    }

    public void setEmailInfo(String emailInfo) {
        this.emailInfo = emailInfo;
    }

    public String getCellPhone() {
        return cellPhone;
    }

    public void setCellPhone(String cellPhone) {
        this.cellPhone = cellPhone;
    }

    public String getDepartmentInfo() {
        return departmentInfo;
    }

    public void setDepartmentInfo(String departmentInfo) {
        this.departmentInfo = departmentInfo;
    }
}