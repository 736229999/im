<%--
  Created by IntelliJ IDEA.
  User: wangchen
  Date: 2016/4/6
  Time: 15:06
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <title></title>
</head>
<body>
<script type="text/javascript" src="js/jquery-1.11.1.min.js"></script>
<script type="text/javascript" src="js/sockjs-0.3.min.js"></script>
<script type="text/javascript">
    $(function () {
        var websocket;
        if ('WebSocket' in window) {
            websocket = new WebSocket("ws://local.fn.com:8080/kefu-backend/webSocketServer");
        } else if ('MozWebSocket' in window) {
            websocket = new MozWebSocket("ws://local.fn.com:8080/kefu-backend/webSocketServer");
        } else {
            websocket = new SockJS("http://local.fn.com:8080/kefu-backend/sockjs/webSocketServer");
        }

        websocket.onopen = function (evnt) {
            console.log('Opened connection for websocket');
        };

        websocket.onmessage = function (evnt) {
            $("#msgcount").append("<p>Server-Returned: <span style='color: #FF2F2F;'>" + evnt.data + "</span></p>")
        };

        websocket.onerror = function (evnt) {
            console.log('Connection error for websocket');
        };

        websocket.onclose = function (evnt) {
            console.log('Connection close for websocket');
        };

        $('#closeBtn').click(function () {
            websocket.close();
        });

        $('#sendBtn').click(function () {
            if (websocket.readyState == websocket.OPEN) {
                var msg = JSON.stringify({"text": $('#message').val()});
                websocket.send(msg);//调用后台handleTextMessage方法
                $('#message').val('');
                $("#msgcount").append("<p>I say: <span style='color: #0099FF;'>" + msg + "</span></p>")
                console.log("发送成功!");
            } else {
                console.log("连接失败!");
            }
        });
    });
</script>

<div id="msgcount" style="margin-bottom: 30px">
</div>

<textarea id="message" rows="4"></textarea>
<button id="sendBtn" type="button">发送</button>
<button id="closeBtn" type="button">关闭链接</button>

</body>
</html>
