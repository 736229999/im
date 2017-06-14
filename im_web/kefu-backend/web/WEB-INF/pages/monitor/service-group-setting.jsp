<%@ page import="java.util.Map" %>
<%@ page import="com.feiniu.fnSubAccount.constants.EmbedEnum" %>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%--
  Created by IntelliJ IDEA.
  User: wangzhen
  Date: 2015/10/16
  Time: 9:56
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title>分组管理</title>
    <link rel="stylesheet" type="text/css" href="../js/jeasyui/themes/default/easyui.css"/>
    <style>
        .datagrid-header-row td {
            background-color: #eaf5ff;
        }

        .embed-field {
            padding: 15px 0px 0px;
            font-size: 14px;
        }

        .embed-field > select,
        .embed-field > label {
            display: inline-block;
            height: 28px;
            line-height: 28px;
        }

        .embed-field > label {
            width: 200px;
            text-align: right;
        }

        .embed-field > select {
            width: 200px;
            text-align: center;
        }
    </style>
    <link rel="stylesheet" type="text/css" href="../js/jeasyui/themes/icon.css"/>
    <link href="../css/content.css" rel="stylesheet" type="text/css"/>
    <link href="../css/base.css" rel="stylesheet" type="text/css"/>
    <link rel="stylesheet" href="../css/fancybox/jquery.fancybox.css"/>
    <link rel="stylesheet" href="../css/monitor/service-group-setting.css"/>
</head>


<body>
<div id="main_warp">
    <div class="cla_nav"><span class="on">查询条件</span></div>
    <div></div>
    <div id="confirmDeleteWindow" class="easyui-window"
         data-options="collapsible : false, minimizable : false, maximizable : false, closed: true,
             closable : true, modal : true, title: '确认是否删除'" style="width: 300px;display: none">
        <p class="content confirm-delete-panel">确认删除这条数据吗？</p>

        <div style="text-align:center;padding:5px 5px 10px;">
            <input id="confirmDelete" class="confirm-btn" value="确认"/>
            <input id="cancelDelete" class="confirm-btn cancel" value="取消"/>
        </div>
    </div>
    <!--条件搜索-->
    <div class="searchbar">
        <ul>
            <li>
                <label for="groupName">分组名称：</label>
                <input type="text" id="groupName" class="easyui-combobox seller_nick1 seller_nick1" value=""
                       data-name="分组名称" autocomplete="off">
            </li>
            <li>
                <label for="lastModifiedPeopleName">最后修改人：</label>
                <input type="text" id="lastModifiedPeopleName" class="easyui-combobox seller_nick1 seller_nick1"
                       value=""
                       data-name="最后修改人" autocomplete="off">
            </li>
            <li>
                <label>最后修改时间：</label>
                <input id="startModifyTime" class="dateSettingTxt icon_calendar" type="text" style="height: 26px"
                       data-name="最后修改开始时间" autocomplete="off">
                <span class="col_to">-</span>
                <input id="endModifyTime" class="dateSettingTxt icon_calendar" type="text" style="height: 26px"
                       data-name="最后修改结束时间" autocomplete="off">
                <input type="button" id="clearModifyTime" class="clear_data_btn" value="清空"/>
            </li>
            <li class="pull-left">
                <input id="search" type="button" class="search_btn" value="查询"/>
                <input id="resetCondition" type="button" class="rewrite_btn" value="重置"/>
            </li>
        </ul>
    </div>
    <!--导出excel表格按钮-->
    <div>
        <input type="button" id="newGroup" value="新建分组" class="operation_btn_bg add_btn_4">
        <input type="button" id="embedSet" value="布点设置" class="operation_btn_bg add_btn_4">
    </div>
    <!--结果-->
    <div id="content">
        <div class="padd_b_10">
            <div id="groupSettingTab" style="width: 100%;">
            </div>
        </div>
    </div>


    <div id="modifycsDialog" class="easyui-window" title="修改" style="width: 500px; height: 300px;"
         data-options="iconCls:'pag-list',modal:true,collapsible:false,minimizable:false,maximizable:false,resizable:true,closed:true">
        <ul style="text-align: center">
            <br/>
            <br/>
            <li>
                <label for="DialogGroupName">分组名称：</label>
                <input type="text" id="DialogGroupName" class="easyui-validatebox seller_nick1 seller_nick1" value=""
                       data-name="导入人员" autocomplete="off" maxlength="20">
                <span for="DialogGroupName" style="color: #a5a5a5; font-size: 12px;">20字以内</span>
            </li>
            <br/>
            <li>
                <label for="DialogGroupDetail">分组描述：</label>
                <textarea id="DialogGroupDetail" value="" name="分组描述" rows="3" maxlength="100"
                          style="width: 182px"> </textarea>
                <label for="DialogGroupName" style="color: #a5a5a5; font-size: 12px;">100字以内</label>
            </li>
            <br/>
            <br/>
            <li>
                <div style="text-align:center;padding:5px;clear: both">
                    <input id="saveAndNext" class="operation_btn_bg con_btn_2" value="下一步">
                    <input id="cancelSaveAndNext" class="operation_btn_bg ret_btn_2" value="返回">
                </div>
            </li>
        </ul>
    </div>

    <div id="addServiceDialog" class="easyui-dialog" title="修改" style="width: 1200px; height: 600px;"
         data-options="iconCls:'pag-list',modal:true,collapsible:false,minimizable:false,maximizable:false,resizable:false,closed:true">
        <div class="cla_nav"><span class="on">查询条件</span></div>
        <div></div>
        <!--条件搜索-->
        <div class="searchbar">
            <ul>
                <li>
                    <label for="AddOrgnName"><span class="required">*</span>组织：</label>
                    <select id="AddOrgnName" name="AddOrgnName" value="" class="ks_select" style="width: 190px;">
                    </select>
                </li>
                <li>
                    <label for="AddServiceName">员工：</label>
                    <input type="text" id="AddServiceName" class="easyui-validatebox seller_nick1 seller_nick1" value=""
                           data-name="员工名称" autocomplete="off">
                </li>
                <li>
                    <label for="AddMoumouName">账号名：</label>
                    <input type="text" id="AddMoumouName" class="easyui-validatebox seller_nick1 seller_nick1" value=""
                           data-name="最后修改人" autocomplete="off">
                </li>
                <li>
                    <input id="Addsearch" type="button" class="search_btn" value="查询"/>
                    <input id="AddresetCondition" type="button" class="rewrite_btn" value="重置"/>
                </li>
            </ul>

        </div>

        <!--结果-->
        <div id="AddContent">
            <div class="padd_b_10">
                <div id="groupSearchTab" style="width: 100%;">
                </div>
            </div>
        </div>

        <input type="button" id="addToGroup" value="添加到分组">
        <input type="button" id="removeService" value="移除客服">


        <!--结果-->
        <div id="ReadyContent">
            <div class="padd_b_10">
                <div id="groupAddedTab" style="width: 100%;">
                </div>
            </div>
        </div>

        <div id="EmptyInfo" style="text-align:center;padding:5px;clear: both;display: none">
            <label style="color: #a5a5a5; font-size: 12px;"> 您的分组还没有任何客服，请添加！</label>
        </div>

        <div style="text-align:center;padding:5px;clear: both">
            <input id="submitModify" class="operation_btn_bg con_btn_2" value="提交">
            <input id="cacelModify" class="operation_btn_bg ret_btn_2" value="返回">
        </div>

    </div>

    <div id="lookDetailDialog" class="easyui-dialog" title="修改" style="width: 600px; height: 400px;"
         data-options="iconCls:'pag-list',modal:true,collapsible:false,minimizable:false,maximizable:false,resizable:false,closed:true">
        <div></div>
        <!--结果-->
        <div id="detailContent">
            <div class="padd_b_10">
                <div id="lookDetailTab" style="width: 100%;">
                </div>
            </div>
        </div>
    </div>

    <div id="setEmbedWindow" class="easyui-window"
         data-options="collapsible : false, minimizable : false, maximizable : false, closed: true,
             closable : true, modal : true, title: '布点设置'" style="width: 500px;height:210px;display: none">
        <div class="wrapper" style="padding-top: 20px">
            <%
                Map<String, Integer> embedSet = (Map<String, Integer>) request.getAttribute("embedSet");
                int groupBindForCart = -1, groupBindForOrder = -1;

                //购物车绑定的分组
                if (embedSet.containsKey(EmbedEnum.CART.getName())) {
                    groupBindForCart = embedSet.get(EmbedEnum.CART.getName());
                }

                //订单页绑定的分组
                if (embedSet.containsKey(EmbedEnum.ORDER.getName())) {
                    groupBindForOrder = embedSet.get(EmbedEnum.ORDER.getName());
                }
            %>
            <c:set var="groupBindForCart" value="<%=groupBindForCart%>"></c:set>
            <c:set var="groupBindForOrder" value="<%=groupBindForOrder%>"></c:set>

            <div class="embed-field" data-id="1">
                <label>购物车布点分组设置：</label>
                <select>
                    <option value="-1" <%=groupBindForCart == -1 ? "selected='selected'" : ""%>></option>
                    <c:forEach items="${groupList}" var="group">
                        <option value="${group.groupId}" ${group.groupId eq groupBindForCart ? 'selected="selected"' : ''}> ${group.groupName}</option>
                    </c:forEach>
                </select>
            </div>
            <div class="embed-field" data-id="2">
                <label>订单页布点分组设置：</label>
                <select>
                    <option value="-1" <%=groupBindForOrder == -1 ? "selected='selected'" : ""%>></option>
                    <c:forEach items="${groupList}" var="group">
                        <option value="${group.groupId}"
                                ${group.groupId eq groupBindForOrder ? 'selected="selected"' : ''}>${group.groupName}</option>
                    </c:forEach>
                </select>
            </div>
        </div>
        <div style="text-align:center;padding-top: 30px">
            <span id="saveEmbedSet" class="operation_btn_bg con_btn_2">保存</span>
            <span id="cancelEmbedSet" class="operation_btn_bg ret_btn_2">返回</span>
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
    <script type="text/javascript" src="../js/monitor/service-group-setting.js"></script>
</div>
</body>
</html>
