/**
 * Created by wangchen on 2015/2/7.
 */
var serviceGroup = (function () {
    var _merchantDataManager = {
        subAccountList: [],
        subAccountMap: {},
        merchantid: null,
        storeInfo: null,
        pageInitialServiceGroupList: [],
        mappingForShowPlace: {}
    };

    var mappingForGroupShowPlaceGenerator = function () {
        return {
            mappingID: 0,
            serviceGroup: {
                groupID: -1
            },
            groupServicePlaceConfig: {
                configID: -1
            }
        };
    }

    var serviceGroupLiTemplate = '<li><a href="#" id="selGroup$groupID" data-groupid="$groupID">$groupName</a></li>';
    var subAccountItemTrTemplate =
        '<tr id="" data-subaccountid="$subAccountID"> ' +
        '<td><input type="checkbox" class="commonCheckBox"/> <a href="javascript:;">$subAccountName</a></td> ' +
        '<td><a type="button" class="td-btn start $diverseOn" title="未分流"></a><a type="button" class="$diverseOff td-btn stop" title="已分流"></a></td> ' +
        '</tr>';
    var subAccountItemDialogTrTemplate =
        '<tr id="dialogSubAccount$subAccountID" data-subaccountid="$subAccountID">' +
        '<td><input type="checkbox" class="zzhCommonCheck"><a>$subAccountName</a></td>' +
        '<td>暂无分组</td>' +
        '<td>客服</td>' +
        '</tr>';

    var treeManager = function () {
        var tree = null;
        return {
            initTree: function (initialTreeData, nodeSelectFn) {
                tree = new LeftTree($('#j_left_tree'), {
                    data: initialTreeData,
                    onSelect: function (node) {
                        var target = $(node.target);
                        if (!!nodeSelectFn && $.isFunction(nodeSelectFn)) {
                            nodeSelectFn.call(null, target, target.data('properties'));
                        }
                    }
                }, {
                    onAdd: function (data) {
                        data.callback({
                            flag: true,
                            node: {
                                id: new Date().getTime(),
                                pid: data.node && data.node.id,
                                name: data.name,
                                attribute: {
                                    addable: false,
                                    editable: true,
                                    removeable: true
                                }
                            }
                        });
                    },
                    onEdit: function (data) {
                        data.callback({
                            flag: true,
                            node: {
                                id: data.node.id,
                                pid: data.node.pid,
                                name: data.name,
                                attribute: data.node.attribute
                            }
                        });
                    },
                    onRemove: function (data) {
                        var flag = $(this).tree('isLeaf', data.node.target);

                        data.callback({
                            flag: flag
                        });
                    }
                });

                tree.init();
            },
            addNode: function (properties) {
                tree.addNode(null, {
                    onAdd: function (data) {
                        $.ajax({
                            async: false,
                            dataType: 'json',
                            type: 'post',
                            url: 'add-group',
                            data: data,
                            success: function (result) {
                                if (!!result && !!result.success) {
                                    data['groupID'] = result.data.groupID;
                                    $('#selectServiceGroupList').append(
                                        serviceGroupLiTemplate.replace(/\$groupID/g, data['groupID']).
                                            replace("$groupName", data['groupName'])
                                    );
                                    $('#editFZFL').removeClass('disable');
                                } else {
                                    throw {
                                        message: '请求处理过程中发生错误' + result.message
                                    }
                                }
                            },
                            error: function () {
                                throw {
                                    message: '请求发生过程中发生错误'
                                };
                            }
                        });
                    },
                    onRemove: function (data) {
                        var selSubAccountListUnderGroup = $('#selSubAccountListUnderGroup>tr');
                        if (selSubAccountListUnderGroup.length > 0) {
                            throw {
                                message: "该分组下还有子账号信息，请先将该分组下的子账号移动到其它分组后再删除。"
                            };
                        } else {
                            $.ajax({
                                async: false,
                                dataType: 'json',
                                type: 'post',
                                url: 'remove-group',
                                data: data,
                                success: function (result) {
                                    if (!!result && !!result.success) {
                                        $('#selGroup' + data['groupID']).remove();
                                        if ($('#j_left_tree li').length == 1) {
                                            $('#returnFZFL').trigger('click');
                                            $('#editFZFL').addClass('disable');
                                        }
                                    } else {
                                        throw {
                                            message: '请求处理过程中发生错误' + result.message
                                        }
                                    }
                                },
                                error: function () {
                                    throw {
                                        message: '请求发生过程中发生错误'
                                    };
                                }
                            });
                        }
                    },
                    onRename: function (data) {
                        $.ajax({
                            async: false,
                            dataType: 'json',
                            type: 'post',
                            url: 'rename-group',
                            data: data,
                            success: function (result) {
                                if (!!result && !!result.success) {
                                    $('#selGroup' + data['groupID']).text(data['groupName']);
                                } else {
                                    throw {
                                        message: '请求处理过程中发生错误' + result.message
                                    }
                                }
                            },
                            error: function () {
                                throw {
                                    message: '请求发生过程中发生错误'
                                };
                            }
                        });
                    }
                }, properties);
            }
        };
    }();

    var dialogManager = (function () {
        var kefuDialog;
        var dialogOptions;

        return {
            init: function (options) {
                dialogOptions = options;
                this.initUI();
                this.bindUI();
            },
            initUI: function () {
                this.initDialog();
                !!dialogOptions.subAccountList && this.initPanel(dialogOptions);
            },
            initDialog: function () {
                kefuDialog = new FNX.CMP.Dialog.ConfirmBox({
                    title: '添加客服对话框',
                    message: $('#alert-dialog-tpl').html(),
                    onConfirm: function () {
                        var panel = $('#subAccountListPanelInDialog');
                        var arrangedSubAccountList = [];

                        panel.find('input[type="checkbox"]:checked:not(:disabled)').each(function () {
                            var currentAccountID = $(this).closest('tr').data('subaccountid');
                            !!currentAccountID && arrangedSubAccountList.push(currentAccountID);
                        });

                        try {
                            if (arrangedSubAccountList.length > 0
                                && !!dialogOptions.submitFn && dialogOptions.submitFn) {
                                dialogOptions.submitFn.call(null, arrangedSubAccountList);

                                alert('添加成功');
                                this.hide();
                            } else {
                                this.hide();
                            }

                        } catch (exception) {
                            alert(exception.message);
                        }
                    }
                }).render();
            },
            initPanel: function () {
                var subAccountList = dialogOptions.subAccountList;
                var panel = $('#subAccountListPanelInDialog');
                var toAppend = '';

                for (var i = 0; i < subAccountList.length; i++) {
                    var currentSubAccount = subAccountList[i];
                    toAppend += subAccountItemDialogTrTemplate.replace(/\$subAccountID/g, currentSubAccount.memberId).
                        replace(/\$subAccountName/g, currentSubAccount.username);
                }
                panel.empty();
                panel.append(toAppend);
            },
            bindUI: function () {
                $(document).on('click', '#addSubAccountToGroup', function () {
                    if (dialogOptions.refreshFn && $.isFunction(dialogOptions.refreshFn)) {
                        dialogOptions.refreshFn.call(null, function (arrangedMappingList, currentGroup) {
                            if (!!arrangedMappingList) {
                                dialogOptions['currentGroup'] = currentGroup;
                                var trElement = $('#subAccountListPanelInDialog tr>td:first-child input').prop({
                                    checked: false,
                                    disabled: false
                                });
                                for (var i = 0; i < arrangedMappingList.length; i++) {
                                    var currentMapping = arrangedMappingList[i];
                                    var trElement = $('#subAccountListPanelInDialog #dialogSubAccount' + currentMapping.subAccountID);
                                    trElement.find('td:first-child input').prop({
                                        checked: true,
                                        disabled: true
                                    });
                                    trElement.children('td:first-child+td').text(currentMapping.serviceGroup.groupName);
                                }
                            }

                            kefuDialog && kefuDialog.show();
                        });
                    }
                });

                $(".aShowSelectBox").click(function () {
                    $(this).next().slideDown(200, function () {
                        var $target = $(this);

                        $(document).bind('click.structure', function () {
                            setTimeout(function () {
                                $(document).unbind('.structure');
                                $target.hide();
                            }, 0);
                        });
                    });
                });

                $(document).on('click', '#dialogCheckAllSubAccountListController', function () {
                    var $this = $(this);
                    $('#subAccountListPanelInDialog tr>td:first-child input:checkbox:not(:disabled)').each(function () {
                        $(this).prop('checked', $this.prop('checked'));
                    });
                });
            }
        };
    })();

    return appManager = {
        stateData: {
            currentSelectGroup: {},
            currentSelectSubAccountList: []
        },
        init: function (options) {
            this.initData(options);
            this.initUI();
            this.bindUI();
        },
        initData: function (options) {
            $tempdata = $('tempdata');
            _merchantDataManager.merchantid = $tempdata.data('merchantid');
            _merchantDataManager.storeInfo = $tempdata.data('storeinfo') || {storeName:''};
            _merchantDataManager.pageInitialServiceGroupList = $tempdata.data('servicegrouplist') || [];

            var allSubAccountList = $tempdata.data('subaccountlist');
            for (var i = 0; i < allSubAccountList.length; i++) {
                var currentSubAccount = allSubAccountList[i];

                if (currentSubAccount.memberType == 2) {
                    _merchantDataManager.subAccountList.push(currentSubAccount);
                    _merchantDataManager.subAccountMap[currentSubAccount["memberId"]] = currentSubAccount;
                }
            }

            var mappedShowPlace = $tempdata.data('showplacemapping') || [];
            for (var i = 0; i < mappedShowPlace.length; i++) {
                var currentMapping = mappedShowPlace[i];
                var placeID = currentMapping.groupServicePlaceConfig.configID;
                _merchantDataManager.mappingForShowPlace[placeID]
                    = currentMapping;
                $('#place' + placeID).data({
                    mappingid: currentMapping.mappingID,
                    serviceGroupID: currentMapping.serviceGroup.groupID
                }).find('td:last-child').text(currentMapping.serviceGroup.groupName);
            }

            $tempdata.remove();
        },
        initUI: function () {
            this.initTree();
            this.initDialog();
        },
        initTree: function () {
            var self = this;
            var selectGroupNodeFn = function (target, properties) {
                self.stateData.currentSelectGroup = $.extend({}, properties);
                self.retrieveSubAccountListUnderGroup(self.stateData.currentSelectGroup.groupID);

                $('#selectServiceGroupList li').show();
                $('#selGroup' + properties.groupID).closest('li').hide();
                $('#showPlaceConfigTable tr > td:first-child>input').prop('checked', false);
                self.groupShowPlacesSwitch();
            };

            var treeData = [];
            for (var i = 0; i < _merchantDataManager.pageInitialServiceGroupList.length; i++) {
                var currentGroup = _merchantDataManager.pageInitialServiceGroupList[i];

                (function (i, currentGroup) {
                    treeData.push({
                        id: i,
                        pid: -1,
                        name: currentGroup.groupName,
                        attribute: {addable: false, editable: true, removeable: true}
                    })
                })(i, currentGroup);
            }
            treeManager.initTree(treeData, selectGroupNodeFn);

            $('#j_left_tree li>div.tree-node').each(function (index) {
                var $this = $(this);
                var currentGroup = _merchantDataManager.pageInitialServiceGroupList[index];

                $this.data({
                    callbacks: {
                        onRemove: function (data) {
                            var selSubAccountListUnderGroup = $('#selSubAccountListUnderGroup>tr');
                            if (selSubAccountListUnderGroup.length > 0) {
                                throw {
                                    message: "该分组下还有子账号信息，请先将该分组下的子账号移动到其它分组后再删除。"
                                };
                            } else {
                                $.ajax({
                                    async: false,
                                    dataType: 'json',
                                    type: 'post',
                                    url: 'remove-group',
                                    data: data,
                                    success: function (result) {
                                        if (!!result && !!result.success) {
                                            $('#selGroup' + data['groupID']).remove();
                                            if ($('#j_left_tree li').length == 1) {
                                                $('#returnFZFL').trigger('click');
                                                $('#editFZFL').addClass('disable');
                                            }
                                        } else {
                                            throw {
                                                message: '请求处理过程中发生错误' + result.message
                                            }
                                        }
                                    },
                                    error: function () {
                                        throw {
                                            message: '请求发生过程中发生错误'
                                        };
                                    }
                                });
                            }
                        },
                        onRename: function (data) {
                            $.ajax({
                                async: false,
                                dataType: 'json',
                                type: 'post',
                                url: 'rename-group',
                                data: data,
                                success: function (result) {
                                    if (!!result && !!result.success) {
                                        $('#selGroup' + data['groupID']).text(data['groupName']);
                                    } else {
                                        throw {
                                            message: '请求处理过程中发生错误' + result.message
                                        }
                                    }
                                },
                                error: function () {
                                    throw {
                                        message: '请求发生过程中发生错误'
                                    };
                                }
                            });
                        }
                    },
                    properties: {
                        merchantID: _merchantDataManager.merchantid,
                        groupName: currentGroup.groupName,
                        lastName: currentGroup.groupName,
                        groupID: currentGroup.groupID
                    }
                });
                if (index === 0) {
                    $('#editFZFL').removeClass('disable');
                    $this.trigger('click');
                }
            });
        },
        initDialog: function () {
            var self = this;

            var subAccountList = [];
            for (var i = 0; i < _merchantDataManager.subAccountList.length; i++) {
                var currentSubAccount = $.extend({}, _merchantDataManager.subAccountList[i]);
                currentSubAccount['username'] = _merchantDataManager.storeInfo['storeName'] + '--' + currentSubAccount['username'];
                subAccountList.push(currentSubAccount);
            }

            dialogManager.init({
                subAccountList: subAccountList,
                submitFn: function (addedSubAccounts) {
                    self.addSubAccount.call(self, addedSubAccounts);
                },
                refreshFn: function (arrangedAccountList) {
                    return self.retrieveArrangedAccountList.call(self, arrangedAccountList);
                }
            });
        },
        bindUI: function () {
            var self = this;

            /**新建客服分组*/
            $('.newDepartment').on('click', function () {
                treeManager.addNode(
                    {
                        merchantID: _merchantDataManager.merchantid,
                        groupName: '新亮灯分组',
                        lastName: '新亮灯分组',
                        groupID: -1
                    });
            });

            $(document).on('click', '#removeSubAccountFromGroup', function () {
                var checkedSubAccountList = $('#selSubAccountListUnderGroup tr>td:first-child input:checkbox:checked');

                if (checkedSubAccountList.length == 0) {
                    alert('请选择要移除的子账号');
                    return;
                }

                var subAccountList = [];
                checkedSubAccountList.each(function () {
                    subAccountList.push($(this).closest('tr').data('subaccountid'));
                });

                self.removeSubAccountList(subAccountList, function () {
                    checkedSubAccountList.each(function () {
                        $(this).closest('tr').remove();
                    });
                });
            });

            $(document).on('click', '#selectServiceGroupList li:not(:first-child) a', function () {
                var checkedSubAccountList = $('#selSubAccountListUnderGroup tr>td:first-child input:checkbox:checked');

                if (checkedSubAccountList.length == 0) {
                    alert('请选择要转移的子账号');
                    return;
                }

                var $this = $(this);
                var subAccountList = [];
                checkedSubAccountList.each(function () {
                    subAccountList.push($(this).closest('tr').data('subaccountid'));
                });
                var destGroupID = $this.data('groupid');

                self.transferSubAccountList(subAccountList, destGroupID, function () {
                    checkedSubAccountList.each(function () {
                        $(this).closest('tr').remove();
                    });
                });
            });

            $(document).on('click', '#checkAllSubAcccountController', function () {
                var $this = $(this);

                $('#selSubAccountListUnderGroup tr>td:first-child input:checkbox').each(function () {
                    $(this).prop('checked', $this.prop('checked'));
                });
            });

            $('#selSubAccountListUnderGroup').on('click', 'tr>td>a.start, tr>td>a.stop', function () {
                var $this = $(this);
                var isDiverseOn = $this.hasClass('stop');
                var subAccountID = $(this).closest('tr').data('subaccountid');

                self.updateDiverseStateForSubAccount(subAccountID, isDiverseOn, function () {
                    $this.addClass('hide').siblings().removeClass('hide')
                });
            });

            $('#returnFZFL').click(function () {
                var showPlaceConfigList = [];
                var checkedShowPlaceList = $('#showPlaceConfigTable>tbody>tr>td:first-child>input:checkbox:checked');

                checkedShowPlaceList.each(function () {
                    var $this = $(this);
                    var parentTr = $this.closest('tr');

                    var currentShowPlaceMapping = mappingForGroupShowPlaceGenerator();
                    currentShowPlaceMapping.serviceGroup.groupID = self.stateData.currentSelectGroup.groupID;
                    currentShowPlaceMapping.groupServicePlaceConfig.configID = parseInt(parentTr.data('placeid'));
                    currentShowPlaceMapping.mappingID = parseInt(parentTr.data('mappingid'));

                    showPlaceConfigList.push(currentShowPlaceMapping);
                });

                self.saveShowPlaceSettings(showPlaceConfigList, function (updatedMapping, serviceGroupID) {
                    $('#showeditFZFL').hide();
                    $('#showListFZFL').show();

                    checkedShowPlaceList.each(function () {
                        var $this = $(this);
                        $this.closest('td').next('td').text(self.stateData.currentSelectGroup.groupName);
                    });

                    /**第一步：删除更新前 当前客服分组对应的显示区域**/
                    if (!!serviceGroupID){
                        $('#showPlaceConfigTable tr').each(function () {
                            var $this = $(this);
                            if ($this.find('td:first-child>input:not(:checked)').length == 1
                                && $this.data('serviceGroupID') == serviceGroupID){
                                $this.data({
                                    mappingid: 0,
                                    serviceGroupID: null
                                }).find('td:last-child').text('暂无分组');
                            }
                        });
                    }

                    /**第二步：更新当前客服分组对应的显示区域*/
                    for (var i = 0; i < updatedMapping.length; i++) {
                        var currentMapping = updatedMapping[i];
                        $('#place' + currentMapping.groupServicePlaceConfig.configID).data({
                            mappingid: currentMapping.mappingID,
                            serviceGroupID: self.stateData.currentSelectGroup.groupID
                        });
                    }

                    self.groupShowPlacesSwitch();
                });
            });

            $('#editFZFL').click(function (e) {
                if ($(this).hasClass('disable')) {
                    e.preventDefault();
                    return false;
                } else {
                    $('#showListFZFL').hide();
                    $('#showeditFZFL').show();
                }
            });

            $('#subAccountShowFilterOptions').on('click', 'li', function () {
                var optionID = $(this).attr('id');

                $('#selSubAccountListUnderGroup>tr>td:nth-child(2)>a:not(.hide)').each(function () {
                    var $this = $(this);
                    var row = $this.closest('tr');

                    switch (optionID) {
                        case 'filterNone':
                            row.removeClass('hide');
                            break;
                        case 'filterByOnline':
                            if ($(this).hasClass('start')) {
                                row.addClass('hide');
                            } else {
                                row.removeClass('hide');
                            }
                            break;
                        case 'filterByOffline':
                            if ($(this).hasClass('stop')) {
                                row.addClass('hide');
                            } else {
                                row.removeClass('hide');
                            }
                            break;
                    }
                    ;
                });
            });

            $(document).on('click', '#allcheck', function () {
                var $this = $(this);
                var hasCheckedBefore = $this.hasClass('checked-before');
                var placeShowConfigTable = $('#showPlaceConfigTable');

                if (hasCheckedBefore) {
                    placeShowConfigTable.find('tr>td:first-child>input[type=checkbox]').prop('checked', false);
                }else {
                    placeShowConfigTable.find('tr>td:first-child>input[type=checkbox]').prop('checked', true);
                }

                $this.toggleClass('checked-before');
            });
        },
        groupShowPlacesSwitch: function () {
            var self = this;
            var toAppend = '';
            var showPlaceTemplate = '<li><a id="showPlace1">$index、$place</a></li>';
            var index = 0;

            $('#showPlaceConfigTable>tbody>tr').each(function () {
                var $this = $(this);
                if ($this.data('serviceGroupID') == self.stateData.currentSelectGroup.groupID) {
                    toAppend += showPlaceTemplate.replace(/\$index/g, ++index).
                        replace(/\$place/g, $this.children('td:first-child').text());
                }
            });

            $('#showListFZFL ul.setting_ul').empty().append(toAppend);
        },
        retrieveSubAccountListUnderGroup: function (groupID) {
            $.post(
                '../service-group-mapping/get-subaccounts-under-group',
                {
                    groupID: groupID
                },
                function (result) {
                    if (!result && !result.success()) {
                        alert(result.message || '服务器发生故障，请重试');
                    } else {
                        var subAccountMappingList = result.data.subAccountMappingList;
                        if (!!subAccountMappingList) {
                            var currentSubAccountListContainer = $('#selSubAccountListUnderGroup');
                            currentSubAccountListContainer.empty();

                            var toAppend = '';
                            for (var i = 0; i < subAccountMappingList.length; i++) {
                                var currentSubAccountMapping = subAccountMappingList[i];
                                var currentSubAccountID = currentSubAccountMapping.subAccountID;
                                var currentSubAccountInfo = _merchantDataManager.subAccountMap['' + currentSubAccountID];
                                var isDiverseOn = currentSubAccountMapping.diverseState == '1';

                                toAppend +=
                                    subAccountItemTrTemplate.replace(/\$subAccountName/g, _merchantDataManager.storeInfo.storeName + "--" +
                                    currentSubAccountInfo.username).replace(/\$subAccountID/g, currentSubAccountInfo.memberId).
                                        replace(/\$diverseOff/g, isDiverseOn ? '' : 'hide').
                                        replace(/\$diverseOn/g, isDiverseOn ? 'hide' : '');
                            }

                            $('#selectServiceGroupList li:first-child a').trigger('click');
                            currentSubAccountListContainer.append(toAppend);
                        }
                    }
                },
                'json'
            )
        },
        retrieveArrangedAccountList: function (successCallback) {
            $.ajax({
                url: '../service-group-mapping/get-categorized-subaccounts.htm',
                data: {
                    merchantID: _merchantDataManager.merchantid
                },
                type: 'post',
                dataType: 'json',
                async: 'false',
                success: function (result) {
                    if (!result || !result.success) {
                        alert(result.message);
                    } else {
                        successCallback.call(this,
                            result.data.categorizedSubAccountList, this.stateData.currentSelectGroup);
                    }
                },
                context: this
            });
        },
        addSubAccount: function (arrangedAccountList) {
            if (!!arrangedAccountList && arrangedAccountList.length > 0) {
                $.post(
                    '../service-group-mapping/add-accounts-to-group.htm',
                    {
                        groupID: this.stateData.currentSelectGroup.groupID,
                        subAccountListString: arrangedAccountList.join(';')
                    },
                    function (result) {
                        if (!result || !result.success) {
                            throw {
                                message: '服务器发生故障，请稍后重试'
                            }
                        } else {
                            $('#j_left_tree>ul>li>.tree-node-selected').trigger('click');
                            var showListFZFL = $('#showListFZFL>.setting_ul>li');
                            if (showListFZFL.length == 0) {
                                $('#setServiceGroupShowPlaceNav').trigger('click');
                                $('#editFZFL').trigger('click');
                            }
                        }
                    },
                    'json'
                )
            }
        },
        removeSubAccountList: function (subAccountList, successCallback) {
            $.post(
                '../service-group-mapping/remove-accounts-from-group',
                {
                    subAccountListString: subAccountList.join(';'),
                    groupID: this.stateData.currentSelectGroup.groupID
                },
                function (result) {
                    if (!result || !result.success) {
                        alert(result.message || '服务器发生故障');
                    } else {
                        successCallback.call(null);
                    }
                },
                'json'
            )
        },
        transferSubAccountList: function (subAccountList, destGroupID, successCallback) {
            $.post(
                '../service-group-mapping/transfer-accounts-to-group',
                {
                    subAccountListString: subAccountList.join(';'),
                    groupID: destGroupID
                },
                function (result) {
                    if (!result || !result.success) {
                        alert(result.message || '服务器发生故障');
                    } else {
                        successCallback.call(null);
                    }
                },
                'json'
            )
        },
        updateDiverseStateForSubAccount: function (subAccountID, isDiverseOn, successCallback) {
            $.post(
                '../service-group-mapping/update-diverse-state-for-account',
                {
                    subAccountID: subAccountID,
                    diverseState: isDiverseOn ? '0' : '1'
                },
                function (result) {
                    if (!result || !result.success) {
                        alert(result.message || '服务器发生故障');
                    } else {
                        successCallback.call(null);
                    }
                },
                'json'
            )
        },
        saveShowPlaceSettings: function (showPlaceList, callback) {
            var self = this;

            $.post(
                '../service-group-mapping/update-show-place-mapping-for-service-group/',
                {
                    showPlaceJson: JSON.stringify(showPlaceList),
                    serviceGroupID: self.stateData.currentSelectGroup.groupID
                },
                function (result) {
                    if (!result || !result.success) {
                        alert((result && result.message) || '服务器发生故障');
                    } else {
                        callback.call(null, (result.data && result.data.updatedMapping) || [], result.data.serviceGroupID);
                    }
                },
                'json'
            )
        }
    };
})();

$(function () {
    serviceGroup.init();
});