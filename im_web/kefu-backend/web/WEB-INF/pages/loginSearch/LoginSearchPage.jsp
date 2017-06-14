<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
  <title>登陆日志查询</title>
  <link rel="stylesheet" type="text/css" href="../js/jeasyui/themes/default/easyui.css"/>
  <link rel="stylesheet" type="text/css" href="../js/jeasyui/themes/icon.css"/>
  <link href="../css/content.css" rel="stylesheet" type="text/css"/>
  <link href="../css/base.css" rel="stylesheet" type="text/css"/>
  <link rel="stylesheet" href="../css/fancybox/jquery.fancybox.css"/>
  <link rel="stylesheet" href="../css/monitor/history-message-list.css"/>
</head>
<body>
<div id="main_warp">
  <!--标签类名称-->
  <div class="cla_nav"><span class="on">查询条件</span></div>
  <div></div>
  <!--条件搜索-->
  <div class="searchbar">
    <ul>
      <li id="storeNameLi">
        <label for="storeName"><span class="required">*</span>店铺名称：</label>
        <input type="text" id="storeName" class="easyui-combobox seller_nick1 seller_nick1" value=""
               data-name="店铺名称" autocomplete="off">
      </li>
      <li>
        <label for="serviceName"><span class="required">*</span>账号：</label>
        <select id="serviceName" name="serviceName" value="" class="ks_select" style="width: 190px;">
        </select>
      </li>
      <li >
        <label><span class="required">*</span>统计时间：</label>
        <input id="startTime" class="dateSettingTxt icon_calendar" type="text" style="height: 26px" data-name="起时间" autocomplete="off">
        <span class="col_to">-</span>
        <input id="endTime" class="dateSettingTxt icon_calendar" type="text" style="height: 26px" data-name="止时间" autocomplete="off">
        <input type="button" id="clearTime" class="clear_data_btn" value="清空"/>
      </li>
      <li class="pull-left clear-fix">
        <input id="search" type="button" class="search_btn" value="查询"/>
        <input id="resetCondition" type="button" class="rewrite_btn" value="重置"/>
      </li>
    </ul>
  </div>

  <!--结果-->
  <div id="content">
    <div class="padd_b_10">
      <div id="loginDetailTab" style="width: 100%;">
      </div>
    </div>
  </div>
</div>


<script type="text/javascript" src="../js/jquery.min.js"></script>
<script type="text/javascript" src="../js/json2.js"></script>
<script type="text/javascript" src="../js/jeasyui/jquery.easyui.min.js"></script>
<script type="text/javascript" src="../js/jeasyui/validateExtend.js"></script>
<script type="text/javascript" src="../js/jeasyui/locale/easyui-lang-zh_CN.js"></script>
<script type="text/javascript" src="../js/jeasyui/validateExtend.js"></script>
<script type="text/javascript" src="../js/myCalendar/WdatePicker.js"></script>
<script type="text/javascript" src="../js/util/generalUtil.js"></script>
<script type="text/javascript" src="../js/plugin/jquery.fancybox.pack.js"></script>
<script type="text/javascript" src="../js/LoginSearch/LoginSearch.js"></script>
</body>
</html>
