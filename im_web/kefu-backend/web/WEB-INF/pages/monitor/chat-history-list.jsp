<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title>监控查询</title>
    <link type="text/css" rel="stylesheet" href="../css/reset.css"/>
    <link type="text/css" rel="stylesheet" href="../css/employee.css"/>
</head>

<body>
<div class="p-contnet">
    <div class="cla_nav"><span class="on">查询条件</span></div>
    <div></div>
    <div class="tabsDiv">
        <ul>
            <li><a class="tabsDivSelect">聊天记录</a></li>
            <li><a href="service-appraise.htm">服务评价</a></li>
        </ul>
    </div>
    <div class="ppglBorderDiv">
        <h2><font class="h2-title">聊天记录查询</font></h2>

        <div class="zzhmessage">
            <div class="formbox">
                <ul>
                    <form action="" method="">
                        <fieldset>
                            <!--<legend>分流方式设置提交</legend>-->
                            <li class="short" id="subMerchantIDPanel">
                                <div class="formname">员工账号：</div>
                                <div class="formtext">
                                    <dl class="commonSelect" style="padding-right: 0px;">
                                        <dt class="w182 dtBGpostion176" style="padding-left: 8px;padding-right: 0px">所有账号</dt>
                                        <dd>
                                            <ul class="w182">
                                                <li data-submerchantid="">
                                                    <a href="#">所有账号</a>
                                                </li>
                                                <c:forEach items="${subMerchantInfoList}" var="subMerchantInfo">
                                                    <li data-submerchantid="${subMerchantInfo.memberId}">
                                                        <a href="#">${subMerchantInfo.username}</a>
                                                    </li>
                                                </c:forEach>
                                            </ul>
                                        </dd>
                                    </dl>
                                </div>
                            </li>
                            <li class="short">
                                <div class="formname">客户账号：</div>
                                <div class="formtext">
                                    <input type="text" id="customerName" class="dateSettingTxt fl mr10"/>

                                    <p class="colff8400 fl hide"><img src="../images/employee/attention.gif" alt=""
                                                                 class="attentionPic"/>员工账号和客户账号必须确定一个</p>
                                </div>
                            </li>
                            <li class="short">
                                <div class="formname">操作时间：</div>
                                <div class="formtext">
                                    <input type="text" id="startTime" class="dateSettingTxt icon_calendar fl mr10"/>
                                    <input type="text" id="endTime" class="dateSettingTxt icon_calendar"/>
                                </div>
                            </li>

                            <li class="short">
                                <div class="formname"></div>
                                <div class="formtext">
                                    <input id="search" type="button" class="buiness_btn_69 mt15" value="查询"/>
                                </div>
                            </li>
                        </fieldset>
                    </form>
                </ul>
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
                        <td id="currentSubMerchantInfoPanel">${storeInfo.storeName}：<span class="subMemberName"></span>
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
</div>
</body>
<script type="text/javascript" language="javascript" src="../js/jquery-1.11.1.min.js"></script>
<script type="text/javascript" language="javascript" src="../js/employee.js"></script>
<script type="text/javascript" src="../js/util/generalUtil.js"></script>
<script type="text/javascript" language="javascript" src="../js/monitor/chatHistoryListManager.js"></script>
<script type="text/javascript" src="../js/myCalendar/WdatePicker.js"></script>
</html>
