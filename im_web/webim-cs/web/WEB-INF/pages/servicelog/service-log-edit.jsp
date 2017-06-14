<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title>服务小记</title>
    <link type="text/css" rel="stylesheet" href="../css/main.css"/>
</head>

<body style="height: 100%;">
<div id="D_right_ifm">
    <div id="J_info_tab" class="head">
        <p id="J_store" class="on">客户</p>

        <p id="J_product">服务小记</p>
    </div>
    <div id="J_store_info" class="with">

        <ul class="ui-info-list ui-line">
            <li>用户名：<span class="fr" id="userName">FN_1992365</span></li>
            <li>
                <div class="fl">昵称：<span class="ui-width-limit" id="nickName" title="#">--</span></div>
                <div class="fr">会员等级：<span id="level">普通会员</span></div>
            </li>

        </ul>
        <div id="consultative-prd" class="ui-line">
            <ul class="ui-info-list">
                <li>订单编号：<span class="fr" id="orderID">20150802918pc121290</span><span class="fr"
                                                                                       id="result">交易完成</span></li>
            </ul>


            <p class="title">咨询商品：</p>

            <div class="picScroll carousel" data-widget="clz:fnx/cmp/switchable/carousel" widget-options="{
			        					        		  effect: 'scrollx',
			        					        		  interval: 1000,
			        					        		  duration: 300,
			        					        		  autoplay: false,
			        					        		  circular: true,
			        					        		  size: 4,
			        					        		  step: 4,
			        					        		  hasTriggers : false
			        					        		}">
                <div class="wrap ui-pic-list" style="height: 75px;width: 338px;">
                    <p class="btn-area">
                        <a data-role="prev" href="javascript:;" class="js_prev ui-switchable-prev-btn" id="js_prev"
                           data-role="prev"></a>
                        <a data-role="next" href="javascript:;" class="js_next ui-switchable-next-btn" id="js_nexty"
                           data-role="next"></a>
                    </p>
                    <ul data-role="content">
                        <li><a href="#"><img src="../images/pc_4.jpg"/></a></li>
                        <li><a href="#"><img src="../images/pc_5.jpg"/></a></li>
                        <li><a href="#"><img src="../images/pc_4.jpg"/></a></li>
                        <li class="on"><a href="#"><img src="../images/pc_5.jpg"/></a></li>

                        <li><a href="#"><img src="../images/pc_4.jpg"/></a></li>
                        <li><a href="#"><img src="../images/pc_5.jpg"/></a></li>
                        <li><a href="#"><img src="../images/pc_4.jpg"/></a></li>
                        <li class="on"><a href="#"><img src="../images/pc_5.jpg"/></a></li>

                        <li><a href="#"><img src="../images/pc_4.jpg"/></a></li>
                        <li><a href="#"><img src="../images/pc_5.jpg"/></a></li>
                        <li><a href="#"><img src="../images/pc_4.jpg"/></a></li>
                        <li class="on"><a href="#"><img src="../images/pc_5.jpg"/></a></li>
                    </ul>
                </div>

            </div>


        </div>
        <ul class="ui-info-list ui-line">
            <li>最近交易：<span class="fr" id="dealTime">2015-07-10 19:20:33</span> <span class="fr"
                                                                                     id="dealResult">交易完成</span></li>
            <li>订单编号：<span class="fr" id="dealOrder">20150802918pc121290</span><span class="fr" id="totalCost">合计：<em>￥123.00</em></span>
            </li>

        </ul>
        <ul class="ui-info-list">
            <li>累计成交<span class="fr" id="allDeal">25笔，</span><span class="fr" id="allNum">1000件宝贝</span></li>
            <li>客单价：<span class="fr" id="meanPrice">￥80123.00 </span></li>

        </ul>
    </div>

    <!-- 常见问题-->
    <div id="J_product_info" style="display:none;">
        <div class="fill">
            <div class="f_content">
                <div class="name">
                    <ul>
                        <li><span><b class="red">＊</b>联系人：</span><span>李哞哞</span></li>
                        <li><span><b class="red"></b>联系电话：</span><span>13688888888</span></li>
                    </ul>
                </div>
                <div class="handle">
                    <ul>
                        <li><span><b class="red">＊</b>服务类型：</span>

                            <p>客诉</p></li>
                        <li><span><b class="red">＊</b>优先级：</span>

                            <p>急件</p></li>
                        <li><span><b class="red">＊</b>客服处理状态：</span><span class="on">选择A</span></li>
                    </ul>
                </div>
                <div class="problem">
                    <h2><b class="red">＊</b>咨询问题选择：</h2>
                    <ul>
                        <li><em></em><span>商品质量问题</span></li>
                        <li><em></em><span>商品质量问题</span></li>
                        <li><em></em><span>退货</span></li>
                        <li><em></em><span>商品质量问题</span></li>
                        <li><em></em><span>修改订单</span></li>
                        <li><em></em><span>商品质量问题</span></li>
                    </ul>
                </div>
                <div class="Commodity">
                    <h2><b class="red">＊</b>咨询商品信息：</h2>
                    <ul>
                        <li><i></i><span>植物意思清华大学是什么会是呢？没有搞明白。</span>

                            <div class="info" style="display:none; margin-left:13px;"><span>植物意思清华大学是什么会是呢？没有搞明白。</span><span>植物意思清华大学是什么会是呢？没有搞明白。</span><span>植物意思清华大学是什么会是呢？没有搞明白。</span>
                            </div>
                        </li>
                        <li><i></i><span>植物意思清华大学是什么会是呢？没有搞明白。</span>

                            <div class="info" style="display:none; margin-left:13px;"><span>植物意思清华大学是什么会是呢？没有搞明白。</span><span>植物意思清华大学是什么会是呢？没有搞明白。</span><span>植物意思清华大学是什么会是呢？没有搞明白。</span>
                            </div>
                        </li>
                    </ul>
                </div>
                <div class="Contact">
                    <h2>填写联系小记：</h2>
                    <span>小记内容小记内容小记内容小记内容小记内容小记内容小记内容小记内容小记内容小记内容小记内容小记内容小记内容</span>
                    <a class="btn_bc" href="rightService.html">编辑</a>
                </div>
                <div class="History">
                    <h2>历史小记：</h2>
                    <ul>
                        <li><p><em></em>2015-08-03 22:12:23 客服001</p>

                            <p class="on">买家咨询订单201507CP100135物流情况。</p></li>
                        <li><p><em></em>2015-08-03 22:12:23 客服001</p>

                            <p class="on">买家咨询订单201507CP100135物流情况。</p></li>
                        <li><p><em></em>2015-08-03 22:12:23 客服001</p>

                            <p class="on">买家咨询订单201507CP100135物流情况。</p></li>
                        <li><p><em></em>2015-08-03 22:12:23 客服001</p>

                            <p class="on">买家咨询订单201507CP100135物流情况。</p></li>
                    </ul>
                </div>
            </div>
        </div>


    </div>
</div>


<script type="text/javascript" src="../js/sea.js"></script>
<script type="text/javascript" src="../js/modules/init/dialogRight.js"></script>
<script type="text/ecmascript" src="../js/jquery/jquery-1.11.0.min.js"></script>
<script>
    $(document).ready(function () {
        $(".Commodity i").click(function () {
            $(this).toggleClass('on');
            $(this).nextAll(".info").slideToggle();
        })
    });
</script>
</body>
</html>