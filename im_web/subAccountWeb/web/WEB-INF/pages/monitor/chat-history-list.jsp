<%@ page import="com.feiniu.subAccount.bean.response.SubMerchantInfo" %>
<%@ page import="org.apache.commons.lang.StringUtils" %>
<%@ page import="java.util.List" %>
<%@ page import="org.apache.commons.collections4.CollectionUtils" %>
<%@ page import="java.util.ArrayList" %>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
<%
    List<SubMerchantInfo> subMerchantInfos = (List<SubMerchantInfo>) (request.getAttribute("subMerchantInfoList"));
    List<Long> submerchantIDList = new ArrayList<Long>(subMerchantInfos.size());

    for (SubMerchantInfo subMerchantInfo : subMerchantInfos) {
        submerchantIDList.add(subMerchantInfo.getMemberId());
    }
%>

<head>
    <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title>监控查询</title>
    <jsp:include page="../common/meta-info.jsp"></jsp:include>
    <link type="text/css" rel="stylesheet" href="../css/employee.css"/>
</head>

<body>
<div class="main-content-area">
    <!--面包屑-->
    <div class="ui-crumbs">
        <p><b>当前位置：</b><a href="javascript:;" class="ui-clickable">商家中心</a><span> &gt; 客服监管</span></p>
    </div>
    <!--面包屑-->
    <div class="ui-tab">
        <ul class="tab-nav">
            <li class="tab-nav-item cur"><a class="tabsDivSelect">聊天记录</a></li>
            <li class="tab-nav-item"><a href="service-appraise.htm">服务评价</a></li>
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
                                    <td>
                                        <dl class="ui-dl">
                                            <dt class="ui-dt">员工账号：</dt>
                                            <dd class="ui-dd">
                                                <select id="subMerchantID" class="ui-select">
                                                    <option value="<%=StringUtils.join(submerchantIDList,";")%>">
                                                        全部账号
                                                    </option>
                                                    <c:forEach items="${subMerchantInfoList}" var="subMerchantInfo">
                                                        <option value="${subMerchantInfo.memberId}">${subMerchantInfo.username}</option>
                                                    </c:forEach>
                                                </select>
                                            </dd>
                                        </dl>
                                    </td>
                                    <td>
                                        <dl class="ui-dl">
                                            <dt class="ui-dt">
                                                客户账号：
                                            </dt>
                                            <dd class="ui-dd">
                                                <input type="text" class="ui-input" id="customerName">
                                            <span>
                                                <img src="../images/employee/attention.gif"
                                                     alt=""
                                                     class="attentionPic"/>员工账号和客户账号必须确定一个
                                            </span>
                                            </dd>
                                        </dl>
                                    </td>
                                    <td colspan="2">
                                        <dl class="ui-dl">
                                            <dt class="ui-dt">
                                                操作时间：
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
                            <button id="search" class="ui-button red" type="button">查询</button>
                        </div>
                    </div>
                    <div class="zzhmessage" style="padding-top: 0px">
                        <div class="formbox">
                            <table class="appraiseTable" border="1">
                                <thead>
                                <tr>
                                    <th width="20%">员工账号</th>
                                    <th width="20%">客户账号</th>
                                    <th width="60%">聊天记录</th>
                                </tr>
                                </thead>
                                <tbody>
                                <tr>
                                    <td id="currentSubMerchantInfoPanel">${storeInfo.storeName}：<span
                                            class="subMemberName"></span>
                                    </td>
                                    <td id="customerMerchantInfo"></td>
                                    <td id="historyMessageListPanel">
                                </tr>
                                </tbody>
                            </table>
                            <div class="employeepage"><a href="" class="pageLeftBg">首页</a><a href="" class="pageLeftBg">上一页</a><a
                                    href="" target="_self" class="active">1</a><a href="" target="_self">2</a><a href=""
                                                                                                                 target="_self">3</a><a
                                    href="" target="_self">4</a><a href="" target="_self">5</a><a href="" target="_self"
                                                                                                  class="pageRightBg">下一页</a><a
                                    href="" target="_self" class="pageRightBg">尾页</a></div>

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
<script type="text/javascript" src="../js/util/generalUtil.js"></script>
<script type="text/javascript" src="../js/util/feiniu-image.js"></script>
<script type="text/javascript" src="../js/myCalendar/WdatePicker.js"></script>
<script type="text/javascript" language="javascript" src="../js/monitor/chatHistoryListManager.js"></script>
</html>
