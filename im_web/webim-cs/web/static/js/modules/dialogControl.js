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

    $.browser = {};
    $.browser.mozilla = /firefox/.test(navigator.userAgent.toLowerCase());
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
                        m4a: "sound/message.m4a",
                        mp3: "sound/message.mp3",
                        wav: "sound/message.wav",
                        ogg: "oggsound/message.ogg"
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

    exports.initUpload = function (options) {
        $('#photoUploader i').uploader({
            action: (function () {
                var basePath = 'webim/upload-photo.do';
                basePath += ObjectUtil.serializeToFormStyle(options.getSessionData.call());

                return basePath;
            })(),
            swf: 'swf/uploader.swf',
            name: 'img',
            //preview: true,
            //showQueue: '#uploadQueue',
            auto: 'true',
            fileTypeDesc: '请选择一张大小在2M内的图片，文件类型可以为(jpg、jpeg、gif、png、tif)',
            fileTypeExts: 'jpg,jpeg,gif,png,tif',
            fileSizeLimit: '2M',
            formData: options.getSessionData,
            onError: function (e) {
                alert(e.message);
            },
            onSuccess: function (e) {

                if (e.data) {
                    var result = eval('(' + e.data + ')');

                    if (result.success) {
                        $('.typing .textarea').append('<img src="' + result.data.imageURL + '">');
                    } else {
                        alert(result.errorMessage || '服务器发生故障，请稍后重试');
                    }
                }
            }
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

        //激活聊天区域各组件相应
        exports.activeChatArea = function () {
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

            $('#J_send').unbind('.im_send').bind('click.im_send', function (e) {
                sendMessage($('.textarea'));

                $('.textarea').html('')

                e.preventDefault();
                return false;
            });

            $('.textarea').bind('keypress', function () {
                TitleHandler.clear();
            }).bind('click', function () {
                TitleHandler.clear();
            });

            var id = $('#J_send_state_warp a.gou').attr('id');
            if ('J_send_state_enter' == id) {
                switchEnterState();
            } else {
                switchCtrEnterState();
            }
        }
        exports.activeChatArea();

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

        //点击重连
        $(document).on('click', '.re-connect', function () {
            if ($.isFunction(options.reconnect)) {
                options.reconnect.call();
            }

            $(this).removeClass('re-connect');
        });

        //编辑框 实现粘贴发送图片
        (function attachPasteImgEventForTextArea() {
            function addEventListener(b, a, c) {
                //for chrome and firefox browser
                if (b.addEventListener) {
                    b.addEventListener(a, c, false)
                } else {
                    //for ie browser
                    if (b.attachEvent) {
                        b.attachEvent("on" + a, function (d) {
                            c.call(b, d)
                        })
                    }
                }
            };

            addEventListener(document, "paste", function (e) {
                document.getElementById("textareaSend").focus();
                //for chrome
                if (e.clipboardData.items != null) {
                    for (var i = 0; i < e.clipboardData.items.length; i++) {
                        if (e.clipboardData.items[i].kind == "file" && e.clipboardData.items[i].type == "image/png") {
                            // get the blob
                            var imageFile = e.clipboardData.items[i].getAsFile();

                            // read the blob as a data URL
                            var fileReader = new FileReader();

                            fileReader.onload = function (eve) {
                                // create an image
                                var image = document.createElement("IMG");
                                image.src = this.result;

                                // insert the image
                                var range = window.getSelection().getRangeAt(0);
                                range.insertNode(image);
                                range.collapse(false);

                                // set the selection to after the image
                                var selection = window.getSelection();
                                selection.removeAllRanges();
                                selection.addRange(range);
                            };

                            //update image
                            fileReader.readAsDataURL(imageFile);

                            // prevent the default paste action
                            e.preventDefault();

                            // only paste 1 image at a time
                            break;
                        }
                    }
                }
            });
        })();
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

        $('.make-evaluation').click(function () {
            exports.showEvaluationWindow();
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

            if (typeof options.onSendingEvaluation === 'function') {
                options.onSendingEvaluation.call(null, message,
                    evaluationInput.length > 0 ? evaluationInput.val() : 0, retrieveFontStyleInfo());
                currentEvaluationDialog.close();
            }
        }
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
        var feiniuItemRegexp = new RegExp('http://www\.(beta1\.fn/sh|feiniu\.com)/item/([a-zA-Z0-9]+)');

        var extractURLCallback = function (href, globalID) {
            if (itemRegexp.test(href)) {
                href.replace(itemRegexp, function (allMatched, $2, skuID) {
                    onNewURLParsed.call(null, 'merchant-item', allMatched, {skuID: parseInt(skuID)}, globalID);
                });
            } else if (storeRegexp.test(href)) {
                href.replace(storeRegexp, function (allMatched, $2, merchantID) {
                    onNewURLParsed.call(null, 'store', allMatched, {merchantID: parseInt(merchantID)}, globalID);
                });
            } else if (feiniuItemRegexp.test(href)) {
                href.replace(feiniuItemRegexp, function (allMatched, $2, merchantID) {
                    onNewURLParsed.call(null, 'feiniu-item', allMatched, {skuID: merchantID}, globalID);
                });
            }
        };

        return function (array, context) {
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
                            var urlCheck = /^http:\/\/.+$/;
                            var imgNameCheck = /^{.+\..{2,4}$/;
                            var imgSrc;
                            if (urlCheck.test(arguments[1])) {
                                imgSrc = 'src="' + arguments[1] + '"';
                            }
                            else if (emotionCheck.test(arguments[1])) {
                                imgSrc = 'src="webim/static/images/feiniu-emotion/' + arguments[1] + '"';
                            }
                            else if (imgNameCheck.test(arguments[1])) {
                                var id = arguments[1].replace(/^{([^.]+)}\.\w+$/, '$1');
                                imgSrc = 'src="webim/static/images/feiniu-emotion/waitImage.gif" class="fn-image-wait-to-set" data-id="' + id + '"';
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
                    msg += '<div class="buyer"><div class="buyer-cont">'
                        + '<div class="msg-title"><img class="touxiang pull-right" src="' + context.buyerImgUrl + '"/>'
                        + '<span class="msg-time">(' + opts.time + ')</span><span class="sender-name">' + opts.name + '</span>'
                        + '</div>'
                        + '<div class="msg-box"><div class="ui-bubble-blue"><em class="bar"></em><div class="cont-wrap">'
                        + messageBody + '</div></div></div></div>';
                } else if (opts.type == 'seller_msg') {
                    msg += '<div class="seller"><div class="seller-cont">'
                        + '<div class="msg-title"><img class="touxiang pull-left" src="' + context.sellerImgUrl + '"/>'
                        + '<span class="sender-name">' + opts.name + '</span><span class="msg-time">(' + opts.time + ')</span></div>'
                        + '<div class="msg-box"><div class="ui-bubble-gray"><em class="bar"></em><div class="cont-wrap">'
                        + messageBody + '</div></div></div></div>';
                }
            }

            return msg;
        }
    }

    exports.appendMessage = function (data, context) {
        var msg = parseMessage([data], context);
        var speakArea = $('#D_left .speak');
        var speak = speakArea.closest('.top');

        speakArea.append(msg);
        exports.scrollToChatViewEnd();
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
    exports.appendSysMessage = (function () {
        var messageTemplate =
            '<div class="dialog-tip-area"><p class="system-tip">系统提示：<br>$msg</p></div>';
        var speakArea = $("#D_left .speak");

        return function (msg) {
            speakArea.append(messageTemplate.replace(/\$msg/, msg));
            exports.scrollToChatViewEnd();
        };
    })();

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
                var storeDesc = StringUtil.trim($('<div>').html(linkInfo.storeDesc || '').text()).substr(0, trimLength);
                url = linkInfo.url;
                storeDesc = storeDesc.length == trimLength ? storeDesc + '...' : storeDesc;

                toAppendLinkThumb.find('.link-type-box a').
                    attr('href', linkInfo.url).html(linkInfo.url);
                toAppendLinkThumb.find('.link-type-cont img').attr('src', linkInfo.storeLogoUrl);
                toAppendLinkThumb.find('.txt-box>.store-name').html(linkInfo.storeName)
                    .next('.store-desc').html(storeDesc);
            }
        } else if (linkType === 'merchant-item') {
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
                var itemTitle = StringUtil.trim(linkInfo.title || '').substr(0, trimLength);
                itemTitle = itemTitle.length == trimLength ? itemTitle + '...' : itemTitle;
                url = linkInfo.productURL;

                toAppendLinkThumb.find('.link-type-box a').
                    attr('href', linkInfo.productURL).html(linkInfo.productURL);
                toAppendLinkThumb.find('.link-type-cont img').attr('src', linkInfo.defaultPic);
                toAppendLinkThumb.find('.txt-box>.item-name').html(itemTitle)
                    .next('.item-price').html('价格：￥' + linkInfo.price + '元');
            }
        } else if (linkType === 'feiniu-item') {
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
                var itemTitle = StringUtil.trim(linkInfo.smTitle || '').substr(0, trimLength);
                itemTitle = itemTitle.length == trimLength ? itemTitle + '...' : itemTitle;
                url = linkInfo.url;

                toAppendLinkThumb.find('.link-type-box a').
                    attr('href', url).html(url);
                toAppendLinkThumb.find('.link-type-cont img').attr('src', linkInfo.smPic);
                toAppendLinkThumb.find('.txt-box>.item-name').html(itemTitle)
                    .next('.item-price').html('价格：￥' + linkInfo.smPrice + '元');
            }
        }

        if (toAppendLinkThumb != null) {
            toAppendLinkThumb.find('img, .store-name, .item-name').
                css('cursor', 'pointer').attr('title', url).click(function () {
                    BrowserUtil.openNewTab(url);
                });

            toRenderItem.after(toAppendLinkThumb);
            toRenderItem.remove();
            exports.scrollToChatViewEnd();
        }
    }

    //准备关闭页面
    exports.prepareToClose = function () {
        exports.frozenChatArea();
        exports.detachWindowCloseListen();

        BrowserUtil.closeWebPage();
    }

    //滑动窗口至最底端
    exports.scrollToChatViewEnd = function () {
        var speakArea = $('#D_left .speak');
        var speak = speakArea.closest('.top');

        speak.scrollTop(speak[0].scrollHeight);
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

    //重设当前排队序号
    exports.setQueueSeq = (function () {
        var navEle = $('#nav');
        var queueEle = navEle.find('.queue-seq');
        var speakArea = $("#D_left .speak");

        return function (seq) {
            if (seq > 0) {
                queueEle.html(seq);
                navEle.show();
            } else if (seq == 0) {
                navEle.hide();
                speakArea.append('<div class="dialog-tip-area"><p class="service-join ui-icon-right">飞牛客服已加入会话</p></div>');
            } else {
                navEle.hide();
            }
        };
    })();

    exports.alertNoConsultantAvailable = (function () {
        var navEle = $('#nav');

        return function () {
            navEle.html('<p>当前没有客服在线</p>');
            exports.appendSysMessage('亲爱的顾客，您可于9:00—22:00间访问在线客服或拨打400-920-6565热线，感谢您的支持。');

            $('#D_left .typing .send').find('.btn,.up').addClass('disabled');
        }
    })();

    //提示用户注意发言时间
    exports.alertSpeak = (function () {
        var timePastTemplate =
            '飞牛客服提醒您，您已$timePast分钟未发言，如果您继续不发言$timeRemain分钟，则会自动断线，请您注意';

        return function (timePast, deadlineTime) {
            if (timePast == deadlineTime) {
                exports.appendSysMessage('由于您连续10分钟没有发言，系统已自动终止了此次咨询。如需继续咨询，请' +
                    '<a class="popu_bule ui-clickable re-connect" href="#">重新连接</a>');
            } else {
                var timeRemain = deadlineTime - timePast;
                exports.appendSysMessage(timePastTemplate.replace(/\$timePast/, timePast).replace(/\$timeRemain/, timeRemain.toString()));
            }
        }
    })();

    //提示用户重新连接
    exports.claimRetryConnect = function () {
        var sysMessage = '系统发生故障，请刷新窗口重试。';
        //sysMessage += '点击这里' +
        //    '<a class="popu_bule ui-clickable re-connect" href="#">重试</a>';

        exports.appendSysMessage(sysMessage);
        exports.frozenChatArea();
    }

    //冻结聊天区域各组件响应
    exports.frozenChatArea = function () {
        $('#J_send').unbind('click.im_send');
        $('#J_send_state').unbind('.im_send_state').unbind('click.im_send_state');

        $('.textarea').unbind('.im_write').unbind('keypress.im_write');
    }

    //注册回调函数
    exports.registerCallbacks = function (options) {
        audioHandler.init();

        var opts = options || {};
        initToolbar(opts);
        exports.initUpload(opts);
        initText(opts);
        initEvaluation(opts);
        TitleHandler.init(document.title);
        parseMessage = parseMessage(options.onRetrievingUrlSummaryInfo);

        $('.typing div.textarea').focus();
    }

    //提示有新消息到达
    exports.notifyNewMessageArrival = function () {
        exports.flashTitle();
        exports.playSound();
    }

    //绑定退出提示
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

    //取消绑定退出提示
    exports.detachWindowCloseListen = function () {
        window.onbeforeunload = null;
        if (window.onpagehide || window.onpagehide != null) {
            window.removeEventListener('pagehide');
        } else {
            $(window).unbind('unload');
        }
    }
});