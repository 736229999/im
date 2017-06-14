define(function (require, exports, module) {
    require('$');
    window.feiniu || (window.feiniu = {});

    var authTokenKey = 'AUTH_TOKEN';

    // 配置
    feiniu.config || (feiniu.config = {});
    if (/^https?\:\/\/[-\w]+\.feiniu\.com/.test(location.href)) { // Online环境
        feiniu.config.cookieDomain = '.feiniu.com';
        feiniu.config.memberHost = 'http://member.feiniu.com';
        feiniu.config.merchantHost = 'http://merchant.feiniu.com';
        feiniu.config.zhaoshangHost = 'http://zhaoshang.feiniu.com';
        feiniu.config.subaccountHost = 'http://subaccount.feiniu.com';
        feiniu.config.backendHost = 'http://backend.feiniu.com';
        feiniu.config.itemHost = 'item-api.beta.fn.com';
        feiniu.config.smartUrlStaticHost = 'http://10.211.64.68:8080/imgweb/image/'; //缩略图URL前缀
        feiniu.config.storeHost = 'http://storeservice.beta.fn.com';
        feiniu.config.detailGoodsHost = 'http://detail.beta.fn.com/item/';
    } else if (/^https?\:\/\/[-\w]+.beta\.fn\.com/.test(location.href)) { // Beta环境
        feiniu.config.cookieDomain = '.fn.com';
        feiniu.config.memberHost = 'http://member.beta.fn.com';
        feiniu.config.merchantHost = 'http://merchant-api.beta.fn.com';
        feiniu.config.zhaoshangHost = 'http://zhaoshang.beta.fn.com';
        feiniu.config.subaccountHost = 'http://subaccount.beta.fn.com';
        feiniu.config.backendHost = 'http://backend.beta.fn.com';
        feiniu.config.itemHost = 'http://item.beta.fn.com';
        feiniu.config.smartUrlStaticHost = 'http://10.211.64.68:8080/imgweb/image/'; //缩略图URL前缀
        feiniu.config.storeHost = 'http://storeservice.beta.fn.com';
        feiniu.config.detailGoodsHost = 'http://detail.beta.fn.com/item/';
    } else if (/^https?\:\/\/wh-(?:qa\d+|test\d+)\.fn\.com/.test(location.href)) { // 公共开发测试环境
        feiniu.config.cookieDomain = '.fn.com';
        feiniu.config.memberHost = 'http://wh-qa01.fn.com:8080/FeiniuMemberWeb';
        feiniu.config.merchantHost = 'http://wh-qa01.fn.com:8080/FeiniuMerchantApiWeb';
        feiniu.config.zhaoshangHost = 'http://wh-qa01.fn.com:8080/FeiniuZhaoshangWeb';
        feiniu.config.subaccountHost = 'http://wh-qa01.fn.com:8080/FeiniuSubAccountWeb';
        feiniu.config.backendHost = 'http://wh-test001.fn.com:8080/feiniuBackend';
        feiniu.config.itemHost = 'http://wh-qa01.fn.com:8080/FeiniuItemWeb';
        feiniu.config.smartUrlStaticHost = 'http://10.211.64.68:8080/imgweb/image/'; //缩略图URL前缀
        feiniu.config.storeHost = 'http://storeservice.beta.fn.com';
        feiniu.config.detailGoodsHost = 'http://wh-item.fn.com/item/';
    } else { // 本地开发测试环境
        feiniu.config.cookieDomain = '.fn.com';
        feiniu.config.memberHost = 'http://wh-qa01.fn.com:8080/FeiniuMemberWeb';
        feiniu.config.merchantHost = 'http://wh-qa01.fn.com:8080/FeiniuMerchantApiWeb';
        feiniu.config.zhaoshangHost = 'http://local.fn.com:8080/FeiniuZhaoshangWeb';
        feiniu.config.subaccountHost = 'http://local.fn.com:8080/FeiniuSubAccountWeb';
        feiniu.config.backendHost = 'http://local.fn.com:8080/feiniuBackend';
        feiniu.config.itemHost = 'http://item-api.beta.fn.com/goodsinfo_api/get_moumouattribute_by_skuid/';
        feiniu.config.smartUrlStaticHost = 'http://10.211.64.68:8080/imgweb/image/'; //缩略图URL前缀
        feiniu.config.storeHost = 'http://storeservice.beta.fn.com';
        feiniu.config.detailGoodsHost = 'http://wh-item.fn.com/item/';
    }

    feiniu.config.memberStaticHost = feiniu.config.memberHost + '/static';
    feiniu.config.merchantStaticHost = feiniu.config.merchantHost + '/static';
    feiniu.config.zhaoshangStaticHost = feiniu.config.zhaoshangHost + '/static';
    feiniu.config.subaccountStaticHost = feiniu.config.subaccountHost + '/static';
    feiniu.config.itemStaticHost = feiniu.config.itemHost + '/static';

    // 错误代码
    feiniu.errorCode || (feiniu.errorCode = {});

    feiniu.errorCode.authenticationError = '100001'; // 身份认证错误

    // 方法
    feiniu.fn || (feiniu.fn = {});

    // 封装jQuery.ajax方法，对常见错误进行统一处理
    feiniu.fn.ajax = function () {
        if (arguments.length > 1) {
            var settings = arguments[1];
            if (settings.complete != null) {
                settings.complete = wrapAjaxCallback(settings.complete);
            }
            if (settings.success != null) {
                settings.success = wrapAjaxCallback(settings.success);
            }
        }
        $.ajax.apply($, arguments);
    };

    // 封装jQuery.get方法，对常见错误进行统一处理
    feiniu.fn.get = function () {
        if (arguments.length > 1) {
            var callbackArgIndex = 1;
            var callback = arguments[callbackArgIndex];
            if (typeof (callback) != 'function') {
                callback = arguments[callbackArgIndex = 2];
            }
            arguments[callbackArgIndex] = wrapAjaxCallback(callback);
        }
        $.get.apply($, arguments);
    };

    // 封装jQuery.post方法，对常见错误进行统一处理
    feiniu.fn.post = function () {
        if (arguments.length > 1) {
            var callbackArgIndex = 1;
            var callback = arguments[callbackArgIndex];
            if (typeof (callback) != 'function') {
                callback = arguments[callbackArgIndex = 2];
            }
            arguments[callbackArgIndex] = wrapAjaxCallback(callback);
        }
        $.post.apply($, arguments);
    };

    // 调用后台服务
    feiniu.fn.callService = function (options) {
        options = $.extend({
            version: '1.0',
            async: true
        }, options);

        feiniu.fn.ajax(options.host + '/service/call.do?callback=?', {
            type: "post",
            data: {
                version: options.version,
                method: options.method,
                params: options.params,
                vericode: options.vericode,
                authToken: feiniu.fn.getAuthToken()
            },
            success: function () {
                if (typeof options.callback == 'function') {
                    options.callback.apply(options.callback, arguments);
                }
            },
            dataType: 'jsonp',
            async: options.async
        });
    };

    // 保存身份Token
    feiniu.fn.setAuthToken = function (authToken, isKeep) {
        var options = {};
        if (!!isKeep) {
            options.expires = new Date(9999999999999);
        }
        feiniu.fn.setCookie(authTokenKey, authToken, options);
    };
    // 获取身份Token
    feiniu.fn.getAuthToken = function () {
        return feiniu.fn.getCookie(authTokenKey);
    };
    // 验证是否已登录
    feiniu.fn.isLogin = function () {
        return !!feiniu.fn.getCookie(authTokenKey);
    };

    // 获取当前登录信息
    feiniu.fn.getCurrentAuth = function (callback) {
        feiniu.fn.callService({
            host: feiniu.config.memberHost,
            method: 'feiniu.member.currentAuth',
            callback: function (data) {
                if (!data.error) {
                    callback.call(callback, JSON.parse(data.result));
                }
            }
        });
    };

    // 跳转到登录页
    feiniu.fn.gotoLogin = function (backUrl) {
        if (!backUrl) {
            backUrl = top.location.href;
        }
        top.location.href = feiniu.config.memberStaticHost + '/html/login.html?backUrl='
        + encodeURIComponent(backUrl);
    };

    // 退出登录
    feiniu.fn.logout = function (param) {
        feiniu.fn.deleteCookie(authTokenKey);
        if (typeof param == 'function') {
            param();
        } else if (typeof param == 'string') {
            location.href = param;
        } else {
            alert('退出登录成功!');
        }
    };

    // 强制用户登录，如未登录将跳转到登录页
    feiniu.fn.requireLogin = function () {
        var authToken = feiniu.fn.getCookie(authTokenKey);
        if (!authToken) {
            feiniu.fn.gotoLogin();
        }
    };

    //将str中的html符号转义,将转义“'，&，<，"，>”五个字符
    feiniu.fn.escapeHtml = function (str, reg) {
        return str ? str.replace(reg || /[&<">'](?:(amp|lt|quot|gt|#39|nbsp);)?/g, function (a, b) {
            if (b) {
                return a;
            } else {
                return {
                    '<': '&lt;',
                    '&': '&amp;',
                    '"': '&quot;',
                    '>': '&gt;',
                    "'": '&#39;'
                }[a]
            }
        }) : '';
    };

    // 渲染模板
    feiniu.fn.renderTemplate = function (template, data) {
        return template.replace(/\{([\w\.]*)\}/g, function (str, key) {
            var keys = key.split('.'), v = data[keys.shift()];
            for (var i = 0, l = keys.length; i < l; i++) v = v[keys[i]];
            v = (typeof v !== "undefined" && v !== null) ? v : '';
            if (typeof v == 'string') {
                v = feiniu.fn.escapeHtml(v);
            }
            return v;
        });
    };

    // 输出分页
    feiniu.fn.pagination = function (container, options) {
        options = $.extend({
            curPage: 1,
            pageRows: 10,
            showNum: 10,
            callback: function () {
            }
        }, options);
        options.curPage = parseInt(options.curPage);
        options.pageRows = parseInt(options.pageRows);

        container = $(container);

        var rightNum = Math.ceil(options.showNum / 2) - 1;
        var leftNum = options.showNum - rightNum - 1;
        var beginPage = options.curPage - leftNum;
        var endPage = options.curPage + rightNum;
        if (beginPage < 1) {
            endPage = endPage - beginPage + 1;
            if (endPage > options.pageAmount) {
                endPage = options.pageAmount;
            }
            beginPage = 1;
        }
        if (endPage > options.pageAmount) {
            beginPage = beginPage - (endPage - options.pageAmount);
            if (beginPage < 1) {
                beginPage = 1;
            }
            endPage = options.pageAmount;
        }

        var bodyTemplate =
            '<div class="cont clearfix">\
                <div class="info-1 fl"> <span>共<em class="on">{totalRows}</em>条记录</span><span>共<em class="on">{pageAmount}</em>页</span> </div>\
                    <div class="right_part"><ul class="pages fl"></ul>\
                        <div class="info-2 fl"> 每页\
                            <div class="btn-group w48">\
                                <button class="btn btn-large dropdown-toggle" data-toggle="dropdown">{pageRows}<span class="caret"></span></button>\
                                <ul class="dropdown-menu">\
                                    <li><a href="javascript:void(0)">10</a></li>\
                                    <li><a href="javascript:void(0)">50</a></li>\
                                    <li><a href="javascript:void(0)">100</a></li>\
                                </ul>\
                            </div>条\
                        </div>\
                        <div class="skip fl">\
                            跳转到<input type="text" class="w40 txt page-go">页\
                        </div>\
                        <a href="##" class="button fl makesure">确认</a> </div>\
                    </div>\
              </div>\
            </div>';

        var bodyHtml = feiniu.fn.renderTemplate(bodyTemplate, options);
        container.html(bodyHtml);

        var pageListContainer = container.find('.pages:first');
        var gotoPageInput = container.find('.page-go');
        var gotoPageConfirm = container.find('.makesure');

        //第页xx条的下拉选择
        $('.tab-ft-wrap .dropdown-menu li').bind('click', function () {
            var st = $(this).text();
            options.callback.call(options.callback, 1, st);
        });


        // 添加首页
        pageListContainer.append(' <li class="sy">&lt;首页</li>');
        if (options.curPage > 1) {
            var page = pageListContainer.find('li:last');
            page.click(function () {
                options.callback.call(options.callback, 1, options.pageRows);
            });
        }

        // 添加上一页
        pageListContainer.append('<li class="pre">&lt;上一页</li>');
        if (options.curPage > 1) {
            var page = pageListContainer.find('li:last');
            page.click(function () {
                options.callback.call(options.callback, options.curPage - 1, options.pageRows);
            });
        }

        // 添加数字页
        for (var i = beginPage; i <= endPage; i++) {
            pageListContainer.append('<li class="nums"><a href="javascript:void(0)">' + i + '</a></li>');
            var page = pageListContainer.find('a:last');
            page.attr('num', i);
            if (options.curPage == i) {
                page.addClass('on');
            } else {
                page.click(function () {
                    options.callback.call(options.callback, $(this).attr('num'), options.pageRows);
                });
            }
        }

        // 添加下一页
        pageListContainer.append('<li class="next">下一页&gt;</li>');
        if (options.curPage < options.pageAmount) {
            var page = pageListContainer.find('li:last');
            page.click(function () {
                options.callback.call(options.callback, options.curPage + 1, options.pageRows);
            });
        }

        // 添加尾页
        pageListContainer.append('<li class="wy">尾页&gt;</li>');
        if (options.curPage < options.pageAmount) {
            var page = pageListContainer.find('li:last');
            page.click(function () {
                options.callback.call(options.callback, options.pageAmount, options.pageRows);
            });
        }

        // 注册跳转至某页面按钮事件
        gotoPageConfirm.click(function () {
            if (!(/^[1-9]+[0-9]*$/.test(gotoPageInput.val()))) {
                alert('请输入正整数');
                return;
            }
            ;
            options.callback.call(options.callback, gotoPageInput.val(), options.pageRows);
        });
    };

    // 获取请求参数
    feiniu.fn.getParameter = function (name) {
        var value = null;
        var uriMatcher = location.href.match(/(?:\?|#!~)(.*)/);
        if (uriMatcher != null) {
            var uri = uriMatcher[1];
            if (!name) {
                value = uri;
            } else {
                var regExp = new RegExp('(?:^|&)' + name + '=([^&]*)');
                value = uri.match(regExp) ? decodeURIComponent(uri.match(regExp)[1]) : null;
            }
        }
        return value;
    };

    //写cookie
    feiniu.fn.setCookie = function (key, value, options) {
        options = $.extend({
            domain: feiniu.config.cookieDomain,
            path: '/'
        }, options);

        var cookie = '';

        if (key != null && value != null) {
            cookie += key + '=' + value + ';';
        }

        if (options.expires != null) {
            cookie += 'expires=' + options.expires.toGMTString() + ';';
        }

        if (options.domain != null) {
            cookie += 'domain=' + options.domain + ';';
        }

        if (options.path != null) {
            cookie += 'path=' + options.path + ';';
        }

        document.cookie = cookie;
    };

    //获取cookie
    feiniu.fn.getCookie = function (key) {
        var cookie = document.cookie;
        var cookieArray = cookie.split(';');
        var value = '';
        for (var i = 0; i < cookieArray.length; i++) {
            if ($.trim(cookieArray[i]).substr(0, key.length) == key) {
                value = $.trim(cookieArray[i]).substr(key.length + 1);
                break;
            }
        }
        return value;
    };

    // 删除cookie
    feiniu.fn.deleteCookie = function (key) {
        var expires = new Date();
        expires.setTime(expires.getTime() - 1);
        feiniu.fn.setCookie(key, '', {
            expires: expires
        });
    };

    // 封装jQuery Ajax方法参数中的callback参数
    function wrapAjaxCallback(callback) {
        return function (data) {
            var error = null;
            if (!!data.responseText) {
                error = JSON.parse(data.responseText).error;
            } else {
                error = data.error;
            }
            if (!!error) {
                if (error.code == feiniu.errorCode.authenticationError) { // 未登录时
                    feiniu.fn.gotoLogin(); // 跳转至登录页
                    return;
                }
            }
            callback.apply(callback, arguments);
        };
    }

    module.exports = feiniu;
});
