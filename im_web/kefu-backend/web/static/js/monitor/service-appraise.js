/**
 * Created by wangchen on 2015/2/12.
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
function doSearch(pageNo,callback){
    var self = serviceAppraise;

    self._params.currentPageNo = pageNo;

    if(!self.checkParams()){
        return null;
    }

    var params = self.getParams();

    if ( params == null){
        return;
    }

    $.post('../service-appraise/queryDetail',
        params,
        function (resultInfo) {
            if (resultInfo && !!resultInfo.success) {
                var pager = resultInfo.data.serviceAppraiseDetailStat;
                callback.call(this,pager);
            } else {
                showInfo((resultInfo || {}).message || '服务器发生故障');
            }
        },
        'json');
};

function doSearchHistoryChat(pageNo,callback){
    var self = serviceAppraise;
    self._params.historyChatCurrentPageNo = pageNo;

    if(!self.checkParams()){
        return null;
    }

    var params = self.getParams();

    if ( params == null){
        return;
    }


    $.post('../service-appraise/get-history-chat-list-by-sessionId',
        params,
        function (resultInfo) {
            if (resultInfo && !!resultInfo.success) {
                var pager = resultInfo.data.pager;
                callback.call(this,pager);
            } else {
                showInfo((resultInfo || {}).message || '服务器发生故障');
            }
        },
        'json');
}

var styleTemplate=
    'class="look" style="cursor: pointer; color: #3366FF"';

var appraiseInfoTemplate =
    '<tr submerchantID="$subMerchantId" subMerchantName="$subMerchantName" itemCount="$itemCount" >' +
    '<td>$subMerchantName </td>' +
    '<td type="0" $style0>$appraiseCount0</td>' +
    '<td type="5" $style5>$appraiseCount5</td>' +
    '<td type="4" $style4>$appraiseCount4</td>' +
    '<td type="3" $style3>$appraiseCount3</td>' +
    '<td type="2" $style2>$appraiseCount2</td>' +
    '<td type="1" $style1>$appraiseCount1</td>' +
    '<td>$goodRate</td>' +
    '</tr>';

var appraiseDetailInfoTemplate =
    '<tr>' +
    '<td>$merchantName</td>' +
    '<td>$buyerName</td>' +
    '<td>$appraise</td>' +
    '<td>$commetDetail</td>' +
    '<td>$commetTime</td>' +
    '<td>$sessionStartTime</td>' +
    '<td>$sessionEndTime</td>' +
    '<td><a class="chat-history-look" sessionId="$sessionId" >会话明细</td>' +
    '</tr>';

function doExportExcel(isDetailExport){
    var self = serviceAppraise;

    var params ="";
    if(isDetailExport)
        params= "../service-appraise/get-detail-excel-file?";
    else
        params = "../service-appraise/get-excel-file?"

    if(!self.checkParams()){
        return null;
    }

    var param = self.getParams();

    if(!isDetailExport){
        var i=0;
        param.subAccountIDListStr = "";
        for(;i<self.dataManager.subMerchantInfoList.length ; i++){
            param.subAccountIDListStr += self.dataManager.subMerchantInfoList[i].memberId +";"
        }
    }

    if ( param == null){
        alert('开始时间不能大于结束时间');
        return;
    }

    params = params + "&startTime="+param.startTime +
        "&endTime="+param.endTime +
        "&pageSize="+param.pageSize +
        "&subMerchantID="+param.subMerchantID +
        "&currentPageNo="+param.currentPageNo +
        "&subAccountIDListStr="+param.subAccountIDListStr +
        "&currentItemCount="+param.currentItemCount+
        "&currentPageAccount="+param.currentPageAccount+
        "&searchType="+param.searchType;

    window.location.href = params;
};

var serviceAppraise ={
    _emotionPath: 'http://merchant.feiniu.com/webim/',
    dataManager : {
        subMerchantIDList: [],
        subMerchantName: '',
        subMerchantInfoList:[]
    },
    _params: {
        startTime: 0,
        endTime: 0,
        searchType:-1,
        subMerchantId: "",
        pageSizeTop:10,
        currentPageNoTop:1,
        currentItemCountTop :0,
        currentPageAccountTop:0,
        pageSize:10,
        currentPageNo:0,
        currentItemCount :0,
        currentPageAccount:0,
        historyChatPageSize: 10,
        historyChatCurrentPageNo: 1,
        historySessionId:0,
        subAccountIDListStr:""
    },

    updateUIbyParamsTop:function(){
        var self = this;
        if(self._params.currentItemCountTop  == 0){
            dwqPager.generPageHtml({
                pagerid : 'dwqPager',//
                pno : 0,//当前页码
                //总页码
                total : 0,
                //总数据条数
                totalRecords :0,
                mode : 'click',//默认值是link，可选link或者click
                pageSize:self._params.pageSizeTop,
                click : function(n){
                    self._params.pageSizeTop = this.pageSize;
                    self._params.currentPageNoTop = n;
                    self.queryAppraiseInfo();
                    //this.selectPage(n);
                }
            },true);
        } else{
            var pageCount = parseInt(self._params.currentItemCountTop/self._params.pageSizeTop);
            if(self._params.currentItemCountTop % self._params.pageSizeTop != 0) pageCount= pageCount +1;
            self._params.currentPageAccountTop = pageCount;
            dwqPager.generPageHtml({
                pagerid : 'dwqPager',//
                pno : self._params.currentPageNoTop,//当前页码
                //总页码
                total : self._params.currentPageAccountTop,
                //总数据条数
                totalRecords : self._params.currentItemCountTop,
                pageSize:self._params.pageSizeTop,
                mode : 'click',//默认值是link，可选link或者click
                click : function(n){
                    self._params.pageSizeTop = this.pageSize;
                    self._params.currentPageNoTop = n;
                    self.queryAppraiseInfo();
                    //this.selectPage(n);
                }
            },true);
        }
    },

    updateUIbyParams:function(){
        var self = this;
        if(self._params.currentItemCount == 0){
            dwqPager.generPageHtml({
                pagerid : 'dwqPager',//
                pno : 0,//当前页码
                //总页码
                total : 0,
                //总数据条数
                totalRecords :0,
                mode : 'click',//默认值是link，可选link或者click
                pageSize:self._params.pageSize,
                click : function(n){
                    self._params.pageSize = this.pageSize;
                    self._params.currentPageNo = n;
                    self.searchDetail(n);
                    //this.selectPage(n);
                }
            },true);
        } else{
            var pageCount = parseInt(self._params.currentItemCount/self._params.pageSize);
            if(self._params.currentItemCount % self._params.pageSize != 0) pageCount= pageCount +1;
            self._params.currentPageAccount = pageCount;
            dwqPager.generPageHtml({
                pagerid : 'dwqPager',//
                pno : self._params.currentPageNo,//当前页码
                //总页码
                total : self._params.currentPageAccount,
                //总数据条数
                totalRecords : self._params.currentItemCount,
                pageSize:self._params.pageSize,
                mode : 'click',//默认值是link，可选link或者click
                click : function(n){
                    self._params.pageSize = this.pageSize;
                    self._params.currentPageNo = n;
                    self.searchDetail(n);
                    //this.selectPage(n);
                }
            },true);
        }
    },
    init: function () {
        this.initData();
        this.initUI();
        this.bindUI();
        this.queryAppraiseInfo();
    },
    initData: function () {
        var self = this;
        $('#subMerchantList dd ul li').each(function () {
            var $this = $(this);
            if ($this.data('submerchantid')) {
                self.dataManager.subMerchantIDList.push($this.data('submerchantid'));
            }
        });

        var currentHref = window.location.href;
        var currentDomain = currentHref.replace(new RegExp('^https?://([^/]+)/.+$'), '$1');

        if (currentDomain.indexOf('beta.fn') > -1) {
            this._emotionPath = 'http://webim.beta1.fn/webim/';
        } else if (currentDomain.indexOf('idc1.fn') > -1) {
            this._emotionPath = 'http://merchant.feiniu.com/webim/';
        }
    },

    initUI: function () {
        var self = this;
        $tempdata = $('tmpdata');

        self.dataManager.subMerchantInfoList = $tempdata.data('submerchantinfolist') || [];
        var serviceAppraiseList = $tempdata.data('serviceappraiselist') || [];
        this.paintAppraiseInfo(serviceAppraiseList);

        var mydate = new Date();
        var startdate = generalUtil.formateDate(mydate);
        $('#endTime').val(startdate);

        mydate.setMonth(mydate.getMonth() -3);
        var date = generalUtil.formateDate(mydate);
        $('#startTime').val(date);

        self.updateUIToParams();
        $tempdata.remove();

        var docHeight = $(document).height();
        var searchBarHeight = $('.searchbar').outerHeight(true);
        var panelHeaderHeight = $('#content>.title_nav').outerHeight(true);

        var searchBarWidth = $('.serverTable').width();

        $('#chatMessageTable').height(docHeight - searchBarHeight  - panelHeaderHeight - 50);

        $('#chatMessageTable').width(searchBarWidth);
    },

    updateUIToParams:function(){
        var self = this;
        self._params.startTime = generalUtil.parseDate($('#startTime').val()).getTime() / 1000;
        self._params.endTime = generalUtil.parseDate($('#endTime').val()).getTime() / 1000 + 24 * 3600;
        if( self._params.startTime > self._params.endTime){
            alert('开始时间不能大于结束时间');
            return -1;
        }

        var moumouName = $('#moumouName').combobox('getText');
        var moumouId = $('#moumouName').combobox('getValue');

        if (moumouName != ""){
            if (moumouName == moumouId){
                alert('没找到指定的客服账号');
                return -2;
            }
            self._params.subAccountIDListStr =moumouId.toString();
            self._params.currentItemCountTop = 1;
            self._params.currentPageAccountTop = 1;
            return 0;
        }

        var i=(self._params.currentPageNoTop-1) * self._params.pageSizeTop;
        var index =1;
        self._params.subAccountIDListStr ="";
        for(;i<self.dataManager.subMerchantInfoList.length ; i++){
            if(index > self._params.pageSizeTop)
                break;
            self._params.subAccountIDListStr += self.dataManager.subMerchantInfoList[i].memberId +";"
            index++;
        }

        self._params.currentItemCountTop = self.dataManager.subMerchantInfoList.length;
        var pageCount = parseInt(self._params.currentItemCountTop/self._params.pageSizeTop);
        if(self._params.currentItemCountTop % self._params.pageSizeTop != 0) pageCount= pageCount +1;
        self._params.currentPageAccountTop = pageCount;


        return 0;
    },

    searchDetail:function(pageNo){
        var self = this;
        $('#serverTablePage').hide();
        $('#main_warp').hide();
        $('#serverTableDetailPage').show();

        doSearch(pageNo,function(pager){
            var containerDetailPage = $('#containerServerTableDetailPage');
            containerDetailPage.empty();

            var commentList = pager;

            if (!!commentList) {
                var toAppend = '';

                for (var i = 0; i < commentList.length; i++) {
                    var currentDetailInfo = commentList[i];
                    var merchantName = self.dataManager.subMerchantName;
                    var buyerName = currentDetailInfo.buyerName;
                    var appraise = currentDetailInfo.appraiseStr;
                    var commetDetail = currentDetailInfo.comment;
                    var commetTime = currentDetailInfo.appraiseTimeStr;
                    var sessionStartTime = currentDetailInfo.sessionStartTimeStr;
                    var sessionEndTime = currentDetailInfo.sessionEndTimeStr;
                    var sessionId = currentDetailInfo.sessionId;

                    toAppend +=
                        appraiseDetailInfoTemplate.replace(/\$merchantName/g,merchantName.toString())
                            .replace(/\$buyerName/g,buyerName.toString())
                            .replace(/\$appraise/g,appraise.toString())
                            .replace(/\$commetDetail/g,commetDetail.toString())
                            .replace(/\$commetTime/g,commetTime.toString())
                            .replace(/\$sessionStartTime/g,sessionStartTime.toString())
                            .replace(/\$sessionEndTime/g,sessionEndTime.toString())
                            .replace(/\$sessionId/g,sessionId.toString());
                }
                containerDetailPage.append(toAppend);
                self.updateUIbyParams();
            }
        });
    },

    searchHistoryChat:function(pageNo){
        var self = this;
        $('#serverTablePage').hide();
        $('#main_warp').show();
        $('#serverTableDetailPage').hide();

        doSearchHistoryChat(pageNo,function(pager){
            var rowData = transformResultToDatagridData(pager,self._emotionPath);
            self.renderTable(rowData);
        })
    },

    queryAppraiseInfo:function(){
        var self = this;
        if(!self.checkParams()){
            return null;
        }

        var params = self.getParamsTop();

        if ( params == null){
            return;
        }

        self.retrieveSubMerchantAppraiseInfo(params, function (serviceAppraiseList) {
            self.paintAppraiseInfo(serviceAppraiseList || []);
            self.updateUIbyParamsTop();
        });
    },
    bindUI: function () {
        var self = this;

        $('#moumouName').combobox({
            valueField: 'memberId',
            textField: 'username',
            mode: 'remote',
            loader: function (param, success) {
                var resultArray = new  Array();
                if (! param.q) {
                    success([]);
                }else{
                    for(var i = 0 ; i < self.dataManager.subMerchantInfoList.length;i++){
                        var subMerchantInfo = self.dataManager.subMerchantInfoList[i];
                        if (subMerchantInfo.username.indexOf(param.q) >=0 ){
                            resultArray.push(self.dataManager.subMerchantInfoList[i]);
                        }
                    }
                }
                success(resultArray);
            }
        });
        $(document).on('click',".look",function() {
            if (!self.checkParams()) {
                return;
            }
            self._params.subMerchantId =  $(this.parentNode).attr("subMerchantID");
            self._params.currentPageNo = 1;
            self._params.pageSize = 10;

            var totalCount = $(this).text();
            self._params.searchType = $(this).attr("type");

            self._params.currentItemCount = totalCount;
            var pageCount = parseInt(self._params.currentItemCount/self._params.pageSize);
            if(self._params.currentItemCount % self._params.pageSize != 0) pageCount= pageCount +1;
            self._params.currentPageAccount = pageCount;

            self.dataManager.subMerchantName = $(this.parentNode).attr("subMerchantName");
            self.searchDetail(1);
        });

        $(document).on('click',".chat-history-look",function() {
            $('#dwqPager').hide();
            self._params.historySessionId=  $(this).attr("sessionid");
            self._params.historyChatPageSize = 10;
            self._params.historyChatCurrentPageNo = 1;

            self.searchHistoryChat(1);
        });

        $('#startTime').click(function () {
            WdatePicker({dateFmt: 'yyyy-MM-dd', minDate: '%y-#{%M-3}-%d', maxDate: '%y-%M-#\{%d\}'});
        });

        $('#endTime').click(function () {
            WdatePicker({dateFmt: 'yyyy-MM-dd', minDate: '%y-#{%M-3}-%d', maxDate: '%y-%M-#\{%d\}'});
        });

        $('#clearTime').click(function () {
            $('#startTime').val("");
            $('#endTime').val("");
        });

        $('#resetCondition').click(function () {
            $('#startTime').val("");
            $('#endTime').val("");
            var $this = $(this);
            $('#subMerchantList dt').data('submerchantid', '-1').html("所有客服");
            $('#moumouName').combo('clear');
        });


        $('#detailBack').click(function () {
            $('#serverTablePage').show();
            $('#serverTableDetailPage').hide();
            $('#main_warp').hide();
            self.updateUIbyParamsTop();
        });

        $('#chatHistoryBack').click(function () {
            $('#serverTablePage').hide();
            $('#serverTableDetailPage').show();
            $('#main_warp').hide();
            $('#dwqPager').show();
        });

        $('#queryAppraiseInfo').click(function () {
            self._params.currentPageNoTop = 1;
            self.queryAppraiseInfo();
        });

        $(document).on('click', '#subMerchantList dd>ul>li', function () {
            var $this = $(this);

            $('#subMerchantList dt').data('submerchantid', $this.data('submerchantid') || '-1').html($this.text());
        });


        $('#detailExcelExport').click(function() {
            doExportExcel(true);
        });

        $('#excelExport').click(function() {
            doExportExcel(false);
        });

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
    },
    checkParams: function () {
        if (!$('#startTime').val()) {
            alert('请选择开始时间');
            return false;
        }
        if (!$('#endTime').val()) {
            alert('请选择结束时间');
            return false;
        }

        return true;
    },
    paintAppraiseInfo: function (serviceAppraiseList) {
        var containerPage = $('#serviceAppraiseInfoPanel');
        containerPage.empty();

        var commentList = serviceAppraiseList;

        if (!!commentList) {
            var toAppend = '';

            for (var i = 0; i < commentList.length; i++) {
                var currentDetailInfo = commentList[i];
                var subMerchantId = currentDetailInfo.subMerchantId;
                var subMerchantName = currentDetailInfo.subMerchantName;
                var appraiseCount = currentDetailInfo.appraiseCount;
                var goodRate = currentDetailInfo.goodRate;
                var itemCount = parseInt(appraiseCount[0].toString())+
                        parseInt(appraiseCount[1].toString())+
                        parseInt(appraiseCount[2].toString())+
                        parseInt(appraiseCount[3].toString())+
                        parseInt(appraiseCount[4].toString())+
                        parseInt(appraiseCount[5].toString());

                var toAppendItem =
                    appraiseInfoTemplate.replace(/\$subMerchantName/g, subMerchantName.toString())
                        .replace(/\$goodRate/g, goodRate.toString())
                        .replace(/\$subMerchantId/g,subMerchantId.toString())
                        .replace(/\$itemCount/g,itemCount.toString());

                for(var j=0;j<6;j++){
                    var appraiseCountItem=new RegExp("\\\$appraiseCount"+j.toString(),"g");
                    var styleItem = new RegExp("\\\$style"+j.toString(),"g");
                    if(appraiseCount[j].toString()!="0" && j != 0){
                        toAppendItem = toAppendItem.replace(styleItem,styleTemplate);
                    }else{
                        toAppendItem = toAppendItem.replace(styleItem," ");
                    }
                    toAppendItem = toAppendItem.replace(appraiseCountItem,appraiseCount[j].toString());
                }

                toAppend += toAppendItem;
            }
            containerPage.append(toAppend);
        }

    },

    retrieveSubMerchantAppraiseInfo: function (param, successCallback) {
        var self = this;

        $.post(
            '../service-appraise/query',
            param,
            function (result) {
                if (!result || !result.success) {
                    alert(result && result.message || '服务器发生故障');
                } else {
                    successCallback.call(self, result.data.serviceAppraiseStat);
                }
            }
        )
    },

    getParamsTop: function () {
        var  self = this;

        if ( self.updateUIToParams() != 0){
            return null;
        }

        return $.extend({}, {
            startTime: self._params.startTime,
            endTime:self._params.endTime,
            pageSize:self._params.pageSizeTop,
            currentPageNo:self._params.currentPageNoTop,
            subMerchantID:self._params.subMerchantId,
            historyChatPageSize: self._params.historyChatPageSize,
            historyChatCurrentPageNo: self._params.historyChatCurrentPageNo,
            historySessionId:self._params.historySessionId,
            subAccountIDListStr:self._params.subAccountIDListStr,
            searchType:self._params.searchType
        });
    },

    getParams: function () {
        var  self = this;

        if ( self.updateUIToParams() != 0){
            return null;
        }

        return $.extend({}, {
            startTime: self._params.startTime,
            endTime:self._params.endTime,
            pageSize:self._params.pageSize,
            currentPageNo:self._params.currentPageNo,
            subMerchantID:self._params.subMerchantId,
            historyChatPageSize: self._params.historyChatPageSize,
            historyChatCurrentPageNo: self._params.historyChatCurrentPageNo,
            historySessionId:self._params.historySessionId,
            subAccountIDListStr:self._params.subAccountIDListStr,
            searchType:self._params.searchType
        });
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
    }
};

$(function () {
    serviceAppraise.init();
})