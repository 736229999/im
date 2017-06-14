<%--
  Created by IntelliJ IDEA.
  User: wangzhen
  Date: 2015/9/18
  Time: 15:36
  To change this template use File | Settings | File Templates.
--%>
<%@ page import="com.feiniu.fnSubAccount.util.JsonUtil" %>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <title>客服绩效</title>
  <link href="../css/reset2.css" rel="stylesheet" type="text/css" />
  <link href="../css/dwqPager.css" rel="stylesheet" type="text/css" />
  <link href="../css/customer.css" rel="stylesheet" type="text/css" />
  <link href="../js/My97DatePicker/skin/WdatePicker.css" rel="stylesheet" type="text/css">
  <link href="../css/content.css" rel="stylesheet" type="text/css"/>
  <link href="../css/base.css" rel="stylesheet" type="text/css"/>


</head>

<tempdata data-directorid="${directorID}"
          data-subaccountlist='<%=JsonUtil.toJsonString(request.getAttribute("subMerchantInfoList"))%>'
        >
</tempdata>

<body>
<div class="all_cus">
  <!--面包屑-->
  <div class="cla_nav"><span class="on">查询条件</span></div>
  <div></div>
  <!--工作量情况-->
  <div id="workLoadInfo">
    <div id="workLoadPage">
      <div class="searchbar">
        <ul>
          <li >
            <div>
              <span class="required">*</span>统计时间：
              <input type="text" id="startWorkLoadTime" class="dateSettingTxt icon_calendar fl mr10">
              <input type="text" id="endWorkLoadTime"
                     class="dateSettingTxt icon_calendar">
              <input type="button" id="clearTime" class="clear_data_btn WorkLoadClearTime" value="清空">
            </div>
          </li>
          <li>
            <input  type="button" class="btn l mr10 query" id="workLoadYesterdayButton" value="昨&nbsp;天">
            <input  type="button" class="btn l mr10 query" id="workLoadLastWeekButton" value="近一周">
            <input  type="button" class="btn l mr10 query" id="workLoadLastMonthButton" value="近一月">
          </li>
          <li class="pull-left mt10">
            <input id="workLoadSearchButton" type="button" class="search_btn" value="查询"/>
            <input id="AddresetCondition" type="button" class="rewrite_btn WorkLoadClearTime" value="重置"/>
          </li>
        </ul>

      </div>
      <input type="button" id="workLoadExportButton" class="export_btn_bg2 export_btn_4 excelExport" value="导出">
      <div class="cl"></div>
      <div class="content_cus">
        <div class="table-m data">
        <table class="table table-hover" style="word-break:break-all; word-wrap:break-word;" width="100%" >
          <thead>
          <tr>
            <td width="7%" class="on" title="客服员工姓名">哞哞客服</td>
            <td width="7%" class="on" title="登录哞哞时长总和">累计登录总时长（小时）</td>
            <td width="7%" class="on" title="哞哞接待（会话接待）总时长">累计接待总时长（小时）</td>
            <td width="7%" class="on" title="咨询客服的总客人数">咨询量（人）</td>
            <td width="7%" class="on" title="哞哞接待的总客人数">接待量（人）</td>
            <td width="7%" class="on" title="咨询客服的次数">咨询次数</td>
            <td width="7%" class="on" title="哞哞接待的次数">接待次数</td>
            <td width="7%" class="on" title="接待次数\咨询次数">回复率</td>
            <td width="7%" class="on" title="客服回复客人消息数的平均值">平均回复消息数</td>
            <td width="7%" class="on" title="哞哞接待每个客人时长的平均值">会话平均时长（分钟）</td>
            <td width="7%" class="on" title="客服首次响应客人咨询时长的平均值">平均首次回复响应时间（秒）</td>
            <td width="7%" class="on last-child" title="查看每个客服工作量明细">操作</td>
          </tr>
          </thead>
          <tbody id="containerWorkLoadPage">
          </tbody>
        </table>

        <div id="noResultsHtml" style="display:none;">
          <p class="tip-record">没有相关记录！</p>
        </div>

      </div>
      </div>
    </div>
      <!--工作量情况-查看明细-->
    <div  id="workLoadDetailPage" style="display:none;">
      <div class="w200 h30">
        <input type="button" id="detailExcelExport" class="export_btn_bg2 export_btn_4 excelExport" value="导出">
        <input type="button" id="workLoadDetailBack" class="back_btn_bg export_btn_4" value="返回">
      </div>
      <div class="cl"></div>
        <div class="content_cus">
          <div class="table-m data">
            <table class="table table-hover" style="word-break:break-all; word-wrap:break-word;" width="100%" >
              <thead>
              <tr>
                <td width="7%" class="on" title="统计时间段每天的日期">日期</td>
                <td width="10%" class="on" title="登录哞哞时长总和">累计登录总时长（小时）</td>
                <td width="10%" class="on" title="哞哞接待（会话接待）总时长">累计接待总时长（小时）</td>
                <td width="7%" class="on" title="咨询客服的总客人数(去重">咨询量(人)</td>
                <td width="7%" class="on" title="哞哞接待的总客人数(去重)">接待量（人）</td>
                <td width="7%" class="on" title="咨询客服的次数">咨询次数</td>
                <td width="7%" class="on" title="哞哞接待的次数">接待次数</td>
                <td width="7%" class="on" title="接待次数\咨询次数">回复率</td>
                <td width="7%" class="on" title="客服回复客人消息数的平均值">平均回复消息数</td>
                <td width="7%" class="on" title="哞哞接待每个客人时长的平均值">会话平均时长（分钟）</td>
                <td width="8%" class="on last-child" title="客服首次响应客人咨询时长的平均值">平均首次回复响应时间（秒）</td>
              </tr>
              </thead>
              <tbody id="containerWorkLoadDetailPage">
              </tbody>
            </table>
          </div>
        </div>
      </div>
      <!--工作量情况-查看明细-->
  </div>
</div>
<div id="dwqPager" class="tab-ft-wrap"></div>
</div>
</body>

<script type="text/javascript" src="../js/jquery-1.11.0.min.js"></script>
<script type="text/javascript" src="../js/My97DatePicker/WdatePicker.js"></script>
<script type="text/javascript" src="../js/jq.tab.js"></script>
<script type="text/javascript" src="../js/util/generalUtil.js"></script>
<script type="text/javascript" src="../js/dwqPager.js"></script>
<script type="text/javascript" src="../js/attendance/Attendance.js"></script>
<script type="text/javascript" src="../js/attendance/workloadSearch.js"></script>
</html>
