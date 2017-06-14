<%@ page import="com.feiniu.fnSubAccount.util.JsonUtil" %>
<%@ page import="com.feiniu.fnSubAccount.util.encrypt.Base64" %>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%--
  Created by IntelliJ IDEA.
  User: wangchen
  Date: 2016/3/8
  Time: 11:13
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<!DOCTYPE html>
<html>
<head>
    <title>Document</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=0">
    <meta name="format-detection" content="email=no">
    <meta name="apple-mobile-web-app-capable" content="yes">
    <meta name="apple-touch-fullscreen" content="yes">
    <meta name="apple-mobile-web-app-status-bar-style" content="black">
    <meta name="applicable-device" content="mobile">
    <meta name="apple-itunes-app" content="app-id=921839681">
    <meta name="format-detection" content="telephone=no">
    <script>
        if (/MSIE (6.0|7.0|8.0)/.test(navigator.userAgent)) {
            location.href = location.protocol + '//m.feiniu.com/nonsupport.html';
        }
    </script>

    <link rel="stylesheet" type="text/css" href="../../js/jeasyui/themes/default/easyui.css"/>
    <link rel="stylesheet" type="text/css" href="../../js/jeasyui/themes/icon.css"/>
    <link href="../../css/content.css" rel="stylesheet" type="text/css"/>
    <link href="../../css/base.css" rel="stylesheet" type="text/css"/>
    <link rel="stylesheet" href="../../css/font-awesome.min.css" type="text/css"/>
    <link rel="stylesheet" type="text/css" href="../../css/relative-settings/home-page-config.css"/>

    <link rel="stylesheet" type="text/css" href="../../css/common.min.css">
    <link rel="stylesheet" type="text/css" href="../..//css/backstage.min.css">
    <link rel="stylesheet" type="text/css" href="../../css/edit.min.css">

    <style>
        .field-group > .field-input .edit-field-info {
            font-size: 13px;
        }
    </style>
</head>
<body>

<div class="publish-control ${!isPreview ? '': 'hide'}" style="margin-bottom: 30px">
    <span class="btn btn-primary preview" id="preview">预览</span>
    <span class="btn btn-success publish" id="publish">发布</span>
</div>

<div class="cms-page">
    <header class="header">
        <a href="javascript:;" class="top-back"><i class="icons"></i><span>返回</span></a>

        <h1 class="ell title">咨询类型</h1>
    </header>

    <div id="moduleList">
        <c:if test="${!empty configList}">
            <c:forEach items="${configList}" var="config">
                <div class="module" id="module-${config.id}" data-id="${config.id}"
                     data-module="<%=new String(Base64.encodeBase64(JsonUtil.toJsonString(pageContext.getAttribute("config")).getBytes()))%>">
                    <a class="fc-m-backstage-type">
                        <img class="icon" src="${config.icon}" width="50" height="50"/>

                        <div class="word">
                            <p class="tlt"><c:out value="${config.title}"></c:out></p>

                            <p class="detail"><c:out value="${config.description}"></c:out></p>
                        </div>
                    </a>

                    <div class="baracts ${isPreview ? 'hide': ''}">
                        <div class="baracts-bg"></div>
                        <div class="baracts-rt">
                            <a href="javascript:;" class="ba-btn ba-bar-remove"></a>
                            <a href="javascript:;" class="ba-btn ba-bar-moveup"></a>
                            <a href="javascript:;" class="ba-btn ba-bar-movedown"></a>
                            <a href="javascript:;" class="ba-btn ba-bar-edit"></a>
                        </div>
                    </div>
                </div>
            </c:forEach>
        </c:if>
    </div>

    <a href="javascript:;" id="addModuleBtn" class="add-module-btn">添加</a>

</div>

<div id="boardEditWindow" class="easyui-window"
     data-options="collapsible : false, minimizable : false, maximizable : false, closed: true,
             closable : true, modal : true, title: '新增/编辑', right:'', bottom:''" style="width: 500px;display: none">
    <div class="wrapper">
        <input type="hidden" name="showOrder" class="edit-field"/>

        <div class="field-group">
            <span class="field-label" style="line-height: 80px;"><span class="required">*</span>图标</span>

            <div class="field-input w350" style="vertical-align: middle">
                <div>
                    <div style="position: relative;height: 80px;padding-left: 100px;padding-top: 36px;box-sizing: border-box">
                        <img class="edit-field photo-field" id="boardIcon" name="icon" src="" width="80"
                             height="80"
                             style="position:absolute;left: 0px;top:0px;width: 80px;height: 80px;border: 1px solid #EEEEEE;"
                             alt="请上传图标">

                        <span type="button" id="uploadAdPhoto" class="btn fa fa-upload"
                              style="height: 28px;padding-top: 6px;box-sizing: border-box;position: relative">&nbsp;上传图片</span>
                    </div>
                    <div style="width: 80px;padding: 5px 0px;color: #a5a5a5;font-size: 12px;text-align: center">80*80
                    </div>
                </div>
                <div style="clear: both;">
                    <span class="edit-field-info field-error hide" style="width: 100%">X图片尺寸不符合要求或者没有上传图片，请上传图片</span>
                </div>
            </div>
        </div>
        <div class="field-group">
            <span class="field-label v-top"><span class="required">*</span>标题</span>

            <div class="field-input w350">
                <div>
                    <input class="edit-field" name="title" maxlength="6" style="width: 55%;-webkit-ime-mode:disabled"/>
                    <span data-maxlength="6" class="edit-field-info field-info" style="width: 42%">（最多输入6个文字）</span>
                </div>
                <div style="clear: both;">
                    <span class="edit-field-info field-error hide">X请填写标题</span>
                </div>
            </div>
        </div>
        <div class="field-group">
            <span class="field-label v-top"><span class="required">*</span>描述</span>

            <div class="field-input w350">
                <div>
                    <input class="edit-field" name="description" maxlength="12" style="width: 55%"/>
                    <span class="edit-field-info field-info" style="width: 42%">（最多输入12个文字）</span>
                </div>
                <div style="clear:both;">
                    <span class="edit-field-info field-error hide">X请填写描述</span>
                </div>
            </div>
        </div>
        <div class="field-group">
            <span class="field-label v-top">绑定分组</span>

            <div class="field-input w350">
                <div>
                    <input class="edit-field" name="isBindGroup" id="isBindGroupCheckBox" type="checkbox"/>
                </div>
            </div>
        </div>
        <div class="field-group group-select">
            <span class="field-label v-top"><span class="required hide">*</span>分组名称</span>

            <input type="hidden" class="edit-field" name="showOrder">

            <div class="field-input w350">
                <div>
                    <select class="easyui-combobox edit-field" name="groupID" id="groupID" style="width: 55%;">
                        <option value="-1" selected>&nbsp;</option>
                        <c:forEach items="${groupList}" var="group">
                            <option value="${group.groupId}">${group.groupName}</option>
                        </c:forEach>
                    </select>
                </div>
                <div>
                    <span class="edit-field-info field-error hide">X请选择分组</span>
                </div>
            </div>
        </div>
        <div class="field-group">
            <span class="field-label v-top">订单信息</span>

            <div class="field-input w350">
                <div>
                    <label class="edit-field-info field-info">
                        <input class="edit-field" name="isBindUserOrder" type="checkbox"/>(用于设置买家咨询在线客服是否需要输入订单信息)
                    </label>
                </div>
            </div>
        </div>
    </div>
    <div class="edit-window-footer">
        <input id="saveEditBoard" class="operation_btn_bg con_btn_2" value="保存"/>
        <input id="cancelEditBoard" class="operation_btn_bg can_btn_2" value="取消"/>
    </div>
</div>
<div id="confirmDeleteWindow" class="easyui-window"
     data-options="collapsible : false, minimizable : false, maximizable : false, closed: true,
             closable : true, modal : true, title: '确认是否删除'" style="width: 300px;display: none">
    <p class="confirm-delete-panel">确认删除这个版块吗？</p>

    <div style="text-align:center;padding:5px 5px 10px;">
        <input id="confirmDelete" class="confirm-btn" value="确认"/>
        <input id="cancelDelete" class="confirm-btn cancel" value="取消"/>
    </div>
</div>

<script type="text/javascript" src="../../js/jquery.min.js"></script>
<script type="text/javascript" src="../../js/json2.js"></script>
<script type="text/javascript" src="../../js/jeasyui/jquery.easyui.min.js"></script>
<script type="text/javascript" src="../../js/jeasyui/validateExtend.js"></script>
<script type="text/javascript" src="../../js/jeasyui/locale/easyui-lang-zh_CN.js"></script>
<script type="text/javascript" src="../../js/jeasyui/validateExtend.js"></script>
<script type="text/javascript" src="../../js/myCalendar/WdatePicker.js"></script>
<script type="text/javascript" src="../../js/util/generalUtil.js"></script>
<script type="text/javascript" src="../../js/util/easyuiUtil.js"></script>
<script type="text/javascript" src="../../js/plugin/uploader/jquery.uploader.js"></script>
<script type="text/javascript" src="../../js/relative-settings/touch-portal-board-config.js"></script>
</body>
</html>
