/* ============================== */
/*
 author      : HQY
 date        : 2014-11-07
 qq          : 363395863
 email       : hqy332211@163.com
 */
/* ============================== */

define(function (require, exports, module) {
    require('$');
    require('jquery/colorpicker/jquery-colorpicker');
    require('jquery/jplayer/jquery.jplayer');
    require('jquery/uploader/jquery.uploader.js');

    var dialog = require('jquery/dialog/jquery-dialog');
    var generalUtil = require('modules/util/generalUtil');

    $.browser = {};
    $.browser.mozilla = /firefox/.test(navigator.userAgent.toLowerCase());
    var isIe = !!window.ActiveXObject || "ActiveXObject" in window;
    var currentEvaluationDialog;

    var audioHandler = {
        _canPlay: true,
        audioPlayer: {},
        toggleAudioSwitch: function () {
            this._canPlay = !this._canPlay;
        },
        checkIfCanPlay: function () {
            return this._canPlay;
        },
        init: function () {
            this.audioPlayer = $('#audioPlayerPanel');
            this.audioPlayer.jPlayer({
                ready: function (event) {
                    $(this).jPlayer("setMedia", {
                        m4a: "static/sound/message.m4a",
                        mp3: "static/sound/message.mp3",
                        wav: "static/sound/message.wav",
                        ogg: "static/oggsound/message.ogg"
                    });
                },
                autohide: {
                    restored: false
                },
                swfPath: "static/swf/",
                supplied: "wav,ogg, m4a, mp3"
            });
        },
        play: function () {
            if (this.checkIfCanPlay()) {
                var self = this;
                setTimeout(function () {
                    try {
                        self.audioPlayer.jPlayer('play');
                    } catch (e) {
                        alert(e);
                    }
                }, 30);
            }
        }
    };

    /**
     * ====================================================
     * 监听窗口Resize初始化
     * ====================================================
     */
    function initResize(opts) {
        var windowSize = null,
            content = $('.content, .person'),
            navHeight = $('#nav').outerHeight(),
            layout = $('.all'),
            dialog = $('.top', content),
            typeHeight = $('.typing', content).outerHeight();

        exports.resize = function () {
            var docWidth = Math.max($(window).width(), 620);
            var docHeight = Math.max($(window).height(), 542);
            var size = docWidth + 'x' + docHeight;
            if (windowSize == size) return;
            windowSize = size;
            var contentHeight = docHeight - navHeight - 2;
            layout.width(docWidth);
            content.height(contentHeight);
            dialog.height(contentHeight - typeHeight);
        }

        exports.resize();

        $(window).unbind('resize.imResize').bind('resize.imResize', function () {
            exports.resize();
        });

    }

    /**
     * ====================================================
     * 工具栏点击
     * ====================================================
     */
    function initToolbar(options) {
        /**
         * ====================================================
         * 表情
         * ====================================================
         */
        /*初始化表情,取消事件冒牌*/
        $('#J_face_popup').unbind('.im_face_popup').bind('click.im_face_popup', function () {
            return false;
        });
        /*添加表情*/
        $('#J_face_popup img').unbind('.im_face_popup').bind('click.im_face_popup', function () {
            $(document).unbind('.im_face_popup');
            $('#J_face_popup').hide();
            var $img = $(this);

            if ($img.length <= 0)return;
            var face = '<img src="' + $img.attr('src') + '" class="face-img" alt="' + $(this).attr('title') + '"/>';
            $('.textarea').insertAtCaret(face);

            $('#D_left .title .face').removeClass('face_up');
        });

        $('#audioSwitcher').unbind('click.audio').bind('click.audio', function () {
            audioHandler.toggleAudioSwitch();
        });

        $('#photoUploader i').uploader({
            action: 'upload-photo.do',
            swf: 'uploader.swf',
            name: 'img',
            preview: true,
            showQueue: '#uploadQueue',
            auto: 'true',
            fileTypeDesc: '请选择一张大小在2M内的图片，文件类型可以为(jpg、jpeg、gif、png、tif)',
            fileTypeExts: 'jpg,jpeg,gif,png,tif',
            fileSizeLimit: '2M',
            formData: {
                userID: options.userID
            },
            onError: function (e) {
                alert(e.message);
            },
            onSuccess: function (e) {

                if (e.data) {
                    var result = eval('(' + e.data + ')');

                    if (result.success) {
                        $('.typing .textarea').append('<img src="' + result.data.imageURL + '">');
                    } else {
                        alert('服务器发生故障，请稍后重试');
                    }
                }
            }
        });

        /**
         * ====================================================
         * 字体
         * ====================================================
         */
        $('#J_ti_tb .font a,#J_ti_tb .cha').each(function () {
            var $target = $(this),
                type = $target.attr('act'),
                clickClz = type + '_up';

            $target.unbind('click.im_font_style').bind('click.im_font_style', function (e) {
                //关闭判断
                if (type == 'cha') {
                    $('#J_ti_tb').hide();
                    return;
                }

                //bold
                if (type == 'b') {
                    if ($target.hasClass(clickClz)) {
                        $('.textarea').css('font-weight', 'normal');
                    } else {
                        $('.textarea').css('font-weight', 'bold');
                    }
                }

                //i
                if (type == 'i') {
                    if ($target.hasClass(clickClz)) {
                        $('.textarea').css('font-style', 'normal');
                    } else {
                        $('.textarea').css('font-style', 'italic');
                    }
                }

                //u
                if (type == 'u') {
                    if ($target.hasClass(clickClz)) {
                        $('.textarea').css('text-decoration', 'normal');
                    } else {
                        $('.textarea').css('text-decoration', 'underline');
                    }
                }

                //color
                try {
                    if (type == 'color') {
                        if ($target.hasClass(clickClz)) {
                            if ($.data(this, '__colorpicker')) {
                                $target.colorpicker('hidePanel');
                            }
                        } else {
                            if (!$.data(this, '__colorpicker')) {
                                $target.colorpicker({
                                    deltaY: -190,
                                    deltaX: -50,
                                    onHidePanel: function () {
                                        $target.removeClass(clickClz);
                                    },
                                    onSelect: function (color) {
                                        $('.textarea').css('color', color);
                                    },
                                    onReset: function () {
                                        $('.textarea').css('color', '#2E2E2E');
                                    }
                                });
                                $.data(this, '__colorpicker', true);
                            }
                            $target.colorpicker('showPanel');
                        }
                    }
                } catch (ex) {
                    e.preventDefault();
                    return false;
                }
                ;

                $target.toggleClass(clickClz);

                e.preventDefault();
                return false;
            });
        });

        /*字体类型*/
        $('.font-family-select').unbind('.im_font_change').bind('change.im_font_change', function () {
            $('.textarea').css('font-family', $(this).val());
        });

        /*字体大小*/
        $('.font-size-select').unbind('.im_font_change').bind('change.im_font_change', function () {
            $('.textarea').css('font-size', parseInt($(this).val()));
        });

        /**
         * ====================================================
         * 工具栏初始化
         * ====================================================
         */
        /*工具栏*/
        $('#D_left .title a:not(.send-photo)').each(function () {
            var $target = $(this),
                type = $target.attr('act'),
                clickClz = type + '_up';

            $target.bind('click.imAction', function (e) {
                // 表情判断
                if (type == 'face') {
                    $(document).unbind('.im_face_popup');

                    if ($target.hasClass('clickClz')) {
                        $('#J_face_popup').hide();
                    } else {
                        $('#J_face_popup').fadeIn(100, function () {
                            $(document).bind('click.im_face_popup', function () {
                                $('#J_face_popup').hide();
                                $target.removeClass(clickClz);
                                $(document).unbind('.im_face_popup');
                                return false;
                            });
                        });
                    }
                }

                // 字体判断
                if (type == 'ti') {
                    $('#J_ti_tb').toggle();
                }

                $target.toggleClass(clickClz);

                e.preventDefault();
                return false;
            });
        });
    }

    function retrieveFontStyleInfo($target) {
        if (!$target) {
            $target = $('#D_left .typing div.textarea').eq(0);
        }
        var features = {};

        $('#J_ti_tb .font a').each(function (index, element) {
            var $this = $(this);
            var type = $this.attr('act');
            var selectClass = type + "_up";

            switch (type) {
                case 'b':
                    if ($this.hasClass(selectClass)) {
                        features['weight'] = 1;
                    } else {
                        features['weight'] = 0;
                    }
                    break;
                case 'i':
                    if ($this.hasClass(selectClass)) {
                        features['style'] = 1;
                    } else {
                        features['style'] = 0;
                    }
                    break;
                case 'u':
                    if ($this.hasClass(selectClass)) {
                        features['line'] = 1;
                    } else {
                        features['line'] = 0;
                    }
                    break;
                case 'color':
                    features['color'] = generalUtil.rgbToInt($target.css('color'));
                    break;
            }
        });
        (function getSelectValue() {
            var fontFamily = $('#J_ti_tb .choose select.font-family-select').val();
            var fontSize = $('#J_ti_tb .choose select.font-size-select').val();

            features['family'] = fontFamily;
            features['size'] = fontSize;
        })();

        return features;
    }

    /**
     * ====================================================
     * 文本框初始化
     * ====================================================
     */
    function initText(options) {

        $('#J_send_state').unbind('.im_send_state').bind('click.im_send_state', function () {
            $(document).unbind('.im_send_state');
            if ($('#J_send_state_warp').is(':visible')) {
                $('#J_send_state_warp').hide();
            } else {
                $('#J_send_state_warp').fadeIn(100, function () {
                    $(document).bind('click.im_send_state', function () {
                        $('#J_send_state_warp').hide();
                        $(document).unbind('.im_send_state');
                    });
                });
            }
        });

        $('#J_send').unbind('.im_send').bind('click.im_send', function () {
            sendMessage($('.textarea'));
        });

        var btns = [];
        $('#J_send_state_warp a').each(function () {
            var $target = $(this),
                id = $target.attr('id');
            btns[id] = $target;

            $target.unbind('.im_send_state').bind('click.im_send_state', function () {
                $('#J_send_state_warp a').removeClass('gou');
                btns[id].addClass('gou');
                if ('J_send_state_enter' == id) {
                    switchEnterState();
                } else {
                    switchCtrEnterState();
                }
                return false;
            });
        });

        function sendMessage($target) {
            if (typeof options.onSendingMessage == 'function') {
                if ($target.length == 0) {
                    return;
                }

                if ($target.html().length == 0 || $target.html() === '<br>') {
                    alert('消息内容不能为空');
                    $target.focus();
                    return;
                }

                $target = $target.eq(0);

                var messageContent = "";
                !!$target && (function () {
                    if ($.browser.mozilla) {
                        messageContent = $target.html().replace(/<[dD][iI][vV][^<>]+>[^<]*<\/[dD][iI][vV]>/g, '');
                        //messageContent = messageContent.replace(/<\/?a [^>]*>/gi, '');
                    } else {
                        var blocks = $target.children();
                        if (blocks.length == 0) {
                            messageContent = $target.html();
                        } else {
                            messageContent = $target.html().
                                replace(/\r\n/gi, '').
                                replace(/<\/?div [^>]*>/gi, '').
                                //replace(/<\/?a [^>]*>/gi, '').
                                replace(/<p [^>]*>/gi, '').replace(/<\/p[^>]*>/gi, '<br>');
                        }
                    }
                })();

                var features = retrieveFontStyleInfo($target);

                options.onSendingMessage.call($target, messageContent, features);
            }
        }

        $('.textarea').bind('keypress', function () {
            TitleHandler.clear();
        }).bind('click', function () {
            TitleHandler.clear();
        });

        function switchEnterState() {
            $('.textarea').unbind('.im_write').bind('keypress.im_write', function (e) {
                if (e.which == 13 || e.which == 10) {
                    if (e.ctrlKey) {
                        var $this = $(this);
                        $this.insertAtCaret('<br>');
                        return false;
                    } else {
                        sendMessage($(this));
                        e.preventDefault();
                        return false;
                    }
                }
            });
        }

        function switchCtrEnterState() {
            $('.textarea').unbind('.im_write').bind('keypress.im_write', function (e) {
                if (e.ctrlKey && (e.which == 13 || e.which == 10)) {
                    sendMessage($(this));
                    return false;
                }
            });
        }

        switchEnterState();
    }

    /**
     * ====================================================
     * 评价事件监听
     * ====================================================
     */
    function initEvaluation(options) {
        $(document).on('click', '#evaluationWindow .eval-submit span', function () {
            sendEvaluation();
        });

        $(document).on('click', '.ui-dialog-header .ui-dialog-close', function () {
            sendEvaluation();
        });

        /***
         * 评价输入框 输入事件监听
         */
        var maxEvaluationCharLength = 100;
        $(document).on('keyup', '#evaluationWindow>.eval-txt>textarea', function () {
            var $this = $(this);
            var val = $this.val();
            if (val.length > maxEvaluationCharLength) {
                $this.val(val.substr(0, maxEvaluationCharLength));
            }

            $this.next().find('.remaining').html(maxEvaluationCharLength - $this.val().length);
        });

        function sendEvaluation() {
            var message = '';
            var evaluationInput = $('#evaluationWindow>.eval-ipt input[name="evaluation"]:checked');
            message = message.concat('客户对您的评价为：');
            message = message.concat(evaluationInput.length > 0 ? evaluationInput.next().text() : '未评估');
            message = message.concat('\r\n');
            message = message.concat('客户对您的建议为：');
            message = message.concat($('#evaluationWindow>.eval-txt textarea').val() || '');

            if (typeof options.onSendingMessage === 'function') {
                options.onSendingEvaluation.call(null, message,
                    evaluationInput.length > 0 ? evaluationInput.val() : 0, retrieveFontStyleInfo());
                currentEvaluationDialog.close();
            }
        }
    }

    function initUserActionListen(options) {
        var needRetry = false;
        var errorMessage = '';

        function requestHistoryList() {
            var time = new Date();
            var oldestTime = $('.msg-time');
            if (oldestTime.length != 0) {
                oldestTime = oldestTime.eq(0).html();
                oldestTime = oldestTime.replace(/\(|\)/g, '');

                if (oldestTime.match(/^\d{1,2}:\d{1,2}:\d{1,2}$/)) {
                    time = new Date();
                    time = time.getFullYear() + "-" + (time.getMonth() + 1) + "-" + time.getDate() + ' ';
                    time += oldestTime;
                } else {
                    time = oldestTime;
                }

                time = new Date(Date.parse(time.replace(/-/g, "/")));
            }

            retrieveHistoryList(time.getTime(), retrieveHistoryListCallback);
        }

        function startRetrive() {
            var showMoreBtn = $('#J_show_more_info');
            showMoreBtn.unbind('click.showmore').bind('click.showmore', requestHistoryList);
        }

        function setRetriveInProgress() {
            var showMoreBtn = $('#J_show_more_info');
            showMoreBtn.unbind('click.showmore', requestHistoryList);
            showMoreBtn.html('正在请求....');
        }

        var completeRetrieve = function () {
            var showMoreBtn = $('#J_show_more_info');
            showMoreBtn.css('cursor', 'pointer').html('查看更多消息');
            showMoreBtn.prev('img').show();
            showMoreBtn.unbind('click.showmore').bind('click.showmore', requestHistoryList);
        }
        exports.continueHistroylistRetrieve = completeRetrieve;

        var finishRetrieve = function (message) {
            //没有更多消息了
            var showMoreBtn = $('#J_show_more_info');
            showMoreBtn.html(message);
            showMoreBtn.prev('img').hide();
            showMoreBtn.css('cursor', 'default');
            showMoreBtn.unbind('click.showmore');
        }
        exports.finishHistorylistRetrieve = finishRetrieve;

        function retrieveHistoryList(time) {
            if (typeof options.onRequestHistoryList === 'function') {
                setRetriveInProgress();
                options.onRequestHistoryList.call(null, time, retrieveHistoryListCallback);
            }
        }

        function retrieveHistoryListCallback(result) {
            if (!result.isSuccess) {
                if (result.canRequestAgain) {
                    completeRetrieve();
                    alert(result.errorMessage);
                }
            } else {
                if (!result.canRequestAgain) {
                    finishRetrieve(result.errorMessage);
                } else {
                    completeRetrieve();
                }
            }
        }

        startRetrive();
    }

    $.fn.extend({
        insertAtCaret: function (html) {
            var self = this;
            var ta = $(this)[0];

            if (ta.onbeforedeactivate) {
                var ieRangeBookMark;
                ta.attachEvent('onbeforedeactivate', function () {
                    ieRangeBookMark = self.getRange().selection.getBookmark();
                });
                ta.attachEvent('onactivate', function () {
                    self.getRange().selection.moveToBookmark(ieRangeBookMark);
                });
            } else {
                $(ta).bind('keyup mouseup', function () {
                    self.saveLastRange();
                });
            }

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
                var range = this.getRange().range,
                    selection = this.getRange().selection;
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
        }
    });

    var evaluationSendingIndicator = {
        html: '<span class="evaluation-message">' +
        '<span class="evaluation-send-indicator"></span>' +
        '<span class="evaluation-body">您已成功推送评价请求给客户</span></span>'
    };

    exports.appendEvaluation = function (options) {
        $.extend(options, evaluationSendingIndicator);
        exports.appendMessage(options);
    };

    var parseMessage = function (onNewURLParsed) {
        var itemRegexp = new RegExp('http://item\.(beta1\.fn|feiniu\.com)/(\\d+)');
        var storeRegexp = new RegExp('http://(storefront\.beta1\.fn|store\.feiniu\.com)/index-(\\d+)\.html');

        var extractURLCallback = function (href, globalID) {
            if (itemRegexp.test(href)) {
                href.replace(itemRegexp, function (allMatched, $2, skuID) {
                    onNewURLParsed.call(null, 'item', allMatched, {skuID: parseInt(skuID)}, globalID);
                });
            } else if (storeRegexp.test(href)) {
                href.replace(storeRegexp, function (allMatched, $2, merchantID) {
                    onNewURLParsed.call(null, 'store', allMatched, {merchantID: parseInt(merchantID)}, globalID);
                });
            }
        };

        return function (array, context, options) {
            var msg = '';

            for (var i = 0; i < array.length; i++) {
                var data = array[i];
                var opts = $.extend({}, {
                    type: 'buyer_msg',//seller_msg
                    html: '',
                    time: '',
                    name: ''
                }, data || {});
                var messageBody = '';

                if (!!opts.features) {
                    messageBody = "<div style='word-break: break-all; padding-right:5px; ";
                    for (var key in opts.features) {
                        switch (key) {
                            case 'weight' :
                                if (!!opts.features[key]) {
                                    messageBody += 'font-weight: bold;';
                                }
                                break;
                            case 'style' :
                                if (!!opts.features[key]) {
                                    messageBody += 'font-style: italic;';
                                }
                                break;
                            case 'line' :
                                if (!!opts.features[key]) {
                                    messageBody += 'text-decoration: underline;';
                                }
                                break;
                            case 'color' :
                                messageBody += 'color: ' + generalUtil.intToRgb(opts.features[key]) + ';';
                                break;
                            case 'family' :
                                messageBody += 'font-family: ' + opts.features[key] + ';';
                                break;
                            case 'size' :
                                messageBody += 'font-size: ' + opts.features[key] + 'px;';
                                break;
                        }
                    }
                    messageBody += "'>";

                    var indexMap = [];
                    var regex = /<image:([^>]+)>/;
                    var msgContent = opts.html;
                    msgContent = msgContent.replace(/<\/?[^>]+>/g, function ($0) {
                        if (regex.test($0)) {
                            return $0;
                        } else {
                            return $0.replace(/</g, '&lt;').replace(/>/g, '&gt;');
                        }
                    });

                    while (regex.test(msgContent)) {
                        msgContent = msgContent.replace(regex, function () {
                            var emotionCheck = /^b\d{2}.gif$/;
                            var urlCheck = /^http:.+$/;
                            var imgNameCheck = /^{.+\..{2,4}$/;
                            var imgSrc;
                            if (urlCheck.test(arguments[1])) {
                                imgSrc = 'src="' + arguments[1] + '"';
                            }
                            else if (emotionCheck.test(arguments[1])) {
                                imgSrc = 'src="static/images/feiniu-emotion/' + arguments[1] + '"';
                            }
                            else if (imgNameCheck.test(arguments[1])) {
                                var id = arguments[1].replace(/^{([^.]+)}\.\w+$/, '$1');
                                imgSrc = 'src="static/images/feiniu-emotion/waitImage.gif" class="fn-image-wait-to-set" data-id="' + id + '"';
                            }

                            var replaced = '<img ' + imgSrc + ' alt="聊天图片"' + '/>';

                            /***
                             * 修复url和图片地址冲突的问题, 避免被识别url
                             */
                            if (urlCheck.test(arguments[1])) {
                                indexMap.push({
                                    key: arguments[2],
                                    value: replaced.length
                                });
                            }
                            return replaced;
                        });
                    }

                    if (indexMap.length > 0) {
                        var start = 0;
                        var content = '';

                        for (var i = 0; i < indexMap.length; i++) {
                            var position = indexMap[i].key;
                            var length = indexMap[i].value;
                            var temp = msgContent.substring(start, position);

                            content += generalUtil.extractURLs(temp, extractURLCallback).replace(/\r/g, '<br/>');
                            content += msgContent.substr(position, length);

                            start = position + length;
                        }

                        var end = msgContent.substring(start);
                        content += generalUtil.extractURLs(end, extractURLCallback).replace(/\r/g, '<br/>');

                        msgContent = content;
                    } else {
                        msgContent = generalUtil.extractURLs(msgContent, extractURLCallback).replace(/\r/g, '<br/>');
                    }

                    messageBody += msgContent + "</div>";
                } else {
                    messageBody = opts.html;
                }

                if (opts.type == 'buyer_msg') {
                    msg += '<div class="buyer recv-' + opts.subMerchantId + '"><div class="buyer-cont">'
                        + '<div class="msg-title"><img class="touxiang pull-right" src="' + context.buyerImgUrl + '"/>'
                        + '<span class="msg-time">(' + opts.time + ')</span><span class="sender-name">' + opts.name + '</span>'
                        + '</div>'
                        + '<div class="msg-box"><div class="ui-bubble-blue"><em class="bar"></em><div class="cont-wrap">'
                        + messageBody + '</div></div></div></div>';
                } else if (opts.type == 'buyer_absent_msg') {
                    msg += '<div class="buyer recv-' + opts.subMerchantId + '" style="display:none;"><div class="buyer-cont">'
                        + '<div class="msg-title"><img class="touxiang pull-right" src="' + context.buyerImgUrl + '"/>'
                        + '<span class="msg-time">(' + opts.time + ')</span><span class="sender-name">' + opts.name + '</span>'
                        + '</div>'
                        + '<div class="msg-box"><div class="ui-bubble-blue"><em class="bar"></em><div class="cont-wrap">'
                        + messageBody + '</div></div></div></div>';
                } else if (opts.type == 'seller_msg') {
                    msg += '<div class="seller recv-' + opts.subMerchantId + '"><div class="seller-cont">'
                        + '<div class="msg-title"><img class="touxiang pull-left" src="' + context.sellerImgUrl + '"/>'
                        + '<span class="sender-name">' + opts.name + '</span><span class="msg-time">(' + opts.time + ')</span></div>'
                        + '<div class="msg-box"><div class="ui-bubble-gray"><em class="bar"></em><div class="cont-wrap">'
                        + messageBody + '</div></div></div></div>';
                } else if (opts.type == 'seller_absent_msg') {
                    msg += '<div class="seller recv-' + opts.subMerchantId + '" style="display:none;"><div class="seller-cont">'
                        + '<div class="msg-title"><img class="touxiang pull-left" src="' + context.sellerImgUrl + '"/>'
                        + '<span class="sender-name">' + opts.name + '</span><span class="msg-time">(' + opts.time + ')</span></div>'
                        + '<div class="msg-box"><div class="ui-bubble-gray"><em class="bar"></em><div class="cont-wrap">'
                        + messageBody + '</div></div></div></div>';
                }

            }

            return msg;
        }
    }

    exports.frozenChatArea = function () {
        $('#J_send').unbind('click.im_send');
        $('#J_send_state').unbind('.im_send_state').unbind('click.im_send_state');

        var showMoreBtn = $('#J_show_more_info');
        showMoreBtn.unbind('click.showmore');

        $('.textarea').unbind('.im_write').unbind('keypress.im_write');

        $(document).off('click', '.person ul li');
        $(document).off('click', '.person h2');
        $(document).off('click', '.person ul li.have_msg');
    }

    exports.appendMessage = function (data, context) {
        var msg = parseMessage([data], context, {showMonth: false});
        var speakArea = $('#D_left .speak');
        var speak = speakArea.closest('.top');

        speakArea.append(msg);
        speak.scrollTop(speak[0].scrollHeight);
    };

    exports.prependMessages = function (data, context) {
        var msg = parseMessage([data], context);
        var speakArea = $('#D_left .speak');
        var speak = speakArea.closest('.top');

        speakArea.prepend(msg);
        //speak.scrollTop(0);
    }

    /**
     * 添加系统消息
     * @param msg 消息内容
     * @param isCurrent 是否属于当前聊天窗口
     * @param subMerchantID 关联的客服id
     */
    exports.appendSysMessages = function (msg, isCurrent, subMerchantID) {
        var msg = '<p class="sys-message recv-$subMerchantID" style="$isVisible">$msg</p>'.replace(/\$msg/, msg)
            .replace(/\$isVisible/, isCurrent ? '' : 'display:none').replace(/\$subMerchantID/, subMerchantID);
        var speakArea = $('#D_left .speak');
        var speak = speakArea.closest('.top');

        speakArea.append(msg);
        speak.scrollTop(speak[0].scrollHeight);
    }

    exports.showEvaluationWindow = function () {
        currentEvaluationDialog = dialog.create({
            title: '请您评价',
            content: $('#evaluationWindow_tpl').html(),
            onclose: function () {
                this.remove();
            }
        });
        currentEvaluationDialog.showModal();
    };

    exports.playSound = function () {
        audioHandler.play();
    };

    var TitleHandler = {
        time: 0,
        title: document.title,
        timer: null,
        init: function (title) {
            this.title = title;
        },
        // 显示新消息提示
        show: function () {
            var self = this;

            this.timer = setTimeout(function () {
                self.time++;
                self.show();

                if (self.time % 2 == 0) {
                    document.title = "【您有新消息，请查看】" + self.title;
                } else {
                    document.title = self.title;
                }
                ;
            }, 5000);
        },
        // 取消新消息提示
        clear: function () {
            var self = this;

            clearTimeout(self.timer);
            document.title = self.title;
        }
    };

    exports.resetFlashTitle = function (title) {
        TitleHandler.init(title);
    };

    exports.flashTitle = function () {
        TitleHandler.show();
    };

    exports.stopFlashTitle = function () {
        TitleHandler.clear();
    };

    exports.refresh = function (options) {
        audioHandler.init();

        var opts = options || {};
        initResize(opts);
        initToolbar(opts);
        initText(opts);
        initEvaluation(opts);
        initUserActionListen(opts);
        TitleHandler.init(document.title);
        parseMessage = parseMessage(options.onNewURLParsed);

        $('.typing div.textarea').focus();
        toleft(opts);

        function toleft(options) {
            $(document).on('click', '.person h2', function () {
                $(this).toggleClass('open');
                $(this).next('ul').slideToggle().siblings('ul').removeClass('open').slideUp().prev('h2').removeClass('open');
            });

            $(document).on('click', '.person ul li.have_msg', function () {
                //移除消息数气泡
                if ($(this).parent().find('.msg-num-box').length == 1) {
                    $(this).parent().prev('h2').find('.msg-num-box').hide();
                }
                $(this).children('.msg-num-box').hide();
            });

            $(document).on('click', '.person ul li', function () {
                var self = $(this);

                //取消其他客服的高亮
                //添加阅读后的样式
                self.parent().siblings('ul').children('li').removeClass('read_msg');
                self.siblings().removeClass('read_msg');
                self.addClass('read_msg');

                var merchantInfo = {
                    subMerchantID: self.attr("data-submerchant-id"),
                    merchantID: self.attr("data-merchant-id"),
                    subMerchantName: self.attr("data-submerchant-name"),
                    storeName: self.attr("data-store-name"),
                    state: self.attr("data-state"),
                    productID: self.attr('data-product-id'),
                    imgUrl: self.find('img').attr('src'),
                    usersign: self.attr('data-usersign')
                };
                if (typeof options.onResetReceiver == 'function') {
                    options.onResetReceiver.call(null, merchantInfo);
                }
            });

            //高亮当前窗口正在联系人
            setTimeout(function () {
                exports.highlightContact(options.subMerchantID);
            }, 500);
        }
    };

    exports.attachWindowCloseListen = function (callback, context) {
        window.onbeforeunload = function (e) {
            e = e || window.event;
            if (e) {
                e.returnValue = '您确定关闭所有会话吗？点击确定关闭所有会话。';
            } else {
                return '您确定关闭所有会话吗？点击确定关闭所有会话。';
            }
        };

        if (window.onpagehide || window.onpagehide != null) {
            window.addEventListener('pagehide', function () {
                callback.call(context);
            }, false);
        } else {
            $(window).unload(function () {
                callback.call(context);
            });
        }
    }

    exports.detachWindowCloseListen = function () {
        window.onbeforeunload = null;
        if (window.onpagehide || window.onpagehide != null) {
            window.removeEventListener('pagehide');
        } else {
            $(window).unbind('unload');
        }
    }

    //渲染链接信息
    exports.renderURLThumbInfo = function (linkType, linkInfo, globalHrefID) {
        var trimLength = 26;
        var toRenderItem, toAppendLinkThumb, url;

        if (linkType === 'store') {
            var storeLinkThumbTemplate = '<div class="link-type-msg">' +
                '<div class="link-type-box"><a class="link" target="_blank"></a></div>' +
                '<div class="link-type-cont">' +
                '<img width="80" height="80" src="">' +
                '<div class="txt-box">' +
                '<h4 class="store-name"></h4>' +
                '<p class="store-desc"></p>' +
                '</div>' +
                '</div>' +
                '</div>';

            toRenderItem = $('#' + globalHrefID);
            toAppendLinkThumb = $(storeLinkThumbTemplate);

            if (toRenderItem.length > 0) {
                var storeDesc = generalUtil.trim($('<div>').html(linkInfo.storeDesc || '').text()).substr(0, trimLength);
                url = linkInfo.url;
                storeDesc = storeDesc.length == trimLength ? storeDesc + '...' : storeDesc;

                toAppendLinkThumb.find('.link-type-box a').
                    attr('href', linkInfo.url).html(linkInfo.url);
                toAppendLinkThumb.find('.link-type-cont img').attr('src', linkInfo.storeLogoUrl);
                toAppendLinkThumb.find('.txt-box>.store-name').html(linkInfo.storeName)
                    .next('.store-desc').html(storeDesc);
            }
        } else if (linkType === 'item') {
            var itemLinkThumbTemplate = '<div class="link-type-msg">' +
                '<div class="link-type-box"><a class="link" target="_blank"></a></div>' +
                '<div class="link-type-cont">' +
                '<img width="80" height="80" src="">' +
                '<div class="txt-box">' +
                '<h4 class="item-name"></h4>' +
                '<p class="item-price"></p>' +
                '</div>' +
                '</div>' +
                '</div>';

            toRenderItem = $('#' + globalHrefID);
            toAppendLinkThumb = $(itemLinkThumbTemplate);

            if (toRenderItem.length > 0) {
                var itemTitle = generalUtil.trim(linkInfo.title || '').substr(0, trimLength);
                itemTitle = itemTitle.length == trimLength ? itemTitle + '...' : itemTitle;
                url = linkInfo.productURL;

                toAppendLinkThumb.find('.link-type-box a').
                    attr('href', linkInfo.productURL).html(linkInfo.productURL);
                toAppendLinkThumb.find('.link-type-cont img').attr('src', linkInfo.pic01);
                toAppendLinkThumb.find('.txt-box>.item-name').html(itemTitle)
                    .next('.item-price').html('价格：￥' + (parseFloat(linkInfo.price || linkInfo.salePrice || 0).toFixed(2)) + '元');
            }
        }

        if (toAppendLinkThumb != null) {
            toAppendLinkThumb.find('img, .store-name, .item-name').
                css('cursor', 'pointer').attr('title', url).click(function () {
                    generalUtil.openNewTab(url);
                });

            toRenderItem.after(toAppendLinkThumb);
            toRenderItem.remove();
            exports.scrollToChatViewEnd();
        }
    }

    var ListUtil = {
        removeAll: function (srcList, targetList, comparator) {
            srcList.sort(comparator);
            targetList.sort(comparator);
            var intersectedChattList = [];

            for (var i = 0, j = 0; i < targetList.length && j < srcList.length;) {
                var oldCurrent = targetList[i];
                var newCurrent = srcList[j];
                var result = comparator.call(null, oldCurrent, newCurrent);

                if (result < 0) {
                    intersectedChattList.push(newCurrent);
                    i++;
                } else if (result == 0) {
                    i++;
                    j++;
                } else {
                    intersectedChattList.push(newCurrent);
                    j++;
                }
            }

            return intersectedChattList.concat(srcList.slice(j, srcList.length));
        }
    };
    //同步正在联系人列表
    exports.syncChattingList = function (chattingList) {
        var oldChattingList = [];
        var comparator = function (submerchantA, submerchantB) {
            return submerchantA.id - submerchantB.id;
        };
        var chatItemTemplate = '<li class="on $stateClass" data-submerchant-id="$id" data-id="$id" data-product-id=$productID' +
            ' data-merchant-id="$merchantID" data-submerchant-name="$subMerchantName" data-store-name="$storeName" data-state="$status"' +
            ' data-usersign="$usersign"' +
            '><img class="tuxiang" src="$storeLogoUrl" width="30" height="35"><span title="$name">$name</span><div class="msg-num-box">0</div></li>'
        var currentChattingListEle = $('#J_current_list');
        currentChattingListEle.children().each(function () {
            var $this = $(this);
            oldChattingList.push({
                id: $this.data('id')
            });
        });

        var retainedChattingList = ListUtil.removeAll(chattingList, oldChattingList, comparator);
        //按时间进行排序
        retainedChattingList.sort(function (a, b) {
            return b.time - a.time;
        });

        var toAppendStr = '';
        for (var i = 0; i < retainedChattingList.length; i++) {
            var submerchant = retainedChattingList[i];
            submerchant.usersign = submerchant.usersign == null ? "" : submerchant.usersign;
            submerchant.storeName = submerchant.storeName == null ? "" : submerchant.storeName;

            toAppendStr += chatItemTemplate.replace(/\$id/g, submerchant.id)
                .replace(/\$name/g, submerchant.storeName + ":" + submerchant.nickname)
                .replace(/\$subMerchantName/g, submerchant.nickname)
                .replace(/\$stateClass/g, submerchant.state == '0' ? '' : 'ui-off-line')
                .replace(/\$merchantID/g, submerchant.merchantId)
                .replace(/\$storeLogoUrl/g, submerchant.headImgUrl || 'static/images/moumoupc/icon(1).png')
                .replace(/\$storeName/g, submerchant.storeName)
                .replace(/\$status/g, submerchant.state)
                .replace(/\$productID/g, submerchant.skuID)
                .replace(/\$usersign/g, submerchant.usersign);
        }
        currentChattingListEle.append(toAppendStr);

        currentChattingListEle.slideDown().addClass('open').siblings('ul').slideUp();
        $('#J_current_label').html('正在联系人 (' + currentChattingListEle.children().length + '人)')
            .closest('h2').addClass('open');
    }

    //未读消息提醒
    exports.syncUnReadMsgList = function (unReadMsgMappingList) {
        $('#J_current_list, #J_recent_list').each(function () {
            var $this = $(this);
            var totalUnReadMsgNum = 0;

            $this.children().each(function () {
                var currentKefu = $(this);
                var subMerchantID = currentKefu.attr("data-submerchant-id");
                var unReadMsgNum = unReadMsgMappingList[subMerchantID];

                if (unReadMsgNum > 0) {
                    currentKefu.addClass('have_msg').find('.msg-num-box').html(unReadMsgNum).show();
                    totalUnReadMsgNum += unReadMsgNum;
                } else {
                    currentKefu.removeClass('have_msg').find('.msg-num-box').html(unReadMsgNum).hide();
                }
            });

            var unReadMsgShowBar = $this.prev('h2');
            if (totalUnReadMsgNum > 0) {
                unReadMsgShowBar.addClass('have_msg');
                unReadMsgShowBar.find('.msg-num-box').html(totalUnReadMsgNum).show();
            } else {
                unReadMsgShowBar.removeClass('have_msg');
                unReadMsgShowBar.find('.msg-num-box').html(totalUnReadMsgNum).hide();
            }
        });
    }

    //准备关闭页面
    exports.prepareToClose = function () {
        exports.frozenChatArea();
        exports.detachWindowCloseListen();

        generalUtil.closeWebPage();
    }

    //滑动窗口至最底端
    exports.scrollToChatViewEnd = function () {
        var speakArea = $('#D_left .speak');
        var speak = speakArea.closest('.top');

        speak.scrollTop(speak[0].scrollHeight);
    }

    //客服转接
    exports.switchSubMerchantInfo = function (srcInfo, destInfo) {
        if (!srcInfo || !destInfo) {
            return;
        }

        var itemList = $('#J_current_list li, #J_recent_list li');
        var srcItem = itemList.filter('[data-submerchant-id=' + srcInfo.id + ']');
        var destItem = itemList.filter('[data-submerchant-id=' + destInfo.id + ']');
        var isContactExistInList = $('.person ul li[data-submerchant-id=' + destInfo.id + ']').length > 0;

        if (srcItem.length > 0) {
            var allSrcMessageList = $('.recv-' + srcInfo.id);
            !isContactExistInList && allSrcMessageList.addClass('recv-' + destInfo.id);

            if (destItem.length > 0) {
                exports.highlightContact(destInfo.id);
            } else {
                var storeName = srcItem.attr('data-store-name');
                var name = destInfo.name;
                var showName = storeName + "：" + name;

                srcItem.attr('data-submerchant-id', destInfo.id);
                srcItem.attr('data-submerchant-name', name);

                srcItem.find('span').html(showName).attr('title', showName);
            }
        }
    }

    //高亮联系人
    exports.highlightContact = function (subMerchantID) {
        var item = $('.person ul li[data-submerchant-id=' + subMerchantID + ']');
        if (item.length > 0) {
            var openLabel = item.closest('ul').prev('h2');
            if (openLabel.hasClass('open')) {
                item.trigger('click');
            } else {
                $('.person h2.open').removeClass('open').next('ul').removeClass('open').slideUp();
                openLabel.addClass('open').next('ul').addClass('open').slideDown();

                item.trigger('click');
            }
        }
    }

});