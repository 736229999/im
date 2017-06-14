<%@ page import="com.feiniu.csWebIm.bean.response.WebRightResponse" %>
<%@ page import="com.feiniu.csWebIm.utils.json.JsonUtil" %>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%--
  Created by IntelliJ IDEA.
  User: wangzhen
  Date: 2015/11/5
  Time: 11:33
  To change this template use File | Settings | File Templates.
--%>


<%
  WebRightResponse webRightResponse = (WebRightResponse) request.getAttribute("webRightResponse");
%>
<html>
<head>
  <meta charset="UTF-8" />
  <title>Document</title>
  <link href="../css/webRight/main.css" rel="stylesheet" type="text/css" />
</head>
<body style="height: 100%;background:#f8f8f8;">
<div id="D_right_ifm">
  <tmpdata data-webRightResponse='<%=JsonUtil.toJsonString(webRightResponse)%>'></tmpdata>
  <div id="J_info_tab" class="head">
    <p id="J_store" class="on">最近订单</p>
    <p id="J_product">常见问题</p>
  </div>
  <div id="J_store_info" class="with">

    <ul class="ui-info-list">
      <li>订单编号：<a class="ui-clickable" href="#" id="orderNo">2014070822012</a></li>
      <li>支付方式：<label id="payType">在线支付</label></li>
      <li>支付金额：<i class="ui-hight-light" id="price">56.02</i>元</li>
      <li>下单时间：<label id="orderTime">2018-09-19 20:10:12</label></li>
      <li style="display:none;">交易状态：<label id="orderStatus">交易完成</label></li>
      <li>咨询商品：</li>
    </ul>

    <div id="package-area" js-hook="tab" class="package">
      <div class="ui-navs">
        <a id="packageControlPrev" class="prev" href="javascript:;">左</a>
        <ul id="packageIndexs">
          <li class="cur packageIndex indexControl">包裹1</li>
          <li class="packageIndex indexControl">包裹2</li>
          <li class="packageIndex indexControl">包裹3</li>
          <li class="packageIndex indexControl">包裹4</li>
          <li class="packageIndex indexControl">包裹5</li>
        </ul>
        <a id="packageControlNext" class="next" href="javascript:;">右</a>
      </div>
      <br>
      <div >
        <div id="picScroller" class="picScroll carousel" >

          <div class="wrap ui-pic-list" style="height: 55px;width: 280px;">
            <p class="btn-area">
              <a data-role="prev" href="javascript:;" class="js_prev ui-switchable-prev-btn" id="js_prev" data-role="prev"></a>
              <a data-role="next" href="javascript:;" class="js_next ui-switchable-next-btn" id="js_nexty" data-role="next"></a>
            </p>
            <ul data-role="content" id="AskingPictures" >

              <li class="hrefPic"><a href="#"><img src="../images/pc_4.jpg" /></a></li>
              <li class="hrefPic"><a href="#"><img src="../images/pc_4.jpg" /></a></li>
              <li class="hrefPic"><a href="#"><img src="../images/pc_4.jpg" /></a></li>
              <li class="hrefPic"><a href="#"><img src="../images/pc_4.jpg" /></a></li>

              <li class="hrefPic"><a href="#"><img src="../images/pc_4.jpg" /></a></li>
              <li class="hrefPic"><a href="#"><img src="../images/pc_4.jpg" /></a></li>
              <li class="hrefPic"><a href="#"><img src="../images/pc_4.jpg" /></a></li>
              <li class="hrefPic"><a href="#"><img src="../images/pc_4.jpg" /></a></li>

              <li class="hrefPic"><a href="#"><img src="../images/pc_4.jpg" /></a></li>
              <li class="hrefPic"><a href="#"><img src="../images/pc_4.jpg" /></a></li>
              <li class="hrefPic"><a href="#"><img src="../images/pc_4.jpg" /></a></li>
              <li class="hrefPic"><a href="#"><img src="../images/pc_4.jpg" /></a></li>

            </ul>
          </div>

        </div>

        <!--物流信息-->
        <div class="Logistics" id="Logistics">
          <div class="track">
            <div class="t_left"> <i class="on"></i> <em class="on"></em> </div>
            <p>古田到站，抵达文化飞云转运站，准备下一站<br>
              2015-09-01 09:19:23</p>
          </div>
          <div class="track">
            <div class="t_left"> <i></i> <em></em> </div>
            <p>古田到站，抵达文化飞云转运站，准备下一站<br>
              2015-09-01 09:19:23</p>
          </div>
          <div class="track">
            <div class="t_left"> <i></i> <em></em> </div>
            <p>古田到站，抵达文化飞云转运站，准备下一站<br>
              2015-09-01 09:19:23</p>
          </div>
          <div class="track">
            <div class="t_left"> <i></i> </div>
            <p>古田到站，抵达文化飞云转运站，准备下一站<br>
              2015-09-01 09:19:23</p>
          </div>
        </div>
      </div>
    </div>
  </div>

  <!-- 常见问题-->
  <div id="J_product_info" style="display:none;">
    <div js-hook="cont-tab" class="problems" id="QuestionAnswers">
      <h3 class="question QuestionAnswer">1.第三方卖家配送费政策是什么？</h3>
      <div class="p_answer QuestionAnswer">
        <div class="bg"> <em></em> <span> 各地的配送方案有差异，在商品详情页面，输入送货地址后，可查询所在地区的预计送达ui-panels cur时间，具体以确认订单信息页面的 服务时间为准。 </span> </div>
      </div>
      <h3 class="question QuestionAnswer">2.飞牛网提供的发票有哪几种？</h3>
      <div class="p_answer QuestionAnswer">

        <div class="bg"> <em></em> <span> 各地的配送方案有差异，在商品详情页面，输入送货地址后，可查询所在地区的预计送达时间，具体以确认订单信息页面的 服务时间为准。 </span> </div>
      </div>
      <h3 class="question QuestionAnswer">3.第三方卖家配送费政策是什么？</h3>
      <div class="p_answer QuestionAnswer">

        <div class="bg"> <em></em> <span> 各地的配送方案有差异，在商品详情页面，输入送货地址后，可查询所在地区的预计送达时间，具体以确认订单信息页面的 服务时间为准。 </span> </div>
      </div>
      <h3 class="question QuestionAnswer">4.闪拍缴纳了保证金，参与竞拍，没有中标，什么时   候退回保证金？</h3>
      <div class="p_answer QuestionAnswer">

        <div class="bg"> <em></em> <span> 各地的配送方案有差异，在商品详情页面，输入送货地址后，可查询所在地区的预计送达时间，具体以确认订单信息页面的 服务时间为准。 </span> </div>
      </div>
      <h3 class="question QuestionAnswer">5.闪拍缴纳了保证金，参与竞拍，没有中标，什么时   候退回保证金？</h3>
      <div class="p_answer QuestionAnswer">

        <div class="bg"> <em></em> <span> 各地的配送方案有差异，在商品详情页面，输入送货地址后，可查询所在地区的预计送达时间，具体以确认订单信息页面的 服务时间为准。 </span> </div>
      </div>
    </div>
  </div>


  <!-- 快速工具 -->
  <div class="tool">
    <h2>快捷工具</h2>
    <div class="ico"> <a href="javascript:;" onClick="window.open('https://member.feiniu.com/order/orderList')"><span></span>
      <p>查询订单</p></a>
    </div>
    <div class="ico"> <a href="javascript:" onClick="window.open('http://sale.feiniu.com/help_center/hc-9.html')"><span class="on"></span>
      <p>退货说明</p></a>
    </div>
    <div class="ico"> <a href="javascript:" onClick="window.open('http://sale.feiniu.com/help_center/hc-10.html')" ><span class="on_1"></span>
      <p>常见问题</p></a>
    </div>
  </div>
</div>






<script type="text/javascript" src="../js/sea.js"></script>
<script type="text/javascript" src="../js/jquery/jquery-1.11.0.min.js"></script>
<script type="text/javascript" src="../js/webRight/dialogRight.js"></script>
<script type="text/javascript" src="../js/jquery/switchable/fnx-switchable.js"></script>
<script type="text/javascript" src="../js/webRight/right.js"></script>

</body>
</html>
