/**
 * Created by wangzhen on 2015/7/27.
 */
function showInfo(content) {
    alert(content);
}

var MaskUtil = (function(){

    var $mask,$maskMsg;

    var defMsg = '正在查询';

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
var attendanceInfoTemplate =
        '<tr>' +
        '<td>$subMerchantName</td>' +
        '<td>$attendanceDayCount</td>' +
        '<td>$attendanceTimeSum</td>' +
        '<td>$attendanceTimeAvg</td>' +
        '<td>$attendanceLeaveNum</td>' +
        '<td>$attendanceLeaveDuration</td>' +
        '<td>$attendanceStartTimeAvg</td>' +
        '<td>$attendanceEndTimeAvg</td>' +
        '<td>$attendanceEarliestTime</td>' +
        '<td>$attendanceLatestTime</td>' +
        '<td><a class="c_bule look" subMerchantID="$subMerchantID" href="#">查看详情</a></td>' +
        '</tr>';

var attendanceDetailInfoTemplate =
    '<tr>' +
    '<td>$date</td>' +
    '<td>$firstOnlineTime</td>' +
    '<td>$lastOnlineTime</td>' +
    '<td>$landingCount</td>' +
    '<td>$landingTimeSum</td>' +
    '<td>$hungUpTimeSum</td>' +
    '<td>$offlineTimeSum</td>' +
    '<td>$leaveNum</td>' +
    '<td>$leaveDuration</td>' +
    '<td>$onlineRate</td>' +
    '</tr>';

var workLoadLookTemplate ='<a class="c_bule d_look" subMerchantID="$subMerchantID" href="#">查看详情</a>';
var workLoadInfoTemplate =
    '<tr>' +
    '<td>$subMerchantName</td>' +
    '<td>$landingTimeSum</td>' +
    '<td>$receptionTimeSum</td>' +
    '<td>$askingCustomerCount</td>' +
    '<td>$responseCustomerCount</td>' +
    '<td>$askingTimes</td>' +
    '<td>$responseTimes</td>' +
    '<td>$responseRate</td>' +
    '<td>$responseMessageCountPerSessionAvg</td>' +
    '<td>$timePerSessionAvg</td>' +
    '<td>$firstResponseTimeAvg</td>' +
    '<td>$lookDetail</td>' +
    '</tr>';


var workLoadDetailInfoTemplate =
    '<tr>' +
    '<td>$date</td>' +
    '<td>$landingTimeSum</td>' +
    '<td>$receptionTimeSum</td>' +
    '<td>$askingCustomerCount</td>' +
    '<td>$responseCustomerCount</td>' +
    '<td>$askingTimes</td>' +
    '<td>$responseTimes</td>' +
    '<td>$responseRate</td>' +
    '<td>$responseMessageCountPerSessionAvg</td>' +
    '<td>$timePerSessionAvg</td>' +
    '<td>$firstResponseTimeAvg</td>' +
    '</tr>';

function doExportExcel(){
    var self = attendanceStatistics;
    var params= "../attendance/get-excel-file.xls?";

    if(!self.checkParams()){
        return null;
    }
    var param = self.getParams();

    params = params + "&startTime="+param.startTime +
        "&endTime="+param.endTime +
        "&pageSize="+param.pageSize +
        "&directorID="+param.directorID +
        "&currentPageNo="+param.currentPageNo +
        "&subMerchantID="+param.subMerchantID +
        "&searchType="+param.searchType ;

    window.location.href = params;
};

function doSearch(pageNo,callback){
    var self = attendanceStatistics;

    if ( self.updateUIToParams() != 0){
        return null;
    }

    self._params.currentPageNo = pageNo;

    if(!self.checkParams()){
        return null;
    }

    var params = self.getParams();
    MaskUtil.mask("正在查询，请稍后");
    $.post('../attendance/getattendancemessagelist',
        params,
        function (resultInfo) {
            MaskUtil.unmask();
            if (resultInfo && !!resultInfo.success) {
                var pager = resultInfo.data.pager;
                var itemCount = resultInfo.data.itemCount;
                self._params.currentItemCount = itemCount;
                callback.call(this,pager);
            } else {
                showInfo((resultInfo || {}).message || '服务器发生故障');
            }
        },
        'json');
};

function searchWithParams(pageNo){
    var self = attendanceStatistics;
    var type = self._params.currrentPageType;
    switch (type){
        case 1:
            doSearch(pageNo,function (pager) {
                var containerAttendancePage = $('#containerAttendanceDetailPage');
                containerAttendancePage.empty();

                var subAccountMappingList = pager;
                if (!!subAccountMappingList) {
                    var toAppend = '';
                    for (var i = 0; i < subAccountMappingList.length; i++) {
                        var currentSubAccountMapping = subAccountMappingList[i];
                        var pdate = currentSubAccountMapping.date;
                        var firstOnlineTime = currentSubAccountMapping.firstOnlineTime;
                        var lastOnlineTime = currentSubAccountMapping.lastOnlineTime;
                        var landingCount = currentSubAccountMapping.landingCount;
                        var landingTimeSum = currentSubAccountMapping.landingTimeSum;
                        var hungUpTimeSum = currentSubAccountMapping.hungUpTimeSum;
                        var offlineTimeSum = currentSubAccountMapping.offlineTimeSum;
                        var onlineRate = currentSubAccountMapping.onlineRate;
                        var leaveNum = currentSubAccountMapping.leaveNum;
                        var leaveDuration = currentSubAccountMapping.leaveDuration;
                        toAppend +=
                            attendanceDetailInfoTemplate.replace(/\$date/g,pdate.toString())
                                .replace(/\$firstOnlineTime/g,firstOnlineTime.toString())
                                .replace(/\$lastOnlineTime/g,lastOnlineTime.toString())
                                .replace(/\$landingCount/g,landingCount.toString())
                                .replace(/\$landingTimeSum/g,landingTimeSum.toString())
                                .replace(/\$hungUpTimeSum/g,hungUpTimeSum.toString())
                                .replace(/\$offlineTimeSum/g,offlineTimeSum.toString())
                                .replace(/\$onlineRate/g,onlineRate.toString())
                                .replace(/\$leaveNum/g,leaveNum.toString())
                                .replace(/\$leaveDuration/g,leaveDuration.toString());
                    }
                    containerAttendancePage.append(toAppend);
                    self.updateUIbyParams();
                }

            });
            break;
        case 3:
            doSearch(pageNo,function (pager) {
                var containerAttendancePage = $('#containerWorkLoadDetailPage');
                containerAttendancePage.empty();

                var subAccountMappingList = pager;
                if (!!subAccountMappingList) {
                    var toAppend = '';
                    for (var i = 0; i < subAccountMappingList.length; i++) {
                        var currentSubAccountMapping = subAccountMappingList[i];
                        var pdate = currentSubAccountMapping.date;
                        var landingTimeSum = currentSubAccountMapping.landingTimeSum;
                        var receptionTimeSum = currentSubAccountMapping.receptionTimeSum;
                        var askingCustomerCount = currentSubAccountMapping.askingCustomerCount;
                        var responseCustomerCount = currentSubAccountMapping.responseCustomerCount;
                        var unresponseCustomerCount = currentSubAccountMapping.unresponseCustomerCount;
                        var responseRate = currentSubAccountMapping.responseRate;
                        var responseMessageCountPerSessionAvg = currentSubAccountMapping.responseMessageCountPerSessionAvg;
                        var timePerSessionAvg = currentSubAccountMapping.timePerSessionAvg;
                        var firstResponseTimeAvg = currentSubAccountMapping.firstResponseTimeAvg;
                        var askingTimes= currentSubAccountMapping.askingTimes;
                        var responseTimes = currentSubAccountMapping.responseTimes;

                        toAppend +=
                            workLoadDetailInfoTemplate.replace(/\$date/g,pdate.toString())
                                .replace(/\$landingTimeSum/g,landingTimeSum.toString())
                                .replace(/\$receptionTimeSum/g,receptionTimeSum.toString())
                                .replace(/\$askingCustomerCount/g,askingCustomerCount.toString())
                                .replace(/\$responseCustomerCount/g,responseCustomerCount.toString())
                                .replace(/\$askingTimes/g,askingTimes.toString())
                                .replace(/\$responseTimes/g,responseTimes.toString())
                                .replace(/\$unresponseCustomerCount/g,unresponseCustomerCount.toString())
                                .replace(/\$responseRate/g,responseRate.toString())
                                .replace(/\$responseMessageCountPerSessionAvg/g,responseMessageCountPerSessionAvg.toString())
                                .replace(/\$timePerSessionAvg/g,timePerSessionAvg.toString())
                                .replace(/\$firstResponseTimeAvg/g,firstResponseTimeAvg.toString());
                    }
                    containerAttendancePage.append(toAppend);
                    self.updateUIbyParams();
                }
            });
            break;
        case 2:
            doSearch(pageNo,function (pager) {
                var containerAttendancePage = $('#containerWorkLoadPage');
                containerAttendancePage.empty();

                var subAccountMappingList = pager;
                if (!!subAccountMappingList) {

                    var toAppend = '';
                    for (var i = 0; i < subAccountMappingList.length; i++) {
                        var currentSubAccountMapping = subAccountMappingList[i];
                        var subMerchantName = currentSubAccountMapping.subMerchantName;
                        var subMerchantGroup = currentSubAccountMapping.subMerchantGroup;
                        var landingTimeSum = currentSubAccountMapping.landingTimeSum;
                        var receptionTimeSum = currentSubAccountMapping.receptionTimeSum;
                        var askingCustomerCount = currentSubAccountMapping.askingCustomerCount;
                        var responseCustomerCount = currentSubAccountMapping.responseCustomerCount;
                        var unresponseCustomerCount = currentSubAccountMapping.unresponseCustomerCount;
                        var responseRate = currentSubAccountMapping.responseRate;
                        var responseMessageCountPerSessionAvg = currentSubAccountMapping.responseMessageCountPerSessionAvg;
                        var timePerSessionAvg = currentSubAccountMapping.timePerSessionAvg;
                        var firstResponseTimeAvg = currentSubAccountMapping.firstResponseTimeAvg;
                        var attendanceSubMerchantID = currentSubAccountMapping.subMerchantId;
                        var askingTimes= currentSubAccountMapping.askingTimes;
                        var responseTimes = currentSubAccountMapping.responseTimes;

                        if(subMerchantName != "汇总" && subMerchantName !="均值") {
                            var lookDetailStr = workLoadLookTemplate.replace(/\$subMerchantID/g, attendanceSubMerchantID.toString());
                            toAppend +=
                                workLoadInfoTemplate.replace(/\$subMerchantName/g, subMerchantName.toString())
                                    .replace(/\$subMerchantGroup/g, subMerchantGroup.toString())
                                    .replace(/\$landingTimeSum/g, landingTimeSum.toString())
                                    .replace(/\$receptionTimeSum/g, receptionTimeSum.toString())
                                    .replace(/\$askingCustomerCount/g, askingCustomerCount.toString())
                                    .replace(/\$responseCustomerCount/g, responseCustomerCount.toString())
                                    .replace(/\$unresponseCustomerCount/g, unresponseCustomerCount.toString())
                                    .replace(/\$askingTimes/g,askingTimes.toString())
                                    .replace(/\$responseTimes/g,responseTimes.toString())
                                    .replace(/\$responseRate/g, responseRate.toString())
                                    .replace(/\$responseMessageCountPerSessionAvg/g, responseMessageCountPerSessionAvg.toString())
                                    .replace(/\$timePerSessionAvg/g, timePerSessionAvg.toString())
                                    .replace(/\$firstResponseTimeAvg/g, firstResponseTimeAvg.toString())
                                    .replace(/\$lookDetail/g,lookDetailStr.toString());
                        }else {
                            toAppend +=
                                workLoadInfoTemplate.replace(/\$subMerchantName/g, subMerchantName.toString())
                                    .replace(/\$subMerchantGroup/g, subMerchantGroup.toString())
                                    .replace(/\$landingTimeSum/g, landingTimeSum.toString())
                                    .replace(/\$receptionTimeSum/g, receptionTimeSum.toString())
                                    .replace(/\$askingCustomerCount/g, askingCustomerCount.toString())
                                    .replace(/\$responseCustomerCount/g, responseCustomerCount.toString())
                                    .replace(/\$unresponseCustomerCount/g, unresponseCustomerCount.toString())
                                    .replace(/\$askingTimes/g,askingTimes.toString())
                                    .replace(/\$responseTimes/g,responseTimes.toString())
                                    .replace(/\$responseRate/g, responseRate.toString())
                                    .replace(/\$responseMessageCountPerSessionAvg/g, responseMessageCountPerSessionAvg.toString())
                                    .replace(/\$timePerSessionAvg/g, timePerSessionAvg.toString())
                                    .replace(/\$firstResponseTimeAvg/g, firstResponseTimeAvg.toString())
                                    .replace(/\$lookDetail/g, "--");
                        }
                    }
                    containerAttendancePage.append(toAppend);
                    self.updateUIbyParams();
                }
            });
            break;
        case 0:
            doSearch(pageNo,function (pager) {
                var containerAttendancePage = $('#containerAttendancePage');
                containerAttendancePage.empty();

                var subAccountMappingList = pager;
                if (!!subAccountMappingList) {
                    var toAppend = '';
                    for (var i = 0; i < subAccountMappingList.length; i++) {
                        var currentSubAccountMapping = subAccountMappingList[i];
                        var subMerchantName = currentSubAccountMapping.subMerchantName;
                        var attendanceDayCount = currentSubAccountMapping.attendanceDayCount;
                        var attendanceTimeSum = currentSubAccountMapping.attendanceTimeSum;
                        var attendanceTimeAvg = currentSubAccountMapping.attendanceTimeAvg;
                        var attendanceStartTimeAvg = currentSubAccountMapping.attendanceStartTimeAvg;
                        var attendanceEndTimeAvg = currentSubAccountMapping.attendanceEndTimeAvg;
                        var attendanceEarliestTime = currentSubAccountMapping.attendanceEarliestTime;
                        var attendanceLatestTime = currentSubAccountMapping.attendanceLatestTime;
                        var attendanceSubMerchantID = currentSubAccountMapping.subMerchantId;
                        var attendanceLeaveDuration = currentSubAccountMapping.attendanceLeaveDuration;
                        var attendanceLeaveNum = currentSubAccountMapping.attendanceLeaveNum;

                        toAppend +=
                            attendanceInfoTemplate.replace(/\$subMerchantName/g,subMerchantName.toString())
                                .replace(/\$attendanceDayCount/g,attendanceDayCount.toString())
                                .replace(/\$attendanceTimeSum/g,attendanceTimeSum.toString())
                                .replace(/\$attendanceTimeAvg/g,attendanceTimeAvg.toString())
                                .replace(/\$attendanceStartTimeAvg/g,attendanceStartTimeAvg.toString())
                                .replace(/\$attendanceEndTimeAvg/g,attendanceEndTimeAvg.toString())
                                .replace(/\$attendanceEarliestTime/g,attendanceEarliestTime.toString())
                                .replace(/\$attendanceLatestTime/g,attendanceLatestTime.toString())
                                .replace(/\$subMerchantID/g,attendanceSubMerchantID.toString())
                                .replace(/\$attendanceLeaveNum/g,attendanceLeaveNum.toString())
                                .replace(/\$attendanceLeaveDuration/g,attendanceLeaveDuration.toString())
                        ;
                    }
                    containerAttendancePage.append(toAppend);
                    self.updateUIbyParams();
                }
            });
            break;
        default :
            break;
    };

};

var attendanceStatistics = {

    dataManager : {
        directorID: -1,
    },

    _params: {
        beginTime:0,
        endTime:0,
        subMerchantId:"",
        searchType:0,
        currrentPageType:0,
        currentPageNo:0,
        pageSize:10,
        currentItemCount:0,
        currentPageAccount:0,
        //0 for attendance page ,1 for attendance  detail page
        //2 for workload page,3 for workload detail page
        pageSizeAttendanceInfo: 10,
        currentPageNoAttendanceInfo: 1,
        currentItemCountAttendanceInfo:0,

        pageSizeAttendanceDetailInfo: 10,
        currentPageNoAttendanceDetailInfo: 1,
        currentItemCountAttendanceDetailInfo:0,

        pageSizeWorkLoadInfo: 10,
        currentPageNoWorkLoadInfo: 1,
        currentItemCountWorkLoadInfo:0,

        pageSizeWorkLoadDetailInfo: 10,
        currentPageNoWorkLoadDetailInfo: 1,
        currentItemCountWorkLoadDetailInfo:0
    },

    flushBackParams:function(){
        var self =this;
        switch (self._params.currrentPageType ){
            case 0:
                self._params.currentPageNo =self._params.currentPageNoAttendanceInfo ;
                self._params.pageSize = self._params.pageSizeAttendanceInfo  ;
                self._params.currentItemCount = self._params.currentItemCountAttendanceInfo  ;
                break;
            case 1:
                self._params.currentPageNo = self._params.currentPageNoAttendanceDetailInfo  ;
                self._params.pageSize = self._params.pageSizeAttendanceDetailInfo ;
                self._params.currentItemCount = self._params.currentItemCountAttendanceDetailInfo ;
                break;
            case 2:
                self._params.currentPageNo =self._params.currentPageNoWorkLoadInfo ;
                self._params.pageSize = self._params.pageSizeWorkLoadInfo ;
                self._params.currentItemCount = self._params.currentItemCountWorkLoadInfo ;
                break;
            case 3:
                self._params.currentPageNo =self._params.currentPageNoWorkLoadDetailInfo ;
                self._params.pageSize =self._params.pageSizeWorkLoadDetailInfo ;
                self._params.currentItemCount = self._params.currentItemCountWorkLoadDetailInfo;
                break;
        }
    },
    flushParams:function(){
        var self =this;
        switch (self._params.currrentPageType ){
            case 0:
                self._params.currentPageNoAttendanceInfo = self._params.currentPageNo;
                self._params.pageSizeAttendanceInfo = self._params.pageSize;
                self._params.currentItemCountAttendanceInfo = self._params.currentItemCount;
                break;
            case 1:
                self._params.currentPageNoAttendanceDetailInfo = self._params.currentPageNo;
                self._params.pageSizeAttendanceDetailInfo = self._params.pageSize;
                self._params.currentItemCountAttendanceDetailInfo = self._params.currentItemCount;
                break;
            case 2:
                self._params.currentPageNoWorkLoadInfo = self._params.currentPageNo;
                self._params.pageSizeWorkLoadInfo = self._params.pageSize;
                self._params.currentItemCountWorkLoadInfo = self._params.currentItemCount;
                break;
            case 3:
                self._params.currentPageNoWorkLoadDetailInfo = self._params.currentPageNo;
                self._params.pageSizeWorkLoadDetailInfo = self._params.pageSize;
                self._params.currentItemCountWorkLoadDetailInfo = self._params.currentItemCount;
                break;
        }
    },

    init: function () {
        this.initUI();
        this.initData();
        this.bindUI();

    },
    initData: function () {
        var self = this;
        $tempdata = $('tempdata');
        self.dataManager.directorID = $tempdata.data('directorid');
        $tempdata.remove();
        searchWithParams(1);
    },

    initUI: function () {
        var self = this;
        self.setDefaultTime(6,1,0);
        self.setDefaultTime(6,1,1);
        $('searchItemCount').val(0);
        $('searchItemPageCount').val(0);
        self.updateUIbyParams();
    },

    checkParams:function (){
        var self = this;
        if(!(self._params.beginTime < self._params.endTime)){
            showInfo("开始时间不能大于结束时间");
            return false;
        }
        return true;
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
            $('#endWorkLoadTime').val(date);
        //set default start Time:six day before
        mydate.setDate(mydate.getDate() - beginTime);
        var date = generalUtil.formateDate(mydate);
        if(Type == 0)
            $('#startAttendanceTime').val(date);
        else
            $('#startWorkLoadTime').val(date);
    },


    bindUI: function () {
        var self = this;
        var sellerIDLabel = $('#sellerIDLabel');
        var sellerID = $('#sellerID');
        $('#startAttendanceTime').click(function () {
            WdatePicker({dateFmt: 'yyyy-MM-dd', minDate: '%y-#{%M-3}-%d', maxDate: '%y-%M-#\{%d\}'});
        });

        $('#endAttendanceTime').click(function () {
            WdatePicker({dateFmt: 'yyyy-MM-dd', minDate: '%y-#{%M-3}-%d', maxDate: '%y-%M-#\{%d\}'});
        });

        $('#startWorkLoadTime').click(function () {
            WdatePicker({dateFmt: 'yyyy-MM-dd', minDate: '%y-#{%M-3}-%d', maxDate: '%y-%M-#\{%d\}'});
        });

        $('#endWorkLoadTime').click(function () {
            WdatePicker({dateFmt: 'yyyy-MM-dd', minDate: '%y-#{%M-3}-%d', maxDate: '%y-%M-#\{%d\}'});
        });
        $(document).on('click',".look",function() {
            self.flushParams();
            $('#attendanceDetailPage').show();
            $('#attendancePage').hide();
            self._params.currrentPageType = 1;
            self.flushBackParams();
            self._params.currentItemCount =0;
            self._params.subMerchantId =  $(this).attr("subMerchantID");
            searchWithParams(1);

        });

        $(document).on('click',".d_look",function() {

            self.flushParams();
            $('#workLoadDetailPage').show();
            $('#workLoadPage').hide();
            self._params.currrentPageType = 3;
            self.flushBackParams();
            self._params.currentItemCount =0;
            self._params.subMerchantId =  $(this).attr("subMerchantID");
            searchWithParams(1);

        });

        $(document).on('click',".AttendacneClearTime",function() {
            $('#startAttendanceTime').val("");
            $('#endAttendanceTime').val("");
        });

        $(document).on('click',".WorkLoadClearTime",function() {
            $('#startWorkLoadTime').val("");
            $('#endWorkLoadTime').val("");
        });

        $('#attendanceYesterdayButton').click(function(){
            self.setDefaultTime(0,1,0);
            searchWithParams(1);
        });

        $('#attendanceWeekButton').click(function(){
            self.setDefaultTime(6,1,0);
            searchWithParams(1);
        });

        $('#attendanceMonthButton').click(function(){
            self.setDefaultTime(30,1,0);
            searchWithParams(1);
        });

        $('#workLoadYesterdayButton').click(function(){
            self.setDefaultTime(0,1,1);
            searchWithParams(1);
        });

        $('#workLoadLastWeekButton').click(function(){
            self.setDefaultTime(6,1,1);
            searchWithParams(1);
        });

        $('#workLoadLastMonthButton').click(function(){
            self.setDefaultTime(30,1,1);
            searchWithParams(1);
        });

        $('#attendanceTabPage').click(function(){
            self.flushParams();
            self._params.currrentPageType = 0;
            $('#attendanceDetailPage').hide();
            $('#attendancePage').show();
            self.flushBackParams();
            self.updateUIbyParams();
        });

        $('#workLoadTabPage').click(function(){
            self.flushParams();
            $('#workLoadDetailPage').hide();
            $('#workLoadPage').show();
            self._params.currrentPageType = 2;
            self.flushBackParams();
            self.updateUIbyParams();
        });


        $('#workLoadSearchButton').click(function () {
            searchWithParams(1);
        });

        $('#attendanceSearchButton').click(function () {
            searchWithParams(1);
        });

        $('#attendanceDetailBack').click(function(){
            self.flushParams();
            $('#attendanceDetailPage').hide();
            $('#attendancePage').show();
            self._params.currrentPageType = 0;
            self.flushBackParams();
            self.updateUIbyParams();
        });

        $('#workLoadDetailBack').click(function(){
            self.flushParams();
            $('#workLoadDetailPage').hide();
            $('#workLoadPage').show();
            self._params.currrentPageType = 2;
            self.flushBackParams();
            self.updateUIbyParams();
        });

        $('.excelExport').click(function() {
                self.flushParams();
                self.flushBackParams();
                doExportExcel();
            });

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
                        searchWithParams(n);
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
                mode : 'click',//默认值是link，可选link或者click
                pageSize:self._params.pageSize,
                click : function(n){
                    self._params.pageSize = this.pageSize;
                    searchWithParams(n);
                    //this.selectPage(n);
                }
            },true);
        }
    },

    updateUIToParams:function() {
        var self = this;

        if (self._params.currrentPageType == 0 || self._params.currrentPageType == 1){
            if ($('#startAttendanceTime').val() == "" || $('#endAttendanceTime').val() == ""){
                showInfo("输入时间不能为空");
                return -1;
            }
            self._params.beginTime = parseInt(generalUtil.getUTCtime($('#startAttendanceTime').val() + " 00:00:00") / 1000);
            self._params.endTime = parseInt(generalUtil.getUTCtime($('#endAttendanceTime').val() + " 23:59:59") / 1000 + 1);
        }else{
            if ($('#startWorkLoadTime').val() == "" || $('#endWorkLoadTime').val() == ""){
                showInfo("输入时间不能为空");
                return -1;
            }
            self._params.beginTime = parseInt(generalUtil.getUTCtime($('#startWorkLoadTime').val() + " 00:00:00") / 1000);
            self._params.endTime = parseInt(generalUtil.getUTCtime($('#endWorkLoadTime').val() + " 23:59:59") / 1000 + 1);

        }

        self._params.searchType = self._params.currrentPageType;
        return 0;
    },

    getParams: function () {
        var  self = this;

        if ( self.updateUIToParams() != 0){
            return null;
        }

        return $.extend({}, {
            startTime: self._params.beginTime,
            endTime:self._params.endTime,
            pageSize:self._params.pageSize,
            currentPageNo:self._params.currentPageNo,
            directorID:self.dataManager.directorID,
            subMerchantID:self._params.subMerchantId,
            searchType:self._params.searchType
        });
    }


};
