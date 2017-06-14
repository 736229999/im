<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ page import="com.feiniu.subAccount.domain.StoreLightConfig" %>
<%@ page import="com.feiniu.subAccount.constants.WeekConstants" %>
<%@ page import="com.feiniu.subAccount.bean.response.StoreInfo" %>
<%@ page import="org.apache.commons.collections4.CollectionUtils" %>
<%@ page import="java.util.List" %>
<%--
  Created by IntelliJ IDEA.
  User: wangchen
  Date: 2015/2/7
  Time: 15:02
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title>店铺亮灯诊断</title>
    <jsp:include page="../common/meta-info.jsp"></jsp:include>
    <link type="text/css" rel="stylesheet" href="../css/reset.css"/>
    <link type="text/css" rel="stylesheet" href="../css/common.css"/>
    <link type="text/css" rel="stylesheet" href="../css/zzh_style.css"/>
</head>

<body>
<tmpdata data-merchantid="${merchantID}"></tmpdata>
<div class="main-content-area">
    <!--面包屑-->
    <div class="ui-crumbs">
        <p><b>当前位置：</b><a href="javascript:;" class="ui-clickable">商家中心</a><span> &gt; 店铺亮灯诊断</span></p>
    </div>
    <!--面包屑-->
    <div class="ui-tab">
        <ul class="tab-nav">
            <li class="tab-nav-item"><a class="tabsDivSelect" href="../diverse/diverse-config.htm">分流设置</a></li>
            <li class="tab-nav-item"><a href="../service-group/service-group-config.htm">客服分组</a></li>
            <li class="tab-nav-item cur"><a href="../store-light/store-light-config.htm">店铺亮灯</a></li>
        </ul>
        <ul class="tab-panel pt-20">
            <li class="tab-panel-item cur">
                <div class="ppglBorderDiv">
                    <h2 class="hackIe6"><font class="h2-title">店铺亮灯诊断</font></h2>
                </div>
                <div class="zzhBasic">
                    <div id="diagnoseResultPanel" class="lightSetting">
                        <div class="lightSettingTitle"><font class="lightH2">亮灯诊断</font><font id="diagnoseBtn"
                                                                                              class="lightSettingRightBtn">再次诊断</font>
                        </div>
                        <p class="warnBox wrong-panel"><em class="wrong"></em><font>售前客服分组灯不亮。原因：组内未添加客服或客服未登录IM</font>
                        </p>

                        <p class="warnBox right-panel"><em class="right"></em><font>售前客服分组灯不亮。原因：组内未添加客服或客服未登录IM</font>
                        </p>
                    </div>
                    <div class="lightSetting">
                        <%
                            StoreInfo storeInfo = (StoreInfo) request.getAttribute("storeInfo");
                            String storeURL = null;

                            if (storeInfo == null) {
                                if (request.getRequestURL().indexOf("feiniu.com") != -1) {
                                    storeURL = "http://store.feiniu.com/index-" + request.getAttribute("merchantID") + ".html";
                                } else {
                                    storeURL = "http://storefront.beta1.fn/index-" + request.getAttribute("merchantID") + ".html";
                                }
                            } else {
                                storeURL = storeInfo.getUrl();
                            }
                        %>
                        <div class="lightSettingTitle"><font class="lightH2">亮灯诊断</font><font
                                class="lightSettingRightBtn"><a
                                href="<%=storeURL%>" target="_blank">进入店铺</a></font><font
                                class="lightSettingRightBtn"><a
                                href="store-light-config.htm">修改亮灯</a></font></div>
                        <div class="mbs-box"><h2 class="lxwmTitle">
                            <span><%=storeInfo == null ? "" : storeInfo.getStoreName()%></span><img
                                src="../images/zzh/contact.gif" alt="联系我们" class="contentUS"/></h2>
                            <%
                                StoreLightConfig storeLightConfig = (StoreLightConfig) request.getAttribute("storeLightConfig");
                                String defaultDay = WeekConstants.getRepresentDay('1');
                                String defaulTime = "00:00";
                                boolean isStoreLightConfigNull = storeLightConfig == null;

                                String firstDayStart = isStoreLightConfigNull ?
                                        defaultDay : WeekConstants.getRepresentDay(storeLightConfig.getFirstWorkDayStart());
                                String firstDayEnd = isStoreLightConfigNull ?
                                        defaultDay : WeekConstants.getRepresentDay(storeLightConfig.getFirstWorkDayEnd());
                                String secondDayStart = isStoreLightConfigNull ? defaultDay :
                                        WeekConstants.getRepresentDay(storeLightConfig.getSecondWorkDayStart());
                                String secodnDayEnd = isStoreLightConfigNull ? defaultDay :
                                        WeekConstants.getRepresentDay(storeLightConfig.getSecondWorkDayEnd());
                                String firstTimeStart = isStoreLightConfigNull ? defaulTime :
                                        storeLightConfig.getFirstWorkTimeStart();
                                String firstTimeEnd = isStoreLightConfigNull ? defaulTime :
                                        storeLightConfig.getFirstWorkTimeEnd();
                                String secondTimeStart = isStoreLightConfigNull ? defaulTime :
                                        storeLightConfig.getSecondWorkTimeStart();
                                String secondTimeEnd = isStoreLightConfigNull ? defaulTime :
                                        storeLightConfig.getSecondWorkTimeEnd();
                                boolean isTelphoneShow = !isStoreLightConfigNull && storeLightConfig.getIsTelphoneShow() == '1';
                                boolean isPhoneShow = !isStoreLightConfigNull && storeLightConfig.getIsPhoneShow() == '1';

                                String telphone = isStoreLightConfigNull ? "" : storeLightConfig.getTelphone();
                                String phone = isStoreLightConfigNull ? "" : storeLightConfig.getPhone();
                            %>
                            <c:if test='<%=CollectionUtils.isNotEmpty((List)request.getAttribute("serviceGroupListShowOnStore"))%>'>
                                <ul class="chat-panel">
                                    <c:forEach items="${serviceGroupListShowOnStore}" var="serviceGroup">
                                        <li class="chat-icon">
                                            <span>${serviceGroup.groupName}：</span>
                                            <span>给我留言</span>
                                        </li>
                                    </c:forEach>
                                </ul>
                            </c:if>
                            <h2 class="mbs-box-title">工作时间</h2>

                            <p><%=firstDayStart%>至<%=firstDayEnd%>   <%=firstTimeStart%>-<%=firstTimeEnd%>
                            </p>

                            <p><%=secondDayStart%>至<%=secodnDayEnd%>  <%=secondTimeStart%>-<%=secondTimeEnd%>
                            </p>

                            <c:if test="<%=isPhoneShow && isTelphoneShow%>">
                                <h2 class="mbs-box-title">联系方式</h2>
                                <c:if test="<%=isPhoneShow%>">
                                    <p>联系电话：<%=phone%>
                                    </p>
                                </c:if>
                                <c:if test="<%=isTelphoneShow%>">
                                    <p>联系手机：<%=telphone%>
                                    </p>
                                </c:if>
                            </c:if>

                        </div>
                    </div>
                </div>
            </li>
        </ul>
    </div>

</div>
<script type="text/javascript" src="../js/jquery-1.11.1.min.js"></script>
<script type="text/javascript" src="../js/store-light/store-light-diagnose.js"></script>
</body>
</html>

