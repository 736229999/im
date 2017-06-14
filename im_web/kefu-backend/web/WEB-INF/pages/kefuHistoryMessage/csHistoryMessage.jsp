<%--
  Created by IntelliJ IDEA.
  User: wangzhen
  Date: 2016/4/6
  Time: 9:01
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title>客服对话记录</title>
    <link rel="stylesheet" type="text/css" href="../js/jeasyui/themes/default/easyui.css"/>
    <link rel="stylesheet" type="text/css" href="../js/jeasyui/themes/icon.css"/>
    <link href="../css/content.css" rel="stylesheet" type="text/css"/>
    <link href="../css/base.css" rel="stylesheet" type="text/css"/>
    <link rel="stylesheet" href="../css/font-awesome.min.css" type="text/css"/>
    <link rel="stylesheet" href="../css/relative-settings/faq-index.css" type="text/css"/>
    <link rel="stylesheet" href="../css/fancybox/jquery.fancybox.css"/>
    <link rel="stylesheet" href="../css/monitor/history-message-list.css"/>
</head>
<body>
<div id="main_warp">
    <div class="cla_nav"><span class="on">查询条件</span></div>
    <div></div>
    <!--条件搜索-->
    <div class="searchbar">
        <ul>
            <li>
                <label><span class="required">*</span>起始时间：</label>
                <input id="startSearchTime" class="dateSettingTxt icon_calendar" type="text" style="height: 26px"
                       data-name="查询开始时间" autocomplete="off">
                <span class="col_to">-</span>
                <input id="endSearchTime" class="dateSettingTxt icon_calendar" type="text" style="height: 26px"
                       data-name="查询结束时间" autocomplete="off">
                <input type="button" id="clearModifyTime" class="clear_data_btn" value="清空"/>
            <li>
                <label id="customerIDLabel" for="customerName" data-tip="邮箱/用户名/手机号">买家账号：</label>
                <input type="text" id="customerName" class="easyui-validatebox seller_nick1"
                       data-options="required:false" value=""
                       auto-complete="off" data-name="买家账号" autocomplete="off">
            </li>
            <li>
                <label for="serviceName">客服名称：</label>
                <input type="text" id="serviceName" class="easyui-combobox seller_nick1 seller_nick1" value=""
                       data-name="客服名称:" autocomplete="off">
            </li>
            </li>
            <li class="pull-left">
                <input id="search" type="button" class="search_btn" value="查询"/>
                <input id="resetCondition" type="button" class="rewrite_btn" value="重置"/>
            </li>
        </ul>
    </div>
    <!--导出excel表格按钮-->
    <div>
        <ul>
            <li class="pull-left" style="margin-right: 30px">
                <input type="button" id="excelExport" class="export_btn_bg export_btn_4" value="导出"
                       onclick="excelExport()">
            </li>
            <li>
                <a class="btn l mr10 query" href="#" id="yesterdayButton">昨&nbsp;天</a>
                <a class="btn l mr10 query" href="#" id="lastWeekButton">近一周</a>
                <a class="btn l mr10 query" href="#" id="lastMonthButton">近一月</a>
            </li>
        </ul>
    </div>
    <br>
    <!--结果-->
    <div id="content">
        <div class="padd_b_10">
            <div id="seachResultTab" style="width: 100%;">
            </div>
        </div>
    </div>

</div>
<div class="chart-mask" style="display: none">
    <div class="wrapper">
        <div class="chart-tab-bar">
            <span class="chart-tab rewrite_btn active" id="bar-tab">柱状图</span>
            <span class="chart-tab rewrite_btn" id="pie-tab">饼图</span>
        </div>
        <div class="chart-container">
            <div class="chart-to-show" id="count-chart-bar"></div>
            <div class="chart-to-show" id="count-chart-pie"></div>
            <div class="loading">
                <span class="loading-indicator"><i class="fa fa-spinner fa-pulse"></i>&nbsp;&nbsp;正在加载...</span>
            </div>
        </div>
    </div>
    <div class="toolbar">
        <input id="chartCloseBtn" class="operation_btn_bg ret_btn_2" value="返回"/>
    </div>
</div>

<div id="chat_detail" style="display:none;">
    <div class="searchbar">
        <div class="w200 h30">
            <input type="button" id="chatHistoryBack" class="back_btn_bg export_btn_4" value="返回">
        </div>
    </div>

    <div id="chat_ontent">
        <div class="title_nav bordertop_1">聊天记录</div>
        <div id="content_chat_list" class="padd_b_10 chat-message-panel">
            <div id="chatMessageTable" style="width: 100%;">
            </div>
        </div>
    </div>
</div>

<script type="text/javascript" src="../js/jquery.min.js"></script>
<script type="text/javascript" src="../js/jeasyui/jquery.easyui.min.js"></script>
<script type="text/javascript" src="../js/jeasyui/validateExtend.js"></script>
<script type="text/javascript" src="../js/jeasyui/locale/easyui-lang-zh_CN.js"></script>
<script type="text/javascript" src="../js/jeasyui/validateExtend.js"></script>
<script type="text/javascript" src="../js/myCalendar/WdatePicker.js"></script>
<script type="text/javascript" src="../js/util/generalUtil.js"></script>
<script type="text/javascript" src="../js/util/easyuiUtil.js"></script>
<script type="text/javascript" src="../js/util/feiniu-image.js"></script>
<script type="text/javascript" src="../js/plugin/uploader/jquery.uploader.js"></script>
<script type="text/javascript" src="../js/plugin/echarts/echarts-all.js"></script>
<script type="text/javascript" src="../js/plugin/jquery.fancybox.pack.js"></script>
<script type="text/javascript" src="../js/kefuHistoryMessage/kefuHistoryMessage.js"></script>

</body>
</html>
