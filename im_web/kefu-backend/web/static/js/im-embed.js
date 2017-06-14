/**
 * Author: morningking
 * Date: 2016/3/15
 * Contact: 243717042@qq.com
 */
if (!seajs) {
    var seajs = {
        use: function (array, callback) {
            var params = [];

            for (var i = 0; i < array.length; i++) {
                params.push(eval(array[i]));
            }

            callback.call(null, params);
        }
    };
}

if (!!window.$) {
    initAll.call(null, $);
} else {
    seajs.use(['$'],
        function (jq) {
            initAll.call(null, $);
        });
}

function initAll($) {
    var IMGlobalConfig = (function () {
        var config = {
            servicePath: 'http://merchant.feiniu.com/webim/',
            metaPath: 'http://kefu-backend.idc1.fn/',
            refreshInterval: 30000
        };
        var currentHref = window.location.href;
        var currentDomain = currentHref.replace(new RegExp('^https?://([^/]+)/.+$'), '$1');

        if (currentDomain.indexOf('fn.com') > -1)/**beta环境**/{
            if (currentDomain.indexOf('local') == 0) {
                config.servicePath = 'http://local.beta1.fn/webim/';
                config.metaPath = 'http://local.fn.com/kefu-backend/';
            } else {
                config.servicePath = 'http://webim.beta1.fn/webim/';
                config.metaPath = 'http://kefu-backend.beta.fn.com/kefu-backend/';
            }

        }

        return config;
    })();

    var GeneralUtil = {
        dynamicWeaveCSS: function (src) {
            var doc = document;
            var link = doc.createElement("link");
            link.setAttribute("rel", "stylesheet");
            link.setAttribute("type", "text/css");
            link.setAttribute("href", src);

            var heads = doc.getElementsByTagName("head");
            if (heads.length)
                heads[0].appendChild(link);
        },
        removeFromArray: function (element, array) {
            if ($.isArray(array)) {
                for (var i = 0; i < array.length; i++) {
                    if (array[i] == element) {
                        array.splice(i, 1);
                    }
                }
            }
        },
        redirectInCurrentWindow: function (href) {
            window.location.href = href;
        },
        openInNewWindow: function (href) {
            var name = '_blank';                        //网页名称，可为空;
            var iWidth = 1000;                          //弹出窗口的宽度;
            var iHeight = 700;                       //弹出窗口的高度;
            //获得窗口的垂直位置
            var iTop = (window.screen.availHeight - 30 - iHeight) / 2;
            //获得窗口的水平位置
            var iLeft = (window.screen.availWidth - 10 - iWidth) / 2;
            var params = 'width=' + iWidth
                    + ',height=' + iHeight
                    + ',top=' + iTop
                    + ',left=' + iLeft
                    + ',channelmode=no'//是否使用剧院模式显示窗口。默认为 no
                    + ',directories=yes'//是否添加目录按钮。默认为 yes
                    + ',fullscreen=no' //是否使用全屏模式显示浏览器
                    + ',location=no'//是否显示地址字段。默认是 yes
                    + ',menubar=no'//是否显示菜单栏。默认是 yes
                    + ',resizable=yes'//窗口是否可调节尺寸。默认是 yes
                    + ',scrollbars=yes'//是否显示滚动条。默认是 yes
                    + ',status=yes'//是否添加状态栏。默认是 yes
                    + ',titlebar=yes'//默认是 yes
                    + ',toolbar=no'//默认是 yes
                ;
            return window.open(href, '_blank', params, false);
        },
        log: function (str) {
            window.console && console.log(str);
        },
        numberCheck: function (param) {
            return param != null && !isNaN(parseInt(param));
        },
        deserialization: function (object) {
            return typeof object == 'string' ? eval('(' + object + ')') : object;
        }
    };
    var serviceHelper = {
        config: {
            defaultJsonPConfig: {
                async: false,
                dataType: 'jsonp',
                jsonp: 'callback',
                type: 'get',
                error: function (jqxhr, settings, thrownError) {
                    GeneralUtil.log(thrownError);
                }
            }
        },
        callJsonPService: function (options) {
            options = $.extend(this.config.defaultJsonPConfig, options);
            $.ajax(options);
        }
    };
    var Base64 = {
        _keyStr: "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=", encode: function (e) {
            var t = "";
            var n, r, i, s, o, u, a;
            var f = 0;
            e = Base64._utf8_encode(e);
            while (f < e.length) {
                n = e.charCodeAt(f++);
                r = e.charCodeAt(f++);
                i = e.charCodeAt(f++);
                s = n >> 2;
                o = (n & 3) << 4 | r >> 4;
                u = (r & 15) << 2 | i >> 6;
                a = i & 63;
                if (isNaN(r)) {
                    u = a = 64
                } else if (isNaN(i)) {
                    a = 64
                }
                t = t + this._keyStr.charAt(s) + this._keyStr.charAt(o) + this._keyStr.charAt(u) + this._keyStr.charAt(a)
            }
            return t
        }, decode: function (e) {
            var t = "";
            var n, r, i;
            var s, o, u, a;
            var f = 0;
            e = e.replace(/[^A-Za-z0-9\+\/\=]/g, "");
            while (f < e.length) {
                s = this._keyStr.indexOf(e.charAt(f++));
                o = this._keyStr.indexOf(e.charAt(f++));
                u = this._keyStr.indexOf(e.charAt(f++));
                a = this._keyStr.indexOf(e.charAt(f++));
                n = s << 2 | o >> 4;
                r = (o & 15) << 4 | u >> 2;
                i = (u & 3) << 6 | a;
                t = t + String.fromCharCode(n);
                if (u != 64) {
                    t = t + String.fromCharCode(r)
                }
                if (a != 64) {
                    t = t + String.fromCharCode(i)
                }
            }
            t = Base64._utf8_decode(t);
            return t
        }, _utf8_encode: function (e) {
            e = e.replace(/\r\n/g, "\n");
            var t = "";
            for (var n = 0; n < e.length; n++) {
                var r = e.charCodeAt(n);
                if (r < 128) {
                    t += String.fromCharCode(r)
                } else if (r > 127 && r < 2048) {
                    t += String.fromCharCode(r >> 6 | 192);
                    t += String.fromCharCode(r & 63 | 128)
                } else {
                    t += String.fromCharCode(r >> 12 | 224);
                    t += String.fromCharCode(r >> 6 & 63 | 128);
                    t += String.fromCharCode(r & 63 | 128)
                }
            }
            return t
        }, _utf8_decode: function (e) {
            var t = "";
            var n = 0;
            var r = c1 = c2 = 0;
            while (n < e.length) {
                r = e.charCodeAt(n);
                if (r < 128) {
                    t += String.fromCharCode(r);
                    n++
                } else if (r > 191 && r < 224) {
                    c2 = e.charCodeAt(n + 1);
                    t += String.fromCharCode((r & 31) << 6 | c2 & 63);
                    n += 2
                } else {
                    c2 = e.charCodeAt(n + 1);
                    c3 = e.charCodeAt(n + 2);
                    t += String.fromCharCode((r & 15) << 12 | (c2 & 63) << 6 | c3 & 63);
                    n += 3
                }
            }
            return t
        }
    };

    var EventType = {
        /**服务器返回 在线客服亮灯数据**/
        RENDER_MERCHANT_ONLINE_INFO: 'RENDER_MERCHANT_ONLINE_INFO'
    };
    var GlobalEventManager = {
        registerEvent: function (event, fn, context) {
            $(document).bind(event, function (event, param) {
                fn.call(context, param);
            });
        },
        dispatchEvent: function (event, params) {
            $.event.trigger(event, params);
        }
    };

    var DataManager = (function () {
        var MerchantList = [];

        return {
            addMerchantID: function (merchantID) {
                if (GeneralUtil.numberCheck(merchantID)) {
                    merchantID = parseInt(merchantID);
                    if ((merchantID > 0) && (-1 == $.inArray(merchantID, MerchantList))) {
                        MerchantList.push(merchantID);
                    }
                }
            },
            getMerchantIDList: function () {
                return MerchantList.slice(0);
            }
        }
    })();

    var TemplateManager = {
        chatIconTemplate: '<div class="chat-icon general" title="在线客服">' +
        '<a class="chat-online" href="#"><span></span></a>' +
        '</div>',
    };

    var ContactsService = {
        conf: {
            venderURL: IMGlobalConfig.servicePath + 'vender',
        },
        getMerchantOnlineInfo: function (merchantIDList, callback) {
            if (merchantIDList.length == 0) {
                callback.call(null, {
                    success: true,
                    data: {
                        merchantOnlineInfo: []
                    }
                });
            } else {
                serviceHelper.callJsonPService({
                    url: this.conf.venderURL,
                    data: {
                        VENDER_INFO_TYPE: 'GET_MERCHANT_ONLINE_INFO',
                        sid: -1,
                        merchantids: merchantIDList.join(',')
                    },
                    success: function (resultInfo) {
                        callback.call(null, resultInfo);
                    }
                });
            }
        }
    };
    var ContactsViewer = {
        init: function () {
            this.bindEvent();
        },
        bindEvent: function () {
            /**点击在线客服，进行相应的客服分流**/
            $(document).on('click', '.fnmm-chat-panel a.chat-online', function (e) {
                var chatPanel = $(this).closest('div.fnmm-chat-panel');
                var imData = chatPanel.data('im');

                GeneralUtil.redirectInCurrentWindow("feiniu-work://?merchantID=" + imData.merchantID);
                e.preventDefault();
                return;

                e.preventDefault();
            });
        },
        updateMerchantOnlineInfo: function (result) {
            for (var i = 0; i < result.length; i++) {
                var merchantItem = result[i];
                if (merchantItem.online) {
                    $('.web-chat-merchant-' + merchantItem.id).removeClass('offline');
                } else {
                    $('.web-chat-merchant-' + merchantItem.id).addClass('offline');
                }
            }
        }
    };
    var ContactController = function (service, viewer) {
        this.service = service;
        this.viewer = viewer;
    };
    ContactController.prototype = {
        timer: null,
        constructor: ContactController,
        init: function () {
            this.initGlobal();
            this.bindEvent();
            this.initData();
        },
        initGlobal: function () {
            ContactsViewer.init();
        },
        bindEvent: function () {
            var self = this;
            GlobalEventManager.registerEvent(EventType.RENDER_MERCHANT_ONLINE_INFO, function (resultInfo) {
                if (resultInfo.success) {
                    /**更新 在线客服在线**/
                    self.viewer.updateMerchantOnlineInfo(resultInfo.data.merchantOnlineInfo);
                }
            });
        },
        initData: function () {
            this.service.getMerchantOnlineInfo(DataManager.getMerchantIDList(), function (param) {
                GlobalEventManager.dispatchEvent(EventType.RENDER_MERCHANT_ONLINE_INFO, param);
            });
        }
    };

    /**遍历布点环境，织入web-im元素、样式表，初始化相关环境变量**/
    $(document).ready(function embedElements() {
        /**遍历布点环境，寻找在线客服panel**/
        $('.fnmm-chat-panel').each(function () {
            var $this = $(this);
            $this.append(TemplateManager.chatIconTemplate);

            var data = GeneralUtil.deserialization($this.data('im'));
            $this.data('im', data);

            if (!!data) {
                DataManager.addMerchantID(data.merchantID);

                $this.find('.chat-online').addClass('web-chat-merchant-' + data.merchantID);
                $this.addClass('fnmm-chat-panel');
                $this.find('.chat-icon').addClass('general');
            }
        });

        GeneralUtil.dynamicWeaveCSS(IMGlobalConfig.metaPath + 'css/bottom.css?v=' + (new Date().getTime()));
    });

    /**布点js-mvc入口**/
    $(document).ready(function initChatApp() {
        var contactController = new ContactController(ContactsService, ContactsViewer);
        contactController.init();
    });
}

