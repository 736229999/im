<%@ page import="com.feiniu.webim.config.ConfigManager" %>
<%--
  Created by IntelliJ IDEA.
  User: wangchen
  Date: 2015/12/16
  Time: 9:01
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%
    response.setStatus(HttpServletResponse.SC_OK);
    String merchantBackendErroPage = ConfigManager.getProperty("merchant_backend_error_page") + "500";
%>
<html>
<head>
    <title>500错误页跳转</title>
</head>
<body>
<c:redirect url="<%=merchantBackendErroPage%>"/>
</body>
</html>