/**
 * Created by wanbingwen on 2015/10/28.
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
function showInfo(content) {
    $.messager.alert('错误', '操作失败:' + content, 'error');
}

//function firstChangeDuration(){
//    var self = csmonitor;
//    var obj = document.getElementById("u113_input");
//    var index = obj.selectedIndex; // 选中索引
//    var value = obj.options[index].value; // 选中值
////自动更新时间间隔
//    var time = parseInt(value) *60*1000;
//    if(self._params.durationRefreshID !=0 ){
//        clearInterval(self._params.durationRefreshID);
//    }
//
//    refreshTime = getCookie("duration");
//    setInterval(function(){
//        self.refresh();
//    },refreshTime);
//
//}


function onchangeDuration() {
    var self = csmonitor;
    var obj = document.getElementById("u113_input");
    var index = obj.selectedIndex; // 选中索引
    var value = obj.options[index].value; // 选中值
//自动更新时间间隔
    var time = parseInt(value) * 60 * 1000;
    if (self._params.durationRefreshID != 0) {
        clearInterval(self._params.durationRefreshID);
    }

    setCookie("duration", time);
    refreshTime = getCookie("duration");
    setInterval(function () {
        self.refresh();
    }, refreshTime);

}

var username = document.cookie.split(";")[0].split("=")[1];
//JS操作cookies方法!
//写cookies
function setCookie(name, value) {
    var Days = 30;
    var exp = new Date();
    exp.setTime(exp.getTime() + Days * 24 * 60 * 60 * 1000);
    document.cookie = name + "=" + escape(value) + ";expires=" + exp.toGMTString();
}

function getCookie(name) {
    var arr, reg = new RegExp("(^| )" + name + "=([^;]*)(;|$)");
    if (arr = document.cookie.match(reg))
        return unescape(arr[2]);
    else
        return null;
}


var queueDetailInfoTemplate =
    '<tr>' +
    '<td>$customerName</td>' +
    '<td>$customerSource</td>' +
    '<td>$customerWaitingTime</td>' +
    '</tr>';


var csmonitor = {

    _dataManager: {
        needRefreashStep1: false,
        currentGroupId: "",
        orgnNameList: null
    },
    _params: {
        pageSize: 10,
        currentPageNo: 0,
        needUpdateDescription: false,
        needAddGroup: false,
        durationRefreshID: 0
    },

    init: function () {
        var self = this;
        this.initUI();
        this.initData();
        this.bindUI();
        self._params.currentPageNo = 1;
        this.refresh(null);
        var isxiaogougou = getCookie("checkxiaogougou");
        var isduration = getCookie("duration") / (60 * 1000);

        $('#u113_input').val(isduration);

        if (isxiaogougou == "true") {
            $('#xiaogougou').attr("checked", "checked");
            var self = csmonitor;
            if (self._params.durationRefreshID != 0) {
                clearInterval(self._params.durationRefreshID);
            }
            refreshTime = getCookie("duration");
            setInterval(function () {
                self.refresh();
            }, refreshTime);
        }
    },

    searchQueueInfo: function (callback) {
        var self = this;
        var url = '../cs-monitor/get-queue-info';
        var params = self.getParams();
        MaskUtil.mask("正在处理请稍后");
        $.post(url,
            params,
            function (resultInfo) {
                MaskUtil.unmask();
                if (resultInfo && !!resultInfo.success) {
                    var data = resultInfo.data;
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
    bindUI: function () {
        var self = this;
        //set tab Columns
        $('#cs-monitorTab').datagrid({
            nowrap: true,
            striped: true,
            width: "auto",
            height: "auto",
            fitColumns: true,
            remoteSort: false,
            pagination: true,
            sortable: false,
            rowStyler: function (index, row) {
                return 'background-color:#f5f5f5;'; // return inline style
            },
            columns: [[
                //{field: 'checker', title: '', width: '15%', align: 'center', sortable: false, checkbox: true},
                {
                    field: 'userNameFull',
                    title: '账号总数：<label id="totalCountTitle" style=\"font-family:\'应用字体 Bold\', \'应用字体\';font-weight:700;font-size:16px;\">0</label>人',
                    width: '10%',
                    align: 'center',
                    sortable: false
                },
                {
                    field: 'onlineNameFull',
                    title: '在线客服：<label id="onlineCSTitle" style=\"font-family:\'应用字体 Bold\', \'应用字体\';font-weight:700;font-size:16px; \">0</label>人',
                    width: '20%',
                    align: 'center',
                    sortable: false,
                    formatter: function (value, rec, index) {
                        var status = rec.status;
                        var statusIcon = "<span class='online-state'>";
                        var btn1 = "";
                        if (value != "--") {
                            if (status == "0") {//上线
                                statusIcon += '<img src="../images/status_online.png">';
                            } else if (status == "2") {//忙碌
                                statusIcon += '<img src="../images/status_busy.png">';
                            } else if (status == "18") {//隐身
                                statusIcon += '<img src="../images/status_stealth.png">';
                            } else if (status == "1") {//离开
                                statusIcon += '<img src="../images/status_leave.png">';

                            }

                            statusIcon = statusIcon + '</span>';
                            btn1 = '<input type="button" class="offlineButton"  value= "下线" indexid=' + rec.cServiceId + ' >';

                        }


                        return value + '<span class="state-control">' + statusIcon + btn1 + '</span>';
                    }
                },
                {
                    field: 'limitNameFull',
                    title: '已达接待上限：<label id="arriveRecpLimitTitle" style=\"font-family:\'应用字体 Bold\', \'应用字体\';font-weight:700;font-size:16px;\">0</label>人',
                    width: '20%',
                    align: 'center',
                    sortable: false
                },
                {
                    field: 'offlineNameFull',
                    title: '离线客服：<label id="offlineCSTitle" style=\"font-family:\'应用字体 Bold\', \'应用字体\';font-weight:700;font-size:16px;\">0</label>人',
                    width: '20%',
                    align: 'center',
                    sortable: false
                },
                {
                    field: 'askingCont',
                    title: '客人咨询量：<label id="consultCountTitle" style=\"font-family:\'应用字体 Bold\', \'应用字体\';font-weight:700;font-size:16px;\">0</label>人',
                    width: '20%',
                    align: 'center',
                    sortable: false
                },
                {
                    field: 'receptionCount',
                    title: '客服接待量：<label id="CSRecpCountTitle" style=\"font-family:\'应用字体 Bold\', \'应用字体\';font-weight:700;font-size:16px; \">0</label>人',
                    width: '10%',
                    align: 'center',
                    sortable: false
                },
            ]]
        });

        var pagination = $('#cs-monitorTab').datagrid('getPager');
        pagination.pagination({
            onSelectPage: function (pageNumber, pageSize) {
                self._params.pageSize = pageSize;
                self._params.currentPageNo = pageNumber;

                self.searchDetailToSearchTab(function (pager) {
                });
            }
        });


        $(document).on('click', ".offlineButton", function () {
            var userid = $(this).attr("indexid");
            self.offlineButton(userid);
        });


        $("#cancelOffline").click(function () {
            $("#cancelOfflineDialog").window("close");
        })


        $('#queueInfoDetail').click(function () {
            $('#queueInfoDialog').window('center');
            $('#queueInfoDialog').dialog('open').dialog('setTitle', '排队详情');
            self.searchQueueInfo(function (pager) {
                var containerDetailPage = $('#queueInfoDialogDetailPage');
                containerDetailPage.empty();

                var commentList = pager.data;
                $('#cs-queue-tab').datagrid({
                    nowrap: false,
                    striped: false,
                    width: "auto",
                    height: "auto",
                    fitColumns: true,
                    remoteSort: false,
                    pagination: false,
                    sortable: false,
                    rowStyler: function (index, row) {
                        return 'background-color:#f5f5f5;'; // return inline style
                    },
                    columns: [[
                        {field: 'customerName', title: '客服账号', width: '30%', align: 'center', sortable: false},
                        {field: 'customerSource', title: '客户来源', width: '30%', align: 'center', sortable: false},
                        {field: 'customerWaitingTime', title: '等待时间', width: '40%', align: 'center', sortable: false}
                    ]]
                });
                $('#cs-queue-tab').datagrid('loadData', {
                    total: commentList.length,
                    rows: commentList
                });
            });
        });
    },

    offlineButton: function (csid) {
        MaskUtil.mask("正在处理请稍后");
        $.post(
            '../cs-monitor/ask-service-offline',
            {
                csId: csid
            },
            function (resultInfo) {
                MaskUtil.unmask();
                if (resultInfo && !!resultInfo.success) {

                    var flag = resultInfo.data.canOffline;
                    if (!flag) {
                        $('#cancelOfflineDialog').window('center');
                        $('#cancelOfflineDialog').dialog('open').dialog('setTitle', '温馨提示');

                    } else {
                        var selectedRow = $('#cs-monitorTab').datagrid('getSelected');
                        var selectedRowIndex = $('#cs-monitorTab').datagrid('getRowIndex', selectedRow);

                        if(selectedRow.limitNameFull !='--'){
                            var arriveRecpLimitCount = $('#arriveRecpLimitTitle').html()-1;
                            $('#arriveRecpLimitTitle').html(arriveRecpLimitCount);
                        }
                        selectedRow['onlineNameFull'] = '--';
                        selectedRow['limitNameFull']='--';
                        selectedRow['offlineNameFull']=selectedRow.userNameFull;
                        selectedRow['askingCont']=0;
                        selectedRow['receptionCount']=0;

                        $('#cs-monitorTab').datagrid('updateRow', {
                            index: selectedRowIndex,
                            row: selectedRow
                        });

                        var onlineCsCount = $('#onlineCSTitle').html()-1;
                        $('#onlineCSTitle').html(onlineCsCount);
                        var offlineCsCount = parseInt($('#offlineCSTitle').html(),10) + 1;
                        $('#offlineCSTitle').html(offlineCsCount);


                    }

                } else {
                    showInfo((resultInfo || {}).message || '服务器发生故障，请稍后再试');
                }
            },
            'json');
    },

    searchListCommon: function (url, params, tabel, callback) {
        var self = this;

    },

    refresh: function (callback) {
        var self = this;
        var mydate = new Date();
        //set default end Time :tody 0:0:0
        var second = mydate.getSeconds();
        var minutse = mydate.getMinutes();
        var hours = mydate.getHours();

        mydate.setSeconds(second);
        mydate.setMinutes(minutse);
        mydate.setHours(hours);
        var date = generalUtil.formateDate(mydate) + " " + generalUtil.formateOnlyTime(mydate);
        $('#refreshTime').text(date);
        var opts = $('#cs-monitorTab').datagrid('options');
        var pagination = $('#cs-monitorTab').datagrid('getPager');
        pagination.pagination('refresh', {
            pageNumber: 1,
            pageSize: 10
        });

        self.searchDetailToSearchTab(function (data) {

            $('#totalCountTitle').text(data.cscount);
            $('#onlineCSTitle').text(data.onlineCSCount);
            $('#arriveRecpLimitTitle').text(data.arriveRecpLimitCount);
            $('#offlineCSTitle').text(data.offlineCSCount);
            $('#consultCountTitle').text(data.consultCount);
            $('#CSRecpCountTitle').text(data.csrecpCount);
            $('#queuingCount').text(data.queuingCount);
        });

    },

    checkxiaogougou: function (callback) {
        var checkbox = document.getElementById('xiaogougou');//
        // alert(checkbox.checked);//是否被选中
        if (checkbox.checked) {
            setCookie("checkxiaogougou", true);

        } else {
            //没选中
            setCookie("checkxiaogougou", false);
        }
    },

    initData: function () {
        var self = this;
        var mydate = new Date();
        //set default end Time :tody 0:0:0
        var second = mydate.getSeconds();
        var minutse = mydate.getMinutes();
        var hours = mydate.getHours();

        mydate.setSeconds(second);
        mydate.setMinutes(minutse);
        mydate.setHours(hours);
        var date = generalUtil.formateDate(mydate) + " " + generalUtil.formateOnlyTime(mydate);
        $('#refreshTime').text(date);
        var isXiaogougou = getCookie("checkxiaogougou");


    },

    initUI: function () {
        var self = this;

        $('#Addfresh').click(function () {
            var self = csmonitor;
            csmonitor.refresh();
        });

        $('#xiaogougou').click(function () {
            var self = csmonitor;
            csmonitor.checkxiaogougou();
        });


    },
    searchDetailToSearchTab: function (callback) {
        var self = this;
        //var params = self.getDetailSearchParams();
        var params = self.getParams();
        //var params = null;
        self._dataManager.needRefreashStep1 = false;
        var url = '../cs-monitor/get-monitor-result-list';
        var table = '#cs-monitorTab';
        self.searchListCommon(url, params, table, callback);
    },

    searchListCommon: function (url, params, tabel, callback) {
        var self = this;
        MaskUtil.mask("正在处理请稍后");
        $.post(url,
            params,
            function (resultInfo) {
                MaskUtil.unmask();
                if (resultInfo && !!resultInfo.success) {
                    var data = resultInfo.data.pager.resultsForCurrentPage;

                    if (data == null) {
                        $(tabel).datagrid('loadData', {
                            total: 0,
                            rows: []
                        });
                    }

                    $('#cs-monitorTab').datagrid('loadData', {
                        total: resultInfo.data.pager.totalRecordSize,
                        rows: data
                    });
                    callback.call(null, resultInfo.data.csMonitorProfile);
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

    getParams: function () {
        var self = this;

        return $.extend({}, {
            pageSize: self._params.pageSize,
            currentPageNo: self._params.currentPageNo
        });
    }


}


function excelExport() {
    var params = "../cs-monitor/get-excel-file.xls?";


    var totalCountTitle = $('#totalCountTitle').val();
    var onlineCSTitle = $('#onlineCSTitle').val();
    var arriveRecpLimitTitle = $('#arriveRecpLimitTitle').val();
    var offlineCSTitle = $('#offlineCSTitle').val();
    var consultCountTitle = $('#consultCountTitle').val();
    var CSRecpCountTitle = $('#CSRecpCountTitle').val();

    var params = params + "&totalCountTitle=" + totalCountTitle.toString() +
        "&onlineCSTitle=" + onlineCSTitle.toString() +
        "&arriveRecpLimitTitle=" + arriveRecpLimitTitle.toString() +
        "&offlineCSTitle=" + offlineCSTitle.toString() +
        "&consultCountTitle=" + consultCountTitle.toString() +
        "&CSRecpCountTitle=" + CSRecpCountTitle.toString();

    window.location.href = params;
};

$(function () {
    csmonitor.init();
});