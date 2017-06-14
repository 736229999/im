/**
 * Author: morningking
 * Date: 2015/9/28
 * Contact: 243717042@qq.com
 */

//页面配置项
var PageConfig = (function () {
    var webimServiceBaseUrl = 'http://merchant.feiniu.com/webim';
    var currentHref = window.location.href;
    var currentDomain = currentHref.replace(new RegExp('^https?://([^/]+)/.+$'), '$1');
    var isLocal = currentDomain.indexOf('local') == 0;

    if (currentDomain.indexOf('beta.fn') > -1)/**beta环境**/{
        webimServiceBaseUrl = 'http://webim.beta1.fn/webim';
    } else if (currentDomain.indexOf('dev.fn') > -1)/**dev环境**/{
        webimServiceBaseUrl = 'http://webim.dev1.fn/webim';
    }

    return {
        //获取表情根地址
        getEmotionBaseUrl: function () {
            return webimServiceBaseUrl + "/static/images/feiniu-emotion/";
        }
    };
})();

//相关配置项
var PhraseConfig = (function () {
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
        },
        maxGroupNameCharLength: function () {
            return 10;
        },
        maxQuickCodeCharLength: function () {
            return 4;
        },
        //分组名称最大字符数
        maxGroupNameCharLength: function () {
            return 10;
        }
    };
})();

//删除调度管理器
var GlobalDeleteScheduler = new GlobalDeleteScheduler();

//表情地址
var ImageTitleArray = [
    {"title": "好的", "path": "b01.gif"},
    {"title": "悲伤", "path": "b02.gif"},
    {"title": "鄙视", "path": "b03.gif"},
    {"title": "大笑", "path": "b04.gif"},
    {"title": "撒钱", "path": "b05.gif"},
    {"title": "恭喜", "path": "b06.gif"},
    {"title": "鼓掌", "path": "b07.gif"},
    {"title": "呼唤", "path": "b08.gif"},
    {"title": "奋斗", "path": "b09.gif"},
    {"title": "胜利", "path": "b10.gif"},
    {"title": "惊讶", "path": "b11.gif"},
    {"title": "开心", "path": "b12.gif"},
    {"title": "可怜", "path": "b13.gif"},
    {"title": "祈祷", "path": "b14.gif"},
    {"title": "礼物", "path": "b15.gif"},
    {"title": "阴险", "path": "b16.gif"},
    {"title": "流汗", "path": "b17.gif"},
    {"title": "大哭", "path": "b18.gif"},
    {"title": "示爱", "path": "b19.gif"},
    {"title": "玫瑰", "path": "b20.gif"},
    {"title": "凋谢", "path": "b21.gif"},
    {"title": "赞", "path": "b22.gif"},
    {"title": "吻", "path": "b23.gif"},
    {"title": "发怒", "path": "b24.gif"},
    {"title": "酷", "path": "b25.gif"},
    {"title": "太阳", "path": "b26.gif"},
    {"title": "困", "path": "b27.gif"},
    {"title": "月亮", "path": "b28.gif"},
    {"title": "偷笑", "path": "b29.gif"},
    {"title": "狂吐", "path": "b30.gif"},
    {"title": "调皮", "path": "b31.gif"},
    {"title": "握手", "path": "b32.gif"},
    {"title": "发呆", "path": "b33.gif"},
    {"title": "思考", "path": "b34.gif"},
    {"title": "色", "path": "b35.gif"},
    {"title": "下雨", "path": "b36.gif"},
    {"title": "心碎", "path": "b37.gif"},
    {"title": "晕", "path": "b38.gif"},
    {"title": "疑问", "path": "b39.gif"},
    {"title": "喝彩", "path": "b40.gif"},
    {"title": "谢谢", "path": "b54.gif"},
    {"title": "再见", "path": "b55.gif"},
    {"title": "奔跑", "path": "b43.gif"},
    {"title": "大怒", "path": "b44.gif"},
    {"title": "祝福", "path": "b45.gif"},
    {"title": "加油", "path": "b46.gif"},
    {"title": "惊喜", "path": "b47.gif"},
    {"title": "无奈", "path": "b48.gif"},
    {"title": "扔番茄", "path": "b49.gif"},
    {"title": "讨厌", "path": "b50.gif"},
    {"title": "吐舌头", "path": "b51.gif"},
    {"title": "伤心", "path": "b52.gif"},
    {"title": "晕", "path": "b53.gif"},
    {"title": "无聊", "path": "b41.gif"},
    {"title": "唱歌", "path": "b42.gif"}
];

var generalUtil = {
    rgbToInt: function (rgbColor) {
        var red = 0, green = 0, blue = 0;

        var digits = /(.*?)rgb\((\d+), (\d+), (\d+)\)/i.exec(rgbColor);
        if (!!digits && digits.length == 5) {
            red = parseInt(digits[2]);
            green = parseInt(digits[3]);
            blue = parseInt(digits[4]);
        } else {
            digits = /(#)([0-9a-f]{2})([0-9a-f]{2})([0-9a-f]{2})/i.exec(rgbColor);
            if (!!digits && digits.length == 5) {
                red = parseInt('0x' + digits[2]);
                green = parseInt('0x' + digits[3]);
                blue = parseInt('0x' + digits[4]);
            }
        }

        var rgb = red | (green << 8) | (blue << 16);

        return rgb;
    },
    intToRgb: function (intColor) {
        var blue = ((intColor & 0xff0000) >> 16).toString(16);
        var green = ((intColor & 0x00ff00) >> 8).toString(16);
        var red = (intColor & 0x0000ff).toString(16);

        return '#' + this.fillZero(red) + this.fillZero(green) + this.fillZero(blue);
    }
};

//主要负责大编辑页面的ui处理
var PhraseUIManager = {
    _phrasePageOption: {
        currentPageNo: 1,
        pageSize: 10
    },

    _groupPageOption: {
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

        //快捷短语表格初始化
        $('#phraseTable').datagrid({
            nowrap: false,
            width: "auto",
            height: "auto",
            loadMsg: '数据装载中......',
            fitColumns: true,
            remoteSort: false,
            pagination: true,
            sortable: false,
            columns: [[
                {
                    field: 'isSelect',
                    title: '全选',
                    width: '4%',
                    align: 'center',
                    checkbox: true
                },
                {
                    field: 'phraseId',
                    title: '操作',
                    width: '9%',
                    align: 'center',
                    sortable: false,
                    formatter: function (value, row, index) {
                        return '<span style="display: inline-block" data-phraseid="' + value + '"><a class="phrase-edit">[编辑]</a><a class="phrase-delete">[删除]</a></span>';
                    }
                },
                {field: 'quickCode', title: '快捷编码', width: '4%', align: 'center', sortable: false},
                {
                    field: 'phraseSummary',
                    title: '快捷短语',
                    width: '30%',
                    align: 'center',
                    sortable: false,
                    formatter: function (value) {
                        return value.replace(/<image:(b\d+\.gif)>/g, '<img src="' + PageConfig.getEmotionBaseUrl() + '$1" width="36" height="36">');
                    }
                },
                {field: 'phraseGroupName', title: '分组', width: '9%', align: 'center', sortable: false},
                {field: 'lastOperator', title: '最后操作人', width: '9%', align: 'center', sortable: false},
                {field: 'updateTime', title: '最后操作时间', width: '9%', align: 'center', sortable: false}
            ]]
        });

        var pagination = $('#phraseTable').datagrid('getPager');
        pagination.pagination({
            onSelectPage: function (pageNumber, pageSize) {
                $.extend(self._phrasePageOption, {
                    currentPageNo: pageNumber,
                    pageSize: pageSize
                });

                self.searchPhraseList(function (list, totalNum, totalShowedFaqNum) {
                    PhraseDataGridManager.loadData({
                        total: totalNum,
                        rows: list
                    });
                });
            }
        });

        (function () {
            var hasInitialized = false;

            $('#quickPhraseGroupEditWindow').window({
                'onOpen': function () {
                    if (!hasInitialized) {
                        $('#groupTable').datagrid({
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
                                {field: 'rowIndex', title: '序号', width: '20%', align: 'center', sortable: false},
                                {
                                    field: 'groupName',
                                    title: '分组名称',
                                    width: '50%',
                                    align: 'center',
                                    sortable: false,
                                    formatter: function (value) {
                                        return '<span class="edit-group-name-container">' +
                                            '<span class="group-label" title="点击编辑分组名称">' + value +
                                            '</span>' +
                                            '<span class="edit-panel hide">' +
                                            '<input name="groupName" value="' + value + '">' +
                                            '<span class="field-error">X 不超过10个字</span>' +
                                            '</span>';
                                    }
                                },
                                {
                                    field: 'operator',
                                    title: '操作',
                                    width: '30%',
                                    align: 'center',
                                    sortable: false,
                                    formatter: function (value) {
                                        return '<span style="display: inline-block" data-phraseid="' + value + '">' +
                                            '<a class="group-edit">[编辑]</a>' +
                                            '<a class="group-save hide">[保存]</a>' +
                                            '<a class="group-cancel hide">[取消]</a>' +
                                            '</span>';
                                    }
                                },
                            ]]
                        });
                        hasInitialized = true;
                    }
                    //翻页
                    var paginationGroup = $('#groupTable').datagrid('getPager');
                    paginationGroup.pagination({
                        onSelectPage: function (pageNumber, pageSize) {
                            $.extend(self._groupPageOption, {
                                currentPageNo: pageNumber,
                                pageSize: pageSize
                            });

                            self.searchGroupList(function (list, totalNum) {
                                GroupDataGridManager.loadData({
                                    total: totalNum,
                                    rows: list
                                });
                            });
                        }
                    });
                }
            });
        })();

        (function setEmotionUrls() {
            $('#J_face_popup .heads img').each(function (index) {
                var $this = $(this);
                var currentImageInfo = ImageTitleArray[index];

                $this.attr('src', PageConfig.getEmotionBaseUrl() + currentImageInfo['path'])
                    .attr('title', currentImageInfo['title']);
            });
        })();
    },

    bindUI: function () {
        var self = this;
        //查询按钮
        $('#search').click(function () {
            self.searchPhraseList(function (list, totalNum, totalShowedFaqNum) {
                PhraseDataGridManager.loadData({
                    total: totalNum,
                    rows: list
                });
            });
        });
        //重置按钮
        $('#resetCondition').click(function () {
            $('#quickCode, #phrasePart').val('');
            $('#groupSearch').combobox('setText', '');
        });
        //添加快捷短语
        $(document).on('click', '#addPhrase', function () {
            PhraseEditManager.setFormValue(null);
            PhraseEditManager.markFormModeAdd();
            $('#phraseEditWindow').show(function () {
                $(this).window('center').window('open');
            });
        });

        //删除选中快捷短语
        $(document).on('click', '#delPhrase', function () {
            PhraseEditManager.setFormValue(null);
            $('#phraseEditWindow').show(function () {
                $(this).window('center').window('open');
            });
        });

        //新增分组
        $(document).on('click', '#addGroup', function () {
            $('#addGroupWindow').show(function () {
                $(this).window('center').window('open');
            });
        });
        //新增分组保存
        $(document).on('click', '#addGroupSaveBtn', function () {
            var groupName = $('#addGroupWindow input[id="newGroupName"]').val();
            $.post(
                PhraseConfig.getBaseReqUrl() + "add-group.do",
                {
                    groupName: groupName
                },
                function (resultInfo) {
                    if (resultInfo.success) {
                        $('#addGroupWindow input[id="newGroupName"]').val("");
                        $('#addGroupWindow').window('close');
                    } else {
                        AlertUtil.showError(resultInfo.errorMessage || '服务器发生故障');
                    }
                }
            );

        });
        //新增分组取消
        $(document).on('click', '#addGroupReturnBtn', function () {
            $('#addGroupWindow').window('close');
        });
        //修改快捷短语分组
        $(document).on('click', '#editGroup', function () {
            //PhraseEditManager.setFormValue(null);
            setTimeout(function () {
                PhraseUIManager.searchGroupList(function (list, totalNum) {
                    GroupDataGridManager.loadData({
                        total: totalNum,
                        rows: list
                    });
                });
            }, 500);
            $('#quickPhraseGroupEditWindow').show(function () {
                $(this).window('center').window('open');
            });
        });

        //导入快捷短语
        $('#excelImport').uploader({
            action: PhraseConfig.getBaseReqUrl() + "import-quick-phrase-list.do",
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

                        self.searchPhraseList(function (list, totalNum ) {
                            PhraseDataGridManager.loadData({
                                    total: totalNum,
                                    rows: list
                            });
                        });
                    } else {
                        AlertUtil.showError('服务器发生故障，请稍后重试');
                    }
                }
            }
        });

        //导出快捷短语
        $(document).on('click', '#excelExport', function () {
            window.location.href = PhraseConfig.getBaseReqUrl() + 'export-quick-phrase-excel.xls'
                + ObjectUtil.serializeToFormStyle(self.getSearchParams());
        });

        //快捷短语分组combobox
        $('.groupCombo').combobox({
            textField: 'groupName',
            valueField: 'rowIndex',
            mode: 'remote',
            loader: function (param, success) {
                PhraseEditManager.searchMachedPhraseGroup(param.q || '', function (data) {
                    success(data);
                });
            }
        });

        //点击编辑分组名称
        $(document).on('click', '#contentGroup .datagrid .group-label, #contentGroup .datagrid .group-edit', function () {
            var $this = $(this);
            var closestTr = $this.closest('tr');

            closestTr.find('.edit-panel').removeClass('hide').find('input').focus();
            closestTr.find('.group-save, .group-cancel').removeClass('hide');
            closestTr.find('.group-edit, .group-label').addClass('hide');
        });

        //取消保存分组名称
        $(document).on('click', '#contentGroup .datagrid .group-cancel', function () {
            var $this = $(this);
            var closestTr = $this.closest('tr');

            closestTr.find('.edit-panel').addClass('hide');
            closestTr.find('.group-save, .group-cancel').addClass('hide');
            closestTr.find('.group-edit, .group-label').removeClass('hide');
        });

        //保存分组名称
        $(document).on('click', '#contentGroup .datagrid .group-save', function () {
            var $this = $(this);
            var closestTr = $this.closest('tr');

            var params = $.extend(GroupDataGridManager.getSelectedRow(), {
                oldGroupName: closestTr.find('.group-label').text(),
                newGroupName: closestTr.find('.edit-panel input').val()
            });

            if (params.newGroupName == '' || params.newGroupName.length > PhraseConfig.maxGroupNameCharLength) {
                AlertUtil.showError('分组名称不符合长度要求,应为1-10个字符');
                return;
            }

            if(params.newGroupName == params.oldGroupName){
               return;
            }
            $.post(
                PhraseConfig.getBaseReqUrl() + 'update-group',
                params,
                function (resultInfo) {
                    if (resultInfo.success) {
                        closestTr.find('.edit-panel').addClass('hide');
                        closestTr.find('.group-save, .group-cancel').addClass('hide');
                        closestTr.find('.group-edit').removeClass('hide');
                        closestTr.find('.group-label').removeClass('hide').html(params.newGroupName);
                        //刷新
                        self.searchPhraseList(function (list, totalNum ) {
                            PhraseDataGridManager.loadData({
                                total: totalNum,
                                rows: list
                            });
                        });
                    }
                },
                'json'
            );
        });

        //字数控制
        $(document).on('keyup', '#contentGroup .datagrid .edit-panel input', function () {
            var $this = $(this);
            var maxTitleCharLength = PhraseConfig.maxGroupNameCharLength();

            if ($this.val() === '') {
                $this.next().removeClass('field-info').addClass('field-error').html('X 分组名称不能为空');
            } else {
                var val = $this.val();
                if (val.length > maxTitleCharLength) {
                    $this.val(val.substr(0, maxTitleCharLength));
                } else {
                    $this.next().removeClass('field-error').addClass('field-info')
                        .html('您还可以输入' + (maxTitleCharLength - val.length) + '字');
                }
            }
        });

        //字数控制
        $('#newGroupName').bind('keyup', function () {
            var $this = $(this);
            var maxTitleCharLength = PhraseConfig.maxGroupNameCharLength();

            if ($this.val() === '') {
                $this.next().removeClass('group-info').addClass('group-error').html('X 分组名称不能为空');
            } else {
                var val = $this.val();
                if (val.length > maxTitleCharLength) {
                    $this.val(val.substr(0, maxTitleCharLength));
                    $this.next().removeClass('group-error').addClass('group-info')
                        .html('您还可以输入0字');
                } else {
                    $this.next().removeClass('group-error').addClass('group-info')
                        .html('您还可以输入' + (maxTitleCharLength - val.length) + '字');
                }
            }
        });

        //过滤分组
        $('#searchGroup').click(function () {
            self.searchGroupList(function (list, totalNum) {
                GroupDataGridManager.loadData({
                    total: totalNum,
                    rows: list
                });
            });
        });
        //修改分组名称-重置
        $('#resetConditionGroup').click(function () {
            $('#groupNamePart').val('');
        });

        var toolbar = $('#phraseEditWindow .toolbar');
        var editArea = toolbar.next('.summary');
        toolbar.find('a').click(function (e) {
            var $this = $(this);
            var act = $this.attr('act');
            var actUp = act + '_up';

            switch (act) {
                //加粗
                case 'b':
                    (function setFontWeight() {
                        if ($this.hasClass(actUp)) {
                            editArea.css('font-weight', 'normal');
                        } else {
                            editArea.css('font-weight', 'bold');
                        }
                    })();
                    break;
                //下划线
                case 'u':
                    (function setTextDecoration() {
                        if ($this.hasClass(actUp)) {
                            editArea.css('text-decoration', 'none');
                        } else {
                            editArea.css('text-decoration', 'underline');
                        }
                    })();
                    break;
                //斜体
                case 'i':
                    (function setFontStyle() {
                        if ($this.hasClass(actUp)) {
                            editArea.css('font-style', 'normal');
                        } else {
                            editArea.css('font-style', 'italic');
                        }
                    })();
                    break;
                //表情
                case 'face':
                    (function openFaceSelect() {
                        $(document).unbind('.im_face_popup');

                        if ($this.hasClass(actUp)) {
                            $('#J_face_popup').hide();
                        } else {
                            $('#J_face_popup').fadeIn(100, function () {
                                $(document).bind('click.im_face_popup', function () {
                                    $('#J_face_popup').hide();
                                    $this.removeClass(actUp);
                                    $(document).unbind('.im_face_popup');
                                    return false;
                                });
                            });
                        }
                    })();
                    break;
                //颜色
                case 'color':
                    (function setColor() {
                        if ($this.hasClass(actUp)) {
                            if ($.data(this, '__colorpicker')) {
                                $this.colorpicker('hidePanel');
                            }
                        } else {
                            if (!$.data(this, '__colorpicker')) {
                                $this.colorpicker({
                                    deltaY: -190,
                                    deltaX: -50,
                                    onHidePanel: function () {
                                        $this.removeClass(actUp);
                                    },
                                    onSelect: function (color) {
                                        editArea.css('color', color);
                                    },
                                    onReset: function () {
                                        editArea.css('color', '#2E2E2E');
                                    }
                                });
                                $.data(this, '__colorpicker', true);
                            }
                            $this.colorpicker('showPanel');
                        }
                    })();
                    break;
            }

            $this.toggleClass(actUp);

            e.preventDefault();
            return false;
        });

        toolbar.find('select').click(function () {
            var $this = $(this);
            var act = $this.attr('act');

            switch (act) {
                case 'font-family':
                    editArea.css('font-family', $this.val());
                    break;
                case 'font-size':
                    editArea.css('font-size', $this.val() + 'px');
                    break;
            }
        });

        $('#J_face_popup img').unbind('.im_face_popup').bind('click.im_face_popup', function () {
            $(document).unbind('.im_face_popup');
            $('#J_face_popup').hide();
            var $img = $(this);

            if ($img.length <= 0)return;
            var face = '<img src="' + $img.attr('src') + '" class="face-img" alt="' + $(this).attr('title') + '"/>';
            editArea.insertAtCaret(face);

            $('#D_left .title .face').removeClass('face_up');
        });

        //删除多条记录
        $('#deletePhrases').click(function () {
            var selectedRows = PhraseDataGridManager.getCheckedRows();

            if (selectedRows.length == 0) {
                AlertUtil.showError('请至少选中一行');
            } else {
                var code = function () {
                    var phraseIds = [];
                    for (var i = 0; i < selectedRows.length; i++) {
                        phraseIds.push(selectedRows[i]['phraseId']);
                    }

                    $.post(
                        PhraseConfig.getBaseReqUrl() + "del-quick-phrase.do",
                        {
                            quickPhraseIds: phraseIds.join(',')
                        },
                        function (resultInfo) {
                            if (resultInfo.success) {
                                PhraseDataGridManager.deleteSelectedRows(phraseIds);
                            } else {
                                AlertUtil.showError(resultInfo.errorMessage || '服务器发生故障');
                            }
                        },
                        'json');
                };

                //删除提示
                GlobalDeleteScheduler.setContext(this).setFunction(code);
                var confirmDeleteWindow = $('#confirmDeleteWindow');
                confirmDeleteWindow.find('.content').html('确定删除这' + selectedRows.length + '行吗？');
                confirmDeleteWindow.show(function () {
                    $(this).window('center').window('open');
                });
            }
        });
    },
    initData: function () {
        this.searchPhraseList(function (list, totalNum) {
            $('#content .datagrid:last-child').hide();
            $('#content .datagrid:first-child').show();
            PhraseDataGridManager.loadData({
                total: totalNum,
                rows: list
            });
        });
    },

    //获取查询参数
    getSearchParams: function (extraParam) {
        var quickPhraseRequest = $.extend({}, {
            quickCode: $('#quickCode').val()
        });
        $.extend(quickPhraseRequest, this._phrasePageOption);
        quickPhraseRequest["quickPhrasePart"] = $('#phrasePart').val();
        quickPhraseRequest["phraseGroupNamePart"] = $('#groupSearch').combobox("getText");
        return quickPhraseRequest;
    },

    //获取分组查询参数
    getGroupSearchParams: function () {
        var groupRequest = {};
        $.extend(groupRequest, this._groupPageOption);
        groupRequest["groupNamePart"] = $('#groupNamePart').val();
        return groupRequest;
    },

    //获取图表显示数据
    //getChartData: function (callback) {
    //    this.searchPhraseList(callback, {
    //        isFetchAll: true,
    //        isShow: true
    //    });
    //},

    //服务请求-获取快捷短语
    searchPhraseList: function (callback, extraParam) {
        var self = this;
        $.post(
            PhraseConfig.getBaseReqUrl() + 'get-quick-phrase.do',
            self.getSearchParams(extraParam),
            function (resultInfo) {
                if (resultInfo.success) {
                    jQuery.each(resultInfo.data.quickPhraseList, function (i, item) {
                        //item.phraseSummary = "<img src=\"http://merchant.feiniu.com/webim/static/images/feiniu-emotion/b37.gif\" class=\"face-img\" alt=\"心碎\">";
                        //todo: wait for wangchen
                    });
                    callback.call(null, resultInfo.data.quickPhraseList || {}, resultInfo.data.totalNum);
                }
            },
            'json'
        );
    },

    //快捷短语分组服务请求
    searchGroupList: function (callback) {
        var self = this;
        $.post(
            PhraseConfig.getBaseReqUrl() + 'get-group-list.do',
            self.getGroupSearchParams(),
            function (resultInfo) {
                if (resultInfo.success) {
                    callback.call(null, resultInfo.data.phraseGroup
                        || {}, resultInfo.data.totalNum);
                }
            },
            'json'
        );
    }
};

//所有数据 表格相关接口提供者
var PhraseDataGridManager = (function () {
    var table = $("#phraseTable");

    return {
        //获取选中的行
        getRow: function (id) {
            var rowArray = table.datagrid('getRows');

            for (var i = 0; i < rowArray.length; i++) {
                if (rowArray[i]['phraseId'] == id) {
                    return rowArray[i];
                }
            }

            return null;
        },
        getCheckedRows: function () {
            return table.datagrid('getChecked').slice();
        },
        getSelectedRow: function (id) {
            var selectedArray = table.datagrid('getSelections');
            for (var i = 0; i < selectedArray.length; i++) {
                if (selectedArray[i]['phraseId'] == id) {
                    return selectedArray[i];
                }
            }

            return null;
        },
        getRowIndex: function (id) {
            var selectedRow = this.getRow(id);
            return table.datagrid('getRowIndex', selectedRow);
        },
        insertRow: function (row) {
            table.datagrid('insertRow', {
                index: 0,
                row: row
            });
        },
        deleteSelectedRows: function (phraseIds) {
            for (var i = 0; i < phraseIds.length; i++) {
                this.deleteRow(phraseIds[i]);
            }
        },
        deleteRow: function (id) {
            table.datagrid('deleteRow', this.getRowIndex(id));
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
                if (rows[i]['phraseId'] == updatedRow.phraseId) {
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
//修改分组名称-表格相关接口提供者
var GroupDataGridManager = (function () {
    var table = $("#groupTable");

    return {
        getSelectedRow: function (id) {
            var selectRowArray = $.extend({}, table.datagrid('getSelected'));

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
        deleteSelectedRow: function (id) {
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

//编辑|添加 快捷短语
var PhraseEditManager = {
    _editMode: 'add',
    _lastSummaryHrefEle: null,
    _lastEditRow: {},

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
                        self.saveLastRange();
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

                self.saveLastRange();
            },
            saveLastRange: function () {
                this.getRange() && (this.lastRange = this.getRange().range);
            },
            getLastRange: function () {
                return this.lastRange || this.getRange();
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
        $(document).on('keyup', '#phraseEditWindow .phrase-field input[name="title"]', function () {
            var $this = $(this);
            var fieldParent = $this.closest('.phrase-field');
            var maxTitleCharLength = PhraseConfig.maxTitleCharLength();

            if ($this.val() === '') {
                fieldParent.find('.phrase-notice').text(maxTitleCharLength + '字以内');
                fieldParent.removeClass('normal').addClass('error');
            } else {
                var val = $this.val();
                if (val.length > maxTitleCharLength) {
                    $this.val(val.substr(0, maxTitleCharLength));
                }

                val = $this.val();
                fieldParent.removeClass('error').addClass('normal');
                fieldParent.find('.phrase-notice').text('您还可以输入' + (maxTitleCharLength - val.length) + '字');
            }
        });
        //描述栏屏蔽右键
        $(document).bind("contextmenu", function (e) {
            return false;
        });

        //快捷短语字数控制
        $(document).on('keydown', '#phraseEditWindow .phrase-field .summary', function (e) {
            var maxSummaryCharLength = PhraseConfig.maxSummaryCharLength();

            var val = self.getEditParams()['phraseSummary'];
            if (e.ctrlKey || val.length == maxSummaryCharLength && e.keyCode != 8 && e.keyCode != 46) {
                e.preventDefault();
                return false;
            }
        });
        //快捷短语字数控制
        $(document).on('keyup', '#phraseEditWindow .phrase-field .summary', function () {
            var $this = $(this);
            var fieldParent = $this.closest('.phrase-field');
            var maxSummaryCharLength = PhraseConfig.maxSummaryCharLength();
            var val = self.getEditParams()['phraseSummary'];

            if (val === '') {
                fieldParent.find('.phrase-notice').text(maxSummaryCharLength + '字以内');
                fieldParent.removeClass('normal').addClass('error');
            } else {
                fieldParent.removeClass('error').addClass('normal');
                fieldParent.find('.phrase-notice').text('您还可以输入' + (maxSummaryCharLength - val.length) + '字');
            }
        });

        //编辑当前问题
        $(document).on('click', '#content .phrase-edit', function (e) {
            var $this = $(this);
            var phraseId = $this.parent().data('phraseid');

            var selectionInfo = PhraseDataGridManager.getRow(phraseId);
            PhraseEditManager.setFormValue(selectionInfo);
            PhraseEditManager.markFormModeEdit();
            $('#phraseEditWindow').show(function () {
                $(this).window('center').window('open');
            });

            e.preventDefault();
            return false;
        });
        //删除当前快捷短语
        $(document).on('click', '#content .phrase-delete', function (e) {
            var $this = $(this);
            var phraseId = $this.parent().data('phraseid');

            var selectedRow = PhraseDataGridManager.getRow(phraseId);
            var code = function () {
                $.post(
                    PhraseConfig.getBaseReqUrl() + "del-quick-phrase.do",
                    {
                        quickPhraseIds: selectedRow.phraseId
                    },
                    function (resultInfo) {
                        if (resultInfo.success) {
                            PhraseDataGridManager.deleteRow(selectedRow.phraseId);
                        } else {
                            AlertUtil.showError(resultInfo.errorMessage || '服务器发生故障');
                        }
                    },
                    'json');
            };

            //删除提示
            GlobalDeleteScheduler.setContext(self).setFunction(code);

            var confirmDeleteWindow = $('#confirmDeleteWindow');
            confirmDeleteWindow.find('.content').html('确定删除这一行吗？<br>快捷编码为：[' + (selectedRow['quickCode'] || '') + ']');
            confirmDeleteWindow.show(function () {
                $(this).window('center').window('open');
            });
            e.preventDefault();
            return false;
        });

        //确认删除
        $('#confirmDelete').click(function () {
            GlobalDeleteScheduler.run();
            $('#confirmDeleteWindow').window('close');
        });

        //取消删除
        $('#cancelDelete').click(function () {
            GlobalDeleteScheduler.cancel();
            $('#confirmDeleteWindow').window('close');
        });
        //位置移动
        //$(document).on('click', '#content a.move-action', function () {
        //    var $this = $(this);
        //    var selectedIndex = ShowedPhraseDataGridManager.getSelectedRowIndex();
        //    var selectedRow = ShowedPhraseDataGridManager.getSelectedRow();
        //    var oldFaqID = -1;
        //    var newIndex = -1;
        //
        //    if ($this.is('.move-up')) {
        //        if (selectedIndex == 0) {
        //            return;
        //        } else {
        //            newIndex = selectedIndex - 1;
        //        }
        //    } else if ($this.is('.move-top')) {
        //        if (selectedIndex == 0) {
        //            return;
        //        } else {
        //            newIndex = 0;
        //        }
        //    } else if ($this.is('.move-down')) {
        //        if (selectedIndex == ShowedPhraseDataGridManager.getCurrentPageRowNo() - 1) {
        //            return;
        //        } else {
        //            newIndex = selectedIndex + 1;
        //        }
        //    } else if ($this.is('.move-bottom')) {
        //        newIndex = ShowedPhraseDataGridManager.getCurrentPageRowNo() - 1;
        //        if (selectedIndex == newIndex) {
        //            return;
        //        }
        //    }
        //
        //    var toReplaceRow = ShowedPhraseDataGridManager.getGivenRow(newIndex);
        //    if (!!toReplaceRow) {
        //        oldFaqID = toReplaceRow['phraseID'];
        //
        //        $.post(
        //            PhraseConfig.getBaseReqUrl() + 'change-phrase-order.do',
        //            {
        //                srcPhraseID: selectedRow['phraseID'],
        //                srcOrder: selectedIndex,
        //                destPhraseID: oldFaqID,
        //                destOrder: newIndex
        //            },
        //            function (resultInfo) {
        //                if (resultInfo.success) {
        //                    ShowedPhraseDataGridManager.exchangeOrder({
        //                        srcOrder: selectedIndex,
        //                        srcRow: $.extend(selectedRow, {showOrder: newIndex}),
        //                        destOrder: newIndex,
        //                        destRow: $.extend(toReplaceRow, {showOrder: selectedIndex})
        //                    });
        //                } else {
        //                    AlertUtil.showError(resultInfo.errorMessage || '服务器发生故障');
        //                }
        //            },
        //            'json'
        //        );
        //    }
        //});

        //编辑快捷短语-提交(update 和 add 两种模式）
        $(document).on('click', '#editPhraseBtn', function () {
            var errorPhraseFields = $('#phraseEditWindow .phrase-field.error');
            if (errorPhraseFields.length > 0) {
                AlertUtil.showError('请填写相关必填字段');
                return;
            } else {
                var serviceUrl = self.isFormModeEdit() ? 'update-quick-phrase.do' : 'add-quick-phrase.do';
                var params = self.getPhrase();

                $.post(
                    PhraseConfig.getBaseReqUrl() + serviceUrl,
                    params,
                    function (resultInfo) {
                        if (resultInfo.success) {
                            if (self.isFormModeEdit()) {
                                PhraseDataGridManager.updateGivenRow(params);
                            } else {
                                PhraseDataGridManager.insertRow(resultInfo.data.quickPhrase);
                            }
                            $('#phraseEditWindow').window('close');
                        } else {
                            AlertUtil.showError(resultInfo.errorMessage || '服务器发生故障');
                        }
                    }
                );

            }
        });
        //编辑快捷短语-返回
        $(document).on('click', '#returnBtn', function () {
            $('#phraseEditWindow').window('close');
        });

        //快捷编码合法性检测
        $(document).on('keyup', '#phraseEditWindow .phrase-field input[name="quickCode"]', function () {
            var $this = $(this);
            var val = $(this).val();
            //中文，字母、数字， 最多4个
            if (val.length > PhraseConfig.maxQuickCodeCharLength()) {
                $this.val(val.substr(0, PhraseConfig.maxQuickCodeCharLength()));
            }
            var reg = new RegExp('^[0-9a-zA-Z\u4E00-\u9FA5]{0,' + PhraseConfig.maxQuickCodeCharLength() + '}$');
            if (reg.test(val)) {
                $this.next().addClass('hide');
            }
            else {
                $this.next().removeClass('hide');
            }
        });
    },
    //获取编辑参数
    getEditParams: function () {
        var editParams = {};

        $('#phraseEditWindow .phrase-field input').each(function () {
            var $this = $(this);
            editParams[$this.attr('name')] = $.isFunction($this.val) ? $this.val() : $this.text();
        });
        var summary = $('#phraseEditWindow .phrase-field .summary').html();
        summary = summary.replace(/[<]span[>]|[<]\/span[>]/gi, '');
        summary = summary.replace(/[<]a[^>]+[>][<]\/a[>]/gi, '');
        summary = summary.replace(/[<]\/?br[>]/gi, '');

        editParams['phraseSummary'] = summary;
        this.isFormModeEdit() && (editParams = $.extend({}, this._lastEditRow, editParams));

        return editParams;
    },
    //获取快捷短语参数
    getPhrase: function () {
        var quickPhrase = {};
        var phraseHtml = $('#phraseEditWindow .phrase-field .summary').html();
        quickPhrase["phraseSummary"] = phraseHtml.replace(/<img[^>]*src="([^"]+\/([^>]+\.\w+))"[^>]*>/gi, function ($0, $1, $2) {
            //if ($1.indexOf('http://') == 0) {
            //    return '<image:' + $1 + '>';
            //} else {
            return '<image:' + $2 + '>';
            //}
        }).replace(/<br\/?>/gi, '\r').replace(/&nbsp;?/gi, ' ').replace(/<\/?[^iI][^>]*>/gi, '')
            .replace(/&lt;/g, '<').replace(/&gt;/g, ' >');

        quickPhrase["phraseId"] = this._lastEditRow.phraseId ;

        quickPhrase["phraseGroupName"] = $('#phraseGroup').combobox('getText');
        quickPhrase["quickCode"] = $('#phraseEditWindow .phrase-field input[name="quickCode"]').val();

        var toolbar = $('#phraseEditWindow .toolbar');
        var editArea = toolbar.next('.summary');
        quickPhrase["fontSize"] = editArea.css("font-size").replace("px", "");
        quickPhrase["fontName"] = editArea.css("font-family");
        var summaryStyle = editArea.css("font-style");
        var summaryDecoration = editArea.css("text-decoration");
        var summaryWeight = editArea.css("font-weight");
        quickPhrase["fontStyle"] = 0;

        //italic
        if (summaryStyle != "normal")
            quickPhrase["fontStyle"] = quickPhrase["fontStyle"] | 0x01;
        //underline
        if (summaryDecoration != "none")
            quickPhrase["fontStyle"] = quickPhrase["fontStyle"] | 0x02;
        //bold
        if (summaryWeight != "400" && summaryWeight != "normal")
            quickPhrase["fontStyle"] = quickPhrase["fontStyle"] | 0x04;

        quickPhrase["fontColor"] = generalUtil.rgbToInt(editArea.css("color"));
        return quickPhrase;
    },

    //分组combobox联想搜索
    searchMachedPhraseGroup: function (value, callback) {
        $.post(
            PhraseConfig.getBaseReqUrl() + "get-group-list.do",
            {
                groupNamePart: value
            },
            function (resultInfo) {
                if (resultInfo && !!resultInfo.success) {
                    callback.call(null, resultInfo.data.phraseGroup);
                } else {
                    showInfo((resultInfo || {}).message || '服务器发生故障，请稍后再试');
                }
            }
        )
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
                phraseId: 0,
                quickCode: '',
                phraseSummary: '',
                phraseGroupName: ''
            };
        }
        this._lastEditRow = row;

        var editWindow = $('#phraseEditWindow');
        editWindow.find('div[name="phraseSummary"]')
            .html(row.phraseSummary.replace(/<image:(b\d+\.gif)>/g, '<img class="face-img" src="' + PageConfig.getEmotionBaseUrl() + '$1">'))
            .trigger('keyup');
        editWindow.find('input[name="quickCode"]').val(row.quickCode).trigger('keyup');
        $('#phraseGroup').combobox('setText', row.phraseGroupName);

    }
    //changeTotalCount: function (delta) {
    //    var totalCountEle = $('#totalPhraseCount');
    //    var oldVal = parseInt(totalCountEle.text());
    //
    //    totalCountEle.text(oldVal + delta);
    //},
    //changeShowedCount: function (delta) {
    //    var showCountEle = $('#showedPhraseCount');
    //    var oldVal = parseInt(showCountEle.text());
    //
    //    showCountEle.text(oldVal + delta);
    //}
};

$(function () {
    PhraseUIManager.init();
    PhraseEditManager.init();
});
