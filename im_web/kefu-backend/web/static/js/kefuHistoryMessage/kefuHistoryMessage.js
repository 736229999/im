/**
 * Created by wangzhen on 2016/4/6.
 */
var MaskUtil = (function () {

    var $mask, $maskMsg;

    var defMsg = '正在处理，请稍待。。。';

    function init() {
        if (!$mask) {
            $mask = $("<div class=\"datagrid-mask mymask\"></div>").appendTo("body");
        }
        if (!$maskMsg) {
            $maskMsg = $("<div class=\"datagrid-mask-msg mymask\">" + defMsg + "</div>")
                .appendTo("body").css({'font-size': '12px'});
        }

        $mask.css({width: "100%", height: $(document).height()});

        var scrollTop = $(document.body).scrollTop();

        $maskMsg.css({
            left: ( $(document.body).outerWidth(true) - 190 ) / 2
            , top: ( ($(window).height() - 45) / 2 ) + scrollTop
        });

    }

    return {
        mask: function (msg) {
            init();
            $mask.show();
            $maskMsg.html(msg || defMsg).show();
        }
        , unmask: function () {
            $mask.hide();
            $maskMsg.hide();
        }
    }

}());
var TemplateManager = {
    panel: '<div class="easyui-panel" title="$title" style="border: none;">$chatMessageList</div>',
    chatHeader: '<p>$headerContent</p>',
    chatMessageBody: '<p>$bodyContent</p>',
    sellerHeader: '<span style="color: #008040">$name($time)</span>',
    customerHeader: '<span style="color: #407ea6">$name($time)</span>',
    panelStart: '<div class="easyui-panel" title="$title" style="width: 100%">',
    panelEnd: '</div>'
};

function showInfo(content) {
    $.messager.alert('错误', '操作失败:' + content, 'error');
};
//主要负责大编辑页面的ui处理
var UIManager = {
    _sessionPageOption: {
        currentPageNo: 1,
        pageSize: 10
    },
    _historyMessagePageOption: {
        currentPageNo: 1,
        pageSize: 10
    },
    init: function () {
        this.initUI();
        this.bindUI();
        this.initData();

        this.setDefaultTime(0, 0);
        this.searchSessionInfo();
    },

    setDefaultTime: function (beginTime, endTime) {
        var mydate = new Date();
        //set default end Time :yesterday
        mydate.setDate(mydate.getDate() - endTime);
        var date = generalUtil.formateDate(mydate);
        $('#endSearchTime').val(date + " 23:59:59");
        //set default start Time:six day before
        mydate.setDate(mydate.getDate() - beginTime);
        var date = generalUtil.formateDate(mydate);
        $('#startSearchTime').val(date + " 00:00:00");
    },

    searchSessionProfile: function (type, callback) {
        var self = this;
        var params = self.getSearchParams();
        if (params == null)
            return;
        MaskUtil.mask("正在处理请稍后");

        params = $.extend(params, {
            type: type
        });

        $.post("../sessionInfo/get-session-profile",
            params,
            function (resultInfo) {
                MaskUtil.unmask();
                if (resultInfo && !!resultInfo.success) {
                    var data = resultInfo.data.resultList;
                    if (data == null) {
                        showInfo((resultInfo || {}).message || '服务器发生故障，请稍后再试');
                    }
                    callback.call(null, data);
                } else {
                    if (resultInfo.errorCode == -2) {
                        callback.call();
                    }
                    else
                        showInfo((resultInfo || {}).message || '服务器发生故障，请稍后再试');
                }
            },
            'json');
    },

    searchSessionInfo: function () {
        var self = this;
        var params = self.getSearchParams();
        if (params == null)
            return;
        MaskUtil.mask("正在处理请稍后");

        $.post("../sessionInfo/get-session-info",
            params,
            function (resultInfo) {
                MaskUtil.unmask();
                if (resultInfo && !!resultInfo.success) {
                    var data = resultInfo.data.pager.resultsForCurrentPage;

                    if (data == null) {
                        $("#seachResultTab").datagrid('loadData', {
                            total: 0,
                            rows: []
                        });
                    }
                    $('#seachResultTab').datagrid('loadData', {
                        total: resultInfo.data.pager.totalRecordSize,
                        rows: data
                    });
                } else {
                    showInfo((resultInfo || {}).message || '服务器发生故障，请稍后再试');
                }
            },
            'json');
    },
    initUI: function () {
        var self = this;
        //时间控件初始化
        $('#startSearchTime').click(function () {
            WdatePicker({
                dateFmt: 'yyyy-MM-dd HH:mm:ss',
                startDate: '%y-%M-%d 00:00:00',
                minDate: '%y-%M-#{%d-30}',
                maxDate: '%y-%M-%d'
            });
        });
        $('#endSearchTime').click(function () {
            WdatePicker({
                dateFmt: 'yyyy-MM-dd HH:mm:ss',
                startDate: '%y-%M-%d 23:59:59',
                minDate: '%y-%M-#{%d-30}',
                maxDate: '%y-%M-#{%d+1}'
            });
        });

        //表格初始化
        $('#seachResultTab').datagrid({
            nowrap: false,
            width: "auto",
            singleSelect: true,
            height: "auto",
            loadMsg: '数据装载中......',
            fitColumns: true,
            remoteSort: false,
            pagination: true,
            sortable: false,
            columns: [[
                {field: 'sessionStartTimeStr', title: '会话开始时间', width: '10%', align: 'center', sortable: false},
                {field: 'sessionEndTimeStr', title: '会话结束时间', width: '10%', align: 'center', sortable: false},
                {field: 'customerNameStr', title: '客户账号', width: '9%', align: 'center', sortable: false},
                {field: 'serviceNameStr', title: '客服账号', width: '9%', align: 'center', sortable: false},
                {field: 'sessionLengthStr', title: '会话总时长', width: '9%', align: 'center', sortable: false},
                {field: 'customerWaitingTimeStr', title: '客户等待时间', width: '9%', align: 'center', sortable: false},
                {
                    field: 'mobilePortStr', title: '移动端入口', width: '9%', align: 'center', sortable: false,
                    formatter: function (value, row, index) {
                        if (value != "-1")
                            return value;
                        else
                            return '<span style="display: inline-block" ><a class="look-chart" portType=1 >查看详情</a></span>';
                    }
                },
                {
                    field: 'pcPortStr', title: 'PC端入口', width: '9%', align: 'center', sortable: false,
                    formatter: function (value, row, index) {
                        if (value != "-1")
                            return value;
                        else
                            return '<span style="display: inline-block" ><a class="look-chart" portType=0 >查看详情</a></span>';
                    }
                },
                {
                    field: 'sessionID',
                    title: '操作',
                    width: '5%',
                    align: 'center',
                    sortable: false,
                    formatter: function (value, row, index) {
                        if (value == "-1")
                            return '--';
                        else
                            return '<span style="display: inline-block" ><a class="look-session-message" sessionId="' + value + '">[查看]</a></span>';
                    }
                },
            ]]
        });
        var pagination = $('#seachResultTab').datagrid('getPager');
        pagination.pagination({
            onSelectPage: function (pageNumber, pageSize) {
                self._sessionPageOption.currentPageNo = pageNumber;
                self._sessionPageOption.pageSize = pageSize;
                self.searchSessionInfo();
            }
        });
    },

    //获取图表显示数据
    getChartData: function (type, callback) {
        this.searchSessionProfile(type, callback);
    },

    searchMoumouNameByPartName: function (value, callback) {
        $.post(
            '../sessionInfo/get-cs-username-by-partname',
            {
                partName: value
            },
            function (resultInfo) {
                if (resultInfo && !!resultInfo.success) {
                    callback.call(null, resultInfo.data.moumouNameList);
                } else {
                    showError((resultInfo || {}).message || '服务器发生故障，请稍后再试');
                }
            }
        );
    },
    bindUI: function () {
        var self = this;

        //清空按钮
        $('#clearTime').click(function () {
            $('#startTime, #endTime').val('');
        });
        //查询按钮
        $('#search').click(function () {
            self._sessionPageOption.currentPageNo = 1;
            self.searchSessionInfo();
        });

        //重置按钮
        $('#resetCondition').click(function () {
            $('#customerName').val('');
            $('#serviceName').combo('clear');
            $('#startSearchTime, #endSearchTime').val('');
        });

        /**
         * 清空按钮
         */
        $('#clearModifyTime').click(function () {
            $('#startSearchTime, #endSearchTime').val('');
        });

        //图标统计

        $(document).on('click', ".look-chart", function () {
            $('#main_warp').hide();
            $('#chat_detail').hide();
            $('.chart-mask').show();
            ChartManager.showLoading();
            var type = $(this).attr("portType");
            self.getChartData(type, function (list) {
                var columnList = [];
                var valueList = [];
                for (var i = 0; i < list.length; i++) {
                    var item = list[i];
                    columnList.push(item.title);
                    valueList.push(item.count);
                }

                var data = {
                    columns: columnList,
                    values: valueList
                };

                if ($('#bar-tab').is('.active')) {
                    ChartManager.showBarChart(data);
                } else {
                    ChartManager.showPieChart(data);
                }
            });
        });

        $(document).on('click', ".look-session-message", function () {
            $('#main_warp').hide();
            $('#chat_detail').show();
            $('.chart-mask').hide();

            var sessionId = $(this).attr("sessionId");
            chatMassageManager.searchHistoryChat(sessionId);
        });

        //关闭图表窗口
        $('#chartCloseBtn').click(function () {
            $('#main_warp').show();
            $('#chat_detail').hide();
            $('.chart-mask').hide();
        });

        $('#chatHistoryBack').click(function () {
            $('#main_warp').show();
            $('#chat_detail').hide();
            $('.chart-mask').hide();
        });

        //切换按钮
        $('.chart-mask .chart-tab-bar>.chart-tab').click(function () {
            var $this = $(this);
            $this.siblings().removeClass('active');
            $this.addClass('active');
            var id = $this.attr('id');

            if (id == 'bar-tab') {
                $('#count-chart-pie').hide();
                $('#count-chart-bar').show();

                ChartManager.showBarChart();
            } else if (id == 'pie-tab') {
                $('#count-chart-pie').show();
                $('#count-chart-bar').hide();

                ChartManager.showPieChart();
            }
        });

        $(document).on('click', '#excelExport', function () {
            var startTime = parseInt(generalUtil.getUTCtime($('#startSearchTime').val()) / 1000);
            var endTime = parseInt(generalUtil.getUTCtime($('#endSearchTime').val()) / 1000);
            var serviceId = $('#serviceName').combo('getValue');
            var customerName = $('#customerName').val();
            if (startTime == null || endTime == null) {
                showInfo("起始时间没有设置");
                return null;
            }
            if (startTime >= endTime) {
                showInfo("结束时间需要大于开始时间");
                return null;
            }
            if (isNaN(serviceId)) {
                showInfo("找不到指定的客服");
                return null;
            }

            var paramsURL = "../sessionInfo/get-excel-file.xls?";
            var paramsURL = paramsURL + "&startTime=" + startTime.toString() +
                "&endTime=" + endTime.toString() +
                "&serviceId=" + serviceId.toString() +
                "&customerName=" + customerName.toString();

            window.location.href = paramsURL;
        });

        $('#yesterdayButton').click(function () {
            self.setDefaultTime(0, 1);
            self.searchSessionInfo();
        });

        $('#lastWeekButton').click(function () {
            self.setDefaultTime(6, 1);
            self.searchSessionInfo();
        });

        $('#lastMonthButton').click(function () {
            self.setDefaultTime(30, 1);
            self.searchSessionInfo();
        });

        var customerLabel = $('#customerIDLabel');
        var customerID = $('#customerName');
        customerID.on('keyup', function () {
            if (customerID.val() != '') {
                customerLabel.addClass('hide-placeholder');
            } else {
                customerLabel.removeClass('hide-placeholder');
            }
        });

        $('#serviceName').combobox({
            valueField: 'userId',
            textField: 'realName',
            mode: 'remote',
            panelHeight: 'auto',
            hasDownArrow: false,
            loader: function (param, success) {
                if (!param.q) {
                    return false;
                    //success([]);
                } else {
                    self.searchMoumouNameByPartName(param.q || '', function (data) {
                        success(data);
                    });
                }
            }
        });

    },
    initData: function () {
    },

    //获取查询参数
    getSearchParams: function () {
        var self = this;
        var startTime = parseInt(generalUtil.getUTCtime($('#startSearchTime').val()) / 1000);
        var endTime = parseInt(generalUtil.getUTCtime($('#endSearchTime').val()) / 1000);
        var serviceId = $('#serviceName').combo('getValue');
        if (startTime == null || endTime == null) {
            showInfo("起始时间没有设置");
            return null;
        }
        if (startTime >= endTime) {
            showInfo("结束时间需要大于开始时间");
            return null;
        }
        if (isNaN(serviceId)) {
            showInfo("找不到指定的客服");
            return null;
        }
        var params = $.extend({}, {
            customerName: $('#customerName').val(),
            serviceId: $('#serviceName').combo('getValue')
        });
        !!startTime && (params['startTime'] = startTime);
        !!endTime && (params['endTime'] = endTime);

        params['pageNo'] = self._sessionPageOption.currentPageNo;
        params['pageSize'] = self._sessionPageOption.pageSize;
        return params;
    }
};
//表格管理器
var ChartManager = (function () {
    var _tempData = {
        columns: [],
        values: []
    }
    var loadingItem;

    function saveTempData(data) {
        $.extend(_tempData, data);
    }

    return {
        init: function () {
            loadingItem = $(".chart-mask > .wrapper .chart-container > .loading");
        },
        showLoading: function () {
            $('#faq-chart-bar, #faq-chart-pie').empty();
            loadingItem.show();
        },
        showBarChart: function (chartParam) {
            chartParam = chartParam || _tempData;
            saveTempData(chartParam || _tempData);
            loadingItem.hide();
            $('#faq-chart-bar').show();

            var myChart = echarts.init(document.getElementById('count-chart-bar'));
            var columnNum = chartParam.columns.length || 1;
            var maxCharLength = Math.floor(1200 / columnNum * 0.8 / 18) || 1;

            var option = {
                tooltip: {
                    show: true
                },
                legend: {
                    data: ['浏览次数'],
                    textStyle: {
                        fontSize: 20
                    }
                },
                xAxis: [
                    {
                        type: 'category',
                        data: chartParam.columns,
                        axisLabel: {
                            interval: 0,
                            formatter: function (c) {
                                if (c.length > maxCharLength) {
                                    var stringBuilder = '';

                                    for (var i = 0; i < c.length; i++) {
                                        stringBuilder += c.charAt(i);
                                        if ((i + 1) % maxCharLength == 0) {
                                            stringBuilder += '\n';
                                        }
                                    }

                                    return stringBuilder;
                                } else {
                                    return c;
                                }
                            }
                        }
                    }
                ],
                axis: [{
                    show: false
                }],
                yAxis: [
                    {
                        type: 'value'
                    }
                ],
                series: [
                    {
                        "name": "浏览次数",
                        "itemStyle": {        // 系列级个性化样式，纵向渐变填充
                            normal: {
                                color: '#5B9BD5'
                            }
                        },
                        "type": "bar",
                        "data": chartParam.values
                    }
                ]
            };

            myChart.setOption(option);
        },
        showPieChart: function (chartParam) {
            chartParam = chartParam || _tempData;
            saveTempData(chartParam);
            loadingItem.hide();
            $('#faq-chart-pie').show();

            var data = [];
            for (var i = 0; i < chartParam.columns.length; i++) {
                data.push({
                    name: chartParam.columns[i],
                    value: chartParam.values[i]
                });
            }

            var myChart = echarts.init(document.getElementById('count-chart-pie'));

            var option = {
                title: {
                    text: '浏览次数',
                    textStyle: {
                        fontSize: 20
                    },
                    x: 'center'
                },
                tooltip: {
                    show: true
                },
                legend: {
                    orient: 'horizontal',
                    x: 'center',
                    y: 'bottom',
                    data: chartParam.columns
                },
                series: [
                    {
                        name: '浏览次数',
                        type: 'pie',
                        radius: '55%',
                        data: data
                    }
                ]
            };

            myChart.setOption(option);
        }
    }
})();

var chatMassageManager = (function () {
    return {
        _emotionPath: 'http://merchant.feiniu.com/webim/',
        init: function () {
            var currentHref = window.location.href;
            var currentDomain = currentHref.replace(new RegExp('^https?://([^/]+)/.+$'), '$1');

            if (currentDomain.indexOf('beta.fn') > -1) {
                this._emotionPath = 'http://webim.beta1.fn/webim/';
            } else if (currentDomain.indexOf('idc1.fn') > -1) {
                this._emotionPath = 'http://merchant.feiniu.com/webim/';
            }
        },
        doSearchHistoryChat: function (subMerchantId, pageSize, pageNo, sessionId, callback) {
            $.post('../sessionInfo/get-session-chat-message',
                $.extend({}, {
                    subMerchantID: subMerchantId,
                    pageSize: pageSize,
                    pageNo: pageNo,
                    sessionId: sessionId
                }),
                function (resultInfo) {
                    if (resultInfo && !!resultInfo.success) {
                        var pager = resultInfo.data.pager;
                        callback.call(this, pager);
                    } else {
                        showInfo((resultInfo || {}).message || '服务器发生故障');
                    }
                },
                'json');

        },
        transformResultToDatagridData: function (pager, _emotionPath) {
            var rowDataMap = {};
            var dateArray = [];
            var historyMessageList = pager.resultsForCurrentPage;

            if (historyMessageList == null) {
                var rowData = {};
                rowData.total = 0;
                rowData.rows = [];

                return rowData;
            }

            for (var i = 0; i < historyMessageList.length; i++) {
                var currentMessage = historyMessageList[i];
                var date = new Date();
                date.setTime(currentMessage.sendTime * 1000);
                var day = generalUtil.formateDate(date);

                if (-1 == $.inArray(day, dateArray)) {
                    dateArray.push(day);
                    rowDataMap[day] = [];
                }

                var dayMessageList = rowDataMap[day];

                var template = '';
                var name = '';
                var time = generalUtil.formateOnlyTime(date);

                name = currentMessage.fromUser.userName;
                template = TemplateManager.customerHeader;

                var messageHeader = template.replace(/\$name/, name).replace(/\$time/, time);
                var messageBody = currentMessage.msg.replace(/<image:([^>]+)>/g, function () {
                    var $1 = arguments[1] || '';
                    if ($1.indexOf('http://') != -1) {
                        return '<a class="fn-fancy-item" href="' + $1 + '"><img src="' + $1 + '" alt="聊天图片"/></a>';
                    } else {
                        if ($1.length < 12)
                            return '<a class="fn-fancy-item" href="' + _emotionPath + 'static/images/feiniu-emotion/' + $1 +
                                '"><img src="' + _emotionPath + 'static/images/feiniu-emotion/' + $1 + '" alt="聊天图片"/></a>';
                        else {
                            var url = feiniu.image.getUrl($1);
                            return '<a class="fn-fancy-item" href="' + url +
                                '"><img src="' + url + '" alt="聊天图片"/></a>';
                        }
                    }

                }).replace(/\r/g, '<br/>');

                dayMessageList.unshift(TemplateManager.chatHeader.replace(/\$headerContent/, messageHeader) +
                    TemplateManager.chatMessageBody.replace(/\$bodyContent/, messageBody));
            }

            var rowData = {};
            rowData.total = pager.totalRecordSize;
            rowData.rows = [];

            for (var i = 0; i < dateArray.length; i++) {
                rowData.rows.push({
                    date: dateArray[i],
                    messageList: rowDataMap[dateArray[i]]
                });
            }

            return rowData;
        },

        renderTable: function (rowData) {
            $('#chatMessageTable').datagrid('loadData', rowData);
            $('#chatMessageTable').datagrid('getPanel').find('.easyui-panel').each(function () {
                var $this = $(this);
                var title = $this.prop('title');

                $this.panel({
                    title: title,
                    border: false
                });
            }).closest('.datagrid-body').unbind();

            $('#chatMessageTable').datagrid('getPanel').find('.fn-fancy-item').fancybox({
                'transitionIn': 'elastic',
                'transitionOut': 'elastic',
                'speedIn': 600,
                'speedOut': 200,
                'overlayShow': false
            });
        },
        searchHistoryChat: function (sessionId) {
            $('body').css('min-height', $(document).height());
            var docHeight = $(document).height();
            var searchBarHeight = $('.searchbar').outerHeight(true);
            var panelHeaderHeight = $('#content>.title_nav').outerHeight(true);

            var searchBarWidth = $('.searchbar').width();

            $('#chatMessageTable').height(docHeight - searchBarHeight - panelHeaderHeight - 100);
            $('#chatMessageTable').width(searchBarWidth);

            (function setDataGridStyle() {
                var cardview = $.extend({}, $.fn.datagrid.defaults.view, {
                    renderRow: function (target, fields, frozen, rowIndex, rowData) {
                        var cc = [];
                        cc.push('<td>');
                        if (!frozen) {
                            cc.push(TemplateManager.panelStart.replace(/\$title/, rowData.date));

                            var chatMessageList = rowData.messageList;
                            if ($.isArray(chatMessageList) && chatMessageList.length > 0) {
                                for (var i = 0; i < chatMessageList.length; i++) {
                                    var message = chatMessageList[i];
                                    var messageHeaderTemplate = '';

                                    if (!!message.isSeller) {
                                        messageHeaderTemplate = TemplateManager.sellerHeader;
                                    } else {
                                        messageHeaderTemplate = TemplateManager.customerHeader;
                                    }

                                    cc.push(message);
                                }
                            }

                            cc.push(TemplateManager.panelEnd);
                        }
                        cc.push('</td>');
                        return cc.join('');
                    }
                });
                $('#chatMessageTable').datagrid({
                    view: cardview,
                    singleSelect: true,
                    fitColumns: true,
                    remoteSort: false,
                    pagination: true,
                    sortable: false,
                    loadMsg: '正在加载，请稍后'
                });
            })();

            var pagination = $('#chatMessageTable').datagrid('getPager');
            pagination.pagination({
                onSelectPage: function (pageNumber, pageSize) {
                    self._params.pageSize = pageSize;
                    self._params.currentPageNo = pageNumber;
                    self.searchHistoryChat(pageNumber);
                }
            });

            var self = this;
            var subMerchantId = $("#serviceName").val();
            self.doSearchHistoryChat(subMerchantId, UIManager._historyMessagePageOption.pageSize,
                UIManager._historyMessagePageOption.currentPageNo, sessionId, function (pager) {
                    var rowData = self.transformResultToDatagridData(pager, self._emotionPath);
                    self.renderTable(rowData);
                });
        }
    }
})();

$(function () {
    UIManager.init();
    ChartManager.init();
    chatMassageManager.init();
});