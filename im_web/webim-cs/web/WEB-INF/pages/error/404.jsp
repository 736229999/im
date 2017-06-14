<%@ page import="com.feiniu.csWebIm.consts.BusinessConfig" %>
<%@ page import="com.feiniu.csWebIm.utils.web.SpringUtils" %>
<%--
  Created by IntelliJ IDEA.
  User: wangchen
  Date: 2015/12/16
  Time: 9:01
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" isErrorPage="true" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%
    response.setStatus(HttpServletResponse.SC_OK);
    BusinessConfig businessConfig = SpringUtils.getBean(config.getServletContext(), BusinessConfig.class);
%>
<html>
<head>
    <title>404错误页跳转</title>
</head>
<body>
<c:redirect url="<%=businessConfig.getErrorIndexPath()%>"></c:redirect>
</body>
</html>