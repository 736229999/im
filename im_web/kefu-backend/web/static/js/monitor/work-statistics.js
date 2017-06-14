/**
 * Created by wangchen on 2015/4/16.
 */
var TemplateManager = {
    panel: '<div class="easyui-panel" title="$title" style="border: none;">$chatMessageList</div>',
    chatHeader: '<p>$headerContent</p>',
    chatMessageBody: '<p>$bodyContent</p>',
    sellerHeader: '<span style="color: #008040">$name($time)</span>',
    customerHeader: '<span style="color: #407ea6">$name($time)</span>',
    panelStart: '<div class="easyui-panel" title="$title" style="width: 100%">',
    panelEnd: '</div>'
};
var tipsTitle =new Array("商家的公司名称","商家编码","统计时间段内登录过哞哞的客服总人数",
            "商家所开店铺名称","统计时间内登录哞哞时长总和","咨询客服的总客人数",
            "哞哞接待的总客人数","咨询客服的总次数","客服接待的总次数","客服接待的总次数/咨询客服的总次数","客服首次响应客户咨询时长的平均值",
            "客服首次响应时间小于等于90S的会话/有客服接待的所有会话","已显示表示商家店铺正常经营");
var myview = $.extend({},$.fn.datagrid.defaults.view,{
    onAfterRender:function(target){
        $.fn.datagrid.defaults.view.onAfterRender.call(this,target);
        var opts = $(target).datagrid('options');
        var vc = $(target).datagrid('getPanel').children('div.datagrid-view');
        vc.children('div.datagrid-empty').remove();
        if (!$(target).datagrid('getRows').length){
            var d = $('<div class="datagrid-empty"></div>').html(opts.emptyMsg || 'no records').appendTo(vc);
            d.css({
                position:'absolute',
                left:0,
                top:50,
                width:'100%',
                textAlign:'center'
            });
        }
    }
});
var MaskUtil = (function(){

        var $mask,$maskMsg;

        var defMsg = '正在处理，请稍待。。。';

        function init(){
            if(!$mask){
                $mask = $("<div class=\"datagrid-mask mymask\"></div>").appendTo("body");
            }
            if(!$maskMsg){
                $maskMsg = $("<div class=\"datagrid-mask-msg mymask\">"+defMsg+"</div>")
                    .appendTo("body").css({'font-size':'12px'});
            }

            $mask.css({width:"100%",height:$(document).height()});

            var scrollTop = $(document.body).scrollTop();

            $maskMsg.css({
                left:( $(document.body).outerWidth(true) - 190 ) / 2
                ,top:( ($(window).height() - 45) / 2 ) + scrollTop
            });

        }

        return {
            mask:function(msg){
                init();
                $mask.show();
                $maskMsg.html(msg||defMsg).show();
            }
            ,unmask:function(){
                $mask.hide();
                $maskMsg.hide();
            }
        }

    }());
function showInfo(content) {
    $.messager.alert('错误', '操作失败:' + content, 'error');
}
function checkNumber(pthis){
    pthis.value=pthis.value.replace(/[^\.\d]/g,'');

    if(pthis.value.toString().indexOf('.') >0){
        if(pthis.value.split('.').length>2) {
            pthis.value = pthis.value.split('.')[0] + '.' + pthis.value.split('.')[1]
        }
        if(pthis.value.toString().split(".").length > 1 &&  pthis.value.toString().split(".")[1].length>1)
        {
            pthis.value = pthis.value.split('.')[0] + '.' + pthis.value.split('.')[1].substring(0,1);
        }
    }else{
        pthis.value = pthis.value.toString().substring(0,10);
    }

}
function checkNumber2lessthan100(pthis){
    pthis.value=pthis.value.replace(/[^\.\d]/g,'');
    if(pthis.value.toString().indexOf('.') >0 ) {
        pthis.value = pthis.value.split('.')[0] + '.' + pthis.value.split('.')[1]
        if(pthis.value.toString().split(".")[1].length>1) {
            pthis.value = pthis.value.split('.')[0] + '.' + pthis.value.split('.')[1].substring(0,1);
        }
    }else{
        pthis.value = pthis.value.toString().substring(0,3);
    }
    if(parseFloat(pthis.value)>100)
        $('#errormessage').show();
    else
        $('#errormessage').hide();
}
function checkNumberlessthan100(pthis){
    pthis.value=pthis.value.replace(/[^\.\d]/g,'');

    if(pthis.value.toString().indexOf('.') >0 ) {
        pthis.value = pthis.value.split('.')[0] + '.' + pthis.value.split('.')[1]
        if(pthis.value.toString().split(".")[1].length>1) {
            pthis.value = pthis.value.split('.')[0] + '.' + pthis.value.split('.')[1].substring(0,1);
        }
    }else{
        pthis.value = pthis.value.toString().substring(0,2);
    }
}
function excelExport() {
    var params= "../monitor/get-excel-file.xls?";

    var  responsein90secondsRateStart= $('#responsein90secondsRateStart').val() == "" ? -1 :parseFloat($('#responsein90secondsRateStart').val());
    var  responsein90secondsRateEnd= $('#responsein90secondsRateEnd').val() == "" ? -1 :parseFloat($('#responsein90secondsRateEnd').val());

    var sellerID= $('#sellerID').val();
    var storeName= $('#storeName').combo('getText');
    var startTime=parseInt(generalUtil.getUTCtime($('#startTime').val() + " 00:00:00") / 1000);
    var endTime=parseInt(generalUtil.getUTCtime($('#endTime').val()+ " 23:59:59") / 1000 + 1);
    var sellerName=$('#sellerName').combo('getText');

    var startResponseIn90sRate= $('#responsein90secondsRateStart').val() == "" ? -1 :parseFloat($('#responsein90secondsRateStart').val());
    var endResponseIn90sRate= $('#responsein90secondsRateEnd').val() == "" ? -1 :parseFloat($('#responsein90secondsRateEnd').val());

    var startOnlineTime= $('#startOnlineTime').val() == "" ? -1 :parseFloat($('#startOnlineTime').val());
    var endOnlineTime= $('#endOnlineTime').val() == "" ? -1 :parseFloat($('#endOnlineTime').val());

    var params = params + "&sellerID="+ sellerID.toString() +
        "&storeName="+ storeName.toString()  +
        "&startTime="+ startTime.toString()  +
        "&endTime="+ endTime.toString()  +
        "&sellerName="+ sellerName.toString()  +
        "&startOnlineTime="+ startOnlineTime.toString()  +
        "&endOnlineTime="+ endOnlineTime.toString()  +
        "&startResponseIn90sRate="+ startResponseIn90sRate.toString()  +
        "&endResponseIn90sRate="+ endResponseIn90sRate.toString();

        window.location.href = params;
};
var workStatistics = {
    _timeDelta: 90 * 24 * 60 * 60 * 1000,
    _emotionPath: 'http://merchant.feiniu.com/webim/',
    _merchantInfo: {
        servicerIDList: [],
        storeName: ''
    },
    _params: {
        pageSize: 10,
        currentPageNo: 1
    },
    init: function () {
        this.initUI();
        this.initData();
        this.bindUI();
    },
    initData: function () {
        var self = this;
        var mydate = new Date();
        var time = generalUtil.formateOnlyTime(mydate);
        //set default end Time :yesterday
        mydate.setDate(mydate.getDate() -1);
        var date = generalUtil.formateDate(mydate);
        $('#endTime').val(date);
        //set default start Time:yesterday
        mydate.setDate(mydate.getDate() );
        var date = generalUtil.formateDate(mydate);
        $('#startTime').val(date);
        $('#errormessage').hide();

        //set search result style

            $('#sellerWorkTab').datagrid({
                nowrap:true,
                striped:true,
                width:"auto",
                height:"auto",
                fitColumns: true,
                remoteSort: false,
                pagination: true,
                sortable: false,
                rowStyler: function(index,row){
                    return 'background-color:#f5f5f5;'; // return inline style
                },
                columns: [[
                    //{field: 'checker', title: '', width: '15%', align: 'center', sortable: false, checkbox: true},
                    {field: 'merchantName', title: '商家名称', width: '11%', align: 'center', sortable: false},
                    {field: 'merchantIDStr', title: '商家编码', width: '8%', align: 'center', sortable: false},
                    {field: 'subMerchantOnlineCount', title: '客服个数(个)', width: '8%', align: 'center', sortable: false},
                    {field: 'shopName', title: '店铺名称', width: '11%', align: 'center', sortable: false},
                    {field:'mainCategoryName',title:'主营经营大类', width:'8%',align:'center',sortable:false},
                    {field: 'sumLandingTime', title: '累计登录总时长（时）', width: '8%', align: 'center', sortable: false},
                    {field: 'askingCount', title: '咨询量(人)', width: '8%', align: 'center', sortable: false},
                    {field: 'replyCount', title: '接待量(人)', width: '8%', align: 'center', sortable: false},
                    {field: 'askingTimes', title: '咨询次数', width: '8%', align: 'center', sortable: false},
                    {field: 'replyTimes', title: '接待次数', width: '8%', align: 'center', sortable: false},
                    {field: 'replyFrequency', title: '回复率', width: '8%', align: 'center', sortable: false},

                    {field: 'avgResponseDuration', title: '平均首次回复响应时间（秒）', width: '8%', align: 'center', sortable: false},
                    {field: 'responseIn90sRate', title: '90s在线响应率', width: '8%', align: 'center', sortable: false},
                    {field: 'merchantStatus', title: '店铺显示状态', width: '8%', align: 'center', sortable: false}
                ]]
            });

            var pagination = $('#sellerWorkTab').datagrid('getPager');
            pagination.pagination({
                onSelectPage: function (pageNumber, pageSize) {
                    self._params.pageSize = pageSize;
                    self._params.currentPageNo = pageNumber;

                    if (self.checkParams()) {
                        self.searchWorkMessageList(function (pager) {
                        });
                    }
                }
            })


        var currentHref = window.location.href;
        var currentDomain = currentHref.replace(new RegExp('^https?://([^/]+)/.+$'), '$1');

        if (currentDomain.indexOf('beta.fn') > -1) {
            this._emotionPath = 'http://webim.beta1.fn/webim/';
        } else if (currentDomain.indexOf('idc1.fn') > -1) {
            this._emotionPath = 'http://merchant.feiniu.com/webim/';
        }
    },

    initUI: function () {
        /**设置表格高度**/
        var docHeight = $(document).height();
        var searchBarHeight = $('.searchbar').outerHeight(true);
        var conditionHeight = $('.cla_nav').outerHeight(true);
        var panelHeaderHeight = $('#content>.title_nav').outerHeight(true);
        var self = this;

        $('#sellerWorkTab').height(docHeight - searchBarHeight - conditionHeight - panelHeaderHeight - 50);
        $('#startTime').click(function () {
            WdatePicker({dateFmt: 'yyyy-MM-dd ', maxDate: '%y-%M-%d'});
        });

        $('#endTime').click(function () {
            WdatePicker({dateFmt: 'yyyy-MM-dd ',  maxDate: '%y-%M-%d'});
        });

        $('#startOnlineTime').on('keyup',function(){
            checkNumber(this);
        });

        $('#endOnlineTime').on('keyup',function(){
            checkNumber(this);
        });
        $('#responsein90secondsRateStart').on('keyup',function(){
            checkNumberlessthan100(this);
        });
        $('#responsein90secondsRateEnd').on('keyup',function(){
            checkNumber2lessthan100(this);
        });
        $('#sellerID').on('keyup',function(){
            checkNumber(this);
            if(this.value.length > 6)
            {
                this.value = this.value.substr(0,6);
            }
        });


    },

    setDefaultTime:function(beginTime,endTime,Type){
        var mydate = new Date();
        var time = generalUtil.formateOnlyTime(mydate);
        //set default end Time :yesterday
        mydate.setDate(mydate.getDate() - endTime);
        var date = generalUtil.formateDate(mydate);
        if(Type == 0)
            $('#endAttendanceTime').val(date);
        else
            $('#endTime').val(date);
        //set default start Time:six day before
        mydate.setDate(mydate.getDate() - beginTime);
        var date = generalUtil.formateDate(mydate);
        if(Type == 0)
            $('#startAttendanceTime').val(date);
        else
            $('#startTime').val(date);
    },

    bindUI: function () {
        var self = this;
        var sellerIDLabel = $('#sellerIDLabel');
        var sellerID = $('#sellerID');

        sellerID.on('keyup', function () {
            if (sellerID.val() != '') {
                sellerIDLabel.addClass('hide-placeholder');
            } else {
                sellerIDLabel.removeClass('hide-placeholder');
            }
        });
        /***
         * 搜索按钮
         */
        $('#search').click(function () {
            $('#workLoadYesterdayButton').removeClass("time-btn-select");
            $('#workLoadLastWeekButton').removeClass("time-btn-select");
            $('#workLoadLastMonthButton').removeClass("time-btn-select");
            if (!self.checkParams()) {
                return;
            }
            var opts = $('#sellerWorkTab').datagrid('options');
            var pagination = $('#sellerWorkTab').datagrid('getPager');
            pagination.pagination('refresh',{
                pageNumber:1,
                pageSize:opts.pageSize
            });

            self.searchWorkMessageList(function (pager) {

            })
        });

        /***
         * 重置按钮
         */
        $('#resetCondition').click(function () {
            $('#sellerID,  #startTime, #endTime,#startOnlineTime,#endOnlineTime,#responsein90secondsRateStart,#responsein90secondsRateEnd').val('');
            $('#storeName').combo('clear');
            $('#sellerName').combo('clear');
        });

        /**
         * 清空按钮
         */
        $('#clearTime').click(function () {
            $('#startTime, #endTime').val('');
        });

        $('#clearOnlineTime').click(function () {
            $('#startOnlineTime, #endOnlineTime').val('');
        });

        $('#clearreplayTime').click(function () {
            $('#responsein90secondsRateEnd, #responsein90secondsRateStart').val('');
        });
        /** 昨天按钮 ...**/
        $('#workLoadYesterdayButton').click(function(){
            if($(this).hasClass("time-btn-unselect")){
                $(this).addClass("time-btn-select");
                //$(this).removeClass("time-btn-unselect");
                $('#workLoadLastWeekButton').removeClass("time-btn-select");
                $('#workLoadLastMonthButton').removeClass("time-btn-select");
            }
            self.setDefaultTime(0,1,1);
            //查询
            if (!self.checkParams()) {
                return;
            }
            var opts = $('#sellerWorkTab').datagrid('options');
            var pagination = $('#sellerWorkTab').datagrid('getPager');
            pagination.pagination('refresh',{
                pageNumber:1,
                pageSize:opts.pageSize
            });

            self.searchWorkMessageList(function (pager) {

            })
        });
        /** 上一周按钮 ...**/
        $('#workLoadLastWeekButton').click(function(){
            if($(this).hasClass("time-btn-unselect")){
                $(this).addClass("time-btn-select");
                //$(this).removeClass("time-btn-unselect");
                $('#workLoadYesterdayButton').removeClass("time-btn-select");
                $('#workLoadLastMonthButton').removeClass("time-btn-select");
            }
            self.setDefaultTime(6,1,1);
            //查询
            if (!self.checkParams()) {
                return;
            }
            var opts = $('#sellerWorkTab').datagrid('options');
            var pagination = $('#sellerWorkTab').datagrid('getPager');
            pagination.pagination('refresh',{
                pageNumber:1,
                pageSize:opts.pageSize
            });

            self.searchWorkMessageList(function (pager) {

            })
        });
        /** 上个月按钮 ...**/
        $('#workLoadLastMonthButton').click(function(){
            if($(this).hasClass("time-btn-unselect")){
                $(this).addClass("time-btn-select");
                //$(this).removeClass("time-btn-unselect");
                $('#workLoadLastWeekButton').removeClass("time-btn-select");
                $('#workLoadYesterdayButton').removeClass("time-btn-select");
            }
            self.setDefaultTime(30,1,1);
            //查询
            if (!self.checkParams()) {
                return;
            }
            var opts = $('#sellerWorkTab').datagrid('options');
            var pagination = $('#sellerWorkTab').datagrid('getPager');
            pagination.pagination('refresh',{
                pageNumber:1,
                pageSize:opts.pageSize
            });

            self.searchWorkMessageList(function (pager) {

            })
        });

        /**店铺名称自动提示**/
        $('#storeName').combobox({
            valueField: 'merchantID',
            textField: 'storeName',
            mode: 'remote',
            loader: function (param, success) {
                if (! param.q) {
                    success([]);
                }else{
                    self.searchStoreNameCompleteList(param.q || '', function (data) {
                        success(data);
                    });
                }
            }
        });
        /**商家名称自动提示**/
        $('#sellerName').combobox({
            valueField: 'id',
            textField: 'name',
            mode: 'remote',
            loader: function (param, success) {
                if (! param.q) {
                    success([]);
                }else{
                    self.searchSellerNameCompleteList(param.q || '', function (data) {
                        success(data.merchantInfoList);
                    });
                }
            }
        });

        var datagridHeader = document.getElementsByClassName("datagrid-header-row");
        var headItems = datagridHeader[0].childNodes;
        for(var i = headItems.length - 1; i >= 0; i--) {
            headItems[i].setAttribute("title",tipsTitle[i]);
        }

        /** 默认昨天数据**/
        if (!self.checkParams()) {
            return;
        }
        var opts = $('#sellerWorkTab').datagrid('options');
        var pagination = $('#sellerWorkTab').datagrid('getPager');
        pagination.pagination('refresh',{
            pageNumber:1,
            pageSize:opts.pageSize
        });

        self.searchWorkMessageList(function (pager) {

        })

        /**
         * 手动发送站内信相关
         */
        $('#sendWarning').click(function(){
            $('#sendWarningDialog').window('center');
            $('#sendWarningDialog').dialog('open');
        });
        $('#moderateWarning').click(function(){
            $('#dialogContent').html("亲爱的商家您好：您被系统检测到近几日多次飞牛哞哞在线时长低于六小时，" +
            "即将被处罚，请您注意登录。如有疑问请及时和类目反馈。谢谢! ");
        });
        $('#seriousWarning').click(function(){
            $('#dialogContent').html("亲爱的商家您好：您上周飞牛哞哞在线时长每天均低于六小时，" +
            "已对您进行处罚，请您注意登录。如有疑问请及时和类目反馈。谢谢! ");
        });
        $("#sendInnerMail").click(function(){
            var url = "../monitor/send-Inner-Mail";
            var infoTitle = "哞哞在线时长不达标警告";
            var messageContent = $('#dialogContent').html();
            self.sendInnerMail(url , infoTitle , messageContent);
        });
        $("#cancelSend").click(function(){
            $("#sendWarningDialog").window("close");
        });

    },
    checkParams: function () {
        var isValid = true;

        if (isValid) {
            if ($('#startTime').val() == "" && $('#endTime').val() == "" ) {
                isValid = false;
                showInfo('请设置统计时间');
            }
        }

        $('.searchbar .easyui-validatebox, .searchbar .dateSettingTxt').each(function () {
            var $this = $(this);
            var isValid0 = false;

            if ($this.hasClass('easyui-validatebox')) {
                isValid0 = $this.validatebox('isValid');
            }else if ($this.hasClass('easyui-combobox')) {
                isValid0 = !!$this.combobox('getValue');
            } else {
                isValid0 = ($this.val() != '');
            }


            if (!isValid0 && !!isValid) {
                if ($this.hasClass('icon_calendar')) {
                    showInfo('请选择' + $this.data('name'));
                } else {
                    showInfo('请输入' + $this.data('name'));
                }

                isValid = false;
            }
        });


        if (isValid) {
            if ($('#startTime').val() == "" || $('#endTime').val() == "" ) {
                isValid = false;
                showInfo('需要设置统计时间范围');
            }
        }

        if (isValid) {
            if ( parseInt(generalUtil.getUTCtime($('#startTime').val() + " 00:00:00") / 1000) >=  parseInt(generalUtil.getUTCtime($('#endTime').val()+ " 23:59:59") / 1000 + 1)) {
                isValid = false;
                showInfo('止时间不能早于起时间');
            }
        }

        if (isValid) {
            if (parseInt($('#startOnlineTime').val()) >= parseInt($('#endOnlineTime').val())) {
                isValid = false;
                showInfo('累计登录时长设置不正确');
            }
        }
        if (isValid) {
            if (parseFloat($('#responsein90secondsRateStart').val()) >= parseFloat($('#responsein90secondsRateEnd').val())) {
                isValid = false;
                showInfo('90秒在线响应率设置不正确');
            }
        }

        return isValid;
    },
    getParams: function () {
        var  responsein90secondsRateStart= $('#responsein90secondsRateStart').val() == "" ? -1 :parseFloat($('#responsein90secondsRateStart').val());
        var  responsein90secondsRateEnd= $('#responsein90secondsRateEnd').val() == "" ? -1 :parseFloat($('#responsein90secondsRateEnd').val());


        var  OnlineTimestart= $('#startOnlineTime').val() == ""||$('#startOnlineTime').val() == "0" || $('#startOnlineTime').val() == "0.0"
            ? -1 :parseFloat($('#startOnlineTime').val());

        var  OnlineTimeend= $('#endOnlineTime').val() == "" ? -1 :parseFloat($('#endOnlineTime').val());
        return $.extend({}, {
            sellerID: $('#sellerID').val(),
            storeName: $('#storeName').combo('getText'),
            startTime: parseInt(generalUtil.getUTCtime($('#startTime').val() + " 00:00:00") / 1000),
            endTime: parseInt(generalUtil.getUTCtime($('#endTime').val()+ " 23:59:59") / 1000 + 1),
            sellerName:$('#sellerName').combo('getText'),
            startOnlineTime: OnlineTimestart,
            endOnlineTime: OnlineTimeend,
            startResponseIn90sRate: responsein90secondsRateStart,
            endResponseIn90sRate: responsein90secondsRateEnd
        }, this._params);
    },
    searchStoreNameCompleteList: function (value, callback) {
        $.post(
            '../monitor/get-store-info-pair',
            {
                storeNamePart: value
            },
            function (resultInfo) {
                if (resultInfo && !!resultInfo.success) {
                    callback.call(null, resultInfo.data.storeInfoPairList);
                } else {
                    showInfo((resultInfo || {}).message || '服务器发生故障，请稍后再试');
                }
            }
        )
    },
    /**发送站内信**/
    sendInnerMail: function(url , infoTitle , messageContent){
       $.post(
           url,
           {
               infoTitle :infoTitle,
               messageContent : messageContent
           },
           function(resultInfo){
               if(resultInfo && !!resultInfo.success){
                   alert("innerMail...");
               }else{
                   showInfo((resultInfo || {}).message || '服务器发生故障，请稍后再试');
               }
           }

       )
    },

    searchSellerNameCompleteList: function (value, callback) {
        $.post(
            '../monitor/get-merchant-info',
            {
                MerchantName: value
            },
            function (resultInfo) {
                if (resultInfo && !!resultInfo.success) {
                    callback.call(null, resultInfo.data);
                } else {
                    showInfo((resultInfo || {}).message || '服务器发生故障，请稍后再试');
                }
            }
        )
    },
    searchWorkMessageList: function (callback) {
        var self = this;
        var params = self.getParams();
        self._params.currentPageNo = 1;

        MaskUtil.mask("正在处理请稍后");
        $.post('../monitor/get-work-message-list',
            params,
            function (resultInfo) {
                MaskUtil.unmask();
                if (resultInfo && !!resultInfo.success) {
                    var data = resultInfo.data.pager.resultsForCurrentPage;
                    if (data == null){
                        $('#sellerWorkTab').datagrid('loadData', {
                            total: 0,
                            rows: []
                        });
                    }
                    $('#sellerWorkTab').datagrid('loadData', {
                        total: resultInfo.data.pager.totalRecordSize,
                        rows: data
                    });
                } else {
                    if(resultInfo.errorCode == -2 ){
                        var list=[];
                        $('#sellerWorkTab').datagrid({
                            data:list,
                            view:myview,
                            emptyMsg:'未查询到数据'
                        });
                    }
                    else
                        showInfo((resultInfo || {}).message || '服务器发生故障，请稍后再试');
                }
            },
            'json');
    },

    renderTable: function (rowData) {
        $('#sellerWorkTab').datagrid('loadData', rowData);
        $('#sellerWorkTab').datagrid('getPanel').find('.easyui-panel').each(function () {
            var $this = $(this);
            var title = $this.prop('title');

            $this.panel({
                title: title,
                border: false
            });
        }).closest('.datagrid-body').unbind();

        $('#sellerWorkTab').datagrid('getPanel').find('.fn-fancy-item').fancybox({
            'transitionIn': 'elastic',
            'transitionOut': 'elastic',
            'speedIn': 600,
            'speedOut': 200,
            'overlayShow': false
        });
    }
};

$(function () {
    workStatistics.init();
});
