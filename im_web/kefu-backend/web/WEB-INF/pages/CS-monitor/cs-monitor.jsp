<%--
  Created by IntelliJ IDEA.
  User: wanbingwen
  Date: 2015/10/28
  Time: 10:08
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
  <title>实时监控</title>

  <link rel="stylesheet" type="text/css" href="../js/jeasyui/themes/default/easyui.css"/>
  <style>
    .datagrid-header-row td{background-color:#eaf5ff;}
  </style>
  <link rel="stylesheet" type="text/css" href="../js/jeasyui/themes/icon.css"/>
  <link href="../css/content.css" rel="stylesheet" type="text/css"/>
  <link href="../css/base.css" rel="stylesheet" type="text/css"/>
  <link rel="stylesheet" href="../css/fancybox/jquery.fancybox.css"/>
  <link rel="stylesheet" href="../css/CS-monitor/cs-monitor.css"/>
</head>

<body>

<div id="main_warp">
  <div >
    <ul>
      <li class="pull-right">
       <label>刷新时间：</label>
        <label id="refreshTime"  type="text" style="height: 26px;width: 150px"  data-name="刷新时间" autocomplete="off">sdfsdfsdf</label>

        <input id="xiaogougou" type="checkbox" value="checkbox" />
        <%--<input id="xiaogougou" type="checkbox" value="checkbox" checked/>--%>

        <label>自动刷新</label>
        <label>每隔</label>

        <select id="u113_input" class="text_sketch" onchange="onchangeDuration()">
            <option value="1">1</option>
            <option value="3">3</option>
            <option value="5">5</option>
            <option value="10">10</option>
            <option value="15">15</option>
        </select>
        <label>分钟</label>
        <!--刷新按钮-->
        <input type="button" id="Addfresh" class="refreshcontext" value="刷新"  autocomplete="off">

      </li>

      <li >
        <!--导出excel表格按钮-->
        <input type="button" id="excelExport" class="export_btn_bg export_btn_4" value="导出" onclick="excelExport()"autocomplete="off">

      </li>

    </ul>
  </div>

  <div id="u118" style="background: url(../images/u118.png) no-repeat;">

      <p style="font-size:18px;">
        <span style="font-family:'应用字体 Bold', '应用字体';font-size:13px;color:#333333;">&nbsp;正在排队客人：</span>
        <span id="queuingCount" style="font-family:'应用字体 Bold', '应用字体';font-size:18px;color:#FF0000;">0</span>
        <span style="font-family:'应用字体 Bold', '应用字体';font-size:13px;color:#333333;">位</span>
          <a id="queueInfoDetail" class="c_bule look" href="#" style="color:#256fb0; ;font-size:13px; text-decoration:none;" index="254">【排队详情】</a>
      </p>
  </div>

  <!--结果-->
  <div id="content">
    <div class="padd_b_10">
      <div id="cs-monitorTab" style="width: 100%;">
      </div>
    </div>
  </div>
    <div id="queueInfoDialog" class="easyui-dialog" title="排队详情" style="width: 500px; height: 300px;"
         data-options="iconCls:'pag-list',modal:true,collapsible:false,minimizable:false,maximizable:false,resizable:false,closed:true">
        <div style="margin-left: 5px;margin-right: 5px;margin-top: 5px;">
            <div class="cl"></div>
            <div id="content_queue_info">
                <div class="padd_b_10">
                    <div id="cs-queue-tab" style="width: 100%;">
                    </div>
                </div>
            </div>
        </div>
    </div>

    <div id="cancelOfflineDialog" class="easyui-dialog" title="温馨提示无法下线" style="width: 350px; height: 165px;"
         data-options="iconCls:'pag-list',modal:true,collapsible:false,minimizable:false,maximizable:false,resizable:false,closed:true">
        <ul>
            <br/>
            <br/>
            <li>
                <label>&nbsp&nbsp&nbsp&nbsp&nbsp </label>

                <span id="dialogContent" >该客服正在会话中,无法下线！</span>
            </li>
            <br/>
            <br/>
            <li>
                <div style="text-align:center;padding:5px;clear: both">
                    <input id="cancelOffline" class="operation_btn_bg con_btn_2" value="确认">
                </div>
            </li>
        </ul>
    </div>

  <script type="text/javascript" src="../js/jquery.min.js"></script>
  <script type="text/javascript" src="../js/jeasyui/jquery.easyui.min.js"></script>
  <script type="text/javascript" src="../js/jeasyui/validateExtend.js"></script>
  <script type="text/javascript" src="../js/jeasyui/locale/easyui-lang-zh_CN.js"></script>
  <script type="text/javascript" src="../js/jeasyui/validateExtend.js"></script>
  <script type="text/javascript" src="../js/myCalendar/WdatePicker.js"></script>
  <script type="text/javascript" src="../js/util/generalUtil.js"></script>
  <script type="text/javascript" src="../js/plugin/jquery.fancybox.pack.js"></script>
  <script type="text/javascript" src="../js/CS-monitor/cs-monitor.js"></script>
</div>
</body>
</html>
