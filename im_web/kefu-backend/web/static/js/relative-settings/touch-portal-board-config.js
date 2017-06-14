/**
 * Author: morningking
 * Date: 2016/3/9
 * Contact: 243717042@qq.com
 */
var TouchPortalBoardConfigurator = (function () {
    var moduleTemplate = '<div class="module" id="module-$id"> ' +
        '<a class="fc-m-backstage-type"> <img class="icon" src="$icon"/> ' +
        '<div class="word"> <p class="tlt">$title</p> ' +
        '<p class="detail">$description</p> ' +
        '</div> </a> ' +
        '<div class="baracts"> ' +
        '<div class="baracts-bg"></div> ' +
        '<div class="baracts-rt"> ' +
        '<a href="javascript:;" class="ba-btn ba-bar-remove"></a> ' +
        '<a href="javascript:;" class="ba-btn ba-bar-moveup"></a> ' +
        '<a href="javascript:;" class="ba-btn ba-bar-edit"></a> ' +
        '</div> </div> </div>';
    var boardEditWindow = $('#boardEditWindow');
    var moduleList = $('#moduleList');
    var SystemEnv = (function () {
        var url = window.location.href;
        var isPreview = BrowserUtil.getQueryValue(url, "isPreview") || BrowserUtil.getQueryValue(url, "isRuntime");

        return {
            isPreview: function () {
                return isPreview;
            }
        }
    })();

    return {
        init: function () {
            this.initUI();
            this.bindUI();
        },
        initUI: function () {
            moduleList.find('.module').each(function (index) {
                var $this = $(this);
                var module = Base64.decode($this.data('module'));

                $this.data('module', JSON.parse(module));

                if (index == 0) {
                    $this.find('.ba-btn.ba-bar-moveup').remove();
                }
                if ($this.next('.module').length == 0) {
                    $this.find('.ba-btn.ba-bar-movedown').remove();
                }
            });

            //删除调度管理器
            this.deleteScheduler = new GlobalDeleteScheduler();
        },
        bindUI: function () {
            if (SystemEnv.isPreview()) {
                moduleList.find('.module').each(function () {
                    $(this).find('.baracts').remove();
                });

                $('#addModuleBtn').remove();
                return;
            }

            var me = this;

            //版块编辑[增删改]
            (function setupBoardConfig() {
                //编辑时字数提示
                $(document).on('keyup', '#boardEditWindow .field-group .edit-field', function () {
                    var $this = $(this);

                    if ($this.is('input')) {
                        var fieldGroup = $this.closest('.field-group');
                        var maxCharLength = $this.attr('maxlength');
                        var value = $this.val();

                        if (fieldGroup.find('.field-label > .required:visible').length > 0) {
                            if (value == '') {
                                fieldGroup.find('.field-error').removeClass('hide');
                            } else {
                                fieldGroup.find('.field-error').addClass('hide');
                            }
                        }

                        var remainingLength = maxCharLength - value.length;
                        remainingLength < 0 && (remainingLength = 0);

                        if (value == '') {
                            fieldGroup.find('.field-info').html('（' + remainingLength + '个字以内）');
                        } else {
                            fieldGroup.find('.field-info').html('（您还可以输入' + remainingLength + '个字）');
                        }
                    }
                });

                //删除版块
                $(document).on('click', '.module .ba-bar-remove.ba-btn', function (e) {
                    var code = function () {
                        var module = $(this).closest('.module');
                        var id = module.data('id');

                        if (!!id) {
                            RemoteServiceInvokeUtil.invokePost(
                                'delete',
                                {
                                    id: id
                                },
                                function success() {
                                    //更新module后面节点的顺序
                                    module.nextAll().each(function () {
                                        var moduleData = $(this).data('module');

                                        moduleData.showOrder -= 1;
                                    });

                                    //如果是第一个模块
                                    if (module.prevAll().length == 0 && module.next().length > 0) {
                                        module.next().find('.ba-btn.ba-bar-moveup').remove();
                                    } else if (module.nextAll().length == 0 && module.prev().length > 0) {//如果是最后一个模块
                                        module.prev().find('.ba-btn.ba-bar-movedown').remove();
                                    }

                                    module.remove();
                                    $('#confirmDeleteWindow').window('close');
                                    AlertUtil.showInfo('删除成功');
                                },
                                function error(resultInfo) {
                                    AlertUtil.showError(resultInfo.errorMessage);
                                }
                            );
                        }
                    };

                    me.deleteScheduler.setContext(this).setFunction(code);

                    $('#confirmDeleteWindow').show(function () {
                        $(this).window('center').window('open');
                    });

                    e.preventDefault();
                    return false;
                });

                //调整顺序
                $(document).on('click', '.module .ba-btn.ba-bar-moveup, .module .ba-btn.ba-bar-movedown', function () {
                    var $this = $(this);
                    var srcModule = $this.closest('.module');
                    var isMoveDown = $(this).is('.ba-bar-movedown');
                    var srcModuleID = srcModule.data('id');
                    var destModuleID;
                    var destModule;

                    if (isMoveDown) {
                        var nextModule = srcModule.next('.module');
                        if (nextModule.length > 0) {
                            destModule = nextModule;
                            destModuleID = nextModule.data('id');
                        } else {
                            return;
                        }
                    } else {
                        var prevModule = srcModule.prev('.module');
                        if (prevModule.length > 0) {
                            destModule = prevModule;
                            destModuleID = prevModule.data('id');
                        } else {
                            return;
                        }
                    }

                    RemoteServiceInvokeUtil.invokePost(
                        'change-order.do',
                        {
                            srcID: srcModuleID,
                            destID: destModuleID
                        },
                        function success() {
                            //交换位置 + 调整值
                            if (isMoveDown) {
                                srcModule.insertAfter(destModule);
                                srcModule.data('module').showOrder += 1;
                                destModule.data('module').showOrder -= 1;
                            } else {
                                srcModule.insertBefore(destModule);
                                srcModule.data('module').showOrder -= 1;
                                destModule.data('module').showOrder += 1;
                            }

                            var srcModuleOperations = srcModule.find('.baracts-rt').remove();
                            var destModuleOperations = destModule.find('.baracts-rt').remove();

                            srcModuleOperations.insertAfter(destModule.find('.baracts-bg'));
                            destModuleOperations.insertAfter(srcModule.find('.baracts-bg'));
                        },
                        function error(resultInfo) {
                            AlertUtil.showError(resultInfo.errorMessage);
                        }
                    )
                });

                //确认删除
                $('#confirmDelete').click(function () {
                    me.deleteScheduler.run();
                });

                //取消删除
                $('#cancelDelete').click(function () {
                    me.deleteScheduler.cancel();
                    $('#confirmDeleteWindow').window('close');
                });
            })();

            //发布、预览、添加、编辑
            (function setupPublish() {
                var isImageControlInited = false;

                /**
                 * 显示编辑版块
                 * @param isModeEdit 编辑模式：新增/修改
                 * @param board 如果为修改模式，此项为修改前的值
                 */
                function showEditPanel(isModeEdit, board) {
                    boardEditWindow.show(function () {
                        var $this = $(this);

                        //1. 填充表单
                        (function fillForm() {
                            $this.find('.field-group .edit-field').each(function () {
                                var self = $(this);
                                var name = self.attr('name') || self.attr('comboname');
                                var value = board[name];

                                if (self.is('.photo-field')) {
                                    self.attr('src', board[name] || '');
                                } else if (self.is('input[type=checkbox]')) {
                                    self.prop('checked', '1' == value);
                                } else if (self.hasClass('easyui-combobox')) {
                                    self.combobox('setValue', null != value ? value : '-1');
                                } else {
                                    self.val(board[name]);
                                }
                            });
                        })();

                        //2. 打开编辑窗口
                        (function openEditWindow() {
                            boardEditWindow.data('id', board != null ? board.id : null);
                        })();

                        //3. 如果图片上传控件没初始化，初始化之
                        (function initUploadControlIfNeed() {
                            $this.data('mode', isModeEdit).window('center').window('open');
                            if (!isImageControlInited) {
                                //图片上传初始化
                                (function setupImageUpload() {
                                    var acceptedImageHeight = 80;
                                    var acceptedImageWidth = 80;

                                    $('#uploadAdPhoto').uploader({
                                        action: "../kefu-home-page-config/upload-baner-image.do",
                                        name: 'file',
                                        auto: 'true',

                                        fileTypeDesc: '请选择图片(.png、.jpg、.gif、.jpeg)',
                                        fileTypeExts: 'png,jpg,jpeg,gif',
                                        fileSizeLimit: '2M',
                                        onError: function (e) {
                                            AlertUtil.showError(e.message);
                                        },
                                        onSuccess: function (e) {
                                            if (e.data) {
                                                var result = eval('(' + e.data + ')');

                                                if (result.success) {
                                                    var data = result.data;
                                                    var photoURL = data.uploadedURL;
                                                    var isLegal = RegexpUtil.isLegalURL(photoURL)
                                                        && data.imageWidth == acceptedImageWidth
                                                        && data.imageHeight == acceptedImageHeight;

                                                    if (isLegal) {
                                                        $('#boardIcon').attr('src', photoURL)
                                                            .closest('.field-group').find('.field-error').addClass('hide');
                                                    } else {
                                                        AlertUtil.showError('图片尺寸不符合要求，宽*高 应为' + acceptedImageWidth + '*' + acceptedImageHeight)
                                                    }
                                                } else {
                                                    AlertUtil.showError('服务器发生故障，请稍后重试');
                                                }
                                            }
                                        }
                                    });
                                })();

                                isImageControlInited = true;
                            }
                        })();
                    });
                }

                //添加版块
                $('#addModuleBtn').click(function () {
                    showEditPanel(false, {});
                });

                //编辑版块
                $(document).on('click', '.baracts .ba-btn.ba-bar-edit', function () {
                    var module = $(this).closest('.module');

                    showEditPanel(true, module.data('module'));
                });

                //编辑版块 保存
                $('#saveEditBoard').click(function () {
                    var checkResult = me.checkParams();

                    if (!checkResult) {
                        AlertUtil.showError('请填写相关必填字段');
                    } else {
                        var isModeEdit = boardEditWindow.data('mode');
                        var editMode = isModeEdit ? 'modify' : 'add';
                        var param = me.getParams();
                        if ('add' === editMode) {
                            var lastModule = moduleList.children('.module:last-child');

                            if (lastModule.length == 0) {
                                param.showOrder = 0;
                            } else {
                                param.showOrder = lastModule.data('module').showOrder + 1;
                            }
                        }

                        RemoteServiceInvokeUtil.invokePost(
                            editMode,
                            param,
                            function success(data) {
                                if ('add' === editMode) {
                                    param.id = data.config.id;
                                    var module = $(StringUtil.replacedWithObjectValue(moduleTemplate, param));
                                    module.data('id', param.id).data('module', param);

                                    moduleList.append(module);
                                    $('<a href="javascript:;" class="ba-btn ba-bar-movedown"></a>')
                                        .insertBefore(module.prev('.module').find('.ba-btn.ba-bar-edit'));
                                } else {
                                    var module = $('#module-' + param.id);
                                    module.find('img.icon').attr('src', param.icon);
                                    module.find('.tlt').html(param.title);
                                    module.find('.detail').html(param.description);

                                    module.data('module', param);
                                }

                                boardEditWindow.window('close');
                            },
                            function error(resultInfo) {
                                AlertUtil.showError(resultInfo.errorMessage);
                            }
                        );
                    }
                });

                //编辑版块 取消
                $('#cancelEditBoard').click(function () {

                });

                //预览
                $('#preview').click(function () {
                    BrowserUtil.openNewTab(window.location.href + '?isPreview=true');
                });

                //发布
                $('#publish').click(function () {
                    RemoteServiceInvokeUtil.invokePost(
                        'publish.do',
                        {},
                        function () {
                            AlertUtil.showInfo('发布成功');
                        },
                        function (resultInfo) {
                            AlertUtil.showError('发布失败：' + resultInfo.errorMessage);
                        }
                    );
                });
            })();
        },
        checkParams: function () {
            var fieldGroupList = boardEditWindow.find('.field-group');

            for (var i = 0; i < fieldGroupList.length; i++) {
                var currentGroup = $(fieldGroupList[i]);

                //如果是必填项
                if (currentGroup.find('.field-label > .required:visible').length > 0) {
                    var input = currentGroup.find('.edit-field');

                    if (input.is('input')) {
                        if (input.val() === '') {
                            currentGroup.find('.field-error').removeClass('hide');
                            input.focus();
                            return false;
                        }
                    } else if (input.is('.photo-field')) {
                        var src = input.attr('src');
                        if (!src) {
                            currentGroup.find('.field-error').removeClass('hide');
                            input.focus();
                            return false;
                        }
                    } else {
                        if (input.combobox('getValue') === '-1') {
                            currentGroup.find('.field-error').removeClass('hide');
                            input.focus();
                            return false;
                        }
                    }
                }
            }

            return true;
        },
        getParams: function () {
            var params = {'dataType': 0};
            var id = boardEditWindow.data('id');
            !!id && (params.id = id);

            boardEditWindow.find('.edit-field').each(function () {
                var $this = $(this);
                var fieldName = $this.attr('name');

                if ($this.is('input')) {
                    var value = $this.val();
                    if ($this.is('[type="checkbox"]')) {
                        value = $this.is(':checked') ? '1' : '0';
                    }

                    params[fieldName] = value;
                } else if ($this.is('select')) {
                    fieldName = $this.attr('comboname');
                    value = $this.combobox('getValue');
                    params[fieldName] = value;
                } else if ($this.is('.photo-field')) {
                    params[fieldName] = $this.attr('src');
                }
            });

            return params;
        }
    }
})();

$(function bootstrap() {
    TouchPortalBoardConfigurator.init();
});
