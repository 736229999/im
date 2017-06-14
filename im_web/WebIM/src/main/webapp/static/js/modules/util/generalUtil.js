define(function (require, exports, module) {
    require('$');

    var NumberRegexp = /^\d+$/;
    var httpRegexp = /(https?:\/\/)([-\w_]+\.)+(com|cn|net|org|fn|edu)([\w\-\.\/\?%&#_=;,]+)?/gi;
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

    var test_canvas = document.createElement("canvas") //try and create sample canvas element
    var canvascheck = (test_canvas.getContext) ? true : false;
    var isHTML5Supported = canvascheck;

    var globalHrefCounter = 0;

    module.exports = {
        isNumber: function (number) {
            return number != null && NumberRegexp.test(number);
        },
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
        formatDate: function (time) {
            var date = new Date();
            time && date.setTime(time);

            return this.fillZero(date.getHours()) + ":" +
                this.fillZero(date.getMinutes()) + ":" +
                this.fillZero(date.getSeconds());
        },
        /**
         * 接收时间的数字表示，并以time的方式来返回
         * @param time long
         */
        formatTime: function (time) {
            var date = new Date();
            time && date.setTime(time);

            return date.getFullYear() + "-" +
                this.fillZero(date.getMonth() + 1) + "-" +
                this.fillZero(date.getDate()) + " " +
                this.fillZero(date.getHours()) + ":" +
                this.fillZero(date.getMinutes()) + ":" +
                this.fillZero(date.getSeconds());
        },
        formatOnlyTime: function (time) {
            var date = new Date();
            time && date.setTime(time);

            return this.fillZero(date.getHours()) + ":" +
                this.fillZero(date.getMinutes()) + ":" +
                this.fillZero(date.getSeconds());
        },
        isBeforeToday: function (time) {
            var today = new Date();
            today.setHours(0);
            today.setMinutes(0);
            today.setSeconds(0);
            today.setMilliseconds(0);

            return today.getTime() > time;
        },
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
        fillZero: function (part) {
            return part.length == 1 || part < 10 ? '0' + part : '' + part;
        },
        metaPath: function () {
            var href = window.location.href;
            var firstOccurence = href.indexOf('/', 7) + 1;
            var servicePath = href.substr(0, href.indexOf('/', firstOccurence));

            this._metaPath = servicePath;
            return servicePath;
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
        },
        openNewTab: function (href) {
            window.open(href, "_blank");
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
        closeWebPage: function () {
            navigator.userAgent.indexOf("MSIE") > 0 ?
                navigator.userAgent.indexOf("MSIE 6.0") > 0 ?
                    (window.opener = null, window.close()) :
                    (window.open("", "_top"), window.top.close()) :
                (window.opener = null, window.open("", "_self", ""), window.close());
        },
        redirectInCurrentWindow: function (href) {
            window.location.href = href;
        },
        encodeUsingBase64: function (str) {
            return Base64.encode(str);
        },
        isHTML5Supported: function () {
            return isHTML5Supported;
        },
        trim: function (string) {
            return string.replace(/^\s+/, '').replace(/\s+$/g, '');
        }
    }
});
