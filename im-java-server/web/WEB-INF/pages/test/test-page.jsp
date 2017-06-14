<%--
  Created by IntelliJ IDEA.
  User: wangzhen
  Date: 2016/1/19
  Time: 16:16
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <title></title>
</head>
<body>
<li>
    <H5>排队测试</H5>
      <input  name="role" type="radio" class="statsReset"  value=1 />场景1
      <input  name="role" type="radio" class="statsReset" value=2 />场景2
      <input  name="role" type="radio" class="statsReset" value=3 />场景3
      <input  name="role" type="radio" class="statsReset" value=4 />场景4
      <input  name="role" type="radio" class="statsReset" value=5 />场景5
      <input  name="role" type="radio" class="statsReset" value=6 />场景6
      <input  name="role" type="radio" class="statsReset" value=7 />场景7
      <input  name="role" type="radio" class="statsReset" value=8 />场景8
      <input  name="role" type="radio" class="statsReset" value=9 />场景9
    <input  type="button"  class="testQueueInsert" value="test">
</li>

<li>
    <H5>分流测试</H5>
    <input  name="shunt" type="radio" class="shuntReset"  value=1 />场景1
    <input  name="shunt" type="radio" class="shuntReset" value=2 />场景2
    <input  name="shunt" type="radio" class="shuntReset" value=3 />场景3
    <input  type="button"  class="testShuntInsert" value="test">
</li>

<li>
    <H5>邀约测试</H5>
    <input  name="invit" type="radio" class="invitReset"  value=1 />场景1
    <input  name="invit" type="radio" class="invitReset" value=2 />场景2
    <input  name="invit" type="radio" class="invitReset" value=3 />场景3
    <input  name="invit" type="radio" class="invitReset" value=4 />场景4
    <input  type="button"  class="testInvitInsert" value="test">
</li>


<li>
    <H5>客服下线测试</H5>
    <input  name="offline" type="radio" class="invitReset"  value=1 />场景1
    <input  name="offline" type="radio" class="invitReset" value=2 />场景2
    <input  name="offline" type="radio" class="invitReset" value=3 />场景3
    <input  name="offline" type="radio" class="invitReset" value=4 />场景4
    <input  type="button"  class="testOFFlineInsert" value="test">
</li>


<li>
    <H5>客服上线测试</H5>
    <input  name="online" type="radio" class="onlineReset"  value=1 />场景1
    <input  name="online" type="radio" class="onlineReset" value=2 />场景2
    <input  name="online" type="radio" class="onlineReset" value=3 />场景3
    <input  name="online" type="radio" class="onlineReset" value=4 />场景4
    <input  type="button"  class="testONlineInsert" value="test">
</li>

<li>
    <H5>关闭会话</H5>
    <input  name="closeSession" type="radio" class="closeSessionReset"  value=1 />场景1
    <input  type="button"  class="closeSessionInsert" value="test">
</li>

    <input  type="button"  class="getReceptionInfo" value="获取排队信息">

<div class="History">
    <h5>排队信息：</h5>
    <ul id="ququeInfos">
        <li class="queueinfo"><p><em></em>test</p></li>
        <li class="queueinfo"><p><em></em>test</p></li>
        <li class="queueinfo"><p><em></em>test</p></li>
        <li class="queueinfo"><p><em></em>test</p></li>
    </ul>
</div>

<div class="History">
    <h5>接待列表：</h5>
    <ul class="receptionInfoList">
        <li class="111111"><p><em></em>111111</p>
            <ul class="receptionList1">
                <li class="info"><p><em></em>$customerId</p></li>
                <li class="info"><p><em></em>$customerId</p></li>
                <li class="info"><p><em></em>$customerId</p></li>
            </ul>
        </li>

        <li class="222222"><p><em></em>222222</p>
            <ul class="receptionList2">
                <li class="info"><p><em></em>$customerId</p></li>
                <li class="info"><p><em></em>$customerId</p></li>
                <li class="info"><p><em></em>$customerId</p></li>
            </ul>
        </li>
    </ul>
</div>

</body>
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/javascript" src="js/jeasyui/jquery.easyui.min.js"></script>
<script type="text/javascript" src="js/test-page.js"></script>

</html>
