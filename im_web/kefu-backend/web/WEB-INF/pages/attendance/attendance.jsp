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
    <!--条件搜索-->
    <div id="attendanceInfo">
        <div id="attendancePage">
            <div class="searchbar">
                <ul>
                    <li >
                        <div>
                        <span class="required">*</span>统计时间：
                            <input type="text" id="startAttendanceTime" class="dateSettingTxt icon_calendar fl mr10">
                            <input type="text" id="endAttendanceTime"
                                   class="dateSettingTxt icon_calendar">
                            <input type="button" id="clearTime" class="clear_data_btn AttendacneClearTime" value="清空">
                        </div>
                    </li>
                    <li>
                        <input  type="button" class="btn l mr10 query" id="attendanceYesterdayButton" value="昨&nbsp;天">
                        <input  type="button" class="btn l mr10 query" id="attendanceWeekButton" value="近一周">
                        <input  type="button" class="btn l mr10 query" id="attendanceMonthButton" value="近一月">
                    </li>
                    <li class="pull-left mt10">
                        <input id="attendanceSearchButton" type="button" class="search_btn" value="查询"/>
                        <input id="AddresetCondition" type="button" class="rewrite_btn AttendacneClearTime" value="重置"/>
                    </li>
                </ul>

            </div>
            <input type="button" id="attendanceExportButton" class="export_btn_bg2 export_btn_4 excelExport" value="导出">
            <div class="cl"></div>
            <div class="content_cus">
                <div class="table-m  data ">
                <table class="table table-hover" style="word-break:break-all; word-wrap:break-word;" width="100%" >
                    <thead>
                    <tr>
                        <td width="9%" class="on" title="客服员工姓名">哞哞客服</td>
                        <td width="9%" class="on" title="客服登录哞哞时长不为0的天数">上班天数</td>
                        <td width="9%" class="on" title="统计时间内登录哞哞时长总和">累计登录总时长（小时）</td>
                        <td width="9%" class="on" title="累计登录总时长/上班天数">日均登录时长（小时）</td>
                        <td width="9%" class="on" title="统计时间内离开状态的次数">累计离开次数</td>
                        <td width="9%" class="on" title="统计时间内离开状态的总时长">累计离开总时长（小时）</td>
                        <td width="9%" class="on" title="反映客服人员上班时间的集中趋势">每天首次上线时间均值</td>
                        <td width="9%" class="on" title="反映客服人员下班时间的集中趋势">每天最后离线时间均值</td>
                        <td width="9%" class="on" title="统计时间内最早登录哞哞的时间">最早上线时间</td>
                        <td width="9%" class="on" title="统计时间内最晚退出登录哞哞的时间">最晚下线时间</td>
                        <td width="9%" class="on " title="查看每个客服出勤明细">操作</td>
                    </tr>
                    </thead>
                    <tbody id="containerAttendancePage">
                    </tbody>
                </table>

                <div id="noResultsHtml" style="display:none;">
                    <p class="tip-record">没有相关记录！</p>
                </div>

                </div>
            </div>
        </div>
        <!--查看明细-->
        <div  id="attendanceDetailPage" style="display:none;">
            <div class="w200 h30">
                <input type="button" id="detailExcelExport" class="export_btn_bg2 export_btn_4 excelExport" value="导出">
                <input type="button" id="attendanceDetailBack" class="back_btn_bg export_btn_4" value="返回">
            </div>
            <div class="cl"></div>
            <div class="content_cus">
                <div class="table-m data">
                    <table class="table table-hover" style="word-break:break-all; word-wrap:break-word;" width="100%" >
                        <thead>
                        <tr>
                            <td width="10%" class="on" title="统计时间段每天的日期">日期</td>
                            <td width="10%" class="on" title="当天客服首次登录哞哞时间">当天首次上线时间</td>
                            <td width="10%" class="on" title="当天客服最后退出哞哞时间">当天最后在线时间</td>
                            <td width="10%" class="on" title="当天客服登录哞哞的次数">登录次数</td>
                            <td width="10%" class="on" title="当天登录哞哞时长总和">当天累计登录总时长（小时）</td>
                            <td width="10%" class="on" title="当天哞哞“挂起”状态时长总和">当天累计挂起总时长（小时）</td>
                            <td width="10%" class="on" title="当天哞哞“离线”状态时长总和">当天累计离线总时长（小时）</td>
                            <td width="10%" class="on" title="当天离开状态的次数">离开次数</td>
                            <td width="10%" class="on" title="当天离开状态的总时长">当天离开总时长（分钟）</td>
                            <td width="10%" class="on last-child" title="（当天累计登录总时长—当天累计挂起总时长）/（当天最后在线时间-当天首次上线时间）">在线率</td>
                        </tr>
                        </thead>
                        <tbody id="containerAttendanceDetailPage">
                        </tbody>
                    </table>
                </div>
            </div>
        </div>
    </div>

</div>
<div id="dwqPager" class="tab-ft-wrap"></div>
    </div>
<!--设置弹窗-->
</body>

<script type="text/javascript" src="../js/jquery-1.11.0.min.js"></script>
<script type="text/javascript" src="../js/My97DatePicker/WdatePicker.js"></script>
<script type="text/javascript" src="../js/jq.tab.js"></script>
<script type="text/javascript" src="../js/util/generalUtil.js"></script>
<script type="text/javascript" src="../js/dwqPager.js"></script>
<script type="text/javascript" src="../js/attendance/Attendance.js"></script>
<script type="text/javascript" src="../js/attendance/attendanceSearch.js"></script>
</html>
