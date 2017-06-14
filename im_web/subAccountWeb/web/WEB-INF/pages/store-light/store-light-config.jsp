<%@ page import="com.feiniu.subAccount.constants.WeekConstants" %>
<%@ page import="com.feiniu.subAccount.domain.StoreLightConfig" %>
<%@ page import="java.util.Map" %>
<%@ page import="com.feiniu.subAccount.util.JsonUtil" %>
<%@ page import="com.feiniu.subAccount.domain.ServiceGroup" %>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%--
  Created by IntelliJ IDEA.
  User: wangchen
  Date: 2015/2/7
  Time: 15:01
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%
    StoreLightConfig currentStoreStoreLightConfig = (StoreLightConfig) request.getAttribute("serviceGroupConfig");
    System.out.println(JsonUtil.toJsonString(currentStoreStoreLightConfig));
%>
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title>店铺亮灯</title>
    <jsp:include page="../common/meta-info.jsp"></jsp:include>
    <link type="text/css" rel="stylesheet" href="../css/reset.css"/>
    <link type="text/css" rel="stylesheet" href="../css/common.css"/>
    <link type="text/css" rel="stylesheet" href="../css/zzh_style.css"/>
</head>
<body>
<tmpdata
        data-merchantid="${merchantID}"
        data-servicegrouplist='<%=JsonUtil.toJsonString(request.getAttribute("serviceGroupList"))%>'>
</tmpdata>
<div class="main-content-area">
    <!--面包屑-->
    <div class="ui-crumbs">
        <p><b>当前位置：</b><a href="javascript:;" class="ui-clickable">商家中心</a><span> &gt; 店铺亮灯</span></p>
    </div>
    <!--面包屑-->
    <div class="ui-tab">
        <ul class="tab-nav">
            <li class="tab-nav-item"><a class="tabsDivSelect" href="../diverse/diverse-config.htm">分流设置</a></li>
            <li class="tab-nav-item"><a href="../service-group/service-group-config.htm">客服分组</a></li>
            <li class="tab-nav-item cur"><a href="#">店铺亮灯</a></li>
        </ul>
        <ul class="tab-panel pt-20">
            <li class="tab-panel-item cur">
                <div class="ppglBorderDiv">
                    <h2 class="hackIe6"><font class="h2-title">店铺亮灯设置</font><a href="store-light-diagnose.htm"><font
                            class="h2-notice">亮灯诊断</font></a>
                    </h2>
                </div>
                <div class="zzhBasic">
                    <div class="basic-content">
                        <div class="red-b-title">
                            <h2>亮灯分组名称</h2>
                            <font class="r-title">显示</font></div>
                        <div id="serviceGroupList" class="red-b-box">
                            <c:forEach items="${serviceGroupList}" var="serviceGroup">
                                <%
                                    ServiceGroup currentServiceGroup = (ServiceGroup) (pageContext.getAttribute("serviceGroup"));
                                %>
                                <div id="group${serviceGroup.groupID}" class="kf service-group"
                                     data-groupid="${serviceGroup.groupID}">
                                    <font>${serviceGroup.groupName}</font><em class="down"></em><em class="up"></em>
                                    <input type="checkbox"
                                           class="zzhCommonCheck" <%=currentServiceGroup.getIsShowOnStore() == '1' ? "checked" : ""%> />
                                </div>
                            </c:forEach>
                        </div>
                        <div class="red-b-title">
                            <h2>工作时间</h2>
                            <font class="r-title">显示</font></div>
                        <div class="red-b-box">
                            <div class="kf"><font class="date">日期</font><font class="time">时间</font></div>
                            <div id="firstTimePanel" class="cz">
                                <div class="date-box">
                                    <dl class="commonSelect start-date" style="position:relative; margin-right:0px;">
                                        <dt class="w56 dtBGpostion64" data-value="<%=currentStoreStoreLightConfig == null ?
                                        '1': currentStoreStoreLightConfig.getFirstWorkDayStart()%>">
                                            <%=
                                            currentStoreStoreLightConfig == null ?
                                                    WeekConstants.getRepresentDay('1') :
                                                    WeekConstants.getRepresentDay(currentStoreStoreLightConfig.getFirstWorkDayStart())
                                            %>
                                        </dt>
                                        <dd>
                                            <ul class="w70">
                                                <c:forEach items="<%=WeekConstants.CHARACTER_LIST%>" var="entry">
                                                    <%
                                                        String content = WeekConstants.getRepresentDay((Character) pageContext.getAttribute("entry"));
                                                    %>
                                                    <li><a href="#" data-value="${entry}"><%=content%>
                                                    </a></li>
                                                </c:forEach>
                                            </ul>
                                        </dd>
                                    </dl>
                                    <font class="font-z">至</font>
                                    <dl class="commonSelect end-date" style="position:relative;">
                                        <dt class="w56 dtBGpostion64" data-value="<%=currentStoreStoreLightConfig == null ?
                                        '1': currentStoreStoreLightConfig.getFirstWorkDayEnd()%>">
                                            <%=
                                            currentStoreStoreLightConfig == null ?
                                                    WeekConstants.getRepresentDay('1') :
                                                    WeekConstants.getRepresentDay(currentStoreStoreLightConfig.getFirstWorkDayEnd())
                                            %>
                                        </dt>
                                        <dd>
                                            <ul class="w70">
                                                <c:forEach items="<%=WeekConstants.CHARACTER_LIST%>" var="entry">
                                                    <%
                                                        String content = WeekConstants.getRepresentDay((Character) pageContext.getAttribute("entry"));
                                                    %>
                                                    <li><a href="#" data-value="${entry}"><%=content%>
                                                    </a></li>
                                                </c:forEach>
                                            </ul>
                                        </dd>
                                    </dl>
                                </div>
                                <div class="time-box">
                                    <input type="text" class="start-time text-100 icon_calendar"
                                           onclick="WdatePicker({dateFmt:'HH:mm',qsEnabled:true,quickSel:['07:30','08:00','08:30','09:00','09:30']})"
                                           value="<%=currentStoreStoreLightConfig == null ? "00:00" : currentStoreStoreLightConfig.getFirstWorkTimeStart()%>"/>
                                    <font class="font-z">至</font>
                                    <input type="text" class="end-time text-100 icon_calendar"
                                           onclick="WdatePicker({dateFmt:'HH:mm',qsEnabled:true,quickSel:['20:30','21:00','21:30','22:00','22:30']})"
                                           value="<%=currentStoreStoreLightConfig == null ? "00:00" : currentStoreStoreLightConfig.getFirstWorkTimeEnd()%>"/>
                                </div>
                                <input type="checkbox" class="zzhCommonCheck"
                                        <%=currentStoreStoreLightConfig == null || currentStoreStoreLightConfig.getIsFirstWorkTimeShow() != '1'
                                                ? "" : "checked"%>
                                        />
                            </div>
                            <div id="secondTimePanel" class="cz">
                                <div class="date-box">
                                    <dl class="commonSelect start-date" style="position:relative; margin-right:0px;">
                                        <dt class="w56 dtBGpostion64" data-value="<%=currentStoreStoreLightConfig == null ?
                                        '1': currentStoreStoreLightConfig.getSecondWorkDayStart()%>">
                                            <%=
                                            currentStoreStoreLightConfig == null ?
                                                    WeekConstants.getRepresentDay('1') :
                                                    WeekConstants.getRepresentDay(currentStoreStoreLightConfig.getSecondWorkDayStart())
                                            %>
                                        </dt>
                                        <dd>
                                            <ul class="w70">
                                                <c:forEach items="<%=WeekConstants.CHARACTER_LIST%>" var="entry">
                                                    <%
                                                        String content = WeekConstants.getRepresentDay((Character) pageContext.getAttribute("entry"));
                                                    %>
                                                    <li><a href="#" data-value="${entry}"><%=content%>
                                                    </a></li>
                                                </c:forEach>
                                            </ul>
                                        </dd>
                                    </dl>
                                    <font class="font-z">至</font>
                                    <dl class="commonSelect end-date" style="position:relative;">
                                        <dt class="w56 dtBGpostion64" data-value="<%=currentStoreStoreLightConfig == null ?
                                        '1': currentStoreStoreLightConfig.getSecondWorkDayEnd()%>">
                                            <%=
                                            currentStoreStoreLightConfig == null ?
                                                    WeekConstants.getRepresentDay('1') :
                                                    WeekConstants.getRepresentDay(currentStoreStoreLightConfig.getSecondWorkDayEnd())
                                            %>
                                        </dt>
                                        <dd>
                                            <ul class="w70">
                                                <c:forEach items="<%=WeekConstants.CHARACTER_LIST%>" var="entry">
                                                    <%
                                                        String content = WeekConstants.getRepresentDay((Character) pageContext.getAttribute("entry"));
                                                    %>
                                                    <li><a href="#" data-value="${entry}"><%=content%>
                                                    </a></li>
                                                </c:forEach>
                                            </ul>
                                        </dd>
                                    </dl>
                                </div>
                                <div class="time-box">
                                    <input type="text" class="text-100 start-time icon_calendar"
                                           onclick="WdatePicker({dateFmt:'HH:mm',qsEnabled:true,quickSel:['07:30','08:00','08:30','09:00','09:30']})"
                                           value="<%=currentStoreStoreLightConfig == null ? "00:00" : currentStoreStoreLightConfig.getSecondWorkTimeStart()%>"/>
                                    <font class="font-z">至</font>
                                    <input type="text" class="text-100 end-time icon_calendar"
                                           onclick="WdatePicker({dateFmt:'HH:mm',qsEnabled:true,quickSel:['20:30','21:00','21:30','22:00','22:30']})"
                                           value="<%=currentStoreStoreLightConfig == null ? "00:00" : currentStoreStoreLightConfig.getSecondWorkTimeEnd()%>"/>
                                </div>
                                <input type="checkbox" class="zzhCommonCheck"
                                        <%=currentStoreStoreLightConfig == null || currentStoreStoreLightConfig.getIsSecondWorkTimeShow() != '1'
                                                ? "" : "checked"%>
                                        />
                            </div>
                        </div>
                        <div class="red-b-title">
                            <h2>联系方式</h2>
                            <font class="r-title">显示</font></div>
                        <div class="red-b-box">
                            <div class="dh"><font>联系电话：</font>
                                <input id="phone" maxlength='30' type="text" class="text-190"
                                       value='<%=currentStoreStoreLightConfig == null ? "" : currentStoreStoreLightConfig.getPhone()%>'/>
                                <font style="color: #999999;font-size: 12px;margin-left: 260px;">
                                    区号-直拨号-分机号</font>
                                <input id="isPhoneChecked" type="checkbox"
                                       class="zzhCommonCheck" <%=currentStoreStoreLightConfig == null || currentStoreStoreLightConfig.getIsPhoneShow() != '1' ? "" : "checked"%>/>
                            </div>

                            <div class="dh"><font>联系手机：</font>
                                <input id="telphone" maxlength='11' type="text" class="text-190"
                                       value='<%=currentStoreStoreLightConfig == null ? "" : currentStoreStoreLightConfig.getTelphone()%>'/>
                                <font style="color: #999999;font-size: 12px;margin-left: 260px;">
                                    11位数</font>
                                <input id="isTelphoneChecked" type="checkbox"
                                       class="zzhCommonCheck" <%=currentStoreStoreLightConfig == null || currentStoreStoreLightConfig.getIsTelphoneShow() != '1' ? "" : "checked"%>/>
                            </div>
                            <div class="dh">
                                <button id="saveStoreLightConfig"
                                        class="ui-button red"
                                        type="sureBtn">确定</button>
                                <button id="saveStoreLightConfig"
                                        class="ui-button"
                                        type="sureBtn">取 消</button>
                            </div>
                        </div>
                    </div>
                </div>
            </li>
        </ul>
    </div>
</div>
</body>
<script type="text/javascript" language="javascript" src="../js/jquery-1.11.1.min.js"></script>
<script src="../js/myCalendar/WdatePicker.js" type="text/javascript"></script>
<script type="text/javascript" language="javascript" src="../js/zzh.js"></script>
<script type="text/javascript" src="../js/util/json2.js"></script>
<script type="text/javascript" src="../js/store-light/store-light-config.js"></script>
</html>

