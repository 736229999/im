<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ page import="com.feiniu.fnSubAccount.bean.AppraiseStat" %>
<%@ page import="com.feiniu.fnSubAccount.bean.SubMerchantInfo" %>
<%@ page import="com.feiniu.fnSubAccount.util.JsonUtil" %>
<%@ page import="org.apache.commons.collections4.CollectionUtils" %>
<%@ page import="java.util.List" %>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%
    List<SubMerchantInfo> subMerchantInfoList = (List<SubMerchantInfo>) request.getAttribute("subMerchantInfoList");
    List<AppraiseStat> serviceAppraiseList = (List<AppraiseStat>) request.getAttribute("serviceAppraiseList");

    boolean hasSubMerchant = CollectionUtils.isNotEmpty(subMerchantInfoList);
%>
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
    <meta content="text/html; charset=utf-8" http-equiv="Content-Type">
    <title>监控查询</title>
    <link type="text/css" rel="stylesheet" href="../css/reset.css"/>
    <link href="../css/dwqPager.css" rel="stylesheet" type="text/css" />
    <link type="text/css" rel="stylesheet" href="../css/employee.css"/>

    <link rel="stylesheet" type="text/css" href="../js/jeasyui/themes/default/easyui.css"/>
    <link rel="stylesheet" type="text/css" href="../js/jeasyui/themes/icon.css"/>
    <link href="../css/content.css" rel="stylesheet" type="text/css"/>
    <link href="../css/base.css" rel="stylesheet" type="text/css"/>
    <link rel="stylesheet" href="../css/fancybox/jquery.fancybox.css"/>
    <link rel="stylesheet" href="../css/monitor/history-message-list.css"/>
</head>

<body>
<div class="p-contnet">
    <tmpdata data-serviceappraiselist='<%=JsonUtil.toJsonString(serviceAppraiseList)%>'
             data-submerchantinfolist='<%=JsonUtil.toJsonString(subMerchantInfoList)%>'>
    </tmpdata>
    <div class="cla_nav"><span class="on">查询条件</span></div>
    <div></div>
    <!--面包屑-->
    <div  class="ppglBorderDiv">
        <div class="zzhmessage">
            <div >
                <div id="serverTablePage" class="formbox">
                    <div class="searchbar">
                        <ul>
                            <form action="" method="" >
                                <li >
                                    <div class="pull-left mt10"><span class="required">*</span></div>
                                    <div class="formname w55">起始时间：</div>
                                    <div class="formtext">
                                        <input type="text" id="startTime" class="dateSettingTxt icon_calendar fl mr10">
                                        <input type="text" id="endTime"
                                               class="dateSettingTxt icon_calendar">
                                        <input type="button" id="clearTime" class="clear_data_btn" value="清空">
                                    </div>
                                </li>

                                <li >
                                    <div class="formname w55">客服账号：</div>
                                    <input type="text" id="moumouName"class="easyui-combobox seller_nick1 seller_nick1" value=""
                                           data-name="客服账号" autocomplete="off">
                                </li>

                            </form>

                            <li class="pull-left w55">
                                <input type="button" id="queryAppraiseInfo" class="search_btn" value="查询"/>
                            </li>
                            <li>
                                <input id="resetCondition" type="button" class="rewrite_btn" value="重置">
                            </li>
                        </ul>
                    </div>

                    <ul>
                        <div class="pull-left w55"></div>
                        <div class="formtext">
                            <input type="button" id="excelExport" class="export_btn_bg2 export_btn_4" value="导出">
                        </div>
                    </ul>
                    <table class="serverTable" border="1" width="100%">
                        <thead>
                        <tr>
                            <th width="14%">帐号名</th>
                            <th width="12%">未评估</th>
                            <th width="12%">非常满意</th>
                            <th width="12%">满意</th>
                            <th width="12%">一般</th>
                            <th width="12%">不满意</th>
                            <th width="12%">非常不满意</th>
                            <th width="12%">好评率</th>
                        </tr>
                        </thead>
                        <tbody id="serviceAppraiseInfoPanel">
                        </tbody>
                    </table>
                </div>

                <!--查看明细-->
                <div  id="serverTableDetailPage" style="display:none;">
                    <div class="w200 h30">
                        <input type="button" id="detailExcelExport" class="export_btn_bg export_btn_4" value="导出">
                        <input type="button" id="detailBack" class="back_btn_bg export_btn_4" value="返回">
                    </div>
                    <div class="cl"></div>
                    <table class="serverTable" border="1" width="100%">
                        <thead>
                        <tr>
                            <th width="11%"  >账号名</th>
                            <th width="14%"  >买家账号</th>
                            <th width="14%"  >满意度</th>
                            <th width="9%"  >评价建议</th>
                            <th width="12%" >评价时间</th>
                            <th width="16%" >会话开始时间</th>
                            <th width="13%"  >会话结束时间</th>
                            <th width="11%"  >聊天记录</th>
                        </tr>
                        </thead>
                        <tbody id="containerServerTableDetailPage">
                        </tbody>
                    </table>

                </div>
                <div id="noResultsHtml" style="display:none;">
                    <p class="tip-record">没有相关记录！</p>
                </div>
                <div id="dwqPager" class="tab-ft-wrap"></div>

            </div>
            <!--查看明细-->
            <div id="main_warp" style="display:none;">

                <div class="searchbar" >
                    <div class="w200 h30">
                        <input type="button" id="chatHistoryBack" class="back_btn_bg export_btn_4" value="返回">
                    </div>
                </div>

                <div id="content">
                    <div class="title_nav bordertop_1">聊天记录</div>
                    <div id="content_chat_list"  class="padd_b_10 chat-message-panel">
                        <div id="chatMessageTable" style="width: 100%;">
                        </div>
                </div>
            </div>
            </div>
        </div>
    </div>
</div>


</body>
<script type="text/javascript" src="../js/dwqPager.js"></script>
<script type="text/javascript" language="javascript" src="../js/jquery-1.11.1.min.js"></script>
<script type="text/javascript" language="javascript" src="../js/employee.js"></script>
<script type="text/javascript" src="../js/myCalendar/WdatePicker.js"></script>
<script type="text/javascript" src="../js/jquery.min.js"></script>
<script type="text/javascript" src="../js/jeasyui/jquery.easyui.min.js"></script>
<script type="text/javascript" src="../js/jeasyui/validateExtend.js"></script>
<script type="text/javascript" src="../js/jeasyui/locale/easyui-lang-zh_CN.js"></script>
<script type="text/javascript" src="../js/jeasyui/validateExtend.js"></script>
<script type="text/javascript" src="../js/myCalendar/WdatePicker.js"></script>
<script type="text/javascript" src="../js/util/feiniu-image.js"></script>
<script type="text/javascript" src="../js/util/generalUtil.js"></script>
<script type="text/javascript" src="../js/plugin/jquery.fancybox.pack.js"></script>
<script type="text/javascript" src="../js/monitor/service-appraise.js"></script>
</html>
