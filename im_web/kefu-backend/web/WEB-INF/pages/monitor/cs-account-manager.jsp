<%@ page import="com.feiniu.fnSubAccount.bean.OrganNameInfo" %>
<%@ page import="com.feiniu.fnSubAccount.bean.CSBackendInfo" %>
<%@ page import="com.feiniu.fnSubAccount.util.JsonUtil" %>
<%@ page import="org.apache.commons.collections.CollectionUtils" %>
<%@ page import="java.util.List" %>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%
  List<OrganNameInfo> serviceGroupDetailList = (List<OrganNameInfo>) request.getAttribute("serviceGroupDetailList");
  List<CSBackendInfo> allMemberList = (List<CSBackendInfo>) request.getAttribute("allMemberList");
  boolean hasGroupDetailList = CollectionUtils.isNotEmpty(serviceGroupDetailList);
%>

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
  <link href="../js/My97DatePicker/skin/WdatePicker.css" rel="stylesheet" type="text/css">
  <link href="../css/cs-account-manager/customer.css" rel="stylesheet" type="text/css" />
</head>
<body>
<div id="main_warp">
  <tmpdata data-allMemberList='<%=JsonUtil.toJsonString(allMemberList)%>'>
    </tmpdata>
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
  <div class="searchbar">
    <ul>
      <form action="" method="" >
        <li class="pull-left">
          <label id="CSNameLable" class="labelplacehold">哞哞登录名：</label>
          <input type="text" id="CSName" class="easyui-combobox seller_nick1 seller_nick1" value=""
          auto-complete="off" data-name="哞哞登录名" autocomplete="off">
        </li>

        <li>
          <label for="OwnerOrgnName">账号所属组织：</label>
          <select id="OwnerOrgnName" name="OwnerOrgnName" value="" class="ks_select" style="width: 190px;">
            <option value=0 >全部组织</option>
            <c:if test="<%=hasGroupDetailList%>">
              <c:forEach items="<%=serviceGroupDetailList%>" var="serviceGroupDetail">
                <option value=${serviceGroupDetail.id}>${serviceGroupDetail.orgnName}</option>
              </c:forEach>
            </c:if>
          </select>
        </li>

        <li>
          <label for="OrgnMemberName">账号所属人员：</label>
          <select id="OrgnMemberName" name="OrgnMemberName" value="" class="ks_select" style="width: 190px;">
            <option value=0 >全部人员</option>
            <c:forEach items="<%=allMemberList%>" var="memberInfo">
              <option value=${memberInfo.operatorid}>${memberInfo.operatorrealname}</option>
            </c:forEach>
          </select>
        </li>
        <li>
          <label for="ImportOrgnName">导入人员组织：</label>
          <select id="ImportOrgnName" name="ImportOrgnName" value="" class="ks_select" style="width: 190px;">
            <option value=0 >全部组织</option>
            <c:forEach items="<%=serviceGroupDetailList%>" var="serviceGroupDetail">
              <option value=${serviceGroupDetail.id}>${serviceGroupDetail.orgnName}</option>
            </c:forEach>
          </select>
        </li>

        <li>
          <label for="ImportMemberName">导入人员：</label>
          <select id="ImportMemberName" name="ImportMemberName" value="" class="ks_select" style="width: 190px;">
            <option value=0 >全部人员</option>
            <c:forEach items="<%=allMemberList%>" var="memberInfo">
              <option value=${memberInfo.operatorid}>${memberInfo.operatorrealname}</option>
            </c:forEach>
          </select>
        </li>

        <li class="pull-left">
          <label for="MemberStatus">账号状态：</label>
          <select id="MemberStatus" name="MemberStatus" value="" class="ks_select" style="width: 190px;">
            <option value=888 >全部</option>
            <option value=0 >停用</option>
            <option value=1 >启用</option>
          </select>
        </li>
        <li>
          <label>导入时间：</label>
          <input id="startTime" class="dateSettingTxt icon_calendar" type="text" style="height: 26px" data-name="起时间" autocomplete="off">
          <span class="col_to">-</span>
          <input id="endTime" class="dateSettingTxt icon_calendar" type="text" style="height: 26px" data-name="止时间" autocomplete="off">
          <input type="button" id="clearTime" class="clear_data_btn" value="清空"/>
        </li>

      </form>

      <li class="pull-left w55">
        <input type="button" id="queryCsAccountInfo" class="search_btn" value="查询"/>
      </li>
      <li>
        <input id="resetCondition" type="button" class="rewrite_btn" value="重置">
      </li>
    </ul>
  </div>
  <!--导入excel表格按钮-->
  <div >

    <input type="button" id="excelImport" class="uploader operation_btn_bg add_btn_2" value="导入" />
    <input type="button" id="startUser" class="operation_btn_bg add_btn_2" value="启用" onclick="startuser()" />
    <input type="button" id="stopUser" class="operation_btn_bg add_btn_2" value="停用" onclick="stopuser()" />
    <input type="button" id="deleteUser" class="operation_btn_bg add_btn_2" value="删除" onclick="deleteuser()" />
    <a href="../material/template/客服账号导入.xlsx" class="template-download" title="点击下载客服账号导入模板">客服账号导入模板下载</a>
  </div>

  <br>
  <!--结果-->
  <div id="content">
    <div class="padd_b_10">
      <div id="searchedTab" style="width: 100%;">
      </div>
    </div>
  </div>


  <div id="centerDiv" data-options="region:'center',border:false">
    <table id="networkQueryGrid"
           data-options="queryForm:'#queryForm',title:'查询结果',iconCls:'pag-list'"></table>
  </div>

  <div id="modifycsDialog" class="easyui-dialog" title="修改" style="width: 400px; height: 300px;"
       data-options="iconCls:'pag-list',modal:true,collapsible:false,minimizable:false,maximizable:false,resizable:false,closed:true">
    <div style="margin-left: 5px;margin-right: 5px;margin-top: 5px;">
      <div class="cus_head">
        <ul>
          <li class="on"  id="basicInfoPage" >基本信息</li>
          <li id="passwordChangePage">密码</li>
        </ul>
      </div>
      <div class="cl"></div>

      <div class="content_cus" id="basicInfo">
        <ul>
          <br>
          <li >
              <p><label>&nbsp;&nbsp;&nbsp</label><span style="font-size:13px color:#333333">账号名：</span>
                <label>&nbsp;&nbsp;&nbsp</label>
                <span class="moumouName">客服001</span>
              </p>
          </li>
          <br>
          <li>
            <p><label>&nbsp;&nbsp;&nbsp</label><span class="required">*</span><span color:#333333;>账号所属组织：</span>
              <select id="ModifyOrgnName" name="ImportOrgnName" value="" class="ks_select" style="width: 190px;">
                <option value=0 >暂无组织</option>
                <c:forEach items="<%=serviceGroupDetailList%>" var="serviceGroupDetail">
                  <option value=${serviceGroupDetail.id}>${serviceGroupDetail.orgnName}</option>
                </c:forEach>
              </select>
            </p>
          </li>
          <br>
          <li>
            <p><label>&nbsp;&nbsp;&nbsp</label><span class="required">*</span><span color:#333333;>账号所属员工：</span>
              <select id="ModifyMemberName" name="ImportMemberName" value="" class="ks_select" style="width: 190px;">
                <option value=0 >暂无关联人员</option>
                <c:forEach items="<%=allMemberList%>" var="memberInfo">
                  <option value=${memberInfo.operatorid}>${memberInfo.operatorrealname}</option>
                </c:forEach>
              </select>
            </p>
          </li>
          <br>
          <li>
            <label>&nbsp;&nbsp;&nbsp</label><span class="required">*</span><label id="limitNumberLable" class="labelplacehold">接待上限：<label>&nbsp;&nbsp</label>
            </label>
              <input type="text" id="limitNumber" class="easyui-validatebox seller_nick1" value=""
                     auto-complete="off" data-name="哞哞登录名" autocomplete="off">
          </li>
          <br>
          <li>
            <div style="text-align:center;padding:5px;clear: both">
              <input id="changeInfoSave" class="operation_btn_bg con_btn_2" value="提交">
              <input id="changeInfoCancel" class="operation_btn_bg ret_btn_2" value="返回">
            </div>
          </li>
        </ul>
      </div>

      <div class="content_cus" id="password" style="display: none">
        <ul>
          <br>
          <li>
            <p><label>&nbsp;&nbsp;&nbsp</label><span >账号名：<label>&nbsp;&nbsp</label></span><span class="moumouName">客服001</span></p>
          </li>
          <br>
          <li>
            <label>&nbsp;&nbsp;&nbsp</label><span class="required">*</span><label id="passwordLable" class="labelplacehold">新密码：<label>&nbsp</label></label>
            <input type="password" id="passwordInput" maxlength='18' class="easyui-validatebox seller_nick1" value=""
                   auto-complete="off" data-name="哞哞登录名" autocomplete="off">
          </li>
          <br>
          <li>
            <label>&nbsp;&nbsp;&nbsp</label><span class="required">*</span><label id="passwordConfimLable" class="labelplacehold">再输一次：</label>
            <input type="password" id="passwordConfim" maxlength='18' class="easyui-validatebox seller_nick1" value=""
                   auto-complete="off" data-name="哞哞登录名" autocomplete="off">
          </li>
          <br><br>
          <li>
            <div style="text-align:center;padding:5px;clear: both">
              <input id="changePasswordSave" class="operation_btn_bg con_btn_2" value="提交">
              <input id="changePasswordCancel" class="operation_btn_bg ret_btn_2" value="返回">
            </div>
          </li>
        </ul>

      </div>
    </div>
  </div>
</div>
<script type="text/javascript" src="../js/jquery.min.js"></script>
<script type="text/javascript" language="javascript" src="../js/employee.js"></script>
<script type="text/javascript" src="../js/jeasyui/jquery.easyui.min.js"></script>
<script type="text/javascript" src="../js/jeasyui/validateExtend.js"></script>
<script type="text/javascript" src="../js/jeasyui/locale/easyui-lang-zh_CN.js"></script>
<script type="text/javascript" src="../js/jeasyui/validateExtend.js"></script>
<script type="text/javascript" src="../js/My97DatePicker/WdatePicker.js"></script>
<script type="text/javascript" src="../js/uploader/jquery.uploader.js"></script>
<script type="text/javascript" src="../js/util/generalUtil.js"></script>
<script type="text/javascript" src="../js/plugin/jquery.fancybox.pack.js"></script>
<script type="text/javascript" src="../js/monitor/cs-account-manager.js"></script>
</body>
</html>

