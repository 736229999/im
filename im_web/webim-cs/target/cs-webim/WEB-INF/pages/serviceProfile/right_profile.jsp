<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ page import="com.feiniu.csWebIm.bean.domain.EmployeeInfo" %>

<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%
  EmployeeInfo employeeInfo = (EmployeeInfo) request.getAttribute("employee");
%>

<!doctype html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <title>Document</title>
  <link href="../css/main.css" rel="stylesheet" type="text/css" />
</head>
<body style="height: 100%;">
<div id="D_right_ifm">
  <div id="J_info_tab" class="head">
    <p id="J_store" class="on">客户</p>
  </div>
  <div id="J_store_info" class="with">

    <ul class="ui-info-list">

      <li>员工信息：<span ><%=employeeInfo.getEmployeeName()%></span></li>
      <li>电子邮箱：<span ><%=employeeInfo.getEmailInfo()%></span></li>
      <li>移动电话：<span><%=employeeInfo.getCellPhone()%></span></li>
      <li>部门组织：<span><%=employeeInfo.getDepartmentInfo()%></span></li>
    </ul>

  </div>


</div>
</div>


</div>
</div>


</body>
</html>