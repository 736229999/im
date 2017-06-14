<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <title>会话统计</title>
  <link href="../css/reset2.css" rel="stylesheet" type="text/css" />
  <link href="../css/content.css" rel="stylesheet" type="text/css">
  <link href="../css/dwqPager.css" rel="stylesheet" type="text/css" />
  <link href="../css/customer.css" rel="stylesheet" type="text/css" />
  <link href="../js/My97DatePicker/skin/WdatePicker.css" rel="stylesheet" type="text/css">
  <link href="../css/content.css" rel="stylesheet" type="text/css"/>
  <link href="../css/base.css" rel="stylesheet" type="text/css"/>

</head>

<body>
<div class="all_cus">
  <!--面包屑-->
  <div class="cla_nav"><span class="on">查询条件</span></div>
  <div></div>
  <!--条件搜索-->
  <div id="sessionInfoInfo">
    <div id="sessionInfoPage">
      <div class="searchbar">
        <ul>
          <li >
            <div>
              <span class="required">*</span>统计时间：
              <input type="text" id="startsessionInfoTime" class="dateSettingTxt icon_calendar fl mr10">
              <input type="text" id="endsessionInfoTime"
                     class="dateSettingTxt icon_calendar">
              <input type="button" id="clearTime" class="clear_data_btn AttendacneClearTime" value="清空">
            </div>
          </li>
          <li>
            <input  type="button" class="btn l mr10 query" id="sessionInfoYesterdayButton" value="昨&nbsp;天">
            <input  type="button" class="btn l mr10 query" id="sessionInfoWeekButton" value="近一周">
            <input  type="button" class="btn l mr10 query" id="sessionInfoMonthButton" value="近一月">
          </li>
          <li class="pull-left mt10">
            <input id="sessionInfoSearchButton" type="button" class="search_btn" value="查询"/>
            <input id="AddresetCondition" type="button" class="rewrite_btn AttendacneClearTime" value="重置"/>
          </li>
        </ul>

      </div>
      <input type="button" id="sessionInfoExportButton" class="export_btn_bg2 export_btn_4 excelExport" value="导出">
      <div class="cl"></div>
      <div class="content_cus">
        <div class="table-m  data ">
          <table class="table table-hover" style="word-break:break-all; word-wrap:break-word;" width="100%">
          <thead>
            <tr>
              <td class="on" width="12%">时段</td>
              <td class="on" width="12%">会话量</td>
              <td class="on" width="12%">所占比例</td>
              <td class="on" width="64%">图表</td>
            </tr>
            </thead>
            <tbody id="sessionInfoPanel">
            <tr>
              <td >test</td>
              <td >test</td>
              <td >test</td>
              <td >
                <span style="background: cornflowerblue;width: 1000px"></span>
              </td>
            </tr>
            <tr>
              <td >test</td>
              <td >test</td>
              <td >test</td>
              <td >
                <div style="background: cornflowerblue;width: 1000px"></div>
              </td>
            </tr>
            <td>
            </tbody>
          </table>

          <div id="noResultsHtml" style="display:none;">
            <p class="tip-record">没有相关记录！</p>
          </div>

        </div>
      </div>
    </div>

  </div>
  </div>
<!--设置弹窗-->
</body>

<script type="text/javascript" src="../js/jquery-1.11.0.min.js"></script>
<script type="text/javascript" src="../js/My97DatePicker/WdatePicker.js"></script>
<script type="text/javascript" src="../js/jq.tab.js"></script>
<script type="text/javascript" src="../js/util/generalUtil.js"></script>
<script type="text/javascript" src="../js/dwqPager.js"></script>
<script type="text/javascript" src="../js/SessionInfo/SessionCount.js"></script>
</html>
