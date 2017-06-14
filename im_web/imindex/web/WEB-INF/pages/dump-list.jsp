<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <title>dump列表页</title>
    <style>
        .btn {
            border: 1px solid #cccccc;
            background-color: #ffffff;
            color: #333333;
            border-radius: 4px;
            cursor: pointer;
            display: inline-block;
            font-size: 14px;
            font-weight: 400;
            line-height: 1.42857;
            margin-bottom: 0;
            padding: 3px 6px;
            text-align: center;
            vertical-align: middle;
            white-space: nowrap;
        }

        .btn:active:focus, .btn:focus {
            outline: thin dotted;
            outline-offset: -2px;
        }

        .btn:focus, .btn:hover {
            background-color: #e6e6e6;
            border-color: #adadad;
            color: #333333;
        }

        .btn:active {
            background-image: none;
            box-shadow: 0 3px 5px rgba(0, 0, 0, 0.125) inset;
            outline: 0 none;
        }

        .btn[disabled], fieldset[disabled] .btn {
            box-shadow: none;
            cursor: not-allowed;
            opacity: 0.65;
        }
    </style>
</head>
<body>
<h4>dump文件列表</h4>

<div>
    <label>选择dump文件类型：</label>
    <label for="type-im-server"><input id="type-im-server" type="radio" value="im-server" name="type" checked>im-server</label>
    <label for="type-pc-merchant"><input type="radio" value="pc-merchant" name="type" id="type-pc-merchant">pc-merchant</label>
</div>

<ul id="dump-list">
    <c:forEach items="${fileList}" var="file">
        <li><a href="download.do?type=im-server&fileName=${file}" target="_self">${file}</a></li>
    </c:forEach>
</ul>

<div style="margin-left: 35px;"><a class="btn" id="load-more">点击加载更多</a></div>

<script type="text/javascript" src="../js/jquery/jquery.js"></script>
<script type="text/javascript">
    $(function () {
        var SystemEnv = {
            type: 'im-server',
            pageNo: 1,
            pageSize: 10,
            hasMore: true
        };

        var Service = {
            get: function () {
                $.post(
                        'dump-list.do',
                        SystemEnv,
                        function (resultInfo) {
                            if (resultInfo.success) {
                                var dumpList = resultInfo.data.dumpList;
                                var stringBuidler = '';

                                if (dumpList.length < SystemEnv.pageSize) {
                                    SystemEnv.hasMore = false;
                                }

                                for (var i = 0; i < dumpList.length; i++) {
                                    stringBuidler = stringBuidler + "<li><a href='download.do?type=" + SystemEnv.type + "&fileName="
                                            + dumpList[i] + "'>" + dumpList[i] + "</a></li>";
                                }

                                $('#dump-list').append(stringBuidler);
                            } else {
                                alert('服务器发生故障');
                            }
                        }
                );
            }
        };

        $('#load-more').click(function () {
            if (!SystemEnv.hasMore) {
                alert('没有更多了');
                return;
            }

            SystemEnv.pageNo++;
            Service.get();
        });

        $('input[type=radio][name=type]').click(function () {
            var $this = $(this);
            SystemEnv.type = $this.val();
            SystemEnv.pageNo = 1;
            SystemEnv.hasMore = true;
            $('#dump-list').empty();

            Service.get();
        });
    });
</script>
</body>
</html>