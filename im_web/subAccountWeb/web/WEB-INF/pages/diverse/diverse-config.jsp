<%@ page import="com.feiniu.subAccount.domain.MerchantDiverseConfig" %>
<%@ page import="java.util.List" %>
<%@ page import="com.feiniu.subAccount.bean.response.SubMerchantInfo" %>
<%@ page import="com.feiniu.subAccount.util.JsonUtil" %>
<%@ page import="com.feiniu.subAccount.domain.DiverseType" %>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%
    MerchantDiverseConfig currentMerchantDiverseConfig = (MerchantDiverseConfig) request.getAttribute("currentMerchantDiverseConfig");
    boolean isConfiged = currentMerchantDiverseConfig != null;
    String subAccountName = "";
    List<SubMerchantInfo> subMerchantInfoList = (List<SubMerchantInfo>) request.getAttribute("subMerchantInfoList");
    if (isConfiged) {
        for (SubMerchantInfo subMerchantInfo : subMerchantInfoList) {
            if (subMerchantInfo.getMemberId() == currentMerchantDiverseConfig.getProxySubAccountID()) {
                subAccountName = subMerchantInfo.getTruename();
                break;
            }
        }
    }
%>
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title>分流设置</title>
    <jsp:include page="../common/meta-info.jsp"></jsp:include>
    <link type="text/css" rel="stylesheet" href="../css/reset.css"/>
    <link type="text/css" rel="stylesheet" href="../css/common.css"/>
    <link type="text/css" rel="stylesheet" href="../css/zzh_style.css"/>
    <link type="text/css" rel="stylesheet" href="../css/popu.css"/>

</head>
<body>
<div class="main-content-area">
    <!--面包屑-->
    <div class="ui-crumbs">
        <p><b>当前位置：</b><a href="javascript:;" class="ui-clickable">商家中心</a><span> &gt; 分流设置</span></p>
    </div>
    <!--面包屑-->
    <div class="ui-tab">
        <ul class="tab-nav">
            <li class="tab-nav-item cur"><a class="tabsDivSelect" href="#none" target="_blank">分流设置</a></li>
            <li class="tab-nav-item"><a href="../service-group/service-group-config.htm">客服分组</a></li>
            <li class="tab-nav-item"><a href="../store-light/store-light-config.htm">店铺亮灯</a></li>
        </ul>
        <ul class="tab-panel pt-20">
            <li class="tab-panel-item cur">

                <div class="ppglBorderDiv">
                    <h2 class="hackIe6"><font class="h2-title">分流方式设置</font><font
                            class="h2-notice">（注：带<b>*</b>号为必填项）</font></h2>

                    <div class="zzhBasic">
                        <div id="diverseTypeConfigPanel" class="flow-setting-box">
                            <c:forEach items="${diverseTypeList}" var="diverseType" varStatus="iterateStatus">
                <span>
                    <font class="fsb-title">
                        <c:if test="${iterateStatus.index == 0}">
                            <i>*</i>分流方式：
                        </c:if>
                    </font>
                    <%
                        DiverseType diverseType = (DiverseType) pageContext.getAttribute("diverseType");

                        boolean isChecked = isConfiged;
                        boolean isRecommended = diverseType.getIsRecommend() == '1';

                        if (isChecked) {
                            isChecked = diverseType.getDiverseTypeID() == currentMerchantDiverseConfig.getDiverseType().getDiverseTypeID();
                        } else {
                            isChecked = isRecommended;
                        }
                    %>
                    <input type="radio" class="zzhradio" value="${diverseType.diverseTypeID}"
                           name="flowSetting" <%=isChecked ? "checked" : ""%>/>${diverseType.diverseTypeName}<%=isRecommended ? "(推荐)" : ""%>
                    <font>
                            ${diverseType.diverseDescription}
                    </font>
                </span>
                            </c:forEach>
                            <div class="line"></div>
        <span class="dlzh"><font class="fsb-title"><i>*</i>代理账号：</font>
         <dl id="subMerchantList" class="commonSelect" data-merchantid="${merchantID}">
             <c:choose>
                 <c:when test="<%=isConfiged%>">
                     <dt data-submerchantid="${currentMerchantDiverseConfig.proxySubAccountID}"
                         class="w168 dtBGpostion176">
                             ${storeInfo.storeName}：<%=subAccountName%>
                     </dt>
                 </c:when>
                 <c:otherwise>
                     <dt class="w168 dtBGpostion176">请选择代理账号</dt>
                 </c:otherwise>
             </c:choose>
             <dd>
                 <ul class="w182">
                     <c:forEach items="${subMerchantInfoList}" var="subMerchant">
                         <c:if test="${subMerchant.memberType == 2}">
                             <li data-submerchantid="${subMerchant.memberId}">
                                 <a href="#">${storeInfo.storeName}：${subMerchant.username}</a>
                             </li>
                         </c:if>
                     </c:forEach>
                 </ul>
             </dd>
         </dl>
       </span>
                            <span><font class="fsb-title"></font><font class="no-mr20">当所有参与分流的客服都不在线或挂起时，客户发送的消息，将由他统一接收，该账号必须为正常状态账号，且登录过哞哞</font></span>
                            <span class="dlzh"><font class="fsb-title"></font> <button id="saveConfigBtn"
                                                                                       class="ui-button red"
                                                                                       type="sureBtn">保存
                            </button></span>
                        </div>
                    </div>
                </div>
            </li>
        </ul>
    </div>
    <!--弹窗-->
    <div class="popup_wrap ${hasGuided == true ? 'hide' : ''}">
        <div class="popup_cont">
        </div>
    </div>
    <div id="placeHold" style="display:none;">
        <div id="p_1"><img src="../images/popu_pic_1.jpg">

            <div class="btn_1"><a class="xia" id="c_1"></a></div>
        </div>
        <div id="p_2"><img src="../images/popu_pic.jpg">

            <div class="btn"><a class="sh" id="c_1_1"></a> <a class="xia" id="c_2"></a></div>
        </div>
        <div id="p_3"><img src="../images/popu_pic_2.jpg">

            <div class="btn"><a class="sh" id="c_2_2"></a> <a class="xia" id="c_3"></a></div>
        </div>
        <div id="p_4"><img src="../images/popu_pic_3.jpg">

            <div class="btn"><a class="sh" id="c_3_3"></a> <a class="xia" id="c_4"></a></div>
        </div>
        <div id="p_5"><img src="../images/popu_pic_4.jpg">

            <div class="btn"><a class="sh" id="c_4_4"></a> <a class="xia" id="c_5"></a></div>
        </div>
        <div id="p_6"><img src="../images/popu_pic_5.jpg">

            <div class="btn"><a class="sh" id="c_5_5"></a> <a class="know"></a></div>
        </div>
    </div>
</div>
</body>
<script type="text/javascript" language="javascript" src="../js/jquery-1.11.1.min.js"></script>
<script type="text/javascript" language="javascript" src="../js/zzh.js"></script>
<script type="text/javascript" src="../js/diverseConfig/diverse-config.js"></script>
<c:if test="${! hasGuided}">
    <script type="text/javascript" src="../js/popup.js"></script>
    <script type="text/javascript" src="../js/flsz.js"></script>
</c:if>
</html>
