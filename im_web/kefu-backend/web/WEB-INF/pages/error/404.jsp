<%@ page import="org.springframework.web.context.support.WebApplicationContextUtils" %>
<%@ page import="com.feiniu.fnSubAccount.util.WebUtil" %>
<%@ page import="com.feiniu.fnSubAccount.constants.BusinessConfig" %>
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
    String backendErroPage = WebUtil.getBean(BusinessConfig.class, config.getServletContext()).getErrorPageBasePath() + "404";
%>
<html>
<head>
    <title>404错误页跳转</title>
</head>
<body>
<c:redirect url="<%=backendErroPage%>"/>
</body>
</html>