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
    <link rel="stylesheet" href="../css/fancybox/jquery.fancybox.css"/>
    <link rel="stylesheet" href="../css/monitor/history-message-list.css"/>
</head>

<tempdata data-dirctorID="${dirctorID}" > </tempdata>
<body>
<div id="main_warp">
    <!--标签类名称-->
    <div class="cla_nav"><span class="on">查询条件</span></div>
    <div></div>
    <!--条件搜索-->
    <div class="searchbar">
        <ul>
            <li>
                <label for="type"><span class="required">*</span>会话来源：</label>
                <select id="type" name="type" value="" class="ks_select" onchange= "onChangeType()" style="width: 190px;">
                <option value="1">飞牛客服</option>
                <option value="0">商家店铺</option>
                </select>
            </li>

            <li>
                <label id="customerIDLabel" for="customerID" data-tip="邮箱/用户名/手机号">买家账号：</label>
                <input type="text" id="customerID" class="easyui-validatebox seller_nick1" data-options="required:false" value=""
                      auto-complete="off" data-name="买家账号" autocomplete="off">
            </li>

            <li id="serviceName" >
                <div >
                    <label id="subMerchantNameLabel" for="subMerchantName">员工账号：</label>
                    <input type="text" id="subMerchantName" class="easyui-validatebox seller_nick1" data-options="required:false" value=""
                           auto-complete="off" data-name="员工账号" autocomplete="off">
                </div>
            </li>
            <li id="storeNameLi" style="display:none;">
                <label for="storeName"><span class="required">*</span>店铺名称：</label>
                <input type="text" id="storeName" class="easyui-combobox seller_nick1 seller_nick1" value=""
                       data-name="店铺名称" autocomplete="off">
            </li>

            <li class="pull-left">
                <label><span class="required">*</span>起始时间：</label>
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

    <div id="content">
        <div class="title_nav bordertop_1">聊天记录</div>
        <div class="padd_b_10 chat-message-panel">
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
<script type="text/javascript" src="../js/util/feiniu-image.js"></script>
<script type="text/javascript" src="../js/util/generalUtil.js"></script>
<script type="text/javascript" src="../js/plugin/jquery.fancybox.pack.js"></script>
<script type="text/javascript" src="../js/monitor/HistoryChatMessageManager.js"></script>
</body>
</html>
