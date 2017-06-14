<%@ page import="java.util.regex.Pattern" %>
<%@ page import="java.util.regex.Matcher" %>
<%--
  Created by IntelliJ IDEA.
  User: wangchen
  Date: 2016/3/24
  Time: 16:58
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%
    String requestURL = request.getRequestURL().toString();
    String host = requestURL.replaceAll("http://([^/:]++).+$", "$1");
    String staticResourceHost = "http://static11.fn-mart.com";

    if (host.endsWith("fn.com")) {
        staticResourceHost = "http://static.beta.fn.com";
    }

%>
<link rel="stylesheet" href="<%=staticResourceHost%>/merchant/css/base/v0.0.1.css"/>
