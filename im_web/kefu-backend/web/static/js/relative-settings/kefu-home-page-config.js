/**
 * Author: morningking
 * Date: 2015/10/15
 * Contact: 243717042@qq.com
 */

var PageConfig = (function () {
    var isRuntime = BrowserUtil.getQueryValue(window.location.href, "isRuntime");
    var isPreview = BrowserUtil.getQueryValue(window.location.href, "isPreview");

    var isDesignMode = !( isRuntime || isPreview);

    return {
        isDesignMode: function () {
            return isDesignMode;
        }
    };
})();

var KefuHomePageManager = (function () {
    return {
        init: function () {
            this.initUI();
            this.bindUI();
        },
        initUI: function () {
            $('.publish-control').removeClass('hide');
        },
        bindUI: function () {
            //展开编辑按钮
            $('.editable').mouseover(function () {
                var $this = $(this);
                $this.addClass('hover');

                if ($this.find('.edit-btn').length == 0) {
                    $this.append('<i class="edit-btn btn fa fa-pencil" title="点击编辑当前信息">&nbsp;&nbsp;编辑</i>');
                }
            }).mouseout(function () {
                var $this = $(this);
                $this.removeClass('hover');
            });

            //编辑按钮事件
            $(document).on('click', '.editable>.edit-btn', function () {
                var $this = $(this);
                var editablePanel = $this.closest('.editable');
                var hrefWindow = $('#' + editablePanel.data('href'));

                hrefWindow.show(function () {
                    if (editablePanel.is('.after-sale-board-edit-panel')) {
                        var isBindGroup = editablePanel.find('.edit-field').filter('[name=isBindGroup]').html();
                        var setRequiredInfo = hrefWindow.find('.field-group.group-select .required');
                        if (isBindGroup == '1') {
                            setRequiredInfo.removeClass('hide');
                        } else {
                            setRequiredInfo.addClass('hide');
                        }

                        AfterSaleBoardManager.setCurrentEditGroup(editablePanel);
                    } else if (editablePanel.is('.ad-edit-panel')) {
                        //判断是否是左边ad编辑框
                        var isLeft = editablePanel.prevAll().length == 0;
                        var editField = hrefWindow.find('.edit-field');

                        if (isLeft) {
                            editField.filter('.ad-banner-image-url').attr('name', 'adBannerLeftImageUrl');
                            editField.filter('.ad-banner-image-href').attr('name', 'adBannerLeftImageHref');
                        } else {
                            editField.filter('.ad-banner-image-url').attr('name', 'adBannerRightImageUrl');
                            editField.filter('.ad-banner-image-href').attr('name', 'adBannerRightImageHref');
                        }

                        hrefWindow.find('img').attr('src', editablePanel.find('.edit-field-fake.to-show').attr('src'));
                        setTimeout(function () {
                            IndexConfigManager.initUI();
                        }, 500);
                        IndexConfigManager.setCurrentEditPanel(editablePanel);
                    } else {
                        IndexConfigManager.setCurrentEditPanel(editablePanel);
                    }

                    editablePanel.find('.edit-field').each(function () {
                        var $this = $(this);
                        var fieldName = $this.attr('name');

                        var item = hrefWindow.find('.edit-field[name="' + fieldName + '"]');
                        if (item.length == 0) {
                            item = hrefWindow.find('.edit-field[comboname="' + fieldName + '"]');
                        }

                        if (item.hasClass('easyui-combobox')) {
                            item.combobox('setValue', $this.html());
                        } else if (item.is('input[type=checkbox]')) {
                            item.prop('checked', $this.html() == '1');
                        } else {
                            item.val($this.html()).attr('title', $this.html()).trigger('keyup');
                        }
                    });

                    $(this).window('center').window('open');
                });
            });

            //预览
            $('#preview-edit').click(function () {
                BrowserUtil.openNewTab(window.location.href + '?isPreview=true');
            });

            //发布
            $('#publish-edit').click(function () {
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
        }
    };
})();

//文本描述、广告栏 编辑管理
var IndexConfigManager = (function () {
    var currentEditPanel = null;
    var noticeEditWindow = $('#noticeEditWindow');
    var adBannerEditWindow = $('#adBannerEditWindow');

    var noticeMaxCharLength = 100;
    var isInitBefore = false;

    var acceptedImageWidth = 490;
    var acceptedImageHeight = 200;

    return {
        init: function () {
            this.bindUI();
        },
        initUI: function () {
            if (!isInitBefore) {
                $('#uploadAdPhoto').uploader({
                    action: "upload-baner-image.do",
                    name: 'file',
                    auto: 'true',

                    fileTypeDesc: '请选择图片(.png、.jpg、.gif、.jpeg)',
                    fileTypeExts: 'png,jpg,jpeg,gif',
                    fileSizeLimit: '2M',
                    onStart: function () {
                        var photoURL = '';
                        adBannerEditWindow.find('.edit-field.ad-banner-image-url')
                            .val(photoURL).attr('title', photoURL)
                            .closest('.field-group').find('.field-error').removeClass('hide');

                        $('#preview-banner-image').attr('src', photoURL);
                    },
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
                                    adBannerEditWindow.find('.edit-field.ad-banner-image-url')
                                        .val(photoURL).attr('title', photoURL)
                                        .closest('.field-group').find('.field-error').addClass('hide');

                                    $('#preview-banner-image').attr('src', photoURL);
                                } else {
                                    AlertUtil.showError('图片尺寸不符合要求，宽*高 应为' + acceptedImageWidth + '*' + acceptedImageHeight)
                                }
                            } else {
                                AlertUtil.showError('服务器发生故障，请稍后重试');
                            }
                        }
                    }
                });
                isInitBefore = true;
            }
        },
        bindUI: function () {
            var self = this;

            //取消编辑 文字编辑区
            $('#cancelEditNotice').click(function () {
                noticeEditWindow.window('close');
            });

            //取消编辑 广告图片
            $('#cancelEditAdBanner').click(function () {
                adBannerEditWindow.window('close');
            });

            //文字编辑字数控制
            noticeEditWindow.find('.edit-field').on('keyup', function () {
                var $this = $(this);

                var fieldGroup = $this.closest('.field-group');
                var value = $this.val();

                if (fieldGroup.find('.field-label > .required:visible').length > 0) {
                    if (value == '') {
                        fieldGroup.find('.field-error').removeClass('hide');
                    } else {
                        fieldGroup.find('.field-error').addClass('hide');
                    }
                }

                var remainingLength = noticeMaxCharLength - value.length;
                remainingLength < 0 && (remainingLength = 0);

                if (value == '') {
                    fieldGroup.find('.field-info').html('（' + remainingLength + '个字以内）');
                } else {
                    fieldGroup.find('.field-info').html('（您还可以输入' + remainingLength + '个字）');
                }
            });
            //提交 文字编辑
            $('#saveEditNotice').click(function () {
                if (self.checkNoticeParams()) {
                    var params = self.getParams();

                    RemoteServiceInvokeUtil.invokePost(
                        'update-index-config.do',
                        params,
                        function () {
                            this.syncData(noticeEditWindow, params);
                            noticeEditWindow.window('close');
                        },
                        function (resultInfo) {
                            AlertUtil.showError(resultInfo.errorMesssage)
                        },
                        self
                    );
                }
            });

            //广告点击链接校验
            adBannerEditWindow.find('.ad-banner-image-href').on('keyup', function () {
                var $this = $(this);
                var val = $this.val();

                var fieldErrorIndicator = $this.closest('.field-group').find('.field-error');
                if (!RegexpUtil.isLegalURL(val)) {
                    fieldErrorIndicator.removeClass('hide')
                } else {
                    fieldErrorIndicator.addClass('hide')
                }
            });
            //提交广告链接编辑
            $('#saveEditAdBanner').click(function () {
                if (self.checkAdParams()) {
                    var params = self.getParams();

                    RemoteServiceInvokeUtil.invokePost(
                        'update-index-config.do',
                        params,
                        function () {
                            adBannerEditWindow.find('.ad-banner-image-url').attr('name', 'adBannerImageUrl');
                            adBannerEditWindow.find('.ad-banner-image-href').attr('name', 'adBannerImageHref');

                            var changedParams = $.extend(self.getParams(), params);
                            this.syncData(adBannerEditWindow, changedParams);

                            adBannerEditWindow.window('close');
                        },
                        function (resultInfo) {
                            AlertUtil.showError(resultInfo.errorMesssage)
                        },
                        self
                    );
                }
            });
        },
        setCurrentEditPanel: function (panel) {
            currentEditPanel = panel;
        },
        checkNoticeParams: function () {
            var notice = noticeEditWindow.find('.edit-field').val();
            if (notice === '') {
                noticeEditWindow.find('.field-error').removeClass('hide');

                return false;
            }

            return true;
        },
        checkAdParams: function () {
            var fieldGroupList = adBannerEditWindow.find('.field-group');
            for (var i = 0; i < fieldGroupList.length; i++) {
                var fieldGroup = $(fieldGroupList[i]);
                var fieldVal = fieldGroup.find('.edit-field').val();

                if (fieldVal == '' || !RegexpUtil.isLegalURL(fieldVal)) {
                    fieldGroup.find('.field-error').removeClass('hide');
                    return false;
                }
            }

            return true;
        },
        getParams: function () {
            var params = {};

            $('.notice-edit-panel').add('.ad-edit-panel').find('.edit-field').each(function () {
                var $this = $(this);
                params[$this.attr('name')] = $this.html();
            });

            var activeWindow = noticeEditWindow.is(':visible') ? noticeEditWindow : adBannerEditWindow;
            activeWindow.find('.edit-field').each(function () {
                var $this = $(this);
                params[$this.attr('name')] = $this.val();
            });

            return params;
        },
        syncData: function (panel, params) {
            if (!!currentEditPanel && !!params && !$.isEmptyObject(params)) {
                currentEditPanel.find('.edit-field,.edit-field-fake').each(function () {
                    var $this = $(this);
                    var name = $this.attr('name');
                    var value = params[name];

                    value == null && (value = '');

                    if ($this.is('.to-href')) {
                        $this.attr('href', value);
                    } else if ($this.is('.to-show')) {
                        $this.attr('src', value);
                    } else {
                        $this.html(value);
                    }
                });
            }
        }
    }

})();

//售后版块 编辑管理
var AfterSaleBoardManager = (function () {
    var afterSaleBoardEditWindow = $('#groupEditWindow');
    var isDescriptionItemPattern = /description_\d/;
    var maxCharLength = 8;
    var currentEditGroup = null;

    return {
        init: function () {
            this.initUI();
            this.bindUI();
        },
        initUI: function () {
            $('#groupID').combobox({
                'onSelect': function () {
                    var $this = $(this);
                    var isBindGroupChecked = $('#isBindGroupCheckBox').is(':checked');
                    var fieldGroup = $this.closest('.field-group');

                    if (isBindGroupChecked) {
                        if ($this.combobox('getValue') == '-1') {
                            fieldGroup.find('.field-error').removeClass('hide');
                        } else {
                            fieldGroup.find('.field-error').addClass('hide');
                        }
                    }
                }
            });
        },
        bindUI: function () {
            var self = this;

            //编辑时字数提示
            $(document).on('keyup', '#groupEditWindow .field-group .edit-field', function () {
                var $this = $(this);

                if ($this.is('input')) {
                    var fieldGroup = $this.closest('.field-group');
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

            //如果绑定分组时，设置必须选择分组信息
            $('#isBindGroupCheckBox').click(function () {
                var $this = $(this);
                var isChecked = $this.is(':checked');

                var requiredIndicator = $this.closest('.field-group').next('.field-group').find('.field-label > .required');
                if (isChecked) {
                    requiredIndicator.removeClass('hide');
                } else {
                    requiredIndicator.addClass('hide');
                    $('#groupID').combobox('setValue', -1);
                }
            });

            //售后分组版块提交
            $('#saveEditGroup').click(function () {
                if (self.checkParams()) {
                    var params = self.getParams();

                    $.post(
                        'update-after-sale-board.do',
                        params,
                        function (resultInfo) {
                            if (resultInfo.success) {
                                self.syncData(params);
                                afterSaleBoardEditWindow.window('close');
                            } else {
                                AlertUtil.showError(resultInfo.errorMessage || '服务器发生故障');
                            }
                        },
                        'json'
                    );
                }
            });

            //关闭 售后分组版块编辑窗口
            $('#cancelEditGroup').click(function () {
                afterSaleBoardEditWindow.window('close');
            });
        },
        checkParams: function () {
            var fieldGroupList = afterSaleBoardEditWindow.find('.field-group');

            for (var i = 0; i < fieldGroupList.length; i++) {
                var currentGroup = $(fieldGroupList[i]);

                //如果是必填项
                if (currentGroup.find('.field-label > .required:visible').length > 0) {
                    var input = currentGroup.find('.edit-field');

                    if (input.is('input')) {
                        if (input.val() === '') {
                            currentGroup.find('.field-error').removeClass('hide');
                            return false;
                        }
                    } else {
                        if (input.combobox('getValue') === '-1') {
                            currentGroup.find('.field-error').removeClass('hide');
                            return false;
                        }
                    }
                }
            }

            return true;
        },
        getParams: function () {
            var params = {descriptionListJson: ''};
            var descriptionList = [];

            afterSaleBoardEditWindow.find('.edit-field').each(function () {
                var $this = $(this);
                var fieldName = $this.attr('name');

                if ($this.is('input')) {
                    var value = $this.val();
                    if ($this.is('[type="checkbox"]')) {
                        value = $this.is(':checked') ? '1' : '0';
                    }

                    params[fieldName] = value;

                    if (isDescriptionItemPattern.test(fieldName) && !!value) {
                        descriptionList.push({v: value});
                    }
                } else if ($this.is('select')) {
                    fieldName = $this.attr('comboname');
                    value = $this.combobox('getValue');
                    params[fieldName] = value;
                }
            });

            params.descriptionListJson = JSON.stringify(descriptionList);

            return params;
        },
        setCurrentEditGroup: function (editGroup) {
            currentEditGroup = editGroup;
        },
        syncData: function (params) {
            if (!!currentEditGroup && !!params && !$.isEmptyObject(params)) {
                currentEditGroup.find('.edit-field').each(function () {
                    var $this = $(this);
                    var name = $this.attr('name');
                    var value = params[name];

                    value == null && (value = '');
                    $this.html(value);
                });
            }
        }
    };
})();

$(document).ready(function () {
    if (PageConfig.isDesignMode()) {
        KefuHomePageManager.init();
        IndexConfigManager.init();
        AfterSaleBoardManager.init();
    } else {
        $('.btn-bind-order').click(function () {
            new Popup({
                "title": '<div class="popu-head"><p class="popu-head-p">请选择订单信息</p></div>',
                "content": $('#p_im'),
                "sign": 1
            });
        });
    }
});
