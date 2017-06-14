<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ page import="com.feiniu.subAccount.util.JsonUtil" %>
<%--
  Created by IntelliJ IDEA.
  User: wangchen
  Date: 2015/2/7
  Time: 14:55
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title>客服分组</title>
    <jsp:include page="../common/meta-info.jsp"></jsp:include>
    <link type="text/css" rel="stylesheet" href="../css/v0.0.1.min.css"/>
    <link type="text/css" rel="stylesheet" href="../css/reset.css"/>
    <link type="text/css" rel="stylesheet" href="../css/employee.css"/>
    <link type="text/css" rel="stylesheet" href="../css/left-tree.css"/>
    <link type="text/css" rel="stylesheet" href="../css/zzh_zdy.css"/>
    <link type="text/css" rel="stylesheet" href="../css/dialog.css"/>
</head>
<body>
<tempdata data-merchantid="${merchantID}"
          data-subaccountlist='<%=JsonUtil.toJsonString(request.getAttribute("subMerchantInfo"))%>'
          data-servicegrouplist='<%=JsonUtil.toJsonString(request.getAttribute("serviceGroupList"))%>'
          data-storeinfo='<%=JsonUtil.toJsonString(request.getAttribute("storeInfo"))%>'
          data-showplacemapping='<%=JsonUtil.toJsonString(request.getAttribute("serviceGroupShowPlaceList"))%>'
        >
</tempdata>
<div class="main-content-area">
    <!--面包屑-->
    <div class="ui-crumbs">
        <p><b>当前位置：</b><a href="javascript:;" class="ui-clickable">商家中心</a><span> &gt; 客服分组</span></p>
    </div>
    <!--面包屑-->
    <div class="ui-tab">
        <ul class="tab-nav">
            <li class="tab-nav-item"><a class="tabsDivSelect" href="../diverse/diverse-config.htm">分流设置</a></li>
            <li class="tab-nav-item cur"><a href="#">客服分组</a></li>
            <li class="tab-nav-item"><a href="../store-light/store-light-config.htm">店铺亮灯</a></li>
        </ul>
        <ul class="tab-panel pt-20">
            <li class="tab-panel-item cur">

                <div class="structure-content">
                    <div class="structureUL" id="mainContainer">
                        <div id="groupController" class="structureTitle">
                            <h2>亮灯分组</h2><a class="newDepartment"></a>
                        </div>
                        <div id="j_left_tree" class="left-tree-warp">
                            <ul class="left-tree"></ul>
                            <div class="op-panel">
                                <a rel="creat" href="javascript:void(0);">新建亮灯分组</a>
                                <a rel="rename" href="javascript:void(0);">重命名</a>
                                <a rel="delete" href="javascript:void(0);">删除</a>
                            </div>
                        </div>
                    </div>
                    <div class="li-content">
                        <div class="g-right-title">
                            <ul class="g-right-Tabs">
                                <li><a class="selected">客服管理</a></li>
                                <li><a id="setServiceGroupShowPlaceNav">分组分流设置</a></li>
                            </ul>
                        </div>
                        <div class="g-right-content">
                            <div class="li-box">
                                <div style="float: left;margin-bottom: 15px">
                                    <button id="addSubAccountToGroup" class="ui-button red" type="sureBtn">添加客服</button>
                                    <button id="removeSubAccountFromGroup" class="ui-button red" type="sureBtn">移除客服
                                    </button>
                                </div>

                                <dl class="commonSelect w130" style="position:relative;">
                                    <dt class="w106 dtBGpostion116">移除到其他分组</dt>
                                    <dd style="position:absolute;top:28px;">
                                        <ul id="selectServiceGroupList" class="w120">
                                            <li><a href="#">移除到其他分组</a></li>
                                            <c:forEach items="${serviceGroupList}" var="serviceGroup">
                                                <li><a href="#" id="selGroup${serviceGroup.getGroupID()}"
                                                       data-groupid="${serviceGroup.getGroupID()}">${serviceGroup.getGroupName()}</a>
                                                </li>
                                            </c:forEach>
                                        </ul>
                                    </dd>
                                </dl>
                                <div class="search hide">
                                    <input type="text" class="searchTxt" value="输入员工帐号名"/>
                                    <input type="button" class="searchBtn" value=""/>
                                </div>
                                <table width="100%" border="1" class="structure-Table">
                                    <thead>
                                    <tr class="tableHeader">
                                        <th width="75%"><span><input type="checkbox" class="commonCheckBox"
                                                                     id="checkAllSubAcccountController"/>
                                    帐号名</span></th>
                                        <th width="25%"><span><a class="aShowSelectBox">分流状态</a>
                                    <div class="showSingleUlBox">
                                        <ul id="subAccountShowFilterOptions">
                                            <li id="filterNone"><a href="javascript:;">所有</a></li>
                                            <li id="filterByOnline"><a href="javascript:;">分流</a></li>
                                            <li id="filterByOffline"><a href="javascript:;">不分流</a></li>
                                        </ul>
                                    </div></span></th>
                                    </tr>
                                    </thead>
                                    <tbody id="selSubAccountListUnderGroup">
                                    </tbody>
                                </table>
                            </div>
                        </div>
                        <div class="g-right-content" style="display:none">
                            <div id="showListFZFL">
                                <p class="attention"><em></em>该分组接待从以下页面点击哞哞亮灯的买家<a class="editA disable" id="editFZFL">点击编辑</a>
                                </p>
                                <ul class="setting_ul">
                                    <li><a id="showPlace1">1、商品搜索列表页</a></li>
                                    <li><a id="showPlace2">2、商家店铺页面</a></li>
                                    <li><a id="showPlace3">3、购物车结算页</a></li>
                                    <li><a id="showPlace4">4、已购买商品（未发货）</a></li>
                                    <li><a id="showPlace5">5、已购买商品（已发货）</a></li>
                                    <li><a id="showPlace6">6、其它</a></li>
                                </ul>
                            </div>
                            <div id="showeditFZFL">
                                <div class="li-box">
                                    <p class="attention">该分组接待从以下页面点击哞哞亮灯的买家<a class="editA" id="returnFZFL">保存设置</a>
                                    </p>
                                    <table id="showPlaceConfigTable" width="100%" border="1" class="structure-Table">
                                        <thead>
                                        <tr class="tableHeader">
                                            <th width="50%"><span><input type="checkbox" class="commonCheckBox"
                                                                         id="allcheck"/>
                                    页面</span></th>
                                            <th width="50%"><span>当前接待分组</span></th>
                                        </tr>
                                        </thead>
                                        <tbody>
                                        <c:forEach items="${allShowPlace}" var="showPlace">
                                            <tr id="place${showPlace.configID}" data-placeid="${showPlace.configID}"
                                                data-mappingID="0">
                                                <td><input type="checkbox" class="commonCheckBox" id=""/>
                                                        ${showPlace.configName}
                                                </td>
                                                <td>暂无分组</td>
                                            </tr>
                                        </c:forEach>
                                        </tbody>
                                    </table>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </li>
        </ul>
    </div>
</div>
<script id="alert-dialog-tpl" type="text/template">
    <div class="p-alert-content">
        <p class="attion"><em class="attentionPic"></em>一个子帐号只能参与一个亮灯分组里分流</p>
        <table class="structure-Table" border="1" width="100%">
            <thead>
            <tr style="background: rgb(246, 246, 246);">
                <th width="40%"><input id="dialogCheckAllSubAccountListController" type="checkbox"
                                       class="zzhCommonCheck">帐号名
                </th>
                <th width="30%">已在分组</th>
                <th width="30%"><span><a class="aShowSelectBox">全部部门</a>
                                <div class="panel-js showSingleUlBox" style="width:130px;">
                                    <ul>
                                        <li><a href="javascript:;">在线客服</a></li>
                                        <li><a href="javascript:;">店铺运营</a>
                                            <ul>
                                                <li><a href="javascript:;">在线客服</a></li>
                                                <li><a href="javascript:;">店铺运营</a></li>
                                            </ul>
                                        </li>
                                        <li><a href="javascript:;">活动推广</a></li>
                                        <li><a href="javascript:;">仓储物流</a></li>
                                        <li><a href="javascript:;">店铺装修</a></li>
                                    </ul>
                                </div></span></th>
            </tr>
            </thead>
            <tbody id="subAccountListPanelInDialog">
            </tbody>
        </table>
    </div>
</script>

<script type="text/javascript" src="../js/jquery-1.11.1.min.js"></script>
<script type="text/javascript" language="javascript" src="../js/fnx-pack-min.js"></script>
<script type="text/javascript" language="javascript" src="../js/jquery.tree.js"></script>
<script type="text/javascript" language="javascript" src="../js/leftTree.js"></script>
<script type="text/javascript" language="javascript" src="../js/zzh.js"></script>
<script type="text/javascript" src="../js/service-group/service-group.js"></script>
</body>
</html>
