<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" isELIgnored="false" %>
<%@page import="com.feiniu.common.core.SystemEnv" %>
<%
    String[] baseHosts = SystemEnv.getProperty("feiniu.backendBaseHost").split(",");
    String baseHost;

    if (baseHosts.length == 1) {
        baseHost = baseHosts[0];
    } else {
        //只考虑prev|online环境
        Object returnURL = request.getAttribute("returnURL");
        String requestUrl = returnURL == null ? "" : returnURL.toString();

        System.out.println("Current Href : " + requestUrl);

        if (!requestUrl.contains("supervisor-monitor")) {
            baseHost = baseHosts[0];
        } else {
            baseHost = baseHosts[1];
        }
    }
%>

<script type="text/javascript">
    function goToLogin(basePath) {
        if (window.parent) {
            window.parent.location.href = basePath + "/login.jsp";
        } else {
            window.location.href = basePath + "/login.jsp";
        }
    }

    goToLogin('<%=baseHost%>');

</script>