seajs.use(['$', 'modules/dialogControl', 'modules/util/generalUtil', 'modules/util/generalServiceHelper', 'JSON'],
    function (jq, dialogControl, generalUtil, serviceHelper, JSON) {
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
                        var mimeType = navigator.mimeTypes["application/x-implugin"];
                        if (mimeType) {
                            var plugin = mimeType.enabledPlugin;
                            if (plugin) return !0;
                        }
                        return !1;
                        //var pluginDiv = document.createElement("div");
                        //pluginDiv.id = "pluginDiv", pluginDiv.style.display = "none", pluginDiv.innerHTML = '<object id="objectForDetectPlugin" classid="CLSID:' + self._classid + '"></object>', document.body.appendChild(pluginDiv);
                        //try {
                        //    return void 0 == eval("objectForDetectPlugin.version") ? (pluginDiv.parentNode.removeChild(pluginDiv), !1) : (pluginDiv.parentNode.removeChild(pluginDiv), !0)
                        //} catch (e) {
                        //    return !1
                        //}
                        //return self._plugin.valid;
                    }
                })();
            },
            checkPluginSupportion: function () {
                return this._isPluginSupported;
            }
        };
        XMLHttpRequest && XMLHttpRequest.prototype && (XMLHttpRequest.prototype.sendAsBinary = function (datastr) {
            function byteValue(x) {
                return x.charCodeAt(0) & 0xff;
            }

            var ords = Array.prototype.map.call(datastr, byteValue);
            var ui8a = new Uint8Array(ords);
            this.send(ui8a.buffer);
        });

        function uploadPicSync(data, callback) {
            var xhr = new XMLHttpRequest();
            xhr.open("POST", "uploadImageSvlt", false);
            xhr.overrideMimeType("application/octet-stream");
            xhr.onreadystatechange = function () {
                if (xhr.readyState == 4 && xhr.status == 200) {

                    var obj = eval('(' + xhr.responseText + ")");

                    callback.call(this, obj.data.imageURL);
                } else if (xhr.readyState == 500) {
                    callback.call(this, null);
                }
            };
            xhr.sendAsBinary(data);
        };
        function upload_pic(data, url) {
            var xhr = new XMLHttpRequest();
            xhr.open("POST", "uploadImageSvlt");
            xhr.overrideMimeType("application/octet-stream");

            xhr.onreadystatechange = function () {
                if (xhr.readyState == 4 && xhr.status == 200) {

                    var obj = eval('(' + xhr.responseText + ")");
                    //fileDateUrlReader.onloadend = function(e) {
                    // create an image
                    var image = document.createElement("IMG");
                    image.src = obj.data.imageURL;

                    // insert the image
                    var range = window.getSelection().getRangeAt(0);
                    range.insertNode(image);
                    range.collapse(false);

                    // set the selection to after the image
                    var selection = window.getSelection();
                    selection.removeAllRanges();
                    selection.addRange(range);
                } else {

                }
            };

            xhr.sendAsBinary(data);

        }


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
        function redirectToPC(param) {
            var href = "feiniu://?";
            for (var p in param) {
                href += p + '=' + param[p] + "&";
            }
            window.location.href = href;
            setTimeout(generalUtil.closeWebPage, 2000);
        }

        var WebIM = function () {
        };
        WebIM.fn = WebIM.prototype = {
            constructor: WebIM
        };
        WebIM.extend = WebIM.fn.extend = function () {
            var target, i = 0, name, src, copy, option;
            // 参数多于两个添加到第一个
            if (arguments.length < 2) {
                target = this;
                i = 0;
            } else {
                target = arguments[0];
                i = 1;
            }
            for (; i < arguments.length; i++) {
                if (!!(option = arguments[i])) {
                    for (name in option) {
                        src = target[name];
                        copy = option[name];
                        if (!!copy && src == copy) {
                            continue;
                        }

                        target[name] = copy;
                    }
                }
            }
        };

        var MessageManager = (function () {
            var unReadMsgMap = {};

            function checkExistence(subMerchantID) {
                return !!subMerchantID && !isNaN(unReadMsgMap[subMerchantID]);
            }

            return {
                init: function (subMerchantIDList) {
                    for (var i = 0; i < subMerchantIDList.length; i++) {
                        unReadMsgMap[subMerchantIDList[i]] = 0;
                    }
                },
                addMapping: function (subMerchantID) {
                    unReadMsgMap[subMerchantID] = 0;
                },
                addUnReadMsg: function (msgList, userID) {
                    var msgList = $.isArray(msgList) ? msgList : [msgList];

                    for (var i = 0; i < msgList.length; i++) {
                        var msg = msgList[i];
                        var targetID = -1;

                        //如果是买家
                        if (msg.sender == userID) {
                            targetID = msg.recver;
                        } else {
                            targetID = msg.sender;
                        }

                        checkExistence([targetID]) && (unReadMsgMap[targetID] = unReadMsgMap[targetID] + 1);
                    }
                },
                clearMsg: function (subMerchantID) {
                    var result = 0;

                    if (checkExistence(subMerchantID)) {
                        result = unReadMsgMap[subMerchantID];
                        unReadMsgMap[subMerchantID] = 0;
                    }

                    return result;
                },
                transfer: function (src, dest) {
                    unReadMsgMap[dest] = unReadMsgMap[src];
                },
                getCopy: function () {
                    return $.extend({}, unReadMsgMap);
                }
            }
        })();

        WebIM.fn.extend({
            /**
             * 配置
             */
            conf: {
                // request 配置
                URL: 'im',
                VENDER_URL: 'vender',
                METHOD: 'GET',

                // event 类型
                E_INIT_CHAT_ENV: 'INIT_CHAT_ENV',
                E_GET_SUB_MERCHANT_ID: 'GET_MERCHANT_SUB_ACCOUNT',
                E_GETLIST: 'GET_CHAT_MESSAGE_LIST',
                E_GET_HISTORY_MESSAGE_LIST: 'GET_HISTORY_MESSAGE_LIST',
                E_PUTMSG: 'PUT_CHAT_MESSAGE',
                E_GET_RECENT_CONTACT: 'GET_RECENTYLE_CONTACTS_FOR_BUYER',
                E_EXIT: 'EXIT',

                ALL: 'all',

                M_ITEM_SELF_CLASS: 'buyer_msg',
                M_ITEM_OTHER_CLASS: 'seller_msg',
                M_ITEM_SELF_ABSENT_CLASS: 'buyer_absent_msg',
                M_ITEM_OTHER_ABSENT_CLASS: 'seller_absent_msg',

                // type 消息类型
                TYPE_GENERAL_MESSAGE: 0,
                TYPE_EVALUATION: 7,
                TYPE_MAKING_EVALUATION: 17,
                TYPE_IMAGE_URL: 19,
                TYPE_SUB_MERCHANT_SWITCH: 20,
                TYPE_WELCOME_MESSAGE: 21,
                TYPE_SYNC_SUBMERCHANT_LIST_IN_CHATTING: 23,
                TYPE_PREPARE_TO_CLOSE: 24,

                // 每次取历史记录的条数
                NUMBER_OF_HISTORY_MESSAGE_FETCH: 25,
                //消息体最大字符数
                MAX_MESSAGE_CONTENT_LENGTH: 500
            },
            /**
             * 用户名
             */
            cache: {
                timeDeltaSeconds: 0,
                //用户标示符
                id: '',
                //用户姓名
                uname: '',
                //用户昵称
                nickName: '',
                /**
                 * 当前用户显示名称
                 */
                showName: '',
                //当前买家头像
                buyerImgUrl: '',
                //当前窗口在用户进程中的序列号
                uuid: '',
                prepared: false,
                hasCorrectlyInit: false,
                //当前正在聊天客服所属商家ID
                merchantID: '',
                //当前正在聊天客服 用户名
                subMerchantName: '',
                //当前正在聊天客服 标示符
                subMerchantID: '',
                //当前客服头像
                sellerImgUrl: '',
                productID: '',
                flag: '',
                orderID: '',
                shopID: '',
                guid: '',
                //历史消息获取情况
                historyBtnInfoMap: {},
                //已经解析的url会放入本地缓存
                hrefCache: {},
                //未弹出的评价窗口映射
                unHandledEvaluation: {}
            },
            data: {
                historyRequestInfo: {
                    currentTime: 0,
                    hasFetched: 0,
                    hasMore: true,
                    isFailed: false,
                    currentFetchNum: 0,
                    lastMsgid: 0
                }
            },
            send: function (data, fn, url, opts) {
                var me = this;

                $.ajax($.extend({
                    url: url || me.conf.URL,
                    type: me.conf.METHOD,
                    data: $.extend({}, data, {uuid: me.cache.uuid}),

                    success: function (text) {
                        var obj = null;
                        try {
                            obj = JSON.parse(text);
                        } catch (e) {
                            // alert(e);
                        }
                        fn.call(me, obj);
                    },
                    error: function (XMLHttpRequest, textStatus, errorThrown) {
                        if (XMLHttpRequest.readyState != 0) {
                            fn.call(me, {
                                success: false,
                                errorMessage: '服务器响应超时'
                            });
                        }
                    }
                }, opts || {}));
            },
            init: function (id, name) {
                this.cache.id = id;
                this.cache.uname = name;
                var me = this;

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
                me.send({
                    'VENDER_INFO_TYPE': 'SYNC_TIME'
                }, function (obj) {
                    if (obj.success) {
                        var currentTime = new Date().getTime() / 1000;
                        me.cache.timeDeltaSeconds = obj.data.serverTime - currentTime;
                        me.conf.MAX_MESSAGE_CONTENT_LENGTH = obj.data.maxMessageContentLength;

                        me.initEnv();
                    } else {
                        alert('初始化聊天窗口失败：同步系统时间发生故障');
                        dialogControl.prepareToClose();

                        window.close();
                    }
                }, me.conf.VENDER_URL);
            },
            initEnv: function () {
                $('#rightSlideBar').hide();

                /**初始化聊天窗口各项参数**/
                var me = this;
                var data = {
                    event: me.conf.E_INIT_CHAT_ENV,
                    sid: me.cache.id,
                    pid: me.cache.merchantID,
                    nickName: me.cache.subMerchantName || '',
                    productID: me.cache.productID
                };

                if (!!me.cache.groupID) {
                    data.diverse_type = 1;
                    data.group_id = me.cache.groupID;
                }
                if (!!me.cache.subMerchantID) {
                    data.sub_merchant_id = me.cache.subMerchantID;
                }

                me.send(data, function (obj) {
                    var data;
                    if (!!obj.success && !!(data = obj.data)) {
                        var merchantInfo = data.subAccount;

                        me.cache.id = data.userID;
                        me.cache.uname = data.userName;
                        me.cache.nickName = data.nickName || '';

                        if (me.cache.nickName) {
                            me.cache.showName = data.nickName + '(' + data.userName + ')';
                        } else {
                            me.cache.showName = data.userName;
                        }

                        me.cache.subMerchantID = merchantInfo.id;
                        me.cache.subMerchantName = merchantInfo.truename || merchantInfo.username || "";
                        me.cache.uuid = data.uuid;
                        me.cache.buyerImgUrl = data.imgUrl || 'static/images/moumoupc/icon(1).png';
                        me.cache.prepared = true;

                        document.title = me.cache.subMerchantName;
                        var onlineStatus = data.subAccount.online ? '在线' : '不在线';
                        $('#J_current_service').html('：' + me.cache.subMerchantName + '[' + onlineStatus + ']');

                        me.getRecentContact();
                    } else {
                        if (!obj.needLogin) {
                            alert('初始化聊天窗口失败：' + obj.errorMessage || '服务器发生故障');
                            dialogControl.prepareToClose();
                            return;
                        } else {
                            generalUtil.redirectInCurrentWindow(me.conf.LOGIN_HREF +
                                "?u=" + generalUtil.encodeUsingBase64(window.location.href));
                            return;
                        }
                    }
                });
            },
            getRecentContact: function () {
                var me = this;
                var data = {
                    event: me.conf.E_GET_RECENT_CONTACT,
                    sid: me.cache.id,
                    senderID: me.cache.id,
                    msgNum: 1
                };

                me.send(data, function (obj) {
                    var data;
                    if (!!obj.success && !!(data = obj.data)) {
                        var submerchantInChatting = [];
                        for (var i in data.contactList) {
                            var contact = data.contactList[i];
                            contact.storeName = contact.storeName || '';
                            MessageManager.addMapping(contact.id);

                            //如果是最近联系人
                            if (!contact.tag) {
                                $('.person #J_recent_list').append(
                                    "<li class='on " + (contact.state == 0 ? '' : 'ui-off-line') + "' data-merchant-id=" + contact.merchantId +
                                    " data-submerchant-id=" + contact.id +
                                    " data-submerchant-name=" + contact.nickname +
                                    " data-store-name=" + contact.storeName +
                                    " data-product-id=" + contact.skuID +
                                    " data-usersign=" + (contact.usersign || '&nbsp;') +
                                    " data-state=" + contact.state + ">"
                                    + "<img class='tuxiang' src='" + (contact.headImgUrl || 'static/images/moumoupc/icon(1).png')
                                    + "' width='30' height='35'><span title='" + contact.storeName + ": " + contact.nickname + "'>"
                                    + contact.storeName + ": " + contact.nickname + "</span>"
                                    + "<div class='msg-num-box'>0</div>"
                                    + "</li>"
                                );
                            } else if (!!contact.tag) {
                                //如果是正在联系人
                                submerchantInChatting.push(contact);
                                if (contact.id == me.cache.subMerchantID) {
                                    me.cache.sellerImgUrl = contact.headImgUrl || 'static/images/moumoupc/icon(1).png';
                                }
                            }
                        }

                        $("#J_recent_label").html("最近联系人 (" + $('.person #J_recent_list').children().length + "人)");
                        dialogControl.syncChattingList(submerchantInChatting);
                        me.initData(obj.data);
                    }
                });
            },
            initData: function (data) {
                var gid = this.cache.subMerchantID;
                var me = this;

                /**检查本机是否安装了pc版客户端**/
                if (PluginManager.checkPluginSupportion()) {
                    redirectToPC({
                        merchantID: me.cache.merchantID,
                        subMerchantID: me.cache.subMerchantID,
                        subMerchantName: me.cache.subMerchantName,
                        customerName: me.cache.uname,
                        productID: me.cache.productID
                    });
                }

                window.refreshTitleHandler = function (title) {
                    dialogControl.resetFlashTitle(title);
                }

                dialogControl.refresh({
                    userID: me.cache.id,
                    subMerchantID: me.cache.subMerchantID,
                    onSendingMessage: function (html, features) {
                        //update UI
                        var msg = html.replace(/<img[^>]*src="([^"]+\/([^>]+\.\w+))"[^>]*>/gi, function ($0, $1, $2) {
                            if ($1.indexOf('http://') == 0) {
                                return '<image:' + $1 + '>';
                            } else {
                                return '<image:' + $2 + '>';
                            }
                        }).replace(/<br\/?>/gi, '\r').replace(/&nbsp;?/gi, ' ').replace(/<\/?[^iI][^>]*>/gi, '')
                            .replace(/&lt;/g, '<').replace(/&gt;/g, '>');

                        //convert base64 image to URL image,and send to others
                        var text = html;
                        var re = /"data:image\/png;base64,.+?"/g;
                        var tag = "data:image\/png;base64,";
                        var needUpload = false;
                        var base64ImgArray = text.match(re);
                        var index = 0;

                        while (base64ImgArray != null && index < base64ImgArray.length) {
                            //convert to binary data
                            var base64_imgStr = base64ImgArray[index];
                            base64_imgStr = base64_imgStr.substring(tag.toString().length + 1, base64_imgStr.toString().length - 1);

                            var binary_str = atob(base64_imgStr);

                            //同步上传图片到图片服务器，应该是异步比较好，目前不支持
                            uploadPicSync(binary_str, function (url) {
                                if (url != null)
                                    text = text.replace(tag + base64_imgStr, url);
                                else {
                                    me.addItemSelf("图片上传失败", features, new Date().getTime() + me.cache.timeDeltaSeconds * 1000);
                                    return;
                                }
                            });

                            index++;
                            needUpload = true;
                        }

                        if (needUpload) {
                            msg = text.replace(/<img[^>]*src="([^"]+\/([^>]+\.\w+))"[^>]*>/gi, function ($0, $1, $2) {
                                if ($1.indexOf('http://') == 0) {
                                    return '<image:' + $1 + '>';
                                } else {
                                    return '<image:' + $2 + '>';
                                }
                            }).replace(/<br\/?>/gi, '\r').replace(/&nbsp;?/gi, ' ').replace(/<\/?[^iI][^>]*>/gi, '')
                                .replace(/&lt;/g, '<').replace(/&gt;/g, ' >');
                        }

                        if (msg.length > me.conf.MAX_MESSAGE_CONTENT_LENGTH) {
                            alert('您发送的消息超过了最大上限：' + me.conf.MAX_MESSAGE_CONTENT_LENGTH + "，请重新输入");
                            return;
                        }

                        var imageRegex = /<image:([^>]+)>/;
                        msg = msg.replace(/<\/?[^>]+>/g, function ($0) {
                            if (imageRegex.test($0)) {
                                return $0;
                            } else {
                                return $0.replace(/</g, '&lt;').replace(/>/g, '&gt;');
                            }
                        });

                        me.addItemSelf(msg, features, new Date().getTime() + me.cache.timeDeltaSeconds * 1000);
                        $('.textarea').empty();

                        me.putMsg({
                            msg: msg,
                            features: JSON.stringify(features),
                            did: gid || me.conf.ALL,
                            type: me.conf.TYPE_GENERAL_MESSAGE
                        });
                    },
                    onSendingEvaluation: function (html, evaluation, features) {
                        me.putMsg({
                            did: gid || me.conf.ALL,
                            msg: html,
                            type: me.conf.TYPE_MAKING_EVALUATION,
                            version: evaluation,
                            features: JSON.stringify(features)
                        });
                    },
                    onRequestHistoryList: function (time, callback) {
                        me.getHistoryList(time, callback);
                    },
                    onNewURLParsed: function (linkType, href, linkData, globalHrefID) {
                        //如果命中缓存
                        if (me.cache.hrefCache[href]) {
                            setTimeout(function () {
                                dialogControl.renderURLThumbInfo(linkType, me.cache.hrefCache[href], globalHrefID);
                            }, 20);
                        } else {
                            var data = {};
                            if (linkType === 'store') {
                                data['VENDER_INFO_TYPE'] = 'GET_STORE_INFO';
                                data['merchant_id'] = linkData['merchantID'];
                            } else if (linkType === 'item') {
                                data['VENDER_INFO_TYPE'] = 'GET_PRODUCT_INFO';
                                data['product_id'] = linkData['skuID'];
                            }

                            if (!$.isEmptyObject(data)) {
                                me.send(data, function (obj) {
                                    if (obj.success) {
                                        var cachedData;
                                        if (linkType === 'store') {
                                            cachedData = obj.data.storeInfo;
                                        } else if (linkType === 'item') {
                                            cachedData = obj.data.goodsAsking;
                                        }

                                        //1. 放入本地缓存
                                        me.cache.hrefCache[href] = cachedData;
                                        //2. 通知刷新链接内容
                                        dialogControl.renderURLThumbInfo(linkType, cachedData, globalHrefID);
                                    }
                                }, me.conf.VENDER_URL);
                            }
                        }
                    },
                    onResetReceiver: function (merchantInfo) {
                        me.cache.merchantID = merchantInfo.merchantID;
                        me.cache.subMerchantID = merchantInfo.subMerchantID;
                        me.cache.subMerchantName = merchantInfo.subMerchantName;
                        me.cache.productID = merchantInfo.productID;
                        me.cache.sellerImgUrl = merchantInfo.imgUrl;

                        document.title = me.cache.subMerchantName;
                        var onlineStatus = merchantInfo.state == 0 ? "在线" : "不在线";
                        $('#J_store_name').html(merchantInfo.storeName);
                        $('#J_current_service').html('：' + me.cache.subMerchantName + '[' + onlineStatus + ']');

                        var trimmedUserSign = generalUtil.trim(merchantInfo.usersign);

                        if (trimmedUserSign == '' || (/*ie7 空格bug*/trimmedUserSign.length == 1 && trimmedUserSign.charCodeAt(0) == 160)) {
                            $('#nav>.merchant-usersign').addClass('hide');
                            $('#J_merchant_info').css({height: '45px'});
                            $('#nav .merchant-info').addClass('expand');
                        } else {
                            $('#nav>.merchant-usersign').removeClass('hide').html(merchantInfo.usersign);
                            $('#J_merchant_info').css({height: '24px'});
                            $('#nav .merchant-info').removeClass('expand');
                        }

                        //隐藏所有聊天记录
                        $('.seller, .buyer, .sys-message').hide();
                        $('.recv-' + merchantInfo.subMerchantID).show();

                        // 判断与该商家是否还有历史消息可以获取
                        if (me.cache.historyBtnInfoMap[me.cache.subMerchantID] != null && !me.cache.historyBtnInfoMap[me.cache.subMerchantID + ""].hasMore) {
                            dialogControl.finishHistorylistRetrieve("没有更多消息了~");
                        } else {
                            dialogControl.continueHistroylistRetrieve();
                        }

                        //重置未读消息数
                        var readMsgNum = MessageManager.clearMsg(merchantInfo.subMerchantID);
                        if (readMsgNum > 0) {
                            dialogControl.syncUnReadMsgList(MessageManager.getCopy());
                        }

                        //判断是否有 评价请求未处理
                        if (me.cache.unHandledEvaluation[merchantInfo.subMerchantID]) {
                            dialogControl.showEvaluationWindow();
                            me.cache.unHandledEvaluation[merchantInfo.subMerchantID] = false;
                        }

                        //右侧信息同步
                        var refreshRight = document.getElementById('rightSlideBar').contentWindow.refreshRight;
                        $.isFunction(refreshRight) && refreshRight.call(null, {
                            merchantID: me.cache.merchantID,
                            subMerchantID: me.cache.subMerchantID,
                            productID: me.cache.productID
                        });
                    }
                });
                dialogControl.attachWindowCloseListen(me.exit, me);

                me.getList();
            },
            checkStatus: function () {
                return this.cache.prepared;
            },
            exit: function () {
                var me = this;

                $.ajax({
                    url: me.conf.URL + "?sid=" + me.cache.id + "&did=" + me.cache.subMerchantID + "&uuid=" + me.cache.uuid +
                    "&event=" + me.conf.E_EXIT,
                    type: "get",
                    async: !1,
                    timeout: 3e3,
                    success: function () {
                    }
                });
            },
            putMsg: function (message) {
                if (!this.checkStatus()) {
                    return;
                }

                var me = this;
                var data = {};

                me.extend(data, message, {
                    event: me.conf.E_PUTMSG,
                    sid: me.cache.id,
                    did: me.cache.subMerchantID,
                    sender: me.cache.id,
                    recver: me.cache.subMerchantID,
                    time: new Date().getTime() + me.cache.timeDeltaSeconds * 1000
                });
                me.send(data, function (obj) {
                    if (!obj || !obj.success) {
                        alert(obj.errorMessage || '服务器发生故障');
                        me.cache.prepared = false;
                        return;
                    }
                });
            },
            /**
             * 获取消息列表
             */
            getList: function () {
                if (!this.checkStatus()) {
                    return;
                } else if (!this.cache.hasCorrectlyInit) {
                    var href = 'right.html?mid=' + this.cache.merchantID + '&flag=' + this.cache.flag;
                    if (this.cache.flag == 1) {
                        href += '&pid=' + this.cache.productID;
                    } else if (this.cache.flag == 2) {
                        href += '&sid=' + this.cache.shopID;
                    }
                    if (this.cache.merchantID == -1) {
                        href += '&smid=' + this.cache.subMerchantID;
                    }

                    $('#rightSlideBar').attr('src', href).show();
                    this.cache.hasCorrectlyInit = true;
                }

                var me = this;

                me.send({
                    event: me.conf.E_GETLIST,
                    sid: me.cache.id,
                    did: me.cache.subMerchantID
                }, function (obj) {
                    if (obj) {
                        if (!!obj.success && !!obj.data) {
                            try {
                                me.handlerMessageList(obj.data.messageList);
                                me.getList();
                            } catch (e) {
                                me.exit();
                                me.cache.prepared = false;

                                dialogControl.prepareToClose();
                                return;
                            }
                        } else {
                            alert(obj.errorMessage || '服务器发生故障');

                            dialogControl.prepareToClose();
                            return;
                        }
                    } else {
                        alert('服务器发生故障');

                        dialogControl.prepareToClose();
                        return;
                    }
                }, null, {timeout: 120000});
            },
            getHistoryList: function (baseTime, callback) {
                if (!this.checkStatus()) {
                    return {
                        isSuccess: false,
                        canRequestAgain: false,
                        errorMessage: '开启会话失败'
                    };
                }
                //this.data.historyRequestInfo.currentFetchNum = this.conf.NUMBER_OF_HISTORY_MESSAGE_FETCH;
                var me = this;
                var historyRequestInfo = me.cache.historyBtnInfoMap[me.cache.subMerchantID] || me.data.historyRequestInfo;
                me.send({
                    event: me.conf.E_GET_HISTORY_MESSAGE_LIST,
                    sid: me.cache.id,
                    fid: me.cache.subMerchantID,
                    time: baseTime,
                    msgid: historyRequestInfo.lastMsgid,
                    msgnum: me.conf.NUMBER_OF_HISTORY_MESSAGE_FETCH
                }, function (obj) {
                    var result = {};

                    if (!!obj && !!obj.success) {
                        var data = obj.data.historyRequestInfo;
                        if (!!data) {
                            var historyInfo = historyRequestInfo;
                            var msgInfoList = data.msgInfo;

                            historyInfo.isFailed = data.isFailed;
                            historyInfo.hasMore = data.hasMore;

                            var isSuccess = true;
                            var canRequestAgain = true;
                            var errorMessage = '';

                            if (historyInfo.isFailed) {
                                isSuccess = false;
                                canRequestAgain = true;
                                errorMessage = '服务器出现错误，请稍后重试';
                            } else {
                                for (var i = 0; i < msgInfoList.length; i++) {
                                    var currentItem = msgInfoList[i];

                                    if (currentItem.id == me.cache.id) {
                                        me.addItemSelf(currentItem.msg, currentItem.features, currentItem.time, true);
                                    } else {
                                        me.eventItem($.extend(currentItem, {isPrepend: true, type: 0}));
                                    }
                                }

                                if (msgInfoList.length > 0) {
                                    historyRequestInfo.lastMsgid = msgInfoList[i - 1].msgid
                                }

                                if (!historyInfo.hasMore) {
                                    isSuccess = true;
                                    canRequestAgain = false;
                                    errorMessage = '没有更多消息了~';
                                }
                            }

                            result = {
                                isSuccess: isSuccess,
                                canRequestAgain: canRequestAgain,
                                errorMessage: errorMessage
                            };
                        } else {
                            result = {
                                isSuccess: false,
                                canRequestAgain: true,
                                errorMessage: '服务器请求超时，请点击重试'
                            };
                        }
                    } else {
                        result = {
                            isSuccess: false,
                            canRequestAgain: true,
                            errorMessage: '服务器出现错误，请稍后重试'
                        };
                    }

                    me.cache.historyBtnInfoMap[me.cache.subMerchantID] = historyRequestInfo;
                    callback.call(this, result);
                });
            },
            /**
             *
             * @param obj
             */
            handlerMessageList: function (messageList) {
                var me = this;
                var data = messageList || [];
                var hasValidMessage = false;

                if ((data instanceof Array && data.length > 0)
                    || (!data instanceof Array && !!data)) {
                    if ($.isArray(data)) {
                        // 如果数据是数组
                        $.each(data, function (i, v) {
                            if (!!v.msg) {
                                var currentMessageValid = me.eventItem(v);
                                hasValidMessage = hasValidMessage || currentMessageValid;
                            }
                        });
                    } else {
                        if (!!v.msg) {
                            hasValidMessage = me.eventItem(v);
                        }
                    }

                    if (hasValidMessage) {
                        dialogControl.flashTitle();
                        dialogControl.playSound();

                        MessageManager.clearMsg(me.cache.subMerchantID);
                        dialogControl.syncUnReadMsgList(MessageManager.getCopy());
                    }
                }
            },
            /**
             * 根据event类型处理消息
             *
             * @param v
             */
            eventItem: function (v) {
                var me = this;
                var isMessageAccept = false;

                if (!!v.msg) {
                    if (me.conf.TYPE_PREPARE_TO_CLOSE == v.type) {
                        throw  {};
                    } else if (me.conf.TYPE_SYNC_SUBMERCHANT_LIST_IN_CHATTING == v.type) {
                        isMessageAccept = true;
                        //dialogControl.syncChattingList(JSON.parse(v.msg || {}));
                    } else if (me.conf.TYPE_GENERAL_MESSAGE == v.type || me.conf.TYPE_WELCOME_MESSAGE == v.type) {
                        var speaker = (v.sender || v.id || '') + '';
                        var time;
                        if (generalUtil.isBeforeToday(v.time)) {
                            time = generalUtil.formatTime(v.time);
                        } else {
                            time = generalUtil.formatDate(v.time);
                        }

                        if (me.cache.id === speaker) {
                            /*买家发言*/
                            //判断是发给当前客服
                            if (me.cache.subMerchantID == v.recver) {
                                me.addItem(v.recver, me.conf.M_ITEM_SELF_CLASS, me.cache.showName,
                                    time, v.msg, v.features, v.isPrepend);
                            } else {//如果不是发给当前客服
                                me.addItem(v.recver, me.conf.M_ITEM_SELF_ABSENT_CLASS, me.cache.showName,
                                    time, v.msg, v.features, v.isPrepend);
                            }
                        } else if (me.cache.subMerchantID == speaker) {
                            /*当前卖家发言*/
                            me.addItem(me.cache.subMerchantID, me.conf.M_ITEM_OTHER_CLASS, me.cache.subMerchantName,
                                time, v.msg, v.features, v.isPrepend);
                        } else {
                            /*非当前卖家发言*/
                            var subMerchantName = $("[data-submerchant-id=" + speaker + "]").attr("data-submerchant-name");
                            me.addItem(v.sender, me.conf.M_ITEM_OTHER_ABSENT_CLASS, subMerchantName,
                                time, v.msg, v.features, v.isPrepend);
                        }

                        isMessageAccept = true;
                    }
                    else if (me.conf.TYPE_IMAGE_URL == v.type) {
                        var msgObj = JSON.parse(v.msg);
                        var imgName = msgObj["imgName"].replace(/^{([^.]+)}\.\w+$/, '$1');
                        var imgUrl = msgObj["imgUrl"];

                        $('.fn-image-wait-to-set[data-id="' + imgName + '"]')
                            .attr('src', imgUrl).removeClass('fn-image-wait-to-set');

                        isMessageAccept = true;
                    }
                    else if (me.conf.TYPE_EVALUATION == v.type) {
                        var isCurrentKefu = v.sender == me.cache.subMerchantID;

                        if (isCurrentKefu) {
                            dialogControl.showEvaluationWindow();
                        } else {
                            me.cache.unHandledEvaluation[v.sender] = true;
                        }

                        isMessageAccept = true;
                    } else if (me.conf.TYPE_SUB_MERCHANT_SWITCH == v.type) {
                        var msgObj = JSON.parse(v.msg) || {};

                        var switchStatus = msgObj.switchStatus;
                        if (switchStatus == 1) {
                            //dialogControl.appendSysMessages('[转接客服]已将您转接至客服：' + msgObj.toSubMerchantName,
                            //    true, msgObj.fromSubMerchantID);
                            isMessageAccept = true;
                        } else if (switchStatus == 0) {
                            dialogControl.appendSysMessages('---[' + me.cache.subMerchantName + ']已将您转接至客服：'
                                + msgObj.toSubMerchantName, true, msgObj.fromSubMerchantID);

                            setTimeout(function () {
                                var srcInfo = {id: msgObj.fromSubMerchantID, name: me.cache.subMerchantName};
                                var destInfo = {id: msgObj.toSubMerchantID, name: msgObj.toSubMerchantName};
                                var isCurrentKefu = msgObj.fromSubMerchantID == me.cache.subMerchantID;

                                isCurrentKefu && $('#J_current_service').html('：' + msgObj.toSubMerchantName + '[在线]');

                                MessageManager.transfer(msgObj.fromSubMerchantID, msgObj.toSubMerchantID);
                                me.cache.subMerchantID = msgObj.toSubMerchantID;
                                me.cache.subMerchantName = msgObj.toSubMerchantName;

                                dialogControl.switchSubMerchantInfo(srcInfo, destInfo);
                                dialogControl.appendSysMessages('---[' + srcInfo.name + ']已将您转接至客服：'
                                    + msgObj.toSubMerchantName, true, msgObj.toSubMerchantID);
                                dialogControl.appendSysMessages('---将由客服人员[' + msgObj.toSubMerchantName + '] 为您服务',
                                    true, msgObj.toSubMerchantID);
                            }, 2000);
                        }
                    }
                }

                isMessageAccept && MessageManager.addUnReadMsg(v, me.cache.id);

                return isMessageAccept;
            },
            addItemSelf: function (text, features, time, isPrepend) {
                if (text) {
                    var me = this;
                    if (isPrepend || generalUtil.isBeforeToday(time)) {
                        time = generalUtil.formatTime(time);
                    } else {
                        time = generalUtil.formatDate(time);
                    }

                    me.addItem(me.cache.subMerchantID, me.conf.M_ITEM_SELF_CLASS, me.cache.showName,
                        time, text, features, isPrepend);
                }
            },
            /**
             * 将消息替换模板,并添加
             *
             * @param className
             * @param name
             * @param time
             * @param text
             *
             */
            addItem: function (subMerchantId, className, name, time, text, features, isPrepend) {
                var message = {
                    subMerchantId: subMerchantId,
                    type: className,
                    html: text || '',
                    features: features,
                    time: time,
                    name: name
                };
                var context = {sellerImgUrl: this.cache.sellerImgUrl, buyerImgUrl: this.cache.buyerImgUrl};

                if (isPrepend) {
                    dialogControl.prependMessages(message, context);
                } else {
                    dialogControl.appendMessage(message, context);
                }
            }
        });

        $(function () {
            var loginHref = 'https://passport.feiniu.com/gateway/login';
            var checkLoginHref = 'https://passport.feiniu.com/login/checkin';
            var currentHref = window.location.href;
            var currentDomain = currentHref.replace(new RegExp('^https?://([^/]+)/.+$'), '$1');

            if (currentDomain.indexOf('beta1.fn') > -1)/**beta环境**/{
                loginHref = 'https://passport.beta1.fn/gateway/login';
                checkLoginHref = 'https://passport.beta1.fn/login/checkin';
            } else if (currentDomain.indexOf('dev1.fn') > -1)/**dev环境**/{
                checkLoginHref = 'https://passport.dev1.fn/login/checkin';
                loginHref = 'https://passport.dev1.fn/gateway/login';
            }

            var webIM = new WebIM();

            var href = decodeURI(location.href);
            var gid = generalUtil.getQueryValue(href, 'gid');
            var nickName = generalUtil.getQueryValue(href, 'nickName');
            var id = generalUtil.getQueryValue(href, 'id');
            var productID = generalUtil.getQueryValue(href, 'productID');
            var flag = generalUtil.getQueryValue(href, 'pageType');
            var shopID = generalUtil.getQueryValue(href, 'shopID');
            var groupID = generalUtil.getQueryValue(href, "groupID");
            var subMerchantID = generalUtil.getQueryValue(href, "subMerchantID");
            id = !!id ? id : 0;
            var name;

            serviceHelper.callJsonPService({
                url: checkLoginHref,
                data: {},
                success: function (data) {
                    if (data.code != '200') {
                        generalUtil.redirectInCurrentWindow(loginHref + "?u=" + generalUtil.encodeUsingBase64(window.location.href));
                    } else {
                        name = data.data.MEM_USERNAME;
                        webIM.cache.merchantID = gid;
                        webIM.cache.subMerchantName = nickName;
                        webIM.cache.subMerchantID = subMerchantID;
                        webIM.cache.productID = productID;
                        webIM.cache.flag = flag;
                        webIM.cache.shopID = shopID;
                        webIM.cache.groupID = groupID;

                        webIM.init(id, name);

                        PluginManager.init();
                    }
                }
            });
        });
    })
;
