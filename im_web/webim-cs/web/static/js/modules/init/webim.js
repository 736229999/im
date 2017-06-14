/**
 * Author: morningking
 * Date: 2015/10/22
 * Contact: 243717042@qq.com
 */

//页面配置项
var PageConfig = (function () {
    window.member_url_view = 'https://passport.feiniu.com';
    var webimServiceBaseUrl = 'http://merchant.feiniu.com/webim';
    var loginHref = 'https://passport.feiniu.com/gateway/login';
    var checkLoginHref = 'https://passport.feiniu.com/login/checkin';
    var uploggerUrl = 'http://static01.fn-mart.com/product/js/lib/upLogger.js';
    var currentHref = window.location.href;
    var currentDomain = currentHref.replace(new RegExp('^https?://([^/]+)/.+$'), '$1');
    var isLocal = currentDomain.indexOf('local') == 0;

    if (currentDomain.indexOf('beta1.fn') > -1)/**beta环境**/{
        loginHref = 'https://passport.beta1.fn/gateway/login';
        checkLoginHref = 'https://passport.beta1.fn/login/checkin';
        window.member_url_view = 'https://passport.beta1.fn';

        webimServiceBaseUrl = 'http://webim.beta1.fn/webim';
        uploggerUrl = 'http://static.beta1.fn/product/js/lib/upLogger.js?' + new Date().getTime();
    } else if (currentDomain.indexOf('dev1.fn') > -1)/**dev环境**/{
        checkLoginHref = 'https://passport.dev1.fn/login/checkin';
        loginHref = 'https://passport.dev1.fn/gateway/login';
        window.member_url_view = 'https://passport.dev1.fn';
        webimServiceBaseUrl = 'http://webim.dev1.fn/webim';
    }

    if (isLocal) {
        webimServiceBaseUrl.replace(/^http:\/\/webim/, 'http://local');
    }

    return {
        //获取表情根地址
        getEmotionBaseUrl: function () {
            return webimServiceBaseUrl + "/static/images/feiniu-emotion/";
        },
        //获取 im服务根地址
        getLocalImServiceBaseUrl: function () {
            return 'webim/im';
        },
        //获取 本服务器根地址
        getLocalVenderServiceBaseUrl: function () {
            return 'webim/vender';
        },
        //会员中心 登录校验地址
        getMemberPassportCheckUrl: function () {
            return checkLoginHref;
        },
        //会员中心 登录地址
        getMemberLoginUrl: function () {
            return loginHref;
        },
        redirectToLoginHref: function () {
            fnDialog({});
        },
        getUploggerUrl: function () {
            return uploggerUrl;
        }
    };
})();

seajs.use([
    '$',
    'modules/dialogControl',
    'JSON',
    PageConfig.getUploggerUrl()
], function (jq, dialogControl, JSON) {
    //默认访问必带参数
    var defaultRequiredParams = {
        ismobile: 0,
        moduleno: 1
    };

    var remoteServiceInvokeUtil = new RemoteServiceInvokeUtil({
        defaultParams: defaultRequiredParams
    });

    //请求事件类型
    var EventType = {
        //同步系统时间
        SYNC_TIME: 'SYNC_TIME',
        //初始化会话环境
        INIT_CHAT_ENV: 'INIT_CHAT_ENV',
        //获取最新排队信息
        PULL_QUEUE_INFO: 'PULL_QUEUE_INFO',
        //拉取消息
        GET_LIST: 'GET_CHAT_MESSAGE_LIST',
        //发送消息
        PUT_MSG: 'PUT_CHAT_MESSAGE',
        //退出会话
        EXIT: 'EXIT'
    };

    //消息类型
    var MessageType = {
        //普通消息
        GENERAL_MESSAGE: 0,
        //客服请求服务评价
        EVALUATION: 7,
        //买家回应服务评价
        MAKING_EVALUATION: 17,
        //图片消息
        IMAGE_URL: 19,
        //客服转接
        SUB_MERCHANT_SWITCH: 20,
        //欢迎消息
        WELCOME_MESSAGE: 21,
        //退出会话
        PREPARTO_CLOSE: 24,
        //提醒买家及时发言
        ALERT_SPEAK_TIME: 25,
        //主管插入会话
        MONITOR_TAKE_OVER_CHAT: 26
    };

    //转接状态类型
    var SwitchStatusType = {
        //普通转接开始
        'GENERAL_SWITCH_START': 1,
        //普通转接结束
        'GENERAL_SWITCH_SUCC': 0,
        //主管接入
        'MONITOR_SWITCH_SUCC': 2
    };

    //webIM相关逻辑
    var WebIM = (function () {
        //会话配置项
        var SessionConfig = {
            //请求排队间隔
            getQueueInterval: function () {
                return 2000;
            }
        };
        //会话状态： 正常|不正常
        var StatusManager = (function () {
            //是否成功初始化
            var isStatusNormal = false;

            return {
                markStatusNormal: function () {
                    isStatusNormal = true;
                    QueuedCountDownLatch.reset();
                },
                markStatusAbnormal: function () {
                    isStatusNormal = false;

                    SessionDataManager.setUuid('');
                    SessionDataManager.setCurrentConsultantID('');
                    SessionDataManager.clearContextData();
                    ConsultantMap.clear();
                    QueuedCountDownLatch.reset();

                    dialogControl.frozenChatArea();
                    dialogControl.detachWindowCloseListen();
                },
                isStatusNormal: function () {
                    return !!isStatusNormal;
                }
            }
        })();
        //会话相关数据
        var SessionDataManager = {
            //浏览器与服务器时间差
            setTimeDeltaBetweenSystem: function (delta) {
                this.timeDeltaBetweenSystem = delta;
            },
            getTimeDeltaBetweenServerAndBrowser: function () {
                return this.timeDeltaBetweenSystem;
            },

            //前台最大允许发言长度
            setMaxMessageLength: function (maxMsgLength) {
                this.maxMsgLength = maxMsgLength;
            },
            getMaxMessageLength: function () {
                return this.maxMsgLength;
            },

            //最大允许 不发言时间
            setMaxNonSpeakTolerantTime: function (time) {
                this.time = time;
            },
            getNonSpeakTolerantTime: function () {
                return this.time;
            },

            //会话唯一标识
            setUuid: function (uuid) {
                this.uuid = uuid;
            },
            getUuid: function () {
                return this.uuid;
            },

            //当前会话是否是重连过
            isCurrentSessionReconnected: function () {
                return this.isReconnected;
            },
            markCurrentSessionAsReconnected: function () {
                this.isReconnected = true;
            },

            //获取视图层需要的上下文数据
            getContextData: function () {
                if (!this.context) {
                    this.context = {
                        buyerImgUrl: Customer.getPhoto(),
                        sellerImgUrl: SessionDataManager.getCurrentConsultant().getPhoto()
                    }
                }

                return this.context;
            },
            clearContextData: function () {
                this.context = null;
            },

            //获取最近一次获取到的客服id
            getCurrentConsultantID: function () {
                return this.currentConsultantID;
            },
            setCurrentConsultantID: function (id) {
                this.currentConsultantID = id;
            },
            //获取最近一次获取到的客服
            getCurrentConsultant: function () {
                return ConsultantMap.get(this.currentConsultantID);
            }
        };
        //环境配置参数
        var EnvPropertyManager = {
            //获取环境参数
            parseEnvVariable: function () {
                var url = window.location.href;

                this.setServiceGroupID(BrowserUtil.getQueryValue(url, "groupID"));
                this.setOrderSeq(BrowserUtil.getQueryValue(url, "orderSeq"));

                var fromPageType = BrowserUtil.getQueryValue(url, "fromPageType");
                if (fromPageType == null || fromPageType == '') {
                    fromPageType = -1;
                } else {
                    fromPageType = fromPageType.replace(/\D/g, '');
                    if (fromPageType == '') {
                        fromPageType = -1;
                    }
                }

                this.setFromPageType(fromPageType);
            },

            //页面来源类型
            getFromPageType: function () {
                return this.fromPageType;
            },
            setFromPageType: function (fromPageType) {
                this.fromPageType = fromPageType;
            },

            //服务组ID
            getServiceGroupID: function () {
                return this.serviceGroupID;
            },
            setServiceGroupID: function (groupID) {
                this.serviceGroupID = groupID;
            },

            //产品ID
            getOrderSeq: function () {
                return this.orderSeq || '';
            },
            setOrderSeq: function (orderSeq) {
                this.orderSeq = orderSeq;
            }
        };

        //买家数据
        var Customer = {
            setID: function (id) {
                this.id = id;
            },
            getID: function () {
                return this.id;
            },

            setUserName: function (userName) {
                this.userName = userName.replace(/^((?:13|15|18)\d)(?:\d{4})(\d{4})$/i, '$1****$2');
            },
            getUserName: function () {
                return this.userName;
            },

            setPhoto: function (photo) {
                this.photo = photo || 'images/customer-default-head-img.jpg';
            },
            getPhoto: function () {
                return this.photo;
            }
        };
        //客服类
        var Consultant = function () {
        };
        Consultant.prototype = {
            constructor: Consultant,

            setID: function (id) {
                this.id = id;
            },
            getID: function () {
                return this.id;
            },

            setUserName: function (userName) {
                this.userName = userName;
            },
            getUserName: function () {
                return this.userName;
            },

            photo: 'images/consultant-head-img.jpg',
            setPhoto: function (photo) {
                this.photo = photo;
            },
            getPhoto: function () {
                return this.photo;
            },

            setOnline: function (isOnline) {
                this.isOnline = isOnline;
            },
            isOnline: function () {
                return this.isOnline;
            }
        };
        //正在联系客服维护
        var ConsultantMap = (function () {
            var map = {};

            return {
                add: function (consultant) {
                    if (consultant instanceof Consultant) {
                        map[consultant.getID()] = consultant;
                    }
                },
                get: function (id) {
                    return map[id];
                },
                remove: function (id) {
                    !!map[id] && delete map[id];
                },
                clear: function () {
                    map = {};
                }
            }
        })();

        //地址摘要信息 本地缓存
        var HrefToSummaryMap = (function () {
            var hrefToSummaryMap = {};

            return {
                getValue: function (href) {
                    if (href != null) {
                        if (href.indexOf('?') > 0) {
                            href = href.substring(0, href.indexOf('?'));
                        }

                        return hrefToSummaryMap[href];
                    } else {
                        return null;
                    }
                },
                put: function (href, summary) {
                    if (href != null) {
                        hrefToSummaryMap[href.toString()] = summary;
                    }
                }
            };
        })();

        //拉取消息并分发
        var PullChatEventLoop = {
            //定时轮询
            doEventLoop: function () {
                if (!StatusManager.isStatusNormal()) {
                    return;
                }

                var code = arguments.callee;
                var context = this;

                remoteServiceInvokeUtil.invokeGet(
                    PageConfig.getLocalImServiceBaseUrl(),
                    {
                        event: EventType.GET_LIST,
                        sid: Customer.getID(),
                        did: SessionDataManager.getCurrentConsultantID(),
                        uuid: SessionDataManager.getUuid()
                    },
                    function successFn(data) {
                        this.handleMessageList(data.messageList);
                        this.doEventLoop();
                    },
                    function errorFn(obj) {
                        dialogControl.appendSysMessage(obj.errorMessage || '服务器发生故障');
                        StatusManager.markStatusAbnormal();
                    },
                    this
                );
            },
            //处理消息流
            handleMessageList: function (messageList) {
                var me = this;
                var data = messageList || [];
                var hasValidMessage = false;

                if ($.isArray(data)) {
                    // 如果数据是数组
                    $.each(data, function (i, v) {
                        if (StatusManager.isStatusNormal()) {
                            if (!!v.msg) {
                                var currentMessageValid = me.handleSingleMessage(v);
                                hasValidMessage = hasValidMessage || currentMessageValid;
                            }
                        } else {
                            throw {
                                'info': 'chat over'
                            };
                        }
                    });
                } else {
                    if (!!data.msg) {
                        hasValidMessage = me.handleSingleMessage(data);
                    }
                }

                if (hasValidMessage) {
                    dialogControl.notifyNewMessageArrival();
                }
            },
            //处理单条消息
            handleSingleMessage: function (message) {
                var isMessageAccept = true;

                switch (message.type) {
                    case MessageType.GENERAL_MESSAGE:
                    case MessageType.WELCOME_MESSAGE:
                        (function showMessage() {
                            if (message.msgtype == 0x969) {
                                WebimManager.exit();

                                //提示买家 客服已经将会话关闭
                                dialogControl.appendSysMessage('会话已结束。如需重新咨询，请<a class="popu_bule ui-clickable re-connect" href="#">重新连接</a>');
                                StatusManager.markStatusAbnormal();
                            } else {
                                var fromKefuInfo = ConsultantMap.get(message.sender);

                                //校验消息来源客服信息
                                if (null != fromKefuInfo) {
                                    var messageInfo = {
                                        speaker: message.sender,
                                        html: message.msg,
                                        features: message.features
                                    };
                                    if (DateUtil.isBeforeToday(message.time)) {
                                        messageInfo.time = DateUtil.formatTime(message.time);
                                    } else {
                                        messageInfo.time = DateUtil.formateOnlyTime(message.time);
                                    }

                                    if (Customer.getID() == message.sender) {
                                        messageInfo.type = 'buyer_msg';
                                        messageInfo.name = Customer.getUserName();
                                    } else {
                                        messageInfo.type = 'seller_msg';
                                        messageInfo.name = ConsultantMap.get(message.sender).getUserName();
                                    }

                                    dialogControl.appendMessage(messageInfo, SessionDataManager.getContextData());
                                }
                            }
                        })();
                        break;
                    case MessageType.IMAGE_URL:
                        (function replaceLoadingImg() {
                            var msgObj = JSON.parse(message.msg);
                            var imgName = msgObj["imgName"].replace(/^{([^.]+)}\.\w+$/, '$1');
                            var imgUrl = msgObj["imgUrl"];

                            $('.fn-image-wait-to-set[data-id="' + imgName + '"]')
                                .attr('src', imgUrl).removeClass('fn-image-wait-to-set');
                        })();
                        break;
                    case MessageType.EVALUATION:
                        (function showEvaluation() {
                            dialogControl.showEvaluationWindow();
                        })();
                        break;
                    case MessageType.ALERT_SPEAK_TIME:
                        (function alertSpeakTime() {
                            dialogControl.alertSpeak(message.msg, SessionDataManager.getNonSpeakTolerantTime());

                            //如果超过 最大容忍未发言时间，则停止定时拉取消息
                            if (message.msg == SessionDataManager.getNonSpeakTolerantTime()) {
                                StatusManager.markStatusAbnormal();
                            }
                        })();
                        break;
                    case MessageType.MONITOR_TAKE_OVER_CHAT:
                        (function displayTakeOverMessage() {
                            var msgObj = JSON.parse(message.msg) || {};

                            dialogControl.appendSysMessage('飞牛客服[' + msgObj.toSubMerchantName + '] 进入会话，希望带给您更满意的咨询服务。');

                            var monitor = new Consultant();
                            monitor.setID(msgObj.toSubMerchantID);
                            monitor.setUserName(msgObj.toSubMerchantName);

                            ConsultantMap.add(monitor);
                            //SessionDataManager.setCurrentConsultantID(monitor.getID());
                        })();
                        break;
                    case MessageType.SUB_MERCHANT_SWITCH:
                        (function displaySwitchMessage() {
                            var msgObj = JSON.parse(message.msg) || {};

                            switch (msgObj.switchStatus) {
                                case SwitchStatusType.GENERAL_SWITCH_START:
                                    break;
                                case SwitchStatusType.GENERAL_SWITCH_SUCC:
                                case SwitchStatusType.MONITOR_SWITCH_SUCC:
                                    dialogControl.appendSysMessage(
                                        '飞牛客服[' + msgObj.toSubMerchantName + '] 开始接手会话，' + '希望带给您更满意的咨询服务。');

                                    var monitor = new Consultant();
                                    monitor.setID(msgObj.toSubMerchantID);
                                    monitor.setUserName(msgObj.toSubMerchantName);

                                    ConsultantMap.add(monitor);
                                    ConsultantMap.remove(msgObj.fromSubMerchantID);

                                    SessionDataManager.setCurrentConsultantID(msgObj.toSubMerchantID);
                                    break;
                                default:
                                    break;
                            }
                        })();
                        break;
                    case MessageType.PREPARTO_CLOSE:
                        (function () {
                            dialogControl.appendSysMessage('由于您已经打开了另外一个会话窗口，本次会话已结束。');
                            StatusManager.markStatusAbnormal();
                            dialogControl.prepareToClose();
                        })();
                        break;
                    default :
                        isMessageAccept = false;
                        break;
                }

                return isMessageAccept;
            }
        };

        //粘贴图片上传、消息图片 转换格式
        var ImageSniffer = (function () {
            (function expandAjax() {
                XMLHttpRequest && XMLHttpRequest.prototype && (XMLHttpRequest.prototype.sendAsBinary = function (datastr) {
                    function byteValue(x) {
                        return x.charCodeAt(0) & 0xff;
                    }

                    var ords = Array.prototype.map.call(datastr, byteValue);
                    var ui8a = new Uint8Array(ords);
                    this.send(ui8a.buffer);
                });
            })();
            function uploadPicSync(data, callback) {
                var xhr = new XMLHttpRequest();
                xhr.open("POST", "webim/uploadImageSvlt", false);
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
            }

            return {
                convertMessageImgs: function (text) {
                    return text.replace(/<img[^>]*src="([^"]+\/([^>]+\.\w+))"[^>]*>/gi, function ($0, $1, $2) {
                        if ($1.indexOf('http://') == 0) {
                            return '<image:' + $1 + '>';
                        } else {
                            return '<image:' + $2 + '>';
                        }
                    }).replace(/<br\/?>/gi, '\r').replace(/&nbsp;?/gi, ' ').replace(/<\/?[^iI][^>]*>/gi, '')
                        .replace(/&lt;/g, '<').replace(/&gt;/g, ' >');
                },
                detectImage: function (text) {
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
                            if (url != null) {
                                text = text.replace(tag + base64_imgStr, url);
                            }
                        });

                        index++;
                        needUpload = true;
                    }

                    return {
                        needUpload: needUpload,
                        text: text
                    };
                }
            }
        })();

        //排队规则
        var QueuedCountDownLatch = (function () {
            var CountDownRule = function (waitMillis, times) {
                this.waitMillis = waitMillis;
                this.times = times;
                this.remainingTimes = times;
            };

            //排队梯度规则
            var countDownRules = [
                new CountDownRule(20, 10),
                new CountDownRule(50, 10),
                new CountDownRule(5000, -1)
            ];
            //当前排队梯度规则序号
            var currentRuleIndex = 0;

            return {
                countDown: function () {
                    var currentCountDownRule = countDownRules[currentRuleIndex];
                    var remainingTimes = currentCountDownRule.remainingTimes;

                    if (remainingTimes > 0) {
                        currentCountDownRule.remainingTimes--;
                    } else if (remainingTimes == 0) {
                        currentCountDownRule = countDownRules[++currentRuleIndex];

                        if (currentCountDownRule.remainingTimes > 0) {
                            currentCountDownRule.remainingTimes--;
                        }
                    }

                    return currentCountDownRule.waitMillis;
                },
                reset: function () {
                    currentRuleIndex = 0;

                    for (var i = 0; i < countDownRules.length; i++) {
                        countDownRules[i].remainingTimes = countDownRules[i].times;
                    }
                }
            }
        })();

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

        var WebimManager = {
            init: function () {
                EnvPropertyManager.parseEnvVariable();
                this.initUI();
                this.syncSysTime();
                this.doLogin();
            },
            initUI: function () {
                (function setEmotionUrls() {
                    $('#J_face_popup .heads img').each(function (index) {
                        var $this = $(this);
                        var currentImageInfo = ImageTitleArray[index];

                        $this.attr('src', PageConfig.getEmotionBaseUrl() + currentImageInfo['path'])
                            .attr('title', currentImageInfo['title']);
                    });
                })();
            },
            //同步系统时间
            syncSysTime: function () {
                remoteServiceInvokeUtil.invokeJsonp({
                    url: PageConfig.getLocalVenderServiceBaseUrl(),
                    data: {
                        VENDER_INFO_TYPE: EventType.SYNC_TIME
                    },
                    success: function successFn(data) {
                        if (data.success) {
                            data = data.data;
                            var currentTime = new Date().getTime();
                            SessionDataManager.setTimeDeltaBetweenSystem(data.serverTime * 1000 - currentTime);
                            SessionDataManager.setMaxMessageLength(data.maxMessageContentLength);
                        }
                    }
                });
            },
            //用户信息校验
            doLogin: function () {
                var context = this;
                var code = arguments.callee;

                remoteServiceInvokeUtil.invokeGet(
                    PageConfig.getLocalImServiceBaseUrl(),
                    {
                        event: EventType.INIT_CHAT_ENV,
                        isFromCS: 1,
                        fromPageType: EnvPropertyManager.getFromPageType(),
                        sid: 0,
                        diverse_type: 2,
                        group_id: EnvPropertyManager.getServiceGroupID(),
                        orderSeq: EnvPropertyManager.getOrderSeq()
                    },
                    function successFn(data) {
                        (function setSessionData() {
                            SessionDataManager.setUuid(data.uuid);
                            SessionDataManager.setMaxNonSpeakTolerantTime(data.maxNonSpeakTolerantTime);
                        })();

                        (function setCustomerInfo() {
                            Customer.setID(data.userID);
                            Customer.setUserName(data.nickName || data.userName);
                            Customer.setPhoto(data.imgUrl);
                        })();

                        (function initRight() {
                            $('#D_right_ifm iframe').attr('src',
                                'buyerRightPage/right.htm?nocachekey=' + new Date().getTime() + '&tokenKey=' + data.authTokenPair.key + '&tokenValue=' + data.authTokenPair.value
                                + '&orderNo=' + EnvPropertyManager.getOrderSeq() + '&goodsAskingType=' + (!EnvPropertyManager.getOrderSeq() ? 0 : 1)
                            );
                        })();

                        dialogControl.attachWindowCloseListen(context.exit, context);

                        if (data.needWait) {//如果需要排队，进入排队流程
                            if (data.queuedSeq == -1) {
                                dialogControl.alertNoConsultantAvailable();
                                alert('亲爱的顾客，您可于9:00—22:00间访问在线客服或拨打400-920-6565热线，感谢您的支持。');

                                context.exit();
                                StatusManager.markStatusAbnormal();
                                dialogControl.prepareToClose();
                            } else if (data.queuedSeq == -2) {
                                alert('您之前已经打开了一个窗口，本窗口将不再有效');
                                dialogControl.setQueueSeq(data.queuedSeq);
                                dialogControl.appendSysMessage('您之前已经打开了一个窗口，本窗口将不再有效');

                                context.exit();
                                StatusManager.markStatusAbnormal();
                                dialogControl.prepareToClose();
                            } else {
                                this.queuedCustomer(data.queuedSeq);
                            }
                        } else {//已经被分流到，进入初始化会话流程
                            this.initSession(data);
                        }
                    },
                    function errorFn(result) {
                        if (result.needLogin) {
                            PageConfig.redirectToLoginHref();
                        } else {
                            dialogControl.claimRetryConnect(code, context);
                        }
                    },
                    this
                );
            },
            //排队分流
            queuedCustomer: function (initSeq) {
                dialogControl.setQueueSeq(initSeq);

                var me = this;
                var code = arguments.callee;

                setTimeout(function () {
                    remoteServiceInvokeUtil.invokeGet(
                        PageConfig.getLocalImServiceBaseUrl(),
                        {
                            event: EventType.PULL_QUEUE_INFO,
                            sid: Customer.getID(),
                            uuid: SessionDataManager.getUuid(),
                            isFromCS: 1,
                            //pid: 0,
                            diverse_type: 2,
                            group_id: EnvPropertyManager.getServiceGroupID(),
                            orderSeq: EnvPropertyManager.getOrderSeq()
                        },
                        function successFn(data) {
                            if (data.needWait) {//如果需要排队，进入排队流程
                                switch (data.queuedSeq) {
                                    case -2:
                                        alert('您已经打开了一个新的窗口，本窗口将不再有效');
                                        dialogControl.setQueueSeq(data.queuedSeq);
                                        dialogControl.appendSysMessage('您已经打开了一个新的窗口，本窗口将不再有效');

                                        me.exit();
                                        StatusManager.markStatusAbnormal();
                                        dialogControl.prepareToClose();
                                        break;
                                    case -1:
                                        alert('亲爱的顾客，您可于9:00—22:00间访问在线客服或拨打400-920-6565热线，感谢您的支持。"');

                                        me.exit();
                                        dialogControl.alertNoConsultantAvailable();
                                        StatusManager.markStatusAbnormal();
                                        dialogControl.prepareToClose();
                                        break;
                                    default :
                                        me.queuedCustomer(data.queuedSeq);
                                        break;
                                }
                            } else {//已经被分流到，进入初始化会话流程
                                me.initSession(data);
                            }
                        },
                        function errorFn() {
                            dialogControl.claimRetryConnect(code, me);
                        },
                        me
                    );
                }, QueuedCountDownLatch.countDown());
            },
            //初始化会话
            initSession: function (sessionData) {
                //1. 设置排队号
                dialogControl.setQueueSeq(0);

                //2. 填充客服信息
                var consultantData = sessionData.subAccount;
                var consultant = new Consultant();

                consultant.setID(consultantData.id);
                consultant.setUserName(consultantData.username);
                consultant.setOnline(consultantData.online);

                StatusManager.markStatusNormal();
                ConsultantMap.add(consultant);
                SessionDataManager.setCurrentConsultantID(consultantData.id);

                //3. 视图层注册回调
                var me = this;
                //如果当前窗口是重连过的
                if (SessionDataManager.isCurrentSessionReconnected()) {
                    dialogControl.activeChatArea();
                    dialogControl.initUpload({
                        //获取会话相关信息
                        getSessionData: function () {
                            if (StatusManager.isStatusNormal()) {
                                return $.extend({
                                        uuid: SessionDataManager.getUuid(),
                                        userID: Customer.getID(),
                                        sid: Customer.getID(),
                                        did: SessionDataManager.getCurrentConsultantID()
                                    },
                                    defaultRequiredParams
                                );
                            } else {
                                throw {
                                    'info': 'Env is in abnormal status, action forbidden'
                                }
                            }
                        }
                    });
                } else {
                    var imageRegex = /<image:([^>]+)>/;

                    dialogControl.registerCallbacks({
                        //获取会话相关信息
                        getSessionData: function () {
                            if (StatusManager.isStatusNormal()) {
                                return $.extend({
                                        uuid: SessionDataManager.getUuid(),
                                        userID: Customer.getID(),
                                        sid: Customer.getID(),
                                        did: SessionDataManager.getCurrentConsultantID()
                                    },
                                    defaultRequiredParams
                                );
                            } else {
                                throw {
                                    'info': 'Env is in abnormal status, action forbidden'
                                }
                            }
                        },
                        //发送消息回调
                        onSendingMessage: function (html, features) {
                            html = html.replace(/<img[^>]*src="([^"]+\/([^>]+\.\w+))"[^>]*>/gi, function ($0, $1, $2) {
                                if ($1.indexOf('http://') == 0) {
                                    return '<image:' + $1 + '>';
                                } else {
                                    return '<image:' + $2 + '>';
                                }
                            }).replace(/<br\/?>/gi, '\r').replace(/&nbsp;?/gi, ' ').replace(/<\/?[^iI][^>]*>/gi, '')
                                .replace(/&lt;/g, '<').replace(/&gt;/g, '>');

                            var detectResult = ImageSniffer.detectImage(html);
                            if (detectResult.needUpload) {
                                html = detectResult.text;
                                html = ImageSniffer.convertMessageImgs(html);
                            }

                            me.putMessage({
                                    msg: html.replace(/<\/?[^>]+>/g, function ($0) {
                                        if (imageRegex.test($0)) {
                                            return $0;
                                        } else {
                                            return $0.replace(/</g, '&lt;').replace(/>/g, '&gt;');
                                        }
                                    }),
                                    features: JSON.stringify(features),
                                    did: SessionDataManager.getCurrentConsultantID(),
                                    type: MessageType.GENERAL_MESSAGE
                                },
                                (function appendMessage() {
                                    var message = {
                                        html: html,
                                        features: features,
                                        time: new Date().getTime(),
                                        name: Customer.getUserName(),
                                        type: 'buyer_msg'
                                    };

                                    return function (data) {
                                        message.time = data.messageTime;
                                        if (DateUtil.isBeforeToday(message.time)) {
                                            message.time = DateUtil.formatTime(message.time);
                                        } else {
                                            message.time = DateUtil.formateOnlyTime(message.time);
                                        }

                                        dialogControl.appendMessage(message, SessionDataManager.getContextData());
                                        $('.textarea').html('');
                                    }
                                })());
                        },
                        //发送评价回调
                        onSendingEvaluation: function (html, evaluation, features) {
                            me.putMessage({
                                did: SessionDataManager.getCurrentConsultantID(),
                                msg: html,
                                type: MessageType.MAKING_EVALUATION,
                                version: evaluation,
                                features: JSON.stringify(features)
                            });
                        },
                        //获取链接的摘要信息
                        onRetrievingUrlSummaryInfo: function (linkType, href, linkData, globalHrefID) {
                            var summary = HrefToSummaryMap.getValue(href);
                            //如果命中缓存
                            if (summary != null) {
                                setTimeout(function () {
                                    dialogControl.renderURLThumbInfo(linkType, summary, globalHrefID);
                                }, 20);
                            } else {
                                var data = {};
                                if (linkType === 'store') {
                                    //商城 店铺页
                                    data['VENDER_INFO_TYPE'] = 'GET_STORE_INFO';
                                    data['merchant_id'] = linkData['merchantID'];
                                } else if (linkType === 'merchant-item') {
                                    //商城 详情页
                                    data['VENDER_INFO_TYPE'] = 'GET_PRODUCT_INFO';
                                    data['product_id'] = linkData['skuID'];
                                } else if (linkType === 'feiniu-item') {
                                    //自营 详情页
                                    data['VENDER_INFO_TYPE'] = 'GET_FEINIU_ITEM_INFO';
                                    data['sm_seq'] = linkData['skuID'];
                                }

                                if (!$.isEmptyObject(data)) {
                                    remoteServiceInvokeUtil.invokeGet(
                                        PageConfig.getLocalVenderServiceBaseUrl(),
                                        data,
                                        function successFn(data) {
                                            var cachedData;
                                            if (linkType === 'store') {
                                                cachedData = data.storeInfo;
                                            } else if (linkType === 'feiniu-item') {
                                                cachedData = $.extend({}, data.feiniuItemInfo, {
                                                    url: href
                                                });
                                            } else if (linkType === 'merchant-item') {
                                                cachedData = data.productInfo;
                                            }

                                            //1. 放入本地缓存
                                            HrefToSummaryMap.put(href, cachedData);
                                            //2. 通知刷新链接内容
                                            dialogControl.renderURLThumbInfo(linkType, cachedData, globalHrefID);
                                        },
                                        function failedFn() {
                                        },
                                        this
                                    );
                                }
                            }
                        },
                        //重连逻辑
                        reconnect: function () {
                            if (!StatusManager.isStatusNormal()) {
                                dialogControl.appendSysMessage('正在为您排队...');
                                SessionDataManager.markCurrentSessionAsReconnected();
                                me.doLogin();
                            }
                        }
                    });
                }

                //4. 开始消息轮询
                PullChatEventLoop.doEventLoop();
            },
            //发消息
            putMessage: function (message, callback) {
                if (!StatusManager.isStatusNormal()) {
                    return;
                }

                if (message.msg.length > SessionDataManager.getMaxMessageLength()) {
                    alert('您发送的消息超过了最大上限：' + SessionDataManager.getMaxMessageLength() + "，请重新输入");
                    return;
                }

                var data = {};
                $.extend(data, message, {
                    event: EventType.PUT_MSG,
                    sid: Customer.getID(),
                    did: SessionDataManager.getCurrentConsultantID(),
                    sender: Customer.getID(),
                    recver: SessionDataManager.getCurrentConsultantID(),
                    time: new Date().getTime() + SessionDataManager.getTimeDeltaBetweenServerAndBrowser(),
                    uuid: SessionDataManager.getUuid()
                });
                remoteServiceInvokeUtil.invokeGet(
                    PageConfig.getLocalImServiceBaseUrl(),
                    data,
                    function successFn(data) {
                        $.isFunction(callback) && callback.call(null, data);
                    },
                    function errorFn(obj) {
                        dialogControl.appendSysMessage(obj.errorMessage || '服务器发生故障');
                        StatusManager.markStatusAbnormal();
                    },
                    this
                )
            },
            //退出会话
            exit: function () {
                remoteServiceInvokeUtil.invokeJsonp({
                    url: PageConfig.getLocalImServiceBaseUrl(),
                    data: $.extend({
                        event: EventType.EXIT,
                        sid: Customer.getID(),
                        did: SessionDataManager.getCurrentConsultantID(),
                        uuid: SessionDataManager.getUuid()
                    }, defaultRequiredParams),
                    success: function (data) {
                    }
                });
            }
        };

        return WebimManager;
    })();

    //引导代码
    (function bootstrap() {
        //校验是否登录
        remoteServiceInvokeUtil.invokeJsonp({
            url: PageConfig.getMemberPassportCheckUrl(),
            data: {},
            success: function (data) {
                if (!data || data.code != '200') {
                    PageConfig.redirectToLoginHref();
                } else {
                    $(function () {
                        WebIM.init();
                    });
                }
            }
        });
    })();
});

