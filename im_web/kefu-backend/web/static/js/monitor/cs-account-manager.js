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
var myview = $.extend({}, $.fn.datagrid.defaults.view, {
    onAfterRender: function (target) {
        $.fn.datagrid.defaults.view.onAfterRender.call(this, target);
        var opts = $(target).datagrid('options');
        var vc = $(target).datagrid('getPanel').children('div.datagrid-view');
        vc.children('div.datagrid-empty').remove();
        if (!$(target).datagrid('getRows').length) {
            var d = $('<div class="datagrid-empty"></div>').html(opts.emptyMsg || 'no records').appendTo(vc);
            d.css({
                position: 'absolute',
                left: 0,
                top: 50,
                width: '100%',
                textAlign: 'center'
            });
        }
    }
});

function checkPassword(str) {
    var re = /^\w{6,17}$/
    if (re.test(str)) {
        return true;
    } else {
        showError("密码必须长度在6~18之间，只能包含字母、数字");
    }
    return false;
}

function stopuser() {
    var rows = $('#searchedTab').datagrid('getChecked');
    var AllRows = $("#searchedTab").datagrid("getRows");
    if (rows != "" && rows != null) {
        var rowIDs = '';
        for (var i = 0; i < rows.length; i++) {
            rowIDs += rows[i].moumouId + ",";
        }
        rowIDs = rowIDs.substring(0, rowIDs.length - 1);
        postcsModiState(function (pager) {
            for (var i = 0; i < rows.length; i++) {
                var index = $("#searchedTab").datagrid("getRowIndex",rows[i]);
                AllRows[index].accountStates = "停用";
                $("#searchedTab").datagrid("uncheckAll");
                $("#searchedTab").datagrid("refreshRow",index);
            }

        }, 0, rowIDs);
    }
    else {
        showError('请选择要停用的用户');
    }
}
function deleteuser() {
    var rows = $('#searchedTab').datagrid('getChecked');
    if (rows != "" && rows != null) {
        var confirmDeleteWindow = $('#confirmDeleteWindow');
        confirmDeleteWindow.find('.content').html('确定删除这些行吗？');
        confirmDeleteWindow.show(function () {
            $(this).window('center').window('open');
        });
    }
    else {
        showError('请选择要删除的账号');
    }
}

function startuser() {
    var rows = $('#searchedTab').datagrid('getChecked');
    var AllRows = $("#searchedTab").datagrid("getRows");
    if (rows != "" && rows != null) {
        var rowIDs = '';
        for (var i = 0; i < rows.length; i++) {
            rowIDs += rows[i].moumouId + ",";
        }
        rowIDs = rowIDs.substring(0, rowIDs.length - 1);
        postcsModiState(function (pager) {
            for (var i = 0; i < rows.length; i++) {
                var index = $("#searchedTab").datagrid("getRowIndex",rows[i]);
                AllRows[index].accountStates = "开启";
                $("#searchedTab").datagrid("uncheckAll");
                $("#searchedTab").datagrid("refreshRow",index);
            }

        }, 1, rowIDs);
    }
    else {
        showError('请选择要启用的账号');
    }
}

function getcsStateParams(type, rows) {
    var csparams = {
        pageSize: 10, currentPageNo: 1
    };
    return $.extend({}, {

        CSName: $('#CSName').combo('getText'),
        ImportUser: $('#ImportUser').val(),
        Starttime: $('#startTime').val(),
        Endtime: $('#endTime').val(),
        CSUserName: $('#CSUserName').val(),
        AccountStates: $('#AccountStates').val(),
        AccountLimits: 10,
        CSPassword: "xxx",
        AccountOperatorable: 0,
        startCount: 0,
        endCount: 0,
        rows: rows,
        status: type
    }, csparams);
}

function postCsDeleteUser(callback,rows){
    var self = this;

    MaskUtil.mask("正在处理请稍后");
    $.post('../account/delete-account',
        {
            ids:rows,
            status:-1
        },
        function (resultInfo) {
            MaskUtil.unmask();
            if (resultInfo && !!resultInfo.success) {
                callback.call();
            } else {
                if (resultInfo.errorCode == -2) {
                    var list = [];
                    $('#searchTab').datagrid({
                        data: list,
                        view: myview,
                        emptyMsg: '删除失败'
                    });
                }
                else
                    showError((resultInfo || {}).message || '服务器发生故障，请稍后再试');
            }
        },
        'json');
}
function postcsModiState(callback, type, rows) {
    var self = this;
    //self._params.currentPageNo = 1;

    MaskUtil.mask("正在处理请稍后");
    $.post('../account/modi-cs-account-status',
        {
            ids:rows,
            status:type
        },
        function (resultInfo) {
            MaskUtil.unmask();
            if (resultInfo && !!resultInfo.success) {
                callback.call();
            } else {
                if (resultInfo.errorCode == -2) {
                    var list = [];
                    $('#searchTab').datagrid({
                        data: list,
                        view: myview,
                        emptyMsg: '未查询到数据'
                    });
                }
                else
                    showError((resultInfo || {}).message || '服务器发生故障，请稍后再试');
            }
        },
        'json');
}

function findRowByMoumouId(moumouId){
    var AllRows = $("#searchedTab").datagrid("getRows");
    for(var i=0;i<AllRows.length;i++){
        if(AllRows[i].moumouId == moumouId)
            return AllRows[i];
    }
    return null;
}

function editUser(moumouId) {
    var self = CSAccountManager;
    var row = findRowByMoumouId(moumouId);
    if (row) {
        $(".moumouName").text(row.moumouUserName);
        document.getElementById('basicInfoPage').setAttribute('class',"on");
        document.getElementById('passwordChangePage').setAttribute('class',"");
        $('#basicInfo').show();
        $('#password').hide();
        $('#modifycsDialog').dialog('open').dialog('setTitle', '修改');
        $('#passwordInput').val("");
        $('#passwordConfim').val("");
        self._dataManager.currentModifyMoumouId = row.moumouId;
        var optionOrgnById ="option[value='"+row.operatorOrganId+"']";
        var optionMemberById = "option[value='"+row.opratorId+"']";

        if (row.operatorOrganId == null || row.operatorOrganId =="0")
            var optionOrgnById = "option[value='0']";

        if(row.opratorId == null || row.opratorId =="0")
            var optionMemberById = "option[value='0']";


        $("#ModifyOrgnName").find(optionOrgnById).attr("selected",true);
        $("#ModifyMemberName").find(optionMemberById).attr("selected",true);
        $("#limitNumber").val(row.accountLimits);
        self._tmpdata.currentMoumouId = row.moumouId;
    }
}


function ModifiedConfirm() {
    postModifiedConfirm(function (pager) {
    });
}

function getcsParams() {
    var csparams = {
        pageSize: 10, currentPageNo: 1
    };
    return $.extend({}, {
        CSName:  $('#CSName').combo('getText'),
        CSAccountAuthor: $('#cs-AccountAuthor').val(),
        ImportUser: $('#cs-ImportUser').val(),
        ImportTeam: $('#cs-ImportTeam').val(),
        AccountStates: 1,/*$('#cs-AccountStates').val(),*/
        CSTeam: $('#cs-Team').combobox('getText'),
        CSUserName: $('#cs-UserName').val(),
        AccountLimits: $('#cs-accountLimits').val(),
        CSPassword: $('#cs-Password').val(),
        Starttime: $('#startTime').val(),
        Endtime: $('#endTime').val(),
        AccountOperatorable: 0,
        startCount: 0,
        endCount: 0,
        rows: "xxx",
        status: 1
    }, csparams);
}

function postModifiedConfirm(callback) {
    var self = this;
    var params = getcsParams();
    //self._params.currentPageNo = 1;

    MaskUtil.mask("正在处理请稍后");
    $.post('../monitor/modi-cs-account-manager',
        params,
        function (resultInfo) {
            MaskUtil.unmask();
            if (resultInfo && !!resultInfo.success) {
                var data = resultInfo.data.pager.resultsForCurrentPage;
                $('#sellerWorkTab').datagrid('updateRow', {
                    index: $('#cs-modiUpdateindex').val(),
                    row: data[0]
                });
                $('.editcls').linkbutton({text: '编辑', plain: true, iconCls: 'icon-edit'});
                $('#modifycsDialog').dialog('close');
            } else {
                if (resultInfo.errorCode == -2) {
                    var list = [];
                    $('#sellerWorkTab').datagrid({
                        data: list,
                        view: myview,
                        emptyMsg: '未查询到数据'
                    });
                }
                else
                    showError((resultInfo || {}).message || '服务器发生故障，请稍后再试');
            }
        },
        'json');
}

function Modifiedcancel() {
    $('#modifycsDialog').dialog('close');

}

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
function showError(content) {
    $.messager.alert('错误', '操作失败:' + content, 'error');
}

function showSuccess(content) {
    $.messager.alert('成功', '操作成功:' + content, 'info');
}

var CSAccountManager;
CSAccountManager = {
    _tmpdata:{
        currentOrgnName:"",
        currentOrgnId:0,
        currentMoumouId:null,
        currentPageNo:1,
        currentPageSize:10
    },
    _dataManager:{
        allMember:null,
        currentOwnerIdList:null,
        currentImportIdList:null,
        currentModifyMoumouId:null
    },
    init: function () {
        this.initUI();
        this.initData();
        this.bindUI();
    },
    initData: function () {
        var self = this;

        $('#endTime').val("");
        $('#startTime').val("");

        $('#searchedTab').datagrid({
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
                {field: 'ck', title: '选择', width: '15%', align: 'center', sortable: false, checkbox: true},
                {field: 'moumouUserName', title: '哞哞登录名', width: '10%', align: 'center', sortable: false},
                {field: 'moumouUserLevel', title: '哞哞账号权限', width: '10%', align: 'center', sortable: false},
                {field: 'importName', title: '导入人员', width: '10%', align: 'center', sortable: false},
                {field: 'importTime', title: '导入时间', width: '10%', align: 'center', sortable: false},
                {field: 'importUserTeam', title: '导入人员组织', width: '10%', align: 'center', sortable: false},
                {field: 'operatorRealName', title: '账号所属人员', width: '10%', align: 'center', sortable: false},
                {field: 'operatorOrganName', title: '账号所属组织', width: '10%', align: 'center', sortable: false},
                {field: 'accountStates', title: '状态', width: '10%', align: 'center', sortable: false},
                {field: 'accountLimits', title: '接待上限', width: '10%', align: 'center', sortable: false},
                {
                    field: 'accountOperatorable', title: '操作', width: '10%', align: 'center', sortable: false,
                    formatter: function (val, row, index) {
                        //var btn = '<a href="www.baidu.com<!--javascript:void(0)-->" class="editcls" onclick="editRow(\''+rec.projectname+'\',\''+rec.projectpackage+'\')" >编辑</a>';
                        var btn = '<a href="#" class="editcls l-btn l-btn-small l-btn-plain" onclick="editUser(' +row.moumouId+
                            ')" group="" id=""><span class="l-btn-left l-btn-icon-left"><span class="l-btn-text">编辑</span><span class="l-btn-icon icon-edit">&nbsp;</span></span></a>';
                        return btn;
                    }


                }
            ]],
            //增加编辑按钮
            onLoadSuccess: function (data) {
                $('.editcls').linkbutton({text: '编辑', plain: true, iconCls: 'icon-edit'});
            }
        });
        var pagination = $('#searchedTab').datagrid('getPager');
        pagination.pagination({
            onSelectPage: function (pageNumber, pageSize) {
                self._tmpdata.currentPageSize = pageSize;
                self._tmpdata.currentPageNo = pageNumber;
                self.searchCSAccount();
            }
        });
        $tempdata = $('tmpdata');
        self._dataManager.allMember = $tempdata.data('allmemberlist');
        $tempdata.remove();
    },
//获取数据的接口,捕捉动作，筛查数据
    initUI: function () {
        /**设置表格高度**/
        var docHeight = $(document).height();
        var searchBarHeight = $('.searchbar').outerHeight(true);
        var conditionHeight = $('.cla_nav').outerHeight(true);
        var panelHeaderHeight = $('#content>.title_nav').outerHeight(true);

        $('#sellerWorkTab').height(docHeight - searchBarHeight - conditionHeight - panelHeaderHeight - 50);
    },

    getOrgNameById:function(id){
        var self =this;
        for (var i= 0 ;i<self._dataManager.allMember.length;i++ ){
            if(self._dataManager.allMember[i].operatorid == id){
                self._tmpdata.currentOrgnId =self._dataManager.allMember[i].organid;
                self._tmpdata.currentOrgnName=self._dataManager.allMember[i].organName;
                return;
            }
        }
        self._tmpdata.currentOrgnId =0;
        self._tmpdata.currentOrgnName="--";
    },

    getSupervisorById:function(id){
        var self =this;
        for (var i= 0 ;i<self._dataManager.allMember.length;i++ ){
            if(self._dataManager.allMember[i].operatorid == id){
                return self._dataManager.allMember[i].issupervisor;
            }
        }
        return 0;
    },

    flushDataToMemberList:function(orgId,selectName,paramIndex){
        var self  =this;
        $.post(
            '../account/get-member-List-in-org',
            {
                orgId: orgId
            },
            function (resultInfo) {
                if (resultInfo && !!resultInfo.success) {
                    var selectControl = document.getElementById(selectName);
                    selectControl.innerHTML = "";
                    var memberList = resultInfo.data.memberList;
                    selectControl.options.add(new Option("全部人员",0));
                    var idList = "";
                    for(var i=0;i<memberList.length;i++){
                        var index = memberList[i];
                        selectControl.options.add(new Option(index.operatorrealname,index.operatorid));
                        idList +=index.operatorid+",";
                    }
                    if(paramIndex == 0)
                        self._dataManager.currentOwnerIdList=idList;
                    else if(paramIndex == 1)
                        self._dataManager.currentImportIdList=idList;
                        
                } else {
                    showError((resultInfo || {}).message || '服务器发生故障，请稍后再试');
                }
            }
        )

    },
    searchCSAccount:function(){
        var self=this;
        var params = self.getParams();
        MaskUtil.mask("正在处理请稍后");
        $.post(
            '../account/get-cs-account-manager',
            params,
            function (resultInfo) {
                MaskUtil.unmask();
                if (resultInfo && !!resultInfo.success) {
                    var data = resultInfo.data.pager.resultsForCurrentPage;
                    var resultArray = new Array();
                    var resiltArraryLast = new  Array();
                    if(params.ownerMemberId == "0" && params.ownerOrgnId != "0" ){
                        for (var i=0;i< data.length;i++){
                            var idItem = data[i].opratorId +",";
                            if(self._dataManager.currentOwnerIdList.indexOf(idItem) >=0)
                                resultArray.push(data[i]);
                        }
                    }else
                        resultArray =data;

                    if(params.importMemberId == "0" && params.importOrgnId != "0" ){
                        for (var i=0;i< resultArray.length;i++){
                            var idItem = resultArray[i].importCsId +",";
                            if(self._dataManager.currentImportIdList.indexOf(idItem) >=0)
                                resiltArraryLast.push(data[i]);
                        }
                    }else
                        resiltArraryLast = resultArray;

                    for(var i=0;i<resiltArraryLast.length ;i++){
                        self.getOrgNameById(resiltArraryLast[i].opratorId);
                        resiltArraryLast[i].operatorOrganId = self._tmpdata.currentOrgnId;
                        resiltArraryLast[i].operatorOrganName= self._tmpdata.currentOrgnName;
                        self.getOrgNameById(resiltArraryLast[i].importCsId);
                        resiltArraryLast[i].importUserOrgnId = self._tmpdata.currentOrgnId;
                        resiltArraryLast[i].importUserTeam= self._tmpdata.currentOrgnName;
                    }
                    $('#searchedTab').datagrid('loadData', {
                        total: resultInfo.data.pager.totalRecordSize,
                        rows: resiltArraryLast
                    });
                } else {
                    if (resultInfo.errorCode == -2) {
                        $('#searchedTab').datagrid('loadData', {
                            total: 0,
                            rows: []
                        });
                    }
                    else
                        showError((resultInfo || {}).message || '服务器发生故障，请稍后再试');
                }
            }
        );
    },
    //捕捉提交数据的动作，与java后台交互
    bindUI: function () {
        var self = this;

        $('#startTime').click(function () {
            WdatePicker({dateFmt: 'yyyy-MM-dd ', minDate: '%y-%M-#{%d-30}', maxDate: '%y-%M-%d'});
        });
        $('#endTime').click(function () {
            WdatePicker({dateFmt: 'yyyy-MM-dd ', minDate: '%y-%M-#{%d-30}', maxDate: '%y-%M-%d'});
        });
        $('#OwnerOrgnName').change(function(){
            self.flushDataToMemberList($('#OwnerOrgnName option:selected').val(),"OrgnMemberName",0);
        });
        $('#ImportOrgnName').change(function(){
            self.flushDataToMemberList($('#ImportOrgnName option:selected').val(),"ImportMemberName",1);
        });
        $('#ModifyOrgnName').change(function(){
            self.flushDataToMemberList($('#ModifyOrgnName option:selected').val(),"ModifyMemberName",3);
        });
        $('#queryCsAccountInfo').click(function(){
            if(self.checkParams()) {
                self.searchCSAccount();
            }
        });
        /*用户导入数据*/
        $('#excelImport').uploader({
            action: '../account/batch-add.do',
            //mode: 'flash',
            //swf: '/js/uploader/uploader.swf',
            name: 'file',
            auto: 'true',
            fileTypeDesc: '请选择excel表格(.xlsx)',
            fileTypeExts: 'xlsx',
            fileSizeLimit: '2M',
            onError: function (e) {
                showError('错误:'+ e.message);
            },
            onStart: function () {
                $('#logPanel').append('<h2>' +
                    generalUtil.formateFullTime(new Date()) + '上传文件：' + arguments[0]['file']['name']
                    + '</h2>' + '<p class="result">上传文件已开始...</p>');
            },
            onSuccess: function (resultInfo) {
                var data = resultInfo.data;
                if (resultInfo.data) {
                    var result = eval('(' + resultInfo.data + ')');

                    if (result.success) {
                        var errorDetail = result.data.errorDetail;
                        var array = [];
                        for (var index in  errorDetail) {
                            array.push({
                                index: index,
                                message: errorDetail[index]
                            });
                        }

                        var detail = '';
                        for (var i = 0; i < array.length; i++) {
                            detail = detail + '<p class="error-detail">第' + array[i]['index'] + '行错误原因：' + array[i]['message'] + '<p>';
                        }
                        var csdata = result.data.pager.resultsForCurrentPage;

                        for(var i=0;i<csdata.length ;i++){
                            self.getOrgNameById(csdata[i].importCsId);
                            csdata[i].importUserOrgnId = self._tmpdata.currentOrgnId;
                            csdata[i].importUserTeam= self._tmpdata.currentOrgnName;
                        }
                        $('#searchedTab').datagrid('loadData', {
                            total: result.data.pager.totalRecordSize,
                            rows: csdata
                        });
                    } else {
                        showError('服务器发生故障，请稍后重试');
                    }
                }
            }
        });

        $('#basicInfoPage').click(function(){
            this.setAttribute('class',"on");
            document.getElementById('passwordChangePage').setAttribute('class',"");
            $('#basicInfo').show();
            $('#password').hide();
        });
        $('#passwordChangePage').click(function(){
            this.setAttribute('class',"on");
            document.getElementById('basicInfoPage').setAttribute('class',"");
            $('#basicInfo').hide();
            $('#password').show();
        });

        $('#changeInfoSave').click(function() {
            var orgnId = $('#ModifyOrgnName option:selected').val();
            if (orgnId == 0) {
                showError("请选择组织");
                return;
            }
            var memberId = $('#ModifyMemberName option:selected').val();
            var memberName = $('#ModifyMemberName option:selected').text();
            if (memberId == 0) {
                showError("请选择人员");
                return;
            }
            var limited = $('#limitNumber').val();
            if (limited == null || limited =='') {
                showError("请设置接待上限");
                return;
            }
            var issupervisor=self.getSupervisorById(memberId);

            MaskUtil.mask("正在处理请稍后");
            $.post('../account/modi-cs-account-detail',
                {
                    orgnId: orgnId,
                    limited: limited,
                    memberId: memberId,
                    memberName: memberName,
                    moumouId: self._tmpdata.currentMoumouId,
                    issupervisor:issupervisor
                },
                function (resultInfo) {
                    MaskUtil.unmask();
                    if (resultInfo && !!resultInfo.success) {
                        showSuccess("更新成功");
                        $('#modifycsDialog').dialog('close');
                        var row = findRowByMoumouId(self._tmpdata.currentMoumouId);
                        row.operatorOrganId =$("#ModifyOrgnName option:selected").val();
                        row.operatorOrganName=$("#ModifyOrgnName option:selected").text();
                        row.operatorRealName=$("#ModifyMemberName option:selected").text();
                        row.opratorId =$("#ModifyMemberName option:selected").val();
                        row.accountLimits = $("#limitNumber").val();
                        if(issupervisor == 0)
                            row.moumouUserLevel="员工";
                        else
                            row.moumouUserLevel="主管";
                        var index = $("#searchedTab").datagrid("getRowIndex",row);
                        $("#searchedTab").datagrid("uncheckAll");
                        $("#searchedTab").datagrid("refreshRow",index);
                    } else {
                        showSuccess((resultInfo || {}).message || '服务器发生故障，请稍后再试');
                    }
                },
                'json');

        });

        $('#changePasswordSave').click(function() {
            var password = $('#passwordInput').val();
            var passwordConfim = $('#passwordConfim').val();
            if (password!=passwordConfim){
                showError("2次密码不一致");
                return;
            }
            if(!checkPassword(password)){
                return;
            }
            MaskUtil.mask("正在处理请稍后");
            $.post('../account/modi-cs-account-password',
                {
                    password: password,
                    moumouId: self._tmpdata.currentMoumouId
                },
                function (resultInfo) {
                    MaskUtil.unmask();
                    if (resultInfo && !!resultInfo.success) {
                        showSuccess("更新成功");
                        $('#modifycsDialog').dialog('close');
                    } else {
                        showError((resultInfo || {}).message || '服务器发生故障，请稍后再试');
                    }
                },
                'json');
        });

        $('#changeInfoCancel').click(function(){
            $('#modifycsDialog').dialog('close');
        });

        $('#changePasswordCancel').click(function(){
            $('#modifycsDialog').dialog('close');
        });

        $('#resetCondition').click(function(){
            $('#CSName').combo('clear');
            $("#OwnerOrgnName").find("option[value='0']").attr("selected",true);
            $("#OrgnMemberName").find("option[value='0']").attr("selected",true);
            $("#ImportOrgnName").find("option[value='0']").attr("selected",true);
            $("#ImportMemberName").find("option[value='0']").attr("selected",true);
            $("#MemberStatus").find("option[value='888']").attr("selected",true);
            $('#endTime').val("");
            $('#startTime').val("");
        });

        $('#clearTime').click(function(){
            $('#endTime').val("");
            $('#startTime').val("");
        });

        $('#CSName').combobox({
            valueField: 'moumouUserName',
            textField: 'moumouUserName',
            mode: 'remote',
            loader: function (param, success) {
                if (! param.q) {
                    success([]);
                }else{
                    self.searchMoumouNameByPartName(param.q || '', function (data) {
                        success(data);
                    });
                }
            }
        });

        //确认删除
        $('#confirmDelete').click(function () {
            var rows = $('#searchedTab').datagrid('getChecked');
            var rowIDs = '';
            for (var i = 0; i < rows.length; i++) {
                rowIDs += rows[i].moumouId + ",";
            }

            rowIDs = rowIDs.substring(0, rowIDs.length - 1);

            postCsDeleteUser(function (pager) {
                for (var i = 0; i < rows.length; i++) {
                    var index = $("#searchedTab").datagrid("getRowIndex",rows[i]);
                    $("#searchedTab").datagrid("deleteRow",index);
                }
                $("#searchedTab").datagrid("reload");
            }, rowIDs);

            $('#confirmDeleteWindow').window('close');
        });

        //取消删除
        $('#cancelDelete').click(function () {
            $('#confirmDeleteWindow').window('close');
        });

        self.searchCSAccount();

    },
    searchMoumouNameByPartName: function (value, callback) {
        $.post(
            '../account/get-moumou-name-by-part-name',
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
    checkParams: function () {
        var isValid = true;

        if (isValid) {
            if (parseInt(generalUtil.getUTCtime($('#startTime').val() + " 00:00:00") / 1000) >= parseInt(generalUtil.getUTCtime($('#endTime').val() + " 23:59:59") / 1000 + 1)) {
                isValid = false;
                showError('止时间不能早于起时间');
            }
        }

        return isValid;
    },
    getParams: function () {
        var self =this;
        var csIdList = "";
        if($('#OrgnMemberName option:selected').val() == "0"){
            if($('#OwnerOrgnName option:selected').val() == "0"){
                csIdList = null;
            }else {
                $("#OrgnMemberName option").each(function () {
                    csIdList += this.value + ",";
                });
                csIdList = csIdList.substr(0, csIdList.length - 1);
            }
        }else{
            csIdList = $('#OrgnMemberName option:selected').val();
        }
        var importCsIdList = "";

        if($('#ImportMemberName option:selected').val() == "0"){
            if($('#ImportOrgnName option:selected').val() == "0"){
                importCsIdList  =null;
            }else {
                $("#ImportMemberName option").each(function () {
                    importCsIdList += this.value + ",";
                });
                importCsIdList = importCsIdList.substr(0, importCsIdList.length - 1);
            }
        }else{
            importCsIdList  = $('#ImportMemberName option:selected').val();
        }

        return $.extend({}, {
            moumouName:  $('#CSName').combo('getText'),
            csIdList : csIdList,
            importCsIdList : importCsIdList,
            status:$('#MemberStatus option:selected').val(),
            startTime:$('#startTime').val(),
            endTime:$('#endTime').val(),
            pageSize:self._tmpdata.currentPageSize,
            pageNo:self._tmpdata.currentPageNo
        });
    },

};

$(function () {
    CSAccountManager.init();
});
