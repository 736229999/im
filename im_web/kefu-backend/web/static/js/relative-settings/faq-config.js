/**
 * Author: morningking
 * Date: 2015/9/28
 * Contact: 243717042@qq.com
 */

//相关配置项
var FAQConfig = (function () {
    var baseReqUrl = './';

    return {
        //服务根地址
        getBaseReqUrl: function () {
            return baseReqUrl;
        },
        //常见问题标题最大字符数
        maxTitleCharLength: function () {
            return 30;
        },
        //常见问题描述最大字符数
        maxSummaryCharLength: function () {
            return 200;
        }
    };
})();

//删除调度管理器
var GlobalDeleteScheduler = new GlobalDeleteScheduler();

//主要负责大编辑页面的ui处理
var FAQUIManager = {
    _faqPageOption: {
        currentPageNo: 1,
        pageSize: 10
    },
    _showedFaqPageOption: {
        currentPageNo: 1,
        pageSize: 10
    },
    init: function () {
        this.initUI();
        this.bindUI();
        this.initData();
    },
    initUI: function () {
        var self = this;
        //时间控件初始化
        $('#startTime').click(function () {
            WdatePicker({dateFmt: 'yyyy-MM-dd HH:mm:ss',startDate:'%y-%M-%d 00:00:00', minDate: '%y-%M-#{%d-30}', maxDate: '%y-%M-%d'});
        });
        $('#endTime').click(function () {
            WdatePicker({dateFmt: 'yyyy-MM-dd HH:mm:ss',startDate:'%y-%M-%d 23:59:59', minDate: '%y-%M-#{%d-30}', maxDate: '%y-%M-#{%d+1}'});
        });

        //表格初始化
        $('#faqTable').datagrid({
            nowrap: false,
            width: "auto",
            singleSelect: true,
            height: "auto",
            loadMsg: '数据装载中......',
            fitColumns: true,
            remoteSort: false,
            pagination: true,
            sortable: false,
            columns: [[
                {
                    field: 'faqID',
                    title: '操作',
                    width: '9%',
                    align: 'center',
                    sortable: false,
                    formatter: function (value, row, index) {
                        return '<span style="display: inline-block" data-faqid="' + value + '"><a class="faq-edit">[编辑]</a><a class="faq-delete">[删除]</a></span>';
                    }
                },
                {
                    field: 'isShow',
                    title: '是否显示',
                    width: '4%',
                    align: 'center',

                    sortable: false,
                    formatter: function (value, row, index) {
                        return '<input data-faqid="' + value + '" class="faq-toggle-show" type="checkbox" ' + (value == '1' ? 'checked' : '' ) + '>';
                    }
                },
                {field: 'title', title: '问题', width: '30%', align: 'center', sortable: false},
                {field: 'faqSummary', title: '解释', width: '30%', align: 'center', sortable: false},
                {field: 'viewCount', title: '累计浏览次数', width: '9%', align: 'center', sortable: false},
                {field: 'lastOperator', title: '最后操作人', width: '9%', align: 'center', sortable: false},
                {field: 'updateTime', title: '最后操作时间', width: '9%', align: 'center', sortable: false}
            ]]
        });
        var pagination = $('#faqTable').datagrid('getPager');
        pagination.pagination({
            onSelectPage: function (pageNumber, pageSize) {
                $.extend(self._faqPageOption, {
                    currentPageNo: pageNumber,
                    pageSize: pageSize
                });

                self.searchFAQList(function (list, totalFaqNum, totalShowedFaqNum) {
                    FAQDataGridManager.loadData({
                        total: totalFaqNum,
                        rows: list
                    });
                });
            }
        });
        //已显示表格 初始化
        $('#showedFaqTable').datagrid({
            nowrap: false,
            width: "auto",
            singleSelect: true,
            height: "auto",
            loadMsg: '数据装载中......',
            fitColumns: true,
            remoteSort: false,
            pagination: true,
            sortable: false,
            columns: [[
                {
                    field: 'faqID',
                    title: '操作',
                    width: '5%',
                    align: 'center',
                    sortable: false,
                    formatter: function (value, row, index) {
                        return '<span style="display: inline-block" data-faqid="' + value + '"><a class="faq-cancel-show">[取消展示]</a></span>';
                    }
                },
                {
                    field: 'showOrder',
                    title: '排序',
                    width: '7%',
                    align: 'center',

                    sortable: false,
                    formatter: function (value, row, index) {
                        var start = '<span data-faq-id="' + row.faqID + '">';
                        var end = '</span>';
                        var content = '';
                        var rows = $('#showedFaqTable').datagrid('getRows').length;
                        var showOrder = row.showOrder;

                        if (showOrder == 0) {
                            content =
                                '<a class="move-action move-bottom" title="下移置底"></a>' +
                                '<a class="move-action move-down" title="下移一位"></a>';
                        } else if (showOrder == 9 || showOrder == rows - 1) {
                            content =
                                '<a class="move-action move-top" title="上移置顶"></a>' +
                                '<a class="move-action move-up" title="上移一位"></a>';
                        } else {
                            content =
                                '<a class="move-action move-top" title="上移置顶"></a>' +
                                '<a class="move-action move-up" title="上移一位"></a>' +
                                '<a class="move-action move-bottom" title="下移置底"></a>' +
                                '<a class="move-action move-down" title="下移一位"></a>';
                        }

                        return start + content + end;
                    }
                },
                {field: 'title', title: '问题', width: '30%', align: 'center', sortable: false},
                {field: 'faqSummary', title: '解释', width: '30%', align: 'center', sortable: false},
                {field: 'viewCount', title: '累计浏览次数', width: '9%', align: 'center', sortable: true},
                {field: 'lastOperator', title: '最后操作人', width: '9%', align: 'center', sortable: false},
                {field: 'updateTime', title: '最后操作时间', width: '9%', align: 'center', sortable: false}
            ]]
        });
        var pagination = $('#showedFaqTable').datagrid('getPager');
        pagination.pagination({
            onSelectPage: function (pageNumber, pageSize) {
                $.extend(self._showedFaqPageOption, {
                    currentPageNo: pageNumber || 1,
                    pageSize: pageSize
                });

                self.searchFAQList(function (list, totalFaqNum, totalShowedFaqNum) {
                    ShowedFAQDataGridManager.loadData({
                        total: totalShowedFaqNum,
                        rows: list
                    });
                });
            }
        });

    },
    bindUI: function () {
        var self = this;

        //清空按钮
        $('#clearTime').click(function () {
            $('#startTime, #endTime').val('');
        });
        //查询按钮
        $('#search').click(function () {
            self.searchFAQList(function (list, totalFaqNum, totalShowedFaqNum) {
                var isShowedFAQTableVisible = $('.count-tab>.tab-btn.active').is('.toggle-showed');

                if (isShowedFAQTableVisible) {
                    ShowedFAQDataGridManager.loadData({
                        total: totalShowedFaqNum,
                        rows: list
                    });
                } else {
                    FAQDataGridManager.loadData({
                        total: totalFaqNum,
                        rows: list
                    });
                }
            });
        });
        //重置按钮
        $('#resetCondition').click(function () {
            $('#keyWord, #startTime, #endTime').val('');
        });

        //tab页切换
        $('.count-tab>.tab-btn').click(function () {
            var $this = $(this);
            $this.siblings().removeClass('active');
            $this.addClass('active');

            self.searchFAQList(function (list, totalFaqNum, totalShowedFaqNum) {
                if ($this.hasClass('toggle-showed')) {
                    $('#content .datagrid:first-child').hide();
                    $('#content .datagrid:last-child').show();

                    setTimeout(function () {
                        ShowedFAQDataGridManager.loadData({
                            total: totalShowedFaqNum,
                            rows: list
                        });
                    }, 200);
                } else {
                    $('#content .datagrid:last-child').hide();
                    $('#content .datagrid:first-child').show();

                    setTimeout(function () {
                        FAQDataGridManager.loadData({
                            total: totalFaqNum,
                            rows: list
                        });
                    }, 200);
                }
            });
        });
        //常见问题统计图表
        $('.count-tab>.chart-entrance').click(function () {
            $('body>.chart-mask').show();
            ChartManager.showLoading();

            self.getChartData(function (list) {
                var columnList = [];
                var valueList = [];
                for (var i = 0; i < list.length; i++) {
                    var item = list[i];
                    columnList.push(item.title);
                    valueList.push(item.viewCount);
                }

                var data = {
                    columns: columnList,
                    values: valueList
                };

                if ($('#bar-tab').is('.active')) {
                    ChartManager.showBarChart(data);
                } else {
                    ChartManager.showPieChart(data);
                }
            });
        });
        //关闭图表窗口
        $('#chartCloseBtn').click(function () {
            $('body>.chart-mask').hide();
        });
        //切换按钮
        $('.chart-mask .chart-tab-bar>.chart-tab').click(function () {
            var $this = $(this);
            $this.siblings().removeClass('active');
            $this.addClass('active');
            var id = $this.attr('id');

            if (id == 'bar-tab') {
                $('#faq-chart-pie').hide();
                $('#faq-chart-bar').show();

                ChartManager.showBarChart();
            } else if (id == 'pie-tab') {
                $('#faq-chart-pie').show();
                $('#faq-chart-bar').hide();

                ChartManager.showPieChart();
            }
        });

        //添加常见问题
        $(document).on('click', '#addFAQ', function () {
            FAQEditManager.setFormValue(null);
            $('#faqEditWindow').show(function () {
                $(this).window('center').window('open');
            });
        });
        //导入常见问题
        $('#excelImport').uploader({
            action: FAQConfig.getBaseReqUrl() + "import-faq-list.do",
            //mode: 'flash',
            swf: '/js/uploader/uploader.swf',
            name: 'file',
            auto: 'true',
            fileTypeDesc: '请选择excel表格(.xlsx)',
            fileTypeExts: 'xlsx',
            fileSizeLimit: '2M',
            onError: function (e) {
                AlertUtil.showError(e.message);
            },
            onSuccess: function (e) {
                if (e.data) {
                    var result = eval('(' + e.data + ')');

                    if (result.success) {
                        //FAQEditManager.changeTotalCount(result.data.importCount);
                        $('#search').trigger('click');
                    } else {
                        AlertUtil.showError('服务器发生故障，请稍后重试');
                    }
                }
            }
        });
        //导出常见问题
        $(document).on('click', '#excelExport', function () {
            window.location.href = FAQConfig.getBaseReqUrl() + 'export-faq-excel.xls'
                + ObjectUtil.serializeToFormStyle(self.getSearchParams());
        });

        //确认删除
        $('#confirmDelete').click(function () {
            GlobalDeleteScheduler.run();
        });

        //取消删除
        $('#cancelDelete').click(function () {
            GlobalDeleteScheduler.cancel();
            $('#confirmDeleteWindow').window('close');
        });
    },
    initData: function () {
        this.searchFAQList(function (list, totalFaqNum) {
            $('#content .datagrid:last-child').hide();
            $('#content .datagrid:first-child').show();

            FAQDataGridManager.loadData({
                total: totalFaqNum,
                rows: list
            });
        });
    },

    //获取查询参数
    getSearchParams: function (extraParam) {
        var startTime = parseInt(generalUtil.getUTCtime($('#startTime').val()));
        var endTime = parseInt(generalUtil.getUTCtime($('#endTime').val()));

        var params = $.extend({}, {
            keyWord: $('#keyWord').val()
        });
        !!startTime && (params['startTime'] = startTime);
        !!endTime && (params['endTime'] = endTime);
        var isToggleShowed = $('.count-tab>.tab-btn.active').is('.toggle-showed');
        if (isToggleShowed) {
            params['isShow'] = true;
            $.extend(params, this._showedFaqPageOption);
        } else {
            $.extend(params, this._faqPageOption);
        }

        (extraParam != null) && ($.extend(params, extraParam));

        return params;
    },

    //获取图表显示数据
    getChartData: function (callback) {
        this.searchFAQList(callback, {
            isFetchAll: true,
            isShow: true
        });
    },

    //服务请求
    searchFAQList: function (callback, extraParam) {
        var self = this;

        $.post(
            FAQConfig.getBaseReqUrl() + 'get-faq-list.do',
            self.getSearchParams(extraParam),
            function (resultInfo) {
                if (resultInfo.success) {
                    callback.call(null, resultInfo.data.faqList || {}, resultInfo.data.totalFaqNum, resultInfo.data.totalShowedFaqNum);
                }
            },
            'json'
        );
    }

};

//所有数据 表格相关接口提供者
var FAQDataGridManager = (function () {
    var table = $("#faqTable");

    return {
        getSelectedRow: function () {
            return $.extend({}, table.datagrid('getSelected'));
        },
        getSelectedRowIndex: function () {
            var selectedRow = table.datagrid('getSelected');
            return table.datagrid('getRowIndex', selectedRow);
        },
        insertRow: function (row) {
            table.datagrid('insertRow', {
                index: 0,
                row: row
            });
        },
        //更新选中行
        updateSelectedRow: function (row) {
            var selectedRow = table.datagrid('getSelected');
            var rowIndex = table.datagrid('getRowIndex', selectedRow);

            table.datagrid('updateRow', {
                index: rowIndex,
                row: $.extend(selectedRow, row)
            });
        },
        deleteSelectedRow: function () {
            table.datagrid('deleteRow', this.getSelectedRowIndex());
        },
        loadData: function (info) {
            table.datagrid('loadData', info);
        },
        //更新特定行
        updateGivenRow: function (updatedRow) {
            var rows = table.datagrid('getData').rows;
            var length = rows.length;
            var rowIndex = -1;
            var givenRow = null;

            for (var i = 0; i < length; i++) {
                if (rows[i]['faqID'] == updatedRow.faqID) {
                    rowIndex = i;
                    givenRow = rows[i];
                    break;
                }
            }

            if (rowIndex != -1) {
                table.datagrid('updateRow', {
                    index: rowIndex,
                    row: $.extend(givenRow, updatedRow)
                });
            }
        }
    };
})();
//已展示数据 表格相关接口提供者
var ShowedFAQDataGridManager = (function () {
    var table = $("#showedFaqTable");

    return {
        getSelectedRow: function () {
            return $.extend({}, table.datagrid('getSelected'));
        },
        getSelectedRowIndex: function () {
            var selectedRow = table.datagrid('getSelected');
            return table.datagrid('getRowIndex', selectedRow);
        },
        getGivenRow: function (index) {
            var rows = table.datagrid('getRows');
            if (index >= 0 && !!rows && rows.length > index) {
                return $.extend({}, rows[index]);
            } else {
                return null;
            }
        },
        getCurrentPageRowNo: function () {
            return table.datagrid('getRows').length;
        },
        loadData: function (info) {
            table.datagrid('loadData', info);
        },
        updateSelectedRow: function (row) {
            var selectedRow = table.datagrid('getSelected');
            var rowIndex = table.datagrid('getRowIndex', selectedRow);

            table.datagrid('updateRow', {
                index: rowIndex,
                row: $.extend(selectedRow, row)
            });
        },
        deleteSelectedRow: function () {
            table.datagrid('deleteRow', this.getSelectedRowIndex());
        },
        exchangeOrder: function (param) {
            table.datagrid('updateRow', {
                index: param.srcOrder,
                row: param.destRow
            });

            table.datagrid('updateRow', {
                index: param.destOrder,
                row: param.srcRow
            });
        }
    };
})();

//编辑|添加 常见问题
var FAQEditManager = {
    _editMode: 'add',
    _lastSummaryHrefEle: null,

    init: function () {
        this.initUI();
        this.bindUI();
    },
    initUI: function () {
        $.fn.extend({
            insertAtCaret: function (html) {
                var self = this;
                var ta = $(this)[0];

                ta.focus();
                if (document.selection) {
                    var c = document.selection.createRange();

                    if (c) {
                        if (document.selection.type.toLowerCase() != "none") {
                            document.selection.clear(); //清除选中
                        }
                        c.pasteHTML(html);
                        c.collapse(false);
                        c.select();
                    }
                } else {
                    var lastRange = this.getLastRange();
                    var range = lastRange.range,
                        selection = lastRange.selection;

                    if (!range) {
                        ta.innerHTML += html;
                        self.forceSaveLastRange();
                        return this;
                    }
                    var oFragment = range.createContextualFragment(html), //把插入内容转变为DocumentFragment
                        oLastNode = oFragment.lastChild; //用于修正编辑光标的位置
                    range.insertNode(oFragment);
                    range.setEndAfter(oLastNode); //把编辑光标放到我们插入内容之后
                    range.setStartAfter(oLastNode);
                    selection.removeAllRanges(); //清除所有选择，要不我们插入的内容与刚才的文本处于选中状态
                    selection.addRange(range); //插入内容
                }

                self.forceSaveLastRange();
            },
            saveLastRange: function () {
                var rangeInfo = this.getRange();
                rangeInfo && $(this).data('range', rangeInfo);
            },
            forceSaveLastRange: function () {
                var rangeInfo = this.getRange();
                $(this).data('range', rangeInfo);
            },
            getLastRange: function () {
                var rangeInfo = $(this).data('range');
                return rangeInfo || this.getRange();
            },
            getRange: function () {
                var selection = (window.getSelection) ? window.getSelection() : document.selection;
                if (!selection) {
                    return null;
                }
                try {
                    var range = selection.createRange ? selection.createRange() : selection.getRangeAt(0);
                    var text = window.getSelection ? range : range.text;
                    var rangeNode = null;
                    if (range.commonAncestorContainer) {
                        rangeNode = range.commonAncestorContainer;
                    } else {
                        if (range.parentElement)
                            rangeNode = range.parentElement();
                    }
                    return {
                        node: rangeNode,
                        range: range,
                        text: text,
                        selection: selection
                    };
                } catch (e) {
                    return null;
                }
            },
            isOrContainsNode: function (ancestor, descendant) {
                var node = descendant;
                while (node) {
                    if (node === ancestor) {
                        return true;
                    }
                    node = node.parentNode;
                }
                return false;
            },
            insertNodeOverSelection: function (node) {
                var containerNode = $(this).get(0);
                node = $(node).get(0);

                var sel, range, html;
                if (window.getSelection) {
                    sel = window.getSelection();
                    if (sel.getRangeAt && sel.rangeCount) {
                        range = sel.getRangeAt(0);
                        if ($.fn.isOrContainsNode(containerNode, range.commonAncestorContainer)) {
                            range.deleteContents();
                            range.insertNode(node);
                        } else {
                            containerNode.appendChild(node);
                        }
                    }
                } else if (document.selection && document.selection.createRange) {
                    range = document.selection.createRange();
                    if ($.fn.isOrContainsNode(containerNode, range.parentElement())) {
                        html = (node.nodeType == 3) ? node.data : node.outerHTML;
                        range.pasteHTML(html);
                    } else {
                        containerNode.appendChild(node);
                    }
                }
            }
        });
    },
    bindUI: function () {
        var self = this;

        //标题字数控制
        $(document).on('keyup', '#faqEditWindow .faq-field input[name="title"]', function () {
            var $this = $(this);
            var fieldParent = $this.closest('.faq-field');
            var maxTitleCharLength = FAQConfig.maxTitleCharLength();

            if ($this.val() === '') {
                fieldParent.find('.faq-notice').text(maxTitleCharLength + '字以内');
                fieldParent.removeClass('normal').addClass('error');
            } else {
                var val = $this.val();
                var remainingLength = maxTitleCharLength - val.length;
                remainingLength < 0 && (remainingLength = 0);

                fieldParent.removeClass('error').addClass('normal');
                fieldParent.find('.faq-notice').text('您还可以输入' + remainingLength + '字');
            }
        });
        //描述栏屏蔽右键
        $(document).bind("contextmenu", function (e) {
            return false;
        });

        //描述字数控制
        $(document).on('keydown', '#faqEditWindow .faq-field .summary', function (e) {
            if (e.ctrlKey) {
                e.preventDefault();
                return false;
            }
        });

        //描述字数控制
        $(document).on('keyup', '#faqEditWindow .faq-field .summary', function () {
            var $this = $(this);
            var fieldParent = $this.closest('.faq-field');
            var maxSummaryCharLength = FAQConfig.maxSummaryCharLength();
            var val = self.getEditParams()['faqSummary'];

            if (val === '') {
                fieldParent.find('.faq-notice').text(maxSummaryCharLength + '字以内');
                fieldParent.removeClass('normal').addClass('error');
            } else {
                var remaining = maxSummaryCharLength - val.length;
                if (remaining < 0) {
                    $this.html(val.substr(0, maxSummaryCharLength));
                    remaining = 0;
                }

                fieldParent.removeClass('error').addClass('normal');
                fieldParent.find('.faq-notice').text('您还可以输入' + remaining + '字');
            }
        });

        //编辑当前问题
        $(document).on('click', '#content .faq-edit', function (e) {
            var selectionInfo = FAQDataGridManager.getSelectedRow();
            FAQEditManager.setFormValue(selectionInfo);

            $('#faqEditWindow').show(function () {
                $(this).window('center').window('open');
            });
            e.preventDefault();
            return false;
        });
        //删除当前问题
        $(document).on('click', '#content .faq-delete', function (e) {
            var code = function () {
                var selectedRow = FAQDataGridManager.getSelectedRow();
                $.post(
                    FAQConfig.getBaseReqUrl() + "delete-faq.do",
                    {
                        faqID: selectedRow.faqID
                    },
                    function (resultInfo) {
                        if (resultInfo.success) {
                            FAQDataGridManager.deleteSelectedRow();

                            self.changeTotalCount(-1);
                            selectedRow.isShow == '1' && (self.changeShowedCount(-1));
                        } else {
                            AlertUtil.showError(resultInfo.errorMessage || '服务器发生故障');
                        }

                        $('#confirmDeleteWindow').window('close');
                    },
                    'json'
                );
            }

            GlobalDeleteScheduler.setContext(self).setFunction(code);

            $('#confirmDeleteWindow').show(function () {
                $(this).window('center').window('open');
            });

            e.preventDefault();
            return false;
        });
        //是否显示该问题
        $(document).on('click', '#content input.faq-toggle-show', function () {
            var $this = $(this);
            var row = FAQDataGridManager.getSelectedRow();
            var param = {
                faqID: row.faqID,
                isShow: row.isShow == '1' ? 0 : 1
            };

            $.post(
                FAQConfig.getBaseReqUrl() + 'change-faq-show-state.do',
                param,
                function (resultInfo) {
                    if (resultInfo.success) {
                        FAQDataGridManager.updateSelectedRow(param);
                        self.changeShowedCount(param.isShow ? 1 : -1);
                    } else {
                        $this.attr('checked', !$this.attr('checked'));
                        AlertUtil.showError(resultInfo.errorMessage || '服务器发生故障');
                    }
                },
                'json'
            );
        });
        //取消展示
        $(document).on('click', '#content a.faq-cancel-show', function () {
            var $this = $(this);
            var row = ShowedFAQDataGridManager.getSelectedRow();
            var param = {
                faqID: row.faqID,
                isShow: 0
            };

            $.post(
                FAQConfig.getBaseReqUrl() + 'change-faq-show-state.do',
                param,
                function (resultInfo) {
                    if (resultInfo.success) {
                        ShowedFAQDataGridManager.deleteSelectedRow();
                        self.changeShowedCount(-1);
                    } else {
                        $this.attr('checked', !$this.attr('checked'));
                        AlertUtil.showError(resultInfo.errorMessage || '服务器发生故障');
                    }
                },
                'json'
            );
        });

        //位置移动
        $(document).on('click', '#content a.move-action', function () {
            var $this = $(this);
            var selectedIndex = ShowedFAQDataGridManager.getSelectedRowIndex();
            var selectedRow = ShowedFAQDataGridManager.getSelectedRow();
            var oldFaqID = -1;
            var newIndex = -1;

            if ($this.is('.move-up')) {
                if (selectedIndex == 0) {
                    return;
                } else {
                    newIndex = selectedIndex - 1;
                }
            } else if ($this.is('.move-top')) {
                if (selectedIndex == 0) {
                    return;
                } else {
                    newIndex = 0;
                }
            } else if ($this.is('.move-down')) {
                if (selectedIndex == ShowedFAQDataGridManager.getCurrentPageRowNo() - 1) {
                    return;
                } else {
                    newIndex = selectedIndex + 1;
                }
            } else if ($this.is('.move-bottom')) {
                newIndex = ShowedFAQDataGridManager.getCurrentPageRowNo() - 1;
                if (selectedIndex == newIndex) {
                    return;
                }
            }

            var toReplaceRow = ShowedFAQDataGridManager.getGivenRow(newIndex);
            if (!!toReplaceRow) {
                oldFaqID = toReplaceRow['faqID'];

                $.post(
                    FAQConfig.getBaseReqUrl() + 'change-faq-order.do',
                    {
                        srcFAQID: selectedRow['faqID'],
                        srcOrder: selectedIndex,
                        destFAQID: oldFaqID,
                        destOrder: newIndex
                    },
                    function (resultInfo) {
                        if (resultInfo.success) {
                            ShowedFAQDataGridManager.exchangeOrder({
                                srcOrder: selectedIndex,
                                srcRow: $.extend(selectedRow, {showOrder: newIndex}),
                                destOrder: newIndex,
                                destRow: $.extend(toReplaceRow, {showOrder: selectedIndex})
                            });
                        } else {
                            AlertUtil.showError(resultInfo.errorMessage || '服务器发生故障');
                        }
                    },
                    'json'
                );
            }
        });

        //编辑常见问题-提交
        $(document).on('click', '#editFAQBtn', function () {
            var errorFAQFields = $('#faqEditWindow .faq-field.error');
            if (errorFAQFields.length > 0) {
                AlertUtil.showError('请填写相关必填字段');
                return;
            } else {
                var serviceUrl = self.isFormModeEdit() ? 'update-faq.do' : 'add-faq.do';
                var params = self.getEditParams();

                $.post(
                    FAQConfig.getBaseReqUrl() + serviceUrl,
                    params,
                    function (resultInfo) {
                        if (resultInfo.success) {
                            if (self.isFormModeEdit()) {
                                FAQDataGridManager.updateSelectedRow(resultInfo.data.faq);
                            } else {
                                self.changeTotalCount(1);
                                FAQDataGridManager.insertRow(resultInfo.data.faq);
                            }
                            $('#faqEditWindow').window('close');
                        } else {
                            AlertUtil.showError(resultInfo.errorMessage || '服务器发生故障');
                        }
                    }
                );

            }
        });
        //编辑常见问题-返回
        $(document).on('click', '#returnBtn', function () {
            $('#faqEditWindow').window('close');
        });

        //添加链接
        $(document).on('click', '#addLinkBtn', function () {
            var mask = $('#faqEditWindow > .mask');
            if (mask.is(':visible')) {
                mask.hide();
            } else {
                setTimeout(function () {
                    mask.show();
                }, 200);
            }
        });

        $(document).on('mouseup keyup', '#faqEditWindow .faq-field .summary', function () {
            $('#faqEditWindow .faq-field .summary').forceSaveLastRange();
        });

        //关闭遮罩层
        $(document).on('click', '#faqEditWindow .add-link-panel .close', function () {
            $('#faqEditWindow .mask').hide();
        });

        //保存编辑的链接信息
        $(document).on('click', '#faqEditWindow .add-link-panel .save', function () {
            if (!self.checkLinkEdit()) {
                return;
            }

            var linkContent = $('#faqEditWindow .add-link-panel input[name="link-content"]').val();
            var linkHref = $('#faqEditWindow .add-link-panel input[name="link-href"]').val();

            var remainingSummaryLength = FAQConfig.maxSummaryCharLength() - self.getEditParams()['faqSummary'];
            var toInsert = '<a href="' + linkHref + '">' + linkContent + '</a>';

            if (remainingSummaryLength < toInsert.length) {
                AlertUtil.showError('描述字段最大长度为' + FAQConfig.maxSummaryCharLength());
            } else {
                var summaryNode = $('#faqEditWindow .faq-field .summary');
                summaryNode.insertAtCaret(toInsert);
                $('#faqEditWindow .mask').hide();
            }
        });

        //文字链 字数控制
        $(document).on('keyup', '#faqEditWindow .add-link-panel input[name="link-content"]', function () {
            var editedSummaryLength = $('#faqEditWindow .faq-field .summary').text().length;
            var remainingSummaryLength = FAQConfig.maxSummaryCharLength() - editedSummaryLength;
            var $this = $(this);

            var val = $this.val();
            if (val.length > remainingSummaryLength) {
                $this.val(val.substr(0, remainingSummaryLength));
            }
        });

        $(document).on('click', '#faqEditWindow .faq-field .summary a', function () {
            var $this = $(this);
            self._lastSummaryHrefEle = $this;
        });

        //取消链接按钮
        $(document).on('click', '#unLinkBtn', function () {
            if (self._lastSummaryHrefEle != null && self._lastSummaryHrefEle.closest('.summary').length > 0) {
                var textedNode = self._lastSummaryHrefEle.text();
                self._lastSummaryHrefEle.replaceWith(textedNode);
                self._lastSummaryHrefEle = null;
            }
        });
    },
    //获取编辑参数
    getEditParams: function () {
        var editParams = {};

        $('#faqEditWindow .faq-field input').each(function () {
            var $this = $(this);
            editParams[$this.attr('name')] = $.isFunction($this.val) ? $this.val() : $this.text();
        });
        var summary = $('#faqEditWindow .faq-field .summary').html();
        summary = summary.replace(/[<]span[>]|[<]\/span[>]/gi, '');
        summary = summary.replace(/[<]a[^>]+[>][<]\/a[>]/gi, '');
        summary = summary.replace(/[<]\/?br[>]/gi, '');
        summary = summary.replace(/[<]img[^>]+[>]/gi, '');

        editParams['faqSummary'] = summary;
        this.isFormModeEdit() && (editParams = $.extend({}, FAQDataGridManager.getSelectedRow(), editParams));

        return editParams;
    },
    //校验链接信息是否合法
    checkLinkEdit: function () {
        var panel = $('#faqEditWindow .add-link-panel');
        var linkContent = panel.find('input[name="link-content"]');
        var linkHref = panel.find('input[name="link-href"]');

        if (linkContent.val() === '') {
            AlertUtil.showError('请填写链接标题');
            return false;
        }

        if (linkHref.val() === '' || !RegexpUtil.isLegalURL(linkHref.val())) {
            AlertUtil.showError('请填写有效的url地址');
            return false;
        }

        return true;
    },
    //设置表单编辑模式为 编辑
    markFormModeEdit: function () {
        this._editMode = 'edit';
    },
    //设置表单编辑模式为 新增
    markFormModeAdd: function () {
        this._editMode = 'add';
    },
    //是否处于编辑模式
    isFormModeEdit: function () {
        return this._editMode === 'edit';
    },
    setFormValue: function (row) {
        if (!row) {
            row = {
                title: '',
                faqSummary: ''
            };
            this.markFormModeAdd();
        } else {
            this.markFormModeEdit();
        }

        var editWindow = $('#faqEditWindow');
        editWindow.find('div[name="summary"]').html(row.faqSummary).trigger('keyup');
        editWindow.find('input[name="title"]').val(row.title).trigger('keyup');
    },
    changeTotalCount: function (delta) {
        var totalCountEle = $('#totalFAQCount');
        var oldVal = parseInt(totalCountEle.text());

        totalCountEle.text(oldVal + delta);
    },
    changeShowedCount: function (delta) {
        var showCountEle = $('#showedFAQCount');
        var oldVal = parseInt(showCountEle.text());

        showCountEle.text(oldVal + delta);
    }
};

//表格管理器
var ChartManager = (function () {
    var _tempData = {
        columns: [],
        values: []
    }
    var loadingItem;

    function saveTempData(data) {
        $.extend(_tempData, data);
    }

    return {
        init: function () {
            loadingItem = $(".chart-mask > .wrapper .chart-container > .loading");
        },
        showLoading: function () {
            $('#faq-chart-bar, #faq-chart-pie').empty();
            loadingItem.show();
        },
        showBarChart: function (chartParam) {
            chartParam = chartParam || _tempData;
            saveTempData(chartParam || _tempData);
            loadingItem.hide();
            $('#faq-chart-bar').show();

            var myChart = echarts.init(document.getElementById('faq-chart-bar'));
            var columnNum = chartParam.columns.length || 1;
            var maxCharLength = Math.floor(1200 / columnNum * 0.8 / 18) || 1;

            var option = {
                tooltip: {
                    show: true
                },
                legend: {
                    data: ['浏览次数'],
                    textStyle: {
                        fontSize: 20
                    }
                },
                xAxis: [
                    {
                        type: 'category',
                        data: chartParam.columns,
                        axisLabel: {
                            interval: 0,
                            formatter: function (c) {
                                if (c.length > maxCharLength) {
                                    var stringBuilder = '';

                                    for (var i = 0; i < c.length; i++) {
                                        stringBuilder += c.charAt(i);
                                        if ((i + 1) % maxCharLength == 0) {
                                            stringBuilder += '\n';
                                        }
                                    }

                                    return stringBuilder;
                                } else {
                                    return c;
                                }
                            }
                        }
                    }
                ],
                axis: [{
                    show: false
                }],
                yAxis: [
                    {
                        type: 'value'
                    }
                ],
                series: [
                    {
                        "name": "浏览次数",
                        "itemStyle": {        // 系列级个性化样式，纵向渐变填充
                            normal: {
                                color: '#5B9BD5'
                            }
                        },
                        "type": "bar",
                        "data": chartParam.values
                    }
                ]
            };

            myChart.setOption(option);
        },
        showPieChart: function (chartParam) {
            chartParam = chartParam || _tempData;
            saveTempData(chartParam);
            loadingItem.hide();
            $('#faq-chart-pie').show();

            var data = [];
            for (var i = 0; i < chartParam.columns.length; i++) {
                data.push({
                    name: chartParam.columns[i],
                    value: chartParam.values[i]
                });
            }

            var myChart = echarts.init(document.getElementById('faq-chart-pie'));

            var option = {
                title: {
                    text: '浏览次数',
                    textStyle: {
                        fontSize: 20
                    },
                    x: 'center'
                },
                tooltip: {
                    show: true
                },
                legend: {
                    orient: 'horizontal',
                    x: 'center',
                    y: 'bottom',
                    data: chartParam.columns
                },
                series: [
                    {
                        name: '浏览次数',
                        type: 'pie',
                        radius: '55%',
                        data: data
                    }
                ]
            };

            myChart.setOption(option);
        }
    }
})();

$(function () {
    FAQUIManager.init();
    FAQEditManager.init();
    ChartManager.init();
});
