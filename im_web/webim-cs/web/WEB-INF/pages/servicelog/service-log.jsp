<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ page import="com.feiniu.csWebIm.bean.request.ServiceLogRequset" %>
<%@ page import="com.feiniu.csWebIm.utils.json.JsonUtil" %>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%
    ServiceLogRequset serviceLogRequest = (ServiceLogRequset) request.getAttribute("serviceLogRequest");
%>
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title>服务小记</title>
    <link type="text/css" rel="stylesheet" href="../css/serviceLog.css"/>
</head>

<body style="height: 100%;">
<div id="D_right_ifm">
    <tmpdata data-serviceLogRequest='<%=JsonUtil.toJsonString(serviceLogRequest)%>'></tmpdata>
    <div id="J_info_tab" class="head">
        <p id="J_store" class="on">客户</p>
        <p id="J_product">服务小记</p>
    </div>
    <div id="J_store_info" class="with">

        <ul class="ui-info-list ui-line">
            <li>用户名：<span  id="customerName" >FN_1992365</span></li>
            <li>
                <div class="fl">昵称：<span class="ui-width-limit" id="customerNickName" title="#">--</span></div>
                <div class="fr">会员等级：<span id="customerLevel">普通会员</span></div>
            </li>

        </ul>
        <div id="consultative-prd" class="ui-line">
            <ul class="ui-info-list" id="currentOrderInfo" >
                <li>订单编号：<span  id= "orderId">20150802918pc121290</span>
                    <span class="fr" id= "orderStatus" style="display:none;">交易完成</span>
                </li>
            </ul>
            <p class="title">咨询商品：</p>
            <div id="picScroller" class="picScroll carousel" >
                <div class="wrap ui-pic-list" style="height: 75px;width: 338px;">
                    <p class="btn-area">
                        <a data-role="prev" href="javascript:;" class="js_prev ui-switchable-prev-btn" id="js_prev" data-role="prev"></a>
                        <a data-role="next" href="javascript:;" class="js_next ui-switchable-next-btn" id="js_nexty" data-role="next"></a>
                    </p>
                    <ul data-role="content" id="AskingPictures" >
                        
                        <li class="hrefPic"><a href="#"><img src="#" /></a></li>
                        <li class="hrefPic"><a href="#"><img src="#" /></a></li>
                        <li class="hrefPic"><a href="#"><img src="#" /></a></li>
                        <li class="hrefPic"><a href="#"><img src="#" /></a></li>
                        
                        <li class="hrefPic"><a href="#"><img src="#" /></a></li>
                        <li class="hrefPic"><a href="#"><img src="#" /></a></li>
                        <li class="hrefPic"><a href="#"><img src="#" /></a></li>
                        <li class="hrefPic"><a href="#"><img src="#" /></a></li>
                        
                        <li class="hrefPic"><a href="#"><img src="#" /></a></li>
                        <li class="hrefPic"><a href="#"><img src="#" /></a></li>
                        <li class="hrefPic"><a href="#"><img src="#" /></a></li>
                        <li class="hrefPic"><a href="#"><img src="#" /></a></li>

                    </ul>
                </div>

            </div>


        </div>
        
        <div id="scroll-holder"></div>
        <ul class="ui-info-list ui-line">
            <li>最近交易：<span  id="lastOrderTime">2015-07-10 19:20:33</span> <span class="fr" id= "lastOrderStatus" style="display: none">交易完成</span></li>
            <li>订单编号：<span  id="lastOrderId">20150802918pc121290</span><span class="fr" >合计：￥<em id= "lastTotalCost">123.00</em></span> </li>

        </ul>
        <ul class="ui-info-list">
            <li>累计成交<span  id= "allDealCount">25</span>笔,<span  id= "totalGoodsNum">1000</span>件宝贝</li>
            <li>客单价：￥<span  id= "meanPrice">80123.00 </span> </li>

        </ul>
    </div>

    <!-- 常见问题-->
    <div id="J_product_info" style="display:none;">
        <div class="fill">
            <div id="product_info" class="f_content">
                <div class="name">
                    <ul>
                        <li><span><b class="red">＊</b>联系人：</span><input id ="userName" onchange="onTextChange()" type="text"  value="李哞哞"/></li>
                        <li><span><b class="red"></b>联系电话：</span><input id ="telephone" onchange="onTextChange()" type="text" /></li>
                    </ul>
                </div>
                <div class="handle">
                    <ul>
                        <li><span><b class="red">＊</b>服务类型：</span>
                            <input name="serviceType" class="serviceType" class="serviceType" type="radio" value="服务查询" onchange="onTextChange()" checked/><p>服务查询</p>
                            <input name="serviceType" class="serviceType"  id = "sSearch" type="radio" value="客诉" onchange="onTextChange()" /><p>客诉</p></li>
                        <li><span><b class="red">＊</b>优先级：</span>
                            <input class="priority" name="priority" id = "common" type="radio" value="普通" onchange="onTextChange()" checked/><p>普通</p>
                            <input class="priority" name="priority" id = "urgent" type="radio" value="急件" onchange="onTextChange()" /><p>急件</p>
                        </li>
                        <li><span><b class="red">＊</b>客服处理状态：</span>
                            <select name="way" id="statuSelect" onchange="onSelectChange()" >
                                <option id = "processing" class="serviceStatus" value="处理中" selected>处理中</option>
                                <option id = "end" class="serviceStatus" value="已结案" >已结案</option>
                                <option id = "afterSales" class="serviceStatus" value="售后处理">售后处理</option>
                            </select>
                        </li>
                    </ul>
                </div>
                <div class="problem">
                    <h2><b class="red">＊</b>咨询问题选择：</h2>
                    <ul id="askingOpts">
                        <li class="askingOpt"><input  name="" type="checkbox" value="" /><span>商品质量问题</span></li>
                        <li class="askingOpt"><input  name="" type="checkbox" value="" /><span>商品质量问题</span></li>
                        <li class="askingOpt"><input  name="" type="checkbox" value="" /><span>退货</span></li>
                        <li class="askingOpt" ><input name="" type="checkbox" value="" /><span>商品质量问题</span></li>
                        <li class="askingOpt"><input  name="" type="checkbox" value="" /><span>修改订单</span></li>
                        <li class="askingOpt"><input  name="" type="checkbox" value="" /><span>商品质量问题</span></li>
                        <li class="askingOpt"><input  name="" type="checkbox" value="" /><span>订单生成取消</span></li>
                        <li class="askingOpt"><input  name="" type="checkbox" value="" /><span>商品质量问题</span></li>
                    </ul>
                </div>
                <div class="Commodity">
                    <h2><b class="red">＊</b>咨询商品信息：</h2>
                    <ul id="askingInfos" >
                        <li ><input id="allSelectAskingInfo"  name="" type="checkbox" value="" /><span>全部</span></li>
                        <li class="askingInfo"><input class="selectAskingInfo" name="askingInfo" type="checkbox" value="" id="1211"/><i></i><span>植物意思清华大学是什么会是呢？没有搞明白。</span><div class="info" style="display:none;"><span>植物意思清华大学是什么会是呢？没有搞明白。</span><span>植物意思清华大学是什么会是呢？没有搞明白。</span><span>植物意思清华大学是什么会是呢？没有搞明白。</span></div></li>
                        <li class="askingInfo"><input class="selectAskingInfo" name="askingInfo" type="checkbox" value="" id="111"/><i></i><span>植物意思清华大学是什么会是呢？没有搞明白。</span><div class="info" style="display:none;"><span>植物意思清华大学是什么会是呢？没有搞明白。</span><span>植物意思清华大学是什么会是呢？没有搞明白。</span><span>植物意思清华大学是什么会是呢？没有搞明白。</span></div></li>
                    </ul>
                </div>
                <div class="Contact">
                    <h2>填写联系小记：</h2>
                    <textarea name="" cols="" rows="" id="serviceLog" onchange="onTextChange()"></textarea>
                    <a class="btn_bc" id="saveServiceLog"  href="#">保存</a>
                </div>
                <div class="History">
                    <h2>历史小记：</h2>
                    <ul id="historyLogs">
                        <li class="historyLog"><p><em></em>2015-08-03 22:12:23    客服001</p><p class="on">买家咨询订单201507CP100135物流情况。</p></li>
                        <li class="historyLog"><p><em></em>2015-08-03 22:12:23    客服001</p><p class="on">买家咨询订单201507CP100135物流情况。</p></li>
                        <li class="historyLog"><p><em></em>2015-08-03 22:12:23    客服001</p><p class="on">买家咨询订单201507CP100135物流情况。</p></li>
                        <li class="historyLog"><p><em></em>2015-08-03 22:12:23    客服001</p><p class="on">买家咨询订单201507CP100135物流情况。</p></li>
                    </ul>
                </div>
            </div>

            <div id="product_info_save" style="display:none;">
                <div class="fill">
                    <div class="f_content">
                        <div class="name">
                            <ul>
                                <li><span><b class="red">＊</b>联系人：</span><span id="savedName">李</span></li>
                                <li><span><b class="red"></b>联系电话：</span><span id="savedTel">13688888888</span></li>
                            </ul>
                        </div>
                        <div class="handle">
                            <ul>
                                <li><span><b class="red">＊</b>服务类型：</span ><p id="savedType">客诉</p></li>
                                <li><span><b class="red">＊</b>优先级：</span><p id="savedLevel">急件</p></li>
                                <li><span><b class="red">＊</b>客服处理状态：</span><p class="on" id="savedStatus">选择A</p></li>
                            </ul>
                        </div>
                        <div class="problem">
                            <h2><b class="red">＊</b>咨询问题选择：</h2>
                            <ul id="savedAskingOpts">
                                <li class="savedAskingOpt"><em></em><span>商品质量问题</span></li>
                                <li class="savedAskingOpt"><em></em><span>商品质量问题</span></li>
                                <li class="savedAskingOpt"><em></em><span>退货</span></li>
                                <li class="savedAskingOpt"><em></em><span>商品质量问题</span></li>
                                <li class="savedAskingOpt"><em></em><span>修改订单</span></li>
                                <li class="savedAskingOpt"><em></em><span>商品质量问题</span></li>
                            </ul>
                        </div>
                        <div class="Commodity">
                            <h2><b class="red">＊</b>咨询商品信息：</h2>
                            <ul id="savedAskingInfos">
                                <li class="savedAskingInfo"><i></i><span>植物意思清华大学是什么会是呢？没有搞明白。</span><div class="info" style="display:none; margin-left:13px;"><span>植物意思清华大学是什么会是呢？没有搞明白。</span><span>植物意思清华大学是什么会是呢？没有搞明白。</span><span>植物意思清华大学是什么会是呢？没有搞明白。</span></div></li>
                                <li class="savedAskingInfo"><i></i><span>植物意思清华大学是什么会是呢？没有搞明白。</span><div class="info" style="display:none; margin-left:13px;"><span>植物意思清华大学是什么会是呢？没有搞明白。</span><span>植物意思清华大学是什么会是呢？没有搞明白。</span><span>植物意思清华大学是什么会是呢？没有搞明白。</span></div></li>
                            </ul>
                        </div>
                        <div class="Contact">
                            <h2>填写联系小记：</h2>
                            <span id="savedServiceLog">小记内容小记内容小记内容小记内容小记内容小记内容小记内容小记内容小记内容小记内容小记内容小记内容小记内容</span>
                            <a class="btn_bc" id="modifyServiceLog" href="#">编辑</a>
                        </div>
                        <div class="History">
                            <h2>历史小记：</h2>
                            <ul id="savedHistoryLogs">
                                <li class="historyLog"><p><em></em>2015-08-03 22:12:23    客服001</p><p class="on">买家咨询订单201507CP100135物流情况。</p></li>
                                <li class="historyLog"><p><em></em>2015-08-03 22:12:23    客服001</p><p class="on">买家咨询订单201507CP100135物流情况。</p></li>
                                <li class="historyLog"><p><em></em>2015-08-03 22:12:23    客服001</p><p class="on">买家咨询订单201507CP100135物流情况。</p></li>
                                <li class="historyLog"><p><em></em>2015-08-03 22:12:23    客服001</p><p class="on">买家咨询订单201507CP100135物流情况。</p></li>
                            </ul>
                        </div>
                    </div>
                </div>


            </div>
        </div>


    </div>



</div>



<script type="text/javascript" src="../js/sea.js"></script>
<script type="text/javascript" src="../js/jquery/jquery-1.11.0.min.js"></script>
<script type="text/javascript" src="../js/modules/init/dialogRight.js"></script>
<script type="text/javascript" src="../js/util/generalUtil.js"></script>
<script type="text/javascript" src="../js/jquery/switchable/fnx-switchable.js"></script>
<script type="text/javascript" src="http://static.beta.fn.com/cms/feiniu-image.js"></script>
<script type="text/javascript" src="../js/servicelog/servicelog.js"></script>

</body>
</html>