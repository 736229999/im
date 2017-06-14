<%--
  Created by IntelliJ IDEA.
  User: wangzhen
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
    <style>
        .datagrid-header-row td{background-color:#eaf5ff;}
    </style>
    <link rel="stylesheet" type="text/css" href="../js/jeasyui/themes/icon.css"/>
    <link href="../css/content.css" rel="stylesheet" type="text/css"/>
    <link href="../css/base.css" rel="stylesheet" type="text/css"/>
    <link rel="stylesheet" href="../css/fancybox/jquery.fancybox.css"/>
    <link rel="stylesheet" href="../css/monitor/work-statistics.css"/>
</head>
<body>
<div id="main_warp">
    <!--路径导航
    <div class="road_warp">当前位置：<a href="#">返回首页</a>><a href="#">商家管理</a>><span>商家处罚规则设定</span></div>
    -->
    <!--标签类名称-->
    <div class="cla_nav"><span class="on">查询条件</span></div>
    <div></div>
    <!--条件搜索-->
    <div class="searchbar">
        <ul>
            <li class="pull-left">
                <label><span class="required">*</span>统计时间：</label>
                <input id="startTime" class="dateSettingTxt icon_calendar" type="text" style="height: 26px" data-name="起时间" autocomplete="off">
                <span class="col_to">-</span>
                <input id="endTime" class="dateSettingTxt icon_calendar" type="text" style="height: 26px" data-name="止时间" autocomplete="off">
                <input type="button" id="clearTime" class="clear_data_btn" value="清空"/>
            </li>
            <li>
                <label id="sellerIDLabel" class="labelplacehold" for="sellerID" data-tip="商家编码(6位数字)">商家编码：</label>
                <input type="text" id="sellerID" class="easyui-validatebox seller_nick1" value=""
                       auto-complete="off" data-name="商家编码" autocomplete="off">
            </li>
            <li>
                <label for="sellerName">商家名称：</label>
                <input type="text" id="sellerName"class="easyui-combobox seller_nick1 seller_nick1" value=""
                       data-name="商家名称" autocomplete="off">
            </li>
            <li>
                <label for="storeName">店铺名称：</label>
                <input type="text" id="storeName"class="easyui-combobox seller_nick1 seller_nick1" value=""
                        data-name="店铺名称" autocomplete="off">
            </li>
            <li class="pull-left">
                <label>累计登录总时长（时）：</label>
                <input id="startOnlineTime" class="TimeSettingTxt " type="text" style="height: 26px;width: 100px  data-name="在线起时间" autocomplete="off">
                <span class="col_to">-</span>
                <input id="endOnlineTime" class="TimeSettingTxt " type="text" style="height: 26px;width: 100px"  data-name="在线止时间" autocomplete="off">
                <input type="button" id="clearOnlineTime" class="clear_data_btn" value="清空"/>
            </li>
            <li >
                <label>90s在线响应率（%）：</label>
                <input id="responsein90secondsRateStart" class="TimeSettingTxt " type="text" style="height: 26px;width: 100px"   data-name="responseIn90SecondsRateStart" autocomplete="off">
                <span class="col_to">-</span>
                <input id="responsein90secondsRateEnd" class="TimeSettingTxt " type="text" style="height: 26px;width: 100px"  data-name="responseIn90SecondsRateEnd" autocomplete="off">
                <input type="button" id="clearreplayTime" class="clear_data_btn" value="清空"/>
            </li>
            <li >
                <input id="search" type="button" class="search_btn" value="查询"/>
                <input id="resetCondition" type="button" class="rewrite_btn" value="重置"/>
                <label id="errormessage" class="hide" ><font color=red>90s在线响应率大于100会按照100进行统计</font> </label>
            </li>
        </ul>
    </div>

    <div >
        <ul>
            <li>
                <!--导出excel表格按钮-->
                <input type="button" id="excelExport" class="export_btn_bg export_btn_4" value="导出" onclick="excelExport()">
                <input  type="button" class="time-btn-unselect" id="workLoadYesterdayButton" value="昨天">
                <input  type="button" class="time-btn-unselect" id="workLoadLastWeekButton" value="上一周">
                <input  type="button" class="time-btn-unselect" id="workLoadLastMonthButton" value="上个月">
                <input type="button" id="sendWarning" class="operation_btn_bg sendAlert" style="visibility: hidden" value="发送警告" >

            </li>
        </ul>
    </div>

    <div id="sendWarningDialog" class="easyui-dialog" title="选择警告类型" style="width: 470px; height: 250px;"
         data-options="iconCls:'pag-list',modal:true,collapsible:false,minimizable:false,maximizable:false,resizable:false,closed:true">
        <ul>
            <br/>
            <br/>
            <li>
                <div style="padding-left: 28px; padding-right: 28px">


                    <input id ="moderateWarning" name="warnType" type="radio" class="" value="1" checked="checked" >中度警告</input>
                    <label>&nbsp&nbsp </label>
                    <input id ="seriousWarning"  name="warnType" type="radio" class="" value="2">重度警告</input>
                    <br/>
                    <br/>
                    <span style="font-weight: bold" margin-right="10px" >站内信警告文案：</span>
                    <span id="dialogContent" >亲爱的商家您好：您被系统检测到近几日多次飞牛哞哞在线时长低于六小时，
                    即将被处罚，请您注意登录。如有疑问请及时和类目反馈。谢谢！</span>
                </div>
                
            </li>
            <br/>
            <br/>
            <li>
                <div style="text-align:center;padding:5px;clear: both">
                    <input id="sendInnerMail" class="operation_btn_bg con_btn_2" value="发送">
                    <input id="cancelSend" class="operation_btn_bg ret_btn_2" value="取消">
                </div>
            </li>
        </ul>
    </div>


    <!--结果-->
    <div id="content">
        <div class="padd_b_10">
            <div id="sellerWorkTab" style="width: 100%;">
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
<script type="text/javascript" src="../js/plugin/jquery.fancybox.pack.js"></script>
<script type="text/javascript" src="../js/monitor/work-statistics.js"></script>
</body>
</html>
