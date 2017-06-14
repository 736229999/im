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

function showInfo(content) {
    $.messager.alert('错误', '操作失败:' + content, 'error');
}

function onChangeType(){
    var isCustomerService = $('#type option:selected').val();
    if(isCustomerService == 0){
        $('#serviceName').hide();
        $('#storeNameLi').show();
    } else {
        $('#serviceName').show();
        $('#storeNameLi').hide();
    }
}

var HistoryChatMessageManager = {
    _timeDelta: 90 * 24 * 60 * 60 * 1000,
    _emotionPath: 'http://merchant.feiniu.com/webim/',
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

            var pagination = $('#chatMessageTable').datagrid('getPager');
            pagination.pagination({
                onSelectPage: function (pageNumber, pageSize) {
                    self._params.pageSize = pageSize;
                    self._params.currentPageNo = pageNumber;

                    if (self.checkParams()) {
                        self.searchHistoryMessageList(function (pager) {
                            var rowData = transformResultToDatagridData(pager,self._emotionPath);
                            self.renderTable(rowData);
                        });
                    }
                }
            })
        })();

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

        $('#chatMessageTable').height(docHeight - searchBarHeight - conditionHeight - panelHeaderHeight - 50);
        $('#startTime').click(function () {
            WdatePicker({dateFmt: 'yyyy-MM-dd HH:mm:ss',startDate:'%y-%M-%d 00:00:00', minDate: '%y-#{%M-3}-%d}', maxDate: '%y-%M-%d'});
        });

        $('#endTime').click(function () {
            WdatePicker({dateFmt: 'yyyy-MM-dd HH:mm:ss',startDate:'%y-%M-%d 23:59:59', minDate: '%y-#{%M-3}-%d', maxDate: '%y-%M-#{%d+1}'});
        });

    },
    bindUI: function () {
        var self = this;
        var customerLabel = $('#customerIDLabel');
        var customerID = $('#customerID');
        var subMerchantNameLabel = $('#subMerchantNameLabel');
        var subMerchantName = $('#subMerchantName');
        customerID.on('keyup', function () {
            if (customerID.val() != '') {
                customerLabel.addClass('hide-placeholder');
            } else {
                customerLabel.removeClass('hide-placeholder');
            }
        });

        subMerchantName.on('keyup', function () {
            if (subMerchantName.val() != '') {
                subMerchantNameLabel.addClass('hide-placeholder');
            } else {
                subMerchantNameLabel.removeClass('hide-placeholder');
            }
        });

        /***
         * 搜索按钮
         */
        $('#search').click(function () {
            if (!self.checkParams()) {
                return;
            }

            self.searchHistoryMessageList(function (pager) {
                var rowData = transformResultToDatagridData(pager,self._emotionPath);
                self.renderTable(rowData);
            })
        });

        /***
         * 重置按钮
         */
        $('#resetCondition').click(function () {
            $('#customerID, #subMerchantName, #startTime, #endTime').val('');
            $('#storeName').combo('clear');
        });

        /**
         * 清空按钮
         */
        $('#clearTime').click(function () {
            $('#startTime, #endTime').val('');
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
            }
        });
    },
    checkParams: function () {
        var isValid = true;

        var isCustomerService = $('#type option:selected').val();
        if(isCustomerService == '0'){
           if($('#storeName').combo('getText') == ""){
               isValid = false;
               showInfo('请输入店铺名称');
           }
        }

        $('.searchbar .easyui-validatebox, .searchbar .dateSettingTxt').each(function () {
            var $this = $(this);
            var isValid0 = false;

            if ($this.hasClass('easyui-validatebox')) {
                isValid0 = $this.validatebox('isValid');
            } else if ($this.hasClass('easyui-combobox')) {
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
            if ($('#startTime').val() > $('#endTime').val()) {
                isValid = false;
                showInfo('止时间不能早于起时间');
            }

            if ($('#startTime').val() == $('#endTime').val()) {
                isValid = false;
                showInfo('起止时间不能相同');
            }
        }

        return isValid;
    },
    getParams: function () {
        return $.extend({}, {
            dirctorID:$('tempdata').data('dirctorid'),
            isCustomerService:$('#type option:selected').val(),
            customerID: $('#customerID').val(),
            subMerchantNamePart: $('#subMerchantName').val(),
            storeName:$('#storeName').combo('getText'),
            startTime: parseInt(generalUtil.getUTCtime($('#startTime').val()) / 1000),
            endTime: parseInt(generalUtil.getUTCtime($('#endTime').val()) / 1000)
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
    searchSubMerchantNameCompleteList: function (value, callback) {
        $.post(
            '../monitor/get-customer-service-info',
            {
                dirctorID:$('tempdata').data('dirctorid'),
                subMerchantNamePart: value
            },
            function (resultInfo) {
                if (resultInfo && !!resultInfo.success) {
                    callback.call(null, resultInfo.data.subMerchantInfoPairList);
                } else {
                    showInfo((resultInfo || {}).message || '服务器发生故障，请稍后再试');
                }
            }
        )
    },
    searchHistoryMessageList: function (callback) {
        var self = this;
        var params = self.getParams();

        if(!self.checkParams() )
            return;

        $.post('../monitor/get-history-message-list',
            params,
            function (resultInfo) {
                if (resultInfo && !!resultInfo.success) {
                    if ($.isFunction(callback)) {
                        callback.call(self, resultInfo.data.pager);
                    }
                } else {
                    showInfo((resultInfo || {}).message || '服务器发生故障，请稍后再试');
                }
            },
            'json');
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
    HistoryChatMessageManager.init();
});
