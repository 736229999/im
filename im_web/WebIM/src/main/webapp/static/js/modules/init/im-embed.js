/**
 * Created by wangchen on 2014/12/24.
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
            metaPath: 'http://merchant.feiniu.com/webim/',
            loginPath: 'https://member.feiniu.com/getaway/login',
            refreshInterval: 30000,
            userNameFieldName: 'fn_username_for_js',
            guidFieldName: 'fn_login'
        };
        var currentHref = window.location.href;
        var currentDomain = currentHref.replace(new RegExp('^https?://([^/]+)/.+$'), '$1');

        if (currentDomain.indexOf('beta1.fn') > -1)/**beta环境**/{
            if (currentDomain.indexOf('local') == 0) {
                config.metaPath = 'http://local.beta1.fn/webim/';
            } else {
                config.metaPath = 'http://webim.beta1.fn/webim/';
            }

            config.loginPath = 'https://member.beta1.fn/sh/getaway/login';
        } else if (currentDomain.indexOf('dev1.fn') > -1)/**dev环境**/{
            if (currentDomain.indexOf('local') == 0) {
                config.metaPath = 'http://local.dev1.fn/webim/';
            } else {
                config.metaPath = 'http://webim.dev1.fn/webim/';
            }

            config.loginPath = 'https://member.dev1.fn/sh/getaway/login';
        }

        if (currentHref.indexOf('https') == 0) {
            config.metaPath = config.metaPath.replace(/^http/, 'https');
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
        redirectInCurrentWindow: function (href) {
            window.location.href = href;
        },
        redirectToLoginPath: function (href) {
            var returnParam = 'u=' + Base64.encode(href || window.location.href);
            var newHref = IMGlobalConfig.loginPath + '?' + returnParam;

            return this.openInNewWindow(newHref);
        },
        cookieUtils: function (name, value, options) {
            if (typeof value != 'undefined') {
                options = options || {};
                if (value === null) {
                    value = '';
                    options = $.extend({}, options);
                    options.expires = -1;
                }
                var expires = '';
                if (options.expires && (typeof options.expires == 'number' || options.expires.toUTCString)) {
                    var date;
                    if (typeof options.expires == 'number') {
                        date = new Date();
                        date.setTime(date.getTime() + (options.expires * 24 * 60 * 60 * 1000));
                    } else {
                        date = options.expires;
                    }
                    expires = '; expires=' + date.toUTCString();
                }
                var path = options.path ? '; path=' + (options.path) : '';
                var domain = options.domain ? '; domain=' + (options.domain) : '';
                var secure = options.secure ? '; secure' : '';
                document.cookie = [name, '=', encodeURIComponent(value), expires, path, domain, secure].join('');
            } else {
                var cookieValue = null;
                if (document.cookie && document.cookie != '') {
                    var cookies = document.cookie.split(';');
                    for (var i = 0; i < cookies.length; i++) {
                        var cookie = jQuery.trim(cookies[i]);
                        if (cookie.substring(0, name.length + 1) == (name + '=')) {
                            cookieValue = decodeURIComponent(cookie.substring(name.length + 1));
                            break;
                        }
                    }
                }
                return cookieValue;
            }
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

    var isIe = !!window.ActiveXObject || "ActiveXObject" in window;
    var PluginManager = {
        _plugin: null,
        _isPluginSupported: false,
        _classid: '0673fe15-633d-5ac6-9378-8eac414d0469',
        init: function () {
            var self = this;

            this._plugin = document.getElementById("ddplugin");
            this._isPluginSupported = (function () {
                if (!isIe) {
                    var mimeType = navigator.mimeTypes["application/x-implugin"];
                    if (mimeType) {
                        var plugin = mimeType.enabledPlugin;
                        if (plugin) return !0;
                    }
                    return !1;
                } else {
                    //GeneralUtil.log(FireBreath._isIEPluginInstalled("imPlugin"));
                    //return FireBreath._isIEPluginInstalled("imPlugin");
                    var pluginDiv = document.createElement("div");
                    pluginDiv.id = "pluginDiv", pluginDiv.style.display = "none", pluginDiv.innerHTML = '<object id="objectForDetectPlugin" classid="CLSID:' + self._classid + '"></object>', document.body.appendChild(pluginDiv);
                    try {
                        return void 0 == eval("objectForDetectPlugin.version") ? (pluginDiv.parentNode.removeChild(pluginDiv), !1) : (pluginDiv.parentNode.removeChild(pluginDiv), !0)
                    } catch (e) {
                        return !1
                    }
                }
            })();
        },
        checkPluginSupportion: function () {
            return this._isPluginSupported;
        }
    };

    var EventType = {
        /**服务器返回 店铺亮灯分组数据**/
        RENDER_STORE_LIGHT_CONFIG: 'RENDER_STORE_LIGHT_CONFIG',
        /**服务器返回 在线客服亮灯数据**/
        RENDER_MERCHANT_ONLINE_INFO: 'RENDER_MERCHANT_ONLINE_INFO',
        /**服务器返回 分组亮灯数据**/
        RENDER_GROUP_ONLINE_INFO: 'RENDER_GROUP_ONLINE_INFO',

        /**服务器返回 初始化布点数据**/
        INIT_EMBED_ENV: 'INIT_EMBED_ENV',
        /**页面数据读取完毕，开始准备和webim服务器进行一系列交互**/
        PREPARE_ENV: 'PREPARE_ENV',

        /**当前买家状态为未登录情况下，获取服务器数据起始点**/
        UPDATE_NON_LOGIN_ENV_INFO: 'UPDATE_NON_LOGIN_ENV_INFO',
        /**当前买家状态为已登录情况下，获取服务器数据起始点**/
        UPDATE_LOGIN_ENV_INFO: 'UPDATE_LOGIN_ENV_INFO',
        /**分发UPDATE_NON_LOGIN_ENV_INFO和UPDATE_LOGIN_ENV_INFO**/
        UPDATE_ENV_INFO: 'UPDATE_ENV_INFO',

        /**向服务器请求 分流**/
        DIVERSION_SERVICE: 'DIVERSION_SERVICE',

        /**动态添加在线客服panel时触发事件**/
        ADD_FNMM_CHAT_PANEL: 'ADD_FNMM_CHAT_PANEL',
        /**清除全部在线客服panel时触发事件**/
        CLEAR_FNMM_CHAT_PANEL: 'CLEAR_FNMM_CHAT_PANEL',
        /**删除 单个或多个 在线客服panel时触发事件**/
        REMOVE_FNMM_CHAT_PANEL: 'REMOVE_FNMM_CHAT_PANEL'
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

    var CustomerInfo = {
        name: '',
        id: 0
    };
    var ProductInfo = function () {
    };
    ProductInfo.prototype = {
        constructor: ProductInfo,
        setMerchantID: function (id) {
            this.id = id;
        },
        getMerchantID: function () {
            return this.id;
        },
        setProductID: function (productID) {
            this.productID = productID;
        },
        getProductID: function () {
            return this.productID;
        },
        setPageType: function (pageType) {
            this.pageType = pageType;
        },
        getPageType: function () {
            return this.pageType;
        },
        setShopID: function (shopID) {
            this.shopID = shopID;
        },
        getShopID: function () {
            return this.shopID;
        }
    };

    var DataManager = (function () {
        var MerchantList = [];
        var MerchantListForStoreLight = [];
        var GroupIDList = [];

        return {
            addMerchantID: function (merchantID) {
                if (GeneralUtil.numberCheck(merchantID)) {
                    merchantID = parseInt(merchantID);
                    if ((merchantID > 0) && (-1 == $.inArray(merchantID, MerchantList))) {
                        MerchantList.push(merchantID);
                    }
                }
            },
            removeMerchantID: function (merchantID) {
                if (GeneralUtil.numberCheck(merchantID)) {
                    merchantID = parseInt(merchantID);
                    GeneralUtil.removeFromArray(merchantID, MerchantList);
                }
            },
            clearMerchantIDList: function () {
                MerchantList = [];
            },
            addGroupID: function (groupID) {
                if (GeneralUtil.numberCheck(groupID)) {
                    groupID = parseInt(groupID);
                    if ((groupID > 0) && (-1 == $.inArray(groupID, GroupIDList))) {
                        GroupIDList.push(groupID);
                    }
                }
            },
            addMerchantIDForStoreLight: function (merchantIDForStoreLight) {
                if (GeneralUtil.numberCheck(merchantIDForStoreLight)) {
                    merchantIDForStoreLight = parseInt(merchantIDForStoreLight);
                    if ((merchantIDForStoreLight > 0) && (-1 == $.inArray(merchantIDForStoreLight, MerchantListForStoreLight))) {
                        MerchantListForStoreLight.push(merchantIDForStoreLight);
                    }
                }
            },
            getMerchantIDList: function () {
                return MerchantList;
            },
            getGroupIDList: function () {
                return GroupIDList;
            },
            getMerchantIDListForStoreLight: function () {
                return MerchantListForStoreLight;
            }
        }
    })();

    var EnvVariables = {
        isRecentlyContactPanelNeedShow: false, /**是否需要 显示/刷新 最近联系人**/
        hasServiceGroupPanel: false, /**页面是否包含 客服分组面板**/
        ifContainsServiceGroup: false, /**页面是否存在 客服分组**/
        isRecentlyContactPanelShow: false, /**最近联系人面板 当前是否处于显示状态**/
        needLogin: false/**是否需要登录校验**/
    };

    var TemplateManager = {
        chatIconTemplate: '<div class="chat-icon general" title="在线客服">' +
        '<a class="chat-online" href="#"><span></span></a>' +
        '</div>',
        recentlyContactTemplate: '<div class="fnmm-web-chat">' +
        '<h3 class="chat-hd"><span>最近联系人</span><i id="chat-panel-control"></i></h3>' +
        '<ul id="FNMM_Recently_Contacter_List" class="chat-list">' + '</ul>' + '</div>' +
        '<object id="ddplugin" width="0" height="0" type="application/x-implugin">' +
        '</object><div id="pluginPanel" width="0" height="0"></div>',
        contactHrefTemplate: '$url?name=$name&nickName=$nickName&gid=$gid&pageType=-1&subMerchantID=$subMerchantID',
        contactTemplate: '<li><a id="$memberid" data-href="" data-merchantid="$merchantid"><i>' +
        '<img src="' + IMGlobalConfig.metaPath + 'static/images/niutou.png"/><i title="在线"></i></i>' +
        '<span class="servicer-name">$name</span></a></li>',
        serviceGroupHeadTemplate: '<h2 class="mbs-box-title">客服分组：</h2>',
        serviceGroupTemplate: '<li class="chat-icon chat-service-group-item $groupID"><span>$groupName</span></li>',
        workTimeHeadTemplate: '<h2 class="mbs-box-title">工作时间：</h2>',
        firstWorkTimeItemTemplate: '<p>$firstWorkDayStart至$firstWorkDayEnd   $firstWorkTimeStart-$firstWorkTimeEnd</p>',
        secondWorkTimeItemTemplate: '<p>$secondWorkDayStart至$secondWorkDayEnd   $secondWorkTimeStart-$secondWorkTimeEnd</p>',
        contactHeadTemplate: '<h2 class="mbs-box-title">联系方式：</h2>',
        contactItemTemplace: '<p>$head：$content</p>',
        baseResultInfo: {
            success: false,
            data: null
        }
    };

    var ConstraintManager = {
        chatPanelVisibleFilter: function () {
            var filteredArray = [];

            $('.fnmm-chat-panel').each(function () {
                var $this = $(this);
                var imProperties = $this.data('im');

                if (imProperties && imProperties.merchantID) {
                    filteredArray.push(imProperties.merchantID);
                }
            });

            return filteredArray;
        },
        serviceGroupVisibleFilter: function () {
            var filteredArray = [];

            $('.chat-service-group .chat-service-group-item').each(function () {
                var $this = $(this);
                var groupID = $this.data('groupID');

                if (groupID) {
                    filteredArray.push(groupID);
                }
            });

            return filteredArray;
        }
    };

    var ContactsService = {
        conf: {
            baseURL: IMGlobalConfig.metaPath + 'im',
            venderURL: IMGlobalConfig.metaPath + 'vender',
            recentlyBuyerNum: 20
        },
        initEmbedEnv: function (merchantIDList, callback) {
            serviceHelper.callJsonPService({
                url: this.conf.baseURL,
                data: {
                    event: 'INIT_EMBED_ENV',
                    merchantids: merchantIDList.join(','),
                    msgnum: this.conf.recentlyBuyerNum
                },
                success: function (resultInfo) {
                    callback.call(null, resultInfo);
                }
            });
        },
        getStoreLightConfig: function (merchantIDList, callback) {
            serviceHelper.callJsonPService({
                url: this.conf.venderURL,
                data: {
                    VENDER_INFO_TYPE: 'GET_STORE_LIGHT_CONFIG',
                    sid: -1,
                    merchantids: merchantIDList.join(',')
                },
                success: function (resultInfo) {
                    callback.call(null, resultInfo || {});
                }
            });
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
        },
        getGroupOnlineInfo: function (groupIDList, callback) {
            if (groupIDList.length == 0) {
                callback.call(null, {
                    success: true,
                    data: {
                        serviceGroupOnlineInfo: []
                    }
                });
            } else {
                serviceHelper.callJsonPService({
                    url: this.conf.venderURL,
                    data: {
                        VENDER_INFO_TYPE: 'GET_SUB_MERCHANT_ONLINE_INFO',
                        sid: -1,
                        group_ids: groupIDList.join(',')
                    },
                    success: function (resultInfo) {
                        callback.call(null, resultInfo);
                    }
                });
            }
        },
        serviceDiversion: function (productInfo) {
            if (CustomerInfo.id == -1) {
                GeneralUtil.redirectToLoginPath();
            }

            var data = {
                event: 'GET_MERCHANT_SUB_ACCOUNT',
                sid: CustomerInfo.id,
                pid: productInfo.getMerchantID()
            };

            if (!!productInfo.groupID) {
                data.diverse_type = 1;
                data.group_id = productInfo.groupID;
            }

            serviceHelper.callJsonPService({
                url: this.conf.baseURL,
                data: data,
                success: function (result) {
                    if (!result) {
                        GeneralUtil.log('服务器发生故障，请稍后重试');
                    } else {
                        if (result.success) {
                            if (result.data && (result = result.data)
                                && result.subAccount.id && result.subAccount.id > 0) {
                                GeneralUtil.redirectInCurrentWindow("feiniu://?customerName=" + CustomerInfo.name
                                    + "&merchantID=" + productInfo.getMerchantID()
                                    + "&subMerchantID=" + result.subAccount.id
                                    + "&subMerchantName=" + result.subAccount.truename
                                    + "&productID=" + productInfo.getProductID());
                            } else {
                                GeneralUtil.log("当前没有在线的客服.");
                            }
                        } else {
                            GeneralUtil.log(result.errorMessage || '请刷新页面重试');
                        }
                    }
                }
            });
        },
        exit: function () {
            if (CustomerInfo.id == -1) {
                return;
            }

            serviceHelper.callJsonPService({
                url: this.conf.baseURL,
                data: {
                    event: 'EXIT',
                    sid: CustomerInfo.id,
                    did: -1
                },
                success: function () {
                }
            });
        }
    };
    var ContactsViewer = {
        _dayShowMapping: {
            1: '周一', 2: '周二', 3: '周三', 4: '周四', 5: '周五', 6: '周六', 7: '周日'
        },
        _contactContainer: null,
        _openedWindowHandler: null,

        init: function () {
            this.initData();
            this.initUI();
            this.bindEvent();
        },
        initData: function () {
            this._contactContainer = $('#FNMM_Recently_Contacter_List');
        },
        bindEvent: function () {
            /**最近联系人：启动本地/web端im**/
            $(document).on('click', '#FNMM_Recently_Contacter_List li a', function (e) {
                if (PluginManager.checkPluginSupportion()) {
                    var href = "feiniu://?";
                    var param = $(this).data('im');

                    for (var p in param) {
                        href += p + '=' + param[p] + "&";
                    }

                    GeneralUtil.redirectInCurrentWindow(href);
                    e.preventDefault();
                } else {
                    GeneralUtil.openInNewWindow($(this).attr('href'));
                    e.preventDefault();
                }
            });

            /**点击在线客服，进行相应的客服分流**/
            $(document).on('click', '.fnmm-chat-panel a.chat-online', function (e) {
                var chatPanel = $(this).closest('div.fnmm-chat-panel');
                var imData = chatPanel.data('im');
                var productInfo = new ProductInfo();

                if (!!imData) {
                    productInfo.setMerchantID(imData.merchantID);
                    productInfo.setProductID(imData.productID);
                    productInfo.setPageType(imData.pageType);
                    productInfo.setShopID(imData.shopID);
                }

                if (PluginManager.checkPluginSupportion()) {
                    /**布点页面如果发现存在客户端，才需要分流，否则把分流放入聊天窗口*/
                    GlobalEventManager.dispatchEvent(EventType.DIVERSION_SERVICE, productInfo);
                    e.preventDefault();
                    return;
                }

                var href = '';
                if (imData.pageType == 1) {
                    href = IMGlobalConfig.metaPath +
                        "?gid=" + productInfo.getMerchantID() +
                        "&productID=" + productInfo.getProductID() +
                        "&pageType=" + productInfo.getPageType();
                } else {
                    href = IMGlobalConfig.metaPath +
                        "?gid=" + productInfo.getMerchantID() +
                        "&shopID=" + productInfo.getShopID() +
                        "&pageType=" + productInfo.getPageType();
                }

                GeneralUtil.openInNewWindow(href);

                e.preventDefault();
            });

            /**客服分组内的分流**/
            $(document).on('click', '.chat-service-group>ul.service-group-panel>li.chat-icon span', function (e) {
                var groupID = $(this).closest('li.chat-icon').data('groupID');
                var chatPanel = $(this).closest('.chat-service-group');
                var imData = chatPanel.data('im');
                var productInfo = new ProductInfo();

                if (!!imData) {
                    productInfo.setMerchantID(imData.merchantID);
                    productInfo.setProductID(imData.productID);
                    productInfo.setPageType(imData.pageType);
                    productInfo.setShopID(imData.shopID);
                }

                if (!!groupID) {
                    productInfo.groupID = groupID;
                }

                if (PluginManager.checkPluginSupportion()) {
                    /**布点页面如果发现存在客户端，才需要分流，否则把分流放入聊天窗口*/
                    GlobalEventManager.dispatchEvent(EventType.DIVERSION_SERVICE, productInfo);
                    e.preventDefault();
                    return;
                }

                var href = IMGlobalConfig.metaPath +
                    "?gid=" + productInfo.getMerchantID() +
                    "&shopID=" + productInfo.getShopID() +
                    "&pageType=" + productInfo.getPageType();
                if (!!groupID) {
                    href += '&groupID=' + groupID;
                }

                GeneralUtil.openInNewWindow(href);

                e.preventDefault();
            });

            /**最近联系人 收缩/展开**/
            $(document).on('click', '.fnmm-web-chat>.chat-hd', function () {
                var indicator = $(this);
                $('#FNMM_Recently_Contacter_List').slideToggle(function () {
                    var $this = $(this);
                    indicator.children('i').toggleClass('reverse');
                    EnvVariables.isRecentlyContactPanelShow = $this.is(':visible');
                });
            });

            $(document).on('click', '#chat-ignore-all', function (e) {
                $('#FNMM_Recently_Contacter_List').slideDown();
                EnvVariables.isRecentlyContactPanelShow = false;
                e.preventDefault();
            });
        },
        initUI: function () {
            this._contactContainer.slideUp();
        },
        renderStoreLightConfig: function (data) {
            var self = this;

            $('.chat-service-group').each(function () {
                var $this = $(this);
                var serviceGroupPanel = $('<ul class="service-group-panel"></ul>');

                if (!!data.serviceGroupConfig && $.isArray(data.serviceGroupConfig) && data.serviceGroupConfig.length > 0) {
                    for (var i = 0; i < data.serviceGroupConfig.length; i++) {
                        var currentServiceGroup = data.serviceGroupConfig[i];
                        var currentServiceGroupEle =
                            TemplateManager.serviceGroupTemplate.replace(/\$groupName/, currentServiceGroup.groupName)
                                .replace(/\$groupID/, 'chat-service-group-item-' + currentServiceGroup.groupID);
                        serviceGroupPanel.append($(currentServiceGroupEle).data('groupID', currentServiceGroup.groupID));
                        DataManager.addGroupID(currentServiceGroup.groupID);
                    }
                    $this.append(TemplateManager.serviceGroupHeadTemplate).append(serviceGroupPanel);
                    EnvVariables.ifContainsServiceGroup = true;
                }

                if (!!data.generalConfig) {
                    var generalConfig = data.generalConfig;

                    if ('1' === generalConfig.isFirstWorkTimeShow || '1' === generalConfig.isSecondWorkTimeShow) {
                        var workTimePanelBuilder = TemplateManager.workTimeHeadTemplate;
                        workTimePanelBuilder = workTimePanelBuilder.concat('<div class="mbs-box-body">');

                        if ('1' === generalConfig.isFirstWorkTimeShow) {
                            workTimePanelBuilder = workTimePanelBuilder.concat(TemplateManager.firstWorkTimeItemTemplate.replace(/\$\w+/g, function (match) {
                                return match.indexOf('Day') != -1 ?
                                    self._dayShowMapping[generalConfig[match.substr(1)]] :
                                    generalConfig[match.substr(1)]
                                    ;
                            }));
                        }
                        if ('1' === generalConfig.isSecondWorkTimeShow) {
                            workTimePanelBuilder = workTimePanelBuilder.concat(TemplateManager.secondWorkTimeItemTemplate.replace(/\$\w+/g, function (match) {
                                return match.indexOf('Day') != -1 ?
                                    self._dayShowMapping[generalConfig[match.substr(1)]] :
                                    generalConfig[match.substr(1)];
                            }));
                        }

                        workTimePanelBuilder = workTimePanelBuilder.concat('</div>');
                        $this.append(workTimePanelBuilder);
                    }

                    if ('1' === generalConfig.isTelphoneShow || '1' === generalConfig.isPhoneShow) {
                        var contactTimePanelBuilder = TemplateManager.contactHeadTemplate;
                        contactTimePanelBuilder = contactTimePanelBuilder.concat('<div class="mbs-box-body">');

                        if ('1' === generalConfig.isTelphoneShow) {
                            contactTimePanelBuilder = contactTimePanelBuilder.concat(
                                TemplateManager.contactItemTemplace.replace(/\$head/, '联系手机').replace(/\$content/, generalConfig.telphone));
                        }
                        if ('1' === generalConfig.isPhoneShow) {
                            contactTimePanelBuilder = contactTimePanelBuilder.concat(
                                TemplateManager.contactItemTemplace.replace(/\$head/, '联系电话').replace(/\$content/, generalConfig.phone));
                        }

                        contactTimePanelBuilder = contactTimePanelBuilder.concat('</div">');
                        $this.append(contactTimePanelBuilder);
                    }
                }
            });
        },
        fillInContacts: function (contactList) {
            var self = this;

            if (contactList instanceof Array
                && contactList.length > 0) {

                self._contactContainer.empty();

                for (var i = 0; i < contactList.length; i++) {
                    var currentContact = contactList[i];
                    currentContact['merchantId'] = currentContact['merchantId'] || -1;
                    currentContact['storeLogoUrl'] = currentContact['storeLogoUrl'] || IMGlobalConfig.metaPath + 'static/images/niutou.png';
                    currentContact['storeName'] = currentContact['storeName'] || '';
                    currentContact['nickName'] = currentContact['nickName'] || '';

                    var data = {};
                    var toAppend = $('#contact' + currentContact.id).parent('li');

                    $.extend(data, currentContact);
                    toAppend = TemplateManager.contactTemplate
                        .replace(/\$memberid/gi, 'contact' + currentContact.id)
                        .replace(/\$name/gi, (currentContact.storeName || "").substring(0, 8) + ":" + currentContact.nickname)
                        .replace(/\$merchantid/gi, currentContact.merchantId);
                    toAppend = $(toAppend);

                    self._contactContainer.append(toAppend);
                    toAppend.children('a').attr({
                        'href': TemplateManager.contactHrefTemplate
                            .replace('$url', IMGlobalConfig.metaPath)
                            .replace('$name', CustomerInfo.name)
                            .replace('$gid', currentContact.merchantId)
                            .replace('$nickName', currentContact.nickname)
                            .replace('$subMerchantID', currentContact.id)
                    }).data('im', {
                        customerName: CustomerInfo.name,
                        merchantID: currentContact.merchantId,
                        subMerchantID: currentContact.id,
                        subMerchantName: currentContact.nickname,
                        productID: 0
                    }).find('img').attr('src', currentContact.storeLogoUrl);

                    var onlineStateClass = currentContact.state == 1 ? 'icon-offline' : 'icon-online';
                    var onlineStatePanel = toAppend.find('a');
                    onlineStatePanel.removeClass('icon-offline').removeClass('icon-online').addClass(onlineStateClass);
                    onlineStatePanel.attr('title', currentContact.state == 0 ? '在线' : '离线');
                }
            } else {
                self._contactContainer.empty().append('<p style="padding-left: 5px">暂无最近联系人</p>');
            }
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
        },
        updateGroupOnlineInfo: function (result) {
            for (var i = 0; i < result.length; i++) {
                var groupItem = result[i];
                if (groupItem.online) {
                    $('.chat-service-group-item-' + groupItem.id).removeClass('offline');
                } else {
                    $('.chat-service-group-item-' + groupItem.id).addClass('offline');
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
            var userName = GeneralUtil.cookieUtils(IMGlobalConfig.userNameFieldName);
            /**需要登录时，userID默认为0**/
            var userID = 0;

            !EnvVariables.needLogin && (userName = '');
            if (!userName) {
                userName = '';
                /**不需要登录时，userName设置为空字符串**/
                userID = -1;
                /**不需要登录时，userID默认为-1**/

                $('.fnmm-web-chat').remove();
                EnvVariables.isRecentlyContactPanelNeedShow = false;
                EnvVariables.needLogin = false;
            }

            CustomerInfo.name = userName;
            CustomerInfo.id = userID;

            ContactsViewer.init();
        },
        initData: function () {
            var self = this;

            /**如果包含客服点灯panel，获取并绘制客服点灯panel**/
            if (EnvVariables.hasServiceGroupPanel) {
                self.service.getStoreLightConfig(DataManager.getMerchantIDListForStoreLight(), function (param) {
                    GlobalEventManager.dispatchEvent(EventType.RENDER_STORE_LIGHT_CONFIG, param);
                });
            } else {
                GlobalEventManager.dispatchEvent(EventType.PREPARE_ENV, null);
            }
        },
        bindEvent: function () {
            var self = this;

            GlobalEventManager.registerEvent(EventType.DIVERSION_SERVICE, function (param) {
                self.service.serviceDiversion.call(self.service, param);
            });

            GlobalEventManager.registerEvent(EventType.RENDER_MERCHANT_ONLINE_INFO, function (resultInfo) {
                if (resultInfo.success) {
                    /**更新 在线客服在线**/
                    self.viewer.updateMerchantOnlineInfo(resultInfo.data.merchantOnlineInfo);

                    /***更新最近联系人**/
                    if (EnvVariables.hasServiceGroupPanel) {
                        self.service.getGroupOnlineInfo(ConstraintManager.serviceGroupVisibleFilter(), function (param) {
                            GlobalEventManager.dispatchEvent(EventType.RENDER_GROUP_ONLINE_INFO, param);
                        });
                    }
                }
            });
            GlobalEventManager.registerEvent(EventType.RENDER_GROUP_ONLINE_INFO, function (resultInfo) {
                if (resultInfo.success) {
                    self.viewer.updateGroupOnlineInfo(resultInfo.data.serviceGroupOnlineInfo);
                    //GlobalEventManager.dispatchEvent(EventType.UPDATE_ENV_INFO, null);
                } else {
                    GeneralUtil.log('轮询服务停止-服务端出现异常');
                }
            });
            GlobalEventManager.registerEvent(EventType.RENDER_STORE_LIGHT_CONFIG, function (resultInfo) {
                if (resultInfo.success) {
                    self.viewer.renderStoreLightConfig(resultInfo.data);
                    GlobalEventManager.dispatchEvent(EventType.PREPARE_ENV, null);
                }
            });

            GlobalEventManager.registerEvent(EventType.PREPARE_ENV, function () {
                if (EnvVariables.needLogin) {
                    /**如果检测到当前环境需要登录im**/
                    self.service.initEmbedEnv(DataManager.getMerchantIDList(), function (param) {
                        GlobalEventManager.dispatchEvent(EventType.INIT_EMBED_ENV, param);
                    });
                } else {
                    GlobalEventManager.dispatchEvent(EventType.UPDATE_ENV_INFO, 1);
                }
            });
            GlobalEventManager.registerEvent(EventType.INIT_EMBED_ENV, function (resultInfo) {
                if (resultInfo.success) {
                    var data = resultInfo.data;
                    if (data) {
                        CustomerInfo.id = data.sid;
                        self.viewer.fillInContacts(data.contactList);

                        GlobalEventManager.dispatchEvent(EventType.UPDATE_ENV_INFO, null);
                    }
                }
            });

            GlobalEventManager.registerEvent(EventType.ADD_FNMM_CHAT_PANEL, function (param) {
                if (!!param && !!param.target && param.target.hasClass('fnmm-chat-panel')) {
                    param.target.each(function () {
                        var $this = $(this);
                        $this.append(TemplateManager.chatIconTemplate);

                        var data = $this.data('im');
                        if (!!data) {
                            $this.find('.chat-online').addClass('web-chat-merchant-' + data.merchantID);

                            if (data.pageType == 1 || data.pageType == 2) {
                                $this.find('.chat-online>span').html('在线客服');
                                EnvVariables.isRecentlyContactPanelNeedShow = true;
                            } else if (data.pageType == 3 || data.pageType == 4) {
                                $this.find('.chat-icon').addClass('special').removeClass('general');
                            }

                            !!data.merchantID && DataManager.addMerchantID(data.merchantID);
                        }
                    });
                }

                clearTimeout(self.timer);
                GlobalEventManager.dispatchEvent(EventType.UPDATE_ENV_INFO, null);
            });
            GlobalEventManager.registerEvent(EventType.CLEAR_FNMM_CHAT_PANEL, function () {
                /**当购物车数据被全部清空时，不在刷新购物车商家在线情况**/
                DataManager.clearMerchantIDList();
                clearTimeout(self.timer);
            });
            GlobalEventManager.registerEvent(EventType.REMOVE_FNMM_CHAT_PANEL, function (param) {
                if (!!param && !!param.target && param.target.hasClass('fnmm-chat-panel')) {
                    param.target.each(function () {
                        var $this = $(this);
                        $this.append(TemplateManager.chatIconTemplate);

                        var data = $this.data('im');
                        if (!!data) {
                            !!data.merchantID && DataManager.removeMerchantID(data.merchantID);
                        }
                    });

                    clearTimeout(self.timer);
                    GlobalEventManager.dispatchEvent(EventType.UPDATE_ENV_INFO, null);
                }
            });

            GlobalEventManager.registerEvent(EventType.UPDATE_NON_LOGIN_ENV_INFO, function () {
                self.service.getMerchantOnlineInfo(ConstraintManager.chatPanelVisibleFilter(), function (param) {
                    GlobalEventManager.dispatchEvent(EventType.RENDER_MERCHANT_ONLINE_INFO, param);
                });
            });
            GlobalEventManager.registerEvent(EventType.UPDATE_LOGIN_ENV_INFO, function () {
                self.service.getMerchantOnlineInfo(ConstraintManager.chatPanelVisibleFilter(), function (param) {
                    GlobalEventManager.dispatchEvent(EventType.RENDER_MERCHANT_ONLINE_INFO, param);
                });
            });
            GlobalEventManager.registerEvent(EventType.UPDATE_ENV_INFO, function (timeout) {
                var type;
                if (EnvVariables.needLogin) {
                    type = EventType.UPDATE_LOGIN_ENV_INFO;
                } else {
                    type = EventType.UPDATE_NON_LOGIN_ENV_INFO;
                }

                /**触发周期-实时查询**/
                !!self.timer && clearTimeout(self.timer);
                //self.timer = setTimeout(function () {
                GlobalEventManager.dispatchEvent(type, {});
                //}, timeout || IMGlobalConfig.refreshInterval);
            });

            if (window.onpagehide || window.onpagehide != null) {
                window.addEventListener('pagehide', function () {
                    self.service.exit.call(self.service);
                }, false);
            } else {
                $(window).unload(function () {
                    self.service.exit.call(self.service);
                });
            }
        }
    };

    /**遍历布点环境，织入web-im元素、样式表，初始化相关环境变量**/
    $(document).ready(function embedElements() {
        $('.fnmm-web-chat').remove();
        //FireBreath.injectPlugin('imPlugin', document.getElementById('pluginPanel'), 'iePlugin');

        /**遍历布点环境，寻找在线客服panel**/
        $('.chat-panel, .fnmm-chat-panel').each(function () {
            var $this = $(this);
            $this.append(TemplateManager.chatIconTemplate);

            var data = GeneralUtil.deserialization($this.data('im'));
            $this.data('im', data);

            if (!!data) {
                DataManager.addMerchantID(data.merchantID);

                $this.find('.chat-online').addClass('web-chat-merchant-' + data.merchantID);
                $this.addClass('fnmm-chat-panel');

                if (data.pageType == 1 || data.pageType == 2) {
                    /**如果是店铺页或者商详页**/
                    $this.find('.chat-online>span').html('在线客服');

                    //EnvVariables.isRecentlyContactPanelNeedShow = true;
                    //EnvVariables.needLogin = true;

                } else if (data.pageType == 3 || data.pageType == 4) {
                    /**如果是购物车或者会员中心**/
                    $this.find('.chat-icon').addClass('special').removeClass('general');
                }
            }
        });

        /**遍历布点环境，寻找分组客服panel**/
        $('.chat-service-group').each(function () {
            var $this = $(this);
            var data = GeneralUtil.deserialization($this.data('im'));
            $this.data('im', data);

            if (!!data && !!data.merchantID) {
                DataManager.addMerchantIDForStoreLight(data.merchantID);
                EnvVariables.hasServiceGroupPanel = true;
            }
        });

        /**是否显示最近联系人**/
        if (!EnvVariables.isRecentlyContactPanelNeedShow) {
            /**对于非商详页和非店铺页来说，刷新间隔设置的尽量长一点，减少服务器压力**/
            IMGlobalConfig.merchantInfoRefreshInterval = 120000;
        } else {
            $('body').append(TemplateManager.recentlyContactTemplate);
        }

        GeneralUtil.dynamicWeaveCSS(IMGlobalConfig.metaPath + 'static/css/bottom.css?v=' + (new Date().getTime()));
    });

    /**布点js-mvc入口**/
    $(document).ready(function initChatApp() {
        var contactController = new ContactController(ContactsService, ContactsViewer);
        contactController.init();

        //PluginManager.init();
    });
}
