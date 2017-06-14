<%@ page import="com.feiniu.csWebIm.bean.domain.AfterSaleBoardConfig" %>
<%@ page import="com.feiniu.csWebIm.bean.domain.KefuIndexConfig" %>
<%@ page import="com.feiniu.csWebIm.consts.SystemConfig" %>
<%@ page import="com.feiniu.csWebIm.utils.json.JsonNodeUtil" %>
<%@ page import="com.feiniu.csWebIm.utils.json.JsonUtil" %>
<%@ page import="org.apache.commons.lang.StringUtils" %>
<%@ page import="org.codehaus.jackson.JsonNode" %>
<%@ page import="java.util.Collections" %>
<%@ page import="java.util.HashMap" %>
<%@ page import="java.util.List" %>
<%@ page import="java.util.Map" %>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions" %>
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
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <title>在线客服首页</title>
    <link href="http://static02.fn-mart.com/images/feiniu_favicon.ico" rel="shortcut icon">
    <link href="css/content.css" rel="stylesheet" type="text/css"/>
    <link href="css/base.css" rel="stylesheet" type="text/css"/>
    <link href="css/page.css" rel="stylesheet" type="text/css"/>
    <link href="css/ui_pop.css" rel="stylesheet" type="text/css"/>
    <link rel="stylesheet" type="text/css" href="js/jquery/fn-login/base.css"/>
    <link rel="stylesheet" type="text/css" href="js/jquery/fn-login/fn_login.css"/>
    <link rel="stylesheet" type="text/css" href="css/home-page-config.css"/>
</head>
<body>
<div class="publish-edit-section">
    <div class="publish-wrapper" style="width: 1190px;margin: 0px auto">
        <div class="w990" style="margin: 0px auto">
            <div class="on_line">
                <div class="logo"><img src="images/logo.jpg"/></div>
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
                    <a class="bule_btn${fn:contains(boardConfig.isBindUserOrder ,'1') ? ' btn-bind-order' : ''}"
                       href="#">立即咨询</a>

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
                <tr class="loading">
                    <td colspan="6" style="text-align: center">正在查询...</td>
                </tr>
                <tr class="no-result hide">
                    <td colspan="5">未查询到您最近一个月内的订单信息</td>
                    <td><a class="continue-contact bule_btn_1" href="#">继续咨询</a></td>
                </tr>
            </table>
        </div>
        <div class="num">
            <p>您还可以输入订单编号：</p>

            <div class="Digital">
                <div class="Start">
                    <input type="text" name="orderSeq"/>
                    <a class="red_btn_1 consult-by-input-orderseq" href="#">开始咨询</a>
                </div>
                <div class="Prompt_red" style="display: none">
                    <i></i>
                    <span>订单号输入错误，请确认后再输入</span>
                </div>
                <div class="Prompt_bule" style="display: none">
                    <i></i>
                    <span>订单号输入正确，请点击“开始咨询”</span>
                </div>
                <a js-hook="shutdown" class="cane" href="#">取消</a>
                <em>您也可以前往<a class="popu_bule" id="memberOrderPage" href="#" target="_blank">订单中心</a>咨询</em>
            </div>
        </div>
    </div>
</div>
<!--弹窗-->

<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/javascript" src="js/util/generalUtil.js"></script>
<script type="text/javascript" src="js/sea.js"></script>
<script type="text/javascript" src="js/popup-v3.js"></script>
<script type="text/javascript" src="js/jquery/fn-login/fnDialog.js" async="async"></script>
<script type="text/javascript" src="js/home-page-index.js"></script>
</body>
</html>

