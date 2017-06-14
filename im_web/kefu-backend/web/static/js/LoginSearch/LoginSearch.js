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
};

var loginManager = {
    _params: {
        pageSize:10,
        currentPageNo:1,
        merchantId:0,
        subMerchantId:0
    },

    init: function () {
        this.initUI();
        this.initData();
        this.bindUI();
    },

    setDefaultTime:function(beginTime,endTime){
        var mydate = new Date();
        //set default end Time :yesterday
        mydate.setDate(mydate.getDate() - endTime);
        var date = generalUtil.formateDate(mydate);
        $('#endTime').val(date + " 23:59:59");
        //set default start Time:six day before
        mydate.setDate(mydate.getDate() - beginTime);
        var date = generalUtil.formateDate(mydate);
        $('#startTime').val(date + " 00:00:00");
    },

    searchSubMerchantNameList:function(merchantId ,callback){
        $.post(
            '../merchantLogInOut/get-submerchant-info-pair',
            {
                merchantId: merchantId
            },
            function (resultInfo) {
                if (resultInfo && !!resultInfo.success) {
                    callback.call(null, resultInfo);
                } else {
                    showInfo((resultInfo || {}).message || '服务器发生故障，请稍后再试');
                }
            }
        )
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

    searchInfo:function(){
        var self = this;
        var params = self.getParams();
        if(params == null)
            return;
        MaskUtil.mask("正在处理请稍后");
        $.post('../merchantLogInOut/detail',
            params,
            function (resultInfo) {
                MaskUtil.unmask();
                if (resultInfo && !!resultInfo.success) {
                    var data = resultInfo.data.data;
                    $("#loginDetailTab").datagrid('loadData', {
                        total: resultInfo.data.count,
                        rows: data
                    });
                } else
                    showInfo((resultInfo || {}).message || '服务器发生故障，请稍后再试');
            },
            'json');
    },

    initData:function(){
        var self =this;
        $('#loginDetailTab').datagrid({
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
                {field: 'logInOutTime', title: '时间', width: '30%', align: 'center', sortable: false},
                {field: 'userName', title: '账号', width: '30%', align: 'center', sortable: false},
                {
                    field: 'logInOutType', title: '日志', width: '40%', align: 'center', sortable: false,
                    formatter: function (val, row, index) {
                        if(val == 1)
                            return "登出";
                        else
                            return "登陆";
                        return btn;
                    }
                }
            ]],
        });

        var pagination = $('#loginDetailTab').datagrid('getPager');
        pagination.pagination({
            onSelectPage: function (pageNumber, pageSize) {
                self._params.pageSize = pageSize;
                self._params.currentPageNo = pageNumber;
                self.searchInfo();

            }
        });
    },

    initUI:function(){
        var self = this;
        self.setDefaultTime(0,0);
    },

    bindUI: function () {
        var self = this;
        $('#startTime').click(function () {
            WdatePicker({dateFmt: 'yyyy-MM-dd HH:mm:ss', maxDate: '%y-%M-%d 23:59:59'});
        });

        $('#endTime').click(function () {
            WdatePicker({dateFmt: 'yyyy-MM-dd HH:mm:ss', maxDate: '%y-%M-%d 23:59:59'});
        });
        /**
         * 清空按钮
         */
        $('#clearTime').click(function () {
            $('#startTime, #endTime').val('');
        });
        $('#resetCondition').click(function () {
            $('#startTime, #endTime').val('');
            var selectControl = document.getElementById("serviceName");
            selectControl.innerHTML = "";
            $('#storeName').combo('clear');
        });
        $('#search').click(function(){
            self.searchInfo();
        });
        /**店铺名称自动提示**/
        $('#storeName').combobox({
            valueField: 'merchantID',
            textField: 'storeName',
            mode: 'remote',
            panelHeight: 'auto',
            hasDownArrow: false,
            loader: function (param, success) {
                if (!param.q) {
                    return false;
                    //success([]);
                } else {
                    self.searchStoreNameCompleteList(param.q || '', function (data) {
                        success(data);
                    });
                }
            },
            onSelect:function(rec){
                self._params.merchantId = rec.merchantID;
                self.searchSubMerchantNameList(rec.merchantID,function(resultInfo){
                    if (resultInfo && !!resultInfo.success) {
                        var selectControl = document.getElementById("serviceName");
                        selectControl.innerHTML = "";
                        var memberList = resultInfo.data.data;
                        selectControl.options.add(new Option("所有",0));
                        for(var i=0;i<memberList.length;i++){
                            var index = memberList[i];
                            selectControl.options.add(new Option(index.username,index.memberId));
                        }
                    } else {
                        showError((resultInfo || {}).message || '服务器发生故障，请稍后再试');
                    }
                })
            }
        });
    },

    getParams:function(){
        var self =this;
        if ($('#startTime').val() == "" && $('#endTime').val() == "" ) {
            isValid = false;
            showInfo('时间为必填项');
            return null;
        }

        var startTime=parseInt(generalUtil.getUTCtime($('#startTime').val() + " 00:00:00") / 1000);
        var endTime=parseInt(generalUtil.getUTCtime($('#endTime').val()+ " 23:59:59") / 1000 + 1);
        if(startTime > endTime){
            showInfo("结束时间不能小于开始时间");
            return null;
        }

        if(self._params.merchantId == 0 ){
            showInfo("没找到指定的商家");
            return null;
        }

        return $.extend({}, {
            merchantId:  self._params.merchantId,
            subMerchantId : $('#serviceName').val(),
            startTime:startTime,
            endTime: endTime,
            pageSize:self._params.pageSize,
            currentPageNo:self._params.currentPageNo
        });
    }
}

$(function () {
    loginManager.init();
});