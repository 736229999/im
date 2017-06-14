<%--
  Created by IntelliJ IDEA.
  User: chenqing
  Date: 2016/2/14
  Time: 16:21
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
  <title>哞哞工作版下载页</title>
  <link href="css/content.css" rel="stylesheet" type="text/css"/>
  <style>
    .wrapper {
      font-family: "Microsoft Yahei", 微软雅黑, "Tahoma", "SimSun";
      padding: 20px;
    }
  </style>
</head>
<body>
<div class="wrapper">
  <h3>飞牛哞哞工作版（运营使用）</h3>

  <p class="version">版本：${config.version}</p>

  <p class="size">大小：${config.size}</p>

  <p class="update-time">更新日期：${config.updateTime}</p>
  <p class="system">系统：${config.system}</p>

  <p><a class="search_btn" href="material/download/pc/${config.fileName}" style="text-decoration: none;font-size: 15px">立即下载</a></p>
</div>
</body>
</html>
