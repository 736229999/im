<%@ page import="com.feiniu.fnSubAccount.consts.SystemConfig" %>
<%@ page import="com.feiniu.fnSubAccount.domain.AfterSaleBoardConfig" %>
<%@ page import="com.feiniu.fnSubAccount.util.json.JsonUtil" %>
<%@ page import="org.apache.commons.lang.StringUtils" %>
<%@ page import="com.feiniu.fnSubAccount.domain.KefuIndexConfig" %>
<%@ page import="org.codehaus.jackson.JsonNode" %>
<%@ page import="com.feiniu.fnSubAccount.util.json.JsonNodeUtil" %>
<%@ page import="java.util.*" %>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%--
  Created by IntelliJ IDEA.
  User: wangchen
  Date: 2015/10/14
  Time: 14:10
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title>运营后台管理系统</title>
    <link rel="stylesheet" type="text/css" href="../../js/jeasyui/themes/default/easyui.css"/>
    <link rel="stylesheet" type="text/css" href="../../js/jeasyui/themes/icon.css"/>
    <link href="../../css/content.css" rel="stylesheet" type="text/css"/>
    <link href="../../css/base.css" rel="stylesheet" type="text/css"/>
    <link href="../../css/page.css" rel="stylesheet" type="text/css"/>
    <link href="../../css/ui_pop.css" rel="stylesheet" type="text/css"/>
    <link rel="stylesheet" type="text/css" href="../../css/relative-settings/home-page-config.css"/>
    <link rel="stylesheet" href="../../css/font-awesome.min.css" type="text/css"/>
</head>
<body>
<div class="publish-control hide">
    <span class="btn btn-primary preview" id="preview-edit">预览</span>
    <span class="btn btn-success publish" id="publish-edit">发布</span>
</div>
<div class="publish-edit-section">
    <div class="publish-wrapper" style="width: 1190px;margin: 0px auto">
        <div class="w990" style="margin: 0px auto">
            <div class="on_line">
                <div class="logo"><img src="../../images/logo.jpg"/></div>
            </div>
            <div class="use editable notice-edit-panel" data-href="noticeEditWindow">
                <p class="edit-field" name="notice">${kefuIndexConfig.notice}</p>
            </div>
            <div class="Customer_service">
                <h2>售后服务咨询</h2>

                <c:forEach items="${afterSaleBoardConfigList}" var="boardConfig" varStatus="status">
                <span class="case ${status.last ? '' : 'mr6'} editable after-sale-board-edit-panel"
                      data-href="groupEditWindow">
                    <h1>
                        <div class="title">
                            <i class="on on_${status.index + 1}"><i class="edit-field"
                                                                    name="title">${boardConfig.title}</i></i>
                        </div>
                    </h1>
                    <p>
                        <%
                            AfterSaleBoardConfig currentConfig = (AfterSaleBoardConfig) (pageContext.getAttribute("boardConfig"));
                            String descriptionListJson = currentConfig.getDescriptionListJson();
                            if (StringUtils.isEmpty(descriptionListJson)) {
                                descriptionListJson = "[]";
                            }
                            List<Map> descriptionList = JsonUtil.toList(descriptionListJson, Map.class);

                            if (descriptionList.size() < SystemConfig.DESCRIPTION_ITEM_NUMBER) {
                                Map emptyValueMap = new HashMap(1);
                                emptyValueMap.put("v", "");
                                for (int i = descriptionList.size(); i < SystemConfig.DESCRIPTION_ITEM_NUMBER; i++) {
                                    descriptionList.add(emptyValueMap);
                                }
                            }
                        %>

                        <c:forEach items="<%=descriptionList%>" var="descriptionItem" varStatus="descItemStatus">
                            <span class="edit-field"
                                  name="description_${descItemStatus.index+1}"><%=((Map) (pageContext.getAttribute("descriptionItem"))).get("v")%></span>
                        </c:forEach>
                    </p>
                    <a class="bule_btn ${boardConfig.isBindUserOrder == '1'? ' btn-bind-order':''}" href="#">立即咨询</a>

                    <i name="boardID" class="edit-field hide">${boardConfig.boardID}</i>
                    <i name="isBindGroup" class="edit-field hide">${boardConfig.isBindGroup}</i>
                    <i name="groupID" class="edit-field hide">${boardConfig.groupID}</i>
                    <i name="isBindUserOrder" class="edit-field hide">${boardConfig.isBindUserOrder}</i>
                </span>
                </c:forEach>
            </div>
            <div class="ad">
                <div class="case mr6 editable ad-edit-panel" data-href="adBannerEditWindow">
                    <a class="edit-field-fake to-href" name="adBannerImageHref" target="_blank"
                       href="${kefuIndexConfig.adBannerLeftImageHref}" title="${kefuIndexConfig.adBannerLeftImageHref}">
                        <img class="edit-field-fake to-show" name="adBannerImageUrl"
                             src="${kefuIndexConfig.adBannerLeftImageUrl}" width="100%" height="100%">
                        <i name="adBannerLeftImageUrl"
                           class="edit-field hide">${kefuIndexConfig.adBannerLeftImageUrl}</i>
                        <i name="adBannerLeftImageHref"
                           class="edit-field hide">${kefuIndexConfig.adBannerLeftImageHref}</i>
                    </a>
                </div>
                <div class="case editable ad-edit-panel" data-href="adBannerEditWindow">
                    <a class="edit-field-fake to-href" name="adBannerImageHref" target="_blank"
                       href="${kefuIndexConfig.adBannerRightImageHref}"
                       title="${kefuIndexConfig.adBannerRightImageHref}">
                        <img class="edit-field-fake to-show" name="adBannerImageUrl"
                             src="${kefuIndexConfig.adBannerRightImageUrl}" width="100%" height="100%">
                        <i name="adBannerRightImageUrl"
                           class="edit-field hide">${kefuIndexConfig.adBannerRightImageUrl}</i>
                        <i name="adBannerRightImageHref"
                           class="edit-field hide">${kefuIndexConfig.adBannerRightImageHref}</i>
                    </a>
                </div>
            </div>
        </div>
        <div class="publish-footer">
            <%
                KefuIndexConfig kefuIndexConfig = (KefuIndexConfig) request.getAttribute("kefuIndexConfig");
                JsonNode dataNode = JsonUtil.toNode(kefuIndexConfig.getJsonOfFeiniuIndex());
                List<String> cssList = Collections.emptyList();
                String footer = "";

                if (dataNode != null && dataNode.has("css")) {
                    cssList = JsonUtil.toList(JsonNodeUtil.getString(dataNode, "css"), String.class);
                    footer = JsonNodeUtil.getString(dataNode, "footer");
                }
            %>
            <c:forEach items="<%=cssList%>" var="cssItem">
                <link type="text/css" href="<%=pageContext.getAttribute("cssItem")%>" charset="utf-8" rel="stylesheet">
            </c:forEach>
            <%=footer%>
        </div>
    </div>
</div>

<!--弹窗-->
<div style="display:none">
    <div class="popu" id="p_im" style="display:block">

        <div class="Order">
            <table width="100%" border="0" cellspacing="0" cellpadding="0">
                <tr>
                    <td>订单编号<br/>201208cp3012041</td>
                    <td><img src="../../images/pc.jpg"/></td>
                    <td><img src="../../images/pc1.jpg"/></td>
                    <td>2015-08-02<br/>13:25:25</td>
                    <td>交易成功</td>
                    <td><a class="bule_btn_1" href="#">开始咨询</a></td>
                </tr>
                <tr>
                    <td colspan="5">未查询到您最近一个月内的订单信息</td>
                    <td><a class="bule_btn_1" href="#">继续咨询</a></td>
                </tr>
            </table>
        </div>
        <div class="num">
            <p>您还可以输入订单编号：</p>

            <div class="Digital">
                <div class="Start">
                    <input type="text"/>
                    <a class="red_btn_1" href="#">开始咨询</a>
                </div>
                <div class="Prompt_red">
                    <i></i>
                    <span>订单号输入错误，请确认后在输入</span>
                </div>
                <div class="Prompt_bule">
                    <i></i>
                    <span>订单号输入正确，请点击“开始咨询”</span>
                </div>
                <a js-hook="shutdown" class="cane" href="#">取消</a>
                <em>您也可以前往<a class="popu_bule" href="#">订单中心</a>咨询</em>
            </div>
        </div>
    </div>
</div>
<!--弹窗-->

<div id="noticeEditWindow" class="easyui-window"
     data-options="collapsible : false, minimizable : false, maximizable : false, closed: true,
             closable : true, modal : true, title: '编辑'" style="width: 500px;display: none">
    <div class="wrapper" style="padding-top: 15px">
        <div class="field-group">
            <span class="field-label v-top"><span class="required">*</span>文字描述：</span>

            <div class="field-input w350">
                <textarea class="edit-field" name="notice" cols="30" rows="3" maxlength="100"
                          style="width: 100%;height: auto;"></textarea>

                <div>
                    <span class="edit-field-info field-error pull-left hide">X 请输入文字描述</span>
                    <span class="edit-field-info field-info" style="float: right">（100字以内）</span>
                </div>
            </div>
        </div>
    </div>
    <div class="edit-window-footer">
        <input id="saveEditNotice" class="operation_btn_bg con_btn_2" value="保存"/>
        <input id="cancelEditNotice" class="operation_btn_bg can_btn_2" value="取消"/>
    </div>
</div>
<div id="groupEditWindow" class="easyui-window"
     data-options="collapsible : false, minimizable : false, maximizable : false, closed: true,
             closable : true, modal : true, title: '编辑', right:'', bottom:''" style="width: 500px;display: none">
    <div class="wrapper">
        <input type="hidden" name="boardID" class="edit-field"/>

        <div class="field-group">
            <span class="field-label v-top"><span class="required">*</span>标题：</span>

            <div class="field-input w350">
                <div>
                    <input class="edit-field" name="title" maxlength="8" style="width: 55%;-webkit-ime-mode:disabled"/>
                    <span data-maxlength="8" class="edit-field-info field-info" style="width: 45%">（8个字以内）</span>
                </div>
                <div>
                    <span class="edit-field-info field-error hide">X请填写标题</span>
                </div>
            </div>
        </div>
        <div class="field-group">
            <span class="field-label v-top"><span class="required">*</span>描述1：</span>

            <div class="field-input w350">
                <div>
                    <input class="edit-field" name="description_1" maxlength="8" style="width: 55%"/>
                    <span class="edit-field-info field-info" style="width: 45%">（8个字以内）</span>
                </div>
                <div>
                    <span class="edit-field-info field-error hide">X请填写描述</span>
                </div>
            </div>
        </div>
        <div class="field-group">
            <span class="field-label v-top"><span class="required">*</span>描述2：</span>

            <div class="field-input w350">
                <div>
                    <input class="edit-field" name="description_2" maxlength="8" style="width: 55%"/>
                    <span class="edit-field-info field-info" style="width: 45%">（8个字以内）</span>
                </div>
                <div>
                    <span class="edit-field-info field-error hide">X请填写描述</span>
                </div>
            </div>
        </div>
        <div class="field-group">
            <span class="field-label v-top">描述3：</span>

            <div class="field-input w350">
                <div>
                    <input class="edit-field" name="description_3" maxlength="8" style="width: 55%"/>
                    <span class="edit-field-info field-info" style="width: 45%">（8个字以内）</span>
                </div>
                <div>
                    <span class="edit-field-info field-error hide">X请填写描述</span>
                </div>
            </div>
        </div>
        <div class="field-group">
            <span class="field-label v-top">描述4：</span>

            <div class="field-input w350">
                <div>
                    <input class="edit-field" name="description_4" maxlength="8" style="width: 55%"/>
                    <span class="edit-field-info field-info" style="width: 45%">（8个字以内）</span>
                </div>
                <div>
                    <span class="edit-field-info field-error hide">X请填写描述</span>
                </div>
            </div>
        </div>
        <div class="field-group">
            <span class="field-label v-top">描述5：</span>

            <div class="field-input w350">
                <div>
                    <input class="edit-field" name="description_5" maxlength="8" style="width: 55%"/>
                    <span class="edit-field-info field-info" style="width: 45%">（8个字以内）</span>
                </div>
                <div>
                    <span class="edit-field-info field-error hide">X请填写描述</span>
                </div>
            </div>
        </div>
        <div class="field-group">
            <span class="field-label v-top">描述6：</span>

            <div class="field-input w350">
                <div>
                    <input class="edit-field" name="description_6" maxlength="8" style="width: 55%"/>
                    <span class="edit-field-info field-info" style="width: 45%">（8个字以内）</span>
                </div>
                <div>
                    <span class="edit-field-info field-error hide">X请填写描述</span>
                </div>
            </div>
        </div>
        <div class="field-group">
            <span class="field-label v-top">描述7：</span>

            <div class="field-input w350">
                <div>
                    <input class="edit-field" name="description_7" maxlength="8" style="width: 55%"/>
                    <span class="edit-field-info field-info" style="width: 45%">（8个字以内）</span>
                </div>
                <div>
                    <span class="edit-field-info field-error hide">X请填写描述</span>
                </div>
            </div>
        </div>
        <div class="field-group">
            <span class="field-label v-top">绑定分组：</span>

            <div class="field-input w350">
                <div>
                    <input class="edit-field" name="isBindGroup" id="isBindGroupCheckBox" type="checkbox"/>
                </div>
            </div>
        </div>
        <div class="field-group group-select">
            <span class="field-label v-top"><span class="required hide">*</span>分组名称：</span>

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
            <span class="field-label v-top">订单信息：</span>

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
        <input id="saveEditGroup" class="operation_btn_bg con_btn_2" value="保存"/>
        <input id="cancelEditGroup" class="operation_btn_bg can_btn_2" value="取消"/>
    </div>
</div>
<div id="adBannerEditWindow" class="easyui-window"
     data-options="collapsible : false, minimizable : false, maximizable : false, closed: true,
             closable : true, modal : true, title: '编辑'" style="width: 580px;display: none">
    <div class="wrapper">
        <div>
            <div class="field-group">
                <span class="field-label v-top"><span class="required">*</span>图片文件名：</span>

                <div class="field-input w400">
                    <div>
                        <input class="edit-field ad-banner-image-url" name="adBannerImageUrl" style="width: 70%"
                               readonly/>

                        <div style="float: left;margin-left: 30px">
                        <span type="button" id="uploadAdPhoto" class="btn fa fa-upload"
                              style="height: 28px;padding-top: 6px;box-sizing: border-box">&nbsp;上传图片</span>
                        </div>
                    </div>
                    <div>
                        <span class="edit-field-info field-info" style="width: 100%">图片尺寸要求：<span class="required">490*200</span></span>
                        <span class="edit-field-info field-error hide" style="width: 100%">X图片尺寸不符合要求，请上传图片</span>
                    </div>
                </div>
            </div>
            <div class="field-group">
                <span class="field-label v-top"><span class="required">*</span>链接地址：</span>

                <div class="field-input w400">
                    <div>
                        <input class="edit-field ad-banner-image-href" name="adBannerImageHref" style="width: 100%"/>
                    </div>
                    <div>
                        <span class="edit-field-info field-error hide" style="width: 100%">X请正确填写图片链接地址</span>
                    </div>
                </div>
            </div>
            <div style="margin: 0px auto;width: 550px;padding-top: 15px">
                <img id="preview-banner-image" style="width: 548px;height: 140px;border: 1px solid #EEEEEE">
            </div>
        </div>
    </div>
    <div style="text-align:center;padding:5px">
        <input id="saveEditAdBanner" class="operation_btn_bg con_btn_2" value="保存"/>
        <input id="cancelEditAdBanner" class="operation_btn_bg can_btn_2" value="取消"/>
    </div>
</div>

<script type="text/javascript" src="../../js/jquery.min.js"></script>
<script type="text/javascript" src="../../js/jeasyui/jquery.easyui.min.js"></script>
<script type="text/javascript" src="../../js/jeasyui/validateExtend.js"></script>
<script type="text/javascript" src="../../js/jeasyui/locale/easyui-lang-zh_CN.js"></script>
<script type="text/javascript" src="../../js/jeasyui/validateExtend.js"></script>
<script type="text/javascript" src="../../js/myCalendar/WdatePicker.js"></script>
<script type="text/javascript" src="../../js/util/generalUtil.js"></script>
<script type="text/javascript" src="../../js/util/easyuiUtil.js"></script>
<script type="text/javascript" src="../../js/json2.js"></script>
<script type="text/javascript" src="../../js/popup-v3.js"></script>
<script type="text/javascript" src="../../js/plugin/uploader/jquery.uploader.js"></script>
<script type="text/javascript" src="../../js/relative-settings/kefu-home-page-config.js"></script>
</body>
</html>
