/**
 * Created by wangzhen on 2015/7/27.
 */
function showInfo(content) {
    alert(content);
}
var sessionInfoTemplate =
    '<tr>' +
    '<td>$time</td>' +
    '<td>$sessionCount</td>' +
    '<td>$sessionRate %</td>' +
    '<td><label width="50%" style="background: url(../images/u119.png) no-repeat;" AutoSize="false"  class="pull-left">&nbsp;$sessionLength</label></td>' +
    '</tr>';

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


function doExportExcel(){
    var self = sessionInfoStatistics;
    var params= "../sessionInfo/get-session-count-file.xls?";

    if(!self.checkParams()){
        return null;
    }
    var param = self.getParams();

    params = params + "&startTime="+param.startTime +
        "&endTime="+param.endTime ;

    window.location.href = params;
};

function doSearch(pageNo,callback){
    var self = sessionInfoStatistics;

    if ( self.updateUIToParams() != 0){
        return null;
    }

    self._params.currentPageNo = pageNo;

    if(!self.checkParams()){
        return null;
    }

    var params = self.getParams();
    MaskUtil.mask("正在查询，请稍后");
    $.post('../sessionInfo/get-session-count-list',
        params,
        function (resultInfo) {
            MaskUtil.unmask();
            if (resultInfo && !!resultInfo.success) {
                var pager = resultInfo.data;
                callback.call(this,pager);
            } else {
                showInfo((resultInfo || {}).message || '服务器发生故障');
            }
        },
        'json');
};

function searchWithParams(pageNo){
    var self = sessionInfoStatistics;
    doSearch(pageNo,function (pager) {
        var containersessionInfoPage = $('#sessionInfoPanel');
        containersessionInfoPage.empty();

        var sessionInfoMappingList = pager.data;
        if (!!sessionInfoMappingList) {
            var toAppend = '';
            for (var i = 0; i < sessionInfoMappingList.length; i++) {
                var currentsessionInfo = sessionInfoMappingList[i];
                var time = currentsessionInfo.time;
                var sessionCount = currentsessionInfo.sessionCount;
                var sessionRate = currentsessionInfo.sessionRate;
                var lenStr='&nbsp;';
                for(var j=0;j< sessionRate ;j++){
                    lenStr +='&nbsp;'
                }
                toAppend +=
                    sessionInfoTemplate.replace(/\$time/g, time.toString())
                        .replace(/\$sessionCount/g, sessionCount.toString())
                        .replace(/\$sessionRate/g, sessionRate.toString())
                        .replace(/\$sessionLength/g, lenStr.toString());
            }
            containersessionInfoPage.append(toAppend);
        }
    });
};

var sessionInfoStatistics = {
    _params: {
        beginTime:0,
        endTime:0
    },

    init: function () {
        this.initUI();
        this.initData();
        this.bindUI();

    },
    initData: function () {
        var self = this;
        searchWithParams(1);
    },

    initUI: function () {
        var self = this;
        self.setDefaultTime(0,0,0);
        self.setDefaultTime(0,0,1);
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
            $('#endsessionInfoTime').val(date);
        else
            $('#endWorkLoadTime').val(date);
        //set default start Time:six day before
        mydate.setDate(mydate.getDate() - beginTime);
        var date = generalUtil.formateDate(mydate);
        if(Type == 0)
            $('#startsessionInfoTime').val(date);
        else
            $('#startWorkLoadTime').val(date);
    },


    bindUI: function () {
        var self = this;

        $('#startsessionInfoTime').click(function () {
            WdatePicker({dateFmt: 'yyyy-MM-dd', minDate: '%y-#{%M-3}-%d', maxDate: '%y-%M-#\{%d\}'});
        });

        $('#endsessionInfoTime').click(function () {
            WdatePicker({dateFmt: 'yyyy-MM-dd', minDate: '%y-#{%M-3}-%d', maxDate: '%y-%M-#\{%d\}'});
        });

        $(document).on('click',".AttendacneClearTime",function() {
            $('#startsessionInfoTime').val("");
            $('#endsessionInfoTime').val("");
        });

        $('#sessionInfoYesterdayButton').click(function(){
            self.setDefaultTime(0,1,0);
            searchWithParams(1);
        });

        $('#sessionInfoWeekButton').click(function(){
            self.setDefaultTime(6,1,0);
            searchWithParams(1);
        });

        $('#sessionInfoMonthButton').click(function(){
            self.setDefaultTime(30,1,0);
            searchWithParams(1);
        });


        $('#sessionInfoSearchButton').click(function () {
            searchWithParams(1);
        });


        $('.excelExport').click(function() {
            doExportExcel();
        });

    },


    updateUIToParams:function() {
        var self = this;

        if ($('#startsessionInfoTime').val() == "" || $('#endsessionInfoTime').val() == "") {
            showInfo("输入时间不能为空");
            return -1;
        }
        self._params.beginTime = parseInt(generalUtil.getUTCtime($('#startsessionInfoTime').val() + " 00:00:00") / 1000);
        self._params.endTime = parseInt(generalUtil.getUTCtime($('#endsessionInfoTime').val() + " 23:59:59") / 1000 + 1);

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
        });
    }


};
$(function () {
    sessionInfoStatistics.init();
    sessionInfoStatistics.updateUIToParams();
});