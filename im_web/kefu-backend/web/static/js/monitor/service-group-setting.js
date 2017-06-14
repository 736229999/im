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
var serviceGroupSetting = {

    _dataManager: {
        needRefreashStep1: false,
        currentGroupId: "",
        orgnNameList: null,
        needShowEmptyInfo: true,
        currentDeleteId: ""
    },
    _params: {
        pageSize: 10,
        currentPageNo: 0,
        needUpdateDescription: false,
        needAddGroup: false
    },

    init: function () {
        this.initUI();
        this.initData();
        this.bindUI();
        this.searchButtonFunction();
    },

    bindUI: function () {
        var self = this;
        //set tab Columns
        $('#groupSettingTab').datagrid({
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
                    field: 'opt', title: '操作', width: '10%', align: 'center',
                    formatter: function (value, rec, index) {
                        var btn1 = '<a  style="color:#256fb0;  text-decoration:none;cursor: pointer;" class="modifyButton" value="修改"' + "index=" + rec.groupId
                            + ' indexId=' + index + '>[修改]</a>';
                        var btn2 = '<a  style="color:#256fb0;  text-decoration:none;cursor: pointer;" class="deleteButton" value="删除"' + "index=" + rec.groupId
                            + ' indexId=' + index + '>[删除]</a>';
                        var blank = '<label >&nbsp</label>';
                        return btn1 + blank + btn2;
                    }
                },
                {field: 'groupName', title: '分组名称', width: '20%', align: 'center', sortable: false},
                {field: 'groupDescription', title: '分组描述', width: '20%', align: 'center', sortable: false},
                {field: 'embedSet', title: '布点位置', width: '10%', align: 'center', sortable: false},
                {field: 'lastModifiedName', title: '最后修改人', width: '10%', align: 'center', sortable: false},
                {field: 'lastModifiedTime', title: '最后修改时间', width: '20%', align: 'center', sortable: false},
                {
                    field: 'optLook', title: '组内客服', width: '10%', align: 'center',
                    formatter: function (value, rec) {
                        var btn1 = '<a class="c_bule look" href="#" style="color:#256fb0;  text-decoration:none;"' + "index=" + rec.groupId + '>查看</a>';
                        return btn1;
                    }
                },
            ]]
        });

        $('#groupSearchTab').datagrid({
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
                {field: 'ck', checkbox: true},
                {field: 'moumouName', title: '账号名', width: '23%', align: 'center', sortable: false},
                {field: 'serviceName', title: '所属员工', width: '23%', align: 'center', sortable: false},
                {field: 'orgnName', title: '所属员工组织', width: '23%', align: 'center', sortable: false},
                {field: 'groupName', title: '所在分组', width: '24%', align: 'center', sortable: false},
            ]]
        });

        $('#groupAddedTab').datagrid({
            nowrap: true,
            striped: true,
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
                {field: 'ck', checkbox: true, width: '10%', title: '全选'},
                {field: 'moumouName', title: '账号名', width: '31%', align: 'center', sortable: false},
                {field: 'serviceName', title: '所属员工', width: '31%', align: 'center', sortable: false},
                {field: 'orgnName', title: '所属员工组织', width: '32%', align: 'center', sortable: false},
            ]]
        });

        $('#lookDetailTab').datagrid({
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
                {field: 'moumouName', title: '账号名', width: '18%', align: 'center', sortable: false},
                {field: 'serviceName', title: '所属员工', width: '18%', align: 'center', sortable: false},
                {field: 'orgnName', title: '所属员工组织', width: '20%', align: 'center', sortable: false},
                {
                    field: 'status', title: '分流状态', width: '18%', align: 'center',
                    formatter: function (value, rec, index) {
                        var status = rec.status;
                        var showinfo = "";
                        if (status == "1") {
                            showinfo = "分流";
                        } else {
                            showinfo = "不分流";
                        }
                        var label = '<label >' + showinfo + '  </label>';
                        return label;
                    }
                },
                {
                    field: 'handle', title: '操作', width: '18%', align: 'center',
                    formatter: function (value, rec, index) {
                        var status = rec.status;
                        var picInfo = "";
                        if (status == "1") {
                            picInfo = "不分流";
                        } else {
                            picInfo = "分流";
                        }
                        var btn1 = '<input type="button" id="newGroup" class="modifyStatus" value= ' + picInfo + " index=" + rec.userId
                            + ' indexId=' + index + ' status=' + status + ' >';
                        return btn1;
                    }
                },

            ]]
        });

        var pagination = $('#groupSettingTab').datagrid('getPager');
        pagination.pagination({
            onSelectPage: function (pageNumber, pageSize) {
                self._params.pageSize = pageSize;
                self._params.currentPageNo = pageNumber;

                if (self.checkParams()) {
                    self.searchSettingList(function (pager) {
                    });
                }
            }
        });

        var pagination = $('#lookDetailTab').datagrid('getPager');
        pagination.pagination({
            onSelectPage: function (pageNumber, pageSize) {
                self._params.pageSize = pageSize;
                self._params.currentPageNo = pageNumber;

                if (self.checkParams()) {
                    self.lookDetail(function (pager) {
                    });
                }
            }
        });

        var pagination = $('#groupSearchTab').datagrid('getPager');
        pagination.pagination({
            onSelectPage: function (pageNumber, pageSize) {
                self._params.pageSize = pageSize;
                self._params.currentPageNo = pageNumber;

                if (self.checkParams()) {
                    self.searchDetailToSearchTab(function (pager) {
                    });
                }
            }
        })

        var pagination = $('#groupAddedTab').datagrid('getPager');
        pagination.pagination({
            onSelectPage: function (pageNumber, pageSize) {
                self._params.pageSize = pageSize;
                self._params.currentPageNo = pageNumber;
                self.searchDetailToAddedTab(function (pager) {
                });

            }
        });

        $(document).on('click', ".look", function () {
            self._params.pageSize = 10;
            self._params.currentPageNo = 1;
            self._dataManager.currentGroupId = $(this).attr("index");
            self.lookDetail(function () {
                $('#lookDetailDialog').window('center');
                $('#lookDetailDialog').dialog('open').dialog('setTitle', '查看分组内客服');
            })
        });

        $(document).on('click', ".modifyButton", function () {
            var rows = $("#groupSettingTab").datagrid("getRows");
            var id = $(this).attr("index");
            var index = self.getRowIndexByID(id, "groupSettingTab");
            var rowdata = rows[index];
            self._dataManager.needRefreashStep1 = false;
            $('#DialogGroupName').val(rowdata.groupName);
            $('#DialogGroupDetail').val(rowdata.groupDescription);
            $('#groupSearchTab').datagrid('loadData', {total: 0, rows: []});
            self._params.pageSize = 10;
            self._params.currentPageNo = 1;
            self._params.needAddGroup = false;
            self._params.needUpdateDescription = false;
            self._dataManager.currentGroupId = $(this).attr("index");
            $('#modifycsDialog').window('center');
            $('#modifycsDialog').window('open').window('setTitle', '添加/修改分组-第一步 设置分组名称');
        });

        $(document).on('click', ".deleteButton", function () {
            self._dataManager.currentDeleteId = $(this).attr("index");
            $('#confirmDeleteWindow').show(function () {
                $(this).window('center').window('open');
            });

        });

        $(document).on('click', ".modifyStatus", function () {
            var indexData = $(this).attr("indexid");
            var userid = $(this).attr("index");
            var status = $(this).attr("status");
            var setStatus = 0;
            if (status == "1")
                setStatus = 0;
            else
                setStatus = 1;

            self.modifyStatus(userid, setStatus, function () {
                var rows = $("#lookDetailTab").datagrid("getRows");
                rows[indexData].status = setStatus.toString();
                $("#lookDetailTab").datagrid("refreshRow", indexData);
            })
        });

        $("#DialogGroupName").change(function () {
            self._params.needUpdateDescription = true;
        });

        $("#DialogGroupDetail").change(function () {
            self._params.needUpdateDescription = true;
        });

        $('#groupName').combobox({
            valueField: 'groupName',
            textField: 'groupName',
            mode: 'remote',
            loader: function (param, success) {
                if (!param.q) {
                    success([]);
                } else {
                    self.searchMachedGroupName(param.q || '', function (data) {
                        success(data);
                    });
                }
            }
        });

        $('#lastModifiedPeopleName').combobox({
            valueField: 'lastModifiedName',
            textField: 'lastModifiedName',
            mode: 'remote',
            loader: function (param, success) {
                if (!param.q) {
                    success([]);
                } else {
                    self.searchMachedLastModifiedPeopleName(param.q || '', function (data) {
                        success(data);
                    });
                }
            }
        });

        //确认删除
        $('#confirmDelete').click(function () {
            var id = self._dataManager.currentDeleteId;
            var index = self.getRowIndexByID(id, "groupSettingTab");
            self.deleteGroup(id, function () {
                $("#groupSettingTab").datagrid("deleteRow", index);
            });
            $('#confirmDeleteWindow').window('close');
        });

        //取消删除
        $('#cancelDelete').click(function () {
            $('#confirmDeleteWindow').window('close');
        });

        //布点设置
        $('#embedSet').click(function () {
            $('#setEmbedWindow').show(function () {
                $(this).window('center').window('open');
            });
        });

        //保存布点设置
        $('#saveEmbedSet').click(function () {
            var embedSetJson = {};
            $('#setEmbedWindow .embed-field').each(function () {
                var $this = $(this);
                embedSetJson[$this.data('id')] = parseInt($this.find('select').val());
            });

            $.ajax({
                type: 'get',
                url: '../serviceGroup/save-embed-sets.do',
                data: {
                    embedSetJson: JSON.stringify(embedSetJson)
                },
                dataType: 'json',
                success: function (resultInfo) {
                    if (resultInfo.success) {
                        window.location.reload();
                    } else {
                        alert(resultInfo.errorMessage || '服务器发生故障');
                    }
                },
                error: function (XMLHttpRequest, textStatus, errorThrown) {
                    if (!!XMLHttpRequest) {
                        //说明正在请求中，可能是外界原因导致中断，如刷新窗口，这种情况忽略之
                        if (XMLHttpRequest.status == 0 && XMLHttpRequest.readyState == 0) {
                            //omit
                        } else {
                            var errorMessage = '网络发生故障，状态码：readyState=' + XMLHttpRequest.readyState + '; status=' + XMLHttpRequest.status;
                            alert(errorMessage);
                        }
                    }
                }
            });
        });

        //取消
        $('#cancelEmbedSet').click(function () {
            $('#setEmbedWindow').window('close');
        });
    },

    getRowIndexByID: function (id, tab) {
        var rows = $("#" + tab).datagrid("getRows");
        for (var i = 0; i < rows.length; i++) {
            if (rows[i].groupId == id) {
                return $("#" + tab).datagrid("getRowIndex", rows[i]);
            }
        }
        return -1;
    },
    deleteGroup: function (groupId, callback) {
        $.post(
            '../serviceGroup/deleteGroup',
            {
                groupId: groupId,
            },
            function (resultInfo) {
                if (resultInfo && !!resultInfo.success) {
                    callback.call();
                } else {
                    showInfo((resultInfo || {}).message || '服务器发生故障，请稍后再试');
                }
            }
        );
    },

    modifyStatus: function (userid, setStatus, callback) {
        var self = this;
        $.post(
            '../serviceGroup/modifyStatus',
            {
                groupId: self._dataManager.currentGroupId,
                userId: userid,
                setStatus: setStatus
            },
            function (resultInfo) {
                if (resultInfo && !!resultInfo.success) {
                    callback.call();
                } else {
                    showInfo((resultInfo || {}).message || '服务器发生故障，请稍后再试');
                }
            }
        );
    },
    searchOrgnName: function (value, callback) {
        $.post(
            '../serviceGroup/get-orgn-name-list',
            {
                orgnNamePart: value
            },
            function (resultInfo) {
                if (resultInfo && !!resultInfo.success) {
                    callback.call(null, resultInfo.data.groupInfoList);
                } else {
                    showInfo((resultInfo || {}).message || '服务器发生故障，请稍后再试');
                }
            }
        );
    },
    searchMachedLastModifiedPeopleName: function (value, callback) {
        $.post(
            '../serviceGroup/get-last-modified-people-list',
            {
                lastModifiedPeopleNamePart: value
            },
            function (resultInfo) {
                if (resultInfo && !!resultInfo.success) {
                    callback.call(null, resultInfo.data.groupInfoList);
                } else {
                    showInfo((resultInfo || {}).message || '服务器发生故障，请稍后再试');
                }
            }
        );
    },
    searchMachedGroupName: function (value, callback) {
        $.post(
            '../serviceGroup/get-group-name-list',
            {
                groupNamePart: value
            },
            function (resultInfo) {
                if (resultInfo && !!resultInfo.success) {
                    callback.call(null, resultInfo.data.groupInfoList);
                } else {
                    showInfo((resultInfo || {}).message || '服务器发生故障，请稍后再试');
                }
            }
        )
    },
    initData: function () {
        var self = this;
        $('#startModifyTime').val("");
        $('#endModifyTime').val("");

        self.searchOrgnName(null, function (list) {
            var obj = document.getElementById('AddOrgnName');
            for (var i = 0; i < list.length; i++) {
                var varItem = new Option(list[i].orgnName, list[i].id);
                obj.options.add(varItem);
            }
        });
    },
    searchButtonFunction: function () {
        var self = this;
        if (!self.checkParams()) {
            return;
        }
        var opts = $('#groupSettingTab').datagrid('options');
        var pagination = $('#groupSettingTab').datagrid('getPager');
        pagination.pagination('refresh', {
            pageNumber: 1,
            pageSize: 10
        });
        self._params.pageSize = 10;
        self._params.currentPageNo = 1;
        self.searchSettingList(function (pager) {
        });
    },
    initUI: function () {
        var self = this;
        $('#startModifyTime').click(function () {
            WdatePicker({dateFmt: 'yyyy-MM-dd', maxDate: '%y-%M-%d '});
        });

        $('#endModifyTime').click(function () {
            WdatePicker({dateFmt: 'yyyy-MM-dd ', maxDate: '%y-%M-%d'});
        });

        /**
         * 清空按钮
         */
        $('#clearModifyTime').click(function () {
            $('#startModifyTime, #endModifyTime').val('');
        });

        /***
         * 重置按钮
         */
        $('#resetCondition').click(function () {
            $('#groupName').combo('clear');
            $('#lastModifiedPeopleName').combo('clear');
            $('#startModifyTime, #endModifyTime').val('');
        });

        /***
         * 搜索按钮
         */
        $('#search').click(function () {
            self.searchButtonFunction();
        });

        $('#Addsearch').click(function () {
            var opts = $('#groupSearchTab').datagrid('options');
            var pagination = $('#groupSearchTab').datagrid('getPager');
            pagination.pagination('refresh', {
                pageNumber: 1,
                pageSize: 10
            });

            self._params.pageSize = 10;
            self._params.currentPageNo = 1;
            self.searchDetailToSearchTab(function (pager) {
            });
        });

        $('#AddresetCondition').click(function () {
            $('#AddServiceName, #AddMoumouName').val('');
            $("#AddOrgnName").find("option[value='0']").attr("selected", true);
        });

        $('#newGroup').click(function () {
            self._params.needAddGroup = true;
            self._params.needUpdateDescription = true;
            self._dataManager.currentGroupId = "-1";
            $('#DialogGroupName, #DialogGroupDetail').val('');
            $('#modifycsDialog').window('center');
            $('#modifycsDialog').window('open').window('setTitle', '添加/修改分组-第一步 设置分组名称');
        });

        $('#saveAndNext').click(function () {
            if ($('#DialogGroupName').val() == '' || $('#DialogGroupDetail').val() == '') {
                showInfo("分组名称或分组描述都不能为空！");
                return;
            }
            var pagination = $('#groupSearchTab').datagrid('getPager');
            pagination.pagination('refresh', {
                pageNumber: 1,
                pageSize: 10
            });
            self._params.pageSize = 10;
            self._params.currentPageNo = 1;
            $('#AddServiceName, #AddMoumouName').val('');
            $('#groupSearchTab').datagrid('loadData', {total: 0, rows: []});
            $('#groupAddedTab').datagrid('loadData', {total: 0, rows: []});
            self.searchDetailToSearchTab(function (pager) {
                self.searchDetailToAddedTab(function () {
                    var rows = $("#groupAddedTab").datagrid("getRows");
                    if (rows == null || rows.length == 0) {
                        $('#EmptyInfo').show();
                    } else {
                        $('#EmptyInfo').hide();
                    }
                });
                $('#modifycsDialog').window('close');
                $('#addServiceDialog').window('center');
                $('#addServiceDialog').dialog('open').dialog('setTitle', '添加/修改分组-第二步 分组客服设置');
            });

        });

        $('#cancelSaveAndNext').click(function () {
            $('#DialogGroupName, #DialogGroupDetail').val('');
            $('#modifycsDialog').window('close');
        });

        $('#cacelModify').click(function () {
            $('#addServiceDialog').dialog('close');
            $('#modifycsDialog').window('center');
            $('#modifycsDialog').window('open').window('setTitle', '添加/修改分组-第一步 设置分组名称');
        });

        $('#addToGroup').click(function () {
            var checkedItems = $('#groupSearchTab').datagrid('getChecked');
            $.each(checkedItems, function (index, item) {
                var b = $('#groupSearchTab').datagrid('getRowIndex', item);
                var rows = $("#groupAddedTab").datagrid("getRows");
                var mfind = 0;
                for (var i = 0; i < rows.length; i++) {
                    if (rows[i].userId == item.userId) {
                        mfind = 1;
                        showInfo(rows[i].moumouName + "已经存在");
                    }
                }
                if (mfind == 0) {
                    $('#groupSearchTab').datagrid('deleteRow', b);
                    $('#groupAddedTab').datagrid('insertRow', {
                        index: 0,
                        row: item
                    });
                }
                ;
                $('#EmptyInfo').hide();
            });
        });

        $('#removeService').click(function () {
            var checkedItems = $('#groupAddedTab').datagrid('getChecked');
            $.each(checkedItems, function (index, item) {
                var b = $('#groupAddedTab').datagrid('getRowIndex', item);
                $('#groupAddedTab').datagrid('deleteRow', b);
            });
        });

        $('#submitModify').click(function () {
            var saveParams = self.getUpdateParams();
            self.saveGroupChange("../serviceGroup/save-group-change", saveParams, function (addedServiceGroupInfo, moditiedTime, moditiedName) {
                $('#addServiceDialog').dialog('close');
                if (addedServiceGroupInfo != null) {
                    $('#groupSettingTab').datagrid('insertRow', {
                        index: 0,
                        row: addedServiceGroupInfo
                    });
                } else {
                    var index = self.getRowIndexByID(self._dataManager.currentGroupId, "groupSettingTab");
                    var rows = $("#groupSettingTab").datagrid("getRows");
                    rows[index].groupName = $('#DialogGroupName').val();
                    rows[index].groupDescription = $('#DialogGroupDetail').val();
                    rows[index].lastModifiedName = moditiedName;
                    rows[index].lastModifiedTime = moditiedTime;

                    $("#groupSettingTab").datagrid("refreshRow", index);
                }
            });
        });

        $('body').css('min-height', $(document).height());
    },

    saveGroupChange: function (url, params, callback) {
        MaskUtil.mask("正在处理请稍后");
        $.post(url,
            params,
            function (resultInfo) {
                MaskUtil.unmask();
                if (resultInfo && !!resultInfo.success) {
                    callback.call(null, resultInfo.data.addedServiceGroupInfo,
                        resultInfo.data.moditiedTime, resultInfo.data.moditiedName);
                } else {
                    if (resultInfo.errorCode == -2) {
                        var list = [];
                        $(tabel).datagrid({
                            data: list,
                            view: myview,
                            emptyMsg: '保存失败'
                        });
                    }
                    else
                        showInfo((resultInfo || {}).message || '服务器发生故障，请稍后再试');
                }
            },
            'json');
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
                    if (self._dataManager.needRefreashStep1) {
                        if (resultInfo.data.name != null)
                            $('#DialogGroupName').val(resultInfo.data.name);
                        if (resultInfo.data.description != null)
                            $('#DialogGroupDetail').val(resultInfo.data.description);
                    }
                    if (data == null) {
                        $(tabel).datagrid('loadData', {
                            total: 0,
                            rows: []
                        });
                    }
                    $(tabel).datagrid('loadData', {
                        total: resultInfo.data.pager.totalRecordSize,
                        rows: data
                    });
                    callback.call();
                } else {
                    if (resultInfo.errorCode == -2) {
                        $(tabel).datagrid('loadData', {
                            total: 0,
                            rows: []
                        });
                        callback.call();
                    }
                    else
                        showInfo((resultInfo || {}).message || '服务器发生故障，请稍后再试');
                }
            },
            'json');
    },

    searchSettingList: function (callback) {
        var self = this;
        var params = self.getParams();
        var url = '../serviceGroup/get-group-list';
        var table = '#groupSettingTab';

        self.searchListCommon(url, params, table, callback);
    },

    lookDetail: function (callback) {
        var self = this;

        var params = $.extend({}, {
            groupId: self._dataManager.currentGroupId,
            pageSize: self._params.pageSize,
            currentPageNo: self._params.currentPageNo
        });

        self._dataManager.needRefreashStep1 = false;
        var url = '../serviceGroup/get-group-detail-list';
        var table = '#lookDetailTab';
        self.searchListCommon(url, params, table, callback);
    },

    searchDetailToSearchTab: function (callback) {
        var self = this;
        var params = self.getDetailSearchParams();
        self._dataManager.needRefreashStep1 = false;
        var url = '../serviceGroup/get-group-detail-list';
        var table = '#groupSearchTab';
        self.searchListCommon(url, params, table, callback);
    },

    searchDetailToAddedTab: function (callback) {
        var self = this;
        var params = $.extend({}, {
            groupId: self._dataManager.currentGroupId
        });
        self._dataManager.needRefreashStep1 = false;
        var url = '../serviceGroup/get-group-detail-list';
        var table = '#groupAddedTab';
        self.searchListCommon(url, params, table, callback);
    },

    getUpdateParams: function () {
        var self = this;
        var rows = $("#groupAddedTab").datagrid("getRows");
        var idList = "";
        for (var i = 0; i < rows.length; i++) {
            idList += rows[i].userId + ",";
        }
        return $.extend({}, {
            groupId: self._dataManager.currentGroupId,
            idList: idList,
            groupDetail: $('#DialogGroupDetail').val(),
            groupName: $('#DialogGroupName').val(),
            needUpdateDescription: self._params.needUpdateDescription,
            needAddGroup: self._params.needAddGroup
        });

    },
    getDetailSearchParams: function () {
        var self = this;
        var obj = document.getElementById('AddOrgnName');
        var index = obj.selectedIndex; //序号，取当前选中选项的序号
        var orgnNameText = obj.options[index].value == "null" ? null : obj.options[index].value;
        return $.extend({}, {
            orgnName: orgnNameText,
            serviceName: $('#AddServiceName').val(),
            moumouName: $('#AddMoumouName').val(),
            pageSize: self._params.pageSize,
            currentPageNo: self._params.currentPageNo
        });
    },

    checkDetailSearchParams: function () {
        if ($('#AddOrgnName').text == "" && $('#AddServiceName').val() == ""
            && $('#AddMoumouName').val() == "") {
            showInfo('查询条件不能全部为空');
            return false;
        }
        return true;
    },

    getParams: function () {
        var self = this;
        return $.extend({}, {
            groupName: $('#groupName').combo('getText'),
            lastModifiedName: $('#lastModifiedPeopleName').combo('getText'),
            startTime: $('#startModifyTime').val(),
            endTime: $('#endModifyTime').val(),
            pageSize: self._params.pageSize,
            currentPageNo: self._params.currentPageNo
        });
    },

    checkParams: function () {
        var isValid = true;

        if ($('#startModifyTime').val() != "" && $('#endModifyTime').val() != "") {
            if (parseInt(generalUtil.getUTCtime($('#startModifyTime').val() + " 00:00:00") / 1000) >= parseInt(generalUtil.getUTCtime($('#endModifyTime').val() + " 00:00:00") / 1000 + 1)) {
                isValid = false;
                showInfo('止时间不能早于起时间');
            }
        }

        return isValid;
    }
}
$(function () {
    serviceGroupSetting.init();
});