<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ page import="com.feiniu.subAccount.bean.response.ServiceAppraiseStat" %>
<%@ page import="com.feiniu.subAccount.bean.response.SubMerchantInfo" %>
<%@ page import="com.feiniu.subAccount.util.JsonUtil" %>
<%@ page import="org.apache.commons.collections4.CollectionUtils" %>
<%@ page import="java.util.ArrayList" %>
<%@ page import="java.util.List" %>
<%@ page import="org.apache.commons.lang.StringUtils" %>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%
    List<SubMerchantInfo> subMerchantInfoList = (List<SubMerchantInfo>) request.getAttribute("subMerchantInfoList");
    List<ServiceAppraiseStat> serviceAppraiseList = (List<ServiceAppraiseStat>) request.getAttribute("serviceAppraiseList");

    boolean hasSubMerchant = CollectionUtils.isNotEmpty(subMerchantInfoList);
    List<Long> submerchantIDList = new ArrayList<Long>(subMerchantInfoList.size());

    for (SubMerchantInfo subMerchantInfo : subMerchantInfoList) {
        submerchantIDList.add(subMerchantInfo.getMemberId());
    }
%>
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
    <meta content="text/html; charset=utf-8" http-equiv="Content-Type">
    <title>监控查询</title>
    <jsp:include page="../common/meta-info.jsp"></jsp:include>
    <link type="text/css" rel="stylesheet" href="../css/reset.css"/>
    <link type="text/css" rel="stylesheet" href="../css/employee.css"/>
</head>

<body>
<tmpdata data-serviceappraiselist='<%=JsonUtil.toJsonString(serviceAppraiseList)%>'>
</tmpdata>
<div class="main-content-area">
    <!--面包屑-->
    <div class="ui-crumbs">
        <p><b>当前位置：</b><a href="javascript:;" class="ui-clickable">商家中心</a><span> &gt; 客服监管</span></p>
    </div>
    <!--面包屑-->
    <div class="ui-tab">
        <ul class="tab-nav">
            <li class="tab-nav-item"><a href="chat-history-list.htm">聊天记录</a></li>
            <li class="tab-nav-item cur"><a class="tabsDivSelect">服务评价</a></li>
        </ul>
        <ul class="tab-panel pt-20">
            <li class="tab-panel-item cur">
                <div class="ppglBorderDiv">
                    <h2><font class="h2-title">聊天记录查询</font></h2>

                    <div class="input-area">
                        <div class="bd">
                            <table width="100%">
                                <tbody>
                                <tr>
                                    <td colspan="1">
                                        <dl class="ui-dl">
                                            <dt class="ui-dt">员工账号：</dt>
                                            <dd class="ui-dd">
                                                <select id="subMerchantID" class="ui-select">
                                                    <option value="<%=StringUtils.join(submerchantIDList ,";")%>">
                                                        全部账号
                                                    </option>
                                                    <c:forEach items="${subMerchantInfoList}" var="subMerchantInfo">
                                                        <option value="${subMerchantInfo.memberId}">${subMerchantInfo.username}</option>
                                                    </c:forEach>
                                                </select>
                                            </dd>
                                        </dl>
                                    </td>
                                    <td colspan="3">
                                        <dl class="ui-dl">
                                            <dt class="ui-dt">
                                                时间：
                                            </dt>
                                            <dd class="ui-dd">
                                                <input type="text" id="startTime" class="ui-date-input Wdate"/>
                                                <input type="text" id="endTime" class="ui-date-input Wdate"/>
                                            </dd>
                                        </dl>
                                    </td>
                                </tr>
                                </tbody>
                            </table>
                        </div>
                        <div class="ft">
                            <button id="queryAppraiseInfo" class="ui-button red" type="button">查询</button>
                        </div>
                    </div>

                    <div class="zzhmessage">
                        <div class="formbox">
                            <table class="serverTable" border="1" width="100%">
                                <thead>
                                <tr>
                                    <th width="28%">帐号名</th>
                                    <th width="12%">未评估</th>
                                    <th width="12%">非常满意</th>
                                    <th width="12%">满意</th>
                                    <th width="12%">一般</th>
                                    <th width="12%">不满意</th>
                                    <th width="12%">非常不满意</th>
                                </tr>
                                </thead>
                                <tbody id="serviceAppraiseInfoPanel">
                                <c:if test="<%=hasSubMerchant%>">
                                    <c:forEach items="<%=subMerchantInfoList%>" var="subMerchantInfo">
                                        <tr id="appraiseInfoForSubMerchant${subMerchantInfo.memberId}">
                                            <td><a>${storeInfo.storeName}--${subMerchantInfo.username}</a></td>
                                            <td data-appraiseid="0">0</td>
                                            <td data-appraiseid="5">0</td>
                                            <td data-appraiseid="4">0</td>
                                            <td data-appraiseid="3">0</td>
                                            <td data-appraiseid="2">0</td>
                                            <td data-appraiseid="1">0</td>
                                        </tr>
                                    </c:forEach>
                                </c:if>
                                </tbody>
                            </table>
                        </div>
                    </div>
                </div>
            </li>
        </ul>
    </div>
</div>
</body>
<script type="text/javascript" language="javascript" src="../js/jquery-1.11.1.min.js"></script>
<script type="text/javascript" language="javascript" src="../js/employee.js"></script>
<script type="text/javascript" src="../js/myCalendar/WdatePicker.js"></script>
<script type="text/javascript" src="../js/util/generalUtil.js"></script>
<script type="text/javascript" src="../js/monitor/service-appraise.js"></script>
</html>
