/**
 * Author: morningking
 * Date: 2016/4/7 10:09
 * Contact: 243717042@qq.com
 */

var SupervisorMonitor = (function () {
    var globalDeleteScheduler = new GlobalDeleteScheduler();

    //聊天消息展示表
    var ChatMessageTable = (function () {
        var table = $('#chatMessageTable');

        var emotionPath = "";
        var currentHref = window.location.href;
        var currentDomain = currentHref.replace(new RegExp('^https?://([^/]+)/.+$'), '$1');

        if (currentDomain.indexOf('beta.fn') > -1 || currentDomain.indexOf('fn.com') > -1) {
            emotionPath = 'http://webim.beta1.fn/webim/';
        } else if (currentDomain.indexOf('idc1.fn') > -1) {
            emotionPath = 'http://merchant.feiniu.com/webim/';
        } else if (currentDomain.indexOf('feiniu.com') > -1) {
            emotionPath = 'http://merchant.feiniu.com/webim/';
        }

        var TemplateManager = {
            panel: '<div class="easyui-panel" title="$title" style="border: none;">$chatMessageList</div>',
            chatHeader: '<p>$headerContent</p>',
            chatMessageBody: '<p>$bodyContent</p>',
            sellerHeader: '<span style="color: #008040">$name($time)</span>',
            customerHeader: '<span style="color: #407ea6">$name($time)</span>'
        };

        return {
            initTable: function () {
                table.height(parseInt($(window).height() / 2) - 20);
            },
            renderTable: function (sessionID, list) {
                var panelID = 'session-id-' + sessionID;
                var panel;

                //隐藏之前的聊天面板
                table.find('.chat-session-panel.active').hide();

                //设置当前会话为活动会话
                if (table.find(panelID).length > 0) {
                    panel = table.find('.' + panelID).show();
                } else {
                    panel = $('<div class="chat-session-panel active ' + panelID + '"></div>');
                    table.append(panel);
                }

                //渲染之
                for (var i = 0; i < list.length; i++) {
                    this.appendRecord(list[i], panel);
                }

                panel.scrollTop(panel[0].scrollHeight);
            },
            appendRecord: function (currentMessage, wrapper) {
                var date = new Date();
                date.setTime(currentMessage.sendTime * 1000);
                var template = '';
                var time = generalUtil.formateFullTime(date);
                var name = currentMessage.fromNickName;

                if (currentMessage.fromUser.userID > 200000000) {
                    template = TemplateManager.sellerHeader;
                } else {
                    template = TemplateManager.customerHeader;
                }

                var messageHeader = template.replace(/\$name/, name).replace(/\$time/, time);
                var messageBody = currentMessage.msg.replace(/<image:([^>]+)>/g, function () {
                    var $1 = arguments[1] || '';
                    if ($1.indexOf('http://') != -1) {
                        return '<a class="fn-fancy-item" href="' + $1 + '"><img src="' + $1 + '" alt="聊天图片"/></a>';
                    } else {
                        if ($1.length < 12)
                            return '<a class="fn-fancy-item" href="' + emotionPath + 'static/images/feiniu-emotion/' + $1 +
                                '"><img src="' + emotionPath + 'static/images/feiniu-emotion/' + $1 + '" alt="聊天图片"/></a>';
                        else {
                            var url = feiniu.image.getUrl($1);
                            return '<a class="fn-fancy-item" href="' + url +
                                '"><img class="photo" src="' + url + '" alt="聊天图片"/></a>';
                        }
                    }

                }).replace(/\r/g, '<br/>');

                var messagePanel = $('<div class="message-panel"><p>' + messageHeader + '</p><p class="message-body">' + messageBody + '</p></div>');
                messagePanel.find('.fn-fancy-item').fancybox({
                    'transitionIn': 'elastic',
                    'transitionOut': 'elastic',
                    'speedIn': 200,
                    'speedOut': 200,
                    'overlayShow': false
                });
                wrapper.append(messagePanel);

                messagePanel.find('img').load(function () {
                    wrapper.scrollTop(wrapper[0].scrollHeight);
                });
            }
        };
    })();

    //聊天会话展示表
    var ChatSessionTable = (function () {
        var table = $('#chatSessionTable');
        var sessionIndicatorTip = $('#session-indicator-tip');
        var FromPageRules = {
            '-1': '未知来源',
            '0': 'portal页',
            '1': '购物车页',
            '2': '订单页'
        };
        var UserLevel = {
            0: '普通会员',
            1: '银卡会员',
            2: '金卡会员',
            3: '白金卡会员'
        };

        var FetchedSessionList = {};
        var currentSessionID = -1;
        var ChatUserToSessionMapper = {};

        function zeroFilled(i) {
            return i < 10 ? '0' + i : i;
        }

        function formatTime(duration) {
            var hours = Math.floor(duration / 3600);
            var minutes = Math.floor((duration - hours * 3600) / 60);
            var seconds = duration - minutes * 60 - hours * 3600;

            return zeroFilled(hours) + ':' + zeroFilled(minutes) + ':' + zeroFilled(seconds);
        }

        return {
            initTable: function () {
                table.datagrid({
                    nowrap: false,
                    width: "auto",
                    singleSelect: true,
                    height: "auto",
                    loadMsg: '数据装载中......',
                    fitColumns: true,
                    remoteSort: false,
                    sortable: false,
                    columns: [[
                        {
                            field: 'customerName',
                            title: '客户账号',
                            width: '20%',
                            align: 'center',
                            sortable: false,
                            formatter: function (value, row) {
                                return value + '（' + UserLevel['' + row.customerLevel] + '）';
                            }
                        },
                        {
                            field: 'fromDevice', title: '客户来源', width: '15%', align: 'center', sortable: false,
                            formatter: function (value, row) {
                                var fromDevice = !!value ? '手机' : 'PC';
                                var fromPage = FromPageRules[row['fromPage']] || '';

                                return fromDevice + '-' + fromPage;
                            }
                        },
                        {
                            field: 'queuedTime',
                            title: '客户等待时间' +
                            '<span class="table-sort-endicator-panel">' +
                            '<span name="isStartChatTimeAsc" class="table-sort-indicator asc fa fa-sort-asc" title="点击进行升序排列"></span>' +
                            '<span name="isStartChatTimeAsc" class="table-sort-indicator desc fa fa-sort-desc" title="点击进行降序排列"></span>' +
                            '</span>',
                            width: '15%',
                            align: 'center',
                            sortable: false,
                            formatter: function (value) {
                                return formatTime(value);
                            }
                        },
                        {
                            field: 'startChatTime',
                            title: '会话开始时间' +
                            '<span class="table-sort-endicator-panel">' +
                            '<span name="isStartChatTimeAsc" class="table-sort-indicator asc fa fa-sort-asc" title="点击进行升序排列"></span>' +
                            '<span name="isStartChatTimeAsc" class="table-sort-indicator desc fa fa-sort-desc" title="点击进行降序排列"></span>' +
                            '</span>'
                            ,
                            width: '15%', align: 'center', sortable: false,
                            formatter: function (value) {
                                return generalUtil.formateFullTime(new Date(value * 1000));
                            }
                        },
                        {
                            field: 'serviceName',
                            title: '客服账号',
                            width: '20%',
                            align: 'center',
                            sortable: false
                        },
                        {
                            field: 'id', title: '操作', width: '15%', align: 'center', sortable: false,
                            formatter: function (value, row) {
                                var id = value;
                                var customerID = row.customerID;
                                var serviceID = row.serviceID;

                                var mapper = ChatUserToSessionMapper[customerID];
                                if (mapper == null) {
                                    mapper = [];
                                    ChatUserToSessionMapper[customerID] = mapper;
                                }
                                mapper.indexOf(id) < 0 && mapper.push(id);

                                mapper = ChatUserToSessionMapper[serviceID];
                                if (mapper == null) {
                                    mapper = [];
                                    ChatUserToSessionMapper[serviceID] = mapper;
                                }
                                mapper.indexOf(id) < 0 && mapper.push(id);

                                return '<span class="intervene-session" title="点击插入会话">[插入会话]</span>';
                            }
                        }
                    ]],
                    onLoadSuccess: function () {
                        for (var key in  ChatUserToSessionMapper) {
                            if (ChatUserToSessionMapper.hasOwnProperty(key)) {
                                var value = ChatUserToSessionMapper[key];
                                value != null && (value.sort());
                            }
                        }

                        $('.chat-session-panel').find('.intervene-session').tooltip();
                        LogUtil.log(JSON.stringify(ChatUserToSessionMapper));
                    }
                });
            },
            renderTable: function (data) {
                table.datagrid('loadData', data);
            },
            bindRowClick: function (callback) {
                table.datagrid({
                    onClickRow: function (index, row) {
                        var id = row['id'];

                        if (!FetchedSessionList[id]) {
                            callback.call(null, row);
                        } else {
                            $('.chat-session-panel.active').hide();
                            var panel = $('.chat-session-panel.session-id-' + id).show();
                            panel.scrollTop(panel[0].scrollHeight);
                        }

                        sessionIndicatorTip.html('（正在监控 [' + row.serviceName + '] 与 [' + row.customerName + '] 的会话）')
                    }
                });
            },
            findSessionID: function (fromID, toID) {
                var fromSessionIDList = ChatUserToSessionMapper[fromID];
                var toSessionIDList = ChatUserToSessionMapper[toID];
                var intersectSessionIDList = [];

                if (fromSessionIDList != null && toSessionIDList != null
                    && fromSessionIDList.length > 0 && toSessionIDList.length > 0) {
                    var fromIndex = 0, toIndex = 0;

                    while (fromIndex < fromSessionIDList.length && toIndex < toSessionIDList.length) {
                        var fromSessionID = fromSessionIDList[fromIndex];
                        var toSessionID = toSessionIDList[toIndex];

                        if (fromSessionID < toSessionID) {
                            fromIndex++;
                        } else if (fromSessionID == toSessionID) {
                            intersectSessionIDList.indexOf(fromSessionID) < 0 && intersectSessionIDList.push(fromSessionID);
                            fromIndex++;
                            toIndex++;
                        } else {
                            toIndex++;
                        }
                    }
                }

                return intersectSessionIDList;
            },
            //获取当前选中行
            getSelectedRow: function () {
                return $.extend({}, table.datagrid('getSelected'));
            },
            //设置当前actived的session
            setActiveSessionID: function (id) {
                currentSessionID = id;
            },
            getActiveSessionID: function () {
                return currentSessionID;
            },
            //标注session已取过聊天记录
            setSessionFetched: function (id) {
                FetchedSessionList[id] = true;
            }
        };
    })();

    //websocket管理
    var ChatWebSocketManager = (function () {
        var websocket;
        var heartBeatInterval = 5000;
        var isClosed = true;

        return {
            init: function () {
                var host = (function parseHost() {
                    var currentHref = window.location.href;
                    var currentDomain = currentHref.replace(new RegExp('^https?://([^/]+)/.+$'), '$1');

                    if (currentDomain.indexOf('fn.com') > -1)/**beta环境**/{
                        if (currentDomain.indexOf('local') > -1) {
                            return "ws://local.fn.com:8080/kefu-backend";
                        } else if (currentDomain.indexOf('beta') > -1) {
                            return "ws://kefu-backend.beta.fn.com:8080";
                        } else {
                            return "ws://kefu-merchant.dev.fn.com:18080";
                        }
                    } else /**prev/online环境**/{
                        if (window._globalInfo.isPreview) {
                            return "wss://kefu-backend.feiniu.com";
                        } else {
                            var random = Math.floor(Math.random() * 2) + 1;
                            return "wss://kefu-backend0" + random + ".feiniu.com";
                        }
                    }
                })();

                if ('WebSocket' in window) {
                    websocket = new WebSocket(host + "/webSocketServer");
                } else if ('MozWebSocket' in window) {
                    websocket = new MozWebSocket(host + "/webSocketServer");
                } else {
                    alert('您当前的浏览器不支持websocket，请使用chrome或者firefox浏览器');
                    throw {
                        'message': '您当前的浏览器不支持websocket，请使用chrome或者firefox浏览器'
                    };
                }

                websocket.onopen = function (evnt) {
                    LogUtil.log('Opened connection for websocket');
                    isClosed = false;

                    setTimeout(function () {
                            if (!isClosed) {
                                LogUtil.log('Send heartbeat to server @' + new Date().getTime())

                                websocket.send(null);
                                setTimeout(arguments.callee, heartBeatInterval);
                            }
                        }, heartBeatInterval
                    );
                };

                websocket.onmessage = function (event) {
                    var message = event.data;

                    if (!!message) {
                        LogUtil.log('成功收到服务端推过来的聊天消息：' + message);

                        message = JSON.parse(message);

                        var fromID = message.from_id;
                        var toID = message.to_id;

                        var findSessionIDList = ChatSessionTable.findSessionID(fromID, toID);

                        for (var i = 0; i < findSessionIDList.length; i++) {
                            var panel = $('.chat-session-panel.session-id-' + findSessionIDList[i]);

                            ChatMessageTable.appendRecord({
                                msg: message.data,
                                fromUser: {
                                    userID: fromID,
                                    userName: message.from_nickname,
                                },
                                sendTime: message.send_time,
                                fromNickName: message.from_nickname
                            }, panel);

                            panel.scrollTop(panel[0].scrollHeight + 450);
                        }

                        LogUtil.log('成功将消息分发给会话列表：' + JSON.stringify(findSessionIDList));
                    }
                };

                websocket.onerror = function (evnt) {
                    isClosed = true;
                    LogUtil.log('Connection error for websocket');
                };

                websocket.onclose = function (evnt) {
                    isClosed = true;
                    LogUtil.log('Connection close for websocket');
                };
            },
            subcribeSession: function (message) {
                if (!!message) {
                    message = JSON.stringify(message);
                    websocket.send(message);
                    LogUtil.log('订阅会话信息：' + message);
                }
            }
        }
    })();

    return {
        init: function () {
            this.initUI();
            this.bindUI();
            this.initData();
        },
        initUI: function () {
            ChatMessageTable.initTable();
            ChatSessionTable.initTable();
        },
        bindUI: function () {
            var context = this;

            (function setupChatSessionTable() {
                ChatSessionTable.bindRowClick(function (row) {
                    if (row != ChatSessionTable.getActiveSessionID()) {
                        ChatSessionTable.setActiveSessionID(row.id);
                        ChatWebSocketManager.subcribeSession({idList: [row.customerID, row.serviceID]});
                        context.getChatMessageList(row);
                    }
                });

                //排序功能
                $('.chat-session-panel .table-sort-indicator').click(function () {
                    var $this = $(this);

                    var params = {};
                    params[$this.attr('name')] = $this.is('.asc');
                    context.getChatSessionList(params);
                });

                //插入会话功能绑定
                $(document).on('click', '.intervene-session', function () {
                    var row = ChatSessionTable.getSelectedRow();
                    if (!window._globalInfo.userIDInMoumou) {
                        AlertUtil.showError('只有主管才能插入,请确认您的身份是主管，并且已经将数据导入至哞哞数据库');
                        return;
                    }

                    $.ajax({
                        url: 'check-is-legal-to-intervene-session.do',
                        type: 'post',
                        dataType: 'json',
                        data: {
                            buyerId: row.customerID,
                            csId: row.serviceID,
                            masterId: window._globalInfo.userIDInMoumou,
                            sessionID: row.id
                        },
                        success: function (resultInfo) {
                            if (resultInfo.success) {
                                var data = resultInfo.data;

                                if (!!data.isChatSessionClosed) {
                                    AlertUtil.showError('该会话已结束！');
                                } else {
                                    if (!!data.isIntervendByOtherMonitor) {
                                        AlertUtil.showError('该会话已被其它主管插入！');
                                    } else {
                                        globalDeleteScheduler.setContext(this).setFunction(function () {
                                            $('#confirmInvokeWindow').window('close');
                                            window.location.href = 'feiniu-cs://?monitorID=' + window._globalInfo.userIDInMoumou
                                                + "&serviceID=" + row.serviceID + "&customerID=" + row.customerID;
                                        });
                                        //确认调起客户端
                                        $('#confirmInvoke').unbind('click.confirmInvoke').bind('click.confirmInvoke', function () {
                                            globalDeleteScheduler.run();
                                        });

                                        //取消调起客户端
                                        $('#cancelInvoke').unbind('click.cancenlInvoke').bind('click.cancenlInvoke', function () {
                                            globalDeleteScheduler.cancel();
                                            $('#confirmInvokeWindow').window('close');
                                        });

                                        $('#confirmInvokeWindow').show(function () {
                                            $(this).window('center').window('open');
                                        });
                                    }
                                }
                            } else {
                                AlertUtil.showError(resultInfo.errorMessage || '服务器发生故障');
                            }
                        },
                        error: function (XMLHttpRequest) {
                            if (!!XMLHttpRequest) {
                                //说明正在请求中，可能是外界原因导致中断，如刷新窗口，这种情况忽略之
                                if (XMLHttpRequest.status == 0 && XMLHttpRequest.readyState == 0) {
                                    //omit
                                } else {
                                    var errorMessage = '网络发生故障，状态码：readyState=' + XMLHttpRequest.readyState + '; status=' + XMLHttpRequest.status;
                                    LogUtil.log(errorMessage);
                                }
                            }
                        }

                    });
                });

                //刷新页面
                $('#refreshBtn').click(function () {
                    window.location.reload(true);
                });
            })();
        },
        initData: function () {
            ChatWebSocketManager.init();
            this.getChatSessionList({});
        },
        getChatSessionList: function (param) {
            $.ajax({
                url: 'get-active-chat-session-list.do',
                dataType: 'json',
                data: param,
                success: function (resultInfo) {
                    if (resultInfo.success) {
                        ChatSessionTable.renderTable({rows: resultInfo.data.list});
                    } else {
                        alert(resultInfo.errorMessage || '服务器发生故障');
                    }
                },
                error: function (XMLHttpRequest) {
                    if (!!XMLHttpRequest) {
                        //说明正在请求中，可能是外界原因导致中断，如刷新窗口，这种情况忽略之
                        if (XMLHttpRequest.status == 0 && XMLHttpRequest.readyState == 0) {
                            //omit
                        } else {
                            var errorMessage = '网络发生故障，状态码：readyState=' + XMLHttpRequest.readyState + '; status=' + XMLHttpRequest.status;
                            LogUtil.log(errorMessage);
                        }
                    }
                }
            });
        },
        getChatMessageList: function (row) {
            $.ajax({
                url: 'get-history-message-list-within-session.do',
                type: 'post',
                dataType: 'json',
                data: {
                    customerID: row.customerID,
                    serviceID: row.serviceID,
                    startChatTime: row.startChatTime
                },
                success: function (resultInfo) {
                    if (resultInfo.success) {
                        ChatMessageTable.renderTable(row.id, resultInfo.data.list);
                        ChatSessionTable.setSessionFetched(row.id);
                    } else {
                        alert(resultInfo.errorMessage || '服务器发生故障');
                    }
                },
                error: function (XMLHttpRequest) {
                    if (!!XMLHttpRequest) {
                        //说明正在请求中，可能是外界原因导致中断，如刷新窗口，这种情况忽略之
                        if (XMLHttpRequest.status == 0 && XMLHttpRequest.readyState == 0) {
                            //omit
                        } else {
                            var errorMessage = '网络发生故障，状态码：readyState=' + XMLHttpRequest.readyState + '; status=' + XMLHttpRequest.status;
                            LogUtil.log(errorMessage);
                        }
                    }
                }
            });
        }
    }
})();

$(function () {
    SupervisorMonitor.init();
});
