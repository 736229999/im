<%@ page import="com.feiniu.subAccount.util.JsonUtil" %>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title>客服绩效</title>
    <jsp:include page="../common/meta-info.jsp"></jsp:include>
    <link href="../css/reset2.css" rel="stylesheet" type="text/css"/>
    <link href="../css/dwqPager.css" rel="stylesheet" type="text/css"/>
    <link href="../css/customer.css" rel="stylesheet" type="text/css"/>
    <link href="../js/My97DatePicker/skin/WdatePicker.css" rel="stylesheet" type="text/css">
</head>

<tempdata data-merchantid="${merchantID}"
          data-subaccountlist='<%=JsonUtil.toJsonString(request.getAttribute("subMerchantInfoList"))%>'
        >
</tempdata>

<body>
<div class="main-content-area">
    <!--面包屑-->
    <div class="ui-crumbs">
        <p><b>当前位置：</b><a href="javascript:;" class="ui-clickable">商家中心</a><span> &gt; 客服绩效</span></p>
    </div>
    <!--面包屑-->
    <div class="ui-tab">
        <ul class="tab-nav">
            <li class="tab-nav-item cur" id="attendanceTabPage" rel="attendanceInfo"><a>出勤情况</a></li>
            <li class="tab-nav-item" id="workLoadTabPage" rel="workLoadInfo"><a>工作量情况</a></li>
        </ul>
        <ul class="tab-panel pt-20">
            <li id="attendanceInfo" class="tab-panel-item cur">
                <!--出勤情况-->
                <div class="content_cus">
                    <div id="attendancePage">
                        <div class="time">
                            <div class="cale">
                                <p>统计时间：</p>

                                <input id="startAttendanceTime" class="ui-date-input Wdate" type="text"
                                       onclick="WdatePicker()"/>

                                <p class="on">到</p>
                                <input id="endAttendanceTime" class="ui-date-input Wdate" type="text"
                                       onclick="WdatePicker()"/>
                            </div>
                            <button class="ui-button red" type="button" id="attendanceYesterdayButton">昨天</button>
                            <button class="ui-button red" type="button" id="attendanceWeekButton">上一周</button>
                            <button class="ui-button red" type="button" id="attendanceMonthButton">上个月</button>
                            <button class="ui-button red" type="button" id="attendanceSearchButton">查询</button>
                            <button class="ui-button red excelExport" type="button" id="attendanceExportButton">导出
                            </button>
                        </div>
                        <div class="cl"></div>
                        <div class="table-m data">
                            <table class="table table-hover" style="word-break:break-all; word-wrap:break-word;"
                                   width="100%">
                                <thead>
                                <tr>
                                    <td width="9%" class="on" title="客服员工姓名">哞哞客服</td>
                                    <td width="9%" class="on" title="客服登录哞哞时长不为0的天数">上班天数</td>
                                    <td width="11%" class="on" title="统计时间内登录哞哞时长总和">累计登录总时长（小时）</td>
                                    <td width="13%" class="on" title="累计登录总时长/上班天数">日均登录时长（小时）</td>
                                    <td width="13%" class="on" title="反映客服人员上班时间的集中趋势">每天首次上线时间均值</td>
                                    <td width="12%" class="on" title="反映客服人员下班时间的集中趋势">每天最晚下线时间均值</td>
                                    <td width="12%" class="on" title="统计时间内最早登录哞哞的时间">最早上线时间</td>
                                    <td width="11%" class="on" title="统计时间内最晚退出登录哞哞的时间">最晚下线时间</td>
                                    <td width="10%" class="on " title="查看每个客服出勤明细">操作</td>
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
                    <!--查看明细-->
                    <div id="attendanceDetailPage" style="display:none;">
                        <div class="w200 h30">
                            <button class="ui-button red" type="button" id="attendanceDetailBack">返回</button>
                            <button class="ui-button red excelExport" type="button" id="attendanceSearchButton">导出
                            </button>
                        </div>
                        <div class="cl"></div>
                        <div class="table-m data">
                            <table class="table table-hover" style="word-break:break-all; word-wrap:break-word;"
                                   width="100%">
                                <thead>
                                <tr>
                                    <td width="11%" class="on" title="统计时间段每天的日期">日期</td>
                                    <td width="14%" class="on" title="当天客服首次登录哞哞时间">当天首次上线时间</td>
                                    <td width="14%" class="on" title="当天客服最后退出哞哞时间">当天最后在线时间</td>
                                    <td width="9%" class="on" title="当天客服登录哞哞的次数">登录次数</td>
                                    <td width="12%" class="on" title="当天登录哞哞时长总和">当天累计登录总时长（小时）</td>
                                    <td width="16%" class="on" title="当天哞哞“挂起”状态时长总和">当天累计挂起总时长（小时）</td>
                                    <td width="13%" class="on" title="当天哞哞“离线”状态时长总和">当天累计离线总时长（小时）</td>
                                    <td width="11%" class="on last-child"
                                        title="（当天累计登录总时长—当天累计挂起总时长）/（当天最后在线时间-当天首次上线时间）">
                                        在线率
                                    </td>
                                </tr>
                                </thead>
                                <tbody id="containerAttendanceDetailPage">
                                </tbody>
                            </table>
                        </div>

                        <div id="noResultsHtml" style="display:none;">
                            <p class="tip-record">没有相关记录！</p>
                        </div>

                    </div>
                    <!--查看明细-->
                </div>
            </li>

            <!--工作量情况-->
            <li class="tab-panel-item" id="workLoadInfo">
                <div class="content_cus">
                    <div id="workLoadPage">
                        <div class="time">
                            <div class="cale">
                                <p>统计时间：</p>
                                <input id="startWorkLoadTime" class="ui-date-input Wdate" type="text"
                                       onclick="WdatePicker()"/>

                                <p class="on">到</p>
                                <input id="endWorkLoadTime" class="ui-date-input Wdate" type="text"
                                       onclick="WdatePicker()"/>
                            </div>
                            <button class="ui-button red" type="button" id="workLoadYesterdayButton">昨天</button>
                            <button class="ui-button red" type="button" id="workLoadLastWeekButton">上一周</button>
                            <button class="ui-button red" type="button" id="workLoadLastMonthButton">上个月</button>
                            <button class="ui-button red" type="button" id="workLoadSearchButton">查询</button>
                            <button class="ui-button red excelExport" type="button" id="workLoadExportButton">导出
                            </button>
                        </div>
                        <div class="cl"></div>
                        <div class="table-m data">
                            <table class="table table-hover" style="word-break:break-all; word-wrap:break-word;"
                                   width="100%">
                                <thead>
                                <tr>
                                    <td width="7%" class="on" title="客服员工姓名">哞哞客服</td>
                                    <td width="7%" class="on" title="客服所在的客服分组">分组名称</td>
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

                    <!--工作量情况-查看明细-->
                    <div id="workLoadDetailPage" style="display:none;">
                        <div class="w200 h30">
                            <button class="ui-button red" type="button" id="workLoadDetailBack">返回</button>
                            <button class="ui-button red excelExport" type="button">导出</button>
                        </div>
                        <div class="cl">
                            <div class="table-m data">
                                <table class="table table-hover" style="word-break:break-all; word-wrap:break-word;"
                                       width="100%">
                                    <thead>
                                    <tr>
                                        <td width="8%" class="on" title="统计时间段每天的日期">日期</td>
                                        <td width="8%" class="on" title="登录哞哞时长总和">累计登录总时长（小时）</td>
                                        <td width="8%" class="on" title="哞哞接待（会话接待）总时长">累计接待总时长（小时）</td>
                                        <td width="8%" class="on" title="咨询客服的总客人数(去重">咨询量(人)</td>
                                        <td width="8%" class="on" title="哞哞接待的总客人数(去重)">接待量（人）</td>
                                        <td width="8%" class="on" title="咨询客服的次数">咨询次数</td>
                                        <td width="8%" class="on" title="哞哞接待的次数">接待次数</td>
                                        <td width="8%" class="on" title="接待次数\咨询次数">回复率</td>
                                        <td width="8%" class="on" title="客服回复客人消息数的平均值">平均回复消息数</td>
                                        <td width="8%" class="on" title="哞哞接待每个客人时长的平均值">会话平均时长（分钟）</td>
                                        <td width="8%" class="on last-child" title="客服首次响应客人咨询时长的平均值">平均首次回复响应时间（秒）</td>
                                    </tr>
                                    </thead>
                                    <tbody id="containerWorkLoadDetailPage">
                                    </tbody>
                                </table>
                            </div>
                        </div>
                        <!--工作量情况-查看明细-->
                    </div>
                </div>
            </li>
        </ul>
    </div>
</div>
<div id="dwqPager" class="tab-ft-wrap"></div>
<!--设置弹窗-->
</body>

<script type="text/javascript" src="../js/jquery-1.11.0.min.js"></script>
<script type="text/javascript" src="../js/My97DatePicker/WdatePicker.js"></script>
<script type="text/javascript" src="../js/util/generalUtil.js"></script>
<script type="text/javascript" src="../js/dwqPager.js"></script>
<script type="text/javascript" src="../js/attendance/Attendance.js"></script>

</html>
