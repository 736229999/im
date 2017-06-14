/**
 * Author: morningking
 * Date: 2015/10/22
 * Contact: 243717042@qq.com
 */

var generalUtil = function () {
    var globalHrefCounter = 0;
    var httpRegexp = /(https?:\/\/)([-\w_]+\.)+(com|cn|net|org|fn|edu)([\w\-\.\/\?%&#_=;,]+)?/gi;

    return {
        rgbToInt: function (rgbColor) {
            var red = 0, green = 0, blue = 0;

            var digits = /(.*?)rgb\((\d+), (\d+), (\d+)\)/i.exec(rgbColor);
            if (!!digits && digits.length == 5) {
                red = parseInt(digits[2]);
                green = parseInt(digits[3]);
                blue = parseInt(digits[4]);
            } else {
                digits = /(#)([0-9a-f]{2})([0-9a-f]{2})([0-9a-f]{2})/i.exec(rgbColor);
                if (!!digits && digits.length == 5) {
                    red = parseInt('0x' + digits[2]);
                    green = parseInt('0x' + digits[3]);
                    blue = parseInt('0x' + digits[4]);
                }
            }

            var rgb = red | (green << 8) | (blue << 16);

            return rgb;
        },
        intToRgb: function (intColor) {
            var blue = ((intColor & 0xff0000) >> 16).toString(16);
            var green = ((intColor & 0x00ff00) >> 8).toString(16);
            var red = (intColor & 0x0000ff).toString(16);

            return '#' + this.fillZero(red) + this.fillZero(green) + this.fillZero(blue);
        },
        fillZero: function (part) {
            return part.length == 1 || part < 10 ? '0' + part : '' + part;
        },
        extractURLs: function (string, callback) {
            return string.replace(httpRegexp, function ($1, $2) {
                globalHrefCounter += 1;
                var globalHrefID = "fn-extract-global-href-" + globalHrefCounter;
                var href = ($2 != null && $2 != '') ? $1 : ('http://' + $1);

                var result = '<a id="' + globalHrefID + '" class="fn-chat-extracted-link" href="' + $1 + '" target="_blank">' + $1 + '</a>';

                !!callback && $.isFunction(callback) && callback.call(null, href, globalHrefID);

                return result;
            });
        }
    };
}();

var DateUtil = (function () {
    var _defaultDateSeparator = '-';
    var _defaultTimeSeparator = ':';

    function zeroFilled(i) {
        return i < 10 ? '0' + i : i;
    }

    return {
        isBeforeToday: function (time) {
            var today = new Date();
            today.setHours(0);
            today.setMinutes(0);
            today.setSeconds(0);
            today.setMilliseconds(0);

            return today.getTime() > time;
        },
        formatDate: function (time) {
            var date = new Date();
            time && date.setTime(time);

            return date.getFullYear() + _defaultDateSeparator
                + zeroFilled(1 + date.getMonth()) + _defaultDateSeparator
                + zeroFilled(date.getDate());
        },
        formatTime: function (time) {
            var date = new Date();
            time && date.setTime(time);

            return date.getFullYear() + _defaultDateSeparator
                + zeroFilled(1 + date.getMonth()) + _defaultDateSeparator
                + zeroFilled(date.getDate()) + " "
                + zeroFilled(date.getHours()) + _defaultTimeSeparator
                + zeroFilled(date.getMinutes()) + _defaultTimeSeparator
                + zeroFilled(date.getSeconds());
        },
        formateOnlyTime: function (time) {
            var date = new Date();
            time && date.setTime(time);

            return zeroFilled(date.getHours()) + _defaultTimeSeparator
                + zeroFilled(date.getMinutes()) + _defaultTimeSeparator
                + zeroFilled(date.getSeconds());
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

            if (dateSplit.length == 7) {
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
        },
        redirectInCurrentWindow: function (href) {
            window.location.href = href;
        }
    }
})();

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
        },
        closeWebPage: function () {
            navigator.userAgent.indexOf("MSIE") > 0 ?
                navigator.userAgent.indexOf("MSIE 6.0") > 0 ?
                    (window.opener = null, window.close()) :
                    (window.open("", "_top"), window.top.close()) :
                (window.opener = null, window.open("", "_self", ""), window.close());
        },
        redirectInCurrentWindow: function (href) {
            window.location.href = href;
        }
    };
})();

var StringUtil = (function () {
    var trimPattern = /^\s+|\s+$/;

    return {
        trim: function (str) {
            return str.replace(trimPattern, '');
        }
    }
})();

var ObjectUtil = (function () {
    return {
        serializeToFormStyle: function (obj) {
            if (!obj || $.isEmptyObject(obj)) {
                return '';
            }

            var stringBuilder = '';
            for (var k in  obj) {
                if (obj.hasOwnProperty(k)) {
                    stringBuilder = stringBuilder + '&' + k + '=' + obj[k];
                }
            }

            return '?' + stringBuilder.substring(1);
        }
    }
})();

var RegexpUtil = {
    isLegalURL: function (string) {
        return /https?:\/\/([\w\-_]+\.)+(com|cn|net|org|fn|edu)(\/[\w\-\.\/\?%&#_=]+)?/.test(string);
    }
};

var LogUtil = {
    log: function (message) {
        return (function (message) {
            if (!!console && !!console.log) {
                return function (message) {
                    console.log(message);
                }
            } else {
                return function (message) {
                }
            }
        })();
    }
};

function RemoteServiceInvokeUtil(config) {
    this.config = config || {};
}

RemoteServiceInvokeUtil.prototype = {
    constructor: RemoteServiceInvokeUtil,
    defaultConfig: {
        defaultErrorMessage: {errorMessage: '服务器发生故障'},
        defaultErrorFn: function (errorMessage) {
            LogUtil.log(errorMessage);
        },
        defaultAjaxConfig: {
            async: true,
            type: 'post',
            contentType: "application/json",
            timeout: 120000,
            dataType: 'json'
        },
        defaultJsonpConfig: {
            async: false,
            dataType: 'jsonp',
            jsonp: 'callback',
            type: 'get',
            error: function (jqxhr, settings, thrownError) {
                LogUtil.log(thrownError);
            }
        }
    },
    invokePost: function (url, params, successFn, failFn, context) {
        $.ajax($.extend(this.defaultConfig.defaultAjaxConfig, this.config.ajaxConfig, {
            url: url,
            type: 'post',
            data: $.extend({}, this.config.defaultParams, params),
            success: function (resultInfo) {
                if (resultInfo.success) {
                    if (!$.isFunction(successFn)) {
                        LogUtil.log("Missed Success Fn for " + url);
                    } else {
                        successFn.call(context, resultInfo.data);
                    }
                } else {
                    $.extend(resultInfo.errorMessage, self.defaultConfig.defaultErrorMessage);

                    if ($.isFunction(failFn)) {
                        failFn.call(context, resultInfo);
                    } else {
                        self.defaultConfig.defaultErrorFn.call(null, resultInfo.errorMessage);
                    }
                }
            },
            error: function (XMLHttpRequest, textStatus, errorThrown) {
                var errorMessage = '网络发生故障，状态码：' + textStatus;

                if ($.isFunction(failFn)) {
                    failFn.call(context, {success: false, errorMessage: errorMessage});
                } else {
                    self.defaultConfig.defaultErrorFn.call(null, errorMessage);
                }
            }
        }));
    },
    invokeGet: function (url, params, successFn, failFn, context) {
        var self = this;

        $.ajax($.extend(this.defaultConfig.defaultAjaxConfig, this.config, {
            url: url,
            type: 'get',
            data: $.extend({}, this.config.defaultParams, params),
            success: function (resultInfo) {
                if (resultInfo.success) {
                    if (!$.isFunction(successFn)) {
                        LogUtil.log("Missed Success Fn for " + url);
                    } else {
                        successFn.call(context, resultInfo.data);
                    }
                } else {
                    $.extend(resultInfo.errorMessage, self.defaultConfig.defaultErrorMessage);

                    if ($.isFunction(failFn)) {
                        failFn.call(context, resultInfo);
                    } else {
                        self.defaultConfig.defaultErrorFn.call(null, resultInfo.errorMessage);
                    }
                }
            },
            error: function (XMLHttpRequest, textStatus, errorThrown) {
                var errorMessage = '网络发生故障，状态码：' + textStatus;

                if ($.isFunction(failFn)) {
                    failFn.call(context, {success: false, errorMessage: errorMessage});
                } else {
                    self.defaultConfig.defaultErrorFn.call(null, errorMessage);
                }
            }
        }));
    },
    invokeJsonp: function (options) {
        options = $.extend(this.defaultConfig.defaultJsonpConfig, this.config.defaultJsonpConfig, options);
        $.ajax(options);
    }
};

var Base64Util = {
    _keyStr: "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=",
    encode: function (e) {
        var t = "";
        var n, r, i, s, o, u, a;
        var f = 0;
        e = this._utf8_encode(e);
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
    },
    decode: function (e) {
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
        t = this._utf8_decode(t);
        return t
    },
    _utf8_encode: function (e) {
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
    },
    _utf8_decode: function (e) {
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
