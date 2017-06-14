/**
 * Author: morningking
 * Date: 2015/02/06
 * Contact: 243717042@qq.com
 */

var generalUtil = function () {
    var _today;

    return {
        _defaultDateSeparator: '-',
        _defaultTimeSeparator: ':',

        defaultDate: function () {
            return _today || (_today = this.formateDate(new Date()));
        },
        formateDate: function (date) {
            return date.getFullYear() + this._defaultDateSeparator
                + this.zeroFilled(1 + date.getMonth()) + this._defaultDateSeparator
                + this.zeroFilled(date.getDate());
        },
        formateFullTime: function (date) {
            return date.getFullYear() + this._defaultDateSeparator
                + this.zeroFilled(1 + date.getMonth()) + this._defaultDateSeparator
                + this.zeroFilled(date.getDate()) + ' '
                + this.zeroFilled(date.getHours()) + this._defaultTimeSeparator
                + this.zeroFilled(date.getMinutes()) + this._defaultTimeSeparator
                + this.zeroFilled(date.getSeconds())
        },
        formateOnlyTime: function (date) {
            return this.zeroFilled(date.getHours()) + this._defaultTimeSeparator
                + this.zeroFilled(date.getMinutes()) + this._defaultTimeSeparator
                + this.zeroFilled(date.getSeconds());
        },
        zeroFilled: function (i) {
            return i < 10 ? '0' + i : i;
        },
        parseDate: function (dateString) {
            var isoExp = /^\s*(\d{4})-(\d\d)-(\d\d)\s*$/,
                date = new Date(NaN), month,
                parts = isoExp.exec(dateString);

            if (parts) {
                month = +parts[2];
                date.setFullYear(parts[1], month - 1, parts[3]);
                if (month != date.getMonth() + 1) {
                    date.setTime(NaN);
                }
            }
            return date;
        },
        getUTCtime: function (time) {
            var dateSplit = /(\d{4})-(\d{2})-(\d{2})\s+(\d{2}):(\d{2}):(\d{2})/.exec(time);

            if (!!dateSplit && dateSplit.length == 7) {
                var date = new Date();
                date.setYear(dateSplit[1]);
                date.setMonth(dateSplit[2] - 1);
                date.setDate(dateSplit[3]);
                date.setHours(dateSplit[4]);
                date.setMinutes(dateSplit[5]);
                date.setSeconds(dateSplit[6]);

                return date.getTime();
            } else {
                return null;
            }
        }
    };
}();

var BrowserUtil = (function () {
    var userAgent = navigator.userAgent.toLowerCase();
    return {
        version: (userAgent.match(/.+(?:rv|it|ra|ie)[\/: ]([\d.]+)/) || [])[1],
        safari: /webkit/.test(userAgent),
        opera: /opera/.test(userAgent),
        msie: /msie/.test(userAgent) && !/opera/.test(userAgent),
        mozilla: /mozilla/.test(userAgent) && !/(compatible|webkit)/.test(userAgent),
        escapeReg: function (str) {
            return str.replace(new RegExp("([.*+?^=!:\x24{}()|[\\]\/\\\\])",
                "g"), '\\\x241');
        },
        getQueryValue: function (url, key) {
            var reg = new RegExp("(^|&|\\?|#)" + this.escapeReg(key)
                + "=([^&#]*)(&|\x24|#)", "");
            var match = url.match(reg);
            if (match) {
                return match[2];
            }
            return null;
        },
        openNewTab: function (href) {
            window.open(href, "_blank");
        }
    };
})();

var StringUtil = (function () {
    var trimPattern = /^\s+|\s+$/;

    return {
        trim: function (str) {
            return str.replace(trimPattern, '');
        },
        replacedWithObjectValue: function (str, obj) {
            for (var prop in obj) {
                str = str.replace(new RegExp('\\$' + prop, 'g'), obj[prop]);
            }

            return str;
        }
    };
})();

var ObjectUtil = (function () {
    return {
        serializeToFormStyle: function (obj) {
            if (!obj || $.isEmptyObject(obj)) {
                return '';
            }

            var stringBuilder = '';
            for (var k in  obj) {
                stringBuilder = stringBuilder + '&' + k + '=' + obj[k];
            }

            return '?' + stringBuilder.substring(1);
        }
    }
})();

var RegexpUtil = {
    isLegalURL: function (string) {
        return /https?:\/\/(?:(?:[\w\-_]+\.)+(com|cn|net|org|fn|edu)|(?:\d{1,3}\.?){4})(\/[\w\-\.\/\?%&#_=]+)?/.test(string);
    }
};

var LogUtil = (function () {
    return {
        log: function (message) {
            if (!!console && !!console.log) {
                console.log(message);
            }
        }
    }
})();

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

var RemoteServiceInvokeUtil = (function () {
    var Config = {
        defaultErrorMessage: {errorMessage: '服务器发生故障'},
        defaultErrorFn: function (errorMessage) {
            LogUtil.info(errorMessage);
        }
    };

    return {

        invokePost: function (url, params, successFn, failFn, context) {
            $.post(
                url,
                $.extend({}, params),
                function (resultInfo) {
                    if (resultInfo.success) {
                        if (!$.isFunction(successFn)) {
                            LogUtil.log("Missed Success Fn for " + url);
                        } else {
                            successFn.call(context, resultInfo.data);
                        }
                    } else {
                        resultInfo.errorMessage = resultInfo.errorMessage || Config.defaultErrorMessage.errorMessage;

                        if ($.isFunction(failFn)) {
                            failFn.call(context, resultInfo);
                        } else {
                            Config.defaultErrorFn.call(null, resultInfo.errorMessage);
                        }
                    }
                }
            )
        }
    }
})();

var transformResultToDatagridData = (function (pager, _emotionPath) {
    var rowDataMap = {};
    var dateArray = [];
    var historyMessageList = pager.resultsForCurrentPage;

    if (historyMessageList == null) {
        var rowData = {};
        rowData.total = 0;
        rowData.rows = [];

        return rowData;
    }

    for (var i = 0; i < historyMessageList.length; i++) {
        var currentMessage = historyMessageList[i];
        var date = new Date();
        date.setTime(currentMessage.sendTime * 1000);
        var day = generalUtil.formateDate(date);

        if (-1 == $.inArray(day, dateArray)) {
            dateArray.push(day);
            rowDataMap[day] = [];
        }

        var dayMessageList = rowDataMap[day];

        var template = '';
        var name = '';
        var time = generalUtil.formateOnlyTime(date);

        name = currentMessage.fromUser.userName;
        template = TemplateManager.customerHeader;

        var messageHeader = template.replace(/\$name/, name).replace(/\$time/, time);
        var messageBody = currentMessage.msg.replace(/<image:([^>]+)>/g, function () {
            var $1 = arguments[1] || '';
            if ($1.indexOf('http://') != -1) {
                return '<a class="fn-fancy-item" href="' + $1 + '"><img src="' + $1 + '" alt="聊天图片"/></a>';
            } else {
                if ($1.length < 12)
                    return '<a class="fn-fancy-item" href="' + _emotionPath + 'static/images/feiniu-emotion/' + $1 +
                        '"><img src="' + _emotionPath + 'static/images/feiniu-emotion/' + $1 + '" alt="聊天图片"/></a>';
                else {
                    var url = feiniu.image.getUrl($1);
                    return '<a class="fn-fancy-item" href="' + url +
                        '"><img src="' + url + '" alt="聊天图片"/></a>';
                }
            }

        }).replace(/\r/g, '<br/>');

        dayMessageList.unshift(TemplateManager.chatHeader.replace(/\$headerContent/, messageHeader) +
            TemplateManager.chatMessageBody.replace(/\$bodyContent/, messageBody));
    }

    var rowData = {};
    rowData.total = pager.totalRecordSize;
    rowData.rows = [];

    for (var i = 0; i < dateArray.length; i++) {
        rowData.rows.push({
            date: dateArray[i],
            messageList: rowDataMap[dateArray[i]]
        });
    }

    return rowData;
});

var GlobalDeleteScheduler = function () {
};
GlobalDeleteScheduler.prototype = {
    constructor: GlobalDeleteScheduler,
    setContext: function (context) {
        this.context = context;
        return this;
    },
    setFunction: function (func) {
        this.code = func;
        return this;
    },
    run: function () {
        if (!!this.context && !!this.code) {
            this.code.call(this.context);
        }
    },
    cancel: function () {
        this.context = null;
        this.code = null;
    }
};
