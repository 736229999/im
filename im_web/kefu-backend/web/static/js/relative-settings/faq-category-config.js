/**
 * Author: morningking
 * Date: 2015/10/12
 * Contact: 243717042@qq.com
 */

//删除调度管理器
var GlobalDeleteScheduler = new GlobalDeleteScheduler();
//相关配置项
var FAQCategoryConfig = (function () {
    var baseReqUrl = './';

    return {
        //服务根地址
        getBaseReqUrl: function () {
            return baseReqUrl;
        },
        //常见问题标题最大字符数
        maxTitleCharLength: function () {
            return 10;
        },
        //常见问题描述最大字符数
        maxRemarkCharLength: function () {
            return 50;
        }
    };
})();

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
            WdatePicker({dateFmt: 'yyyy-MM-dd HH:mm:ss', startDate:'%y-%M-%d 23:59:59', minDate: '%y-%M-#{%d-30}', maxDate: '%y-%M-#{%d+1}'});
        });

        //表格初始化
        $('#faqCategoryTable').datagrid({
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
                    field: 'categoryID',
                    title: '操作',
                    width: '9%',
                    align: 'center',
                    sortable: false,
                    formatter: function (value, row, index) {
                        return '<span style="display: inline-block" data-faq-category-id="' + value + '">' +
                            '<a class="faq-edit">[编辑]</a><a class="faq-delete">[删除]</a></span>';
                    }
                },
                {
                    field: 'isShow',
                    title: '是否显示',
                    width: '4%',
                    align: 'center',

                    sortable: false,
                    formatter: function (value) {
                        return '<input data-faq-category-id="' + value + '" class="faq-toggle-show" type="checkbox" '
                            + (value == '1' ? 'checked' : '' ) + '>';
                    }
                },
                {field: 'title', title: '问题', width: '30%', align: 'center', sortable: false},
                {field: 'remark', title: '备注', width: '30%', align: 'center', sortable: false},
                {field: 'consultCount', title: '累计咨询次数', width: '9%', align: 'center', sortable: false},
                {field: 'lastOperator', title: '最后操作人', width: '9%', align: 'center', sortable: false},
                {field: 'updateTime', title: '最后操作时间', width: '9%', align: 'center', sortable: false}
            ]]
        });
        var pagination = $('#faqCategoryTable').datagrid('getPager');
        pagination.pagination({
            onSelectPage: function (pageNumber, pageSize) {
                $.extend(self._faqPageOption, {
                    currentPageNo: pageNumber,
                    pageSize: pageSize
                });

                self.searchFAQCategoryList(function (list, totalFaqNum, totalShowedFaqNum) {
                    FAQCategoryDataGridManager.loadData({
                        total: totalFaqNum,
                        rows: list
                    });
                });
            }
        });
        //已显示表格 初始化
        $('#showedFaqCategoryTable').datagrid({
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
                    field: 'categoryID',
                    title: '操作',
                    width: '5%',
                    align: 'center',
                    sortable: false,
                    formatter: function (value, row, index) {
                        return '<span style="display: inline-block" data-faq-category-id="' + value + '">' +
                            '<a class="faq-cancel-show">[取消展示]</a></span>';
                    }
                },
                {field: 'title', title: '问题', width: '30%', align: 'center', sortable: false},
                {field: 'remark', title: '解释', width: '30%', align: 'center', sortable: false},
                {field: 'consultCount', title: '累计咨询次数', width: '9%', align: 'center', sortable: true},
                {field: 'lastOperator', title: '最后操作人', width: '9%', align: 'center', sortable: false},
                {field: 'updateTime', title: '最后操作时间', width: '9%', align: 'center', sortable: false}
            ]]
        });
        var pagination = $('#showedFaqCategoryTable').datagrid('getPager');
        pagination.pagination({
            onSelectPage: function (pageNumber, pageSize) {
                $.extend(self._showedFaqPageOption, {
                    currentPageNo: pageNumber || 1,
                    pageSize: pageSize
                });

                self.searchFAQCategoryList(function (list, totalFaqNum, totalShowedFaqNum) {
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
            self.searchFAQCategoryList(function (list, totalFaqCategoryNum, totalShowedFaqCategoryNum) {
                var isShowedFAQTableVisible = $('.count-tab>.tab-btn.active').is('.toggle-showed');

                if (isShowedFAQTableVisible) {
                    ShowedFAQDataGridManager.loadData({
                        total: totalShowedFaqCategoryNum,
                        rows: list
                    });
                } else {
                    FAQCategoryDataGridManager.loadData({
                        total: totalFaqCategoryNum,
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

            self.searchFAQCategoryList(function (list, totalFaqCategoryNum, totalShowedFaqCategoryNum) {
                if ($this.hasClass('toggle-showed')) {
                    $('#content .datagrid:first-child').hide();
                    $('#content .datagrid:last-child').show();

                    setTimeout(function () {
                        ShowedFAQDataGridManager.loadData({
                            total: totalShowedFaqCategoryNum,
                            rows: list
                        });
                    }, 200);
                } else {
                    $('#content .datagrid:last-child').hide();
                    $('#content .datagrid:first-child').show();

                    setTimeout(function () {
                        FAQCategoryDataGridManager.loadData({
                            total: totalFaqCategoryNum,
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
                    valueList.push(item.consultCount);
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
        $(document).on('click', '#addFAQCategory', function () {
            FAQCategoryEditManager.setFormValue(null);
            $('#faqEditWindow').show(function () {
                $(this).window('center').window('open');
            });
        });
        //导入常见问题
        $('#excelImport').uploader({
            action: FAQCategoryConfig.getBaseReqUrl() + "import.do",
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
                        $('#search').trigger('click');
                        //FAQCategoryEditManager.changeTotalCount(result.data.importCount);
                    } else {
                        AlertUtil.showError('服务器发生故障，请稍后重试');
                    }
                }
            }
        });
        //导出常见问题
        $(document).on('click', '#excelExport', function () {
            window.location.href = FAQCategoryConfig.getBaseReqUrl() + 'export.xls'
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
        this.searchFAQCategoryList(function (list, totalFaqNum) {
            $('#content .datagrid:last-child').hide();
            $('#content .datagrid:first-child').show();

            FAQCategoryDataGridManager.loadData({
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
        this.searchFAQCategoryList(callback, {
            isFetchAll: true,
            isShow: true
        });
    },

    //服务请求
    searchFAQCategoryList: function (callback, extraParam) {
        var self = this;

        $.post(
            FAQCategoryConfig.getBaseReqUrl() + 'get-faq-category-list.do',
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
var FAQCategoryDataGridManager = (function () {
    var table = $("#faqCategoryTable");

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
    var table = $("#showedFaqCategoryTable");

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
var FAQCategoryEditManager = {
    _editMode: 'add',
    _lastSummaryHrefEle: null,

    init: function () {
        this.initUI();
        this.bindUI();
    },
    initUI: function () {
    },
    bindUI: function () {
        var self = this;

        //标题字数控制
        $(document).on('keyup', '#faqEditWindow .faq-field input.title', function () {
            var $this = $(this);
            var fieldParent = $this.closest('.faq-field');
            var maxTitleCharLength = FAQCategoryConfig.maxTitleCharLength();

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

        //描述字数控制
        $(document).on('keyup', '#faqEditWindow .faq-field .remark', function () {
            var $this = $(this);
            var fieldParent = $this.closest('.faq-field');
            var maxRemarkCharLength = FAQCategoryConfig.maxRemarkCharLength();

            var val = $this.val();
            if (val.length > maxRemarkCharLength) {
                $this.val(val.substr(0, maxRemarkCharLength));
            }

            val = $this.val();
            fieldParent.removeClass('error').addClass('normal');
            fieldParent.find('.faq-notice').text('您还可以输入' + (maxRemarkCharLength - val.length) + '字');
        });

        //编辑当前问题
        $(document).on('click', '#content .faq-edit', function (e) {
            var selectionInfo = FAQCategoryDataGridManager.getSelectedRow();
            FAQCategoryEditManager.setFormValue(selectionInfo);

            $('#faqEditWindow').show(function () {
                $(this).window('center').window('open');
            });

            e.preventDefault();
            return false;
        });
        //删除当前问题
        $(document).on('click', '#content .faq-delete', function (e) {
            var code = function () {
                var selectedRow = FAQCategoryDataGridManager.getSelectedRow();

                $.post(
                    FAQCategoryConfig.getBaseReqUrl() + "delete.do",
                    {
                        categoryID: selectedRow.categoryID
                    },
                    function (resultInfo) {
                        if (resultInfo.success) {
                            FAQCategoryDataGridManager.deleteSelectedRow();

                            self.changeTotalCount(-1);
                            selectedRow.isShow == '1' && (self.changeShowedCount(-1));
                        } else {
                            AlertUtil.showError(resultInfo.errorMessage || '服务器发生故障');
                        }

                        $('#confirmDeleteWindow').window('close');
                    },
                    'json'
                );
            };

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
            var row = FAQCategoryDataGridManager.getSelectedRow();
            var param = {
                categoryID: row.categoryID,
                isShow: row.isShow == '1' ? 0 : 1
            };

            $.post(
                FAQCategoryConfig.getBaseReqUrl() + 'change-show-state.do',
                param,
                function (resultInfo) {
                    if (resultInfo.success) {
                        FAQCategoryDataGridManager.updateSelectedRow(param);
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
                categoryID: row.categoryID,
                isShow: 0
            };

            $.post(
                FAQCategoryConfig.getBaseReqUrl() + 'change-show-state.do',
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

        //编辑常见问题-提交
        $(document).on('click', '#editFAQCategoryBtn', function () {
            var errorFAQFields = $('#faqEditWindow .faq-field.error');
            if (errorFAQFields.length > 0) {
                AlertUtil.showError('请填写相关必填字段');
                return;
            } else {
                var serviceUrl = self.isFormModeEdit() ? 'update.do' : 'add.do';
                var params = self.getEditParams();

                $.post(
                    FAQCategoryConfig.getBaseReqUrl() + serviceUrl,
                    params,
                    function (resultInfo) {
                        if (resultInfo.success) {
                            if (self.isFormModeEdit()) {
                                FAQCategoryDataGridManager.updateSelectedRow(resultInfo.data.faqCategory);
                            } else {
                                self.changeTotalCount(1);
                                FAQCategoryDataGridManager.insertRow(resultInfo.data.faqCategory);
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
    },
    //获取编辑参数
    getEditParams: function () {
        var editParams = {};

        var title = $('#faqEditWindow .faq-field input[name="title"]').val();
        var remark = $('#faqEditWindow .faq-field .remark').val();

        editParams['title'] = title.substring(0, FAQCategoryConfig.maxTitleCharLength());
        editParams['remark'] = remark.substring(0, FAQCategoryConfig.maxRemarkCharLength());
        this.isFormModeEdit() && (editParams = $.extend({}, FAQCategoryDataGridManager.getSelectedRow(), editParams));

        return editParams;
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
                remark: ''
            };
            this.markFormModeAdd();
        } else {
            this.markFormModeEdit();
        }

        var editWindow = $('#faqEditWindow');
        editWindow.find('textarea[name="remark"]').val(row.remark).trigger('keyup');
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
                    data: ['咨询次数'],
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
                        "name": "咨询次数",
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
                    text: '咨询次数',
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
                        name: '咨询次数',
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
    FAQCategoryEditManager.init();
    ChartManager.init();
});
