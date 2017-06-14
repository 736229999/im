/**
 * Author: morningking
 * Date: 2015/11/23
 * Contact: 243717042@qq.com
 */
//页面配置项
var PageConfig = (function () {
    var loginHref = 'https://passport.feiniu.com/gateway/login';
    var memberOrderHref = 'https://member.feiniu.com/order/orderList';
    var uploggerUrl = 'http://static01.fn-mart.com/product/js/lib/upLogger.js';
    var currentHref = window.location.href;
    var currentDomain = currentHref.replace(new RegExp('^https?://([^/]+)/.+$'), '$1');
    window.member_url_view = 'https://passport.feiniu.com';

    if (currentDomain.indexOf('beta1.fn') > -1)/**beta环境**/{
        loginHref = 'https://passport.beta1.fn/gateway/login';
        memberOrderHref = 'https://member.beta1.fn/sh/order/orderList';
        window.member_url_view = 'https://passport.beta1.fn';
        uploggerUrl = 'http://static.beta1.fn/product/js/lib/upLogger.js?' + new Date().getTime();
    } else if (currentDomain.indexOf('dev1.fn') > -1)/**dev环境**/{
        loginHref = 'https://passport.dev1.fn/gateway/login';
        memberOrderHref = 'https://member.dev1.fn/sh/order/orderList';
        window.member_url_view = 'https://passport.dev1.fn';
    }

    var memberDomain = document.domain;
    memberDomain = memberDomain.substring(memberDomain.indexOf('.') + 1);
    document.domain = memberDomain;

    return {
        //我的订单 页面地址
        getMemberOrderUrl: function () {
            return memberOrderHref;
        },
        //会员中心 登录地址
        getMemberLoginUrl: function () {
            return loginHref;
        },
        //埋点 js路径
        getUploggerUrl: function () {
            return uploggerUrl;
        }
    };
})();

seajs.use([
    '$',
    PageConfig.getUploggerUrl()
], function (jq) {
    var HomePageManager = (function () {
        var remoteServiceInvokeUtil = new RemoteServiceInvokeUtil();

        return {
            _currentGroupID: null,
            init: function () {
                this.initData();
                this.bindUI();
            },
            initData: function () {
                $('#memberOrderPage').attr('href', PageConfig.getMemberOrderUrl());
            },
            bindUI: function () {
                var self = this;

                $(document).on('click', '.case .bule_btn', function (e) {
                    var $this = $(this);
                    var isBindGroup = $this.nextAll('.edit-field[name="isBindGroup"]').html() == '1';

                    if (isBindGroup) {
                        self._currentGroupID = $this.nextAll('.edit-field[name="groupID"]').html();
                    } else {
                        self._currentGroupID = null;
                    }

                    if (!$this.is('.btn-bind-order:visible')) {
                        //tracking 埋点
                        var e = e || window.event;
                        var x = e.clientX + (document.documentElement.scrollLeft || document.body.scrollLeft),
                            y = e.clientY + (document.documentElement.scrollTop || document.body.scrollTop);
                        var toHref = 'index.jsp?fromPageType=0&needTrack=1';
                        self._currentGroupID != null && (toHref += '&groupID=' + self._currentGroupID);

                        upLogger.sparam.trackType = '2';
                        upLogger.sparam.trackTagName = '4026';
                        upLogger.sparam.funType = '4';
                        upLogger.sparam.xyPos = x + '|' + y;
                        upLogger.sparam.pageContentId = $this.closest('.case').find('.title').text();
                        upLogger.sparam.forwardUrl = window.location.href + toHref;
                        upLogger.sparam.city = upUtil.getCookie('C_dist').split("_")[1];
                        var urlStr = upUtil.getParam(upLogger.sparam);
                        window.name = window.name + '|%' + urlStr;

                        BrowserUtil.redirectInCurrentWindow(toHref);
                    } else {
                        upLogger.acceptEventParams($this, '', '2', '4026', '4', e, $this.closest('.case').find('.title').text());

                        remoteServiceInvokeUtil.invokeGet(
                            'get-user-order.do?' + (new Date().getTime()),
                            {},
                            function successFn(data) {
                                new Popup({
                                    "title": '<div class="popu-head"><p class="popu-head-p">请选择订单信息</p></div>',
                                    "content": $('#p_im'),
                                    "sign": 1
                                });

                                $('.Prompt_red').hide();
                                var orderTable = $('.ui_pop .Order table tbody');
                                orderTable.children('.loading').addClass('hide');
                                if (data.userOrderList.length == 0) {
                                    orderTable.children('.no-result').removeClass('hide');
                                } else {
                                    var template = '<tr data-seq="$seq"><td>订单编号<br/>$seq</td>' +
                                        ' <td colspan="2" style="text-align: center"><img src="$picURL" width="60" height="60"/></td> ' +
                                        '<td>$tradeTime</td> <td>$status</td> <td><a class="bule_btn_1 consult-with-order-no" href="#">开始咨询</a></td></tr>';
                                    var stringBuilder = '';
                                    for (var i = 0; i < data.userOrderList.length; i++) {
                                        var userOrder = data.userOrderList[i];
                                        userOrder.tradeTime = userOrder.tradeTime.replace(/\s+/g, '<br>');

                                        stringBuilder += StringUtil.replaceAllRef(template, userOrder);
                                    }

                                    orderTable.append(stringBuilder);
                                }
                            },
                            function failedFn(resultInfo) {
                                if (resultInfo.needLogin) {
                                    fnDialog({});
                                } else {
                                    alert('获取最近订单失败，服务器出现故障');
                                }
                            },
                            self
                        );
                    }

                    e.preventDefault();
                    return false;
                });

                //手动输入订单，点击咨询
                $(document).on('click', '.consult-by-input-orderseq', function (e) {
                    var value = $(this).prev('input[name="orderSeq"]').val();

                    if (!value) {
                        var toHref = 'index.jsp?1=1&fromPageType=0';
                        self._currentGroupID != null && (toHref += '&groupID=' + self._currentGroupID);

                        BrowserUtil.redirectInCurrentWindow(toHref);
                    } else {
                        remoteServiceInvokeUtil.invokeGet(
                            'check-order-seq.do?' + (new Date().getTime()),
                            {
                                orderSeq: value
                            },
                            function successFn(data) {
                                if (data.isLegal) {
                                    $('.Prompt_red').fadeOut();

                                    var toHref = 'index.jsp?fromPageType=0&orderSeq=' + value;
                                    self._currentGroupID != null && (toHref += '&groupID=' + self._currentGroupID);

                                    BrowserUtil.redirectInCurrentWindow(toHref);
                                } else {
                                    $('.Prompt_red').fadeIn();
                                }
                            },
                            function errorFn(resultInfo) {
                                if (resultInfo.needLogin) {
                                    fnDialog({});
                                } else {
                                    alert(resultInfo.errorMessage || '服务器发生故障');
                                }
                            },
                            self
                        );
                    }

                    e.preventDefault();
                    return false;
                });

                //最近一个月订单每个订单，点击咨询
                $(document).on('click', '.ui_pop .Order table tbody .consult-with-order-no', function (e) {
                    var $this = $(this);
                    var closestTr = $this.closest('tr');
                    var toHref = 'index.jsp?fromPageType=0&orderSeq=' + closestTr.data('seq');
                    self._currentGroupID != null && (toHref += '&groupID=' + self._currentGroupID);

                    BrowserUtil.redirectInCurrentWindow(toHref);

                    e.preventDefault();
                    return false;
                });

                //没最近订单时，点击最近咨询
                $(document).on('click', '.no-result .continue-contact', function (e) {
                    var toHref = 'index.jsp?fromPageType=0';
                    self._currentGroupID != null && (toHref += '&groupID=' + self._currentGroupID);

                    BrowserUtil.redirectInCurrentWindow(toHref);

                    e.preventDefault();
                    return false;
                });
            }
        };
    })();

    $(function () {
        (function bootstrap() {
            $(function () {
                HomePageManager.init();
            });
        })();
    });

});