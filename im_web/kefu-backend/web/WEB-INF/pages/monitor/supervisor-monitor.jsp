<%@ page import="com.feiniu.fnSubAccount.constants.BusinessConfig" %>
<%@ page import="com.feiniu.fnSubAccount.util.WebUtil" %>
<%--
  Created by IntelliJ IDEA.
  User: wangchen
  Date: 2015/4/15
  Time: 15:30
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title>运营后台管理系统</title>
    <link rel="stylesheet" type="text/css" href="../js/jeasyui/themes/default/easyui.css"/>
    <link rel="stylesheet" type="text/css" href="../js/jeasyui/themes/icon.css"/>
    <link href="../css/content.css" rel="stylesheet" type="text/css"/>
    <link href="../css/base.css" rel="stylesheet" type="text/css"/>
    <link rel="stylesheet" href="../css/font-awesome.min.css"/>
    <link rel="stylesheet" href="../css/fancybox/jquery.fancybox.css"/>
    <link rel="stylesheet" href="../css/monitor/supervisor-monitor.css"/>
    <%
        BusinessConfig businessConfig = WebUtil.getBean(BusinessConfig.class, request.getSession().getServletContext());
        request.setAttribute("isPreview", businessConfig.isPreview());
    %>
</head>

<body>
<div id="main_warp">
    <div id="content">
        <div class="table-panel">
            <div class="title_nav bordertop_1"><span>会话信息(显示当前会话中的会话信息)</span>
                <span class="fa fa-refresh easyui-tooltip" id="refreshBtn" title="点击刷新"></span>
            </div>
            <div class="padd_b_10 chat-session-panel">
                <div id="chatSessionTable" style="width: 100%;">
                </div>
            </div>
        </div>
        <div class="table-panel" style="margin-top: 20px">
            <div class="title_nav bordertop_1">聊天记录<span id="session-indicator-tip"></span></div>
            <div class="padd_b_10 chat-message-panel">
                <div id="chatMessageTable" style="width: 100%;overflow-y: auto">
                </div>
            </div>
        </div>
    </div>

    <div id="confirmInvokeWindow" class="easyui-window"
         data-options="collapsible : false, minimizable : false, maximizable : false, closed: true,
             closable : true, modal : true, title: '确认是否插入会话'" style="width: 300px;display: none">
        <p class="confirm-delete-panel">您确认插入会话吗？</p>

        <div style="text-align:center;padding:5px 5px 10px;">
            <input id="confirmInvoke" class="confirm-btn" value="确认"/>
            <input id="cancelInvoke" class="confirm-btn cancel" value="取消"/>
        </div>
    </div>
</div>

<script type="text/javascript">
    window._globalInfo = {
        userIDInMoumou: '${userIDInMoumou}',
        userIDInBackend: '${userIDInBackend}',
        isPreview: ${isPreview}
    }
</script>
<script type="text/javascript" src="../js/common/json2.min.js"></script>
<script type="text/javascript" src="../js/jquery.min.js"></script>
<script type="text/javascript" src="../js/jeasyui/jquery.easyui.min.js"></script>
<script type="text/javascript" src="../js/jeasyui/validateExtend.js"></script>
<script type="text/javascript" src="../js/jeasyui/locale/easyui-lang-zh_CN.js"></script>
<script type="text/javascript" src="../js/jeasyui/validateExtend.js"></script>
<script type="text/javascript" src="../js/myCalendar/WdatePicker.js"></script>
<script type="text/javascript" src="../js/util/feiniu-image.js"></script>
<script type="text/javascript" src="../js/util/generalUtil.js"></script>
<script type="text/javascript" src="../js/util/easyuiUtil.js"></script>
<script type="text/javascript" src="../js/plugin/jquery.fancybox.pack.js"></script>
<script type="text/javascript" src="../js/monitor/SupervisorMonitor.js"></script>
</body>
</html>
