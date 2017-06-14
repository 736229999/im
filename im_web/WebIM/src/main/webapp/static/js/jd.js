/*!  2014-12-11 15:57:24 */

function ChatBllOrder() {
    this.debug = !1, this.orderList = [], this.repairsList = [], this.orderIndex = 0, this.repairsIndex = 0, this.showConsultOrder = !0
}
function Highlighter(options) {
    this.element = null, this.colors = options && options.color ? options.color : "red", this.addEvent = options && "false" == options.evt ? options.evt : "true", this.global = options && options.g ? options.g : "", this.show = !1
}
function buildPopTipLayer() {
    $(".im-icon-file").hide(), $(".im-icon-pic").hide();
    var html = "<div class='pop_tip_mask'></div><div class='pop_tip_main'><div style='margin-top: 30px;'><span class='pop_tip_text'>安装完成后，请</span><span style='color: #e64346'>重启浏览器</span><span class='pop_tip_text'>就可以使用</span><br/><span class='pop_tip_text' style='margin-top:10px;'>京东咚咚咨询啦</span></div><div style='margin-top: 18px;'><a href='#' class='know_btn' onclick='closeTipLayer();'>我知道了</a></div></div>";
    $("body").append(html), $(document).height(), document.body.scrollHeight
}
function buildPopTipLayerV3() {
    $(".im-icon-file").hide(), $(".im-icon-pic").hide();
    var html = "<div class='pop_tip_mask'></div><div class='pop_tip_main_v3'><div style='margin-top: 170px;'><a href='#' class='know_btn_v3' onclick='closeTipLayerV3();'></a></div></div>";
    $("body").append(html), $(document).height(), document.body.scrollHeight
}
function buildPopNotSupportTipLayer() {
    $(".im-icon-file").hide(), $(".im-icon-pic").hide();
    var html = "<div class='pop_tip_mask'></div><div class='pop_tip_main_not_support'><div style='margin-top: 30px;'><span class='pop_tip_text'>抱歉，暂时不支持Windows以外的系统</span></div><div style='margin-top: 18px;'><a href='#' class='know_btn' onclick='closeNotSupportTipLayer();'>我知道了</a></div></div>";
    $("body").append(html), $(document).height(), document.body.scrollHeight
}
function buildPopTipEvent() {
    $(".pop_tip_main .know_btn").unbind("click").bind("click", closeTipLayer), $(".pop_tip_main_not_support .know_btn").unbind("click").bind("click", closeNotSupportTipLayer)
}
function buildPopTipEventV3() {
    $(".pop_tip_main_v3 .know_btn_v3").unbind("click").bind("click", closeTipLayerV3)
}
function closeTipLayer() {
    return $(".pop_tip_mask").remove(), $(".pop_tip_main").remove(), $(".im-icon-file").show(), $(".im-icon-pic").show(), clickRecord("support_know"), !1
}
function closeNotSupportTipLayer() {
    return $(".pop_tip_mask").remove(), $(".pop_tip_main_not_support").remove(), $(".im-icon-pic").show(), clickRecord("not_support_know"), !1
}
function closeTipLayerV3() {
    return clickRecord("support_know"), setTimeout(closeWebPage, 1e3), !1
}
function closeWebPage() {
    navigator.userAgent.indexOf("MSIE") > 0 ? navigator.userAgent.indexOf("MSIE 6.0") > 0 ? (window.opener = null, window.close()) : (window.open("", "_top"), window.top.close()) : navigator.userAgent.indexOf("Firefox") > 0 ? window.location.href = "about:blank " : (window.opener = null, window.open("", "_self", ""), window.close())
}
function startMaskAnimate() {
    var timer;
    return $(".girl").animate({
        top: "50%"
    }, "normal", function () {
        $(".dd_tip_main_v3").fadeIn(1e3), $(".dd_tip_main_v3 .word").fadeIn(1e3), $(".dd_tip_main_v3 .download").fadeIn(1e3), $(".dd_tip_main_v3 .heart").fadeIn(1e3), timer = setInterval(heartAnimate, 1e3)
    }), timer
}
function heartAnimate() {
    var elem = $(".dd_tip_main_v3 .heart");
    heartBig(elem)
}
function heartBig(elem) {
    $(elem).css("backgroundPositionY", "-60px"), setTimeout(function () {
        heartSmall(elem)
    }, 500)
}
function heartSmall(elem) {
    $(elem).css("backgroundPositionY", "0px")
}
function heartAnimate2() {
    var count = 0,
        elem = $(".dd_tip_main_v3 .heart"),
        t = setInterval(function () {
            2 > count ? (heartBig(elem), count++) : clearInterval(t)
        }, 500)
}
function detectOS() {
    return navigator.userAgent.indexOf("Window") > 0 ? "Windows" : navigator.userAgent.indexOf("Mac OS X") > 0 ? "Mac " : navigator.userAgent.indexOf("Linux") > 0 ? "Linux" : "NUll"
}

function detectOSDetailNew() {
    var sUserAgent = navigator.userAgent,
        isWin = "Win32" == navigator.platform || "Windows" == navigator.platform,
        isMac = "Mac68K" == navigator.platform || "MacPPC" == navigator.platform || "Macintosh" == navigator.platform || "MacIntel" == navigator.platform;
    if (isMac) return "Mac";
    var isUnix = "X11" == navigator.platform && !isWin && !isMac;
    if (isUnix) return "Unix";
    var isLinux = (navigator.platform + "").indexOf("Linux") > -1;
    if (isLinux) return "Linux";
    if (isWin) {
        var isWin2K = sUserAgent.indexOf("Windows NT 5.0") > -1 || sUserAgent.indexOf("Windows 2000") > -1;
        if (isWin2K) return "Win2000";
        var isWinXP = sUserAgent.indexOf("Windows NT 5.1") > -1 || sUserAgent.indexOf("Windows XP") > -1;
        if (isWinXP) return "WinXP";
        var isWin2003 = sUserAgent.indexOf("Windows NT 5.2") > -1 || sUserAgent.indexOf("Windows 2003") > -1;
        if (isWin2003) return "Win2003";
        var isWin2003 = sUserAgent.indexOf("Windows NT 6.0") > -1 || sUserAgent.indexOf("Windows Vista") > -1;
        if (isWin2003) return "WinVista";
        var isWin2003 = sUserAgent.indexOf("Windows NT 6.1") > -1 || sUserAgent.indexOf("Windows 7") > -1;
        if (isWin2003) return "Win7"
    }
    return "other"
}
function riskCapture(pamsuid, common, msg) {
    jQuery.ajax({
        url: "/riskCaptcha.action?t=" + (new Date).getTime(),
        type: "GET",
        dataType: "html",
        data: {
            pamsUid: pamsuid,
            message: msg
        },
        timeout: 3e4,
        error: function () {
            common.systemInfo("网络繁忙，请稍后再试<a href='#' onclick='location.reload(false);'> 请点击这里重试</a>", "im-msg-error")
        },
        success: function (xml) {
            $("#text_in").html("");
            var result = $.parseJSON(xml);
            result && result.arbitrateResultCode !== void 0 && ("BLACKLIST_HIT" === result.arbitrateResultCode ? (common.systemInfo(result.displayMessage, "im-msg-prompt"), riskCheckFlag = !1) : "ERROR_ANSWER" === result.arbitrateResultCode ? (common.systemInfo(tipMsg(result.captchaJssUrl, result.displayMessage), "im-msg-prompt"), riskCheckFlag = !1) : "ERROR_EXCEED_MAX" === result.arbitrateResultCode ? (common.systemInfo(result.displayMessage, "im-msg-prompt"), riskCheckFlag = !1) : "RIGHT_ANSWER" === result.arbitrateResultCode && (riskCheckFlag = !0))
        }
    })
}
function tipMsg(url, msg) {
    return "<span id='risk_sys_msg'>" + msg + "<br/><img src='" + url + "' style='height:30px;padding:10px;'/>" + "</span>"
}
function riskCheck(paramuid, common) {
    return "undefined" != typeof fromAgile && "true" === fromAgile ? (riskCheckFlag = !0, void 0) : (riskCheckFlag || riskCheckInvoked || (riskCheckInvoked = !0, jQuery.ajax({
        url: "/riskCheck.action?t=" + (new Date).getTime(),
        type: "GET",
        dataType: "html",
        data: {
            pamsUid: paramuid
        },
        timeout: 3e4,
        error: function () {
            common.systemInfo("网络繁忙，请稍后再试<a href='#' onclick='location.reload(false);'> 请点击这里重试</a>", "im-msg-error")
        },
        success: function (xml) {
            var result = $.parseJSON(xml);
            result && result.arbitrateResultCode !== void 0 && ("BLACKLIST_HIT" === result.arbitrateResultCode ? (common.systemInfo(result.displayMessage, "im-msg-prompt"), riskCheckFlag = !1) : "CAPTCHA" === result.arbitrateResultCode ? (common.systemInfo(tipMsg(result.captchaJssUrl, result.displayMessage), "im-msg-prompt"), riskCheckFlag = !1, jQuery("#sendMsg").unbind("click"), jQuery("#sendMsg").click(function () {
                var text_in_json = common.getTextIn();
                if (null == text_in_json) return common.systemInfo("消息不能为空", "text_1_st"), $("#text_in").html(), $("#text_in").blur(), $("#text_in").focus(), void 0;
                var text_in = $.trim(text_in_json.D);
                riskCapture(paramuid, common, text_in)
            })) : "VALIDATE_PASS" === result.arbitrateResultCode && (riskCheckFlag = !0))
        }
    })), void 0)
}
function urlClick(url, from) {
    var os = detectOS();
    "Windows" == os ? buildPopTipLayer() : buildPopNotSupportTipLayer(), buildPopTipEvent(), buildPopTipEventV3(), clickRecord(from), window.event.cancelBubble = !0, window.event.returnValue = !1, "Windows" == os && window.open("" == dd_download_url ? url : dd_download_url, "_blank"), window.event.cancelBubble = !0, window.event.returnValue = !1
}
function clickRecord(from) {
    try {
        $.ajax({
            url: "/dd/recordClick.action?from=normal_" + from + "&t=" + (new Date).getTime(),
            type: "GET",
            dataType: "html",
            success: function () {
            }
        })
    } catch (e) {
    }
}
function FileProgress(file, targetID) {
    if (this.fileProgressID = file.id, this.opacity = 100, this.height = 0, this.fileProgressWrapper = document.getElementById(this.fileProgressID), this.fileProgressWrapper) this.fileProgressElement = this.fileProgressWrapper.firstChild, this.reset();
    else {
        this.fileProgressWrapper = document.createElement("div"), this.fileProgressWrapper.className = "progressWrapper", this.fileProgressWrapper.id = this.fileProgressID, this.fileProgressElement = document.createElement("div"), this.fileProgressElement.className = "progressContainer";
        var progressCancel = document.createElement("a");
        progressCancel.className = "progressCancel", progressCancel.href = "#", progressCancel.style.visibility = "hidden", progressCancel.appendChild(document.createTextNode(" "));
        var progressText = document.createElement("div");
        progressText.className = "progressName", progressText.appendChild(document.createTextNode(file.name));
        var progressBar = document.createElement("div");
        progressBar.className = "progressBarInProgress";
        var progressStatus = document.createElement("div");
        progressStatus.className = "progressBarStatus", progressStatus.innerHTML = "&nbsp;", this.fileProgressElement.appendChild(progressCancel), this.fileProgressElement.appendChild(progressText), this.fileProgressElement.appendChild(progressStatus), this.fileProgressElement.appendChild(progressBar), this.fileProgressWrapper.appendChild(this.fileProgressElement), document.getElementById(targetID).appendChild(this.fileProgressWrapper)
    }
    this.height = this.fileProgressWrapper.offsetHeight, this.setTimer(null)
}
this.JSON || (this.JSON = {}), function () {
    function f(n) {
        return 10 > n ? "0" + n : n
    }

    function quote(string) {
        return escapable.lastIndex = 0, escapable.test(string) ? '"' + string.replace(escapable, function (a) {
            var c = meta[a];
            return "string" == typeof c ? c : "\\u" + ("0000" + a.charCodeAt(0).toString(16)).slice(-4)
        }) + '"' : '"' + string + '"'
    }

    function str(key, holder) {
        var i, k, v, length, partial, mind = gap,
            value = holder[key];
        switch (value && "object" == typeof value && "function" == typeof value.toJSON && (value = value.toJSON(key)), "function" == typeof rep && (value = rep.call(holder, key, value)), typeof value) {
            case "string":
                return quote(value);
            case "number":
                return isFinite(value) ? value + "" : "null";
            case "boolean":
            case "null":
                return value + "";
            case "object":
                if (!value) return "null";
                if (gap += indent, partial = [], "[object Array]" === Object.prototype.toString.apply(value)) {
                    for (length = value.length, i = 0; length > i; i += 1) partial[i] = str(i, value) || "null";
                    return v = 0 === partial.length ? "[]" : gap ? "[\n" + gap + partial.join(",\n" + gap) + "\n" + mind + "]" : "[" + partial.join(",") + "]", gap = mind, v
                }
                if (rep && "object" == typeof rep) for (length = rep.length, i = 0; length > i; i += 1) k = rep[i], "string" == typeof k && (v = str(k, value), v && partial.push(quote(k) + (gap ? ": " : ":") + v));
                else for (k in value) Object.hasOwnProperty.call(value, k) && (v = str(k, value), v && partial.push(quote(k) + (gap ? ": " : ":") + v));
                return v = 0 === partial.length ? "{}" : gap ? "{\n" + gap + partial.join(",\n" + gap) + "\n" + mind + "}" : "{" + partial.join(",") + "}", gap = mind, v
        }
    }

    "function" != typeof Date.prototype.toJSON && (Date.prototype.toJSON = function () {
        return isFinite(this.valueOf()) ? this.getUTCFullYear() + "-" + f(this.getUTCMonth() + 1) + "-" + f(this.getUTCDate()) + "T" + f(this.getUTCHours()) + ":" + f(this.getUTCMinutes()) + ":" + f(this.getUTCSeconds()) + "Z" : null
    }, String.prototype.toJSON = Number.prototype.toJSON = Boolean.prototype.toJSON = function () {
        return this.valueOf()
    });
    var cx = /&$/g,
        escapable = /&$/g,
        gap, indent, meta = {
            "\b": "\\b",
            "	": "\\t",
            "\n": "\\n",
            "\f": "\\f",
            "\r": "\\r",
            '"': '\\"',
            "\\": "\\\\"
        },
        rep;
    "function" != typeof JSON.stringify && (JSON.stringify = function (value, replacer, space) {
        var i;
        if (gap = "", indent = "", "number" == typeof space) for (i = 0; space > i; i += 1) indent += " ";
        else "string" == typeof space && (indent = space);
        if (rep = replacer, replacer && "function" != typeof replacer && ("object" != typeof replacer || "number" != typeof replacer.length)) throw Error("JSON.stringify");
        return str("", {
            "": value
        })
    }), "function" != typeof JSON.parse && (JSON.parse = function (text, reviver) {
        function walk(holder, key) {
            var k, v, value = holder[key];
            if (value && "object" == typeof value) for (k in value) Object.hasOwnProperty.call(value, k) && (v = walk(value, k), void 0 !== v ? value[k] = v : delete value[k]);
            return reviver.call(holder, key, value)
        }

        var j;
        if (text += "", cx.lastIndex = 0, cx.test(text) && (text = text.replace(cx, function (a) {
                return "\\u" + ("0000" + a.charCodeAt(0).toString(16)).slice(-4)
            })), /^[\],:{}\s]*$/.test(text.replace(/\\(?:["\\\/bfnrt]|u[0-9a-fA-F]{4})/g, "@").replace(/"[^"\\\n\r]*"|true|false|null|-?\d+(?:\.\d*)?(?:[eE][+\-]?\d+)?/g, "]").replace(/(?:^|:|,)(?:\s*\[)+/g, ""))) return j = eval("(" + text + ")"), "function" == typeof reviver ? walk({
            "": j
        }, "") : j;
        throw new SyntaxError("JSON.parse")
    })
}(), function (a, b) {
    function cy(a) {
        return f.isWindow(a) ? a : 9 === a.nodeType ? a.defaultView || a.parentWindow : !1
    }

    function cu(a) {
        if (!cj[a]) {
            var b = c.body,
                d = f("<" + a + ">").appendTo(b),
                e = d.css("display");
            d.remove(), ("none" === e || "" === e) && (ck || (ck = c.createElement("iframe"), ck.frameBorder = ck.width = ck.height = 0), b.appendChild(ck), cl && ck.createElement || (cl = (ck.contentWindow || ck.contentDocument).document, cl.write((f.support.boxModel ? "<!doctype html>" : "") + "<html><body>"), cl.close()), d = cl.createElement(a), cl.body.appendChild(d), e = f.css(d, "display"), b.removeChild(ck)), cj[a] = e
        }
        return cj[a]
    }

    function ct(a, b) {
        var c = {};
        return f.each(cp.concat.apply([], cp.slice(0, b)), function () {
            c[this] = a
        }), c
    }

    function cs() {
        cq = b
    }

    function cr() {
        return setTimeout(cs, 0), cq = f.now()
    }

    function ci() {
        try {
            return new a.ActiveXObject("Microsoft.XMLHTTP")
        } catch (b) {
        }
    }

    function ch() {
        try {
            return new a.XMLHttpRequest
        } catch (b) {
        }
    }

    function cb(a, c) {
        a.dataFilter && (c = a.dataFilter(c, a.dataType));
        var g, h, j, l, m, n, o, p, d = a.dataTypes,
            e = {},
            i = d.length,
            k = d[0];
        for (g = 1; i > g; g++) {
            if (1 === g) for (h in a.converters)"string" == typeof h && (e[h.toLowerCase()] = a.converters[h]);
            if (l = k, k = d[g], "*" === k) k = l;
            else if ("*" !== l && l !== k) {
                if (m = l + " " + k, n = e[m] || e["* " + k], !n) {
                    p = b;
                    for (o in e) if (j = o.split(" "), (j[0] === l || "*" === j[0]) && (p = e[j[1] + " " + k])) {
                        o = e[o], o === !0 ? n = p : p === !0 && (n = o);
                        break
                    }
                }
                !n && !p && f.error("No conversion from " + m.replace(" ", " to ")), n !== !0 && (c = n ? n(c) : p(o(c)))
            }
        }
        return c
    }

    function ca(a, c, d) {
        var h, i, j, k, e = a.contents,
            f = a.dataTypes,
            g = a.responseFields;
        for (i in g) i in d && (c[g[i]] = d[i]);
        for (;
            "*" === f[0];) f.shift(), h === b && (h = a.mimeType || c.getResponseHeader("content-type"));
        if (h) for (i in e) if (e[i] && e[i].test(h)) {
            f.unshift(i);
            break
        }
        if (f[0] in d) j = f[0];
        else {
            for (i in d) {
                if (!f[0] || a.converters[i + " " + f[0]]) {
                    j = i;
                    break
                }
                k || (k = i)
            }
            j = j || k
        }
        return j ? (j !== f[0] && f.unshift(j), d[j]) : void 0
    }

    function b_(a, b, c, d) {
        if (f.isArray(b)) f.each(b, function (b, e) {
            c || bD.test(a) ? d(a, e) : b_(a + "[" + ("object" == typeof e ? b : "") + "]", e, c, d)
        });
        else if (c || "object" !== f.type(b)) d(a, b);
        else for (var e in b) b_(a + "[" + e + "]", b[e], c, d)
    }

    function b$(a, c) {
        var d, e, g = f.ajaxSettings.flatOptions || {};
        for (d in c) c[d] !== b && ((g[d] ? a : e || (e = {}))[d] = c[d]);
        e && f.extend(!0, a, e)
    }

    function bZ(a, c, d, e, f, g) {
        f = f || c.dataTypes[0], g = g || {}, g[f] = !0;
        for (var l, h = a[f], i = 0, j = h ? h.length : 0, k = a === bS; j > i && (k || !l); i++) l = h[i](c, d, e), "string" == typeof l && (!k || g[l] ? l = b : (c.dataTypes.unshift(l), l = bZ(a, c, d, e, l, g)));
        return (k || !l) && !g["*"] && (l = bZ(a, c, d, e, "*", g)), l
    }

    function bY(a) {
        return function (b, c) {
            if ("string" != typeof b && (c = b, b = "*"), f.isFunction(c)) for (var h, i, j, d = b.toLowerCase().split(bO), e = 0, g = d.length; g > e; e++) h = d[e], j = /^\+/.test(h), j && (h = h.substr(1) || "*"), i = a[h] = a[h] || [], i[j ? "unshift" : "push"](c)
        }
    }

    function bB(a, b, c) {
        var d = "width" === b ? a.offsetWidth : a.offsetHeight,
            e = "width" === b ? 1 : 0,
            g = 4;
        if (d > 0) {
            if ("border" !== c) for (; g > e; e += 2) c || (d -= parseFloat(f.css(a, "padding" + bx[e])) || 0), "margin" === c ? d += parseFloat(f.css(a, c + bx[e])) || 0 : d -= parseFloat(f.css(a, "border" + bx[e] + "Width")) || 0;
            return d + "px"
        }
        if (d = by(a, b), (0 > d || null == d) && (d = a.style[b]), bt.test(d)) return d;
        if (d = parseFloat(d) || 0, c) for (; g > e; e += 2) d += parseFloat(f.css(a, "padding" + bx[e])) || 0, "padding" !== c && (d += parseFloat(f.css(a, "border" + bx[e] + "Width")) || 0), "margin" === c && (d += parseFloat(f.css(a, c + bx[e])) || 0);
        return d + "px"
    }

    function bo(a) {
        var b = c.createElement("div");
        return bh.appendChild(b), b.innerHTML = a.outerHTML, b.firstChild
    }

    function bn(a) {
        var b = (a.nodeName || "").toLowerCase();
        "input" === b ? bm(a) : "script" !== b && a.getElementsByTagName !== void 0 && f.grep(a.getElementsByTagName("input"), bm)
    }

    function bm(a) {
        ("checkbox" === a.type || "radio" === a.type) && (a.defaultChecked = a.checked)
    }

    function bl(a) {
        return a.getElementsByTagName !== void 0 ? a.getElementsByTagName("*") : a.querySelectorAll !== void 0 ? a.querySelectorAll("*") : []
    }

    function bk(a, b) {
        var c;
        1 === b.nodeType && (b.clearAttributes && b.clearAttributes(), b.mergeAttributes && b.mergeAttributes(a), c = b.nodeName.toLowerCase(), "object" === c ? b.outerHTML = a.outerHTML : "input" !== c || "checkbox" !== a.type && "radio" !== a.type ? "option" === c ? b.selected = a.defaultSelected : "input" === c || "textarea" === c ? b.defaultValue = a.defaultValue : "script" === c && b.text !== a.text && (b.text = a.text) : (a.checked && (b.defaultChecked = b.checked = a.checked), b.value !== a.value && (b.value = a.value)), b.removeAttribute(f.expando), b.removeAttribute("_submit_attached"), b.removeAttribute("_change_attached"))
    }

    function bj(a, b) {
        if (1 === b.nodeType && f.hasData(a)) {
            var c, d, e, g = f._data(a),
                h = f._data(b, g),
                i = g.events;
            if (i) {
                delete h.handle, h.events = {};
                for (c in i) for (d = 0, e = i[c].length; e > d; d++) f.event.add(b, c, i[c][d])
            }
            h.data && (h.data = f.extend({}, h.data))
        }
    }

    function bi(a) {
        return f.nodeName(a, "table") ? a.getElementsByTagName("tbody")[0] || a.appendChild(a.ownerDocument.createElement("tbody")) : a
    }

    function U(a) {
        var b = V.split("|"),
            c = a.createDocumentFragment();
        if (c.createElement) for (; b.length;) c.createElement(b.pop());
        return c
    }

    function T(a, b, c) {
        if (b = b || 0, f.isFunction(b)) return f.grep(a, function (a, d) {
            var e = !!b.call(a, d, a);
            return e === c
        });
        if (b.nodeType) return f.grep(a, function (a) {
            return a === b === c
        });
        if ("string" == typeof b) {
            var d = f.grep(a, function (a) {
                return 1 === a.nodeType
            });
            if (O.test(b)) return f.filter(b, d, !c);
            b = f.filter(b, d)
        }
        return f.grep(a, function (a) {
            return f.inArray(a, b) >= 0 === c
        })
    }

    function S(a) {
        return !a || !a.parentNode || 11 === a.parentNode.nodeType
    }

    function K() {
        return !0
    }

    function J() {
        return !1
    }

    function n(a, b, c) {
        var d = b + "defer",
            e = b + "queue",
            g = b + "mark",
            h = f._data(a, d);
        !(!h || "queue" !== c && f._data(a, e) || "mark" !== c && f._data(a, g) || !setTimeout(function () {
            !f._data(a, e) && !f._data(a, g) && (f.removeData(a, d, !0), h.fire())
        }, 0))
    }

    function m(a) {
        for (var b in a) if (("data" !== b || !f.isEmptyObject(a[b])) && "toJSON" !== b) return !1;
        return !0
    }

    function l(a, c, d) {
        if (d === b && 1 === a.nodeType) {
            var e = "data-" + c.replace(k, "-$1").toLowerCase();
            if (d = a.getAttribute(e), "string" == typeof d) {
                try {
                    d = "true" === d ? !0 : "false" === d ? !1 : "null" === d ? null : f.isNumeric(d) ? +d : j.test(d) ? f.parseJSON(d) : d
                } catch (g) {
                }
                f.data(a, c, d)
            } else d = b
        }
        return d
    }

    function h(a) {
        var c, d, b = g[a] = {};
        for (a = a.split(/\s+/), c = 0, d = a.length; d > c; c++) b[a[c]] = !0;
        return b
    }

    var c = a.document,
        d = a.navigator,
        e = a.location,
        f = function () {
            function J() {
                if (!e.isReady) {
                    try {
                        c.documentElement.doScroll("left")
                    } catch (a) {
                        return setTimeout(J, 1), void 0
                    }
                    e.ready()
                }
            }

            var h, z, A, B, e = function (a, b) {
                    return new e.fn.init(a, b, h)
                },
                f = a.jQuery,
                g = a.$,
                i = /^(?:[^#<]*(<[\w\W]+>)[^>]*$|#([\w\-]*)$)/,
                j = /\S/,
                k = /^\s+/,
                l = /\s+$/,
                m = /^<(\w+)\s*\/?>(?:<\/\1>)?$/,
                n = /^[\],:{}\s]*$/,
                o = /\\(?:["\\\/bfnrt]|u[0-9a-fA-F]{4})/g,
                p = /"[^"\\\n\r]*"|true|false|null|-?\d+(?:\.\d*)?(?:[eE][+\-]?\d+)?/g,
                q = /(?:^|:|,)(?:\s*\[)+/g,
                r = /(webkit)[ \/]([\w.]+)/,
                s = /(opera)(?:.*version)?[ \/]([\w.]+)/,
                t = /(msie) ([\w.]+)/,
                u = /(mozilla)(?:.*? rv:([\w.]+))?/,
                v = /-([a-z]|[0-9])/gi,
                w = /^-ms-/,
                x = function (a, b) {
                    return (b + "").toUpperCase()
                },
                y = d.userAgent,
                C = Object.prototype.toString,
                D = Object.prototype.hasOwnProperty,
                E = Array.prototype.push,
                F = Array.prototype.slice,
                G = String.prototype.trim,
                H = Array.prototype.indexOf,
                I = {};
            return e.fn = e.prototype = {
                constructor: e,
                init: function (a, d, f) {
                    var g, h, j, k;
                    if (!a) return this;
                    if (a.nodeType) return this.context = this[0] = a, this.length = 1, this;
                    if ("body" === a && !d && c.body) return this.context = c, this[0] = c.body, this.selector = a, this.length = 1, this;
                    if ("string" == typeof a) {
                        if (g = "<" !== a.charAt(0) || ">" !== a.charAt(a.length - 1) || 3 > a.length ? i.exec(a) : [null, a, null], g && (g[1] || !d)) {
                            if (g[1]) return d = d instanceof e ? d[0] : d, k = d ? d.ownerDocument || d : c, j = m.exec(a), j ? e.isPlainObject(d) ? (a = [c.createElement(j[1])], e.fn.attr.call(a, d, !0)) : a = [k.createElement(j[1])] : (j = e.buildFragment([g[1]], [k]), a = (j.cacheable ? e.clone(j.fragment) : j.fragment).childNodes), e.merge(this, a);
                            if (h = c.getElementById(g[2]), h && h.parentNode) {
                                if (h.id !== g[2]) return f.find(a);
                                this.length = 1, this[0] = h
                            }
                            return this.context = c, this.selector = a, this
                        }
                        return !d || d.jquery ? (d || f).find(a) : this.constructor(d).find(a)
                    }
                    return e.isFunction(a) ? f.ready(a) : (a.selector !== b && (this.selector = a.selector, this.context = a.context), e.makeArray(a, this))
                },
                selector: "",
                jquery: "1.7.2",
                length: 0,
                size: function () {
                    return this.length
                },
                toArray: function () {
                    return F.call(this, 0)
                },
                get: function (a) {
                    return null == a ? this.toArray() : 0 > a ? this[this.length + a] : this[a]
                },
                pushStack: function (a, b, c) {
                    var d = this.constructor();
                    return e.isArray(a) ? E.apply(d, a) : e.merge(d, a), d.prevObject = this, d.context = this.context, "find" === b ? d.selector = this.selector + (this.selector ? " " : "") + c : b && (d.selector = this.selector + "." + b + "(" + c + ")"), d
                },
                each: function (a, b) {
                    return e.each(this, a, b)
                },
                ready: function (a) {
                    return e.bindReady(), A.add(a), this
                },
                eq: function (a) {
                    return a = +a, -1 === a ? this.slice(a) : this.slice(a, a + 1)
                },
                first: function () {
                    return this.eq(0)
                },
                last: function () {
                    return this.eq(-1)
                },
                slice: function () {
                    return this.pushStack(F.apply(this, arguments), "slice", F.call(arguments).join(","))
                },
                map: function (a) {
                    return this.pushStack(e.map(this, function (b, c) {
                        return a.call(b, c, b)
                    }))
                },
                end: function () {
                    return this.prevObject || this.constructor(null)
                },
                push: E,
                sort: [].sort,
                splice: [].splice
            }, e.fn.init.prototype = e.fn, e.extend = e.fn.extend = function () {
                var a, c, d, f, g, h, i = arguments[0] || {},
                    j = 1,
                    k = arguments.length,
                    l = !1;
                for ("boolean" == typeof i && (l = i, i = arguments[1] || {}, j = 2), "object" != typeof i && !e.isFunction(i) && (i = {}), k === j && (i = this, --j); k > j; j++) if (null != (a = arguments[j])) for (c in a) d = i[c], f = a[c], i !== f && (l && f && (e.isPlainObject(f) || (g = e.isArray(f))) ? (g ? (g = !1, h = d && e.isArray(d) ? d : []) : h = d && e.isPlainObject(d) ? d : {}, i[c] = e.extend(l, h, f)) : f !== b && (i[c] = f));
                return i
            }, e.extend({
                noConflict: function (b) {
                    return a.$ === e && (a.$ = g), b && a.jQuery === e && (a.jQuery = f), e
                },
                isReady: !1,
                readyWait: 1,
                holdReady: function (a) {
                    a ? e.readyWait++ : e.ready(!0)
                },
                ready: function (a) {
                    if (a === !0 && !--e.readyWait || a !== !0 && !e.isReady) {
                        if (!c.body) return setTimeout(e.ready, 1);
                        if (e.isReady = !0, a !== !0 && --e.readyWait > 0) return;
                        A.fireWith(c, [e]), e.fn.trigger && e(c).trigger("ready").off("ready")
                    }
                },
                bindReady: function () {
                    if (!A) {
                        if (A = e.Callbacks("once memory"), "complete" === c.readyState) return setTimeout(e.ready, 1);
                        if (c.addEventListener) c.addEventListener("DOMContentLoaded", B, !1), a.addEventListener("load", e.ready, !1);
                        else if (c.attachEvent) {
                            c.attachEvent("onreadystatechange", B), a.attachEvent("onload", e.ready);
                            var b = !1;
                            try {
                                b = null == a.frameElement
                            } catch (d) {
                            }
                            c.documentElement.doScroll && b && J()
                        }
                    }
                },
                isFunction: function (a) {
                    return "function" === e.type(a)
                },
                isArray: Array.isArray ||
                function (a) {
                    return "array" === e.type(a)
                },
                isWindow: function (a) {
                    return null != a && a == a.window
                },
                isNumeric: function (a) {
                    return !isNaN(parseFloat(a)) && isFinite(a)
                },
                type: function (a) {
                    return null == a ? a + "" : I[C.call(a)] || "object"
                },
                isPlainObject: function (a) {
                    if (!a || "object" !== e.type(a) || a.nodeType || e.isWindow(a)) return !1;
                    try {
                        if (a.constructor && !D.call(a, "constructor") && !D.call(a.constructor.prototype, "isPrototypeOf")) return !1
                    } catch (c) {
                        return !1
                    }
                    var d;
                    for (d in a);
                    return d === b || D.call(a, d)
                },
                isEmptyObject: function (a) {
                    for (var b in a) return !1;
                    return !0
                },
                error: function (a) {
                    throw Error(a)
                },
                parseJSON: function (b) {
                    return "string" == typeof b && b ? (b = e.trim(b), a.JSON && a.JSON.parse ? a.JSON.parse(b) : n.test(b.replace(o, "@").replace(p, "]").replace(q, "")) ? Function("return " + b)() : (e.error("Invalid JSON: " + b), void 0)) : null
                },
                parseXML: function (c) {
                    if ("string" != typeof c || !c) return null;
                    var d, f;
                    try {
                        a.DOMParser ? (f = new DOMParser, d = f.parseFromString(c, "text/xml")) : (d = new ActiveXObject("Microsoft.XMLDOM"), d.async = "false", d.loadXML(c))
                    } catch (g) {
                        d = b
                    }
                    return (!d || !d.documentElement || d.getElementsByTagName("parsererror").length) && e.error("Invalid XML: " + c), d
                },
                noop: function () {
                },
                globalEval: function (b) {
                    b && j.test(b) && (a.execScript ||
                    function (b) {
                        a.eval.call(a, b)
                    })(b)
                },
                camelCase: function (a) {
                    return a.replace(w, "ms-").replace(v, x)
                },
                nodeName: function (a, b) {
                    return a.nodeName && a.nodeName.toUpperCase() === b.toUpperCase()
                },
                each: function (a, c, d) {
                    var f, g = 0,
                        h = a.length,
                        i = h === b || e.isFunction(a);
                    if (d) if (i) {
                        for (f in a) if (c.apply(a[f], d) === !1) break
                    } else for (; h > g && c.apply(a[g++], d) !== !1;);
                    else if (i) {
                        for (f in a) if (c.call(a[f], f, a[f]) === !1) break
                    } else for (; h > g && c.call(a[g], g, a[g++]) !== !1;);
                    return a
                },
                trim: G ?
                    function (a) {
                        return null == a ? "" : G.call(a)
                    } : function (a) {
                    return null == a ? "" : (a + "").replace(k, "").replace(l, "")
                },
                makeArray: function (a, b) {
                    var c = b || [];
                    if (null != a) {
                        var d = e.type(a);
                        null == a.length || "string" === d || "function" === d || "regexp" === d || e.isWindow(a) ? E.call(c, a) : e.merge(c, a)
                    }
                    return c
                },
                inArray: function (a, b, c) {
                    var d;
                    if (b) {
                        if (H) return H.call(b, a, c);
                        for (d = b.length, c = c ? 0 > c ? Math.max(0, d + c) : c : 0; d > c; c++) if (c in b && b[c] === a) return c
                    }
                    return -1
                },
                merge: function (a, c) {
                    var d = a.length,
                        e = 0;
                    if ("number" == typeof c.length) for (var f = c.length; f > e; e++) a[d++] = c[e];
                    else for (; c[e] !== b;) a[d++] = c[e++];
                    return a.length = d, a
                },
                grep: function (a, b, c) {
                    var e, d = [];
                    c = !!c;
                    for (var f = 0, g = a.length; g > f; f++) e = !!b(a[f], f), c !== e && d.push(a[f]);
                    return d
                },
                map: function (a, c, d) {
                    var f, g, h = [],
                        i = 0,
                        j = a.length,
                        k = a instanceof e || j !== b && "number" == typeof j && (j > 0 && a[0] && a[j - 1] || 0 === j || e.isArray(a));
                    if (k) for (; j > i; i++) f = c(a[i], i, d), null != f && (h[h.length] = f);
                    else for (g in a) f = c(a[g], g, d), null != f && (h[h.length] = f);
                    return h.concat.apply([], h)
                },
                guid: 1,
                proxy: function (a, c) {
                    if ("string" == typeof c) {
                        var d = a[c];
                        c = a, a = d
                    }
                    if (!e.isFunction(a)) return b;
                    var f = F.call(arguments, 2),
                        g = function () {
                            return a.apply(c, f.concat(F.call(arguments)))
                        };
                    return g.guid = a.guid = a.guid || g.guid || e.guid++, g
                },
                access: function (a, c, d, f, g, h, i) {
                    var j, k = null == d,
                        l = 0,
                        m = a.length;
                    if (d && "object" == typeof d) {
                        for (l in d) e.access(a, c, l, d[l], 1, h, f);
                        g = 1
                    } else if (f !== b) {
                        if (j = i === b && e.isFunction(f), k && (j ? (j = c, c = function (a, b, c) {
                                return j.call(e(a), c)
                            }) : (c.call(a, f), c = null)), c) for (; m > l; l++) c(a[l], d, j ? f.call(a[l], l, c(a[l], d)) : f, i);
                        g = 1
                    }
                    return g ? a : k ? c.call(a) : m ? c(a[0], d) : h
                },
                now: function () {
                    return (new Date).getTime()
                },
                uaMatch: function (a) {
                    a = a.toLowerCase();
                    var b = r.exec(a) || s.exec(a) || t.exec(a) || 0 > a.indexOf("compatible") && u.exec(a) || [];
                    return {
                        browser: b[1] || "",
                        version: b[2] || "0"
                    }
                },
                sub: function () {
                    function a(b, c) {
                        return new a.fn.init(b, c)
                    }

                    e.extend(!0, a, this), a.superclass = this, a.fn = a.prototype = this(), a.fn.constructor = a, a.sub = this.sub, a.fn.init = function (d, f) {
                        return f && f instanceof e && !(f instanceof a) && (f = a(f)), e.fn.init.call(this, d, f, b)
                    }, a.fn.init.prototype = a.fn;
                    var b = a(c);
                    return a
                },
                browser: {}
            }), e.each("Boolean Number String Function Array Date RegExp Object".split(" "), function (a, b) {
                I["[object " + b + "]"] = b.toLowerCase()
            }), z = e.uaMatch(y), z.browser && (e.browser[z.browser] = !0, e.browser.version = z.version), e.browser.webkit && (e.browser.safari = !0), j.test(" ") && (k = /^[\s ]+/, l = /[\s ]+$/), h = e(c), c.addEventListener ? B = function () {
                c.removeEventListener("DOMContentLoaded", B, !1), e.ready()
            } : c.attachEvent && (B = function () {
                "complete" === c.readyState && (c.detachEvent("onreadystatechange", B), e.ready())
            }), e
        }(),
        g = {};
    f.Callbacks = function (a) {
        a = a ? g[a] || h(a) : {};
        var e, i, j, k, l, m, c = [],
            d = [],
            n = function (b) {
                var d, e, g, h;
                for (d = 0, e = b.length; e > d; d++) g = b[d], h = f.type(g), "array" === h ? n(g) : "function" === h && (!a.unique || !p.has(g)) && c.push(g)
            },
            o = function (b, f) {
                for (f = f || [], e = !a.memory || [b, f], i = !0, j = !0, m = k || 0, k = 0, l = c.length; c && l > m; m++) if (c[m].apply(b, f) === !1 && a.stopOnFalse) {
                    e = !0;
                    break
                }
                j = !1, c && (a.once ? e === !0 ? p.disable() : c = [] : d && d.length && (e = d.shift(), p.fireWith(e[0], e[1])))
            },
            p = {
                add: function () {
                    if (c) {
                        var a = c.length;
                        n(arguments), j ? l = c.length : e && e !== !0 && (k = a, o(e[0], e[1]))
                    }
                    return this
                },
                remove: function () {
                    if (c) for (var b = arguments, d = 0, e = b.length; e > d; d++) for (var f = 0; c.length > f && (b[d] !== c[f] || (j && l >= f && (l--, m >= f && m--), c.splice(f--, 1), !a.unique)); f++);
                    return this
                },
                has: function (a) {
                    if (c) for (var b = 0, d = c.length; d > b; b++) if (a === c[b]) return !0;
                    return !1
                },
                empty: function () {
                    return c = [], this
                },
                disable: function () {
                    return c = d = e = b, this
                },
                disabled: function () {
                    return !c
                },
                lock: function () {
                    return d = b, (!e || e === !0) && p.disable(), this
                },
                locked: function () {
                    return !d
                },
                fireWith: function (b, c) {
                    return d && (j ? a.once || d.push([b, c]) : (!a.once || !e) && o(b, c)), this
                },
                fire: function () {
                    return p.fireWith(this, arguments), this
                },
                fired: function () {
                    return !!i
                }
            };
        return p
    };
    var i = [].slice;
    f.extend({
        Deferred: function (a) {
            var j, b = f.Callbacks("once memory"),
                c = f.Callbacks("once memory"),
                d = f.Callbacks("memory"),
                e = "pending",
                g = {
                    resolve: b,
                    reject: c,
                    notify: d
                },
                h = {
                    done: b.add,
                    fail: c.add,
                    progress: d.add,
                    state: function () {
                        return e
                    },
                    isResolved: b.fired,
                    isRejected: c.fired,
                    then: function (a, b, c) {
                        return i.done(a).fail(b).progress(c), this
                    },
                    always: function () {
                        return i.done.apply(i, arguments).fail.apply(i, arguments), this
                    },
                    pipe: function (a, b, c) {
                        return f.Deferred(function (d) {
                            f.each({
                                done: [a, "resolve"],
                                fail: [b, "reject"],
                                progress: [c, "notify"]
                            }, function (a, b) {
                                var g, c = b[0],
                                    e = b[1];
                                f.isFunction(c) ? i[a](function () {
                                    g = c.apply(this, arguments), g && f.isFunction(g.promise) ? g.promise().then(d.resolve, d.reject, d.notify) : d[e + "With"](this === i ? d : this, [g])
                                }) : i[a](d[e])
                            })
                        }).promise()
                    },
                    promise: function (a) {
                        if (null == a) a = h;
                        else for (var b in h) a[b] = h[b];
                        return a
                    }
                },
                i = h.promise({});
            for (j in g) i[j] = g[j].fire, i[j + "With"] = g[j].fireWith;
            return i.done(function () {
                e = "resolved"
            }, c.disable, d.lock).fail(function () {
                e = "rejected"
            }, b.disable, d.lock), a && a.call(i, i), i
        },
        when: function (a) {
            function m(a) {
                return function (b) {
                    e[a] = arguments.length > 1 ? i.call(arguments, 0) : b, j.notifyWith(k, e)
                }
            }

            function l(a) {
                return function (c) {
                    b[a] = arguments.length > 1 ? i.call(arguments, 0) : c, --g || j.resolveWith(j, b)
                }
            }

            var b = i.call(arguments, 0),
                c = 0,
                d = b.length,
                e = Array(d),
                g = d,
                j = 1 >= d && a && f.isFunction(a.promise) ? a : f.Deferred(),
                k = j.promise();
            if (d > 1) {
                for (; d > c; c++) b[c] && b[c].promise && f.isFunction(b[c].promise) ? b[c].promise().then(l(c), j.reject, m(c)) : --g;
                g || j.resolveWith(j, b)
            } else j !== a && j.resolveWith(j, d ? [a] : []);
            return k
        }
    }), f.support = function () {
        var b, d, e, g, h, i, j, k, m, n, o, p = c.createElement("div");
        if (c.documentElement, p.setAttribute("className", "t"), p.innerHTML = "   <link/><table></table><a href='/a' style='top:1px;float:left;opacity:.55;'>a</a><input type='checkbox'/>", d = p.getElementsByTagName("*"), e = p.getElementsByTagName("a")[0], !d || !d.length || !e) return {};
        g = c.createElement("select"), h = g.appendChild(c.createElement("option")), i = p.getElementsByTagName("input")[0], b = {
            leadingWhitespace: 3 === p.firstChild.nodeType,
            tbody: !p.getElementsByTagName("tbody").length,
            htmlSerialize: !!p.getElementsByTagName("link").length,
            style: /top/.test(e.getAttribute("style")),
            hrefNormalized: "/a" === e.getAttribute("href"),
            opacity: /^0.55/.test(e.style.opacity),
            cssFloat: !!e.style.cssFloat,
            checkOn: "on" === i.value,
            optSelected: h.selected,
            getSetAttribute: "t" !== p.className,
            enctype: !!c.createElement("form").enctype,
            html5Clone: "<:nav></:nav>" !== c.createElement("nav").cloneNode(!0).outerHTML,
            submitBubbles: !0,
            changeBubbles: !0,
            focusinBubbles: !1,
            deleteExpando: !0,
            noCloneEvent: !0,
            inlineBlockNeedsLayout: !1,
            shrinkWrapBlocks: !1,
            reliableMarginRight: !0,
            pixelMargin: !0
        }, f.boxModel = b.boxModel = "CSS1Compat" === c.compatMode, i.checked = !0, b.noCloneChecked = i.cloneNode(!0).checked, g.disabled = !0, b.optDisabled = !h.disabled;
        try {
            delete p.test
        } catch (r) {
            b.deleteExpando = !1
        }
        if (!p.addEventListener && p.attachEvent && p.fireEvent && (p.attachEvent("onclick", function () {
                b.noCloneEvent = !1
            }), p.cloneNode(!0).fireEvent("onclick")), i = c.createElement("input"), i.value = "t", i.setAttribute("type", "radio"), b.radioValue = "t" === i.value, i.setAttribute("checked", "checked"), i.setAttribute("name", "t"), p.appendChild(i), j = c.createDocumentFragment(), j.appendChild(p.lastChild), b.checkClone = j.cloneNode(!0).cloneNode(!0).lastChild.checked, b.appendChecked = i.checked, j.removeChild(i), j.appendChild(p), p.attachEvent) for (n in {
            submit: 1,
            change: 1,
            focusin: 1
        }) m = "on" + n, o = m in p, o || (p.setAttribute(m, "return;"), o = "function" == typeof p[m]), b[n + "Bubbles"] = o;
        return j.removeChild(p), j = g = h = p = i = null, f(function () {
            var d, e, g, i, j, l, m, n, q, r, s, t, u = c.getElementsByTagName("body")[0];
            !u || (m = 1, t = "padding:0;margin:0;border:", r = "position:absolute;top:0;left:0;width:1px;height:1px;", s = t + "0;visibility:hidden;", n = "style='" + r + t + "5px solid #000;", q = "<div " + n + "display:block;'><div style='" + t + "0;display:block;overflow:hidden;'></div></div>" + "<table " + n + "' cellpadding='0' cellspacing='0'>" + "<tr><td></td></tr></table>", d = c.createElement("div"), d.style.cssText = s + "width:0;height:0;position:static;top:0;margin-top:" + m + "px", u.insertBefore(d, u.firstChild), p = c.createElement("div"), d.appendChild(p), p.innerHTML = "<table><tr><td style='" + t + "0;display:none'></td><td>t</td></tr></table>", k = p.getElementsByTagName("td"), o = 0 === k[0].offsetHeight, k[0].style.display = "", k[1].style.display = "none", b.reliableHiddenOffsets = o && 0 === k[0].offsetHeight, a.getComputedStyle && (p.innerHTML = "", l = c.createElement("div"), l.style.width = "0", l.style.marginRight = "0", p.style.width = "2px", p.appendChild(l), b.reliableMarginRight = 0 === (parseInt((a.getComputedStyle(l, null) || {
                marginRight: 0
            }).marginRight, 10) || 0)), p.style.zoom !== void 0 && (p.innerHTML = "", p.style.width = p.style.padding = "1px", p.style.border = 0, p.style.overflow = "hidden", p.style.display = "inline", p.style.zoom = 1, b.inlineBlockNeedsLayout = 3 === p.offsetWidth, p.style.display = "block", p.style.overflow = "visible", p.innerHTML = "<div style='width:5px;'></div>", b.shrinkWrapBlocks = 3 !== p.offsetWidth), p.style.cssText = r + s, p.innerHTML = q, e = p.firstChild, g = e.firstChild, i = e.nextSibling.firstChild.firstChild, j = {
                doesNotAddBorder: 5 !== g.offsetTop,
                doesAddBorderForTableAndCells: 5 === i.offsetTop
            }, g.style.position = "fixed", g.style.top = "20px", j.fixedPosition = 20 === g.offsetTop || 15 === g.offsetTop, g.style.position = g.style.top = "", e.style.overflow = "hidden", e.style.position = "relative", j.subtractsBorderForOverflowNotVisible = -5 === g.offsetTop, j.doesNotIncludeMarginInBodyOffset = u.offsetTop !== m, a.getComputedStyle && (p.style.marginTop = "1%", b.pixelMargin = "1%" !== (a.getComputedStyle(p, null) || {
                marginTop: 0
            }).marginTop), d.style.zoom !== void 0 && (d.style.zoom = 1), u.removeChild(d), l = p = d = null, f.extend(b, j))
        }), b
    }();
    var j = /^(?:\{.*\}|\[.*\])$/,
        k = /([A-Z])/g;
    f.extend({
        cache: {},
        uuid: 0,
        expando: "jQuery" + (f.fn.jquery + Math.random()).replace(/\D/g, ""),
        noData: {
            embed: !0,
            object: "clsid:D27CDB6E-AE6D-11cf-96B8-444553540000",
            applet: !0
        },
        hasData: function (a) {
            return a = a.nodeType ? f.cache[a[f.expando]] : a[f.expando], !!a && !m(a)
        },
        data: function (a, c, d, e) {
            if (f.acceptData(a)) {
                var g, h, i, j = f.expando,
                    k = "string" == typeof c,
                    l = a.nodeType,
                    m = l ? f.cache : a,
                    n = l ? a[j] : a[j] && j,
                    o = "events" === c;
                if (!(n && m[n] && (o || e || m[n].data) || !k || d !== b)) return;
                return n || (l ? a[j] = n = ++f.uuid : n = j), m[n] || (m[n] = {}, l || (m[n].toJSON = f.noop)), ("object" == typeof c || "function" == typeof c) && (e ? m[n] = f.extend(m[n], c) : m[n].data = f.extend(m[n].data, c)), g = h = m[n], e || (h.data || (h.data = {}), h = h.data), d !== b && (h[f.camelCase(c)] = d), o && !h[c] ? g.events : (k ? (i = h[c], null == i && (i = h[f.camelCase(c)])) : i = h, i)
            }
        },
        removeData: function (a, b, c) {
            if (f.acceptData(a)) {
                var d, e, g, h = f.expando,
                    i = a.nodeType,
                    j = i ? f.cache : a,
                    k = i ? a[h] : h;
                if (!j[k]) return;
                if (b && (d = c ? j[k] : j[k].data)) {
                    f.isArray(b) || (b in d ? b = [b] : (b = f.camelCase(b), b = b in d ? [b] : b.split(" ")));
                    for (e = 0, g = b.length; g > e; e++) delete d[b[e]];
                    if (!(c ? m : f.isEmptyObject)(d)) return
                }
                if (!c && (delete j[k].data, !m(j[k]))) return;
                f.support.deleteExpando || !j.setInterval ? delete j[k] : j[k] = null, i && (f.support.deleteExpando ? delete a[h] : a.removeAttribute ? a.removeAttribute(h) : a[h] = null)
            }
        },
        _data: function (a, b, c) {
            return f.data(a, b, c, !0)
        },
        acceptData: function (a) {
            if (a.nodeName) {
                var b = f.noData[a.nodeName.toLowerCase()];
                if (b) return b !== !0 && a.getAttribute("classid") === b
            }
            return !0
        }
    }), f.fn.extend({
        data: function (a, c) {
            var d, e, g, h, i, j = this[0],
                k = 0,
                m = null;
            if (a === b) {
                if (this.length && (m = f.data(j), 1 === j.nodeType && !f._data(j, "parsedAttrs"))) {
                    for (g = j.attributes, i = g.length; i > k; k++) h = g[k].name, 0 === h.indexOf("data-") && (h = f.camelCase(h.substring(5)), l(j, h, m[h]));
                    f._data(j, "parsedAttrs", !0)
                }
                return m
            }
            return "object" == typeof a ? this.each(function () {
                f.data(this, a)
            }) : (d = a.split(".", 2), d[1] = d[1] ? "." + d[1] : "", e = d[1] + "!", f.access(this, function (c) {
                return c === b ? (m = this.triggerHandler("getData" + e, [d[0]]), m === b && j && (m = f.data(j, a), m = l(j, a, m)), m === b && d[1] ? this.data(d[0]) : m) : (d[1] = c, this.each(function () {
                    var b = f(this);
                    b.triggerHandler("setData" + e, d), f.data(this, a, c), b.triggerHandler("changeData" + e, d)
                }), void 0)
            }, null, c, arguments.length > 1, null, !1))
        },
        removeData: function (a) {
            return this.each(function () {
                f.removeData(this, a)
            })
        }
    }), f.extend({
        _mark: function (a, b) {
            a && (b = (b || "fx") + "mark", f._data(a, b, (f._data(a, b) || 0) + 1))
        },
        _unmark: function (a, b, c) {
            if (a !== !0 && (c = b, b = a, a = !1), b) {
                c = c || "fx";
                var d = c + "mark",
                    e = a ? 0 : (f._data(b, d) || 1) - 1;
                e ? f._data(b, d, e) : (f.removeData(b, d, !0), n(b, c, "mark"))
            }
        },
        queue: function (a, b, c) {
            var d;
            return a ? (b = (b || "fx") + "queue", d = f._data(a, b), c && (!d || f.isArray(c) ? d = f._data(a, b, f.makeArray(c)) : d.push(c)), d || []) : void 0
        },
        dequeue: function (a, b) {
            b = b || "fx";
            var c = f.queue(a, b),
                d = c.shift(),
                e = {};
            "inprogress" === d && (d = c.shift()), d && ("fx" === b && c.unshift("inprogress"), f._data(a, b + ".run", e), d.call(a, function () {
                f.dequeue(a, b)
            }, e)), c.length || (f.removeData(a, b + "queue " + b + ".run", !0), n(a, b, "queue"))
        }
    }), f.fn.extend({
        queue: function (a, c) {
            var d = 2;
            return "string" != typeof a && (c = a, a = "fx", d--), d > arguments.length ? f.queue(this[0], a) : c === b ? this : this.each(function () {
                var b = f.queue(this, a, c);
                "fx" === a && "inprogress" !== b[0] && f.dequeue(this, a)
            })
        },
        dequeue: function (a) {
            return this.each(function () {
                f.dequeue(this, a)
            })
        },
        delay: function (a, b) {
            return a = f.fx ? f.fx.speeds[a] || a : a, b = b || "fx", this.queue(b, function (b, c) {
                var d = setTimeout(b, a);
                c.stop = function () {
                    clearTimeout(d)
                }
            })
        },
        clearQueue: function (a) {
            return this.queue(a || "fx", [])
        },
        promise: function (a, c) {
            function m() {
                --h || d.resolveWith(e, [e])
            }

            "string" != typeof a && (c = a, a = b), a = a || "fx";
            for (var l, d = f.Deferred(), e = this, g = e.length, h = 1, i = a + "defer", j = a + "queue", k = a + "mark"; g--;)(l = f.data(e[g], i, b, !0) || (f.data(e[g], j, b, !0) || f.data(e[g], k, b, !0)) && f.data(e[g], i, f.Callbacks("once memory"), !0)) && (h++, l.add(m));
            return m(), d.promise(c)
        }
    });
    var w, x, y, o = /[\n\t\r]/g,
        p = /\s+/,
        q = /\r/g,
        r = /^(?:button|input)$/i,
        s = /^(?:button|input|object|select|textarea)$/i,
        t = /^a(?:rea)?$/i,
        u = /^(?:autofocus|autoplay|async|checked|controls|defer|disabled|hidden|loop|multiple|open|readonly|required|scoped|selected)$/i,
        v = f.support.getSetAttribute;
    f.fn.extend({
        attr: function (a, b) {
            return f.access(this, f.attr, a, b, arguments.length > 1)
        },
        removeAttr: function (a) {
            return this.each(function () {
                f.removeAttr(this, a)
            })
        },
        prop: function (a, b) {
            return f.access(this, f.prop, a, b, arguments.length > 1)
        },
        removeProp: function (a) {
            return a = f.propFix[a] || a, this.each(function () {
                try {
                    this[a] = b, delete this[a]
                } catch (c) {
                }
            })
        },
        addClass: function (a) {
            var b, c, d, e, g, h, i;
            if (f.isFunction(a)) return this.each(function (b) {
                f(this).addClass(a.call(this, b, this.className))
            });
            if (a && "string" == typeof a) for (b = a.split(p), c = 0, d = this.length; d > c; c++) if (e = this[c], 1 === e.nodeType) if (e.className || 1 !== b.length) {
                for (g = " " + e.className + " ", h = 0, i = b.length; i > h; h++)~g.indexOf(" " + b[h] + " ") || (g += b[h] + " ");
                e.className = f.trim(g)
            } else e.className = a;
            return this
        },
        removeClass: function (a) {
            var c, d, e, g, h, i, j;
            if (f.isFunction(a)) return this.each(function (b) {
                f(this).removeClass(a.call(this, b, this.className))
            });
            if (a && "string" == typeof a || a === b) for (c = (a || "").split(p), d = 0, e = this.length; e > d; d++) if (g = this[d], 1 === g.nodeType && g.className) if (a) {
                for (h = (" " + g.className + " ").replace(o, " "), i = 0, j = c.length; j > i; i++) h = h.replace(" " + c[i] + " ", " ");
                g.className = f.trim(h)
            } else g.className = "";
            return this
        },
        toggleClass: function (a, b) {
            var c = typeof a,
                d = "boolean" == typeof b;
            return f.isFunction(a) ? this.each(function (c) {
                f(this).toggleClass(a.call(this, c, this.className, b), b)
            }) : this.each(function () {
                if ("string" === c) for (var e, g = 0, h = f(this), i = b, j = a.split(p); e = j[g++];) i = d ? i : !h.hasClass(e), h[i ? "addClass" : "removeClass"](e);
                else("undefined" === c || "boolean" === c) && (this.className && f._data(this, "__className__", this.className), this.className = this.className || a === !1 ? "" : f._data(this, "__className__") || "")
            })
        },
        hasClass: function (a) {
            for (var b = " " + a + " ", c = 0, d = this.length; d > c; c++) if (1 === this[c].nodeType && (" " + this[c].className + " ").replace(o, " ").indexOf(b) > -1) return !0;
            return !1
        },
        val: function (a) {
            var c, d, e, g = this[0];
            return arguments.length ? (e = f.isFunction(a), this.each(function (d) {
                var h, g = f(this);
                1 === this.nodeType && (h = e ? a.call(this, d, g.val()) : a, null == h ? h = "" : "number" == typeof h ? h += "" : f.isArray(h) && (h = f.map(h, function (a) {
                    return null == a ? "" : a + ""
                })), c = f.valHooks[this.type] || f.valHooks[this.nodeName.toLowerCase()], c && "set" in c && c.set(this, h, "value") !== b || (this.value = h))
            })) : g ? (c = f.valHooks[g.type] || f.valHooks[g.nodeName.toLowerCase()], c && "get" in c && (d = c.get(g, "value")) !== b ? d : (d = g.value, "string" == typeof d ? d.replace(q, "") : null == d ? "" : d)) : void 0
        }
    }), f.extend({
        valHooks: {
            option: {
                get: function (a) {
                    var b = a.attributes.value;
                    return !b || b.specified ? a.value : a.text
                }
            },
            select: {
                get: function (a) {
                    var b, c, d, e, g = a.selectedIndex,
                        h = [],
                        i = a.options,
                        j = "select-one" === a.type;
                    if (0 > g) return null;
                    for (c = j ? g : 0, d = j ? g + 1 : i.length; d > c; c++) if (e = i[c], !(!e.selected || (f.support.optDisabled ? e.disabled : null !== e.getAttribute("disabled")) || e.parentNode.disabled && f.nodeName(e.parentNode, "optgroup"))) {
                        if (b = f(e).val(), j) return b;
                        h.push(b)
                    }
                    return j && !h.length && i.length ? f(i[g]).val() : h
                },
                set: function (a, b) {
                    var c = f.makeArray(b);
                    return f(a).find("option").each(function () {
                        this.selected = f.inArray(f(this).val(), c) >= 0
                    }), c.length || (a.selectedIndex = -1), c
                }
            }
        },
        attrFn: {
            val: !0,
            css: !0,
            html: !0,
            text: !0,
            data: !0,
            width: !0,
            height: !0,
            offset: !0
        },
        attr: function (a, c, d, e) {
            var g, h, i, j = a.nodeType;
            return a && 3 !== j && 8 !== j && 2 !== j ? e && c in f.attrFn ? f(a)[c](d) : a.getAttribute === void 0 ? f.prop(a, c, d) : (i = 1 !== j || !f.isXMLDoc(a), i && (c = c.toLowerCase(), h = f.attrHooks[c] || (u.test(c) ? x : w)), d !== b ? null === d ? (f.removeAttr(a, c), void 0) : h && "set" in h && i && (g = h.set(a, d, c)) !== b ? g : (a.setAttribute(c, "" + d), d) : h && "get" in h && i && null !== (g = h.get(a, c)) ? g : (g = a.getAttribute(c), null === g ? b : g)) : void 0
        },
        removeAttr: function (a, b) {
            var c, d, e, g, h, i = 0;
            if (b && 1 === a.nodeType) for (d = b.toLowerCase().split(p), g = d.length; g > i; i++) e = d[i], e && (c = f.propFix[e] || e, h = u.test(e), h || f.attr(a, e, ""), a.removeAttribute(v ? e : c), h && c in a && (a[c] = !1))
        },
        attrHooks: {
            type: {
                set: function (a, b) {
                    if (r.test(a.nodeName) && a.parentNode) f.error("type property can't be changed");
                    else if (!f.support.radioValue && "radio" === b && f.nodeName(a, "input")) {
                        var c = a.value;
                        return a.setAttribute("type", b), c && (a.value = c), b
                    }
                }
            },
            value: {
                get: function (a, b) {
                    return w && f.nodeName(a, "button") ? w.get(a, b) : b in a ? a.value : null
                },
                set: function (a, b, c) {
                    return w && f.nodeName(a, "button") ? w.set(a, b, c) : (a.value = b, void 0)
                }
            }
        },
        propFix: {
            tabindex: "tabIndex",
            readonly: "readOnly",
            "for": "htmlFor",
            "class": "className",
            maxlength: "maxLength",
            cellspacing: "cellSpacing",
            cellpadding: "cellPadding",
            rowspan: "rowSpan",
            colspan: "colSpan",
            usemap: "useMap",
            frameborder: "frameBorder",
            contenteditable: "contentEditable"
        },
        prop: function (a, c, d) {
            var e, g, h, i = a.nodeType;
            return a && 3 !== i && 8 !== i && 2 !== i ? (h = 1 !== i || !f.isXMLDoc(a), h && (c = f.propFix[c] || c, g = f.propHooks[c]), d !== b ? g && "set" in g && (e = g.set(a, d, c)) !== b ? e : a[c] = d : g && "get" in g && null !== (e = g.get(a, c)) ? e : a[c]) : void 0
        },
        propHooks: {
            tabIndex: {
                get: function (a) {
                    var c = a.getAttributeNode("tabindex");
                    return c && c.specified ? parseInt(c.value, 10) : s.test(a.nodeName) || t.test(a.nodeName) && a.href ? 0 : b
                }
            }
        }
    }), f.attrHooks.tabindex = f.propHooks.tabIndex, x = {
        get: function (a, c) {
            var d, e = f.prop(a, c);
            return e === !0 || "boolean" != typeof e && (d = a.getAttributeNode(c)) && d.nodeValue !== !1 ? c.toLowerCase() : b
        },
        set: function (a, b, c) {
            var d;
            return b === !1 ? f.removeAttr(a, c) : (d = f.propFix[c] || c, d in a && (a[d] = !0), a.setAttribute(c, c.toLowerCase())), c
        }
    }, v || (y = {
        name: !0,
        id: !0,
        coords: !0
    }, w = f.valHooks.button = {
        get: function (a, c) {
            var d;
            return d = a.getAttributeNode(c), d && (y[c] ? "" !== d.nodeValue : d.specified) ? d.nodeValue : b
        },
        set: function (a, b, d) {
            var e = a.getAttributeNode(d);
            return e || (e = c.createAttribute(d), a.setAttributeNode(e)), e.nodeValue = b + ""
        }
    }, f.attrHooks.tabindex.set = w.set, f.each(["width", "height"], function (a, b) {
        f.attrHooks[b] = f.extend(f.attrHooks[b], {
            set: function (a, c) {
                return "" === c ? (a.setAttribute(b, "auto"), c) : void 0
            }
        })
    }), f.attrHooks.contenteditable = {
        get: w.get,
        set: function (a, b, c) {
            "" === b && (b = "false"), w.set(a, b, c)
        }
    }), f.support.hrefNormalized || f.each(["href", "src", "width", "height"], function (a, c) {
        f.attrHooks[c] = f.extend(f.attrHooks[c], {
            get: function (a) {
                var d = a.getAttribute(c, 2);
                return null === d ? b : d
            }
        })
    }), f.support.style || (f.attrHooks.style = {
        get: function (a) {
            return a.style.cssText.toLowerCase() || b
        },
        set: function (a, b) {
            return a.style.cssText = "" + b
        }
    }), f.support.optSelected || (f.propHooks.selected = f.extend(f.propHooks.selected, {
        get: function (a) {
            var b = a.parentNode;
            return b && (b.selectedIndex, b.parentNode && b.parentNode.selectedIndex), null
        }
    })), f.support.enctype || (f.propFix.enctype = "encoding"), f.support.checkOn || f.each(["radio", "checkbox"], function () {
        f.valHooks[this] = {
            get: function (a) {
                return null === a.getAttribute("value") ? "on" : a.value
            }
        }
    }), f.each(["radio", "checkbox"], function () {
        f.valHooks[this] = f.extend(f.valHooks[this], {
            set: function (a, b) {
                return f.isArray(b) ? a.checked = f.inArray(f(a).val(), b) >= 0 : void 0
            }
        })
    });
    var z = /^(?:textarea|input|select)$/i,
        A = /^([^\.]*)?(?:\.(.+))?$/,
        B = /(?:^|\s)hover(\.\S+)?\b/,
        C = /^key/,
        D = /^(?:mouse|contextmenu)|click/,
        E = /^(?:focusinfocus|focusoutblur)$/,
        F = /^(\w*)(?:#([\w\-]+))?(?:\.([\w\-]+))?$/,
        G = function (a) {
            var b = F.exec(a);
            return b && (b[1] = (b[1] || "").toLowerCase(), b[3] = b[3] && RegExp("(?:^|\\s)" + b[3] + "(?:\\s|$)")), b
        },
        H = function (a, b) {
            var c = a.attributes || {};
            return !(b[1] && a.nodeName.toLowerCase() !== b[1] || b[2] && (c.id || {}).value !== b[2] || b[3] && !b[3].test((c["class"] || {}).value))
        },
        I = function (a) {
            return f.event.special.hover ? a : a.replace(B, "mouseenter$1 mouseleave$1")
        };
    f.event = {
        add: function (a, c, d, e, g) {
            var h, i, j, k, l, m, n, o, p, r, s;
            if (3 !== a.nodeType && 8 !== a.nodeType && c && d && (h = f._data(a))) {
                for (d.handler && (p = d, d = p.handler, g = p.selector), d.guid || (d.guid = f.guid++), j = h.events, j || (h.events = j = {}), i = h.handle, i || (h.handle = i = function (a) {
                    return void 0 === f || a && f.event.triggered === a.type ? b : f.event.dispatch.apply(i.elem, arguments)
                }, i.elem = a), c = f.trim(I(c)).split(" "), k = 0; c.length > k; k++) l = A.exec(c[k]) || [], m = l[1], n = (l[2] || "").split(".").sort(), s = f.event.special[m] || {}, m = (g ? s.delegateType : s.bindType) || m, s = f.event.special[m] || {}, o = f.extend({
                    type: m,
                    origType: l[1],
                    data: e,
                    handler: d,
                    guid: d.guid,
                    selector: g,
                    quick: g && G(g),
                    namespace: n.join(".")
                }, p), r = j[m], r || (r = j[m] = [], r.delegateCount = 0, s.setup && s.setup.call(a, e, n, i) !== !1 || (a.addEventListener ? a.addEventListener(m, i, !1) : a.attachEvent && a.attachEvent("on" + m, i))), s.add && (s.add.call(a, o), o.handler.guid || (o.handler.guid = d.guid)), g ? r.splice(r.delegateCount++, 0, o) : r.push(o), f.event.global[m] = !0;
                a = null
            }
        },
        global: {},
        remove: function (a, b, c, d, e) {
            var h, i, j, k, l, m, n, o, p, q, r, s, g = f.hasData(a) && f._data(a);
            if (g && (o = g.events)) {
                for (b = f.trim(I(b || "")).split(" "), h = 0; b.length > h; h++) if (i = A.exec(b[h]) || [], j = k = i[1], l = i[2], j) {
                    for (p = f.event.special[j] || {}, j = (d ? p.delegateType : p.bindType) || j, r = o[j] || [], m = r.length, l = l ? RegExp("(^|\\.)" + l.split(".").sort().join("\\.(?:.*\\.)?") + "(\\.|$)") : null, n = 0; r.length > n; n++) s = r[n], !(!e && k !== s.origType || c && c.guid !== s.guid || l && !l.test(s.namespace) || d && d !== s.selector && ("**" !== d || !s.selector) || (r.splice(n--, 1), s.selector && r.delegateCount--, !p.remove || !p.remove.call(a, s)));
                    0 === r.length && m !== r.length && ((!p.teardown || p.teardown.call(a, l) === !1) && f.removeEvent(a, j, g.handle), delete o[j])
                } else for (j in o) f.event.remove(a, j + b[h], c, d, !0);
                f.isEmptyObject(o) && (q = g.handle, q && (q.elem = null), f.removeData(a, ["events", "handle"], !0))
            }
        },
        customEvent: {
            getData: !0,
            setData: !0,
            changeData: !0
        },
        trigger: function (c, d, e, g) {
            if (!e || 3 !== e.nodeType && 8 !== e.nodeType) {
                var j, k, l, m, n, o, p, q, r, s, h = c.type || c,
                    i = [];
                if (E.test(h + f.event.triggered)) return;
                if (h.indexOf("!") >= 0 && (h = h.slice(0, -1), k = !0), h.indexOf(".") >= 0 && (i = h.split("."), h = i.shift(), i.sort()), (!e || f.event.customEvent[h]) && !f.event.global[h]) return;
                if (c = "object" == typeof c ? c[f.expando] ? c : new f.Event(h, c) : new f.Event(h), c.type = h, c.isTrigger = !0, c.exclusive = k, c.namespace = i.join("."), c.namespace_re = c.namespace ? RegExp("(^|\\.)" + i.join("\\.(?:.*\\.)?") + "(\\.|$)") : null, o = 0 > h.indexOf(":") ? "on" + h : "", !e) {
                    j = f.cache;
                    for (l in j) j[l].events && j[l].events[h] && f.event.trigger(c, d, j[l].handle.elem, !0);
                    return
                }
                if (c.result = b, c.target || (c.target = e), d = null != d ? f.makeArray(d) : [], d.unshift(c), p = f.event.special[h] || {}, p.trigger && p.trigger.apply(e, d) === !1) return;
                if (r = [
                        [e, p.bindType || h]
                    ], !g && !p.noBubble && !f.isWindow(e)) {
                    for (s = p.delegateType || h, m = E.test(s + h) ? e : e.parentNode, n = null; m; m = m.parentNode) r.push([m, s]), n = m;
                    n && n === e.ownerDocument && r.push([n.defaultView || n.parentWindow || a, s])
                }
                for (l = 0; r.length > l && !c.isPropagationStopped(); l++) m = r[l][0], c.type = r[l][1], q = (f._data(m, "events") || {})[c.type] && f._data(m, "handle"), q && q.apply(m, d), q = o && m[o], q && f.acceptData(m) && q.apply(m, d) === !1 && c.preventDefault();
                return c.type = h, !(g || c.isDefaultPrevented() || p._default && p._default.apply(e.ownerDocument, d) !== !1 || "click" === h && f.nodeName(e, "a") || !f.acceptData(e) || !o || !e[h] || ("focus" === h || "blur" === h) && 0 === c.target.offsetWidth || f.isWindow(e) || (n = e[o], n && (e[o] = null), f.event.triggered = h, e[h](), f.event.triggered = b, !n || !(e[o] = n))), c.result
            }
        },
        dispatch: function (c) {
            c = f.event.fix(c || a.event);
            var k, l, m, n, o, p, q, r, s, t, d = (f._data(this, "events") || {})[c.type] || [],
                e = d.delegateCount,
                g = [].slice.call(arguments, 0),
                h = !c.exclusive && !c.namespace,
                i = f.event.special[c.type] || {},
                j = [];
            if (g[0] = c, c.delegateTarget = this, !i.preDispatch || i.preDispatch.call(this, c) !== !1) {
                if (e && (!c.button || "click" !== c.type)) for (n = f(this), n.context = this.ownerDocument || this, m = c.target; m != this; m = m.parentNode || this) if (m.disabled !== !0) {
                    for (p = {}, r = [], n[0] = m, k = 0; e > k; k++) s = d[k], t = s.selector, p[t] === b && (p[t] = s.quick ? H(m, s.quick) : n.is(t)), p[t] && r.push(s);
                    r.length && j.push({
                        elem: m,
                        matches: r
                    })
                }
                for (d.length > e && j.push({
                    elem: this,
                    matches: d.slice(e)
                }), k = 0; j.length > k && !c.isPropagationStopped(); k++) for (q = j[k], c.currentTarget = q.elem, l = 0; q.matches.length > l && !c.isImmediatePropagationStopped(); l++) s = q.matches[l], (h || !c.namespace && !s.namespace || c.namespace_re && c.namespace_re.test(s.namespace)) && (c.data = s.data, c.handleObj = s, o = ((f.event.special[s.origType] || {}).handle || s.handler).apply(q.elem, g), o !== b && (c.result = o, o === !1 && (c.preventDefault(), c.stopPropagation())));
                return i.postDispatch && i.postDispatch.call(this, c), c.result
            }
        },
        props: "attrChange attrName relatedNode srcElement altKey bubbles cancelable ctrlKey currentTarget eventPhase metaKey relatedTarget shiftKey target timeStamp view which".split(" "),
        fixHooks: {},
        keyHooks: {
            props: "char charCode key keyCode".split(" "),
            filter: function (a, b) {
                return null == a.which && (a.which = null != b.charCode ? b.charCode : b.keyCode), a
            }
        },
        mouseHooks: {
            props: "button buttons clientX clientY fromElement offsetX offsetY pageX pageY screenX screenY toElement".split(" "),
            filter: function (a, d) {
                var e, f, g, h = d.button,
                    i = d.fromElement;
                return null == a.pageX && null != d.clientX && (e = a.target.ownerDocument || c, f = e.documentElement, g = e.body, a.pageX = d.clientX + (f && f.scrollLeft || g && g.scrollLeft || 0) - (f && f.clientLeft || g && g.clientLeft || 0), a.pageY = d.clientY + (f && f.scrollTop || g && g.scrollTop || 0) - (f && f.clientTop || g && g.clientTop || 0)), !a.relatedTarget && i && (a.relatedTarget = i === a.target ? d.toElement : i), !a.which && h !== b && (a.which = 1 & h ? 1 : 2 & h ? 3 : 4 & h ? 2 : 0), a
            }
        },
        fix: function (a) {
            if (a[f.expando]) return a;
            var d, e, g = a,
                h = f.event.fixHooks[a.type] || {},
                i = h.props ? this.props.concat(h.props) : this.props;
            for (a = f.Event(g), d = i.length; d;) e = i[--d], a[e] = g[e];
            return a.target || (a.target = g.srcElement || c), 3 === a.target.nodeType && (a.target = a.target.parentNode), a.metaKey === b && (a.metaKey = a.ctrlKey), h.filter ? h.filter(a, g) : a
        },
        special: {
            ready: {
                setup: f.bindReady
            },
            load: {
                noBubble: !0
            },
            focus: {
                delegateType: "focusin"
            },
            blur: {
                delegateType: "focusout"
            },
            beforeunload: {
                setup: function (a, b, c) {
                    f.isWindow(this) && (this.onbeforeunload = c)
                },
                teardown: function (a, b) {
                    this.onbeforeunload === b && (this.onbeforeunload = null)
                }
            }
        },
        simulate: function (a, b, c, d) {
            var e = f.extend(new f.Event, c, {
                type: a,
                isSimulated: !0,
                originalEvent: {}
            });
            d ? f.event.trigger(e, null, b) : f.event.dispatch.call(b, e), e.isDefaultPrevented() && c.preventDefault()
        }
    }, f.event.handle = f.event.dispatch, f.removeEvent = c.removeEventListener ?
        function (a, b, c) {
            a.removeEventListener && a.removeEventListener(b, c, !1)
        } : function (a, b, c) {
        a.detachEvent && a.detachEvent("on" + b, c)
    }, f.Event = function (a, b) {
        return this instanceof f.Event ? (a && a.type ? (this.originalEvent = a, this.type = a.type, this.isDefaultPrevented = a.defaultPrevented || a.returnValue === !1 || a.getPreventDefault && a.getPreventDefault() ? K : J) : this.type = a, b && f.extend(this, b), this.timeStamp = a && a.timeStamp || f.now(), this[f.expando] = !0, void 0) : new f.Event(a, b)
    }, f.Event.prototype = {
        preventDefault: function () {
            this.isDefaultPrevented = K;
            var a = this.originalEvent;
            !a || (a.preventDefault ? a.preventDefault() : a.returnValue = !1)
        },
        stopPropagation: function () {
            this.isPropagationStopped = K;
            var a = this.originalEvent;
            !a || (a.stopPropagation && a.stopPropagation(), a.cancelBubble = !0)
        },
        stopImmediatePropagation: function () {
            this.isImmediatePropagationStopped = K, this.stopPropagation()
        },
        isDefaultPrevented: J,
        isPropagationStopped: J,
        isImmediatePropagationStopped: J
    }, f.each({
        mouseenter: "mouseover",
        mouseleave: "mouseout"
    }, function (a, b) {
        f.event.special[a] = {
            delegateType: b,
            bindType: b,
            handle: function (a) {
                var h, c = this,
                    d = a.relatedTarget,
                    e = a.handleObj;
                return e.selector, (!d || d !== c && !f.contains(c, d)) && (a.type = e.origType, h = e.handler.apply(this, arguments), a.type = b), h
            }
        }
    }), f.support.submitBubbles || (f.event.special.submit = {
        setup: function () {
            return f.nodeName(this, "form") ? !1 : (f.event.add(this, "click._submit keypress._submit", function (a) {
                var c = a.target,
                    d = f.nodeName(c, "input") || f.nodeName(c, "button") ? c.form : b;
                d && !d._submit_attached && (f.event.add(d, "submit._submit", function (a) {
                    a._submit_bubble = !0
                }), d._submit_attached = !0)
            }), void 0)
        },
        postDispatch: function (a) {
            a._submit_bubble && (delete a._submit_bubble, this.parentNode && !a.isTrigger && f.event.simulate("submit", this.parentNode, a, !0))
        },
        teardown: function () {
            return f.nodeName(this, "form") ? !1 : (f.event.remove(this, "._submit"), void 0)
        }
    }), f.support.changeBubbles || (f.event.special.change = {
        setup: function () {
            return z.test(this.nodeName) ? (("checkbox" === this.type || "radio" === this.type) && (f.event.add(this, "propertychange._change", function (a) {
                "checked" === a.originalEvent.propertyName && (this._just_changed = !0)
            }), f.event.add(this, "click._change", function (a) {
                this._just_changed && !a.isTrigger && (this._just_changed = !1, f.event.simulate("change", this, a, !0))
            })), !1) : (f.event.add(this, "beforeactivate._change", function (a) {
                var b = a.target;
                z.test(b.nodeName) && !b._change_attached && (f.event.add(b, "change._change", function (a) {
                    this.parentNode && !a.isSimulated && !a.isTrigger && f.event.simulate("change", this.parentNode, a, !0)
                }), b._change_attached = !0)
            }), void 0)
        },
        handle: function (a) {
            var b = a.target;
            return this !== b || a.isSimulated || a.isTrigger || "radio" !== b.type && "checkbox" !== b.type ? a.handleObj.handler.apply(this, arguments) : void 0
        },
        teardown: function () {
            return f.event.remove(this, "._change"), z.test(this.nodeName)
        }
    }), f.support.focusinBubbles || f.each({
        focus: "focusin",
        blur: "focusout"
    }, function (a, b) {
        var d = 0,
            e = function (a) {
                f.event.simulate(b, a.target, f.event.fix(a), !0)
            };
        f.event.special[b] = {
            setup: function () {
                0 === d++ && c.addEventListener(a, e, !0)
            },
            teardown: function () {
                0 === --d && c.removeEventListener(a, e, !0)
            }
        }
    }), f.fn.extend({
        on: function (a, c, d, e, g) {
            var h, i;
            if ("object" == typeof a) {
                "string" != typeof c && (d = d || c, c = b);
                for (i in a) this.on(i, c, d, a[i], g);
                return this
            }
            if (null == d && null == e ? (e = c, d = c = b) : null == e && ("string" == typeof c ? (e = d, d = b) : (e = d, d = c, c = b)), e === !1) e = J;
            else if (!e) return this;
            return 1 === g && (h = e, e = function (a) {
                return f().off(a), h.apply(this, arguments)
            }, e.guid = h.guid || (h.guid = f.guid++)), this.each(function () {
                f.event.add(this, a, e, d, c)
            })
        },
        one: function (a, b, c, d) {
            return this.on(a, b, c, d, 1)
        },
        off: function (a, c, d) {
            if (a && a.preventDefault && a.handleObj) {
                var e = a.handleObj;
                return f(a.delegateTarget).off(e.namespace ? e.origType + "." + e.namespace : e.origType, e.selector, e.handler), this
            }
            if ("object" == typeof a) {
                for (var g in a) this.off(g, c, a[g]);
                return this
            }
            return (c === !1 || "function" == typeof c) && (d = c, c = b), d === !1 && (d = J), this.each(function () {
                f.event.remove(this, a, d, c)
            })
        },
        bind: function (a, b, c) {
            return this.on(a, null, b, c)
        },
        unbind: function (a, b) {
            return this.off(a, null, b)
        },
        live: function (a, b, c) {
            return f(this.context).on(a, this.selector, b, c), this
        },
        die: function (a, b) {
            return f(this.context).off(a, this.selector || "**", b), this
        },
        delegate: function (a, b, c, d) {
            return this.on(b, a, c, d)
        },
        undelegate: function (a, b, c) {
            return 1 == arguments.length ? this.off(a, "**") : this.off(b, a, c)
        },
        trigger: function (a, b) {
            return this.each(function () {
                f.event.trigger(a, b, this)
            })
        },
        triggerHandler: function (a, b) {
            return this[0] ? f.event.trigger(a, b, this[0], !0) : void 0
        },
        toggle: function (a) {
            var b = arguments,
                c = a.guid || f.guid++,
                d = 0,
                e = function (c) {
                    var e = (f._data(this, "lastToggle" + a.guid) || 0) % d;
                    return f._data(this, "lastToggle" + a.guid, e + 1), c.preventDefault(), b[e].apply(this, arguments) || !1
                };
            for (e.guid = c; b.length > d;) b[d++].guid = c;
            return this.click(e)
        },
        hover: function (a, b) {
            return this.mouseenter(a).mouseleave(b || a)
        }
    }), f.each("blur focus focusin focusout load resize scroll unload click dblclick mousedown mouseup mousemove mouseover mouseout mouseenter mouseleave change select submit keydown keypress keyup error contextmenu".split(" "), function (a, b) {
        f.fn[b] = function (a, c) {
            return null == c && (c = a, a = null), arguments.length > 0 ? this.on(b, null, a, c) : this.trigger(b)
        }, f.attrFn && (f.attrFn[b] = !0), C.test(b) && (f.event.fixHooks[b] = f.event.keyHooks), D.test(b) && (f.event.fixHooks[b] = f.event.mouseHooks)
    }), function () {
        function x(a, b, c, e, f, g) {
            for (var h = 0, i = e.length; i > h; h++) {
                var j = e[h];
                if (j) {
                    var k = !1;
                    for (j = j[a]; j;) {
                        if (j[d] === c) {
                            k = e[j.sizset];
                            break
                        }
                        if (1 === j.nodeType) if (g || (j[d] = c, j.sizset = h), "string" != typeof b) {
                            if (j === b) {
                                k = !0;
                                break
                            }
                        } else if (m.filter(b, [j]).length > 0) {
                            k = j;
                            break
                        }
                        j = j[a]
                    }
                    e[h] = k
                }
            }
        }

        function w(a, b, c, e, f, g) {
            for (var h = 0, i = e.length; i > h; h++) {
                var j = e[h];
                if (j) {
                    var k = !1;
                    for (j = j[a]; j;) {
                        if (j[d] === c) {
                            k = e[j.sizset];
                            break
                        }
                        if (1 === j.nodeType && !g && (j[d] = c, j.sizset = h), j.nodeName.toLowerCase() === b) {
                            k = j;
                            break
                        }
                        j = j[a]
                    }
                    e[h] = k
                }
            }
        }

        var a = /((?:\((?:\([^()]+\)|[^()]+)+\)|\[(?:\[[^\[\]]*\]|['"][^'"]*['"]|[^\[\]'"]+)+\]|\\.|[^ >+~,(\[\\]+)+|[>+~])(\s*,\s*)?((?:.|\r|\n)*)/g,
            d = "sizcache" + (Math.random() + "").replace(".", ""),
            e = 0,
            g = Object.prototype.toString,
            h = !1,
            i = !0,
            j = /\\/g,
            k = /\r\n/g,
            l = /\W/;
        [0, 0].sort(function () {
            return i = !1, 0
        });
        var m = function (b, d, e, f) {
            e = e || [], d = d || c;
            var h = d;
            if (1 !== d.nodeType && 9 !== d.nodeType) return [];
            if (!b || "string" != typeof b) return e;
            var i, j, k, l, n, q, r, t, u = !0,
                v = m.isXML(d),
                w = [],
                x = b;
            do
                if (a.exec(""), i = a.exec(x), i && (x = i[3], w.push(i[1]), i[2])) {
                    l = i[3];
                    break
                }
            while (i);
            if (w.length > 1 && p.exec(b)) if (2 === w.length && o.relative[w[0]]) j = y(w[0] + w[1], d, f);
            else for (j = o.relative[w[0]] ? [d] : m(w.shift(), d); w.length;) b = w.shift(), o.relative[b] && (b += w.shift()), j = y(b, j, f);
            else if (!f && w.length > 1 && 9 === d.nodeType && !v && o.match.ID.test(w[0]) && !o.match.ID.test(w[w.length - 1]) && (n = m.find(w.shift(), d, v), d = n.expr ? m.filter(n.expr, n.set)[0] : n.set[0]), d) for (n = f ? {
                expr: w.pop(),
                set: s(f)
            } : m.find(w.pop(), 1 !== w.length || "~" !== w[0] && "+" !== w[0] || !d.parentNode ? d : d.parentNode, v), j = n.expr ? m.filter(n.expr, n.set) : n.set, w.length > 0 ? k = s(j) : u = !1; w.length;) q = w.pop(), r = q, o.relative[q] ? r = w.pop() : q = "", null == r && (r = d), o.relative[q](k, r, v);
            else k = w = [];
            if (k || (k = j), k || m.error(q || b), "[object Array]" === g.call(k)) if (u) if (d && 1 === d.nodeType) for (t = 0; null != k[t]; t++) k[t] && (k[t] === !0 || 1 === k[t].nodeType && m.contains(d, k[t])) && e.push(j[t]);
            else for (t = 0; null != k[t]; t++) k[t] && 1 === k[t].nodeType && e.push(j[t]);
            else e.push.apply(e, k);
            else s(k, e);
            return l && (m(l, h, e, f), m.uniqueSort(e)), e
        };
        m.uniqueSort = function (a) {
            if (u && (h = i, a.sort(u), h)) for (var b = 1; a.length > b; b++) a[b] === a[b - 1] && a.splice(b--, 1);
            return a
        }, m.matches = function (a, b) {
            return m(a, null, null, b)
        }, m.matchesSelector = function (a, b) {
            return m(b, null, null, [a]).length > 0
        }, m.find = function (a, b, c) {
            var d, e, f, g, h, i;
            if (!a) return [];
            for (e = 0, f = o.order.length; f > e; e++) if (h = o.order[e], (g = o.leftMatch[h].exec(a)) && (i = g[1], g.splice(1, 1), "\\" !== i.substr(i.length - 1) && (g[1] = (g[1] || "").replace(j, ""), d = o.find[h](g, b, c), null != d))) {
                a = a.replace(o.match[h], "");
                break
            }
            return d || (d = b.getElementsByTagName !== void 0 ? b.getElementsByTagName("*") : []), {
                set: d,
                expr: a
            }
        }, m.filter = function (a, c, d, e) {
            for (var f, g, h, i, j, k, l, n, p, q = a, r = [], s = c, t = c && c[0] && m.isXML(c[0]); a && c.length;) {
                for (h in o.filter) if (null != (f = o.leftMatch[h].exec(a)) && f[2]) {
                    if (k = o.filter[h], l = f[1], g = !1, f.splice(1, 1), "\\" === l.substr(l.length - 1)) continue;
                    if (s === r && (r = []), o.preFilter[h]) if (f = o.preFilter[h](f, s, d, r, e, t)) {
                        if (f === !0) continue
                    } else g = i = !0;
                    if (f) for (n = 0; null != (j = s[n]); n++) j && (i = k(j, f, n, s), p = e ^ i, d && null != i ? p ? g = !0 : s[n] = !1 : p && (r.push(j), g = !0));
                    if (i !== b) {
                        if (d || (s = r), a = a.replace(o.match[h], ""), !g) return [];
                        break
                    }
                }
                if (a === q) {
                    if (null != g) break;
                    m.error(a)
                }
                q = a
            }
            return s
        }, m.error = function (a) {
            throw Error("Syntax error, unrecognized expression: " + a)
        };
        var n = m.getText = function (a) {
                var b, c, d = a.nodeType,
                    e = "";
                if (d) {
                    if (1 === d || 9 === d || 11 === d) {
                        if ("string" == typeof a.textContent) return a.textContent;
                        if ("string" == typeof a.innerText) return a.innerText.replace(k, "");
                        for (a = a.firstChild; a; a = a.nextSibling) e += n(a)
                    } else if (3 === d || 4 === d) return a.nodeValue
                } else for (b = 0; c = a[b]; b++) 8 !== c.nodeType && (e += n(c));
                return e
            },
            o = m.selectors = {
                order: ["ID", "NAME", "TAG"],
                match: {
                    ID: /#((?:[\wÀ-￿\-]|\\.)+)/,
                    CLASS: /\.((?:[\wÀ-￿\-]|\\.)+)/,
                    NAME: /\[name=['"]*((?:[\wÀ-￿\-]|\\.)+)['"]*\]/,
                    ATTR: /\[\s*((?:[\wÀ-￿\-]|\\.)+)\s*(?:(\S?=)\s*(?:(['"])(.*?)\3|(#?(?:[\wÀ-￿\-]|\\.)*)|)|)\s*\]/,
                    TAG: /^((?:[\wÀ-￿\*\-]|\\.)+)/,
                    CHILD: /:(only|nth|last|first)-child(?:\(\s*(even|odd|(?:[+\-]?\d+|(?:[+\-]?\d*)?n\s*(?:[+\-]\s*\d+)?))\s*\))?/,
                    POS: /:(nth|eq|gt|lt|first|last|even|odd)(?:\((\d*)\))?(?=[^\-]|$)/,
                    PSEUDO: /:((?:[\wÀ-￿\-]|\\.)+)(?:\((['"]?)((?:\([^\)]+\)|[^\(\)]*)+)\2\))?/
                },
                leftMatch: {},
                attrMap: {
                    "class": "className",
                    "for": "htmlFor"
                },
                attrHandle: {
                    href: function (a) {
                        return a.getAttribute("href")
                    },
                    type: function (a) {
                        return a.getAttribute("type")
                    }
                },
                relative: {
                    "+": function (a, b) {
                        var c = "string" == typeof b,
                            d = c && !l.test(b),
                            e = c && !d;
                        d && (b = b.toLowerCase());
                        for (var h, f = 0, g = a.length; g > f; f++) if (h = a[f]) {
                            for (;
                                (h = h.previousSibling) && 1 !== h.nodeType;);
                            a[f] = e || h && h.nodeName.toLowerCase() === b ? h || !1 : h === b
                        }
                        e && m.filter(b, a, !0)
                    },
                    ">": function (a, b) {
                        var c, d = "string" == typeof b,
                            e = 0,
                            f = a.length;
                        if (d && !l.test(b)) {
                            for (b = b.toLowerCase(); f > e; e++) if (c = a[e]) {
                                var g = c.parentNode;
                                a[e] = g.nodeName.toLowerCase() === b ? g : !1
                            }
                        } else {
                            for (; f > e; e++) c = a[e], c && (a[e] = d ? c.parentNode : c.parentNode === b);
                            d && m.filter(b, a, !0)
                        }
                    },
                    "": function (a, b, c) {
                        var d, f = e++,
                            g = x;
                        "string" == typeof b && !l.test(b) && (b = b.toLowerCase(), d = b, g = w), g("parentNode", b, f, a, d, c)
                    },
                    "~": function (a, b, c) {
                        var d, f = e++,
                            g = x;
                        "string" == typeof b && !l.test(b) && (b = b.toLowerCase(), d = b, g = w), g("previousSibling", b, f, a, d, c)
                    }
                },
                find: {
                    ID: function (a, b, c) {
                        if (b.getElementById !== void 0 && !c) {
                            var d = b.getElementById(a[1]);
                            return d && d.parentNode ? [d] : []
                        }
                    },
                    NAME: function (a, b) {
                        if (b.getElementsByName !== void 0) {
                            for (var c = [], d = b.getElementsByName(a[1]), e = 0, f = d.length; f > e; e++) d[e].getAttribute("name") === a[1] && c.push(d[e]);
                            return 0 === c.length ? null : c
                        }
                    },
                    TAG: function (a, b) {
                        return b.getElementsByTagName !== void 0 ? b.getElementsByTagName(a[1]) : void 0
                    }
                },
                preFilter: {
                    CLASS: function (a, b, c, d, e, f) {
                        if (a = " " + a[1].replace(j, "") + " ", f) return a;
                        for (var h, g = 0; null != (h = b[g]); g++) h && (e ^ (h.className && (" " + h.className + " ").replace(/[\t\n\r]/g, " ").indexOf(a) >= 0) ? c || d.push(h) : c && (b[g] = !1));
                        return !1
                    },
                    ID: function (a) {
                        return a[1].replace(j, "")
                    },
                    TAG: function (a) {
                        return a[1].replace(j, "").toLowerCase()
                    },
                    CHILD: function (a) {
                        if ("nth" === a[1]) {
                            a[2] || m.error(a[0]), a[2] = a[2].replace(/^\+|\s*/g, "");
                            var b = /(-?)(\d*)(?:n([+\-]?\d*))?/.exec("even" === a[2] && "2n" || "odd" === a[2] && "2n+1" || !/\D/.test(a[2]) && "0n+" + a[2] || a[2]);
                            a[2] = b[1] + (b[2] || 1) - 0, a[3] = b[3] - 0
                        } else a[2] && m.error(a[0]);
                        return a[0] = e++, a
                    },
                    ATTR: function (a, b, c, d, e, f) {
                        var g = a[1] = a[1].replace(j, "");
                        return !f && o.attrMap[g] && (a[1] = o.attrMap[g]), a[4] = (a[4] || a[5] || "").replace(j, ""), "~=" === a[2] && (a[4] = " " + a[4] + " "), a
                    },
                    PSEUDO: function (b, c, d, e, f) {
                        if ("not" === b[1]) {
                            if (!((a.exec(b[3]) || "").length > 1 || /^\w/.test(b[3]))) {
                                var g = m.filter(b[3], c, d, !0 ^ f);
                                return d || e.push.apply(e, g), !1
                            }
                            b[3] = m(b[3], null, null, c)
                        } else if (o.match.POS.test(b[0]) || o.match.CHILD.test(b[0])) return !0;
                        return b
                    },
                    POS: function (a) {
                        return a.unshift(!0), a
                    }
                },
                filters: {
                    enabled: function (a) {
                        return a.disabled === !1 && "hidden" !== a.type
                    },
                    disabled: function (a) {
                        return a.disabled === !0
                    },
                    checked: function (a) {
                        return a.checked === !0
                    },
                    selected: function (a) {
                        return a.parentNode && a.parentNode.selectedIndex, a.selected === !0
                    },
                    parent: function (a) {
                        return !!a.firstChild
                    },
                    empty: function (a) {
                        return !a.firstChild
                    },
                    has: function (a, b, c) {
                        return !!m(c[3], a).length
                    },
                    header: function (a) {
                        return /h\d/i.test(a.nodeName)
                    },
                    text: function (a) {
                        var b = a.getAttribute("type"),
                            c = a.type;
                        return "input" === a.nodeName.toLowerCase() && "text" === c && (b === c || null === b)
                    },
                    radio: function (a) {
                        return "input" === a.nodeName.toLowerCase() && "radio" === a.type
                    },
                    checkbox: function (a) {
                        return "input" === a.nodeName.toLowerCase() && "checkbox" === a.type
                    },
                    file: function (a) {
                        return "input" === a.nodeName.toLowerCase() && "file" === a.type
                    },
                    password: function (a) {
                        return "input" === a.nodeName.toLowerCase() && "password" === a.type
                    },
                    submit: function (a) {
                        var b = a.nodeName.toLowerCase();
                        return ("input" === b || "button" === b) && "submit" === a.type
                    },
                    image: function (a) {
                        return "input" === a.nodeName.toLowerCase() && "image" === a.type
                    },
                    reset: function (a) {
                        var b = a.nodeName.toLowerCase();
                        return ("input" === b || "button" === b) && "reset" === a.type
                    },
                    button: function (a) {
                        var b = a.nodeName.toLowerCase();
                        return "input" === b && "button" === a.type || "button" === b
                    },
                    input: function (a) {
                        return /input|select|textarea|button/i.test(a.nodeName)
                    },
                    focus: function (a) {
                        return a === a.ownerDocument.activeElement
                    }
                },
                setFilters: {
                    first: function (a, b) {
                        return 0 === b
                    },
                    last: function (a, b, c, d) {
                        return b === d.length - 1
                    },
                    even: function (a, b) {
                        return 0 === b % 2
                    },
                    odd: function (a, b) {
                        return 1 === b % 2
                    },
                    lt: function (a, b, c) {
                        return c[3] - 0 > b
                    },
                    gt: function (a, b, c) {
                        return b > c[3] - 0
                    },
                    nth: function (a, b, c) {
                        return c[3] - 0 === b
                    },
                    eq: function (a, b, c) {
                        return c[3] - 0 === b
                    }
                },
                filter: {
                    PSEUDO: function (a, b, c, d) {
                        var e = b[1],
                            f = o.filters[e];
                        if (f) return f(a, c, b, d);
                        if ("contains" === e) return (a.textContent || a.innerText || n([a]) || "").indexOf(b[3]) >= 0;
                        if ("not" === e) {
                            for (var g = b[3], h = 0, i = g.length; i > h; h++) if (g[h] === a) return !1;
                            return !0
                        }
                        m.error(e)
                    },
                    CHILD: function (a, b) {
                        var c, e, f, g, i, j, k = b[1],
                            l = a;
                        switch (k) {
                            case "only":
                            case "first":
                                for (; l = l.previousSibling;) if (1 === l.nodeType) return !1;
                                if ("first" === k) return !0;
                                l = a;
                            case "last":
                                for (; l = l.nextSibling;) if (1 === l.nodeType) return !1;
                                return !0;
                            case "nth":
                                if (c = b[2], e = b[3], 1 === c && 0 === e) return !0;
                                if (f = b[0], g = a.parentNode, g && (g[d] !== f || !a.nodeIndex)) {
                                    for (i = 0, l = g.firstChild; l; l = l.nextSibling) 1 === l.nodeType && (l.nodeIndex = ++i);
                                    g[d] = f
                                }
                                return j = a.nodeIndex - e, 0 === c ? 0 === j : 0 === j % c && j / c >= 0
                        }
                    },
                    ID: function (a, b) {
                        return 1 === a.nodeType && a.getAttribute("id") === b
                    },
                    TAG: function (a, b) {
                        return "*" === b && 1 === a.nodeType || !!a.nodeName && a.nodeName.toLowerCase() === b
                    },
                    CLASS: function (a, b) {
                        return (" " + (a.className || a.getAttribute("class")) + " ").indexOf(b) > -1
                    },
                    ATTR: function (a, b) {
                        var c = b[1],
                            d = m.attr ? m.attr(a, c) : o.attrHandle[c] ? o.attrHandle[c](a) : null != a[c] ? a[c] : a.getAttribute(c),
                            e = d + "",
                            f = b[2],
                            g = b[4];
                        return null == d ? "!=" === f : !f && m.attr ? null != d : "=" === f ? e === g : "*=" === f ? e.indexOf(g) >= 0 : "~=" === f ? (" " + e + " ").indexOf(g) >= 0 : g ? "!=" === f ? e !== g : "^=" === f ? 0 === e.indexOf(g) : "$=" === f ? e.substr(e.length - g.length) === g : "|=" === f ? e === g || e.substr(0, g.length + 1) === g + "-" : !1 : e && d !== !1
                    },
                    POS: function (a, b, c, d) {
                        var e = b[2],
                            f = o.setFilters[e];
                        return f ? f(a, c, b, d) : void 0
                    }
                }
            },
            p = o.match.POS,
            q = function (a, b) {
                return "\\" + (b - 0 + 1)
            };
        for (var r in o.match) o.match[r] = RegExp(o.match[r].source + /(?![^\[]*\])(?![^\(]*\))/.source), o.leftMatch[r] = RegExp(/(^(?:.|\r|\n)*?)/.source + o.match[r].source.replace(/\\(\d+)/g, q));
        o.match.globalPOS = p;
        var s = function (a, b) {
            return a = Array.prototype.slice.call(a, 0), b ? (b.push.apply(b, a), b) : a
        };
        try {
            Array.prototype.slice.call(c.documentElement.childNodes, 0)[0].nodeType
        } catch (t) {
            s = function (a, b) {
                var c = 0,
                    d = b || [];
                if ("[object Array]" === g.call(a)) Array.prototype.push.apply(d, a);
                else if ("number" == typeof a.length) for (var e = a.length; e > c; c++) d.push(a[c]);
                else for (; a[c]; c++) d.push(a[c]);
                return d
            }
        }
        var u, v;
        c.documentElement.compareDocumentPosition ? u = function (a, b) {
            return a === b ? (h = !0, 0) : a.compareDocumentPosition && b.compareDocumentPosition ? 4 & a.compareDocumentPosition(b) ? -1 : 1 : a.compareDocumentPosition ? -1 : 1
        } : (u = function (a, b) {
            if (a === b) return h = !0, 0;
            if (a.sourceIndex && b.sourceIndex) return a.sourceIndex - b.sourceIndex;
            var c, d, e = [],
                f = [],
                g = a.parentNode,
                i = b.parentNode,
                j = g;
            if (g === i) return v(a, b);
            if (!g) return -1;
            if (!i) return 1;
            for (; j;) e.unshift(j), j = j.parentNode;
            for (j = i; j;) f.unshift(j), j = j.parentNode;
            c = e.length, d = f.length;
            for (var k = 0; c > k && d > k; k++) if (e[k] !== f[k]) return v(e[k], f[k]);
            return k === c ? v(a, f[k], -1) : v(e[k], b, 1)
        }, v = function (a, b, c) {
            if (a === b) return c;
            for (var d = a.nextSibling; d;) {
                if (d === b) return -1;
                d = d.nextSibling
            }
            return 1
        }), function () {
            var a = c.createElement("div"),
                d = "script" + (new Date).getTime(),
                e = c.documentElement;
            a.innerHTML = "<a name='" + d + "'/>", e.insertBefore(a, e.firstChild), c.getElementById(d) && (o.find.ID = function (a, c, d) {
                if (c.getElementById !== void 0 && !d) {
                    var e = c.getElementById(a[1]);
                    return e ? e.id === a[1] || e.getAttributeNode !== void 0 && e.getAttributeNode("id").nodeValue === a[1] ? [e] : b : []
                }
            }, o.filter.ID = function (a, b) {
                var c = a.getAttributeNode !== void 0 && a.getAttributeNode("id");
                return 1 === a.nodeType && c && c.nodeValue === b
            }), e.removeChild(a), e = a = null
        }(), function () {
            var a = c.createElement("div");
            a.appendChild(c.createComment("")), a.getElementsByTagName("*").length > 0 && (o.find.TAG = function (a, b) {
                var c = b.getElementsByTagName(a[1]);
                if ("*" === a[1]) {
                    for (var d = [], e = 0; c[e]; e++) 1 === c[e].nodeType && d.push(c[e]);
                    c = d
                }
                return c
            }), a.innerHTML = "<a href='#'></a>", a.firstChild && a.firstChild.getAttribute !== void 0 && "#" !== a.firstChild.getAttribute("href") && (o.attrHandle.href = function (a) {
                return a.getAttribute("href", 2)
            }), a = null
        }(), c.querySelectorAll &&
        function () {
            var a = m,
                b = c.createElement("div"),
                d = "__sizzle__";
            if (b.innerHTML = "<p class='TEST'></p>", !b.querySelectorAll || 0 !== b.querySelectorAll(".TEST").length) {
                m = function (b, e, f, g) {
                    if (e = e || c, !g && !m.isXML(e)) {
                        var h = /^(\w+$)|^\.([\w\-]+$)|^#([\w\-]+$)/.exec(b);
                        if (h && (1 === e.nodeType || 9 === e.nodeType)) {
                            if (h[1]) return s(e.getElementsByTagName(b), f);
                            if (h[2] && o.find.CLASS && e.getElementsByClassName) return s(e.getElementsByClassName(h[2]), f)
                        }
                        if (9 === e.nodeType) {
                            if ("body" === b && e.body) return s([e.body], f);
                            if (h && h[3]) {
                                var i = e.getElementById(h[3]);
                                if (!i || !i.parentNode) return s([], f);
                                if (i.id === h[3]) return s([i], f)
                            }
                            try {
                                return s(e.querySelectorAll(b), f)
                            } catch (j) {
                            }
                        } else if (1 === e.nodeType && "object" !== e.nodeName.toLowerCase()) {
                            var k = e,
                                l = e.getAttribute("id"),
                                n = l || d,
                                p = e.parentNode,
                                q = /^\s*[+~]/.test(b);
                            l ? n = n.replace(/'/g, "\\$&") : e.setAttribute("id", n), q && p && (e = e.parentNode);
                            try {
                                if (!q || p) return s(e.querySelectorAll("[id='" + n + "'] " + b), f)
                            } catch (r) {
                            } finally {
                                l || k.removeAttribute("id")
                            }
                        }
                    }
                    return a(b, e, f, g)
                };
                for (var e in a) m[e] = a[e];
                b = null
            }
        }(), function () {
            var a = c.documentElement,
                b = a.matchesSelector || a.mozMatchesSelector || a.webkitMatchesSelector || a.msMatchesSelector;
            if (b) {
                var d = !b.call(c.createElement("div"), "div"),
                    e = !1;
                try {
                    b.call(c.documentElement, "[test!='']:sizzle")
                } catch (f) {
                    e = !0
                }
                m.matchesSelector = function (a, c) {
                    if (c = c.replace(/\=\s*([^'"\]]*)\s*\]/g, "='$1']"), !m.isXML(a)) try {
                        if (e || !o.match.PSEUDO.test(c) && !/!=/.test(c)) {
                            var f = b.call(a, c);
                            if (f || !d || a.document && 11 !== a.document.nodeType) return f
                        }
                    } catch (g) {
                    }
                    return m(c, null, null, [a]).length > 0
                }
            }
        }(), function () {
            var a = c.createElement("div");
            if (a.innerHTML = "<div class='test e'></div><div class='test'></div>", a.getElementsByClassName && 0 !== a.getElementsByClassName("e").length) {
                if (a.lastChild.className = "e", 1 === a.getElementsByClassName("e").length) return;
                o.order.splice(1, 0, "CLASS"), o.find.CLASS = function (a, b, c) {
                    return void 0 === b.getElementsByClassName || c ? void 0 : b.getElementsByClassName(a[1])
                }, a = null
            }
        }(), m.contains = c.documentElement.contains ?
            function (a, b) {
                return a !== b && (a.contains ? a.contains(b) : !0)
            } : c.documentElement.compareDocumentPosition ?
            function (a, b) {
                return !!(16 & a.compareDocumentPosition(b))
            } : function () {
            return !1
        }, m.isXML = function (a) {
            var b = (a ? a.ownerDocument || a : 0).documentElement;
            return b ? "HTML" !== b.nodeName : !1
        };
        var y = function (a, b, c) {
            for (var d, e = [], f = "", g = b.nodeType ? [b] : b; d = o.match.PSEUDO.exec(a);) f += d[0], a = a.replace(o.match.PSEUDO, "");
            a = o.relative[a] ? a + "*" : a;
            for (var h = 0, i = g.length; i > h; h++) m(a, g[h], e, c);
            return m.filter(f, e)
        };
        m.attr = f.attr, m.selectors.attrMap = {}, f.find = m, f.expr = m.selectors, f.expr[":"] = f.expr.filters, f.unique = m.uniqueSort, f.text = m.getText, f.isXMLDoc = m.isXML, f.contains = m.contains
    }();
    var L = /Until$/,
        M = /^(?:parents|prevUntil|prevAll)/,
        N = /,/,
        O = /^.[^:#\[\.,]*$/,
        P = Array.prototype.slice,
        Q = f.expr.match.globalPOS,
        R = {
            children: !0,
            contents: !0,
            next: !0,
            prev: !0
        };
    f.fn.extend({
        find: function (a) {
            var c, d, b = this;
            if ("string" != typeof a) return f(a).filter(function () {
                for (c = 0, d = b.length; d > c; c++) if (f.contains(b[c], this)) return !0
            });
            var g, h, i, e = this.pushStack("", "find", a);
            for (c = 0, d = this.length; d > c; c++) if (g = e.length, f.find(a, this[c], e), c > 0) for (h = g; e.length > h; h++) for (i = 0; g > i; i++) if (e[i] === e[h]) {
                e.splice(h--, 1);
                break
            }
            return e
        },
        has: function (a) {
            var b = f(a);
            return this.filter(function () {
                for (var a = 0, c = b.length; c > a; a++) if (f.contains(this, b[a])) return !0
            })
        },
        not: function (a) {
            return this.pushStack(T(this, a, !1), "not", a)
        },
        filter: function (a) {
            return this.pushStack(T(this, a, !0), "filter", a)
        },
        is: function (a) {
            return !!a && ("string" == typeof a ? Q.test(a) ? f(a, this.context).index(this[0]) >= 0 : f.filter(a, this).length > 0 : this.filter(a).length > 0)
        },
        closest: function (a, b) {
            var d, e, c = [],
                g = this[0];
            if (f.isArray(a)) {
                for (var h = 1; g && g.ownerDocument && g !== b;) {
                    for (d = 0; a.length > d; d++) f(g).is(a[d]) && c.push({
                        selector: a[d],
                        elem: g,
                        level: h
                    });
                    g = g.parentNode, h++
                }
                return c
            }
            var i = Q.test(a) || "string" != typeof a ? f(a, b || this.context) : 0;
            for (d = 0, e = this.length; e > d; d++) for (g = this[d]; g;) {
                if (i ? i.index(g) > -1 : f.find.matchesSelector(g, a)) {
                    c.push(g);
                    break
                }
                if (g = g.parentNode, !g || !g.ownerDocument || g === b || 11 === g.nodeType) break
            }
            return c = c.length > 1 ? f.unique(c) : c, this.pushStack(c, "closest", a)
        },
        index: function (a) {
            return a ? "string" == typeof a ? f.inArray(this[0], f(a)) : f.inArray(a.jquery ? a[0] : a, this) : this[0] && this[0].parentNode ? this.prevAll().length : -1
        },
        add: function (a, b) {
            var c = "string" == typeof a ? f(a, b) : f.makeArray(a && a.nodeType ? [a] : a),
                d = f.merge(this.get(), c);
            return this.pushStack(S(c[0]) || S(d[0]) ? d : f.unique(d))
        },
        andSelf: function () {
            return this.add(this.prevObject)
        }
    }), f.each({
        parent: function (a) {
            var b = a.parentNode;
            return b && 11 !== b.nodeType ? b : null
        },
        parents: function (a) {
            return f.dir(a, "parentNode")
        },
        parentsUntil: function (a, b, c) {
            return f.dir(a, "parentNode", c)
        },
        next: function (a) {
            return f.nth(a, 2, "nextSibling")
        },
        prev: function (a) {
            return f.nth(a, 2, "previousSibling")
        },
        nextAll: function (a) {
            return f.dir(a, "nextSibling")
        },
        prevAll: function (a) {
            return f.dir(a, "previousSibling")
        },
        nextUntil: function (a, b, c) {
            return f.dir(a, "nextSibling", c)
        },
        prevUntil: function (a, b, c) {
            return f.dir(a, "previousSibling", c)
        },
        siblings: function (a) {
            return f.sibling((a.parentNode || {}).firstChild, a)
        },
        children: function (a) {
            return f.sibling(a.firstChild)
        },
        contents: function (a) {
            return f.nodeName(a, "iframe") ? a.contentDocument || a.contentWindow.document : f.makeArray(a.childNodes)
        }
    }, function (a, b) {
        f.fn[a] = function (c, d) {
            var e = f.map(this, b, c);
            return L.test(a) || (d = c), d && "string" == typeof d && (e = f.filter(d, e)), e = this.length > 1 && !R[a] ? f.unique(e) : e, (this.length > 1 || N.test(d)) && M.test(a) && (e = e.reverse()), this.pushStack(e, a, P.call(arguments).join(","))
        }
    }), f.extend({
        filter: function (a, b, c) {
            return c && (a = ":not(" + a + ")"), 1 === b.length ? f.find.matchesSelector(b[0], a) ? [b[0]] : [] : f.find.matches(a, b)
        },
        dir: function (a, c, d) {
            for (var e = [], g = a[c]; g && 9 !== g.nodeType && (d === b || 1 !== g.nodeType || !f(g).is(d));) 1 === g.nodeType && e.push(g), g = g[c];
            return e
        },
        nth: function (a, b, c) {
            b = b || 1;
            for (var e = 0; a && (1 !== a.nodeType || ++e !== b); a = a[c]);
            return a
        },
        sibling: function (a, b) {
            for (var c = []; a; a = a.nextSibling) 1 === a.nodeType && a !== b && c.push(a);
            return c
        }
    });
    var V = "abbr|article|aside|audio|bdi|canvas|data|datalist|details|figcaption|figure|footer|header|hgroup|mark|meter|nav|output|progress|section|summary|time|video",
        W = / jQuery\d+="(?:\d+|null)"/g,
        X = /^\s+/,
        Y = /<(?!area|br|col|embed|hr|img|input|link|meta|param)(([\w:]+)[^>]*)\/>/gi,
        Z = /<([\w:]+)/,
        $ = /<tbody/i,
        _ = /<|&#?\w+;/,
        ba = /<(?:script|style)/i,
        bb = /<(?:script|object|embed|option|style)/i,
        bc = RegExp("<(?:" + V + ")[\\s/>]", "i"),
        bd = /checked\s*(?:[^=]|=\s*.checked.)/i,
        be = /\/(java|ecma)script/i,
        bf = /^\s*<!(?:\[CDATA\[|\-\-)/,
        bg = {
            option: [1, "<select multiple='multiple'>", "</select>"],
            legend: [1, "<fieldset>", "</fieldset>"],
            thead: [1, "<table>", "</table>"],
            tr: [2, "<table><tbody>", "</tbody></table>"],
            td: [3, "<table><tbody><tr>", "</tr></tbody></table>"],
            col: [2, "<table><tbody></tbody><colgroup>", "</colgroup></table>"],
            area: [1, "<map>", "</map>"],
            _default: [0, "", ""]
        },
        bh = U(c);
    bg.optgroup = bg.option, bg.tbody = bg.tfoot = bg.colgroup = bg.caption = bg.thead, bg.th = bg.td, f.support.htmlSerialize || (bg._default = [1, "div<div>", "</div>"]), f.fn.extend({
        text: function (a) {
            return f.access(this, function (a) {
                return a === b ? f.text(this) : this.empty().append((this[0] && this[0].ownerDocument || c).createTextNode(a))
            }, null, a, arguments.length)
        },
        wrapAll: function (a) {
            if (f.isFunction(a)) return this.each(function (b) {
                f(this).wrapAll(a.call(this, b))
            });
            if (this[0]) {
                var b = f(a, this[0].ownerDocument).eq(0).clone(!0);
                this[0].parentNode && b.insertBefore(this[0]), b.map(function () {
                    for (var a = this; a.firstChild && 1 === a.firstChild.nodeType;) a = a.firstChild;
                    return a
                }).append(this)
            }
            return this
        },
        wrapInner: function (a) {
            return f.isFunction(a) ? this.each(function (b) {
                f(this).wrapInner(a.call(this, b))
            }) : this.each(function () {
                var b = f(this),
                    c = b.contents();
                c.length ? c.wrapAll(a) : b.append(a)
            })
        },
        wrap: function (a) {
            var b = f.isFunction(a);
            return this.each(function (c) {
                f(this).wrapAll(b ? a.call(this, c) : a)
            })
        },
        unwrap: function () {
            return this.parent().each(function () {
                f.nodeName(this, "body") || f(this).replaceWith(this.childNodes)
            }).end()
        },
        append: function () {
            return this.domManip(arguments, !0, function (a) {
                1 === this.nodeType && this.appendChild(a)
            })
        },
        prepend: function () {
            return this.domManip(arguments, !0, function (a) {
                1 === this.nodeType && this.insertBefore(a, this.firstChild)
            })
        },
        before: function () {
            if (this[0] && this[0].parentNode) return this.domManip(arguments, !1, function (a) {
                this.parentNode.insertBefore(a, this)
            });
            if (arguments.length) {
                var a = f.clean(arguments);
                return a.push.apply(a, this.toArray()), this.pushStack(a, "before", arguments)
            }
        },
        after: function () {
            if (this[0] && this[0].parentNode) return this.domManip(arguments, !1, function (a) {
                this.parentNode.insertBefore(a, this.nextSibling)
            });
            if (arguments.length) {
                var a = this.pushStack(this, "after", arguments);
                return a.push.apply(a, f.clean(arguments)), a
            }
        },
        remove: function (a, b) {
            for (var d, c = 0; null != (d = this[c]); c++)(!a || f.filter(a, [d]).length) && (!b && 1 === d.nodeType && (f.cleanData(d.getElementsByTagName("*")), f.cleanData([d])), d.parentNode && d.parentNode.removeChild(d));
            return this
        },
        empty: function () {
            for (var b, a = 0; null != (b = this[a]); a++) for (1 === b.nodeType && f.cleanData(b.getElementsByTagName("*")); b.firstChild;) b.removeChild(b.firstChild);
            return this
        },
        clone: function (a, b) {
            return a = null == a ? !1 : a, b = null == b ? a : b, this.map(function () {
                return f.clone(this, a, b)
            })
        },
        html: function (a) {
            return f.access(this, function (a) {
                var c = this[0] || {},
                    d = 0,
                    e = this.length;
                if (a === b) return 1 === c.nodeType ? c.innerHTML.replace(W, "") : null;
                if (!("string" != typeof a || ba.test(a) || !f.support.leadingWhitespace && X.test(a) || bg[(Z.exec(a) || ["", ""])[1].toLowerCase()])) {
                    a = a.replace(Y, "<$1></$2>");
                    try {
                        for (; e > d; d++) c = this[d] || {}, 1 === c.nodeType && (f.cleanData(c.getElementsByTagName("*")), c.innerHTML = a);
                        c = 0
                    } catch (g) {
                    }
                }
                c && this.empty().append(a)
            }, null, a, arguments.length)
        },
        replaceWith: function (a) {
            return this[0] && this[0].parentNode ? f.isFunction(a) ? this.each(function (b) {
                var c = f(this),
                    d = c.html();
                c.replaceWith(a.call(this, b, d))
            }) : ("string" != typeof a && (a = f(a).detach()), this.each(function () {
                var b = this.nextSibling,
                    c = this.parentNode;
                f(this).remove(), b ? f(b).before(a) : f(c).append(a)
            })) : this.length ? this.pushStack(f(f.isFunction(a) ? a() : a), "replaceWith", a) : this
        },
        detach: function (a) {
            return this.remove(a, !0)
        },
        domManip: function (a, c, d) {
            var e, g, h, i, j = a[0],
                k = [];
            if (!f.support.checkClone && 3 === arguments.length && "string" == typeof j && bd.test(j)) return this.each(function () {
                f(this).domManip(a, c, d, !0)
            });
            if (f.isFunction(j)) return this.each(function (e) {
                var g = f(this);
                a[0] = j.call(this, e, c ? g.html() : b), g.domManip(a, c, d)
            });
            if (this[0]) {
                if (i = j && j.parentNode, e = f.support.parentNode && i && 11 === i.nodeType && i.childNodes.length === this.length ? {
                        fragment: i
                    } : f.buildFragment(a, this, k), h = e.fragment, g = 1 === h.childNodes.length ? h = h.firstChild : h.firstChild, g) {
                    c = c && f.nodeName(g, "tr");
                    for (var l = 0, m = this.length, n = m - 1; m > l; l++) d.call(c ? bi(this[l], g) : this[l], e.cacheable || m > 1 && n > l ? f.clone(h, !0, !0) : h)
                }
                k.length && f.each(k, function (a, b) {
                    b.src ? f.ajax({
                        type: "GET",
                        global: !1,
                        url: b.src,
                        async: !1,
                        dataType: "script"
                    }) : f.globalEval((b.text || b.textContent || b.innerHTML || "").replace(bf, "/*$0*/")), b.parentNode && b.parentNode.removeChild(b)
                })
            }
            return this
        }
    }), f.buildFragment = function (a, b, d) {
        var e, g, h, i, j = a[0];
        return b && b[0] && (i = b[0].ownerDocument || b[0]), i.createDocumentFragment || (i = c), 1 === a.length && "string" == typeof j && 512 > j.length && i === c && "<" === j.charAt(0) && !bb.test(j) && (f.support.checkClone || !bd.test(j)) && (f.support.html5Clone || !bc.test(j)) && (g = !0, h = f.fragments[j], h && 1 !== h && (e = h)), e || (e = i.createDocumentFragment(), f.clean(a, i, e, d)), g && (f.fragments[j] = h ? e : 1), {
            fragment: e,
            cacheable: g
        }
    }, f.fragments = {}, f.each({
        appendTo: "append",
        prependTo: "prepend",
        insertBefore: "before",
        insertAfter: "after",
        replaceAll: "replaceWith"
    }, function (a, b) {
        f.fn[a] = function (c) {
            var d = [],
                e = f(c),
                g = 1 === this.length && this[0].parentNode;
            if (g && 11 === g.nodeType && 1 === g.childNodes.length && 1 === e.length) return e[b](this[0]), this;
            for (var h = 0, i = e.length; i > h; h++) {
                var j = (h > 0 ? this.clone(!0) : this).get();
                f(e[h])[b](j), d = d.concat(j)
            }
            return this.pushStack(d, a, e.selector)
        }
    }), f.extend({
        clone: function (a, b, c) {
            var d, e, g, h = f.support.html5Clone || f.isXMLDoc(a) || !bc.test("<" + a.nodeName + ">") ? a.cloneNode(!0) : bo(a);
            if (!(f.support.noCloneEvent && f.support.noCloneChecked || 1 !== a.nodeType && 11 !== a.nodeType || f.isXMLDoc(a))) for (bk(a, h), d = bl(a), e = bl(h), g = 0; d[g]; ++g) e[g] && bk(d[g], e[g]);
            if (b && (bj(a, h), c)) for (d = bl(a), e = bl(h), g = 0; d[g]; ++g) bj(d[g], e[g]);
            return d = e = null, h
        },
        clean: function (a, b, d, e) {
            var g, h, i, j = [];
            b = b || c, b.createElement === void 0 && (b = b.ownerDocument || b[0] && b[0].ownerDocument || c);
            for (var l, k = 0; null != (l = a[k]); k++) if ("number" == typeof l && (l += ""), l) {
                if ("string" == typeof l) if (_.test(l)) {
                    l = l.replace(Y, "<$1></$2>");
                    var r, m = (Z.exec(l) || ["", ""])[1].toLowerCase(),
                        n = bg[m] || bg._default,
                        o = n[0],
                        p = b.createElement("div"),
                        q = bh.childNodes;
                    for (b === c ? bh.appendChild(p) : U(b).appendChild(p), p.innerHTML = n[1] + l + n[2]; o--;) p = p.lastChild;
                    if (!f.support.tbody) {
                        var s = $.test(l),
                            t = "table" !== m || s ? "<table>" !== n[1] || s ? [] : p.childNodes : p.firstChild && p.firstChild.childNodes;
                        for (i = t.length - 1; i >= 0; --i) f.nodeName(t[i], "tbody") && !t[i].childNodes.length && t[i].parentNode.removeChild(t[i])
                    }
                    !f.support.leadingWhitespace && X.test(l) && p.insertBefore(b.createTextNode(X.exec(l)[0]), p.firstChild), l = p.childNodes, p && (p.parentNode.removeChild(p), q.length > 0 && (r = q[q.length - 1], r && r.parentNode && r.parentNode.removeChild(r)))
                } else l = b.createTextNode(l);
                var u;
                if (!f.support.appendChecked) if (l[0] && "number" == typeof(u = l.length)) for (i = 0; u > i; i++) bn(l[i]);
                else bn(l);
                l.nodeType ? j.push(l) : j = f.merge(j, l)
            }
            if (d) for (g = function (a) {
                return !a.type || be.test(a.type)
            }, k = 0; j[k]; k++) if (h = j[k], e && f.nodeName(h, "script") && (!h.type || be.test(h.type))) e.push(h.parentNode ? h.parentNode.removeChild(h) : h);
            else {
                if (1 === h.nodeType) {
                    var v = f.grep(h.getElementsByTagName("script"), g);
                    j.splice.apply(j, [k + 1, 0].concat(v))
                }
                d.appendChild(h)
            }
            return j
        },
        cleanData: function (a) {
            for (var b, c, i, d = f.cache, e = f.event.special, g = f.support.deleteExpando, h = 0; null != (i = a[h]); h++) if ((!i.nodeName || !f.noData[i.nodeName.toLowerCase()]) && (c = i[f.expando])) {
                if (b = d[c], b && b.events) {
                    for (var j in b.events) e[j] ? f.event.remove(i, j) : f.removeEvent(i, j, b.handle);
                    b.handle && (b.handle.elem = null)
                }
                g ? delete i[f.expando] : i.removeAttribute && i.removeAttribute(f.expando), delete d[c]
            }
        }
    });
    var by, bz, bA, bp = /alpha\([^)]*\)/i,
        bq = /opacity=([^)]*)/,
        br = /([A-Z]|^ms)/g,
        bs = /^[\-+]?(?:\d*\.)?\d+$/i,
        bt = /^-?(?:\d*\.)?\d+(?!px)[^\d\s]+$/i,
        bu = /^([\-+])=([\-+.\de]+)/,
        bv = /^margin/,
        bw = {
            position: "absolute",
            visibility: "hidden",
            display: "block"
        },
        bx = ["Top", "Right", "Bottom", "Left"];
    f.fn.css = function (a, c) {
        return f.access(this, function (a, c, d) {
            return d !== b ? f.style(a, c, d) : f.css(a, c)
        }, a, c, arguments.length > 1)
    }, f.extend({
        cssHooks: {
            opacity: {
                get: function (a, b) {
                    if (b) {
                        var c = by(a, "opacity");
                        return "" === c ? "1" : c
                    }
                    return a.style.opacity
                }
            }
        },
        cssNumber: {
            fillOpacity: !0,
            fontWeight: !0,
            lineHeight: !0,
            opacity: !0,
            orphans: !0,
            widows: !0,
            zIndex: !0,
            zoom: !0
        },
        cssProps: {
            "float": f.support.cssFloat ? "cssFloat" : "styleFloat"
        },
        style: function (a, c, d, e) {
            if (a && 3 !== a.nodeType && 8 !== a.nodeType && a.style) {
                var g, h, i = f.camelCase(c),
                    j = a.style,
                    k = f.cssHooks[i];
                if (c = f.cssProps[i] || i, d === b) return k && "get" in k && (g = k.get(a, !1, e)) !== b ? g : j[c];
                if (h = typeof d, "string" === h && (g = bu.exec(d)) && (d = +(g[1] + 1) * +g[2] + parseFloat(f.css(a, c)), h = "number"), null == d || "number" === h && isNaN(d)) return;
                if ("number" === h && !f.cssNumber[i] && (d += "px"), !(k && "set" in k && (d = k.set(a, d)) === b)) try {
                    j[c] = d
                } catch (l) {
                }
            }
        },
        css: function (a, c, d) {
            var e, g;
            return c = f.camelCase(c), g = f.cssHooks[c], c = f.cssProps[c] || c, "cssFloat" === c && (c = "float"), g && "get" in g && (e = g.get(a, !0, d)) !== b ? e : by ? by(a, c) : void 0
        },
        swap: function (a, b, c) {
            var e, f, d = {};
            for (f in b) d[f] = a.style[f], a.style[f] = b[f];
            e = c.call(a);
            for (f in b) a.style[f] = d[f];
            return e
        }
    }), f.curCSS = f.css, c.defaultView && c.defaultView.getComputedStyle && (bz = function (a, b) {
        var c, d, e, g, h = a.style;
        return b = b.replace(br, "-$1").toLowerCase(), (d = a.ownerDocument.defaultView) && (e = d.getComputedStyle(a, null)) && (c = e.getPropertyValue(b), "" === c && !f.contains(a.ownerDocument.documentElement, a) && (c = f.style(a, b))), !f.support.pixelMargin && e && bv.test(b) && bt.test(c) && (g = h.width, h.width = c, c = e.width, h.width = g), c
    }), c.documentElement.currentStyle && (bA = function (a, b) {
        var c, d, e, f = a.currentStyle && a.currentStyle[b],
            g = a.style;
        return null == f && g && (e = g[b]) && (f = e), bt.test(f) && (c = g.left, d = a.runtimeStyle && a.runtimeStyle.left, d && (a.runtimeStyle.left = a.currentStyle.left), g.left = "fontSize" === b ? "1em" : f, f = g.pixelLeft + "px", g.left = c, d && (a.runtimeStyle.left = d)), "" === f ? "auto" : f
    }), by = bz || bA, f.each(["height", "width"], function (a, b) {
        f.cssHooks[b] = {
            get: function (a, c, d) {
                return c ? 0 !== a.offsetWidth ? bB(a, b, d) : f.swap(a, bw, function () {
                    return bB(a, b, d)
                }) : void 0
            },
            set: function (a, b) {
                return bs.test(b) ? b + "px" : b
            }
        }
    }), f.support.opacity || (f.cssHooks.opacity = {
        get: function (a, b) {
            return bq.test((b && a.currentStyle ? a.currentStyle.filter : a.style.filter) || "") ? parseFloat(RegExp.$1) / 100 + "" : b ? "1" : ""
        },
        set: function (a, b) {
            var c = a.style,
                d = a.currentStyle,
                e = f.isNumeric(b) ? "alpha(opacity=" + 100 * b + ")" : "",
                g = d && d.filter || c.filter || "";
            c.zoom = 1, b >= 1 && "" === f.trim(g.replace(bp, "")) && (c.removeAttribute("filter"), d && !d.filter) || (c.filter = bp.test(g) ? g.replace(bp, e) : g + " " + e)
        }
    }), f(function () {
        f.support.reliableMarginRight || (f.cssHooks.marginRight = {
            get: function (a, b) {
                return f.swap(a, {
                    display: "inline-block"
                }, function () {
                    return b ? by(a, "margin-right") : a.style.marginRight
                })
            }
        })
    }), f.expr && f.expr.filters && (f.expr.filters.hidden = function (a) {
        var b = a.offsetWidth,
            c = a.offsetHeight;
        return 0 === b && 0 === c || !f.support.reliableHiddenOffsets && "none" === (a.style && a.style.display || f.css(a, "display"))
    }, f.expr.filters.visible = function (a) {
        return !f.expr.filters.hidden(a)
    }), f.each({
        margin: "",
        padding: "",
        border: "Width"
    }, function (a, b) {
        f.cssHooks[a + b] = {
            expand: function (c) {
                var d, e = "string" == typeof c ? c.split(" ") : [c],
                    f = {};
                for (d = 0; 4 > d; d++) f[a + bx[d] + b] = e[d] || e[d - 2] || e[0];
                return f
            }
        }
    });
    var bU, bV, bC = /%20/g,
        bD = /\[\]$/,
        bE = /\r?\n/g,
        bF = /#.*$/,
        bG = /^(.*?):[ \t]*([^\r\n]*)\r?$/gm,
        bH = /^(?:color|date|datetime|datetime-local|email|hidden|month|number|password|range|search|tel|text|time|url|week)$/i,
        bI = /^(?:about|app|app\-storage|.+\-extension|file|res|widget):$/,
        bJ = /^(?:GET|HEAD)$/,
        bK = /^\/\//,
        bL = /\?/,
        bM = /<script\b[^<]*(?:(?!<\/script>)<[^<]*)*<\/script>/gi,
        bN = /^(?:select|textarea)/i,
        bO = /\s+/,
        bP = /([?&])_=[^&]*/,
        bQ = /^([\w\+\.\-]+:)(?:\/\/([^\/?#:]*)(?::(\d+))?)?/,
        bR = f.fn.load,
        bS = {},
        bT = {},
        bW = ["*/"] + ["*"];
    try {
        bU = e.href
    } catch (bX) {
        bU = c.createElement("a"), bU.href = "", bU = bU.href
    }
    bV = bQ.exec(bU.toLowerCase()) || [], f.fn.extend({
        load: function (a, c, d) {
            if ("string" != typeof a && bR) return bR.apply(this, arguments);
            if (!this.length) return this;
            var e = a.indexOf(" ");
            if (e >= 0) {
                var g = a.slice(e, a.length);
                a = a.slice(0, e)
            }
            var h = "GET";
            c && (f.isFunction(c) ? (d = c, c = b) : "object" == typeof c && (c = f.param(c, f.ajaxSettings.traditional), h = "POST"));
            var i = this;
            return f.ajax({
                url: a,
                type: h,
                dataType: "html",
                data: c,
                complete: function (a, b, c) {
                    c = a.responseText, a.isResolved() && (a.done(function (a) {
                        c = a
                    }), i.html(g ? f("<div>").append(c.replace(bM, "")).find(g) : c)), d && i.each(d, [c, b, a])
                }
            }), this
        },
        serialize: function () {
            return f.param(this.serializeArray())
        },
        serializeArray: function () {
            return this.map(function () {
                return this.elements ? f.makeArray(this.elements) : this
            }).filter(function () {
                return this.name && !this.disabled && (this.checked || bN.test(this.nodeName) || bH.test(this.type))
            }).map(function (a, b) {
                var c = f(this).val();
                return null == c ? null : f.isArray(c) ? f.map(c, function (a) {
                    return {
                        name: b.name,
                        value: a.replace(bE, "\r\n")
                    }
                }) : {
                    name: b.name,
                    value: c.replace(bE, "\r\n")
                }
            }).get()
        }
    }), f.each("ajaxStart ajaxStop ajaxComplete ajaxError ajaxSuccess ajaxSend".split(" "), function (a, b) {
        f.fn[b] = function (a) {
            return this.on(b, a)
        }
    }), f.each(["get", "post"], function (a, c) {
        f[c] = function (a, d, e, g) {
            return f.isFunction(d) && (g = g || e, e = d, d = b), f.ajax({
                type: c,
                url: a,
                data: d,
                success: e,
                dataType: g
            })
        }
    }), f.extend({
        getScript: function (a, c) {
            return f.get(a, b, c, "script")
        },
        getJSON: function (a, b, c) {
            return f.get(a, b, c, "json")
        },
        ajaxSetup: function (a, b) {
            return b ? b$(a, f.ajaxSettings) : (b = a, a = f.ajaxSettings), b$(a, b), a
        },
        ajaxSettings: {
            url: bU,
            isLocal: bI.test(bV[1]),
            global: !0,
            type: "GET",
            contentType: "application/x-www-form-urlencoded; charset=UTF-8",
            processData: !0,
            async: !0,
            accepts: {
                xml: "application/xml, text/xml",
                html: "text/html",
                text: "text/plain",
                json: "application/json, text/javascript",
                "*": bW
            },
            contents: {
                xml: /xml/,
                html: /html/,
                json: /json/
            },
            responseFields: {
                xml: "responseXML",
                text: "responseText"
            },
            converters: {
                "* text": a.String,
                "text html": !0,
                "text json": f.parseJSON,
                "text xml": f.parseXML
            },
            flatOptions: {
                context: !0,
                url: !0
            }
        },
        ajaxPrefilter: bY(bS),
        ajaxTransport: bY(bT),
        ajax: function (a, c) {
            function w(a, c, l, m) {
                if (2 !== s) {
                    s = 2, q && clearTimeout(q), p = b, n = m || "", v.readyState = a > 0 ? 4 : 0;
                    var o, r, u, y, z, w = c,
                        x = l ? ca(d, v, l) : b;
                    if (a >= 200 && 300 > a || 304 === a) if (d.ifModified && ((y = v.getResponseHeader("Last-Modified")) && (f.lastModified[k] = y), (z = v.getResponseHeader("Etag")) && (f.etag[k] = z)), 304 === a) w = "notmodified", o = !0;
                    else try {
                            r = cb(d, x), w = "success", o = !0
                        } catch (A) {
                            w = "parsererror", u = A
                        } else u = w, (!w || a) && (w = "error", 0 > a && (a = 0));
                    v.status = a, v.statusText = "" + (c || w), o ? h.resolveWith(e, [r, w, v]) : h.rejectWith(e, [v, w, u]), v.statusCode(j), j = b, t && g.trigger("ajax" + (o ? "Success" : "Error"), [v, d, o ? r : u]), i.fireWith(e, [v, w]), t && (g.trigger("ajaxComplete", [v, d]), --f.active || f.event.trigger("ajaxStop"))
                }
            }

            "object" == typeof a && (c = a, a = b), c = c || {};
            var k, n, o, p, q, r, t, u, d = f.ajaxSetup({}, c),
                e = d.context || d,
                g = e !== d && (e.nodeType || e instanceof f) ? f(e) : f.event,
                h = f.Deferred(),
                i = f.Callbacks("once memory"),
                j = d.statusCode || {},
                l = {},
                m = {},
                s = 0,
                v = {
                    readyState: 0,
                    setRequestHeader: function (a, b) {
                        if (!s) {
                            var c = a.toLowerCase();
                            a = m[c] = m[c] || a, l[a] = b
                        }
                        return this
                    },
                    getAllResponseHeaders: function () {
                        return 2 === s ? n : null
                    },
                    getResponseHeader: function (a) {
                        var c;
                        if (2 === s) {
                            if (!o) for (o = {}; c = bG.exec(n);) o[c[1].toLowerCase()] = c[2];
                            c = o[a.toLowerCase()]
                        }
                        return c === b ? null : c
                    },
                    overrideMimeType: function (a) {
                        return s || (d.mimeType = a), this
                    },
                    abort: function (a) {
                        return a = a || "abort", p && p.abort(a), w(0, a), this
                    }
                };
            if (h.promise(v), v.success = v.done, v.error = v.fail, v.complete = i.add, v.statusCode = function (a) {
                    if (a) {
                        var b;
                        if (2 > s) for (b in a) j[b] = [j[b], a[b]];
                        else b = a[v.status], v.then(b, b)
                    }
                    return this
                }, d.url = ((a || d.url) + "").replace(bF, "").replace(bK, bV[1] + "//"), d.dataTypes = f.trim(d.dataType || "*").toLowerCase().split(bO), null == d.crossDomain && (r = bQ.exec(d.url.toLowerCase()), d.crossDomain = !(!r || r[1] == bV[1] && r[2] == bV[2] && (r[3] || ("http:" === r[1] ? 80 : 443)) == (bV[3] || ("http:" === bV[1] ? 80 : 443)))), d.data && d.processData && "string" != typeof d.data && (d.data = f.param(d.data, d.traditional)), bZ(bS, d, c, v), 2 === s) return !1;
            if (t = d.global, d.type = d.type.toUpperCase(), d.hasContent = !bJ.test(d.type), t && 0 === f.active++ && f.event.trigger("ajaxStart"), !d.hasContent && (d.data && (d.url += (bL.test(d.url) ? "&" : "?") + d.data, delete d.data), k = d.url, d.cache === !1)) {
                var x = f.now(),
                    y = d.url.replace(bP, "$1_=" + x);
                d.url = y + (y === d.url ? (bL.test(d.url) ? "&" : "?") + "_=" + x : "")
            }
            (d.data && d.hasContent && d.contentType !== !1 || c.contentType) && v.setRequestHeader("Content-Type", d.contentType), d.ifModified && (k = k || d.url, f.lastModified[k] && v.setRequestHeader("If-Modified-Since", f.lastModified[k]), f.etag[k] && v.setRequestHeader("If-None-Match", f.etag[k])), v.setRequestHeader("Accept", d.dataTypes[0] && d.accepts[d.dataTypes[0]] ? d.accepts[d.dataTypes[0]] + ("*" !== d.dataTypes[0] ? ", " + bW + "; q=0.01" : "") : d.accepts["*"]);
            for (u in d.headers) v.setRequestHeader(u, d.headers[u]);
            if (d.beforeSend && (d.beforeSend.call(e, v, d) === !1 || 2 === s)) return v.abort(), !1;
            for (u in {
                success: 1,
                error: 1,
                complete: 1
            }) v[u](d[u]);
            if (p = bZ(bT, d, c, v)) {
                v.readyState = 1, t && g.trigger("ajaxSend", [v, d]), d.async && d.timeout > 0 && (q = setTimeout(function () {
                    v.abort("timeout")
                }, d.timeout));
                try {
                    s = 1, p.send(l, w)
                } catch (z) {
                    if (!(2 > s)) throw z;
                    w(-1, z)
                }
            } else w(-1, "No Transport");
            return v
        },
        param: function (a, c) {
            var d = [],
                e = function (a, b) {
                    b = f.isFunction(b) ? b() : b, d[d.length] = encodeURIComponent(a) + "=" + encodeURIComponent(b)
                };
            if (c === b && (c = f.ajaxSettings.traditional), f.isArray(a) || a.jquery && !f.isPlainObject(a)) f.each(a, function () {
                e(this.name, this.value)
            });
            else for (var g in a) b_(g, a[g], c, e);
            return d.join("&").replace(bC, "+")
        }
    }), f.extend({
        active: 0,
        lastModified: {},
        etag: {}
    });
    var cc = f.now(),
        cd = /(\=)\?(&|$)|\?\?/i;
    f.ajaxSetup({
        jsonp: "callback",
        jsonpCallback: function () {
            return f.expando + "_" + cc++
        }
    }), f.ajaxPrefilter("json jsonp", function (b, c, d) {
        var e = "string" == typeof b.data && /^application\/x\-www\-form\-urlencoded/.test(b.contentType);
        if ("jsonp" === b.dataTypes[0] || b.jsonp !== !1 && (cd.test(b.url) || e && cd.test(b.data))) {
            var g, h = b.jsonpCallback = f.isFunction(b.jsonpCallback) ? b.jsonpCallback() : b.jsonpCallback,
                i = a[h],
                j = b.url,
                k = b.data,
                l = "$1" + h + "$2";
            return b.jsonp !== !1 && (j = j.replace(cd, l), b.url === j && (e && (k = k.replace(cd, l)), b.data === k && (j += (/\?/.test(j) ? "&" : "?") + b.jsonp + "=" + h))), b.url = j, b.data = k, a[h] = function (a) {
                g = [a]
            }, d.always(function () {
                a[h] = i, g && f.isFunction(i) && a[h](g[0])
            }), b.converters["script json"] = function () {
                return g || f.error(h + " was not called"), g[0]
            }, b.dataTypes[0] = "json", "script"
        }
    }), f.ajaxSetup({
        accepts: {
            script: "text/javascript, application/javascript, application/ecmascript, application/x-ecmascript"
        },
        contents: {
            script: /javascript|ecmascript/
        },
        converters: {
            "text script": function (a) {
                return f.globalEval(a), a
            }
        }
    }), f.ajaxPrefilter("script", function (a) {
        a.cache === b && (a.cache = !1), a.crossDomain && (a.type = "GET", a.global = !1)
    }), f.ajaxTransport("script", function (a) {
        if (a.crossDomain) {
            var d, e = c.head || c.getElementsByTagName("head")[0] || c.documentElement;
            return {
                send: function (f, g) {
                    d = c.createElement("script"), d.async = "async", a.scriptCharset && (d.charset = a.scriptCharset), d.src = a.url, d.onload = d.onreadystatechange = function (a, c) {
                        (c || !d.readyState || /loaded|complete/.test(d.readyState)) && (d.onload = d.onreadystatechange = null, e && d.parentNode && e.removeChild(d), d = b, c || g(200, "success"))
                    }, e.insertBefore(d, e.firstChild)
                },
                abort: function () {
                    d && d.onload(0, 1)
                }
            }
        }
    });
    var cg, ce = a.ActiveXObject ?
        function () {
            for (var a in cg) cg[a](0, 1)
        } : !1, cf = 0;
    f.ajaxSettings.xhr = a.ActiveXObject ?
        function () {
            return !this.isLocal && ch() || ci()
        } : ch, function (a) {
        f.extend(f.support, {
            ajax: !!a,
            cors: !!a && "withCredentials" in a
        })
    }(f.ajaxSettings.xhr()), f.support.ajax && f.ajaxTransport(function (c) {
        if (!c.crossDomain || f.support.cors) {
            var d;
            return {
                send: function (e, g) {
                    var i, j, h = c.xhr();
                    if (c.username ? h.open(c.type, c.url, c.async, c.username, c.password) : h.open(c.type, c.url, c.async), c.xhrFields) for (j in c.xhrFields) h[j] = c.xhrFields[j];
                    c.mimeType && h.overrideMimeType && h.overrideMimeType(c.mimeType), !c.crossDomain && !e["X-Requested-With"] && (e["X-Requested-With"] = "XMLHttpRequest");
                    try {
                        for (j in e) h.setRequestHeader(j, e[j])
                    } catch (k) {
                    }
                    h.send(c.hasContent && c.data || null), d = function (a, e) {
                        var j, k, l, m, n;
                        try {
                            if (d && (e || 4 === h.readyState)) if (d = b, i && (h.onreadystatechange = f.noop, ce && delete cg[i]), e) 4 !== h.readyState && h.abort();
                            else {
                                j = h.status, l = h.getAllResponseHeaders(), m = {}, n = h.responseXML, n && n.documentElement && (m.xml = n);
                                try {
                                    m.text = h.responseText
                                } catch (a) {
                                }
                                try {
                                    k = h.statusText
                                } catch (o) {
                                    k = ""
                                }
                                j || !c.isLocal || c.crossDomain ? 1223 === j && (j = 204) : j = m.text ? 200 : 404
                            }
                        } catch (p) {
                            e || g(-1, p)
                        }
                        m && g(j, k, m, l)
                    }, c.async && 4 !== h.readyState ? (i = ++cf, ce && (cg || (cg = {}, f(a).unload(ce)), cg[i] = d), h.onreadystatechange = d) : d()
                },
                abort: function () {
                    d && d(0, 1)
                }
            }
        }
    });
    var ck, cl, co, cq, cj = {},
        cm = /^(?:toggle|show|hide)$/,
        cn = /^([+\-]=)?([\d+.\-]+)([a-z%]*)$/i,
        cp = [
            ["height", "marginTop", "marginBottom", "paddingTop", "paddingBottom"],
            ["width", "marginLeft", "marginRight", "paddingLeft", "paddingRight"],
            ["opacity"]
        ];
    f.fn.extend({
        show: function (a, b, c) {
            var d, e;
            if (a || 0 === a) return this.animate(ct("show", 3), a, b, c);
            for (var g = 0, h = this.length; h > g; g++) d = this[g], d.style && (e = d.style.display, !f._data(d, "olddisplay") && "none" === e && (e = d.style.display = ""), ("" === e && "none" === f.css(d, "display") || !f.contains(d.ownerDocument.documentElement, d)) && f._data(d, "olddisplay", cu(d.nodeName)));
            for (g = 0; h > g; g++) d = this[g], d.style && (e = d.style.display, ("" === e || "none" === e) && (d.style.display = f._data(d, "olddisplay") || ""));
            return this
        },
        hide: function (a, b, c) {
            if (a || 0 === a) return this.animate(ct("hide", 3), a, b, c);
            for (var d, e, g = 0, h = this.length; h > g; g++) d = this[g], d.style && (e = f.css(d, "display"), "none" !== e && !f._data(d, "olddisplay") && f._data(d, "olddisplay", e));
            for (g = 0; h > g; g++) this[g].style && (this[g].style.display = "none");
            return this
        },
        _toggle: f.fn.toggle,
        toggle: function (a, b, c) {
            var d = "boolean" == typeof a;
            return f.isFunction(a) && f.isFunction(b) ? this._toggle.apply(this, arguments) : null == a || d ? this.each(function () {
                var b = d ? a : f(this).is(":hidden");
                f(this)[b ? "show" : "hide"]()
            }) : this.animate(ct("toggle", 3), a, b, c), this
        },
        fadeTo: function (a, b, c, d) {
            return this.filter(":hidden").css("opacity", 0).show().end().animate({
                opacity: b
            }, a, c, d)
        },
        animate: function (a, b, c, d) {
            function g() {
                e.queue === !1 && f._mark(this);
                var g, h, i, j, k, l, m, n, o, p, q, b = f.extend({}, e),
                    c = 1 === this.nodeType,
                    d = c && f(this).is(":hidden");
                b.animatedProperties = {};
                for (i in a) if (g = f.camelCase(i), i !== g && (a[g] = a[i], delete a[i]), (k = f.cssHooks[g]) && "expand" in k) {
                    l = k.expand(a[g]), delete a[g];
                    for (i in l) i in a || (a[i] = l[i])
                }
                for (g in a) {
                    if (h = a[g], f.isArray(h) ? (b.animatedProperties[g] = h[1], h = a[g] = h[0]) : b.animatedProperties[g] = b.specialEasing && b.specialEasing[g] || b.easing || "swing", "hide" === h && d || "show" === h && !d) return b.complete.call(this);
                    c && ("height" === g || "width" === g) && (b.overflow = [this.style.overflow, this.style.overflowX, this.style.overflowY], "inline" === f.css(this, "display") && "none" === f.css(this, "float") && (f.support.inlineBlockNeedsLayout && "inline" !== cu(this.nodeName) ? this.style.zoom = 1 : this.style.display = "inline-block"))
                }
                null != b.overflow && (this.style.overflow = "hidden");
                for (i in a) j = new f.fx(this, b, i), h = a[i], cm.test(h) ? (q = f._data(this, "toggle" + i) || ("toggle" === h ? d ? "show" : "hide" : 0), q ? (f._data(this, "toggle" + i, "show" === q ? "hide" : "show"), j[q]()) : j[h]()) : (m = cn.exec(h), n = j.cur(), m ? (o = parseFloat(m[2]), p = m[3] || (f.cssNumber[i] ? "" : "px"), "px" !== p && (f.style(this, i, (o || 1) + p), n = (o || 1) / j.cur() * n, f.style(this, i, n + p)), m[1] && (o = ("-=" === m[1] ? -1 : 1) * o + n), j.custom(n, o, p)) : j.custom(n, h, ""));
                return !0
            }

            var e = f.speed(b, c, d);
            return f.isEmptyObject(a) ? this.each(e.complete, [!1]) : (a = f.extend({}, a), e.queue === !1 ? this.each(g) : this.queue(e.queue, g))
        },
        stop: function (a, c, d) {
            return "string" != typeof a && (d = c, c = a, a = b), c && a !== !1 && this.queue(a || "fx", []), this.each(function () {
                function h(a, b, c) {
                    var e = b[c];
                    f.removeData(a, c, !0), e.stop(d)
                }

                var b, c = !1,
                    e = f.timers,
                    g = f._data(this);
                if (d || f._unmark(!0, this), null == a) for (b in g) g[b] && g[b].stop && b.indexOf(".run") === b.length - 4 && h(this, g, b);
                else g[b = a + ".run"] && g[b].stop && h(this, g, b);
                for (b = e.length; b--;) e[b].elem === this && (null == a || e[b].queue === a) && (d ? e[b](!0) : e[b].saveState(), c = !0, e.splice(b, 1));
                (!d || !c) && f.dequeue(this, a)
            })
        }
    }), f.each({
        slideDown: ct("show", 1),
        slideUp: ct("hide", 1),
        slideToggle: ct("toggle", 1),
        fadeIn: {
            opacity: "show"
        },
        fadeOut: {
            opacity: "hide"
        },
        fadeToggle: {
            opacity: "toggle"
        }
    }, function (a, b) {
        f.fn[a] = function (a, c, d) {
            return this.animate(b, a, c, d)
        }
    }), f.extend({
        speed: function (a, b, c) {
            var d = a && "object" == typeof a ? f.extend({}, a) : {
                complete: c || !c && b || f.isFunction(a) && a,
                duration: a,
                easing: c && b || b && !f.isFunction(b) && b
            };
            return d.duration = f.fx.off ? 0 : "number" == typeof d.duration ? d.duration : d.duration in f.fx.speeds ? f.fx.speeds[d.duration] : f.fx.speeds._default, (null == d.queue || d.queue === !0) && (d.queue = "fx"), d.old = d.complete, d.complete = function (a) {
                f.isFunction(d.old) && d.old.call(this), d.queue ? f.dequeue(this, d.queue) : a !== !1 && f._unmark(this)
            }, d
        },
        easing: {
            linear: function (a) {
                return a
            },
            swing: function (a) {
                return -Math.cos(a * Math.PI) / 2 + .5
            }
        },
        timers: [],
        fx: function (a, b, c) {
            this.options = b, this.elem = a, this.prop = c, b.orig = b.orig || {}
        }
    }), f.fx.prototype = {
        update: function () {
            this.options.step && this.options.step.call(this.elem, this.now, this), (f.fx.step[this.prop] || f.fx.step._default)(this)
        },
        cur: function () {
            if (null != this.elem[this.prop] && (!this.elem.style || null == this.elem.style[this.prop])) return this.elem[this.prop];
            var a, b = f.css(this.elem, this.prop);
            return isNaN(a = parseFloat(b)) ? b && "auto" !== b ? b : 0 : a
        },
        custom: function (a, c, d) {
            function h(a) {
                return e.step(a)
            }

            var e = this,
                g = f.fx;
            this.startTime = cq || cr(), this.end = c, this.now = this.start = a, this.pos = this.state = 0, this.unit = d || this.unit || (f.cssNumber[this.prop] ? "" : "px"), h.queue = this.options.queue, h.elem = this.elem, h.saveState = function () {
                f._data(e.elem, "fxshow" + e.prop) === b && (e.options.hide ? f._data(e.elem, "fxshow" + e.prop, e.start) : e.options.show && f._data(e.elem, "fxshow" + e.prop, e.end))
            }, h() && f.timers.push(h) && !co && (co = setInterval(g.tick, g.interval))
        },
        show: function () {
            var a = f._data(this.elem, "fxshow" + this.prop);
            this.options.orig[this.prop] = a || f.style(this.elem, this.prop), this.options.show = !0, a !== b ? this.custom(this.cur(), a) : this.custom("width" === this.prop || "height" === this.prop ? 1 : 0, this.cur()), f(this.elem).show()
        },
        hide: function () {
            this.options.orig[this.prop] = f._data(this.elem, "fxshow" + this.prop) || f.style(this.elem, this.prop), this.options.hide = !0, this.custom(this.cur(), 0)
        },
        step: function (a) {
            var b, c, d, e = cq || cr(),
                g = !0,
                h = this.elem,
                i = this.options;
            if (a || e >= i.duration + this.startTime) {
                this.now = this.end, this.pos = this.state = 1, this.update(), i.animatedProperties[this.prop] = !0;
                for (b in i.animatedProperties) i.animatedProperties[b] !== !0 && (g = !1);
                if (g) {
                    if (null != i.overflow && !f.support.shrinkWrapBlocks && f.each(["", "X", "Y"], function (a, b) {
                            h.style["overflow" + b] = i.overflow[a]
                        }), i.hide && f(h).hide(), i.hide || i.show) for (b in i.animatedProperties) f.style(h, b, i.orig[b]), f.removeData(h, "fxshow" + b, !0), f.removeData(h, "toggle" + b, !0);
                    d = i.complete, d && (i.complete = !1, d.call(h))
                }
                return !1
            }
            return 1 / 0 == i.duration ? this.now = e : (c = e - this.startTime, this.state = c / i.duration, this.pos = f.easing[i.animatedProperties[this.prop]](this.state, c, 0, 1, i.duration), this.now = this.start + (this.end - this.start) * this.pos), this.update(), !0
        }
    }, f.extend(f.fx, {
        tick: function () {
            for (var a, b = f.timers, c = 0; b.length > c; c++) a = b[c], !a() && b[c] === a && b.splice(c--, 1);
            b.length || f.fx.stop()
        },
        interval: 13,
        stop: function () {
            clearInterval(co), co = null
        },
        speeds: {
            slow: 600,
            fast: 200,
            _default: 400
        },
        step: {
            opacity: function (a) {
                f.style(a.elem, "opacity", a.now)
            },
            _default: function (a) {
                a.elem.style && null != a.elem.style[a.prop] ? a.elem.style[a.prop] = a.now + a.unit : a.elem[a.prop] = a.now
            }
        }
    }), f.each(cp.concat.apply([], cp), function (a, b) {
        b.indexOf("margin") && (f.fx.step[b] = function (a) {
            f.style(a.elem, b, Math.max(0, a.now) + a.unit)
        })
    }), f.expr && f.expr.filters && (f.expr.filters.animated = function (a) {
        return f.grep(f.timers, function (b) {
            return a === b.elem
        }).length
    });
    var cv, cw = /^t(?:able|d|h)$/i,
        cx = /^(?:body|html)$/i;
    cv = "getBoundingClientRect" in c.documentElement ?
        function (a, b, c, d) {
            try {
                d = a.getBoundingClientRect()
            } catch (e) {
            }
            if (!d || !f.contains(c, a)) return d ? {
                top: d.top,
                left: d.left
            } : {
                top: 0,
                left: 0
            };
            var g = b.body,
                h = cy(b),
                i = c.clientTop || g.clientTop || 0,
                j = c.clientLeft || g.clientLeft || 0,
                k = h.pageYOffset || f.support.boxModel && c.scrollTop || g.scrollTop,
                l = h.pageXOffset || f.support.boxModel && c.scrollLeft || g.scrollLeft,
                m = d.top + k - i,
                n = d.left + l - j;
            return {
                top: m,
                left: n
            }
        } : function (a, b, c) {
        for (var d, e = a.offsetParent, g = a, h = b.body, i = b.defaultView, j = i ? i.getComputedStyle(a, null) : a.currentStyle, k = a.offsetTop, l = a.offsetLeft;
             (a = a.parentNode) && a !== h && a !== c && (!f.support.fixedPosition || "fixed" !== j.position);) d = i ? i.getComputedStyle(a, null) : a.currentStyle, k -= a.scrollTop, l -= a.scrollLeft, a === e && (k += a.offsetTop, l += a.offsetLeft, f.support.doesNotAddBorder && (!f.support.doesAddBorderForTableAndCells || !cw.test(a.nodeName)) && (k += parseFloat(d.borderTopWidth) || 0, l += parseFloat(d.borderLeftWidth) || 0), g = e, e = a.offsetParent), f.support.subtractsBorderForOverflowNotVisible && "visible" !== d.overflow && (k += parseFloat(d.borderTopWidth) || 0, l += parseFloat(d.borderLeftWidth) || 0), j = d;
        return ("relative" === j.position || "static" === j.position) && (k += h.offsetTop, l += h.offsetLeft), f.support.fixedPosition && "fixed" === j.position && (k += Math.max(c.scrollTop, h.scrollTop), l += Math.max(c.scrollLeft, h.scrollLeft)), {
            top: k,
            left: l
        }
    }, f.fn.offset = function (a) {
        if (arguments.length) return a === b ? this : this.each(function (b) {
            f.offset.setOffset(this, a, b)
        });
        var c = this[0],
            d = c && c.ownerDocument;
        return d ? c === d.body ? f.offset.bodyOffset(c) : cv(c, d, d.documentElement) : null
    }, f.offset = {
        bodyOffset: function (a) {
            var b = a.offsetTop,
                c = a.offsetLeft;
            return f.support.doesNotIncludeMarginInBodyOffset && (b += parseFloat(f.css(a, "marginTop")) || 0, c += parseFloat(f.css(a, "marginLeft")) || 0), {
                top: b,
                left: c
            }
        },
        setOffset: function (a, b, c) {
            var d = f.css(a, "position");
            "static" === d && (a.style.position = "relative");
            var m, n, e = f(a),
                g = e.offset(),
                h = f.css(a, "top"),
                i = f.css(a, "left"),
                j = ("absolute" === d || "fixed" === d) && f.inArray("auto", [h, i]) > -1,
                k = {},
                l = {};
            j ? (l = e.position(), m = l.top, n = l.left) : (m = parseFloat(h) || 0, n = parseFloat(i) || 0), f.isFunction(b) && (b = b.call(a, c, g)), null != b.top && (k.top = b.top - g.top + m), null != b.left && (k.left = b.left - g.left + n), "using" in b ? b.using.call(a, k) : e.css(k)
        }
    }, f.fn.extend({
        position: function () {
            if (!this[0]) return null;
            var a = this[0],
                b = this.offsetParent(),
                c = this.offset(),
                d = cx.test(b[0].nodeName) ? {
                    top: 0,
                    left: 0
                } : b.offset();
            return c.top -= parseFloat(f.css(a, "marginTop")) || 0, c.left -= parseFloat(f.css(a, "marginLeft")) || 0, d.top += parseFloat(f.css(b[0], "borderTopWidth")) || 0, d.left += parseFloat(f.css(b[0], "borderLeftWidth")) || 0, {
                top: c.top - d.top,
                left: c.left - d.left
            }
        },
        offsetParent: function () {
            return this.map(function () {
                for (var a = this.offsetParent || c.body; a && !cx.test(a.nodeName) && "static" === f.css(a, "position");) a = a.offsetParent;
                return a
            })
        }
    }), f.each({
        scrollLeft: "pageXOffset",
        scrollTop: "pageYOffset"
    }, function (a, c) {
        var d = /Y/.test(c);
        f.fn[a] = function (e) {
            return f.access(this, function (a, e, g) {
                var h = cy(a);
                return g === b ? h ? c in h ? h[c] : f.support.boxModel && h.document.documentElement[e] || h.document.body[e] : a[e] : (h ? h.scrollTo(d ? f(h).scrollLeft() : g, d ? g : f(h).scrollTop()) : a[e] = g, void 0)
            }, a, e, arguments.length, null)
        }
    }), f.each({
        Height: "height",
        Width: "width"
    }, function (a, c) {
        var d = "client" + a,
            e = "scroll" + a,
            g = "offset" + a;
        f.fn["inner" + a] = function () {
            var a = this[0];
            return a ? a.style ? parseFloat(f.css(a, c, "padding")) : this[c]() : null
        }, f.fn["outer" + a] = function (a) {
            var b = this[0];
            return b ? b.style ? parseFloat(f.css(b, c, a ? "margin" : "border")) : this[c]() : null
        }, f.fn[c] = function (a) {
            return f.access(this, function (a, c, h) {
                var i, j, k, l;
                return f.isWindow(a) ? (i = a.document, j = i.documentElement[d], f.support.boxModel && j || i.body && i.body[d] || j) : 9 === a.nodeType ? (i = a.documentElement, i[d] >= i[e] ? i[d] : Math.max(a.body[e], i[e], a.body[g], i[g])) : h === b ? (k = f.css(a, c), l = parseFloat(k), f.isNumeric(l) ? l : k) : (f(a).css(c, h), void 0)
            }, c, a, arguments.length, null)
        }
    }), a.jQuery = a.$ = f, "function" == typeof define && define.amd && define.amd.jQuery && define("jquery", [], function () {
        return f
    })
}(window), function (c, h) {
    c.fn.jPlayer = function (a) {
        var b = "string" == typeof a,
            d = Array.prototype.slice.call(arguments, 1),
            f = this;
        return a = !b && d.length ? c.extend.apply(null, [!0, a].concat(d)) : a, b && "_" === a.charAt(0) ? f : (b ? this.each(function () {
            var e = c.data(this, "jPlayer"),
                g = e && c.isFunction(e[a]) ? e[a].apply(e, d) : e;
            return g !== e && g !== h ? (f = g, !1) : void 0
        }) : this.each(function () {
            var e = c.data(this, "jPlayer");
            e ? (e.option(a || {})._init(), e.option(a || {})) : c.data(this, "jPlayer", new c.jPlayer(a, this))
        }), f)
    }, c.jPlayer = function (a, b) {
        if (arguments.length) {
            this.element = c(b), this.options = c.extend(!0, {}, this.options, a);
            var d = this;
            this.element.bind("remove.jPlayer", function () {
                d.destroy()
            }), this._init()
        }
    }, c.jPlayer.event = {
        ready: "jPlayer_ready",
        resize: "jPlayer_resize",
        error: "jPlayer_error",
        warning: "jPlayer_warning",
        loadstart: "jPlayer_loadstart",
        progress: "jPlayer_progress",
        suspend: "jPlayer_suspend",
        abort: "jPlayer_abort",
        emptied: "jPlayer_emptied",
        stalled: "jPlayer_stalled",
        play: "jPlayer_play",
        pause: "jPlayer_pause",
        loadedmetadata: "jPlayer_loadedmetadata",
        loadeddata: "jPlayer_loadeddata",
        waiting: "jPlayer_waiting",
        playing: "jPlayer_playing",
        canplay: "jPlayer_canplay",
        canplaythrough: "jPlayer_canplaythrough",
        seeking: "jPlayer_seeking",
        seeked: "jPlayer_seeked",
        timeupdate: "jPlayer_timeupdate",
        ended: "jPlayer_ended",
        ratechange: "jPlayer_ratechange",
        durationchange: "jPlayer_durationchange",
        volumechange: "jPlayer_volumechange"
    }, c.jPlayer.htmlEvent = ["loadstart", "abort", "emptied", "stalled", "loadedmetadata", "loadeddata", "canplaythrough", "ratechange"], c.jPlayer.pause = function () {
        c.each(c.jPlayer.prototype.instances, function (a, b) {
            b.data("jPlayer").status.srcSet && b.jPlayer("pause")
        })
    }, c.jPlayer.timeFormat = {
        showHour: !1,
        showMin: !0,
        showSec: !0,
        padHour: !1,
        padMin: !0,
        padSec: !0,
        sepHour: ":",
        sepMin: ":",
        sepSec: ""
    }, c.jPlayer.convertTime = function (a) {
        a = new Date(1e3 * a);
        var b = a.getUTCHours(),
            d = a.getUTCMinutes();
        return a = a.getUTCSeconds(), b = c.jPlayer.timeFormat.padHour && 10 > b ? "0" + b : b, d = c.jPlayer.timeFormat.padMin && 10 > d ? "0" + d : d, a = c.jPlayer.timeFormat.padSec && 10 > a ? "0" + a : a, (c.jPlayer.timeFormat.showHour ? b + c.jPlayer.timeFormat.sepHour : "") + (c.jPlayer.timeFormat.showMin ? d + c.jPlayer.timeFormat.sepMin : "") + (c.jPlayer.timeFormat.showSec ? a + c.jPlayer.timeFormat.sepSec : "")
    }, c.jPlayer.uaMatch = function (a) {
        a = a.toLowerCase();
        var b = /(opera)(?:.*version)?[ \/]([\w.]+)/,
            d = /(msie) ([\w.]+)/,
            f = /(mozilla)(?:.*? rv:([\w.]+))?/;
        return a = /(webkit)[ \/]([\w.]+)/.exec(a) || b.exec(a) || d.exec(a) || 0 > a.indexOf("compatible") && f.exec(a) || [], {
            browser: a[1] || "",
            version: a[2] || "0"
        }
    }, c.jPlayer.browser = {};
    var m = c.jPlayer.uaMatch(navigator.userAgent);
    m.browser && (c.jPlayer.browser[m.browser] = !0, c.jPlayer.browser.version = m.version), c.jPlayer.prototype = {
        count: 0,
        version: {
            script: "2.0.0",
            needFlash: "2.0.0",
            flash: "unknown"
        },
        options: {
            swfPath: "js",
            solution: "html, flash",
            supplied: "mp3",
            preload: "metadata",
            volume: .8,
            muted: !1,
            backgroundColor: "#000000",
            cssSelectorAncestor: "#jp_interface_1",
            cssSelector: {
                videoPlay: ".jp-video-play",
                play: ".jp-play",
                pause: ".jp-pause",
                stop: ".jp-stop",
                seekBar: ".jp-seek-bar",
                playBar: ".jp-play-bar",
                mute: ".jp-mute",
                unmute: ".jp-unmute",
                volumeBar: ".jp-volume-bar",
                volumeBarValue: ".jp-volume-bar-value",
                currentTime: ".jp-current-time",
                duration: ".jp-duration"
            },
            idPrefix: "jp",
            errorAlerts: !1,
            warningAlerts: !1
        },
        instances: {},
        status: {
            src: "",
            media: {},
            paused: !0,
            format: {},
            formatType: "",
            waitForPlay: !0,
            waitForLoad: !0,
            srcSet: !1,
            video: !1,
            seekPercent: 0,
            currentPercentRelative: 0,
            currentPercentAbsolute: 0,
            currentTime: 0,
            duration: 0
        },
        _status: {
            volume: h,
            muted: !1,
            width: 0,
            height: 0
        },
        internal: {
            ready: !1,
            instance: h,
            htmlDlyCmdId: h
        },
        solution: {
            html: !0,
            flash: !0
        },
        format: {
            mp3: {
                codec: 'audio/mpeg; codecs="mp3"',
                flashCanPlay: !0,
                media: "audio"
            },
            m4a: {
                codec: 'audio/mp4; codecs="mp4a.40.2"',
                flashCanPlay: !0,
                media: "audio"
            },
            oga: {
                codec: 'audio/ogg; codecs="vorbis"',
                flashCanPlay: !1,
                media: "audio"
            },
            wav: {
                codec: 'audio/wav; codecs="1"',
                flashCanPlay: !1,
                media: "audio"
            },
            webma: {
                codec: 'audio/webm; codecs="vorbis"',
                flashCanPlay: !1,
                media: "audio"
            },
            m4v: {
                codec: 'video/mp4; codecs="avc1.42E01E, mp4a.40.2"',
                flashCanPlay: !0,
                media: "video"
            },
            ogv: {
                codec: 'video/ogg; codecs="theora, vorbis"',
                flashCanPlay: !1,
                media: "video"
            },
            webmv: {
                codec: 'video/webm; codecs="vorbis, vp8"',
                flashCanPlay: !1,
                media: "video"
            }
        },
        _init: function () {
            var a = this;
            if (this.element.empty(), this.status = c.extend({}, this.status, this._status), this.internal = c.extend({}, this.internal), this.formats = [], this.solutions = [], this.require = {}, this.htmlElement = {}, this.html = {}, this.html.audio = {}, this.html.video = {}, this.flash = {}, this.css = {}, this.css.cs = {}, this.css.jq = {}, this.status.volume = this._limitValue(this.options.volume, 0, 1), this.status.muted = this.options.muted, this.status.width = this.element.css("width"), this.status.height = this.element.css("height"), this.element.css({
                    "background-color": this.options.backgroundColor
                }), c.each(this.options.supplied.toLowerCase().split(","), function (e, g) {
                    var i = g.replace(/^\s+|\s+$/g, "");
                    if (a.format[i]) {
                        var j = !1;
                        c.each(a.formats, function (n, k) {
                            return i === k ? (j = !0, !1) : void 0
                        }), j || a.formats.push(i)
                    }
                }), c.each(this.options.solution.toLowerCase().split(","), function (e, g) {
                    var i = g.replace(/^\s+|\s+$/g, "");
                    if (a.solution[i]) {
                        var j = !1;
                        c.each(a.solutions, function (n, k) {
                            return i === k ? (j = !0, !1) : void 0
                        }), j || a.solutions.push(i)
                    }
                }), this.internal.instance = "jp_" + this.count, this.instances[this.internal.instance] = this.element, "" === this.element.attr("id") && this.element.attr("id", this.options.idPrefix + "_jplayer_" + this.count), this.internal.self = c.extend({}, {
                    id: this.element.attr("id"),
                    jq: this.element
                }), this.internal.audio = c.extend({}, {
                    id: this.options.idPrefix + "_audio_" + this.count,
                    jq: h
                }), this.internal.video = c.extend({}, {
                    id: this.options.idPrefix + "_video_" + this.count,
                    jq: h
                }), this.internal.flash = c.extend({}, {
                    id: this.options.idPrefix + "_flash_" + this.count,
                    jq: h,
                    swf: this.options.swfPath + ("" !== this.options.swfPath && "/" !== this.options.swfPath.slice(-1) ? "/" : "") + "Jplayer.swf"
                }), this.internal.poster = c.extend({}, {
                    id: this.options.idPrefix + "_poster_" + this.count,
                    jq: h
                }), c.each(c.jPlayer.event, function (e, g) {
                    a.options[e] !== h && (a.element.bind(g + ".jPlayer", a.options[e]), a.options[e] = h)
                }), this.htmlElement.poster = document.createElement("img"), this.htmlElement.poster.id = this.internal.poster.id, this.htmlElement.poster.onload = function () {
                    (!a.status.video || a.status.waitForPlay) && a.internal.poster.jq.show()
                }, this.element.append(this.htmlElement.poster), this.internal.poster.jq = c("#" + this.internal.poster.id), this.internal.poster.jq.css({
                    width: this.status.width,
                    height: this.status.height
                }), this.internal.poster.jq.hide(), this.require.audio = !1, this.require.video = !1, c.each(this.formats, function (e, g) {
                    a.require[a.format[g].media] = !0
                }), this.html.audio.available = !1, this.require.audio && (this.htmlElement.audio = document.createElement("audio"), this.htmlElement.audio.id = this.internal.audio.id, this.html.audio.available = !!this.htmlElement.audio.canPlayType), this.html.video.available = !1, this.require.video && (this.htmlElement.video = document.createElement("video"), this.htmlElement.video.id = this.internal.video.id, this.html.video.available = !!this.htmlElement.video.canPlayType), this.flash.available = this._checkForFlash(10), this.html.canPlay = {}, this.flash.canPlay = {}, c.each(this.formats, function (e, g) {
                    a.html.canPlay[g] = a.html[a.format[g].media].available && "" !== a.htmlElement[a.format[g].media].canPlayType(a.format[g].codec), a.flash.canPlay[g] = a.format[g].flashCanPlay && a.flash.available
                }), this.html.desired = !1, this.flash.desired = !1, c.each(this.solutions, function (e, g) {
                    if (0 === e) a[g].desired = !0;
                    else {
                        var i = !1,
                            j = !1;
                        c.each(a.formats, function (n, k) {
                            a[a.solutions[0]].canPlay[k] && ("video" === a.format[k].media ? j = !0 : i = !0)
                        }), a[g].desired = a.require.audio && !i || a.require.video && !j
                    }
                }), this.html.support = {}, this.flash.support = {}, c.each(this.formats, function (e, g) {
                    a.html.support[g] = a.html.canPlay[g] && a.html.desired, a.flash.support[g] = a.flash.canPlay[g] && a.flash.desired
                }), this.html.used = !1, this.flash.used = !1, c.each(this.solutions, function (e, g) {
                    c.each(a.formats, function (i, j) {
                        return a[g].support[j] ? (a[g].used = !0, !1) : void 0
                    })
                }), this.html.used || this.flash.used || this._error({
                    type: c.jPlayer.error.NO_SOLUTION,
                    context: "{solution:'" + this.options.solution + "', supplied:'" + this.options.supplied + "'}",
                    message: c.jPlayer.errorMsg.NO_SOLUTION,
                    hint: c.jPlayer.errorHint.NO_SOLUTION
                }), this.html.active = !1, this.html.audio.gate = !1, this.html.video.gate = !1, this.flash.active = !1, this.flash.gate = !1, this.flash.used) {
                var b = "id=" + escape(this.internal.self.id) + "&vol=" + this.status.volume + "&muted=" + this.status.muted;
                if (c.browser.msie && 8 >= Number(c.browser.version)) {
                    var d = '<object id="' + this.internal.flash.id + '"';
                    d += ' classid="clsid:d27cdb6e-ae6d-11cf-96b8-444553540000"', d += ' codebase="' + document.URL.substring(0, document.URL.indexOf(":")) + '://fpdownload.macromedia.com/pub/shockwave/cabs/flash/swflash.cab"', d += ' type="application/x-shockwave-flash"', d += ' width="0" height="0">', d += "</object>";
                    var f = [];
                    for (f[0] = '<param name="movie" value="' + this.internal.flash.swf + '" />', f[1] = '<param name="quality" value="high" />', f[2] = '<param name="FlashVars" value="' + b + '" />', f[3] = '<param name="allowScriptAccess" value="always" />', f[4] = '<param name="bgcolor" value="' + this.options.backgroundColor + '" />', b = document.createElement(d), d = 0; f.length > d; d++) b.appendChild(document.createElement(f[d]));
                    this.element.append(b)
                } else f = '<embed name="' + this.internal.flash.id + '" id="' + this.internal.flash.id + '" src="' + this.internal.flash.swf + '"', f += ' width="0" height="0" bgcolor="' + this.options.backgroundColor + '"', f += ' quality="high" FlashVars="' + b + '"', f += ' allowScriptAccess="always"', f += ' type="application/x-shockwave-flash" pluginspage="http://www.macromedia.com/go/getflashplayer" />', this.element.append(f);
                this.internal.flash.jq = c("#" + this.internal.flash.id), this.internal.flash.jq.css({
                    width: "0px",
                    height: "0px"
                })
            }
            this.html.used && (this.html.audio.available && (this._addHtmlEventListeners(this.htmlElement.audio, this.html.audio), this.element.append(this.htmlElement.audio), this.internal.audio.jq = c("#" + this.internal.audio.id)), this.html.video.available && (this._addHtmlEventListeners(this.htmlElement.video, this.html.video), this.element.append(this.htmlElement.video), this.internal.video.jq = c("#" + this.internal.video.id), this.internal.video.jq.css({
                width: "0px",
                height: "0px"
            }))), this.html.used && !this.flash.used && window.setTimeout(function () {
                a.internal.ready = !0, a.version.flash = "n/a", a._trigger(c.jPlayer.event.ready)
            }, 100), c.each(this.options.cssSelector, function (e, g) {
                a._cssSelector(e, g)
            }), this._updateInterface(), this._updateButtons(!1), this._updateVolume(this.status.volume), this._updateMute(this.status.muted), this.css.jq.videoPlay.length && this.css.jq.videoPlay.hide(), c.jPlayer.prototype.count++
        },
        destroy: function () {
            this._resetStatus(), this._updateInterface(), this._seeked(), this.css.jq.currentTime.length && this.css.jq.currentTime.text(""), this.css.jq.duration.length && this.css.jq.duration.text(""), this.status.srcSet && this.pause(), c.each(this.css.jq, function (a, b) {
                b.unbind(".jPlayer")
            }), this.element.removeData("jPlayer"), this.element.unbind(".jPlayer"), this.element.empty(), this.instances[this.internal.instance] = h
        },
        enable: function () {
        },
        disable: function () {
        },
        _addHtmlEventListeners: function (a, b) {
            var d = this;
            a.preload = this.options.preload, a.muted = this.options.muted, a.addEventListener("progress", function () {
                b.gate && !d.status.waitForLoad && (d._getHtmlStatus(a), d._updateInterface(), d._trigger(c.jPlayer.event.progress))
            }, !1), a.addEventListener("timeupdate", function () {
                b.gate && !d.status.waitForLoad && (d._getHtmlStatus(a), d._updateInterface(), d._trigger(c.jPlayer.event.timeupdate))
            }, !1), a.addEventListener("durationchange", function () {
                b.gate && !d.status.waitForLoad && (d.status.duration = this.duration, d._getHtmlStatus(a), d._updateInterface(), d._trigger(c.jPlayer.event.durationchange))
            }, !1), a.addEventListener("play", function () {
                b.gate && !d.status.waitForLoad && (d._updateButtons(!0), d._trigger(c.jPlayer.event.play))
            }, !1), a.addEventListener("playing", function () {
                b.gate && !d.status.waitForLoad && (d._updateButtons(!0), d._seeked(), d._trigger(c.jPlayer.event.playing))
            }, !1), a.addEventListener("pause", function () {
                b.gate && !d.status.waitForLoad && (d._updateButtons(!1), d._trigger(c.jPlayer.event.pause))
            }, !1), a.addEventListener("waiting", function () {
                b.gate && !d.status.waitForLoad && (d._seeking(), d._trigger(c.jPlayer.event.waiting))
            }, !1), a.addEventListener("canplay", function () {
                b.gate && !d.status.waitForLoad && (a.volume = d._volumeFix(d.status.volume), d._trigger(c.jPlayer.event.canplay))
            }, !1), a.addEventListener("seeking", function () {
                b.gate && !d.status.waitForLoad && (d._seeking(), d._trigger(c.jPlayer.event.seeking))
            }, !1), a.addEventListener("seeked", function () {
                b.gate && !d.status.waitForLoad && (d._seeked(), d._trigger(c.jPlayer.event.seeked))
            }, !1), a.addEventListener("suspend", function () {
                b.gate && !d.status.waitForLoad && (d._seeked(), d._trigger(c.jPlayer.event.suspend))
            }, !1), a.addEventListener("ended", function () {
                b.gate && !d.status.waitForLoad && (c.jPlayer.browser.webkit || (d.htmlElement.media.currentTime = 0), d.htmlElement.media.pause(), d._updateButtons(!1), d._getHtmlStatus(a, !0), d._updateInterface(), d._trigger(c.jPlayer.event.ended))
            }, !1), a.addEventListener("error", function () {
                b.gate && !d.status.waitForLoad && (d._updateButtons(!1), d._seeked(), d.status.srcSet && (d.status.waitForLoad = !0, d.status.waitForPlay = !0, d.status.video && d.internal.video.jq.css({
                    width: "0px",
                    height: "0px"
                }), d._validString(d.status.media.poster) && d.internal.poster.jq.show(), d.css.jq.videoPlay.length && d.css.jq.videoPlay.show(), d._error({
                    type: c.jPlayer.error.URL,
                    context: d.status.src,
                    message: c.jPlayer.errorMsg.URL,
                    hint: c.jPlayer.errorHint.URL
                })))
            }, !1), c.each(c.jPlayer.htmlEvent, function (f, e) {
                a.addEventListener(this, function () {
                    b.gate && !d.status.waitForLoad && d._trigger(c.jPlayer.event[e])
                }, !1)
            })
        },
        _getHtmlStatus: function (a, b) {
            var d = 0,
                f = 0,
                e = 0,
                g = 0;
            d = a.currentTime, f = this.status.duration > 0 ? 100 * d / this.status.duration : 0, "object" == typeof a.seekable && a.seekable.length > 0 ? (e = this.status.duration > 0 ? 100 * a.seekable.end(a.seekable.length - 1) / this.status.duration : 100, g = 100 * a.currentTime / a.seekable.end(a.seekable.length - 1)) : (e = 100, g = f), b && (f = g = d = 0), this.status.seekPercent = e, this.status.currentPercentRelative = g, this.status.currentPercentAbsolute = f, this.status.currentTime = d
        },
        _resetStatus: function () {
            this.status = c.extend({}, this.status, c.jPlayer.prototype.status)
        },
        _trigger: function (a, b, d) {
            a = c.Event(a), a.jPlayer = {}, a.jPlayer.version = c.extend({}, this.version), a.jPlayer.status = c.extend(!0, {}, this.status), a.jPlayer.html = c.extend(!0, {}, this.html), a.jPlayer.flash = c.extend(!0, {}, this.flash), b && (a.jPlayer.error = c.extend({}, b)), d && (a.jPlayer.warning = c.extend({}, d)), this.element.trigger(a)
        },
        jPlayerFlashEvent: function (a, b) {
            if (a !== c.jPlayer.event.ready || this.internal.ready || (this.internal.ready = !0, this.version.flash = b.version, this.version.needFlash !== this.version.flash && this._error({
                    type: c.jPlayer.error.VERSION,
                    context: this.version.flash,
                    message: c.jPlayer.errorMsg.VERSION + this.version.flash,
                    hint: c.jPlayer.errorHint.VERSION
                }), this._trigger(a)), this.flash.gate) switch (a) {
                case c.jPlayer.event.progress:
                    this._getFlashStatus(b), this._updateInterface(), this._trigger(a);
                    break;
                case c.jPlayer.event.timeupdate:
                    this._getFlashStatus(b), this._updateInterface(), this._trigger(a);
                    break;
                case c.jPlayer.event.play:
                    this._seeked(), this._updateButtons(!0), this._trigger(a);
                    break;
                case c.jPlayer.event.pause:
                    this._updateButtons(!1), this._trigger(a);
                    break;
                case c.jPlayer.event.ended:
                    this._updateButtons(!1), this._trigger(a);
                    break;
                case c.jPlayer.event.error:
                    this.status.waitForLoad = !0, this.status.waitForPlay = !0, this.status.video && this.internal.flash.jq.css({
                        width: "0px",
                        height: "0px"
                    }), this._validString(this.status.media.poster) && this.internal.poster.jq.show(), this.css.jq.videoPlay.length && this.css.jq.videoPlay.show(), this.status.video ? this._flash_setVideo(this.status.media) : this._flash_setAudio(this.status.media), this._error({
                        type: c.jPlayer.error.URL,
                        context: b.src,
                        message: c.jPlayer.errorMsg.URL,
                        hint: c.jPlayer.errorHint.URL
                    });
                    break;
                case c.jPlayer.event.seeking:
                    this._seeking(), this._trigger(a);
                    break;
                case c.jPlayer.event.seeked:
                    this._seeked(), this._trigger(a);
                    break;
                default:
                    this._trigger(a)
            }
            return !1
        },
        _getFlashStatus: function (a) {
            this.status.seekPercent = a.seekPercent, this.status.currentPercentRelative = a.currentPercentRelative, this.status.currentPercentAbsolute = a.currentPercentAbsolute, this.status.currentTime = a.currentTime, this.status.duration = a.duration
        },
        _updateButtons: function (a) {
            this.status.paused = !a, this.css.jq.play.length && this.css.jq.pause.length && (a ? (this.css.jq.play.hide(), this.css.jq.pause.show()) : (this.css.jq.play.show(), this.css.jq.pause.hide()))
        },
        _updateInterface: function () {
            this.css.jq.seekBar.length && this.css.jq.seekBar.width(this.status.seekPercent + "%"), this.css.jq.playBar.length && this.css.jq.playBar.width(this.status.currentPercentRelative + "%"), this.css.jq.currentTime.length && this.css.jq.currentTime.text(c.jPlayer.convertTime(this.status.currentTime)), this.css.jq.duration.length && this.css.jq.duration.text(c.jPlayer.convertTime(this.status.duration))
        },
        _seeking: function () {
            this.css.jq.seekBar.length && this.css.jq.seekBar.addClass("jp-seeking-bg")
        },
        _seeked: function () {
            this.css.jq.seekBar.length && this.css.jq.seekBar.removeClass("jp-seeking-bg")
        },
        setMedia: function (a) {
            var b = this;
            this._seeked(), clearTimeout(this.internal.htmlDlyCmdId);
            var d = this.html.audio.gate,
                f = this.html.video.gate,
                e = !1;
            c.each(this.formats, function (g, i) {
                var j = "video" === b.format[i].media;
                return c.each(b.solutions, function (n, k) {
                    if (b[k].support[i] && b._validString(a[i])) {
                        var l = "html" === k;
                        return j ? l ? (b.html.audio.gate = !1, b.html.video.gate = !0, b.flash.gate = !1) : (b.html.audio.gate = !1, b.html.video.gate = !1, b.flash.gate = !0) : l ? (b.html.audio.gate = !0, b.html.video.gate = !1, b.flash.gate = !1) : (b.html.audio.gate = !1, b.html.video.gate = !1, b.flash.gate = !0), b.flash.active || b.html.active && b.flash.gate || d === b.html.audio.gate && f === b.html.video.gate ? b.clearMedia() : d !== b.html.audio.gate && f !== b.html.video.gate && (b._html_pause(), b.status.video && b.internal.video.jq.css({
                            width: "0px",
                            height: "0px"
                        }), b._resetStatus()), j ? (l ? (b._html_setVideo(a), b.html.active = !0, b.flash.active = !1) : (b._flash_setVideo(a), b.html.active = !1, b.flash.active = !0), b.css.jq.videoPlay.length && b.css.jq.videoPlay.show(), b.status.video = !0) : (l ? (b._html_setAudio(a), b.html.active = !0, b.flash.active = !1) : (b._flash_setAudio(a), b.html.active = !1, b.flash.active = !0), b.css.jq.videoPlay.length && b.css.jq.videoPlay.hide(), b.status.video = !1), e = !0, !1
                    }
                }), e ? !1 : void 0
            }), e ? (this._validString(a.poster) ? this.htmlElement.poster.src !== a.poster ? this.htmlElement.poster.src = a.poster : this.internal.poster.jq.show() : this.internal.poster.jq.hide(), this.status.srcSet = !0, this.status.media = c.extend({}, a), this._updateButtons(!1), this._updateInterface()) : (this.status.srcSet && !this.status.waitForPlay && this.pause(), this.html.audio.gate = !1, this.html.video.gate = !1, this.flash.gate = !1, this.html.active = !1, this.flash.active = !1, this._resetStatus(), this._updateInterface(), this._updateButtons(!1), this.internal.poster.jq.hide(), this.html.used && this.require.video && this.internal.video.jq.css({
                width: "0px",
                height: "0px"
            }), this.flash.used && this.internal.flash.jq.css({
                width: "0px",
                height: "0px"
            }), this._error({
                type: c.jPlayer.error.NO_SUPPORT,
                context: "{supplied:'" + this.options.supplied + "'}",
                message: c.jPlayer.errorMsg.NO_SUPPORT,
                hint: c.jPlayer.errorHint.NO_SUPPORT
            }))
        },
        clearMedia: function () {
            this._resetStatus(), this._updateButtons(!1), this.internal.poster.jq.hide(), clearTimeout(this.internal.htmlDlyCmdId), this.html.active ? this._html_clearMedia() : this.flash.active && this._flash_clearMedia()
        },
        load: function () {
            this.status.srcSet ? this.html.active ? this._html_load() : this.flash.active && this._flash_load() : this._urlNotSetError("load")
        },
        play: function (a) {
            a = "number" == typeof a ? a : 0 / 0, this.status.srcSet ? this.html.active ? this._html_play(a) : this.flash.active && this._flash_play(a) : this._urlNotSetError("play")
        },
        videoPlay: function () {
            this.play()
        },
        pause: function (a) {
            a = "number" == typeof a ? a : 0 / 0, this.status.srcSet ? this.html.active ? this._html_pause(a) : this.flash.active && this._flash_pause(a) : this._urlNotSetError("pause")
        },
        pauseOthers: function () {
            var a = this;
            c.each(this.instances, function (b, d) {
                a.element !== d && d.data("jPlayer").status.srcSet && d.jPlayer("pause")
            })
        },
        stop: function () {
            this.status.srcSet ? this.html.active ? this._html_pause(0) : this.flash.active && this._flash_pause(0) : this._urlNotSetError("stop")
        },
        playHead: function (a) {
            a = this._limitValue(a, 0, 100), this.status.srcSet ? this.html.active ? this._html_playHead(a) : this.flash.active && this._flash_playHead(a) : this._urlNotSetError("playHead")
        },
        mute: function () {
            this.status.muted = !0, this.html.used && this._html_mute(!0), this.flash.used && this._flash_mute(!0), this._updateMute(!0), this._updateVolume(0), this._trigger(c.jPlayer.event.volumechange)
        },
        unmute: function () {
            this.status.muted = !1, this.html.used && this._html_mute(!1), this.flash.used && this._flash_mute(!1), this._updateMute(!1), this._updateVolume(this.status.volume), this._trigger(c.jPlayer.event.volumechange)
        },
        _updateMute: function (a) {
            this.css.jq.mute.length && this.css.jq.unmute.length && (a ? (this.css.jq.mute.hide(), this.css.jq.unmute.show()) : (this.css.jq.mute.show(), this.css.jq.unmute.hide()))
        },
        volume: function (a) {
            a = this._limitValue(a, 0, 1), this.status.volume = a, this.html.used && this._html_volume(a), this.flash.used && this._flash_volume(a), this.status.muted || this._updateVolume(a), this._trigger(c.jPlayer.event.volumechange)
        },
        volumeBar: function (a) {
            if (!this.status.muted && this.css.jq.volumeBar) {
                var b = this.css.jq.volumeBar.offset();
                a = a.pageX - b.left, b = this.css.jq.volumeBar.width(), this.volume(a / b)
            }
        },
        volumeBarValue: function (a) {
            this.volumeBar(a)
        },
        _updateVolume: function (a) {
            this.css.jq.volumeBarValue.length && this.css.jq.volumeBarValue.width(100 * a + "%")
        },
        _volumeFix: function (a) {
            var b = .001 * Math.random();
            return a + (.5 > a ? b : -b)
        },
        _cssSelectorAncestor: function (a, b) {
            this.options.cssSelectorAncestor = a, b && c.each(this.options.cssSelector, function (d, f) {
                self._cssSelector(d, f)
            })
        },
        _cssSelector: function (a, b) {
            var d = this;
            "string" == typeof b ? c.jPlayer.prototype.options.cssSelector[a] ? (this.css.jq[a] && this.css.jq[a].length && this.css.jq[a].unbind(".jPlayer"), this.options.cssSelector[a] = b, this.css.cs[a] = this.options.cssSelectorAncestor + " " + b, this.css.jq[a] = b ? c(this.css.cs[a]) : [], this.css.jq[a].length && this.css.jq[a].bind("click.jPlayer", function (f) {
                return d[a](f), c(this).blur(), !1
            }), b && 1 !== this.css.jq[a].length && this._warning({
                type: c.jPlayer.warning.CSS_SELECTOR_COUNT,
                context: this.css.cs[a],
                message: c.jPlayer.warningMsg.CSS_SELECTOR_COUNT + this.css.jq[a].length + " found for " + a + " method.",
                hint: c.jPlayer.warningHint.CSS_SELECTOR_COUNT
            })) : this._warning({
                type: c.jPlayer.warning.CSS_SELECTOR_METHOD,
                context: a,
                message: c.jPlayer.warningMsg.CSS_SELECTOR_METHOD,
                hint: c.jPlayer.warningHint.CSS_SELECTOR_METHOD
            }) : this._warning({
                type: c.jPlayer.warning.CSS_SELECTOR_STRING,
                context: b,
                message: c.jPlayer.warningMsg.CSS_SELECTOR_STRING,
                hint: c.jPlayer.warningHint.CSS_SELECTOR_STRING
            })
        },
        seekBar: function (a) {
            if (this.css.jq.seekBar) {
                var b = this.css.jq.seekBar.offset();
                a = a.pageX - b.left, b = this.css.jq.seekBar.width(), this.playHead(100 * a / b)
            }
        },
        playBar: function (a) {
            this.seekBar(a)
        },
        currentTime: function () {
        },
        duration: function () {
        },
        option: function (a, b) {
            var d = a;
            if (0 === arguments.length) return c.extend(!0, {}, this.options);
            if ("string" == typeof a) {
                var f = a.split(".");
                if (b === h) {
                    for (var e = c.extend(!0, {}, this.options), g = 0; f.length > g; g++) {
                        if (e[f[g]] === h) return this._warning({
                            type: c.jPlayer.warning.OPTION_KEY,
                            context: a,
                            message: c.jPlayer.warningMsg.OPTION_KEY,
                            hint: c.jPlayer.warningHint.OPTION_KEY
                        }), h;
                        e = e[f[g]]
                    }
                    return e
                }
                for (e = d = {}, g = 0; f.length > g; g++) f.length - 1 > g ? (e[f[g]] = {}, e = e[f[g]]) : e[f[g]] = b
            }
            return this._setOptions(d), this
        },
        _setOptions: function (a) {
            var b = this;
            return c.each(a, function (d, f) {
                b._setOption(d, f)
            }), this
        },
        _setOption: function (a, b) {
            var d = this;
            switch (a) {
                case "cssSelectorAncestor":
                    this.options[a] = b, c.each(d.options.cssSelector, function (f, e) {
                        d._cssSelector(f, e)
                    });
                    break;
                case "cssSelector":
                    c.each(b, function (f, e) {
                        d._cssSelector(f, e)
                    })
            }
            return this
        },
        resize: function (a) {
            this.html.active && this._resizeHtml(a), this.flash.active && this._resizeFlash(a), this._trigger(c.jPlayer.event.resize)
        },
        _resizePoster: function () {
        },
        _resizeHtml: function () {
        },
        _resizeFlash: function (a) {
            this.internal.flash.jq.css({
                width: a.width,
                height: a.height
            })
        },
        _html_initMedia: function () {
            this.status.srcSet && !this.status.waitForPlay && this.htmlElement.media.pause(), "none" !== this.options.preload && this._html_load(), this._trigger(c.jPlayer.event.timeupdate)
        },
        _html_setAudio: function (a) {
            var b = this;
            c.each(this.formats, function (d, f) {
                return b.html.support[f] && a[f] ? (b.status.src = a[f], b.status.format[f] = !0, b.status.formatType = f, !1) : void 0
            }), this.htmlElement.media = this.htmlElement.audio, this._html_initMedia()
        },
        _html_setVideo: function (a) {
            var b = this;
            c.each(this.formats, function (d, f) {
                return b.html.support[f] && a[f] ? (b.status.src = a[f], b.status.format[f] = !0, b.status.formatType = f, !1) : void 0
            }), this.htmlElement.media = this.htmlElement.video, this._html_initMedia()
        },
        _html_clearMedia: function () {
            this.htmlElement.media && (this.htmlElement.media.id === this.internal.video.id && this.internal.video.jq.css({
                width: "0px",
                height: "0px"
            }), this.htmlElement.media.pause(), this.htmlElement.media.src = "", c.browser.msie && Number(c.browser.version) >= 9 || this.htmlElement.media.load())
        },
        _html_load: function () {
            if (this.status.waitForLoad) {
                this.status.waitForLoad = !1, this.htmlElement.media.src = this.status.src;
                try {
                    this.htmlElement.media.load()
                } catch (a) {
                }
            }
            clearTimeout(this.internal.htmlDlyCmdId)
        },
        _html_play: function (a) {
            var b = this;
            if (this._html_load(), this.htmlElement.media.play(), !isNaN(a)) try {
                this.htmlElement.media.currentTime = a
            } catch (d) {
                return this.internal.htmlDlyCmdId = setTimeout(function () {
                    b.play(a)
                }, 100), void 0
            }
            this._html_checkWaitForPlay()
        },
        _html_pause: function (a) {
            var b = this;
            if (a > 0 ? this._html_load() : clearTimeout(this.internal.htmlDlyCmdId), this.htmlElement.media.pause(), !isNaN(a)) try {
                this.htmlElement.media.currentTime = a
            } catch (d) {
                return this.internal.htmlDlyCmdId = setTimeout(function () {
                    b.pause(a)
                }, 100), void 0
            }
            a > 0 && this._html_checkWaitForPlay()
        },
        _html_playHead: function (a) {
            var b = this;
            this._html_load();
            try {
                if ("object" == typeof this.htmlElement.media.seekable && this.htmlElement.media.seekable.length > 0) this.htmlElement.media.currentTime = a * this.htmlElement.media.seekable.end(this.htmlElement.media.seekable.length - 1) / 100;
                else {
                    if (!(this.htmlElement.media.duration > 0) || isNaN(this.htmlElement.media.duration)) throw "e";
                    this.htmlElement.media.currentTime = a * this.htmlElement.media.duration / 100
                }
            } catch (d) {
                return this.internal.htmlDlyCmdId = setTimeout(function () {
                    b.playHead(a)
                }, 100), void 0
            }
            this.status.waitForLoad || this._html_checkWaitForPlay()
        },
        _html_checkWaitForPlay: function () {
            this.status.waitForPlay && (this.status.waitForPlay = !1, this.css.jq.videoPlay.length && this.css.jq.videoPlay.hide(), this.status.video && (this.internal.poster.jq.hide(), this.internal.video.jq.css({
                width: this.status.width,
                height: this.status.height
            })))
        },
        _html_volume: function (a) {
            this.html.audio.available && (this.htmlElement.audio.volume = a), this.html.video.available && (this.htmlElement.video.volume = a)
        },
        _html_mute: function (a) {
            this.html.audio.available && (this.htmlElement.audio.muted = a), this.html.video.available && (this.htmlElement.video.muted = a)
        },
        _flash_setAudio: function (a) {
            var b = this;
            try {
                c.each(this.formats, function (f, e) {
                    if (b.flash.support[e] && a[e]) {
                        switch (e) {
                            case "m4a":
                                b._getMovie().fl_setAudio_m4a(a[e]);
                                break;
                            case "mp3":
                                b._getMovie().fl_setAudio_mp3(a[e])
                        }
                        return b.status.src = a[e], b.status.format[e] = !0, b.status.formatType = e, !1
                    }
                }), "auto" === this.options.preload && (this._flash_load(), this.status.waitForLoad = !1)
            } catch (d) {
                this._flashError(d)
            }
        },
        _flash_setVideo: function (a) {
            var b = this;
            try {
                c.each(this.formats, function (f, e) {
                    if (b.flash.support[e] && a[e]) {
                        switch (e) {
                            case "m4v":
                                b._getMovie().fl_setVideo_m4v(a[e])
                        }
                        return b.status.src = a[e], b.status.format[e] = !0, b.status.formatType = e, !1
                    }
                }), "auto" === this.options.preload && (this._flash_load(), this.status.waitForLoad = !1)
            } catch (d) {
                this._flashError(d)
            }
        },
        _flash_clearMedia: function () {
            this.internal.flash.jq.css({
                width: "0px",
                height: "0px"
            });
            try {
                this._getMovie().fl_clearMedia()
            } catch (a) {
                this._flashError(a)
            }
        },
        _flash_load: function () {
            try {
                this._getMovie().fl_load()
            } catch (a) {
                this._flashError(a)
            }
            this.status.waitForLoad = !1
        },
        _flash_play: function (a) {
            try {
                this._getMovie().fl_play(a)
            } catch (b) {
                this._flashError(b)
            }
            this.status.waitForLoad = !1, this._flash_checkWaitForPlay()
        },
        _flash_pause: function (a) {
            try {
                this._getMovie().fl_pause(a)
            } catch (b) {
                this._flashError(b)
            }
            a > 0 && (this.status.waitForLoad = !1, this._flash_checkWaitForPlay())
        },
        _flash_playHead: function (a) {
            try {
                this._getMovie().fl_play_head(a)
            } catch (b) {
                this._flashError(b)
            }
            this.status.waitForLoad || this._flash_checkWaitForPlay()
        },
        _flash_checkWaitForPlay: function () {
            this.status.waitForPlay && (this.status.waitForPlay = !1, this.css.jq.videoPlay.length && this.css.jq.videoPlay.hide(), this.status.video && (this.internal.poster.jq.hide(), this.internal.flash.jq.css({
                width: this.status.width,
                height: this.status.height
            })))
        },
        _flash_volume: function (a) {
            try {
                this._getMovie().fl_volume(a)
            } catch (b) {
                this._flashError(b)
            }
        },
        _flash_mute: function (a) {
            try {
                this._getMovie().fl_mute(a)
            } catch (b) {
                this._flashError(b)
            }
        },
        _getMovie: function () {
            return document[this.internal.flash.id]
        },
        _checkForFlash: function (a) {
            var d, b = !1;
            if (window.ActiveXObject) try {
                new ActiveXObject("ShockwaveFlash.ShockwaveFlash." + a), b = !0
            } catch (f) {
            } else navigator.plugins && navigator.mimeTypes.length > 0 && (d = navigator.plugins["Shockwave Flash"]) && navigator.plugins["Shockwave Flash"].description.replace(/.*\s(\d+\.\d+).*/, "$1") >= a && (b = !0);
            return c.browser.msie && Number(c.browser.version) >= 9 ? !1 : b
        },
        _validString: function (a) {
            return a && "string" == typeof a
        },
        _limitValue: function (a, b, d) {
            return b > a ? b : a > d ? d : a
        },
        _urlNotSetError: function (a) {
            this._error({
                type: c.jPlayer.error.URL_NOT_SET,
                context: a,
                message: c.jPlayer.errorMsg.URL_NOT_SET,
                hint: c.jPlayer.errorHint.URL_NOT_SET
            })
        },
        _flashError: function (a) {
            this._error({
                type: c.jPlayer.error.FLASH,
                context: this.internal.flash.swf,
                message: c.jPlayer.errorMsg.FLASH + a.message,
                hint: c.jPlayer.errorHint.FLASH
            })
        },
        _error: function (a) {
            this._trigger(c.jPlayer.event.error, a), this.options.errorAlerts && this._alert("Error!" + (a.message ? "\n\n" + a.message : "") + (a.hint ? "\n\n" + a.hint : "") + "\n\nContext: " + a.context)
        },
        _warning: function (a) {
            this._trigger(c.jPlayer.event.warning, h, a), this.options.errorAlerts && this._alert("Warning!" + (a.message ? "\n\n" + a.message : "") + (a.hint ? "\n\n" + a.hint : "") + "\n\nContext: " + a.context)
        },
        _alert: function (a) {
            alert("jPlayer " + this.version.script + " : id='" + this.internal.self.id + "' : " + a)
        }
    }, c.jPlayer.error = {
        FLASH: "e_flash",
        NO_SOLUTION: "e_no_solution",
        NO_SUPPORT: "e_no_support",
        URL: "e_url",
        URL_NOT_SET: "e_url_not_set",
        VERSION: "e_version"
    }, c.jPlayer.errorMsg = {
        FLASH: "jPlayer's Flash fallback is not configured correctly, or a command was issued before the jPlayer Ready event. Details: ",
        NO_SOLUTION: "No solution can be found by jPlayer in this browser. Neither HTML nor Flash can be used.",
        NO_SUPPORT: "It is not possible to play any media format provided in setMedia() on this browser using your current options.",
        URL: "Media URL could not be loaded.",
        URL_NOT_SET: "Attempt to issue media playback commands, while no media url is set.",
        VERSION: "jPlayer " + c.jPlayer.prototype.version.script + " needs Jplayer.swf version " + c.jPlayer.prototype.version.needFlash + " but found "
    }, c.jPlayer.errorHint = {
        FLASH: "Check your swfPath option and that Jplayer.swf is there.",
        NO_SOLUTION: "Review the jPlayer options: support and supplied.",
        NO_SUPPORT: "Video or audio formats defined in the supplied option are missing.",
        URL: "Check media URL is valid.",
        URL_NOT_SET: "Use setMedia() to set the media URL.",
        VERSION: "Update jPlayer files."
    }, c.jPlayer.warning = {
        CSS_SELECTOR_COUNT: "e_css_selector_count",
        CSS_SELECTOR_METHOD: "e_css_selector_method",
        CSS_SELECTOR_STRING: "e_css_selector_string",
        OPTION_KEY: "e_option_key"
    }, c.jPlayer.warningMsg = {
        CSS_SELECTOR_COUNT: "The number of methodCssSelectors found did not equal one: ",
        CSS_SELECTOR_METHOD: "The methodName given in jPlayer('cssSelector') is not a valid jPlayer method.",
        CSS_SELECTOR_STRING: "The methodCssSelector given in jPlayer('cssSelector') is not a String or is empty.",
        OPTION_KEY: "The option requested in jPlayer('option') is undefined."
    }, c.jPlayer.warningHint = {
        CSS_SELECTOR_COUNT: "Check your css selector and the ancestor.",
        CSS_SELECTOR_METHOD: "Check your method name.",
        CSS_SELECTOR_STRING: "Check your css selector is a string.",
        OPTION_KEY: "Check your option name."
    }
}(jQuery), function ($) {
    function getPlugin() {
        var plugin = null;
        return plugin = isIe ? document.getElementById("ddplugin-msie") : document.getElementById("ddplugin")
    }

    function splitUrlParam(search, param) {
        var value = search.substring(search.indexOf(param));
        return value.indexOf("&") > 0 && (value = value.substring(0, value.indexOf("&"))), value = value.split("=")[1]
    }

    function checkInstall(clsId, functionName) {
        if (!isIe) {
            var mimeType = navigator.mimeTypes["application/dd-plugin"];
            if (mimeType) {
                var plugin = mimeType.enabledPlugin;
                if (plugin) return !0
            }
            return !1
        }
        var pluginDiv = document.createElement("div");
        pluginDiv.id = "pluginDiv", pluginDiv.style.display = "none", pluginDiv.innerHTML = '<object id="objectForDetectPlugin" classid="CLSID:' + clsId + '"></object>', document.body.appendChild(pluginDiv);
        try {
            return void 0 == eval("objectForDetectPlugin." + functionName) ? (pluginDiv.parentNode.removeChild(pluginDiv), !1) : (pluginDiv.parentNode.removeChild(pluginDiv), !0)
        } catch (e) {
            return !1
        }
    }

    function close(settings, statusManager) {
        try {
            var url = "/close.action?t=" + (new Date).getTime(),
                param = statusManager.statusDataMap[statusManager.currentStatus];
            (settings.sid || param && param.lineKey) && (settings.sid && (url += "&sid=" + settings.sid), (param.lineKey || param.lineNo) && (url += "&lineKey=" + param.lineKey + "&lineNo=" + param.lineNo), $.ajax({
                url: url,
                type: "GET",
                async: !1,
                success: function () {
                }
            })), statusManager.currentStatus = statusManager.systemStatus.leave, window.onbeforeunload = function () {
            }, window.opener = null, window.open("", "_self"), window.close()
        } catch (e) {
            try {
                window.open("", "_parent", ""), window.close()
            } catch (e) {
                window.close()
            }
        }
    }

    function startDD(type, value, pin, statusManager, settings, common) {
        try {
            var csid = "B35D742C-5983-40C1-A8C0-A7DBFA2EF05E",
                plugin = getPlugin(csid);
            checkInstall(csid, "CheckVal") ? $.ajax({
                url: "/dd/getAuthId.action?t=" + (new Date).getTime(),
                async: !1,
                data: {
                    pin: pin
                },
                success: function (aid) {
                    if ("" != $.trim(aid)) try {
                        var result = plugin.CheckAndStartClient2(type + "", value + "", pin, aid);
                        1 == result ? (showLoading(), removeLoading(), close(settings, statusManager)) : (pluginStartFailureRecord(settings, pin, result), showMsg(common), removeLoading())
                    } catch (e) {
                        restartDd(plugin, type, value, pin, aid, 1, settings, statusManager)
                    } else removeLoading()
                },
                error: function () {
                    showMsg(common), removeLoading()
                }
            }) : showMsg(common, settings)
        } catch (e) {
            removeLoading(), canChat = !0
        }
    }

    function pluginStartFailureRecord(settings, pin, code) {
        try {
            var browser = BrowserDetect.browser + ":" + BrowserDetect.version,
                os = detectOSDetailNew(),
                desc = "",
                errorInfos = settings.errorMsgManager;
            for (var p in errorInfos) if (errorInfos[p].code === code) {
                desc = errorInfos[p].description;
                break
            }
            var param = {
                reporter: "chat.web",
                errorCode: code,
                userName: pin,
                description: os + ";" + browser + ";" + desc
            };
            $.ajax({
                url: "http://shop.chat.jd.com/client/log/collectErrLog.action",
                type: "GET",
                dataType: "jsonp",
                jsonp: "CallBack",
                data: {
                    client_json: JSON.stringify(param)
                },
                async: !1,
                success: function () {
                }
            })
        } catch (e) {
        }
    }

    function restartDd(plugin, type, value, pin, aid, count, settings, statusManager) {
        var code = 0;
        try {
            code = plugin.CheckAndStartClient2(type + "", value + "", pin, aid)
        } catch (e) {
        }
        if (1 == code) removeLoading(), close(settings, statusManager);
        else if (5 > count) {
            var num = count + 1;
            setTimeout(function () {
                restartDd(plugin, type, value, pin, aid, num, settings, statusManager)
            }, 1e3)
        } else pluginStartFailureRecord(settings, pin, code), removeLoading(), jdchat.dialogBox.winAlert("插件加载失败，暂时进入网页咨询"), $(".thickbox1").css("zIndex", "10000"), canChat = !0
    }

    function log(e) {
        window.console && window.console.log && console.log(e)
    }

    function extractStacktrace(e) {
        return e.stacktrace ? e.stacktrace : e.stack ? e.stack : e.message || e.description
    }

    function buildDownload() {
        var html = "<div class='dd_container'><div class='dd_main'><a href='javascript:;' class='download' hidefocus='true'></a><a href='javascript:;' class='close' hidefocus='true'></a></div></div><div class='dd_mask'></div>";
        $("body").append(html)
    }

    function buildLoading() {
        var html = "<div class='dd_loading' style='display: none;'><div><img src='http://static.360buyimg.com/im/img/dd_loading.gif'/><span class='dd_text'>正在启动京东咚咚客户端...</span></div></div><div class='dd_mask' style='display: none;'></div>";
        $("body").append(html)
    }

    function showLoading() {
        $(".dd_loading").show(), $(".dd_mask").show()
    }

    function removeLoading() {
        $(".dd_loading").remove(), $(".dd_mask").remove()
    }

    function bindEvent(url) {
        $(".dd_main a.download").unbind("click").bind("click", function () {
            return window.open(url, "_blank"), $(".dd_main a.close").click(), !1
        }), $(".dd_main a.close").unbind("click").bind("click", function () {
            return $(".dd_container").remove(), $(".dd_mask").remove(), !1
        })
    }

    function buildDownloadNew() {
        var html = "<div class='dd_tip_mask'></div><div class='dd_tip_main'><div class='arrow'></div><a href='javascript:;' class='download'></a><a href='javascript:;' class='close'></a></div>";
        $(".im-icon-file").hide(), $(".im-icon-pic").hide(), $("body").append(html)
    }

    function buildDownloadNewV2() {
        var html = "<div class='dd_tip_mask_v2'></div><div class='dd_tip_main_v2'><div class='smoke'></div><a href='javascript:;' class='download'></a><a href='javascript:;' class='close'></a></div>";
        $(".im-icon-file").hide(), $(".im-icon-pic").hide(), $("body").append(html);
        var docHeight = $(document).height(),
            scrollHeight = document.body.scrollHeight;
        $(".dd_tip_mask_v2").height(docHeight > scrollHeight ? docHeight : scrollHeight)
    }

    function buildDownloadNewV3() {
        var html = "<div class='dd_tip_mask_v3'></div><div class='girl'></div><div class='dd_tip_main_v3'><div class='heart'></div><a href='javascript:;' class='download'></a><a href='javascript:;' class='word'></a></div>";
        $(".im-icon-file").hide(), $(".im-icon-pic").hide(), $("body").append(html);
        var docHeight = $(document).height(),
            scrollHeight = document.body.scrollHeight;
        $(".dd_tip_mask_v3").height(docHeight > scrollHeight ? docHeight : scrollHeight)
    }

    function buildDownloadNewV4() {
        var html = "<div class='dd_tip_mask_v4'></div><div class='dd_tip_main_v4'><a href='javascript:;' class='download_v4'></a><div class='download_tip_v4'></div><div class='continue_consult_div'><a href='javascript:;' class='word_v4' style='font-family: 微软雅黑;'>京东咚咚网页版>></a></div></div>";
        $(".im-icon-file").hide(), $(".im-icon-pic").hide(), $("body").append(html)
    }

    function bindDownloadEventV4(url) {
        function closeMask() {
            $(".im-icon-file").show(), $(".im-icon-pic").show(), $(".dd_tip_mask_v4").remove(), $(".dd_tip_main_v4").remove()
        }

        $(".download_v4").unbind("mouseover").bind("mouseover", function () {
            return $(".download_tip_v4").fadeIn(1e3), !1
        }), $(".download_v4").unbind("mouseout").bind("mouseout", function () {
            return $(".download_tip_v4").fadeOut(1e3), !1
        }), $(".dd_tip_main_v4 .download_v4").unbind("click").bind("click", function () {
            return closeMask(), setTimeout(closeWebPage, 2e3), window.open(url, "_blank"), $.ajax({
                url: "/dd/recordClick.action?t=" + (new Date).getTime() + "&from=normal_download",
                type: "GET",
                dataType: "html",
                success: function () {
                }
            }), !1
        }), $(".dd_tip_main_v4 .word_v4").unbind("click").bind("click", function () {
            closeMask(), $.ajax({
                url: "/dd/recordClick.action?t=" + (new Date).getTime() + "&from=normal_continue_consult",
                type: "GET",
                dataType: "html",
                success: function () {
                    canChat = !0
                }
            })
        })
    }

    function bindDownloadEvent(url) {
        function closeMask() {
            $(".im-icon-file").show(), $(".im-icon-pic").show(), $(".dd_tip_mask_v3").remove(), $(".dd_tip_main_v3").remove(), $(".girl").remove(), heartTimer && clearInterval(heartTimer)
        }

        var timer, smokeTimer, heartTimer;
        heartTimer = startMaskAnimate(), $(".dd_tip_main_v3 .download").unbind("click").bind("click", function () {
            closeMask();
            var os = detectOS();
            return "Windows" == os ? buildPopTipLayerV3() : buildPopNotSupportTipLayer(), buildPopTipEvent(), buildPopTipEventV3(), window.open(url, "_blank"), $.ajax({
                url: "/dd/recordClick.action?t=" + (new Date).getTime() + "&from=normal_download",
                type: "GET",
                dataType: "html",
                success: function () {
                }
            }), !1
        }), $(".dd_tip_main_v3 .word").unbind("click").bind("click", function () {
            closeMask(), $.ajax({
                url: "/dd/recordClick.action?t=" + (new Date).getTime() + "&from=normal_continue_consult",
                type: "GET",
                dataType: "html",
                success: function () {
                    canChat = !0
                }
            })
        }), $(".dd_tip_main_v2 .close").unbind("click").bind("click", function () {
            return $(".im-icon-file").show(), $(".im-icon-pic").show(), $(".dd_tip_mask_v3").remove(), $(".dd_tip_main_v3").remove(), timer && clearInterval(timer), smokeTimer && clearInterval(smokeTimer), !1
        })
    }

    function showMsg(common, settings) {
        $.ajax({
            url: "/dd/getDownloadURL.action?t=" + (new Date).getTime(),
            type: "GET",
            async: !1,
            dataType: "json",
            success: function (json) {
                if (json && json.url) if (settings.downloadURL = json.url, dd_download_url = json.url, json.show) buildDownloadNewV4(), bindDownloadEventV4(json.url);
                else {
                    canChat = !0;
                    var msg = '还在为咨询客服不能截图，不能看聊天记录而风中凌乱吗？<img src="http://static.360buyimg.com/im/img/s73.gif" style="vertical-align:middle"><br><a href="http://dongdong.jd.com/?hmsr=chat&hmmd=&hmpl=&hmkw=&hmci=" target="_blank">京东咚咚客户端</a>已经全新上线啦，新功能新体验，保障交易安全，<a  target="_blank" href="' + json.url + '">猛戳下载</a>';
                    common.systemInfo(msg, "im-msg-notice")
                } else canChat = !0
            },
            error: function () {
                canChat = !0
            }
        })
    }

    $.extend({
        startClient: function (pin, statusManager, settings, common) {
            if (!settings.startClientCalled) {
                settings.startClientCalled = !0;
                var ua = navigator.userAgent;
                if (-1 == ua.indexOf("Windows")) return canChat = !0, void 0;
                buildLoading();
                var type, value, search = window.location.search;
                if (search.indexOf("skuId") > 0) {
                    type = 2;
                    var venderId, csid = "B35D742C-5983-40C1-A8C0-A7DBFA2EF05E",
                        plugin = getPlugin(csid);
                    if (search.indexOf("venderId") > 0 && (venderId = splitUrlParam(search, "venderId")), !plugin && plugin.CheckVal >= 1001) {
                        type = 5;
                        var price, skuId = splitUrlParam(search, "skuId");
                        search.indexOf("price") > 0 && (price = splitUrlParam(search, "price"));
                        var wname;
                        search.indexOf("wname") > 0 && (wname = splitUrlParam(search, "wname"));
                        var imgUrl;
                        search.indexOf("imgUrl") > 0 && (imgUrl = splitUrlParam(search, "imgUrl"));
                        var itemUrl;
                        search.indexOf("itemUrl") > 0 && (itemUrl = splitUrlParam(search, "itemUrl"));
                        var chatSkuContext = {};
                        chatSkuContext.skuId = skuId, chatSkuContext.venderId = venderId, chatSkuContext.price = price, chatSkuContext.wname = wname, chatSkuContext.imgUrl = imgUrl, chatSkuContext.itemUrl = itemUrl, value = JSON.stringify(chatSkuContext)
                    } else value = venderId
                } else if (search.indexOf("pid") > 0) {
                    type = 1;
                    var pid = splitUrlParam(search, "pid"),
                        csid = "B35D742C-5983-40C1-A8C0-A7DBFA2EF05E",
                        plugin = getPlugin(csid);
                    if (!plugin && plugin.CheckVal >= 1001) {
                        type = 5;
                        var chatSkuContext = {};
                        chatSkuContext.skuId = value, value = JSON.stringify(chatSkuContext)
                    } else value = pid
                } else if (search.indexOf("venderId") > 0) {
                    type = 2;
                    var venderId = search.substring(search.indexOf("venderId")),
                        index = venderId.indexOf("&");
                    index > 0 && (venderId = venderId.substring(0, index)), value = venderId.split("=")[1]
                } else if (search.indexOf("shopId") > 0) {
                    type = 3;
                    var shopId = search.substring(search.indexOf("shopId")),
                        index = shopId.indexOf("&");
                    index > 0 && (shopId = shopId.substring(0, index)), value = shopId.split("=")[1]
                }
                return "undefined" != typeof fromConvChat && fromConvChat && "undefined" != typeof consultGid && (type = 4, value = parseInt(consultGid, 10)), isNaN(type) || !pin || "" == $.trim(pin) ? (canChat = !0, void 0) : (startDD(type, value, pin, statusManager, settings, common), void 0)
            }
        },
        showDownloadMsg: function () {
            var ua = navigator.userAgent;
            if (-1 != ua.indexOf("Windows")) {
                var csid = "B35D742C-5983-40C1-A8C0-A7DBFA2EF05E";
                checkInstall(csid, "CheckVal") || $.ajax({
                    url: "/dd/getDownloadURL.action?t=" + (new Date).getTime(),
                    type: "GET",
                    dataType: "html",
                    success: function (html) {
                        html && "" != html && (buildDownload(), bindEvent(html))
                    }
                })
            }
        }
    });
    var isIe = !!window.ActiveXObject || "ActiveXObject" in window
}(jQuery), ChatBllOrder.prototype.getOrder = function (target) {
    var _this = this;
    if (!_this.debug) {
        var loading = null;
        if (loading = $(".im-loading", $(".orders").parent()).length > 0 ? $(".im-loading", $(".orders").parent()) : $('<div class="im-loading"><img src="/img/loading.gif" alt="正在加载中"></div>').appendTo($(".orders").parent()), _this.orderList && _this.orderList.length > 0) return $(".no-order").hide(), $(".orders").show(), _this.showOrderInfo(0), loading.remove(), void 0;
        if (null == _this.orderList) return $(".no-order").show(), $(".orders").hide(), loading.remove(), void 0;
        $.ajax({
            url: "/order/getOneMonthOrder.action",
            type: "get",
            success: function (xml) {
                try {
                    _this.orderList = eval("(" + xml + ")"), $(target).attr("active", !0), _this.orderList || ($(".no-order").show(), $(".orders").hide()), _this.orderList.length > 0 ? ($(".no-order").hide(), $(".orders").show(), _this.showOrderInfo(0)) : ($(".no-order").show(), $(".orders").hide())
                } catch (e) {
                    _this.orderList = null, $(".no-order").show(), $(".orders").hide(), loading.remove()
                }
                loading.remove()
            },
            error: function () {
                loading.remove()
            }
        })
    }
}, ChatBllOrder.prototype.showOrderInfo = function (index) {
    var _this = this,
        _orderList = _this.orderList;
    if (_orderList) {
        var totalNum = _orderList && _orderList.length > 99 ? "N+" : _orderList.length;
        _this.buildOrderInfoView(_orderList[index]), $("#j_orderTotalNum").html(totalNum).parent().show(), $("#j_prevBtn").unbind("click").bind("click", function () {
            _this.prevOrder()
        }), $("#j_nextBtn").unbind("click").bind("click", function () {
            _this.nextOrder()
        })
    }
}, ChatBllOrder.prototype.getOrderStatus = function () {
}, ChatBllOrder.prototype.buildOrderInfoView = function (data) {
    function renderOrderStatus(json) {
        var $detail = $(".im-order-detail:eq(1)").empty().hide();
        if (json.length > 0) {
            $detail.show(), $(".im-tab-contents").css("overflow", "auto");
            for (var table = "", i = json.length - 1; i >= 0; i--) table += '<div class="im-item-group">', table += '<div class="im-label-content" style="margin-left:0">' + json[i].opTime + "<br />" + json[i].content + "</div></div>";
            $detail.html(table)
        }
    }

    var _this = this;
    $("#j_orderNum").html(_this.orderIndex + 1 + "/" + _this.orderList.length), void 0 == data.status ? $("#j_orderState").parent().hide() : $("#j_orderState").parent().show().end().html(data.status), $("#j_orderId").html(data.orderid), $("#j_shouldPay").html(data.orderprice), $("#j_paymentTypeName").html(data.paytype), $("#j_customerName").html(data.customer), $("#j_dateSubmit").html($.jdtoolbox.curTime(data.time));
    var xdsp = "";
    try {
        for (var imgArr = data.products, i = 0; imgArr.length > i; i++) xdsp += '<li class="im-item"><a target="_blank" href="' + imgArr[i].url + '"><img src="' + imgArr[i].imgurl + '" title="' + imgArr[i].name + '" /></a></li>'
    } catch (e) {
    }
    $("#j_wareImgUrl").html(xdsp), renderOrderStatus(data.orderTrack)
}, ChatBllOrder.prototype.collectA = function (val) {
    var rrr = /(?:<a[^>]*>)(.*?)(?:<\/a[^>]*>)/gi,
        rra = val.match(rrr);
    return rra
}, ChatBllOrder.prototype.nextOrder = function () {
    var _this = this;
    1 != _this.orderList.length && (_this.orderIndex += 1, _this.orderIndex == _this.orderList.length && (_this.orderIndex = 0), _this.buildOrderInfoView(_this.orderList[_this.orderIndex]))
}, ChatBllOrder.prototype.prevOrder = function () {
    var _this = this;
    1 != _this.orderList.length && (_this.orderIndex -= 1, 0 > _this.orderIndex && (_this.orderIndex = _this.orderList.length - 1), _this.buildOrderInfoView(_this.orderList[_this.orderIndex]))
}, ChatBllOrder.prototype.getRepairs = function (target) {
    var _this = this,
        loading = null;
    loading = $(".im-loading", $(".reqairs").parent()).length > 0 ? $(".im-loading", $(".reqairs").parent()) : $('<div class="im-loading"><img src="/img/loading.gif" alt="正在加载中"></div>').appendTo($(".reqairs").parent()), $.ajax({
        url: "/httpApi/repairs.action",
        success: function (msg) {
            try {
                _this.repairsList = eval("(" + msg + ")"), $(target).attr("active", !0), _this.repairsList.length > 0 ? ($(".no-reqairs").hide(), $(".reqairs").show(), _this.showRepairsInfo(0)) : ($(".no-reqairs").show(), $(".reqairs").hide())
            } catch (e) {
                $(".no-reqairs").show(), $(".reqairs").hide()
            }
            loading.remove()
        },
        error: function () {
            loading.remove()
        }
    })
}, ChatBllOrder.prototype.showRepairsInfo = function () {
    var _this = this;
    _this.repairsList && ($("#j_repairPrevBtn").unbind("click").bind("click", function () {
        return _this.reqPrevOrder(), !1
    }), $("#j_repairNextBtn").unbind("click").bind("click", function () {
        return _this.reqNextOrder(), !1
    }), _this.buildReqairs(_this.repairsList[_this.repairsIndex]))
}, ChatBllOrder.prototype.buildReqairs = function (data) {
    var _this = this;
    $("#j_repairNum").html(_this.repairsIndex + 1 + "/" + _this.repairsList.length), $("#j_repairId").html(data.repairId), $("#j_repairStatus").html(data.repairStatusStr), $("#j_repairTime").html($.jdtoolbox.curTime(data.repairTime)), $("#j_repairWareName").html(data.wareName)
}, ChatBllOrder.prototype.reqNextOrder = function () {
    var _this = this;
    1 != _this.repairsList.length && (_this.repairsIndex += 1, _this.repairsIndex == _this.repairsList.length && (_this.repairsIndex = 0), _this.buildReqairs(_this.repairsList[_this.repairsIndex]))
}, ChatBllOrder.prototype.reqPrevOrder = function () {
    var _this = this;
    1 != _this.repairsList.length && (_this.repairsIndex -= 1, 0 > _this.repairsIndex && (_this.repairsIndex = _this.repairsList.length - 1), _this.buildReqairs(_this.repairsList[_this.repairsIndex]))
}, ChatBllOrder.prototype.getConsultOrder = function (target) {
    var _this = this;
    this.showConsultOrder && $.ajax({
        url: "/order/consultOrder.action?orderId=" + oid,
        success: function (html) {
            if (_this.showConsultOrder = !1, !html) return $(target).hide().next().click(), void 0;
            var json = eval("(" + html + ")");
            json.status ? $("#c_orderState").parent().hide() : $("#c_orderState").parent().show().end().html(json.status), $("#c_orderId").html(json.orderid), $("#c_shouldPay").html(json.orderprice), $("#c_paymentTypeName").html(json.paytype), $("#c_customerName").html(json.customer), $("#c_dateSubmit").html($.jdtoolbox.curTime(json.time));
            var xdsp = "";
            try {
                for (var imgArr = json.products, i = 0; imgArr.length > i; i++) xdsp += '<li class="im-item"><a target="_blank" href="' + imgArr[i].url + '"><img src="' + imgArr[i].imgurl + '" title="' + imgArr[i].name + '" /></a></li>'
            } catch (e) {
            }
            if ($("#c_wareImgUrl").html(xdsp), $(".im-tab-content").eq(0).show().children(":eq(0)").show(), json.orderTrack && $.isArray(json.orderTrack) && json.orderTrack.length > 0) {
                $(".im-tab-contents").css("overflow", "auto");
                for (var $detail = $("#c_orderTracks"), table = "", i = json.orderTrack.length - 1; i >= 0; i--) table += '<div class="im-item-group">', table += '<div class="im-label-content" style="margin-left:0">' + json.orderTrack[i].opTime + "<br />" + json.orderTrack[i].content + "</div></div>";
                $("#c_orderTracks").html(table).show()
            }
        },
        error: function () {
            _this.showConsultOrder = !1
        }
    })
}, function ($) {
    function getData() {
        void 0 != _options && $.ajax({
            url: "/fqa/index.action",
            data: {
                orgId: _options.orgId,
                pid: _options.pid,
                venderId: _options.venderId,
                rank3: _options.classify,
                brandName: _options.brandName
            },
            success: function (xml) {
                var json = $.jdtoolbox.string2json("(" + xml + ")");
                if (null == json || 0 == json.length) return $this.append('<li><p class="im-txt-lighter im-txt-center">暂无数据</p></li>'), void 0;
                for (var arr = [], i = 0; json.length > i; i++) {
                    var obj = {};
                    obj.title = json[i].question, obj.content = json[i].answer, arr.push(obj)
                }
                defaultOptions = arr, clearTree($this), buildTreeView($this)
            },
            error: function () {
                $this.append('<li><p class="im-txt-lighter im-txt-center">暂无数据</p></li>')
            }
        })
    }

    function init() {
        getData()
    }

    function clearTree(el) {
        el.html("")
    }

    function buildTreeView(el) {
        for (var i = 0, ln = defaultOptions.length; ln > i; i++) el.append(buildItem(defaultOptions[i], i + 1))
    }

    function buildItem(data, index) {
        var item = ['<li class="im-item">', '<p class="im-question"><a href="#" class="im-txt-link j_title"></a></p>', '<div class="im-answer-box">', '<div class="im-answer-content"></div>', '<span class="im-box-arrow"></span></div></li>'],
            $item = $(item.join("")),
            title = $item.find(".j_title");
        return title.html(index + "、" + filter(data.title)), $item.find(".im-answer-content").html(filter(data.content)), title.bind("click", function () {
            $($(this).parent().parent()).toggleClass("unfold"), $(this).find(".im-answer-content").toggle()
        }), $item
    }

    function filter(content) {
        var $dom = $("<span></span>").html(content);
        return $dom.find("a").each(function () {
            $(this).attr("target", "_blank")
        }), $dom.html()
    }

    var defaultOptions = [{
            title: "下单后多久能送到？",
            content: "在商品有现货的情况下，北京、上海、广州客户，下单后一半24小时内可收到货（郊县配送时间可能会更长一些）；其他地区用户，将根据您的收货地址及所选的配送方式而不同，一半到货时间在1-7天。"
        }, {
            title: "下单后多久能送到？",
            content: "在商品有现货的情况下，北京、上海、广州客户，下单后一半24小时内可收到货（郊县配送时间可能会更长一些）；其他地区用户，将根据您的收货地址及所选的配送方式而不同，一半到货时间在1-7天。"
        }, {
            title: "下单后多久能送到？",
            content: "在商品有现货的情况下，北京、上海、广州客户，下单后一半24小时内可收到货（郊县配送时间可能会更长一些）；其他地区用户，将根据您的收货地址及所选的配送方式而不同，一半到货时间在1-7天。"
        }],
        _options = null,
        $this = null;
    $.fn.tree = function (options) {
        $this = $(this), _options = options, init($(this))
    }
}(jQuery), function ($) {
    function _init(target) {
        for (var $this = target, tabs = $this.find("li"), i = 0, ln = tabs.size(); ln > i; i++) {
            var _li = $(tabs[i]);
            _li.bind("click", function () {
                var target = $(this);
                target.hasClass("current") || "true" === target.attr("active") || target.trigger("tabOnClickHandle", target), winSwitching(target)
            })
        }
    }

    function winSwitching(el) {
        var _index = el.index();
        $(el).addClass("current"), $(el).siblings().removeClass("current"), $(".im-tab-contents").children().each(function (index, el) {
            index == _index ? $(el).show() : $(el).hide()
        })
    }

    $.fn.Tab = function () {
        _init($(this))
    }
}(jQuery), function ($) {
    function css(el, prop) {
        return parseInt($.css(el[0], prop)) || 0
    }

    function width(el) {
        return el[0].offsetWidth + css(el, "marginLeft") + css(el, "marginRight")
    }

    function height(el) {
        return el[0].offsetHeight + css(el, "marginTop") + css(el, "marginBottom")
    }

    $.fn.jCarouselLite = function (o) {
        return o = $.extend({
            btnPrev: null,
            btnNext: null,
            btnGo: null,
            mouseWheel: !1,
            auto: null,
            speed: 200,
            easing: null,
            vertical: !1,
            circular: !1,
            visible: 2,
            start: 0,
            scroll: 1,
            beforeStart: null,
            afterEnd: null
        }, o || {}), this.each(function () {
            function vis() {
                return li.slice(curr).slice(0, v)
            }

            function go(to) {
                if (!running) {
                    if (o.beforeStart && o.beforeStart.call(this, vis()), o.circular) o.start - v - 1 >= to ? (ul.css(animCss, -((itemLength - 2 * v) * liSize) + "px"), curr = to == o.start - v - 1 ? itemLength - 2 * v - 1 : itemLength - 2 * v - o.scroll) : to >= itemLength - v + 1 ? (ul.css(animCss, -(v * liSize) + "px"), curr = to == itemLength - v + 1 ? v + 1 : v + o.scroll) : curr = to;
                    else {
                        if (0 > to || to > itemLength - v) return;
                        curr = to
                    }
                    running = !0, ul.animate("left" == animCss ? {
                        left: -(curr * liSize)
                    } : {
                        top: -(curr * liSize)
                    }, o.speed, o.easing, function () {
                        o.afterEnd && o.afterEnd.call(this, vis()), running = !1
                    }), o.circular || ($(o.btnPrev + "," + o.btnNext).removeClass("disabled"), $(0 > curr - o.scroll && o.btnPrev || curr + o.scroll > itemLength - v && o.btnNext || []).addClass("disabled"))
                }
                return !1
            }

            var running = !1,
                animCss = o.vertical ? "top" : "left",
                sizeCss = o.vertical ? "height" : "width",
                div = $(this),
                ul = $("ul", div),
                tLi = $("li", ul),
                tl = tLi.size(),
                v = o.visible;
            o.circular && (ul.prepend(tLi.slice(tl - v - 1 + 1).clone(!0)).append(tLi.slice(0, v).clone(!0)), o.start += v);
            var li = $("li", ul),
                itemLength = li.size(),
                curr = o.start;
            div.css("visibility", "visible"), li.css({
                overflow: "hidden",
                "float": o.vertical ? "none" : "left"
            }), ul.css({
                margin: "0",
                padding: "0",
                position: "relative",
                "list-style-type": "none",
                "z-index": "1"
            }), div.css({
                overflow: "hidden",
                position: "relative",
                "z-index": "2",
                left: "0px"
            });
            var liSize = o.vertical ? height(li) : width(li),
                ulSize = liSize * itemLength;
            li.css({
                width: li.width(),
                height: li.height()
            }), ul.css(sizeCss, ulSize + "px").css(animCss, -(curr * liSize)), div.css(sizeCss, "288px"), o.btnPrev && $(o.btnPrev).click(function () {
                return go(curr - o.scroll)
            }), o.btnNext && $(o.btnNext).click(function () {
                return go(curr + o.scroll)
            }), o.btnGo && $.each(o.btnGo, function (i, val) {
                $(val).click(function () {
                    return go(o.circular ? o.visible + i : i)
                })
            }), o.mouseWheel && div.mousewheel && div.mousewheel(function (e, d) {
                return d > 0 ? go(curr - o.scroll) : go(curr + o.scroll)
            }), o.auto && setInterval(function () {
                go(curr + o.scroll)
            }, o.auto + o.speed)
        })
    }
}(jQuery), function ($) {
    function hexToR(h) {
        return parseInt(cutHex(h).substring(0, 2), 16)
    }

    function hexToG(h) {
        return parseInt(cutHex(h).substring(2, 4), 16)
    }

    function hexToB(h) {
        return parseInt(cutHex(h).substring(4, 6), 16)
    }

    function cutHex(h) {
        return "#" == h.charAt(0) ? h.substring(1, 7) : h
    }

    function init(_this, options) {
        var ColorHex = ["00", "33", "66", "99", "CC", "FF"],
            SpColorHex = ["FF0000", "00FF00", "0000FF", "FFFF00", "00FFFF", "FF00FF"],
            colorTable = "";
        for (i = 0; 2 > i; i++) for (j = 0; 6 > j; j++) for (colorTable += "<tr height=12>", colorTable += '<td width=11 style="background-color:#000000">', colorTable = 0 == i ? colorTable + '<td width=11 style="background-color:#' + ColorHex[j] + ColorHex[j] + ColorHex[j] + '">' : colorTable + '<td width=11 style="background-color:#' + SpColorHex[j] + '">', colorTable += '<td width=11 style="background-color:#000000">', k = 0; 3 > k; k++) for (l = 0; 6 > l; l++) colorTable = colorTable + '<td width=11 style="background-color:#' + ColorHex[k + 3 * i] + ColorHex[l] + ColorHex[j] + '">';
        colorTable = '<a href="#" class="im-pop-close im-icon-close " id="j_colorClose"></a><table width=253 border="0" cellspacing="0" cellpadding="0" NaN<tr height=30><td colspan=21><table cellpadding="0" cellspacing="1" border="0" style="border-collapse: collapse"><tr><td width="3"><td><input type="text" id="DisColor" name="DisColor" size="6" disabled style="border:solid 1px #000000;background-color:#000000"></td><td width="3"><td><input type="text" name="HexColor" size="7" id="HexColor"style="border:inset 1px;font-family:Arial;" value="#000000"></td></tr></table></td></table><table border="1" cellspacing="0" cellpadding="0" style="border-collapse: collapse" bordercolor="000000" id="jdcolor_body_table" style="cursor:hand;">' + colorTable + "</table>", _this.html(colorTable), $("#jdcolor_body_table").click(function (event) {
            var formatColor = doclick(event, options);
            options.click(formatColor)
        }), $("#jdcolor_body_table").mouseover(function (event) {
            doOver(event, options), options.over()
        }), $("#jdcolor_body_table").mouseout(function (event) {
            doOut(event, options), options.out()
        }), $("#j_colorClose").click(function () {
            $("#colorButton").removeClass("im-icon-color-hover"), $(this).parent().toggle()
        })
    }

    function doclick(event, options) {
        var $target = $(event.target),
            backgroundColor = "";
        $target.is("td") && (backgroundColor = $target.css("background-color"));
        var formatColor = format(backgroundColor, options);
        return formatColor
    }

    function doOver(event, options) {
        var $target = $(event.target);
        $target.is("td") && current != $target && (current = $target, backgroundColor = $target.css("background-color"), $("#DisColor").css("background-color", backgroundColor), $("#HexColor").val(format(backgroundColor, options)))
    }

    function doOut(event, options) {
        null != current && (current = null, $("#DisColor").css("background-color", "#000000"), $("#HexColor").val(format("#000000", options)))
    }

    function format(backgroundColor, options) {
        var os = getOs(),
            formatColor = backgroundColor;
        return $.browser.msie && $.browser.version >= 9 && (options.colorFormat = "HSB"), "MSIE" == os && "RGB" == options.colorFormat ? (formatColor = "rgb(", formatColor += hexToR(backgroundColor) + ",", formatColor += hexToG(backgroundColor) + ",", formatColor += hexToB(backgroundColor), formatColor += ")") : "Firefox" == os && "HSB" == options.colorFormat && (formatColor = backgroundColor.colorHex()), formatColor
    }

    function getOs() {
        return navigator.userAgent.indexOf("MSIE") > 0 ? "MSIE" : (isFirefox = navigator.userAgent.indexOf("Firefox") > 0) ? "Firefox" : (isSafari = navigator.userAgent.indexOf("Safari") > 0) ? "Safari" : (isCamino = navigator.userAgent.indexOf("Camino") > 0) ? "Camino" : (isMozilla = navigator.userAgent.indexOf("Gecko/") > 0) ? "Gecko" : void 0
    }

    var current = null;
    $.fn.jdColor = function (options) {
        var settings = $.extend({}, $.fn.jdColor.defaults, options);
        return init(this, settings), this
    }, $.fn.jdColor.defaults = {
        colorTrClass: "colorTr",
        over: function () {
        },
        out: function () {
        },
        click: function () {
        },
        colorFormat: "RGB"
    }
}(jQuery), jQuery.jdtoolbox = {
    cookie: function (key, value, options) {
        if (arguments.length > 1 && "[object Object]" != value + "") {
            if (options = jQuery.extend({}, options), (null === value || void 0 === value) && (options.expires = -1), "number" == typeof options.expires) {
                var days = options.expires,
                    t = options.expires = new Date;
                t.setDate(t.getDate() + days)
            }
            return value += "", document.cookie = [encodeURIComponent(key), "=", options.raw ? value : encodeURIComponent(value), options.expires ? "; expires=" + options.expires.toUTCString() : "", options.path ? "; path=" + options.path : "", options.domain ? "; domain=" + options.domain : "", options.secure ? "; secure" : ""].join("")
        }
        options = value || {};
        var result, decode = options.raw ?
            function (s) {
                return s
            } : decodeURIComponent;
        return (result = RegExp("(?:^|; )" + encodeURIComponent(key) + "=([^;]*)").exec(document.cookie)) ? decode(result[1]) : null
    },
    toHexString: function (args) {
        if (null == args) return null;
        if ("" == args) return "";
        for (var len = args.length, argsHex = "", i = 0; len > i; i++) {
            var code = args.charCodeAt(i),
                hexCode = code.toString(16);
            argsHex += hexCode
        }
        return argsHex
    },
    browerType: function () {
        return navigator.userAgent.indexOf("MSIE") > 0 ? "MSIE" : navigator.userAgent.indexOf("Firefox") > 0 ? "Firefox" : navigator.userAgent.indexOf("Safari") > 0 ? "Safari" : navigator.userAgent.indexOf("Camino") > 0 ? "Camino" : navigator.userAgent.indexOf("Gecko/") > 0 ? "Gecko" : null
    },
    curTime: function (time) {
        var t = new Date;
        time && (t = new Date(time));
        var s = "";
        s += t.getFullYear() + "-";
        var m = t.getMonth() + 1;
        10 > m && (m = "0" + m), s += m + "-";
        var d = t.getDate();
        10 > d && (d = "0" + d), s += d + " ";
        var h = t.getHours();
        10 > h && (h = "0" + h), s += h + ":";
        var m = t.getMinutes();
        10 > m && (m = "0" + m), s += m + ":";
        var sec = t.getSeconds();
        return 10 > sec && (sec = "0" + sec), s += sec
    },
    sysTime: function (time) {
        var t = new Date;
        time && (t = new Date(time));
        var s = "",
            h = t.getHours();
        10 > h && (h = "0" + h), s += h + ":";
        var m = t.getMinutes();
        10 > m && (m = "0" + m), s += m + ":";
        var sec = t.getSeconds();
        return 10 > sec && (sec = "0" + sec), s += sec
    },
    removeCssAttr: function (tagId, attrKey) {
        var styleValue = $("#" + tagId).attr("style");
        if (styleValue.indexOf(attrKey) >= 0) {
            var f = styleValue.substring(0, styleValue.indexOf(attrKey)),
                b = styleValue.substring(styleValue.indexOf(attrKey), styleValue.length);
            b = b.indexOf(";") >= 0 ? b.substring(b.indexOf(";") + 1, b.length) : "", styleValue = f + b
        }
        "" == styleValue ? $("#" + tagId).removeAttr("style") : $("#" + tagId).attr("style", styleValue)
    },
    json2string: function (obj) {
        switch (typeof obj) {
            case "string":
                return '"' + obj.replace(/(["\\])/g, "\\$1") + '"';
            case "object":
                if (obj instanceof Array) {
                    for (var strArr = [], len = obj.length, i = 0; len > i; i++) strArr.push($.jdtoolbox.json2string(obj[i]));
                    return "[" + strArr.join(",") + "]"
                }
                if (null == obj || void 0 == obj) return "null";
                var string = [];
                for (var property in obj) string.push($.jdtoolbox.json2string(property) + ":" + $.jdtoolbox.json2string(obj[property]));
                return "{" + string.join(",") + "}";
            case "number":
                return obj;
            case !1:
                return obj
        }
    },
    replaceSpecialChar: function (s) {
        for (var result = "", i = 0; s.length > i; i++) {
            var c = s.charAt(i);
            switch (c) {
                case '"':
                    c = '\\"';
                    break;
                case "\\":
                    c = "\\\\";
                    break;
                case "/":
                    c = "\\/";
                    break;
                case "\b":
                    c = "\\b";
                    break;
                case "\f":
                    c = "\\f";
                    break;
                case "\n":
                    c = "";
                    break;
                case "\r":
                    c = "";
                    break;
                case "	":
                    c = "\\t";
                case "'":
                    c = "\\'"
            }
            result += c
        }
        return result
    },
    string2json: function (string) {
        try {
            return eval("(" + string + ")")
        } catch (e) {
            return null
        }
    },
    rgb: function (color) {
        var rgbArray = Array(3);
        try {
            6 > color.length && (color = "#000000")
        } catch (e) {
        }
        return 0 == color.indexOf("#") ? (rh = color.substring(1, 2), gh = color.substring(3, 4), bh = color.substring(5, 6), ri = parseInt(rh, 16), r = ri.toString(10), rgbArray[0] = r, gi = parseInt(gh, 16), g = gi.toString(10), rgbArray[1] = g, bi = parseInt(bh, 16), b = bi.toString(10), rgbArray[2] = b) : (rgb = color.substring(color.indexOf("(") + 1, color.indexOf(")")), rgbArray = rgb.split(",")), rgbArray
    },
    rgb2color: function (rgbArray) {
        var color = "rgb(";
        return color += rgbArray[0] + ",", color += rgbArray[1] + ",", color += rgbArray[2], color += ")"
    },
    isNull: function (obj) {
        return void 0 != obj && null != obj ? !1 : !0
    },
    isBlank: function (obj) {
        return void 0 != obj && null != obj && "" != obj ? !1 : !0
    },
    replaceHtmlTag: function (srcString) {
        return srcString && (srcString = srcString.replace(/<.*?>/gi, " ")), srcString
    }
};
var jdchat = {
        versionType: "normal",
        basePath: "",
        serverType: "",
        chatStatus: !1,
        status: 0,
        winStatus: !1,
        jdCommon: "",
        questionNum: 6
    },
    canChat = !1,
    riskCheckFlag = !1,
    riskCheckInvoked = !1,
    initTimer = null;
jdchat.statusManager = new function () {
    var systemStatus = {
            init: "正在初始化",
            queue: "排队中",
            nobody: "没有客服在线",
            message: "留言中",
            concat: "接入中",
            chating: "正在聊天中",
            leave: "离开",
            refresh: "用户刷新或关闭页面"
        },
        currentStatus = systemStatus.leave,
        statusDataMap = {};
    return {
        systemStatus: systemStatus,
        currentStatus: currentStatus,
        statusDataMap: statusDataMap
    }
};
var BrowserDetect = {
    init: function () {
        this.browser = this.searchString(this.dataBrowser) || "An unknown browser", this.version = this.searchVersion(navigator.userAgent) || this.searchVersion(navigator.appVersion) || "an unknown version"
    },
    searchString: function (data) {
        for (var i = 0; data.length > i; i++) {
            var dataString = data[i].string,
                dataProp = data[i].prop;
            if (this.versionSearchString = data[i].versionSearch || data[i].identity, dataString) {
                if (-1 != dataString.indexOf(data[i].subString)) return data[i].identity
            } else if (dataProp) return data[i].identity
        }
    },
    searchVersion: function (dataString) {
        var index = dataString.indexOf(this.versionSearchString);
        if (-1 != index) return parseFloat(dataString.substring(index + this.versionSearchString.length + 1))
    },
    dataBrowser: [{
        string: navigator.userAgent,
        subString: "Chrome",
        identity: "Chrome"
    }, {
        string: navigator.userAgent,
        subString: "OmniWeb",
        versionSearch: "OmniWeb/",
        identity: "OmniWeb"
    }, {
        string: navigator.vendor,
        subString: "Apple",
        identity: "Safari",
        versionSearch: "Version"
    }, {
        prop: window.opera,
        identity: "Opera",
        versionSearch: "Version"
    }, {
        string: navigator.vendor,
        subString: "iCab",
        identity: "iCab"
    }, {
        string: navigator.vendor,
        subString: "KDE",
        identity: "Konqueror"
    }, {
        string: navigator.userAgent,
        subString: "Firefox",
        identity: "Firefox"
    }, {
        string: navigator.vendor,
        subString: "Camino",
        identity: "Camino"
    }, {
        string: navigator.userAgent,
        subString: "Netscape",
        identity: "Netscape"
    }, {
        string: navigator.userAgent,
        subString: "MSIE",
        identity: "Explorer",
        versionSearch: "MSIE"
    }, {
        string: navigator.userAgent,
        subString: "Gecko",
        identity: "Mozilla",
        versionSearch: "rv"
    }, {
        string: navigator.userAgent,
        subString: "Mozilla",
        identity: "Netscape",
        versionSearch: "Mozilla"
    }]
};
BrowserDetect.init(), jdchat.resourcesPath = new function () {
    function getSkinResource(imgName, ap) {
        var _basePath = 0 == ap ? "/img/" : jdchat.basePath + "/img/";
        return imgName && (_basePath += imgName), _basePath
    }

    function getJSResource(JSResource) {
        var _basePath = jdchat.basePath + "/js/";
        return JSResource && (_basePath += JSResource), _basePath
    }

    return {
        getSkinResource: getSkinResource,
        getJSResource: getJSResource
    }
}, jdchat.switchMessage = new function () {
    function showSysInfo(code, extendMsg) {
        jdchat.common().systemInfo(switchByCode(code, extendMsg), "text_1_s")
    }

    function switchByCode(code, extendMsg) {
        var returnInfo = "";
        switch (code) {
            case 2:
                returnInfo = "运气稍差，被别人抢先了0.01秒，请您继续等待！";
                break;
            case 3:
                returnInfo = "您与当前客服的连接已中断,如果您需要继续咨询，<a href='#' onclick='$(this).trigger(\"im-reload\")'> 请点击此处</a>";
                break;
            case 4:
                returnInfo = "您已与此商家建立了对话，请去原窗口继续咨询；如果原窗口刚刚被您关闭，请十分钟后再试！";
                break;
            case 0:
            case 10:
                returnInfo = "您与当前客服的连接已中断,如果您需要继续咨询，<a href='#' onclick='$(this).trigger(\"im-reload\")'> 请点击此处</a>";
                break;
            case 11:
                returnInfo = "对不起，当前客服繁忙，请您稍等！";
                break;
            case 15:
                returnInfo = '由于您的发送速度太快<br />"' + extendMsg + '" 消息发送失败，请您稍后再试';
                break;
            case 20:
                returnInfo = "由于您更换了登录账号，与当前客服的连接已中断,如果您需要继续咨询，<a href='#' onclick='$(this).trigger(\"im-reload\")'> 请点击此处</a>";
                break;
            case 21:
            case 22:
                returnInfo = "建立链接错误，与当前客服的连接已中断,如果您需要继续咨询，<a href='#' onclick='$(this).trigger(\"im-reload\")'> 请点击此处</a>";
                break;
            case 100:
                returnInfo = "您与当前客服的连接已中断,如果您需要继续咨询，<a href='#' onclick='$(this).trigger(\"im-reload\")'> 请点击此处</a>";
                break;
            case 101:
                returnInfo = "消息发送失败,接收者不能是自己！";
                break;
            case 360:
                returnInfo = "消息发送失败,您发送的内容太长了！";
                break;
            default:
        }
        return returnInfo
    }

    return {
        showSysInfo: showSysInfo
    }
}, jdchat.dialogBox = new function () {
    function winAlert(msg) {
        if ($(".thickArea").is(":visible")) {
            if (beforeMsg == msg) return closeDailog(), void 0;
            tmID && ($(".thickcon #msg").text(msg), setPosition($(".thickbox1")), clearInterval(tmID), index = 5, $(".thickcountdown #jd-countdown").text(index), tmID = setInterval(show, 1e3))
        } else 0 == $(".thickArea").children().size() && $(".thickArea").append(box), $(".thickcon #msg").text(msg), setPosition($(".thickbox1")), $(".thickArea").show(), tmID = setInterval(show, 1e3);
        beforeMsg = msg
    }

    function setPosition(o) {
        var itop = (document.documentElement.clientHeight - o.height()) / 2 + document.documentElement.scrollTop + document.body.scrollTop,
            ileft = (document.documentElement.clientWidth - o.width()) / 2 + document.documentElement.scrollLeft + document.body.scrollLeft;
        o.css({
            position: "absolute",
            top: itop + "px",
            left: ileft + "px"
        })
    }

    function show() {
        index--, waitingTime > index ? closeDailog() : $(".thickcountdown #jd-countdown").text(index)
    }

    function closeDailog() {
        clearInterval(tmID), index = 5, $(".thickArea").hide(), $(".thickcountdown #jd-countdown").text(index)
    }

    var box = $('<div class="thickbox1" style="height:150px; width:410px;"></div>');
    box.append($('<div class="thicktitle"><span>提示</span></div>')), box.append($('<div class="thickcon"><span id="msg" style="font-weight:bolder; color:red"></span></div>')), box.append($('<div class="thickcountdown"><span id="jd-countdown">5</span>自动关闭</div>'));
    var clsBtn = $('<a href="#" class="thickclose" value="关闭" >关闭</a>');
    clsBtn.bind("click", closeDailog), box.append(clsBtn);
    var beforeMsg = "",
        tmID = null,
        index = 5,
        waitingTime = 1;
    return {
        winAlert: winAlert
    }
}, jdchat.waitMessage = new function () {
    function getWaitTotalTime(index) {
        for (var total = 0, i = 0; index > i; i++) total += waitingTimeQueue[i];
        return total
    }

    function setContentFunc(func) {
        _contentFunc = func
    }

    function getTime(count) {
        return waitingStep * waitingTimeQueue[count]
    }

    function create() {
        if (waitingTimerCount > waitingTimeQueue.length - 1 && waitingTimeQueue.push(waitingTimeQueue[waitingTimeQueue.length - 1]), waitingTimerCount > 0) {
            var waitedTime = getWaitTotalTime(waitingTimerCount);
            _contentFunc(waitedTime)
        }
        var later = getTime(waitingTimerCount);
        timer = setTimeout(create, later), waitingTimerCount++
    }

    function stop() {
        timer && clearTimeout(timer)
    }

    var _contentFunc = null,
        timer = null,
        waitingStep = 6e4,
        waitingTimeQueue = [2, 3, 5],
        waitingTimerCount = 0;
    return {
        create: create,
        stop: stop,
        setContentFunc: setContentFunc
    }
}, jdchat.common = function (settings, createLinkFun, autoAllotFun, intoChatFun) {
    function new_ms() {
        if (settings.newMsg) {
            var title = document.title;
            document.title = "京东在线客服" == title || "商家在线客服" == title ? "【收到新未读消息】_在线客服_京东商城" : "normal" == jdchat.versionType ? "商家在线客服" : "京东在线客服"
        } else document.title = "normal" == jdchat.versionType ? "商家在线客服" : "京东在线客服";
        window.setTimeout(new_ms, 500)
    }

    function setExpression() {
        "none" == $("#j_popFace").css("display") ? ($("#expressionButton").addClass("im-icon-face-current"), $("#j_popFace").show()) : ($("#expressionButton").removeClass("im-icon-face-current"), $("#j_popFace").hide()), "none" != $(".j_popWord").css("display") && ($("#j_font").removeClass("im-icon-word-current"), $(".j_popWord").hide()), "none" != $(".j_recommend").css("display") && ($("#j_font").addClass("im-icon-word-current"), $(".j_recommend").hide())
    }

    function insertImg(str) {
        $("#text_in").blur(), document.getElementById("text_in").focus();
        var selection = document.selection ? document.selection : window.getSelection(),
            range = selection.createRange ? selection.createRange() : selection.getRangeAt(0);
        if ("MSIE" == $.jdtoolbox.browerType()) str = $("#text_in").html() + str, $("#text_in").html(""), range.pasteHTML(str), range.collapse(!1), range.select();
        else {
            range.collapse(!1);
            var content = $("#text_in").html();
            str = content + str;
            for (var hasR = range.createContextualFragment(str), hasR_lastChild = hasR.lastChild; hasR_lastChild && "br" == hasR_lastChild.nodeName.toLowerCase() && hasR_lastChild.previousSibling && "br" == hasR_lastChild.previousSibling.nodeName.toLowerCase();) {
                var e = hasR_lastChild;
                hasR_lastChild = hasR_lastChild.previousSibling, hasR.removeChild(e)
            }
            if ($("#text_in").html(""), range.insertNode(hasR), hasR_lastChild) {
                var last = document.getElementById("text_in").lastChild;
                range.setEndAfter(last), range.setStartAfter(last)
            }
            selection.removeAllRanges(), selection.addRange(range)
        }
    }

    function degree() {
        return settings.curKf ? 0 == settings.count ? (systemInfo("对不起，未与客服进行在线对话不能评价！", "im-msg-notice"), void 0) : settings.isDegree ? (systemInfo("对不起，您已对本次服务做出过评价了！", "im-msg-notice"), void 0) : ("none" == $(".j_recommend").css("display") ? ($(".j_recommend").show(), $(".add").show(), $(".add").html(" ")) : $(".j_recommend").hide(), "none" != $(".j_popWord").css("display") && ($("#j_font").removeClass("im-icon-word-current"), $(".j_popWord").hide()), "none" != $("#j_popFace").css("display") && ($("#expressionButton").removeClass("im-icon-face-current"), $("#j_popFace").hide()), void 0) : (systemInfo("非常抱歉，没有客服加入对话，无法进行满意度评价！", "im-msg-notice"), void 0)
    }

    function initFontType() {
        var fontType = jQuery.jdtoolbox.cookie("fontType");
        return $.each(settings.fontType, function (index, value) {
            var o = '<option value="' + value.value + '">' + value.name + "</option>";
            $(o).appendTo("#text_in_font_type")
        }), null != fontType && "" != fontType && ($("#text_in_font_type").children("option").each(function () {
            $(this).attr("value") == fontType && $(this).attr("selected", "selected")
        }), setFontType(fontType)), fontType
    }

    function initFontSize() {
        var fontSize = jQuery.jdtoolbox.cookie("fontSize");
        return $.each(settings.fontSize, function (index, value) {
            var o = '<option value="' + value.value + '">' + value.name + "</option>";
            $(o).appendTo("#text_in_font_size")
        }), null != fontSize && "" != fontSize && ($("#text_in_font_size").children("option").each(function () {
            $(this).attr("value") == fontSize && $(this).attr("selected", "selected")
        }), setFontSize(fontSize)), fontSize
    }

    function initHotKey() {
        var hotKey = jQuery.jdtoolbox.cookie("hotKey");
        return hotKey || (hotKey = "hotkey1"), hotKey
    }

    function initColor() {
        var color = jQuery.jdtoolbox.cookie("color");
        return null != color && "" != color && ($("#colorButton").css("background-color", color), setColor(color)), color
    }

    function setFontType(fontType) {
        null == fontType || "" == fontType ? $.jdtoolbox.removeCssAttr("text_in", "font-family") : $("#text_in").css("font-family", fontType)
    }

    function setFontSize(fontSize) {
        null == fontSize || "" == fontSize ? $.jdtoolbox.removeCssAttr("text_in", "font-size") : $("#text_in").css("font-size", fontSize)
    }

    function setColor(color) {
        null == color || "" == color ? $.jdtoolbox.removeCssAttr("text_in", "color") : $("#text_in").css("color", color)
    }

    function setHotKey(hotKey) {
        null != hotKey && "" != hotKey && ($("#" + hotKey).addClass("current").siblings().removeClass("current"), $(".im-btn-send-area").attr("title", $.trim($("#" + hotKey).text())))
    }

    function changeFontType() {
        var fontType = $("#text_in_font_type").val();
        jQuery.jdtoolbox.cookie("fontType", fontType, {
            expires: settings.expires,
            path: settings.path,
            domain: settings.domain
        }), setFontType(fontType)
    }

    function changeFontSize() {
        var fontSize = $("#text_in_font_size").val();
        jQuery.jdtoolbox.cookie("fontSize", fontSize, {
            expires: settings.expires,
            path: settings.path,
            domain: settings.domain
        }), setFontSize(fontSize)
    }

    function getTextIn() {
        var inputText = $("#text_in");
        if (inputText.find("iframe").remove(), inputText.find("link").remove(), inputText.find("frame").remove(), inputText.find("frameset").remove(), inputText.find("input").remove(), inputText.find("select").remove(), inputText.find("textarea").remove(), inputText.find("button").remove(), inputText.find("param").remove(), inputText.find("object").remove(), inputText.find("div").each(function () {
                if ($(this).find("img").length > 0) {
                    var html = $(this).html();
                    return $(this).replaceWith(html), void 0
                }
                "" == $.trim($(this).text()) && $(this).remove()
            }), inputText.find("p").each(function () {
                var html = $(this).html().replace(/&nbsp;/gi, "");
                html = $.trim(html), "" == html && $(this).remove()
            }), "MSIE" == $.jdtoolbox.browerType()) {
            var html = inputText.html();
            html = html.replace(/<p>/gi, ""), html = html.replace(/<\/p>/gi, "<br />"), inputText.html(html)
        }
        inputText.find("script").remove();
        var text_in = "";
        text_in = sendEscape(inputText.html());
        var text_in_json = {};
        if (null == text_in || void 0 == text_in) return null;
        text_in_json.D = text_in;
        var fontType = $("#text_in").css("font-family"),
            f = -1;
        $.each(settings.fontType, function (index, value) {
            fontType == value.value && (f = value.code)
        }), -1 != f && (text_in_json.F = f);
        var fontSize = $("#text_in").css("font-size"),
            s = -1;
        $.each(settings.fontSize, function (index, value) {
            fontSize == value.value && (s = value.code)
        }), -1 == s && (fontSize = document.getElementById("text_in").style.fontSize, fontSize && $.each(settings.fontSize, function (index, value) {
            fontSize == value.value && (s = value.code)
        })), -1 != s && (text_in_json.S = s);
        var color = $("#text_in").css("color"),
            rgbArray = null;
        return "#000000" != color && "rgb(0,0,0)" != color && (rgbArray = $.jdtoolbox.rgb(color)), null != rgbArray && (text_in_json.R = rgbArray[0], text_in_json.G = rgbArray[1], text_in_json.B = rgbArray[2]), text_in_json
    }

    function getTextJson(text_in) {
        var text_in_json = {};
        if (null == text_in || void 0 == text_in) return null;
        text_in_json.D = text_in;
        var fontType = $("#text_in").css("font-family"),
            f = -1;
        $.each(settings.fontType, function (index, value) {
            fontType == value.value && (f = value.code)
        }), -1 != f && (text_in_json.F = f);
        var fontSize = $("#text_in").css("font-size"),
            s = -1;
        $.each(settings.fontSize, function (index, value) {
            fontSize == value.value && (s = value.code)
        }), -1 != s && (text_in_json.S = s);
        var color = $("#text_in").css("color"),
            rgbArray = null;
        return "#000000" != color && "rgb(0,0,0)" != color && (rgbArray = $.jdtoolbox.rgb(color)), null != rgbArray && (text_in_json.R = rgbArray[0], text_in_json.G = rgbArray[1], text_in_json.B = rgbArray[2]), text_in_json
    }

    function autoAllot() {
        if (autoAllotFun) return autoAllotFun(), void 0;
        if (settings.isChange) {
            var showMsg = "";
            showMsg = settings.zj.accountPin.length - "客服".length == settings.zj.accountPin.lastIndexOf("客服") ? "正在转接" + settings.zj.accountPin + "，请稍后..." : "正在转接" + settings.zj.accountPin + "客服，请稍后...", 3 == jdchat.serverType && (showMsg = "正在转接京东客服，请稍后..."), systemInfo(showMsg, "text_1_s")
        } else if (null != settings.kf) return createLinkFun(settings.kf), void 0;
        var data = {
            pid: settings.pid,
            pamsUid: settings.pamsUid
        };
        if (!$.jdtoolbox.isNull(settings.zj)) {
            var zjuuid = settings.zj.uuid;
            data.zjuuid = zjuuid
        }
        var start = (new Date).getTime();
        jQuery.ajax({
            url: "/refresh.action?t=" + start,
            type: "GET",
            data: data,
            timeout: 3e4,
            success: function (xml) {
                if (null != xml && "" != xml) {
                    var result = $.jdtoolbox.string2json("(" + xml + ")");
                    if (result.systime) {
                        var rtp = (new Date).getTime() - start;
                        settings.systime = result.systime - 1e3 + rtp, clock()
                    }
                    1 == result.code ? (settings.pin = result.pin, settings.kf = result.account, settings.kf.serverType = result.serverType ? result.serverType : "", null != result.brandName ? ($("#logoTitle").html("&nbsp;&nbsp;" + result.brandName + "品牌厂商在线客服"), $("#shopLogo").remove()) : null != result.shop && ($("#logoTitle").html("&nbsp;&nbsp;" + result.shop.appName + "在线客服"), $.jdtoolbox.isNull(result.shop.shopLogo) || $("#shopLogo")[0] || ($("<a href='http://mall.360buy.com/index-" + result.shop.id + ".html' target='_blank'><img id='shopLogo' height='50px' width='180px'></a>").insertAfter("#shopTitle"), $("#shopLogo").attr("src", getPopShopLogoURL(result.shop.shopLogo)))), "jingdong" == jdchat.versionType && $(".im-go-jimi").show(), createLinkFun(settings.kf, settings.isChange)) : setTimeout(autoAllot, 1e4)
                }
            },
            complete: function (xml) {
                xml = null
            }
        })
    }

    function clock() {
        settings.systime && (settings.systime += 1e3, curr && clearTimeout(curr), curr = setTimeout(clock, 1e3))
    }

    function getVibrationJson() {
        var json = "{";
        return json += "D:'#A_振动'", json += "}"
    }

    function sendEscape(srcString) {
        if (!srcString) return null;
        var temp = srcString;
        return temp = temp.replace(/<br\/?>/g, ""), 0 == temp.length ? temp : (temp = temp.replace(/<p>&nbsp;<\/p>/gi, ""), 0 == temp.length ? temp : 0 == temp.length ? temp : (temp = temp.replace(/&nbsp;/gi, ""), temp = $.trim(temp), 1 >= temp.length ? temp : (temp = temp.replace(/<p> <\/p>/gi, ""), 0 == temp.length ? temp : ("hotkey1" == initHotKey() && (srcString = srcString.replace(/<br\/?>/g, "")), srcString))))
    }

    function showUnescape(srcString) {
        return srcString = $.fn.jdExpression.replaceName(srcString)
    }

    function closeChat() {
        return checkLeave()
    }

    function closePage() {
        statusManager.currentStatus = statusManager.systemStatus.leave;
        try {
            window.opener = null, window.open("", "_self"), window.close()
        } catch (e) {
            try {
                window.open("", "_parent", ""), window.close()
            } catch (e) {
                window.close()
            }
        }
        settings.stopPoll = !0, systemInfo("您已关闭会话，感谢您的支持", "text_1_s")
    }

    function checkLeave(flag) {
        var param = null;
        if (statusManager.statusDataMap[statusManager.systemStatus.chating] ? param = statusManager.statusDataMap[statusManager.systemStatus.chating] : statusManager.statusDataMap[statusManager.systemStatus.queue] ? param = statusManager.statusDataMap[statusManager.systemStatus.queue] : (param = {}, param.lineKey = "", param.lineNo = "", settings.uid || (settings.uid = ""), "" == settings.sid && statusManager.statusDataMap[statusManager.systemStatus.concat] && (settings.sid = statusManager.statusDataMap[statusManager.systemStatus.concat].sid)), statusManager.currentStatus != statusManager.systemStatus.leave) {
            var url = "/close.action?t=" + (new Date).getTime() + "&sid=" + settings.sid + "&count=" + settings.count + "&resCode=" + settings.res_id + "&cid=" + settings.cid + "&lineKey=" + param.lineKey + "&lineNo=" + param.lineNo + "&pamsUid=" + settings.pamsUid + "&uid=" + settings.uid;
            jQuery.ajax({
                url: url,
                type: "get",
                async: !1,
                timeout: 3e3,
                success: function () {
                    statusManager.currentStatus != statusManager.systemStatus.refresh && (flag || closePage())
                }
            }), settings && settings.cur_ajax_request && (settings.cur_ajax_request.abort(), settings.isPoll = !1)
        }
    }

    function systemInfo(msg, style) {
        var scrollDiv = $("#scrollDiv");
        $lastDiv = $("#scrollDiv > div").last(), $("#scrollDiv div[logicType='dongdong-tip']").remove(), scrollDiv.append(buildSysMsgView(msg, style)), setTimeout(function () {
            $("#scrollDiv").scrollTop($("#scrollDiv")[0].scrollHeight)
        }, 0), clearMany()
    }

    function buildSysMsgView(msg, style) {
        var defaultStyle = "im-msg-notice",
            defaultIconStyle = "im-icon-notice",
            msgView = ['<div class="im-msg">', '<i class=""></i>', "<p></p>", '<span class="im-msg-tl"></span>', '<span class="im-msg-tr"></span>', '<span class="im-msg-bl"></span>', '<span class="im-msg-br"></span>', "</div>"],
            $sysMsg = $(msgView.join(""));
        switch (style && "text_1_s" != style && "text_1_st" != style && (defaultStyle = style), style) {
            case "im-msg-prompt":
                defaultIconStyle = "im-icon-exclamation";
                break;
            case "im-msg-success":
                defaultIconStyle = "im-icon-success";
                break;
            case "im-msg-error":
                defaultIconStyle = "im-icon-error";
                break;
            default:
        }
        return $sysMsg.addClass(defaultStyle), $sysMsg.find("i").addClass(defaultIconStyle), $sysMsg.find("p").html(msg), $sysMsg
    }

    function buildRiskTipMsgView(msg, style) {
        var defaultStyle = "im-msg-notice",
            defaultIconStyle = "im-icon-notice",
            msgView = ['<div id="risktip" class="im-msg">', '<i class=""></i>', "<p></p>", '<span class="im-msg-tl"></span>', '<span class="im-msg-tr"></span>', '<span class="im-msg-bl"></span>', '<span class="im-msg-br"></span>', "</div>"],
            $sysMsg = $(msgView.join(""));
        switch (style && "text_1_s" != style && "text_1_st" != style && (defaultStyle = style), style) {
            case "im-msg-prompt":
                defaultIconStyle = "im-icon-exclamation";
                break;
            case "im-msg-success":
                defaultIconStyle = "im-icon-success";
                break;
            case "im-msg-error":
                defaultIconStyle = "im-icon-error";
                break;
            default:
        }
        return $sysMsg.addClass(defaultStyle), $sysMsg.find("i").addClass(defaultIconStyle), $sysMsg.find("p").html(msg), $sysMsg
    }

    function showSystemMessage(message, logicType, showType, style) {
        var chatPanel = $("#scrollDiv"),
            content = systemView(message, logicType, showType, style),
            onePanels = $("#scrollDiv div[showtype='one']");
        if (onePanels.length > 0 && onePanels.remove(), "serving" == logicType && ($("#scrollDiv div[logicType='into-queue']").remove(), $("#scrollDiv div[logicType='queue-progress']").remove(), $("#scrollDiv div[logicType='queue-out']").remove(), $("#scrollDiv div[logicType='dongdong-tip']").remove()), "dongdong-tip" == logicType && $("#scrollDiv div[logicType='dongdong-tip']").remove(), "append" == showType) chatPanel.append(content);
        else if ("only" == showType) {
            var messagePanel = $("#scrollDiv div[logictype='" + logicType + "']");
            messagePanel.length > 0 ? ($lastSpan = messagePanel.children("p"), $lastSpan.html(message)) : chatPanel.append(content)
        }
        chatPanel.scrollTop(chatPanel[0].scrollHeight), clearMany()
    }

    function systemView(message, logicType, showType, style) {
        var defaultStyle = "im-msg-notice",
            defaultIconStyle = "im-icon-notice",
            msgView = ['<div class="im-msg">', '<i class=""></i>', "<p></p>", '<span class="im-msg-tl"></span>', '<span class="im-msg-tr"></span>', '<span class="im-msg-bl"></span>', '<span class="im-msg-br"></span>', "</div>"],
            $sysMsg = $(msgView.join(""));
        switch (style && "text_1_s" != style && "text_1_st" != style && (defaultStyle = style), style) {
            case "im-msg-prompt":
                defaultIconStyle = "im-icon-exclamation";
                break;
            case "im-msg-success":
                defaultIconStyle = "im-icon-success";
                break;
            case "im-msg-error":
                defaultIconStyle = "im-icon-error";
                break;
            default:
        }
        return $sysMsg.addClass(defaultStyle), $sysMsg.attr("logictype", logicType), $sysMsg.attr("showtype", showType), $sysMsg.find("i").addClass(defaultIconStyle), $sysMsg.find("p").html(message), $sysMsg
    }

    function clearMany() {
        var childEl = $("#scrollDiv").find(".text_1_c"),
            size = childEl.length;
        if (size > 100) for (var i = 0; 50 > i; i++) childEl.eq(i).prev().remove(), childEl.eq(i).remove()
    }

    function getZjToOldJson() {
        var json = "{";
        return json += "D:'#A_转接-o',pin:'" + settings.pin + "'", json += "}"
    }

    function getZjJson() {
        var json = "{";
        return json += "D:'#A_转接',pin:'" + settings.pin + "'", json += "}"
    }

    function replaceURL(srcString) {
        var regexA = RegExp("[iI][mM][gG][\\s\\S]*[sS][rR][cC][\\s\\S]*[=]"),
            regexIMG = RegExp("[aA][\\s\\S]*[hH][rR][eE][Ff][\\s\\S]*[=]");
        if (regexA.test(srcString) || regexIMG.test(srcString)) return srcString;
        var atag, reg = /(http:\/\/|https:\/\/)((\w|=|\?|\.|\/|&|-|\+|\(|\)|;|%|#|,|_|:)+)/g;
        if (reg.test(srcString)) atag = srcString.replace(reg, "<a href='$1$2' target=\"_blank\" tips='tips'>$1$2</a>");
        else {
            var reg = /(www\.)((\w|=|\?|\.|\/|&|-|\+|\(|\)|;|%|#|,|_|:)+)/g;
            atag = srcString.replace(reg, "<a href='http://$1$2' target=\"_blank\" tips='tips'>$1$2</a>")
        }
        return atag
    }

    function isJdLink(link) {
        var reg = /(http:\/\/|https:\/\/)(\w{2,}\.)*(360buy\.com|jd\.com)((\w|=|\?|\.|\/|&|-|\+|\(|\)|;|%)*)/;
        return reg.test(link)
    }

    function showMsg(bean, showNick, timeStr, isUnescape, serviceSend, keyWordTip, urlTipMap) {
        showNick || (showNick = statusManager.currentStatus == statusManager.systemStatus.message ? "[留言]" : settings.serviceNickName ? settings.serviceNickName : settings.curKf.nickName ? settings.curKf.nickName : settings.curKf.pin);
        var $text1s, saveStyle = !1;
        if (serviceSend || "[留言]" == showNick) {
            if ($text1s = buildServiceContent(), 3 == jdchat.serverType) $text1s.find(".im-txt-bold").html("京东客服");
            else {
                if ($text1s.find(".im-txt-bold").html(showNick), settings.waiterAvatarLoaded && $text1s.find(".userPic").attr("src", settings.waiterAvatar), settings.waiterAvatar && !settings.waiterAvatarLoaded) try {
                    var img = new Image;
                    img.src = settings.waiterAvatar, img.onload = function () {
                        $text1s.find(".userPic").attr("src", settings.waiterAvatar), settings.waiterAvatarLoaded = !0
                    }
                } catch (e) {
                }
                if ("[留言]" === showNick && ($text1s.find(".userPic").attr("src", "/img/user1.png"), settings.customerAvatarLoaded && $text1s.find(".userPic").attr("src", settings.customerAvatar), settings.customerAvatar && !settings.customerAvatarLoaded)) try {
                    var img = new Image;
                    img.src = settings.customerAvatar, img.onload = function () {
                        $text1s.find(".userPic").attr("src", settings.customerAvatar), settings.customerAvatarLoaded = !0
                    }
                } catch (e) {
                }
            }
            saveStyle = !0
        } else if ($text1s = buildClientContent(), $text1s.find(".im-txt-bold").html(showNick), settings.customerAvatarLoaded && $text1s.find(".userPic").attr("src", settings.customerAvatar), settings.customerAvatar && !settings.customerAvatarLoaded) try {
            var img = new Image;
            img.src = settings.customerAvatar, img.onload = function () {
                $text1s.find(".userPic").attr("src", settings.customerAvatar), settings.customerAvatarLoaded = !0
            }
        } catch (e) {
        }
        $text1s.find(".im-send-time").html(timeStr), $text1s.appendTo("#scrollDiv");
        var $text1c = $text1s.find(".im-message-content p");
        if ("string" == typeof bean && bean.indexOf("{") >= 0 && (bean = bean.substring(bean.indexOf("{"), bean.lastIndexOf("}") + 1), bean = $.jdtoolbox.string2json(bean)), "object" == typeof bean) {
            var content = replaceURL(bean.D);
            $text1c.html(showUnescape(content));
            var fontTypeCode = bean.F;
            null != fontTypeCode && void 0 != fontTypeCode && $.each(settings.fontType, function (index, value) {
                fontTypeCode == value.code && ($text1c.css("font-family", value.value), saveStyle && jQuery.jdtoolbox.cookie("s-fontType", value.value, {
                    expires: settings.expires,
                    path: settings.path,
                    domain: settings.domain
                }))
            });
            var fontSizeCode = bean.S;
            if (null != fontSizeCode && void 0 != fontSizeCode) {
                var svalue = null;
                $.each(settings.fontSize, function (index, value) {
                    fontSizeCode == value.code && ($text1c.css("font-size", value.value), svalue = value.value)
                }), null == svalue && $text1c.css("font-size", fontSizeCode), saveStyle && jQuery.jdtoolbox.cookie("s-fontSize", fontSizeCode, {
                    expires: settings.expires,
                    path: settings.path,
                    domain: settings.domain
                })
            }
            var colorR = bean.R,
                colorG = bean.G,
                colorB = bean.B;
            if (null != colorR && void 0 != colorR || null != colorG && void 0 != colorG || null != colorB && void 0 != colorB) {
                var r = null != colorR && void 0 != colorR ? colorR : 0,
                    g = null != colorG && void 0 != colorG ? colorG : 0,
                    b = null != colorB && void 0 != colorB ? colorB : 0,
                    rgbArray = Array(3);
                rgbArray[0] = r, rgbArray[1] = g, rgbArray[2] = b;
                var color = $.jdtoolbox.rgb2color(rgbArray);
                $text1c.css("color", color), saveStyle && jQuery.jdtoolbox.cookie("s-color", color, {
                    expires: settings.expires,
                    path: settings.path,
                    domain: settings.domain
                })
            }
        } else {
            var str = replaceURL(bean);
            $text1c.html(showUnescape(str))
        }
        $("#scrollDiv a[tips='tips']").unbind("click"), $("#scrollDiv a[tips='tips']").bind("click", function () {
            var href = $(this).attr("href");
            return isJdLink(href) ? (window.open(href), !1) : confirm("确定要访问以下地址：\n\n" + href + "\n\n请注意检查链接是否安全") ? (window.open(href), !1) : !1
        }), clearMany(), setTimeout(function () {
            $("#scrollDiv").scrollTop($("#scrollDiv")[0].scrollHeight)
        }, 0), $("img", $("#scrollDiv")), $(".message-img").length > 0 && $("#scrollDiv .message-img").parent().lightBox(), showSecurityTip(keyWordTip, urlTipMap)
    }

    function showSecurityTip(keywordTip, urlTips) {
        var xOffset = 10,
            yOffset = 20;
        if (keywordTip && "undefined" != keywordTip && systemInfo(keywordTip, "im-msg-prompt"), urlTips && "undefined" != urlTips) {
            var linkElem = $("#scrollDiv a[tips='tips']:last").last(),
                href = $(linkElem).attr("href");
            if (-1 != href.indexOf("http://")) {
                var urlTip = urlTips["" + href];
                if (urlTip) $("<span class='riskIcon'></span>").insertBefore($(linkElem)), $(linkElem).css("marginLeft", "20px"), $(linkElem).unbind("mouseover"), $(linkElem).bind("mouseover", function (e) {
                    $("body").append(buildRiskTipMsgView(urlTip, "im-msg-prompt")), $("#risktip").css("top", e.pageY + xOffset + "px").css("left", e.pageX - yOffset + "px").fadeIn("fast")
                }), $(linkElem).unbind("mouseout"), $(linkElem).bind("mouseout", function () {
                    $("#risktip").remove()
                });
                else {
                    var addr = href.substring(href.indexOf("http://") + 7);
                    urlTip = urlTips["" + addr], urlTip && ($("<span class='riskIcon'></span>").insertBefore($(linkElem)), $(linkElem).css("marginLeft", "20px"), $(linkElem).unbind("mouseover"), $(linkElem).bind("mouseover", function (e) {
                        $("body").append(buildRiskTipMsgView(urlTip, "im-msg-prompt")), $("#risktip").css("top", e.pageY + xOffset + "px").css("left", e.pageX - yOffset + "px").fadeIn("fast")
                    }), $(linkElem).unbind("mouseout"), $(linkElem).bind("mouseout", function () {
                        $("#risktip").remove()
                    }))
                }
            } else {
                var urlTip = urlTips["" + href];
                urlTip ? ($("<span class='riskIcon'></span>").insertBefore($(linkElem)), $(linkElem).css("marginLeft", "20px"), $(linkElem).css("marginLeft", "20px"), $(linkElem).unbind("mouseover"), $(linkElem).bind("mouseover", function (e) {
                    $("body").append(buildRiskTipMsgView(urlTip, "im-msg-prompt")), $("#risktip").css("top", e.pageY + xOffset + "px").css("left", e.pageX - yOffset + "px").fadeIn("fast")
                }), $(linkElem).unbind("mouseout"), $(linkElem).bind("mouseout", function () {
                    $("#risktip").remove()
                })) : (urlTip = urlTips["http://" + href], urlTip && ($("<span class='riskIcon'></span>").insertBefore($(linkElem)), $(linkElem).css("marginLeft", "20px"), $(linkElem).css("marginLeft", "20px"), $(linkElem).unbind("mouseover"), $(linkElem).bind("mouseover", function (e) {
                    $("body").append(buildRiskTipMsgView(urlTip, "im-msg-prompt")), $("#risktip").css("top", e.pageY + xOffset + "px").css("left", e.pageX - yOffset + "px").fadeIn("fast")
                }), $(linkElem).unbind("mouseout"), $(linkElem).bind("mouseout", function () {
                    $("#risktip").remove()
                })))
            }
        }
    }

    function messageImgResize() {
        var $imgArray = $("#scrollDiv .message-img"),
            chatWidth = $("#scrollDiv").width();
        $imgArray.each(function (index, el) {
            var $img = $(el),
                imgWidth = $img.attr("data-width");
            imgWidth > chatWidth && ($img.width(Math.abs(chatWidth - 150)), $img.height((chatWidth - 150) * $img.attr("data-height") / imgWidth))
        })
    }

    function buildServiceContent() {
        var sContent = ['<div class="im-item im-others">', '<div class="im-message clearfix">', '<div class="im-user-area">', '<a href="#" class="im-user-pic">', '<img alt="京东客服" src="/img/user3.png" class="userPic">', '<img alt="" src="/img/cap48.png" class="im-user-pic-cap"></a>', "</div>", '<div class="im-message-detail">', '<table class="im-message-table" border="0" cellpadding="0" cellspacing="0">', " <tbody>", "   <tr>", '      <td class="lt"></td>', '     <td class="tt"></td>', '     <td class="rt"></td>', " </tr>", "<tr>", ' <td class="lm"><span></span></td>', '<td class="mm">', '<div class="im-message-title">', '<p class="im-message-owner">', '	<span class="im-txt-bold"></span>', "</p>", '	<span class="im-send-time">10:26</span>', "</div>", ' <div class="im-message-content">', " 	<p></p>", " </div>", " </td>", ' <td class="rm"><span></span></td>', "</tr>", "<tr>", '        <td class="lb"></td>', '           <td class="bm"></td>', '            <td class="rb"></td>', "         </tr>", "      </tbody>", "   </table>", "</div>"];
        return $(sContent.join(""))
    }

    function buildClientContent() {
        var cContent = ['<div class="im-item im-me">', '<div class="im-message clearfix">', '<div class="im-user-area">', '<a href="#" class="im-user-pic">', '<img alt="" src="/img/user1.png" class="userPic">', '<img alt="" src="/img/cap48.png" class="im-user-pic-cap"></a>', "</div>", '<div class="im-message-detail">', '<table class="im-message-table" border="0" cellpadding="0" cellspacing="0">', " <tbody>", "   <tr>", '      <td class="lt"></td>', '     <td class="tt"></td>', '     <td class="rt"></td>', " </tr>", "<tr>", ' <td class="lm"></td>', '<td class="mm">', '<div class="im-message-title">', '<p class="im-message-owner">', '	<span class="im-txt-bold"></span>', "</p>", '	<span class="im-send-time">10:26</span>', "</div>", ' <div class="im-message-content">', " 	<p></p>", " </div>", " </td>", ' <td class="rm"><span></span></td>', "</tr>", "<tr>", '        <td class="lb"></td>', '           <td class="bm"></td>', '            <td class="rb"></td>', "         </tr>", "      </tbody>", "   </table>", "</div>"];
        return $(cContent.join(""))
    }

    function checkInputCount(contentId, limit, tipId, text) {
        var _text = text || "",
            len = 0,
            jdom = $("#" + contentId);
        if (0 != jdom.length) {
            var nodeName = jdom.get(0).nodeName.toLowerCase();
            len = "textarea" == nodeName ? $.trim(jdom.val()).length : $.trim(jdom.text()).length;
            var r = limit - len;
            0 > r ? (r = -r, $("#" + tipId).html('您输入的文字已超过<span class="im-txt-num">' + r + "</span>字")) : $("#" + tipId).html(_text + ' 还可以输入<span class="im-txt-num">' + r + "</span>字")
        }
    }

    function poll() {
        var shareData = statusManager.statusDataMap[statusManager.systemStatus.concat],
            sid = shareData.sid,
            cid = shareData.cid;
        settings.isPoll = !0, settings.stopPoll || (null != settings.cur_ajax_request && settings.cur_ajax_request.abort(), pcount > 1 || (settings.cur_ajax_request = jQuery.ajax({
            url: "/chat/poll?t=" + (new Date).getTime(),
            type: "GET",
            data: {
                sid: sid,
                cid: cid
            },
            timeout: 4e4,
            beforeSend: function () {
                pcount++
            },
            complete: function (xml) {
                try {
                    if ("" != xml.responseText && 100 == xml.responseText) return systemInfo("由于您连续10分钟未发布任何消息，系统已经终止了此次咨询。如需继续咨询，<a href='#' onclick='$(this).trigger(\"im-reload\")'> 请点击此处</a>", "text_1_s"), "normal" == jdchat.versionType && showSystemMessage("一不留神就断开终止咨询？使用京东咚咚客户端随时发问&nbsp;&nbsp;<a href='" + settings.downloadURL + "' onclick=\"javascript:urlClick('" + "http://dongdong.jd.com','CHAT_OTHER');\"" + " target='_blank'>立即体验</a>", "dongdong-tip", "only"), jQuery("#sendMsg").unbind("click"), jQuery("#text_in").unbind("keypress"), settings.isPoll = !1, void 0;
                    if ("" != xml.responseText && 0 == xml.responseText) return systemInfo("系统已经终止了此次咨询。如果您需要继续咨询，<a href='#' onclick='$(this).trigger(\"im-reload\")'> 请点击此处</a>", "text_1_st"), settings.curKf.serverType && 3 != settings.curKf.serverType && $("#shopTitle").html(settings.curKf.nickName + '<font style="font-size:12px; font-weight:normal;">（离线）</font>'), "normal" == jdchat.versionType && showSystemMessage("一不留神就断开终止咨询？使用京东咚咚客户端随时发问&nbsp;&nbsp;<a href='" + settings.downloadURL + "' onclick=\"javascript:urlClick('" + "http://dongdong.jd.com','CHAT_OTHER');\"" + "  target='_blank'>立即体验</a>", "dongdong-tip", "only"), jQuery("#sendMsg").unbind("click"), jQuery("#text_in").unbind("keypress"), settings.isPoll = !1, void 0;
                    "" != xml.responseText && settings.lastData != xml.responseText && (parseMsg(xml.responseText), "jingdong" == jdchat.versionType && $(jdchat).trigger(jdchat.newMsgHandle, xml.responseText), xml = null)
                } catch (e) {
                }
                poll(), pcount--
            }
        })))
    }

    function parseMsg(msg) {
        if (settings.lastData = msg, msg) {
            for (var msgBeanArray = $.jdtoolbox.string2json("(" + msg + ")"), i = 0; msgBeanArray.length > i; i++) {
                msgBean = msgBeanArray[i];
                var msgContent = msgBean.content,
                    keywordTip = msgBean.keyWordPrompt,
                    urlTipMap = msgBean.urlPrompMap;
                if ("string" == typeof msgContent && msgContent.indexOf("{") >= 0 && (msgContent = msgContent.substring(msgContent.indexOf("{"), msgContent.lastIndexOf("}") + 1), msgContent = $.jdtoolbox.string2json(msgContent)), jdchat.serverType = msgBean.serverType, "object" != typeof msgContent || $.jdtoolbox.isNull(msgContent.C) || 1 != msgContent.C) settings.res_id = 100, "jingdong" == jdchat.versionType ? showMsg(msgContent, "京东客服", $.jdtoolbox.sysTime(settings.systime), !1, msgBean.servicesend, keywordTip, urlTipMap) : (3 == settings.curKf.serverType ? $("#shopTitle").html('京东客服<font style="font-size:12px; font-weight:normal;">（在线）</font>') : $("#shopTitle").html(settings.kf.nickName + '<font style="font-size:12px; font-weight:normal;">（在线）</font>'), showMsg(msgContent, msgBean.sender, $.jdtoolbox.sysTime(settings.systime), !1, msgBean.servicesend, keywordTip, urlTipMap)), settings.newMsg = !0;
                else {
                    msgContentData = msgContent.D;
                    var dataArray = msgContentData.split("|");
                    settings.isChange = !0, settings.orgId = msgContent.TO, settings.zj = {}, settings.zj.uuid = dataArray[0], settings.zj.accountPin = dataArray[1], autoAllot()
                }
            }
            try {
                $("#jquery_jplayer_1").jPlayer("play")
            } catch (e) {
                alert(e)
            }
        }
    }

    function getHistory(profile) {
        var data = {};
        settings.groupId && (data.groupId = settings.groupId), settings.brandName && (data.brandName = encodeURIComponent(encodeURIComponent(settings.brandName))), settings.venderId && (data.venderId = settings.venderId), settings.classify && (data.classify = settings.classify), settings.receiver && (data.receiver = encodeURIComponent(settings.receiver)), settings.pid && (data.pid = settings.pid), settings.cid && (data.cid = settings.cid), settings.sid && (data.sid = settings.sid), settings.operatorId && (data.operatorId = settings.operatorId), jQuery.ajax({
            url: "/history.action?t=" + (new Date).getTime(),
            type: "post",
            data: data,
            timeout: 12e4,
            success: function (xml) {
                try {
                    if (null != xml) {
                        var msgBean = $.jdtoolbox.string2json(xml);
                        if (msgBean.length > 0) for (var i = msgBean.length - 1; i >= 0; i--) if (0 > msgBean[i].content.indexOf("#A")) {
                            var content = $("<div></div>").html(msgBean[i].content),
                                keywordTip = msgBean[i].keyWordPrompt,
                                urlTipMap = msgBean[i].urlPrompMap;
                            content.find("*").each(function () {
                                for (var attrs = this.attributes, i = 0; attrs.length > i; i++) {
                                    var name = attrs[i].name.toLowerCase();
                                    0 == name.indexOf("on") && $(this).removeAttr(attrs[i].name)
                                }
                            }), getStyle(content, msgBean[i].servicesend);
                            var str = content.wrap("<span></span>").parent().html();
                            if ($(str).text() == "顾客 " + settings.pin + " 加入咨询") continue;
                            showMsg(str, msgBean[i].servicesend ? msgBean[i].serviceId : msgBean[i].customer, msgBean[i].created.replace("年", "-").replace("月", "-").replace("日", ""), !1, msgBean[i].servicesend, keywordTip, urlTipMap)
                        }
                        if (null != profile && "" != $.trim(profile)) {
                            profile = profile.replace(/</g, "&lt;").replace(/>/, "&gt;");
                            var json = getTextJson(profile);
                            showMsg(json, settings.kf.nickName, $.jdtoolbox.sysTime(settings.systime), !0, !0)
                        }
                    }
                } catch (e) {
                }
            }
        })
    }

    function getStyle($jdom, service) {
        var color, fontSize, fontType;
        service ? (color = jQuery.jdtoolbox.cookie("s-color"), fontSize = jQuery.jdtoolbox.cookie("s-fontSize"), fontType = jQuery.jdtoolbox.cookie("s-fontType")) : (color = jQuery.jdtoolbox.cookie("color"), fontSize = $.jdtoolbox.cookie("fontSize"), fontType = $.jdtoolbox.cookie("fontType")), color && $jdom.css("color", color), fontSize && $jdom.css("fontSize", fontSize), fontType && $jdom.css("fontFamily", fontType)
    }

    function collectA(val) {
        var rrr = /(?:<a[^>]*>)(.*?)(?:<\/a[^>]*>)/gi,
            rra = val.match(rrr);
        return rra
    }

    function getPopShopLogoURL(url) {
        return null != url && (url = (1 == url.indexOf("popshop") ? "http://img30.360buyimg.com" : "http://images.shop.360buy.com") + url), url
    }

    function getQueueData(lineKey, lineNo) {
        if (statusManager.currentStatus == statusManager.systemStatus.queue) {
            var data = {
                pid: settings.pid,
                lineKey: lineKey,
                lineNo: lineNo,
                pamsUid: settings.pamsUid
            };
            if (!$.jdtoolbox.isNull(settings.zj)) {
                var zjuuid = settings.zj.uuid;
                data.zjuuid = zjuuid
            }
            settings.orgId && (data.orgId = settings.orgId);
            var start = (new Date).getTime();
            jQuery.ajax({
                url: "refresh.action?t=" + start,
                type: "GET",
                data: data,
                timeout: 3e4,
                error: function () {
                    firstError ? (showSystemMessage("网络异常是否重新咨询<a href='#' onclick='$(this).trigger(\"im-reload\")'> 请点击此处重新咨询</a>", "queue-out", "only"), jdchat.waitMessage.stop()) : firstError = setTimeout(getQueueData, 5e3)
                },
                success: function (xml) {
                    if (null != xml && "" != xml) {
                        var result = $.jdtoolbox.string2json("(" + xml + ")");
                        if (result.systime) {
                            var rtp = (new Date).getTime() - start;
                            settings.systime = result.systime - 1e3 + rtp, clock()
                        }
                        if (-1 == result.code) 200 > result.waitNo ? showSystemMessage("您前面还有<code style='color:red;'>" + result.waitNo + "</code>位用户等待咨询", "queue-progress", "only") : result.waitNo >= 200 && 300 > result.waitNo ? showSystemMessage("目前您排在第<code style='color:red;'>" + result.waitNo + "</code>位,近期促销火爆，请您谅解 ", "queue-progress", "only") : showSystemMessage("当前客服繁忙，请耐心等待分配客服或使用网站自助服务", "queue-progress", "only"), result.waitNo > 3 && "normal" == jdchat.versionType && showSystemMessage("排队人数太多？登录京东咚咚客户端即刻咨询&nbsp;&nbsp;<a href='" + settings.downloadURL + "' onclick=\"javascript:urlClick('" + "http://dongdong.jd.com','CHAT_OTHER');\"" + "  target='_blank'>立即下载</a>", "dongdong-tip", "only"), setTimeout(function () {
                            getQueueData(lineKey, lineNo)
                        }, 5e3);
                        else if (1 == result.code) statusManager.currentStatus = statusManager.systemStatus.concat, settings.pin = result.pin, settings.kf = result.account, null != result.brandName ? ($("#logoTitle").html("&nbsp;&nbsp;" + result.brandName + "品牌厂商在线客服"), $("#shopLogo").remove()) : null != result.shop && ($("#logoTitle").html("&nbsp;&nbsp;" + result.shop.appName + "在线客服"), $.jdtoolbox.isNull(result.shop.shopLogo) || $("#shopLogo")[0] || ($("<a href='http://mall.360buy.com/index-" + result.shop.id + ".html' target='_blank'><img id='shopLogo' height='50px' width='180px'></a>").insertAfter("#shopTitle"), $("#shopLogo").attr("src", getPopShopLogoURL(result.shop.shopLogo)))), null != settings.kf.nickName ? 3 == jdchat.serverType ? $("#shopTitle").html('京东客服<font style="font-size:12px; font-weight:normal;">（在线）</font>') : $("#shopTitle").html(settings.kf.nickName + '<font style="font-size:12px; font-weight:normal;">（在线）</font>') : 3 == jdchat.serverType ? $("#shopTitle").html('京东客服<font style="font-size:12px; font-weight:normal;">（在线）</font>') : $("#shopTitle").html(settings.kf.pin + '<font style="font-size:12px; font-weight:normal;">（在线）</font>'), "normal" == jdchat.versionType ? createLinkFun(settings.kf, settings.isChange) : intoChatFun && intoChatFun(result), "jingdong" == jdchat.versionType && $(".im-go-jimi").show(), $("#shopTitle").show();
                        else {
                            if (-2 == result.code) return showSystemMessage("非常抱歉，由于网络繁忙您此次排队失败 <a href='#' onclick='$(this).trigger(\"im-reload\")'> 请点击此处重新排队</a>", "queue-out", "only"), jdchat.waitMessage.stop(), void 0;
                            setTimeout(getQueueData, 1e4)
                        }
                    }
                },
                complete: function (xml) {
                    xml = null
                }
            })
        }
    }

    function preLoad() {
        return this.support.loading ? void 0 : (alert("您的浏览器不支持播放flash文件，请安装flash插件"), !1)
    }

    function loadFailed() {
        alert("加载flash出现未知错误")
    }

    function fileQueued(file) {
        try {
            var progress = new FileProgress(file, this.customSettings.progressTarget);
            progress.setStatus("等待上传..."), progress.toggleCancel(!0, this)
        } catch (ex) {
            this.debug(ex)
        }
    }

    function fileQueueError(file, errorCode, message) {
        try {
            if (errorCode === SWFUpload.QUEUE_ERROR.QUEUE_LIMIT_EXCEEDED) return alert("You have attempted to queue too many files.\n" + (0 === message ? "You have reached the upload limit." : "You may select " + (message > 1 ? "up to " + message + " files." : "one file."))), void 0;
            var progress = new FileProgress(file, this.customSettings.progressTarget);
            switch (progress.setError(), progress.toggleCancel(!1), errorCode) {
                case SWFUpload.QUEUE_ERROR.FILE_EXCEEDS_SIZE_LIMIT:
                    progress.setStatus("文件大小" + this.settings.file_types_description), this.debug("Error Code: File too big, File name: " + file.name + ", File size: " + file.size + ", Message: " + message);
                    break;
                case SWFUpload.QUEUE_ERROR.ZERO_BYTE_FILE:
                    progress.setStatus("不能上传空文件."), this.debug("Error Code: Zero byte file, File name: " + file.name + ", File size: " + file.size + ", Message: " + message);
                    break;
                case SWFUpload.QUEUE_ERROR.INVALID_FILETYPE:
                    progress.setStatus("无效的文件类型."), this.debug("Error Code: Invalid File Type, File name: " + file.name + ", File size: " + file.size + ", Message: " + message);
                    break;
                default:
                    null !== file && progress.setStatus("未知错误"), this.debug("Error Code: " + errorCode + ", File name: " + file.name + ", File size: " + file.size + ", Message: " + message)
            }
        } catch (ex) {
            this.debug(ex)
        }
    }

    function fileDialogComplete(numFilesSelected) {
        $(".flash").show();
        try {
            this.startUpload()
        } catch (ex) {
            this.debug(ex)
        }
    }

    function uploadStart(file) {
        try {
            var progress = new FileProgress(file, this.customSettings.progressTarget);
            progress.setStatus("开始上传..."), progress.toggleCancel(!0, this)
        } catch (ex) {
        }
        return !0
    }

    function uploadProgress(file, bytesLoaded, bytesTotal) {
        try {
            var percent = Math.ceil(100 * (bytesLoaded / bytesTotal)),
                progress = new FileProgress(file, this.customSettings.progressTarget);
            progress.setProgress(percent), progress.setStatus("正在上传中...")
        } catch (ex) {
            this.debug(ex)
        }
    }

    function imgZoom(img) {
        var defaultWidth = 250,
            dstWidth = img.thumbnailsWidth,
            dstHeight = img.thumbnailsHeight;
        return (img.width > defaultWidth || img.height > defaultWidth) && (img.width >= img.height ? (dstWidth = defaultWidth, dstHeight = parseInt(defaultWidth * img.height / img.width)) : (dstHeight = defaultWidth, dstWidth = parseInt(defaultWidth * img.width / img.height))), {
            W: dstWidth,
            H: dstHeight
        }
    }

    function uploadSuccess(file, serverData) {
        var result, img;
        try {
            var progress = new FileProgress(file, this.customSettings.progressTarget);
            progress.setComplete(), progress.setStatus("上传完成."), progress.toggleCancel(!1), result = jQuery.jdtoolbox.string2json(serverData), 0 === result.status && ("image" === result.type ? (img = result.thumbnailsWidth && result.thumbnailsHeight ? '<img class="message-img" data-width=' + result.width + " data-height=" + result.height + " src=" + result.url + ' style="width:' + imgZoom(result).W + "px; height:" + imgZoom(result).H + 'px" />' : '<img class="message-img" data-width=' + result.width + " data-height=" + result.height + " src=" + result.url + ' style="width:' + result.width + "px; height:" + result.height + 'px" />', insertImg('<a rel="gallery" class="image-file" href="' + result.url + '">' + img + "</a>")) : "file" === result.type && insertImg('<a href="' + result.url + '" target="_blank" rel="send-file"><img src="http://chat.jd.com/img/send_file_icon.gif" style="width:20px;height:20px;" /> ' + file.name + "</a>"))
        } catch (ex) {
            this.debug(ex)
        }
    }

    function uploadError(file, errorCode, message) {
        try {
            var progress = new FileProgress(file, this.customSettings.progressTarget);
            switch (progress.setError(), progress.toggleCancel(!1), errorCode) {
                case SWFUpload.UPLOAD_ERROR.HTTP_ERROR:
                    progress.setStatus("上传错误: " + message), this.debug("Error Code: HTTP Error, File name: " + file.name + ", Message: " + message);
                    break;
                case SWFUpload.UPLOAD_ERROR.UPLOAD_FAILED:
                    progress.setStatus("上传失败."), this.debug("Error Code: Upload Failed, File name: " + file.name + ", File size: " + file.size + ", Message: " + message);
                    break;
                case SWFUpload.UPLOAD_ERROR.IO_ERROR:
                    progress.setStatus("Server (IO) Error"), this.debug("Error Code: IO Error, File name: " + file.name + ", Message: " + message);
                    break;
                case SWFUpload.UPLOAD_ERROR.SECURITY_ERROR:
                    progress.setStatus("Security Error"), this.debug("Error Code: Security Error, File name: " + file.name + ", Message: " + message);
                    break;
                case SWFUpload.UPLOAD_ERROR.UPLOAD_LIMIT_EXCEEDED:
                    progress.setStatus(SWFUpload.settings.file_types_description), this.debug("Error Code: Upload Limit Exceeded, File name: " + file.name + ", File size: " + file.size + ", Message: " + message);
                    break;
                case SWFUpload.UPLOAD_ERROR.FILE_VALIDATION_FAILED:
                    progress.setStatus("Failed Validation.  Upload skipped."), this.debug("Error Code: File Validation Failed, File name: " + file.name + ", File size: " + file.size + ", Message: " + message);
                    break;
                case SWFUpload.UPLOAD_ERROR.FILE_CANCELLED:
                    0 === this.getStats().files_queued && (document.getElementById(this.customSettings.cancelButtonId).disabled = !0), progress.setStatus("已取消"), progress.setCancelled();
                    break;
                case SWFUpload.UPLOAD_ERROR.UPLOAD_STOPPED:
                    progress.setStatus("已停止");
                    break;
                default:
                    progress.setStatus("Unhandled Error: " + errorCode), this.debug("Error Code: " + errorCode + ", File name: " + file.name + ", File size: " + file.size + ", Message: " + message)
            }
        } catch (ex) {
            this.debug(ex)
        }
    }

    function uploadComplete() {
        0 === this.getStats().files_queued && $(".flash").hide()
    }

    function queueComplete() {
    }

    function autoAddMsg(msg) {
        function showMsg(bean, showNick, timeStr) {
            document.getElementById("scrollDiv"), showNick || (showNick = statusManager.currentStatus == statusManager.systemStatus.message ? "[留言]" : settings.curKf.pin);
            var $text1s;
            $text1s = showNick == settings.pin ? $('<div class="text_1_u"></div>') : $('<div class="text_1_s"></div>'), $text1s.html(showNick);
            var $timeSpan = $("<span></span>");
            $timeSpan.html(timeStr), $timeSpan.appendTo($text1s), $text1s.appendTo("#scrollDiv");
            var $text1c = $('<div class="text_1_c"></div>');
            $text1c.html($.jdtoolbox.string2json(bean.D).title);
            var $tempDomNode = $("<span></span>").append($text1c);
            $("#scrollDiv").html($("#scrollDiv").html() + $tempDomNode.html()), setTimeout(function () {
                $("#scrollDiv").scrollTop($("#scrollDiv")[0].scrollHeight)
            }, 1e3)
        }

        if (statusManager.currentStatus == statusManager.systemStatus.chating || statusManager.currentStatus == statusManager.systemStatus.message) {
            var text_in_json = {},
                _msg = $.jdtoolbox.string2json(msg);
            text_in_json.D = msg, text_in_json.type = 1, showMsg(text_in_json, settings.pin, $.jdtoolbox.sysTime(settings.systime), !0), clearMany();
            var shareData = statusManager.statusDataMap[statusManager.systemStatus.concat],
                sid = shareData.sid,
                cid = shareData.cid,
                data = {
                    pid: settings.pid,
                    sid: sid,
                    cid: cid,
                    count: settings.count,
                    message: encodeURIComponent($.jdtoolbox.json2string(text_in_json))
                };
            statusManager.currentStatus != statusManager.systemStatus.message && (data.receiver = encodeURI(settings.curKf.pin));
            var start = (new Date).getTime();
            jQuery.ajax({
                url: "/post.action?pamsUid=" + settings.pamsUid + "&t=" + start,
                type: "post",
                data: data,
                timeout: 15e3,
                error: function () {
                    systemInfo("消息:" + _msg.title + "   发送失败,请稍后再试!", "text_1_s")
                },
                success: function (xml) {
                    if (null != xml && "" != xml) {
                        var result = $.jdtoolbox.string2json("(" + xml + ")");
                        if (result.systime) {
                            var rtp = (new Date).getTime() - start;
                            settings.systime = result.systime - 1e3 + rtp, clock()
                        }
                        1 == result.code ? (settings.isPoll || poll(), settings.count++) : 2 == result.code ? (systemInfo("运气稍差，被别人抢先了0.01秒，请您继续等待！", "text_1_s"), settings.kf = null, autoAllot(), settings.cur_ajax_request && (settings.cur_ajax_request.abort(), settings.isPoll = !1)) : 100 == result.code ? (systemInfo("您与当前客服的连接已中断,如果您需要继续咨询，<a href='#' onclick='$(this).trigger(\"im-reload\")'> 请点击此处</a>", "text_1_s"), settings.cur_ajax_request && (settings.cur_ajax_request.abort(), settings.isPoll = !1), "normal" == jdchat.versionType && common.showSystemMessage("一不留神就断开终止咨询？使用京东咚咚客户端随时发问&nbsp;&nbsp;<a href='" + settings.downloadURL + "' onclick=\"javascript:urlClick('" + "http://dongdong.jd.com','CHAT_OTHER');\"" + ">立即体验</a>", "dongdong-tip", "only")) : 15 == result.code ? systemInfo('由于您的发送速度太快<br />"' + text_in_json.D + '" 消息发送失败，请您稍后再试', "text_1_s") : 20 == result.code ? (checkLeave(), systemInfo("由于您更换了登录账号，与当前客服的连接已中断,如果您需要继续咨询，<a href='#' onclick='$(this).trigger(\"im-reload\")'> 请点击此处</a>", "text_1_s")) : 22 == result.code || 21 == result.code ? (checkLeave(), systemInfo("建立链接错误，与当前客服的连接已中断,如果您需要继续咨询，<a href='#' onclick='$(this).trigger(\"im-reload\")'> 请点击此处</a>", "text_1_s")) : 3 == result.code ? systemInfo("建立链接失败，请稍后再试！", "text_1_s") : 4 == result.code ? (alert("您已与此商家建立了对话，请去原窗口继续咨询；如果原窗口刚刚被您关闭，请十分钟后再试！"), settings.cur_ajax_request && (settings.cur_ajax_request.abort(), settings.isPoll = !1)) : 10 == result.code ? systemInfo("消息发送失败，请稍后再试！", "text_1_s") : 11 == result.code ? (systemInfo("对不起，当前客服繁忙，请您稍等！", "text_1_s"), settings.cur_ajax_request && (settings.cur_ajax_request.abort(), settings.isPoll = !1), settings = null) : 101 == result.code ? systemInfo("消息发送失败,接收者不能是自己!", "text_1_s") : 360 == result.code && systemInfo("消息发送失败,您发送的内容太长了!", "text_1_s")
                    }
                }
            }), $("#text_in").html(""), $("#text_in").blur(), $("#text_in").focus()
        }
    }

    function buildQuestionView(data, name) {
        if (!($("#im-question").length > 0 || 0 == data.length)) {
            var sItemView = buildServiceContent();
            sItemView.attr("id", "im-question");
            var contentWrap = sItemView.find(".im-message-content p"),
                title = $("<div></div>").html("您好，由于你尚在排队等候中，为了能帮助您快速解决问题，我们为您推荐了以下常见问题及解决方案。");
            name && sItemView.find(".im-txt-bold").html(name);
            for (var UL = $("<ul></ul>"), i = 0, Ln = data.length; Ln > i; i++) UL.append(buildQuestionItem(data[i], i + 1));
            contentWrap.append(title), contentWrap.append(UL), sItemView.appendTo($("#scrollDiv"))
        }
    }

    function ModuleQuestion(options, name) {
        void 0 != options && $.ajax({
            url: "/fqa/index.action",
            data: {
                orgId: options.orgId,
                pid: options.pid,
                venderId: options.venderId,
                rank3: options.classify,
                brandName: options.brandName
            },
            success: function (xml) {
                var json = $.jdtoolbox.string2json("(" + xml + ")");
                if (0 != json.length) {
                    for (var Ln = Math.min(json.length, jdchat.questionNum), arr = [], i = 0; Ln > i; i++) {
                        var obj = {};
                        obj.title = json[i].question, obj.content = json[i].answer, arr.push(obj)
                    }
                    buildQuestionView(arr, name)
                }
            },
            error: function () {
            }
        })
    }

    function buildQuestionItem(data, index) {
        var item = ['<li class="im-item">', '<p class="im-question"><a href="#" class="j_title"></a></p>', '<div class="question-content"></div></li>'],
            $item = $(item.join("")),
            title = $item.find(".j_title");
        return title.html(index + "、" + filter(data.title)), $item.find(".question-content").html(filter(data.content)), title.bind("click", function () {
            $($(this).parent().parent()).find(".question-content").toggle()
        }).hover(function () {
            $(this).css("color", "#C00")
        }, function () {
            $(this).css("color", "#005ea7")
        }), $item
    }

    function filter(content) {
        var $dom = $("<span></span>").html(content);
        return $dom.find("a").each(function () {
            $(this).attr("target", "_blank")
        }), $dom.html()
    }

    function setFsUploadProgress() {
        $("#fsUploadProgress").removeAttr("style").attr("style", " position: absolute; left: 0px; right:0; z-index: 99; bottom: 106px;height:0px")
    }

    statusManager = jdchat.statusManager, window.onbeforeunload = function () {
        return jdchat.status ? void 0 : statusManager.currentStatus != statusManager.systemStatus.leave ? "是否终止聊天" : void 0
    }, $(window).unload(function () {
        statusManager.currentStatus != statusManager.systemStatus.leave && (statusManager.currentStatus = statusManager.systemStatus.refresh, checkLeave())
    }).bind("im-reload", function () {
        var param = null;
        if (statusManager.statusDataMap[statusManager.systemStatus.chating] ? param = statusManager.statusDataMap[statusManager.systemStatus.chating] : statusManager.statusDataMap[statusManager.systemStatus.queue] ? param = statusManager.statusDataMap[statusManager.systemStatus.queue] : (param = {}, param.lineKey = "", param.lineNo = "", settings.uid || (settings.uid = ""), "" == settings.sid && statusManager.statusDataMap[statusManager.systemStatus.concat] && (settings.sid = statusManager.statusDataMap[statusManager.systemStatus.concat].sid)), statusManager.currentStatus != statusManager.systemStatus.leave) {
            var url = "/close.action?t=" + (new Date).getTime() + "&sid=" + settings.sid + "&count=" + settings.count + "&resCode=" + settings.res_id + "&cid=" + settings.cid + "&lineKey=" + param.lineKey + "&lineNo=" + param.lineNo + "&pamsUid=" + settings.pamsUid + "&uid=" + settings.uid;
            jQuery.ajax({
                url: url,
                type: "get",
                async: !1,
                timeout: 3e3,
                success: function () {
                    location.reload(!1)
                },
                error: function () {
                    location.reload(!1)
                }
            })
        }
    });
    var curr = null,
        pcount = 0,
        firstError = null;
    return {
        new_ms: new_ms,
        setExpression: setExpression,
        insertImg: insertImg,
        initFontType: initFontType,
        initFontSize: initFontSize,
        initHotKey: initHotKey,
        initColor: initColor,
        setFontType: setFontType,
        setFontSize: setFontSize,
        setColor: setColor,
        setHotKey: setHotKey,
        changeFontType: changeFontType,
        changeFontSize: changeFontSize,
        getTextIn: getTextIn,
        autoAllot: autoAllot,
        getVibrationJson: getVibrationJson,
        sendEscape: sendEscape,
        showUnescape: showUnescape,
        checkLeave: checkLeave,
        systemInfo: systemInfo,
        clearMany: clearMany,
        getZjToOldJson: getZjToOldJson,
        getZjJson: getZjJson,
        showMsg: showMsg,
        poll: poll,
        parseMsg: parseMsg,
        getHistory: getHistory,
        collectA: collectA,
        getQueueData: getQueueData,
        showSystemMessage: showSystemMessage,
        getPopShopLogoURL: getPopShopLogoURL,
        closeChat: closeChat,
        closePage: closePage,
        degree: degree,
        getTextJson: getTextJson,
        clock: clock,
        checkInputCount: checkInputCount,
        preLoad: preLoad,
        loadFailed: loadFailed,
        fileQueued: fileQueued,
        fileQueueError: fileQueueError,
        fileDialogComplete: fileDialogComplete,
        uploadStart: uploadStart,
        uploadProgress: uploadProgress,
        uploadSuccess: uploadSuccess,
        uploadError: uploadError,
        uploadComplete: uploadComplete,
        queueComplete: queueComplete,
        autoAddMsg: autoAddMsg,
        setFsUploadProgress: setFsUploadProgress,
        ModuleQuestion: ModuleQuestion,
        messageImgResize: messageImgResize
    }
}, Highlighter.prototype.highlight = function (node, keywords) {
    var _this = this;
    if (_this.element = node, keywords && node && node.nodeType && 1 == node.nodeType && (keywords = this.parsewords(keywords), null != keywords)) for (var i = 0; keywords.length > i; i++) this.colorword(node, keywords[i])
}, Highlighter.prototype.colorword = function (node, keyword) {
    for (var _this = this, i = 0; node.childNodes.length > i; i++) {
        var childNode = node.childNodes[i];
        if (3 == childNode.nodeType) {
            var re = RegExp(keyword.word, "i");
            if (-1 == childNode.data.search(re)) continue;
            re = RegExp("(" + keyword.word + ")", _this.global.toLowerCase() + "i");
            var forkNode = document.createElement("span"),
                newSpan = '<a style="color:' + keyword.color + '" data =' + keyword.detail + ">$1</a>";
            if (forkNode.innerHTML = childNode.data.replace(re, newSpan), _this.show && Boolean(_this.addEvent)) for (var i = 0; forkNode.getElementsByTagName("a").length > i; i++) forkNode.getElementsByTagName("a")[i].onmouseover = function () {
                _this.msgBox(this, this.getAttribute("data"))
            }, forkNode.getElementsByTagName("a")[i].onmouseout = function () {
                document.getElementById("box").style.display = "none"
            };
            node.replaceChild(forkNode, childNode)
        } else 1 == childNode.nodeType && this.colorword(childNode, keyword);
        if ("G" != _this.global.toUpperCase()) break
    }
}, Highlighter.prototype.msgBox = function (seft, string) {
    var _this = this,
        _string = string === void 0 || null == string ? "" : string,
        div = document.createElement("div");
    div.setAttribute("id", "box");
    var target = null;
    document.getElementById("box") || document.body.appendChild(div), target = document.getElementById("box"), _this.setStyle(target, {
        position: "absolute",
        width: "200px",
        height: "100px",
        backgroundColor: "#FFF",
        top: _this.getPosition(seft).Y + _this.getPosition(seft).H + "px",
        left: _this.getPosition(seft).X + "px",
        border: "#000 solid 1px"
    });
    for (var i = 0; target.childNodes.length > i; i++) target.removeChild(target.childNodes[i]);
    target.appendChild(document.createTextNode(_string)), _this.setStyle(target, {
        display: "block"
    })
}, Highlighter.prototype.getPosition = function (element) {
    function getWidth(e) {
        return e.offsetWidth
    }

    function getHeight(e) {
        return e.offsetHeight
    }

    function getTop(e) {
        var offset = e.offsetTop;
        return null != e.offsetParent && (offset += getTop(e.offsetParent)), offset
    }

    function getLeft(e) {
        var offset = e.offsetLeft;
        return null != e.offsetParent && (offset += getLeft(e.offsetParent)), offset
    }

    return {
        Y: getTop(element),
        X: getLeft(element),
        W: getWidth(element),
        H: getHeight(element)
    }
}, Highlighter.prototype.setStyle = function (element, styleObj) {
    if (element && styleObj) for (var i in styleObj) element.style[i] = styleObj[i]
}, Highlighter.prototype.parsewords = function (keywords) {
    if (null == keywords || 0 == keywords.length) return null;
    for (var results = [], i = 0; keywords.length > i; i++) {
        var keyword = {},
            color = this.colors;
        keyword.word = keywords[i], keyword.detail = keywords[i], keyword.color = color, results.push(keyword)
    }
    return results
};
var AutoBox = function (seft, $) {
        function registerEvent(handleOnclick) {
            _handleOnclick = handleOnclick;
            var oldValue = "",
                newValue = "";
            window.setInterval(function () {
                if (newValue = $("#text_in").text(), oldValue != newValue) {
                    if ("" == newValue || newValue.length > AJAXMax) return setTimeout(function () {
                        $("#list").empty(), $("#list").hide()
                    }, 0), void 0;
                    AJAXMax >= newValue.length && seft.ajax.getMsg(), oldValue = newValue
                }
            }, 100), $(document).bind("mouseup", function () {
                $("#list").hide()
            }), $(window).resize(function () {
                UL.css({
                    bottom: $(window).height() - $("#text_in").position().top - 25 + "px",
                    left: Left
                })
            })
        }

        function creatSearchList(data) {
            $("#list").empty();
            for (var i = 0; data.length > i; i++) {
                var _LI = createItem(data[i]);
                _LI.bind("click", function () {
                    return seft.ajax.sendMsg($(this).attr("data")), !1
                }), UL.append(_LI)
            }
            UL.appendTo($("body")).show(), $("#list div").hover(function () {
                $(this).css({
                    background: "#CCC"
                })
            }, function () {
                $(this).css({
                    background: "none"
                })
            }), UL.css({
                bottom: $(window).height() - $("#text_in").position().top - 25 + "px",
                left: Left
            }), highLighter.highlight(document.getElementById("list"), $("#text_in").text())
        }

        function createItem(data) {
            var LI = $('<div style="width:99%;height:30px;line-height:30px;overflow:hidden;padding:0 5px 0 5px"></div>');
            return LI.attr("data", $.jdtoolbox.json2string(data)), LI.append('<a href="javascript:void(0)"><div title="' + data.title + '" style="width:70%;display:inline-block;height:24px;overflow:hidden">' + data.title + '</div><div style="display:inline-block;color:#099;height:24px;float:right">' + "咨询" + data.counts + "次数</div>" + "</a>"), LI
        }

        var _element = null,
            UL = $('<div id="list"></div>'),
            highLighter = null,
            curAjax = null,
            AJAXMax = 15,
            Left = "9px",
            ListStyle = {},
            _handleOnclick = function () {
            },
            privateSettings = null;
        return seft.init = function (element, handleOnclick, style, settings) {
            privateSettings = settings || {}, _element = element instanceof Object ? element : $(element), window.setTimeout(function () {
                registerEvent(handleOnclick)
            }, 10), seft.setStyle(style), highLighter = new Highlighter({
                color: "red",
                evt: "false",
                g: "g"
            })
        }, seft.setStyle = function (style) {
            var defaultStyle = {
                    position: "absolute",
                    border: "#000 solid 1px",
                    "text-align": "left",
                    width: "80%",
                    border: "#CCC ridge 3px",
                    "background-color": "#FFF",
                    "-moz-border-radius": "4px",
                    "-webkit-border-radius": "4px",
                    "border-radius": "4px",
                    "border-bottom": "block",
                    "-webkit-box-shadow": "2px black",
                    "-moz-box-shadow": "2px black"
                },
                _style = {};
            if (void 0 != typeof style) {
                for (i in style) defaultStyle[i] = style[i] || defaultStyle[i];
                _style = defaultStyle
            }
            $("#list a").css({
                "text-decoration": "none",
                width: "100%"
            }), _style.left = Left, UL.css(_style)
        }, seft.checkStyle = function (defaultS, newS) {
            ListStyle = defaultS;
            for (i in defaultS) ListStyle[i] = newS[i] || defaultS[i]
        }, seft.ajax = {
            sendMsg: function (msg) {
                _handleOnclick(msg)
            },
            getMsg: function () {
                privateSettings.curKf && (curAjax = $.ajax({
                    url: "http://robot.360buy.com/search/search.action?q=" + encodeURI(encodeURI($("#text_in").text())) + "&pcAccount=" + encodeURI(encodeURI(privateSettings.curKf.pin)) + "&sf=title&rf=id|title|counts|standardId&sort=counts_0",
                    type: "GET",
                    dataType: "jsonp",
                    timeout: 3e4,
                    success: function (xml) {
                        if (null != xml) {
                            var json = xml;
                            return json && 0 == json.length ? ($("#list").empty(), $("#list").hide(), void 0) : (creatSearchList(json), void 0)
                        }
                    },
                    error: function () {
                    }
                }))
            }
        }, seft
    }(AutoBox || {}, jQuery),
    hostname = window.location.hostname;
document.domain = /360buy\.com/.test(hostname) ? "360buy.com" : "jd.com";
var dd_download_url = "";
(function ($) {
    function initExecute() {
        function _checkInputCount(contentId, limit, tipId, text) {
            var _text = text || "",
                len = 0,
                jdom = $("#" + contentId);
            if (0 != jdom.length) {
                var nodeName = jdom.get(0).nodeName.toLowerCase();
                len = "textarea" == nodeName ? $.trim(jdom.val()).length : $.trim(jdom.text()).length;
                var r = limit - len;
                0 > r ? (r = -r, $("#" + tipId).html('您输入的文字已超过<span class="im-txt-num">' + r + "</span>字")) : $("#" + tipId).html(_text + ' 还可以输入<span class="im-txt-num">' + r + "</span>字")
            }
        }

        function loadEvaq() {
            $.ajax({
                url: "/degree/getEvaQ.action?t=" + (new Date).getTime(),
                type: "GET",
                success: function (xml) {
                    try {
                        var list = eval("(" + xml + ")");
                        if (!list.length || 0 == list.length) return $(".evaq_list").children().length > 0 ? registerDegreeEvent() : $(".degreelt50").remove(), void 0;
                        for (var html = "", i = 0; list.length > i; i++) html += "<input type='checkbox' value='" + list[i].id + "' />" + list[i].title;
                        $(".evaq_list").empty().html(html), registerDegreeEvent()
                    } catch (e) {
                        $(".evaq_list").children().length > 0 ? registerDegreeEvent() : $(".degreelt50").remove()
                    }
                    $("#submitDegreeButton").attr("hidefocus", "true").attr("outline", "none")
                }
            })
        }

        function registerDegreeEvent() {
            $(".evaq_list input").change(function () {
                var textLen = $.trim($("#degreeContent").val()).length;
                $(".evaq_list").find("input:checked").length > 0 || textLen > 0 ? ($("#submitDegreeButton").removeClass("im-btn-dis").addClass("im-btn-submit"), $("#submitDegreeButton").bind("click", sendDegree)) : ($("#submitDegreeButton").removeClass("im-btn-submit").addClass("im-btn-dis"), $("#submitDegreeButton").unbind("click"))
            })
        }

        $("#text_in").click(function () {
        }), "Firefox" == $.jdtoolbox.browerType() && ($("#text_in").mousedown(function () {
            var val = $("#text_in").html();
            "" == val && $("#text_in").removeAttr("contenteditable")
        }), $("#text_in").css("outline", "none"), $("#text_in").mouseup(function () {
            $("#text_in").attr("contenteditable", "true"), $("#text_in").focus()
        })), $(document).click(function () {
            settings.newMsg = !1
        }), $("#change").click(function () {
            $(".cbut").toggle()
        }), $(".text_text3").jdExpression({
            imgClick: function (event) {
                var src = event.target.id,
                    img = this.getImg(src);
                common.insertImg(img), $(".text_text3").hide()
            }
        }), $(".cbut li").click(function () {
            $(document).unbind("keypress"), $(this).toggleClass("current"), $(this).siblings().toggleClass("current"), $(".cbut").hide(), $(".im-btn-send-area").attr("title", $.trim($(this).text()));
            var hotKeyId = $(this).attr("id");
            jQuery.jdtoolbox.cookie("hotKey", hotKeyId, {
                expires: settings.expires,
                path: settings.path,
                domain: settings.domain
            })
        }), $("#scrollDiv").bind("mousedown", function () {
            "none" != $(".j_popWord").css("display") && ($("#j_font").removeClass("im-icon-word-current"), $(".j_popWord").hide()), "none" != $(".j_recommend").css("display") && $(".j_recommend").hide(), "none" != $("#j_popFace").css("display") && ($("#expressionButton").removeClass("im-icon-face-current"), $("#j_popFace").hide())
        }), $("#talk_c").click(function () {
            return !settings.isDegree && settings.count > 0 && statusManager.currentStatus == statusManager.systemStatus.chating && !settings.groupId ? ($("#degreeButton").click(), $(".degree-tip").html("为了提高我们的服务质量，请您对本次服务做出评价，谢谢！").show(), void 0) : (common.checkLeave(), void 0)
        }), $("#text_in").click(function () {
            "none" != $(".j_popWord").css("display") && ($("#j_font").removeClass("im-icon-word-current"), $(".j_popWord").hide()), "none" != $(".j_recommend").css("display") && $(".j_recommend").hide(), "none" != $("#j_popFace").css("display") && ($("#expressionButton").removeClass("im-icon-face-current"), $("#j_popFace").hide())
        }), $("#text_in").keypress(function (e) {
            if (settings.newMsg = !1, $("#hotkey1").hasClass("current")) {
                if (!e.ctrlKey && (13 == e.which || 10 == e.which)) return setTimeout(add, 0), !1;
                if (e.ctrlKey && (13 == e.which || 10 == e.which)) {
                    var brower = $.jdtoolbox.browerType();
                    if ("MSIE" == brower) {
                        var selection = document.selection,
                            range = selection.createRange(),
                            str = "<br />";
                        range.pasteHTML(str), range.collapse(!1), range.select()
                    } else "Firefox" == brower ? common.insertImg("<div></div>") : common.insertImg("<div><br /></div>")
                }
            } else if (!e.ctrlKey || 13 != e.which && 10 != e.which || setTimeout(add, 0), !e.ctrlKey && 13 == e.which) {
                var brower = $.jdtoolbox.browerType();
                if ("MSIE" == brower) {
                    var selection = document.selection,
                        range = selection.createRange(),
                        str = "<br />";
                    return range.pasteHTML(str), range.collapse(!1), range.select(), !1
                }
            }
            return !0
        });
        var upload = null;
        $("#text_in").keydown(function (event) {
            if (8 == event.which) upload = [], $("#text_in").find("a[rel='send-file']").each(function () {
                upload.push($(this).html())
            });
            else if (event.ctrlKey && 90 == event.which && null != delUpload && delUpload.length > 0) {
                var s = delUpload.pop();
                common.insertImg(s)
            }
            _checkInputCount("text_in", 360, "chat_count")
        }), $("#text_in").keyup(function (event) {
            8 == event.which && $("#text_in").find("a[rel='send-file']").each(function () {
                for (var text = $(this).html(), i = 0; upload.length > i; i++) {
                    var s = upload[i];
                    if (s == text) break;
                    if (i == upload.length - 1) {
                        $(this).html(s);
                        var del = $(this).wrap("<span></span>").parent().html();
                        delUpload.push(del), systemInfo("ctrl + z 可以撤销刚删除的内容", "text_1_st"), $(this).parent().remove()
                    }
                }
            }), _checkInputCount("text_in", 360, "chat_count")
        }), $("#degreeContent").keyup(function () {
            _checkInputCount("degreeContent", 256, "degreeInput")
        }), $("#degreeContent").keydown(function () {
            _checkInputCount("degreeContent", 256, "degreeInput")
        }), $("#j_fontClose").bind("click", function () {
            $("#j_font").removeClass("im-icon-word-current"), $(this).parent().toggle()
        }), $("#jquery_jplayer_1").jPlayer({
            ready: function () {
                $(this).jPlayer("setMedia", {
                    m4a: jdchat.resourcesPath.getSkinResource("BELL7.m4a"),
                    oga: jdchat.resourcesPath.getSkinResource("BELL7.ogg")
                })
            },
            ended: function () {
            },
            swfPath: "/js",
            supplied: "m4a, oga"
        }), $("#j_font").click(function (ev) {
            "none" == $(".j_popWord").css("display") ? ($(ev.target).addClass("im-icon-word-current"), $(".j_popWord").show()) : ($(ev.target).removeClass("im-icon-word-current"), $(".j_popWord").hide()), "none" != $(".j_recommend").css("display") && $(".j_recommend").hide(), "none" != $("#j_popFace").css("display") && ($("#expressionButton").removeClass("im-icon-face-current"), $("#j_popFace").hide())
        }), $("#j_popFace").jdExpression({
            imgClick: function (event) {
                var src = event.target.id,
                    img = this.getImg(src);
                common.insertImg(img), $("#expressionButton").removeClass("im-icon-face-current"), $("#j_popFace").hide()
            }
        }), $("#degreeButton").click(function () {
            $(".j_recommend").find("input:checked").each(function () {
                $(this).removeAttr("checked")
            }), $(".degreeeq50").hide(), $(".degreelt50").hide(), $("#degreeContent").val(""), $(".j_recommend").removeAttr("style").attr("style", "left:216px; bottom: 38px; display:none"), $("#submitDegreeButton").removeClass("im-btn-submit").addClass("im-btn-dis"), $("#submitDegreeButton").unbind("click"), $(".degree-tip").hide(), 0 == $(".evaq_list").children().length && loadEvaq(), common.degree()
        }), $("#colorPanel").jdColor({
            click: function (color) {
                jQuery.jdtoolbox.cookie("color", color, {
                    expires: settings.expires,
                    path: settings.path,
                    domain: settings.domain
                }), common.setColor(color), $("#colorButton").removeClass("im-icon-color-hover"), $("#colorPanel").hide()
            }
        }), $("#colorButton").click(function () {
            $("#colorButton").addClass("im-icon-color-hover"), $("#colorPanel").show()
        }), $("#actionVibrationButton").click(function () {
            sendVibration()
        }), $("#degreeContent").keyup(function () {
            var len = $(".j_recommend :checked").length,
                textLen = $.trim($(this).val()).length,
                check = $(".evaq_list").children(":checked").length,
                degree = $("input:radio[name='degree'][checked]").val();
            return degree && degree >= 50 ? void 0 : textLen > 0 && len > 0 && degree && 50 > degree || len > 0 && check > 0 && degree && 50 > degree ? ($("#submitDegreeButton").removeClass("im-btn-dis").addClass("im-btn-submit"), $("#submitDegreeButton").bind("click", sendDegree), void 0) : ($("#submitDegreeButton").removeClass("im-btn-submit").addClass("im-btn-dis"), $("#submitDegreeButton").unbind("click"), void 0)
        }), $("input:radio[name='sloveQ']").click(function () {
            $("#submitDegreeButton").addClass("im-btn-submit").removeClass("im-btn-dis"), $("#submitDegreeButton").bind("click", sendDegree)
        }), $("input:radio[name='degree']").click(function () {
            $(".degree-tip").hide(), $(".evaq_list").children(":checkbox").removeAttr("checked"), $("input:radio[name='sloveQ']").removeAttr("checked"), $("#submitDegreeButton").removeClass("im-btn-submit").addClass("im-btn-dis"), $("#submitDegreeButton").unbind("click");
            var d = $(this).val();
            if (d > 50 && ($("#submitDegreeButton").addClass("im-btn-submit").removeClass("im-btn-dis"), $("#submitDegreeButton").bind("click", sendDegree)), 50 == d ? $(".degreeeq50").show() : $(".degreeeq50").hide(), 50 > d) {
                $(".degreelt50").show();
                var textLen = $.trim($("#degreeContent").val()).length;
                textLen > 0 && ($("#submitDegreeButton").addClass("im-btn-submit").removeClass("im-btn-dis"), $("#submitDegreeButton").bind("click", sendDegree))
            } else $(".degreelt50").hide()
        }), $("#degreeContent").keyup(function () {
            var len = $(".j_recommend :checked").length,
                textLen = $.trim($(this).val()).length,
                check = $(".evaq_list").children(":checked").length,
                degree = $("input:radio[name='degree'][checked]").val();
            return degree && degree >= 50 ? void 0 : textLen > 0 && len > 0 && degree && 50 > degree || len > 0 && check > 0 && degree && 50 > degree ? ($("#submitDegreeButton").removeClass("im-btn-dis").addClass("im-btn-submit"), $("#submitDegreeButton").bind("click", sendDegree), void 0) : ($("#submitDegreeButton").removeClass("im-btn-submit").addClass("im-btn-dis"), $("#submitDegreeButton").unbind("click"), void 0)
        }), common.showSystemMessage("正在验证登录....", "connect-server", "one"), autoSizeChatWindow();
        var os = detectOS();
        "Windows" != os || location.pathname.indexOf("agile") >= 0 || location.search.indexOf("groupId") > 0 ? (canChat = !0, initService()) : login(loadEvaq), document.body.onkeyup = function (e) {
            var event = e || window.event,
                code = event.keyCode || event.which;
            return 68 == code || 83 == code || 84 == code ? (event.stopPropagation && event.stopPropagation(), 0 == event.cancelBubble && (event.cancelBubble = !0), !1) : !0
        };
        var fontType = common.initFontType(),
            fontSize = common.initFontSize(),
            hotKey = common.initHotKey(),
            color = common.initColor();
        common.setFontType(fontType), common.setFontSize(fontSize), common.setHotKey(hotKey), common.setColor(color), $("#text_in_font_type").change(function () {
            common.changeFontType()
        }), $("#submitDegreeButton").click(function () {
            sendDegree()
        }), $("input:radio[name='degree']").click(function () {
            var d = $(this).val();
            50 == d ? $(".degreeeq50").show() : $(".degreeeq50").hide(), 50 > d ? $(".degreelt50").show() : $(".degreelt50").hide()
        });
        var degreeInputType = null;
        $(".j_recommend input").change(function () {
            if (3 == jdchat.serverType) {
                var val = $(this).val();
                50 >= val ? ($("#submitDegreeButton").removeClass("im-btn-submit").addClass("im-btn-dis"), $("#submitDegreeButton").unbind("click")) : ($("#submitDegreeButton").removeClass("im-btn-dis").addClass("im-btn-submit"), $("#submitDegreeButton").bind("click", sendDegree))
            }
        }), $(".degreeeq50 input").change(function () {
            3 == jdchat.serverType && ($("#submitDegreeButton").removeClass("im-btn-dis").addClass("im-btn-submit"), $("#submitDegreeButton").bind("click", sendDegree))
        }), $(".evaq_list input").change(function () {
            3 == jdchat.serverType && ($(".evaq_list").find("input:checked").length > 0 ? ($("#submitDegreeButton").removeClass("im-btn-dis").addClass("im-btn-submit"), $("#submitDegreeButton").bind("click", sendDegree)) : ($("#submitDegreeButton").removeClass("im-btn-submit").addClass("im-btn-dis"), $("#submitDegreeButton").unbind("click")))
        }), $("#expressionButton").click(function () {
            common.setExpression()
        }), $("#text_in_font_size").change(function () {
            common.changeFontSize()
        }), common.new_ms(), $("#text_in").blur(), $("#text_in").focus(), $("a.jf").click(function () {
            var href = $("a.jf").attr("href"),
                offset = href.indexOf("?");
            offset > 0 && (href = href.substring(0, offset)), settings.kf ? href = href + "?jfpin=" + encodeURIComponent(encodeURIComponent(settings.kf.pin)) : settings.curKf && (href = href + "?jfpin=" + encodeURIComponent(encodeURIComponent(settings.curKf.pin))), $(this).attr("href", href)
        })
    }

    function sendDegree() {
        if (0 == settings.count) return $(".degree-tip").html("非常抱歉，没有客服加入对话，无法进行满意度评价").show(), void 0;
        if (settings.isDegree) return $(".degree-tip").html("对不起，您已对本次服务做出过评价了").show(), void 0;
        if (settings.lastEvaluationTime && 1e4 > (new Date).getTime() - settings.lastEvaluationTime) return $(".degree-tip").html("正在提交数据，请稍后再试").show(), void 0;
        if (void 0 == $("input:radio[name='degree'][checked]").val()) return $(".degree-tip").html("为了提高我们的服务质量，请您选择一个满意度，谢谢！").show(), void 0;
        var degree = $("input:radio[name='degree'][checked]").val(),
            evaluation = $.trim($("#degreeContent").val());
        if (evaluation = $.jdtoolbox.replaceHtmlTag(evaluation), evaluation.length > 256) return $(".degree-tip").html("附加说明最多只允许发布256个字符，请删除超出部分再发布").show(), void 0;
        var sloveQ = $("input:radio[name='sloveQ'][checked]").val();
        if (50 == degree && !sloveQ) return $(".degree-tip").html("请选择您的问题是否得到解决").show(), void 0;
        var qlist = 1;
        if ($(".evaq_list").find("input:checked").each(function () {
                qlist *= parseInt($(this).val())
            }), 50 > degree && 1 == qlist && (qlist = -2, 0 == evaluation.length)) return $(".degree-tip").html("请告诉我们对客服不满意的原因，以便我们提高服务水平，谢谢！").show(), void 0;
        var evaqId = 1 == qlist ? sloveQ : qlist;
        $("input:radio[name='degree']").removeAttr("checked"), $("#degreeContent").val(""), settings.lastEvaluationTime = (new Date).getTime();
        var orgId = 0;
        settings.orgId && (orgId = settings.orgId), jQuery.ajax({
            url: "/degree/evaluation.action?t=" + (new Date).getTime(),
            type: "post",
            data: {
                pid: settings.pid,
                server: encodeURI(settings.curKf.pin),
                evaluation: encodeURI(evaluation),
                degree: degree,
                cid: settings.cid,
                sid: settings.sid,
                pamsUid: settings.pamsUid,
                uid: settings.uid,
                orgId: orgId,
                rank3: settings.classify,
                brandName: encodeURI(settings.brandName),
                shopId: settings.shopId,
                evaqId: evaqId
            },
            timeout: 3e4,
            success: function (xml) {
                if (null != xml && "" != xml) {
                    var result = $.jdtoolbox.string2json("(" + xml + ")");
                    3 == jdchat.serverType ? systemInfo("您对 京东客服 " + result.message, "text_1_s") : settings.curKf.nickName ? systemInfo("您对" + settings.curKf.nickName + result.message, "text_1_s") : systemInfo("您对" + settings.curKf.pin + result.message, "text_1_s"), $(".j_recommend").hide(), "success" == result.result ? settings.isDegree = !0 : 1 == result.code && (settings.isDegree = !0)
                }
            }
        })
    }

    function initHotKey() {
        return jQuery.jdtoolbox.cookie("hotKey")
    }

    function autoAllot() {
        common.autoAllot()
    }

    function checkLeave() {
        common.checkLeave()
    }

    function systemInfo(msg, style) {
        common.systemInfo(msg, style)
    }

    function getFQA() {
        var data = {};
        settings.pid && (data.pid = settings.pid), settings.classify && (data.rank3 = settings.classify), settings.brandName && (data.brandName = encodeURIComponent(encodeURIComponent(settings.brandName))), settings.venderId && (data.venderId = settings.venderId), $.ajax({
            url: "/fqa/index.action",
            type: "GET",
            data: data,
            success: function (xml) {
                var json = $.jdtoolbox.string2json("(" + xml + ")");
                if (!(null == json || 0 >= json.length)) {
                    var parent = $("#faq_id");
                    parent.append("<p></p>")
                }
            }
        })
    }

    function sendVibration() {
        if (statusManager.currentStatus != statusManager.systemStatus.chating) return common.systemInfo("非聊天模式不能发送震动", "im-msg-prompt"), void 0;
        if (lastsend) {
            var curr = (new Date).getTime(),
                diff = curr - lastsend;
            if (15e3 >= diff) return systemInfo("您发送的震动过于频繁，请稍后发送", "text_1_s"), void 0;
            lastsend = curr
        } else lastsend = (new Date).getTime();
        var vibrationJson = common.getVibrationJson(),
            shareData = statusManager.statusDataMap[statusManager.systemStatus.concat],
            sid = shareData.sid,
            cid = shareData.cid,
            data = {
                pid: settings.pid,
                sid: sid,
                cid: cid,
                count: settings.count,
                message: encodeURI($.jdtoolbox.json2string(vibrationJson)),
                orgId: settings.orgId,
                uid: settings.uid
            };
        statusManager.currentStatus != statusManager.systemStatus.message && (data.receiver = encodeURI(settings.curKf.pin));
        var start = (new Date).getTime();
        jQuery.ajax({
            url: "post.action?pamsUid=" + settings.pamsUid + "&t=" + start,
            type: "post",
            data: data,
            timeout: 15e3,
            error: function () {
                systemInfo("震动消息发送失败,请稍后再试!", "text_1_s")
            },
            success: function (xml) {
                if (null != xml && "" != xml) {
                    var result = $.jdtoolbox.string2json("(" + xml + ")");
                    if (result.systime) {
                        var rtp = (new Date).getTime() - start;
                        settings.systime = result.systime - 1e3 + rtp, common.clock()
                    }
                    1 == result.code ? (settings.isPoll || poll(), 3 == jdchat.serverType ? systemInfo("向 京东客服 发送了一个震动", "text_1_st") : settings.kf.nickName ? systemInfo("向" + settings.kf.nickName + "发送了一个震动", "text_1_st") : systemInfo("向" + settings.kf.pin + "发送了一个震动", "text_1_st"), clearMany(), settings.count++, msgSendCount++) : 2 == result.code ? (jdchat.switchMessage.showSysInfo(result.code), settings.cur_ajax_request && (settings.cur_ajax_request.abort(), settings.isPoll = !1)) : 100 == result.code ? (jdchat.switchMessage.showSysInfo(result.code), settings.cur_ajax_request && (settings.cur_ajax_request.abort(), settings.isPoll = !1)) : 15 == result.code ? jdchat.switchMessage.showSysInfo(result.code, "震动") : 3 == result.code ? jdchat.switchMessage.showSysInfo(result.code) : 20 == result.code ? (checkLeave(), jdchat.switchMessage.showSysInfo(result.code)) : 22 == result.code || 21 == result.code ? (checkLeave(), jdchat.switchMessage.showSysInfo(result.code)) : 4 == result.code ? common.systemInfo("您已与此商家建立了对话，请去原窗口继续咨询；如果原窗口刚刚被您关闭，请十分钟后再试！", "im-msg-prompt") : 10 == result.code || 0 == result.code ? jdchat.switchMessage.showSysInfo(result.code) : 101 == result.code ? jdchat.switchMessage.showSysInfo(result.code) : 360 == result.code && jdchat.switchMessage.showSysInfo(result.code)
                }
                settings.newMsg = !1, $("#text_in").blur(), $("#text_in").focus()
            }
        })
    }

    function add() {
        checkMsg(), delUpload = []
    }

    function sendMsg(text_in_json, hide) {
        var title = settings.pin;
        if ("" == settings.receiver && (title = "[留言]", settings.count >= 20)) return systemInfo("避免客服登录时，收到过多的离线消息，暂只能留言20条，请您谅解", "text_1_s"), void 0;
        hide || (common.showMsg(text_in_json, title, $.jdtoolbox.sysTime(settings.systime), !0, !1), clearMany());
        var shareData = statusManager.statusDataMap[statusManager.systemStatus.concat],
            sid = null,
            cid = null;
        hide ? (sid = settings.sid, cid = settings.cid) : (sid = shareData.sid, cid = shareData.cid);
        for (var msg = encodeURIComponent($.jdtoolbox.json2string(text_in_json)); msg && msg.indexOf("%C2%A0") > 0;) msg = msg.replace("%C2%A0", "&nbsp;");
        var data = {
            pid: settings.pid,
            sid: sid,
            cid: cid,
            count: settings.count,
            message: msg,
            orgId: settings.orgId,
            uid: settings.uid
        };
        statusManager.currentStatus != statusManager.systemStatus.message && (data.receiver = encodeURI(settings.curKf.pin));
        var text_in = $.trim(text_in_json.D),
            start = (new Date).getTime();
        jQuery.ajax({
            url: "post.action?pamsUid=" + settings.pamsUid + "&t=" + start,
            type: "post",
            data: data,
            timeout: 15e3,
            error: function () {
                hide || systemInfo("消息:" + text_in + "   发送失败,请稍后再试!", "text_1_s")
            },
            success: function (xml) {
                if (null != xml && "" != xml) {
                    var result = $.jdtoolbox.string2json("(" + xml + ")");
                    if (result.systime) {
                        var rtp = (new Date).getTime() - start;
                        settings.systime = result.systime - 1e3 + rtp, common.clock()
                    }
                    1 == result.code ? ("" != settings.receiver && (3 == jdchat.serverType ? $("#shopTitle").html('京东客服<font style="font-size:12px; font-weight:normal;">（在线）</font>') : $("#shopTitle").html(settings.kf.nickName + '<font style="font-size:12px; font-weight:normal;">（在线）</font>')), settings.isPoll || "" == settings.receiver || poll(), settings.count++, hide || msgSendCount++) : 2 == result.code ? (jdchat.switchMessage.showSysInfo(result.code), settings.kf = null, autoAllot(), settings.cur_ajax_request && (settings.cur_ajax_request.abort(), settings.isPoll = !1)) : 100 == result.code ? "" != settings.receiver || hide ? (jdchat.switchMessage.showSysInfo(result.code), settings.cur_ajax_request && (settings.cur_ajax_request.abort(), settings.isPoll = !1)) : systemInfo("留言失败，请稍后重试", "text_1_st") : 15 == result.code ? jdchat.switchMessage.showSysInfo(result.code, text_in_json.D) : 20 == result.code ? (checkLeave(), jdchat.switchMessage.showSysInfo(result.code)) : 21 == result.code || 22 == result.code ? (checkLeave(), jdchat.switchMessage.showSysInfo(result.code)) : 3 == result.code ? jdchat.switchMessage.showSysInfo(result.code) : 4 == result.code ? (common.systemInfo("您已与此商家建立了对话，请去原窗口继续咨询；如果原窗口刚刚被您关闭，请十分钟后再试！", "im-msg-prompt"), settings.cur_ajax_request && (settings.cur_ajax_request.abort(), settings.isPoll = !1)) : 10 == result.code ? jdchat.switchMessage.showSysInfo(result.code) : 11 == result.code ? (jdchat.switchMessage.showSysInfo(result.code), $("#shopTitle").html(settings.kf.nickName + '<font style="font-size:12px; font-weight:normal;">（离线）</font>'), settings.cur_ajax_request && (settings.cur_ajax_request.abort(), settings.isPoll = !1)) : 101 == result.code ? jdchat.switchMessage.showSysInfo(result.code) : 360 == result.code ? jdchat.switchMessage.showSysInfo(result.code) : 0 == result.code && (3 != jdchat.serverType ? $("#shopTitle").html(settings.kf.nickName + '<font style="font-size:12px; font-weight:normal;">（离线）</font>') : $("#shopTitle").html('京东客服<font style="font-size:12px; font-weight:normal;">（离线）</font>'), jdchat.switchMessage.showSysInfo(result.code))
                }
            }
        })
    }

    function extractStacktrace(e) {
        return e.stacktrace ? e.stacktrace : e.stack ? e.stack : e.message || e.description
    }

    function collectError(e) {
        var sendCount = settings.send_error_count++,
            start = (new Date).getTime(),
            browser = BrowserDetect.browser + ":" + BrowserDetect.version,
            curKf = settings.curKf ? settings.curKf.pin : "暂无客服",
            message = "curKf=" + curKf;
        message += ",sid=" + settings.sid, message += ",cid=" + settings.cid, message += ",currentStatus=" + statusManager.currentStatus, message += ",sendMsg=" + $("#text_in").html(), e && (message += ",errorInfo=" + extractStacktrace(e));
        var data = {
            message: encodeURI(message),
            browser: browser
        };
        jQuery.ajax({
            url: "/error/index.action?t=" + start,
            type: "post",
            data: data,
            timeout: 15e3,
            success: function (xml) {
                xml || 3 > sendCount && setTimeout(collectError, 3e3)
            },
            error: function () {
                3 > sendCount && setTimeout(collectError, 3e3)
            }
        })
    }

    function checkMsg() {
        var text_in_json = common.getTextIn();
        try {
            if (statusManager.currentStatus != statusManager.systemStatus.chating && statusManager.currentStatus != statusManager.systemStatus.message) return;
            if (null == text_in_json) return systemInfo("消息不能为空", "text_1_st"), $("#text_in").html(), $("#text_in").blur(), $("#text_in").focus(), void 0;
            var text_in = $.trim(text_in_json.D);
            text_in = $.fn.jdExpression.replaceImg(text_in);
            var tempDOM = $("<div></div>").html(text_in),
                img_len = 0,
                stop = !1;
            if (tempDOM.find("img").each(function () {
                    if (img_len++, "emotion" != $(this).attr("class") && "gallery" != $(this).parent().attr("rel") && "send-file" != $(this).parent().attr("rel")) {
                        var src = "";
                        try {
                            src = $(this).attr("src")
                        } catch (e) {
                            src = "", collectError(e)
                        }
                        if (src && 0 == src.indexOf("file://")) return systemInfo("本地文件无法读取", "text_1_st"), $(this).remove(), void 0;
                        if (src && src.length > 360) return systemInfo("请使用贴图功能将图片上传后，再发送"), stop = !0, void 0;
                        var img = "";
                        try {
                            img = '<a rel="gallery" class="image-file" href="' + src + '"><img class="message-img" src="' + src + '"></a>'
                        } catch (e) {
                            img = "", collectError(e)
                        }
                        $(this).replaceWith(img)
                    }
                }), stop) return;
            var len_limit = !1,
                msg_len = 0;
            tempDOM.find("a[rel='send-file']").each(function () {
                var s = $(this).wrap("<span></span>").parent().html();
                return $(this).parent().replaceWith(s), s.length > 360 ? (systemInfo("文件名超长,请修改后再发送", "text_1_st"), len_limit = !0, void 0) : void 0
            });
            var img_text_len = 0;
            tempDOM.find("a[class='image-file']").each(function () {
                var s = $(this).wrap("<span></span>").parent().html();
                return $(this).parent().replaceWith(s), s.length > 360 ? (len_limit = !0, systemInfo("图片名超长,请修改后再发送", "text_1_st"), void 0) : (img_text_len += $(this).text().length, void 0)
            });
            var emtion = 0;
            tempDOM.find("img").each(function () {
                "emotion" == $(this).attr("class") && (emtion += 6);
                var s = $(this).wrap("<span></span>").parent().html();
                return $(this).parent().replaceWith(s), s.length > 360 ? (len_limit = !0, systemInfo("包含的图片链接长度太长,请删除或更换", "text_1_st"), void 0) : (msg_len++, void 0)
            });
            var text_len = $.trim(tempDOM.text()).length - img_text_len + emtion;
            if (msg_len += 0 == text_len % 360 ? text_len / 360 : text_len / 360 + 1, msg_len > 5) return systemInfo("发送的内容过长，请缩减后重新发送", "text_1_st"), void 0;
            if (len_limit) return;
            var st = 0;
            tempDOM.find("a[rel='send-file']").each(function () {
                var s = $(this).wrap("<span></span>").parent().html();
                text_in_json.D = s, sendMsg(text_in_json), $(this).parent().remove(), st = 1
            }), tempDOM.find("a.image-file").each(function () {
                if ("emotion" != $(this).attr("class") && "gallery" == $(this).attr("rel") && 0 != $(this).find("img.message-img").length) {
                    var s = $(this).wrap("<span></span>").parent().html();
                    text_in_json.D = s, sendMsg(text_in_json), $(this).parent().remove(), st = 1
                }
            }), tempDOM.find("img").each(function () {
                var s = $(this).wrap("<span></span>").parent().html();
                text_in_json.D = s, sendMsg(text_in_json), $(this).parent().remove(), st = 1
            });
            var text = $.trim(tempDOM.text());
            if (text = $.trim(text.replace(/&nbsp;/gim, "")), "" == text) return st || systemInfo("空消息客服无法处理", "text_1_st"), $("#text_in").html(""), void 0;
            if (text_len = text.length, 360 >= text_len && text_len > 0) text_in_json.D = text.replace(/</g, "&lt;").replace(/>/g, "&gt;"), sendMsg(text_in_json);
            else for (var len = 0 == text_len % 360 ? text_len / 360 : text_len / 360 + 1, sn = 0, loop = 0; text.length > sn && len - 1 > loop;) {
                loop++;
                var s = $.trim(text.substring(sn, sn + 360));
                if (360 == s.length) {
                    var st = s.substring(s.length - 5);
                    st.indexOf("#") >= 0 && (s = s.substring(0, s.length - 5))
                }
                text_in_json.D = s, sn += s.length, "" != s && sendMsg(text_in_json)
            }
            $("#text_in").html(""), window.chrome && $("#text_in").html("&nbsp;"), $("#text_in").blur(), $("#text_in").focus(), $("#chat_count").html('还可以输入<span class="im-word-num">360</span>字')
        } catch (e) {
            sendMsg(text_in_json), collectError(e)
        }
    }

    function clearMany() {
        common.clearMany()
    }

    function getLeaveMsg() {
        var data = {},
            lastwaiter = "";
        settings.lastwaiter && (lastwaiter = encodeURIComponent(settings.lastwaiter)), data.leaveWaiter = lastwaiter, data.venderId = settings.venderId, settings.wid && "" != settings.wid && (data.wid = settings.wid);
        var top = null;
        jQuery.ajax({
            url: "venderLeaveMsg.action",
            data: data,
            success: function (json) {
                json = eval("(" + json + ")");
                var show = !0;
                if (1 == json.code) {
                    var msg = {};
                    json.venderLeaveMsg ? msg = eval("(" + json.venderLeaveMsg + ")") : show = !1, 0 == msg.length && (show = !1), show && (top = $(".im-msg-notice"), systemInfo("以下为离线咨询", "text_1_s"));
                    for (var i = 0; msg.length > i && show; i++) {
                        var m = msg[i];
                        common.showMsg(m.content, m.servicesend ? m.serviceId : m.customer, m.created, !1, m.servicesend)
                    }
                    null != top && top.remove().appendTo("#scrollDiv")
                } else show = !1;
                show || common.showSystemMessage("很抱歉，暂未查询到相关留言记录", "", "append")
            },
            error: function () {
                common.showSystemMessage("很抱歉，暂未查询到相关留言记录", "", "append")
            }
        })
    }

    function readNotice() {
        var path = window.location.pathname,
            seq = -1;
        if (0 == path.indexOf("/pop") ? seq = 1 : 0 == path.indexOf("/agile") ? seq = 7 : 0 == path.indexOf("/sup") ? seq = 2 : 0 == path.indexOf("/jdchat") && (seq = 3), -1 == seq) {
            var search = window.location.search;
            if (search.indexOf("shopId") > 0 || search.indexOf("venderId") > 0) seq = 1;
            else if (search.indexOf("groupId") > 0) seq = 7;
            else if (search.indexOf("pid") > 0) {
                var pid = search.substring(search.indexOf("pid"));
                pid.indexOf("&") > 0 && (pid = pid.substring(0, pid.indexOf("&"))), pid = pid.split("=")[1], seq = pid.length >= 10 ? 1 : 2
            }
        }
        seq > 0 && $.ajax({
            url: "/notice/getNotice.action?t=" + (new Date).getTime() + "&seq=" + seq,
            type: "GET",
            dataType: "html",
            async: !1,
            success: function (html) {
                html && "" != $.trim(html) && common.systemInfo(html, "im-msg-notice")
            },
            error: function () {
            }
        })
    }

    function initService() {
        if (riskCheck(settings.pamsUid, common), !canChat || !riskCheckFlag) return initTimer = setTimeout(initService, 500), void 0;
        initTimer && clearTimeout(initTimer), statusManager.currentStatus = statusManager.systemStatus.init, common.showSystemMessage("正在连接服务器.....", "connect-server", "one");
        var start = (new Date).getTime();
        jQuery.ajax({
            url: "init.action?t=" + (new Date).getTime(),
            type: "GET",
            dataType: "html",
            data: {
                pid: settings.pid,
                pamsUid: settings.pamsUid,
                groupId: settings.groupId
            },
            timeout: 3e4,
            error: function () {
                common.systemInfo("网络繁忙，请稍后再试<a href='#' onclick='location.reload(false);'> 请点击这里重试</a>", "im-msg-error")
            },
            success: function (xml) {
                var _name;
                if (null != xml && "" != xml) {
                    var result = $.jdtoolbox.string2json("(" + xml + ")");
                    if ($(".im-shop-tab .im-item").eq(0).trigger("click"), $(".im-product-pic").height($(".im-product").height() - 20), result.uid && (settings.uid = result.uid), result.systime) {
                        var rtp = (new Date).getTime() - start;
                        settings.systime = result.systime - 1e3 + rtp, common.clock()
                    }
                    if (0 == result.LOGIN_STATE) return 1 == result.code ? (statusManager.currentStatus = statusManager.systemStatus.chating, statusManager.statusDataMap[statusManager.currentStatus] = {
                        lineKey: result.lineKey,
                        waitNo: result.waitNo,
                        lineNo: result.lineNo
                    }) : -1 == result.code && (statusManager.currentStatus = statusManager.systemStatus.queue, statusManager.statusDataMap[statusManager.currentStatus] = {
                        lineKey: result.lineKey,
                        waitNo: result.waitNo,
                        lineNo: result.lineNo
                    }), login(), void 0;
                    if (1 == result.LOGIN_STATE && result.pin && (1 == result.code ? (statusManager.currentStatus = statusManager.systemStatus.chating, statusManager.statusDataMap[statusManager.currentStatus] = {
                            lineKey: result.lineKey,
                            waitNo: result.waitNo,
                            lineNo: result.lineNo
                        }) : -1 == result.code && (statusManager.currentStatus = statusManager.systemStatus.queue, statusManager.statusDataMap[statusManager.currentStatus] = {
                            lineKey: result.lineKey,
                            waitNo: result.waitNo,
                            lineNo: result.lineNo
                        }), $.startClient(result.pin, statusManager, settings, common)), result.error && "NotLogin" == result.error) return login(), void 0;
                    if (settings.downloadURL && "" != $.trim(settings.downloadURL) || (settings.downloadURL = "http://dongdong.jd.com"), topMsg("京东咚咚客户端上线啦，支持截图、查看聊天记录。&nbsp;&nbsp;<a href='" + settings.downloadURL + "' onclick=\"javascript:urlClick('" + "http://dongdong.jd.com','CHAT_OTHER');\"" + "  target='_blank'>猛戳下载</a>"), readNotice(), result.group) settings.groupId = result.group.id, 0 == result.group.allowLeaveMsg && (settings.allowLeaveMsg = !1), $("#degreeButton").hide(), $("div.im-edit-toolbar .im-icon-msg-record").hide(), $("#logoTitle").html("&nbsp;&nbsp;" + result.group.name + "在线客服"), $("<img id='logo' height='50px' width='180px' onerror='this.style.display=\"none\";'>").insertAfter("#shopTitle"), $("#logo").attr("src", result.group.logo), $(".im-shop-tab .im-item").eq(0).hide().end().eq(1).hide().end().eq(2).addClass("current"), $(".im-shop-tab .im-item").eq(2).trigger("click"), $(".im-shop-service .im-shop-tab").trigger("tabOnClickHandle", $(".im-shop-service .im-shop-tab li").eq(2)), $(".im-common-question").hide(), _name = result.group.name;
                    else if (result.brandName) $("#logoTitle").html("&nbsp;&nbsp;" + result.brandName + "品牌厂商在线客服"), settings.brandName = result.brandName, settings.classify = result.classify, $(".im-shop-tab .im-item :eq(1)").hide(), settings.pid ? $(".im-shop-tab .im-item").eq(0).trigger("click") : ($(".im-shop-tab .im-item :eq(0)").hide(), $(".im-shop-tab .im-item").eq(2).trigger("click"), $(".im-shop-service .im-shop-tab").trigger("tabOnClickHandle", $(".im-shop-service .im-shop-tab li").eq(2))), _name = result.brandName;
                    else if (result.shop) {
                        if (_name = result.shop.appName, result.shop.appName && $("#logoTitle").html("&nbsp;&nbsp;" + result.shop.appName + "在线客服"), result.shop.id && (settings.shopId = result.shop.id), result.shop.venderId && (settings.venderId = result.shop.venderId), result.shop.operatorId && (settings.operatorId = result.shop.operatorId), settings.pid ? $(".im-shop-tab .im-item").eq(0).trigger("click") : ($(".im-shop-tab .im-item :eq(0)").hide(), $(".im-shop-tab .im-item").eq(1).trigger("click"), $(".im-shop-service .im-shop-tab").trigger("tabOnClickHandle", $(".im-shop-service .im-shop-tab li").eq(1))), result.jf && $(".jf").show(), result.shop.popWelcome) {
                            var json = eval("(" + result.shop.popWelcome + ")");
                            if (json.enable) for (var i = 0; json.profile.length > i; i++) {
                                var p = json.profile[i];
                                p.enable && (profile = p.content)
                            }
                        }
                    } else $(".im-shop-tab .im-item").eq(0).hide().end().eq(1).hide().end().eq(2).addClass("current"), $(".im-shop-tab .im-item").eq(2).trigger("click"), $(".im-shop-service .im-shop-tab").trigger("tabOnClickHandle", $(".im-shop-service .im-shop-tab li").eq(2)), $(".im-common-question").hide();
                    if ($(".im-question-list").tree(settings), 1 == settings.msgElves && getLeaveMsg(), 11 == result.code) systemInfo("客服忙，暂时不能给您提供服务!", "text_1_s"), $("#shopTitle").html(settings.kf.pin + '<font style="font-size:12px; font-weight:normal;">（离线）</font>');
                    else if (1 == result.code) statusManager.statusDataMap[statusManager.systemStatus.chating] = {
                        lineKey: result.lineKey,
                        waitNo: result.waitNo,
                        lineNo: result.lineNo
                    }, jQuery("#pin_span").html("" + result.pin), settings.pin = result.pin, settings.kf = result.account, null != settings.kf.nickName ? $("#shopTitle").html(settings.kf.nickName + '<font style="font-size:12px; font-weight:normal;">（在线）</font>') : 3 == jdchat.serverType ? $("#shopTitle").html('京东客服<font style="font-size:12px; font-weight:normal;">（在线）</font>') : $("#shopTitle").html(settings.kf.pin + '<font style="font-size:12px; font-weight:normal;">（在线）</font>'), $("#shopTitle").show(), creakLink(settings.kf);
                    else if (-1 === result.code) result.jf && $(".jf").show(), statusManager.currentStatus = statusManager.systemStatus.queue, statusManager.statusDataMap[statusManager.currentStatus] = {
                        lineKey: result.lineKey,
                        waitNo: result.waitNo,
                        lineNo: result.lineNo
                    }, common.showSystemMessage("很抱歉，当前客服繁忙请您耐心排队", "into-queue", "only"), common.getQueueData(result.lineKey, result.lineNo), jdchat.waitMessage.setContentFunc(function (waitTime) {
                        common.showSystemMessage("您已经等待了" + waitTime + "分钟,系统正在继续为您排队<br/>如果您无法再排队您可以: <a href='javascript:void(0);' class='message-out' >留言后离开</a>  <a href='javascript:void(0)' class='outgoing'>不留言直接离开</a>", "alert-queue", "only"), common.showSystemMessage("还在排队？使用京东咚咚客户端立即咨询，灰一般的感觉&nbsp;&nbsp;<a href='" + settings.downloadURL + "' onclick=\"javascript:urlClick('" + "http://dongdong.jd.com','CHAT_OTHER');\"" + "  target='_blank'>马上安装</a>", "dongdong-tip", "only"), $(".message-out").click(function () {
                            return statusManager.currentStatus = statusManager.systemStatus.message, jdchat.waitMessage.stop(), $("#scrollDiv").html(""), common.showSystemMessage("现在您可以留言了，最多可发送20条留言消息<br/>客服的回复将显示在“<a href='http://chat.jd.com/chatlog/chatlog.action' target='_blank'>我的京东/消息精灵</a>”中,请注意查看", "alert-messsage-2", "only"), jQuery("#sendMsg").unbind("click"), jQuery("#sendMsg").click(function () {
                                add()
                            }), creakLink(), !1
                        }), $(".outgoing").click(function () {
                            return checkLeave(), !1
                        })
                    }), jdchat.waitMessage.create(), common.ModuleQuestion(settings, _name);
                    else if (0 === result.code) systemInfo("非常抱歉，当前无客服在线，无法接待您！", "text_1_s");
                    else if (9 === result.code) {
                        null != result.brandName ? $("#logoTitle").html("&nbsp;&nbsp;" + result.brandName + "品牌厂商客服") : null != result.shop && result.shop.appName && $("#logoTitle").html("&nbsp;&nbsp;" + result.shop.appName + "客服"), statusManager.currentStatus = statusManager.systemStatus.nobody, statusManager.currentStatus = statusManager.systemStatus.message, jdchat.waitMessage.stop();
                        var msg = "当前客服不在线,如果有服务需要请留言,客服在下次登录时会接收到；最多可发送20条留言消息，客服的回复将显示在“<a href='http://chat.jd.com/chatlog/chatlog.action' target='_blank'>我的京东/消息精灵</a>”中,请注意查看 ";
                        result.lastLoginTime && (msg = msg + "</br></br> 客服最近一次登录时间是：" + result.lastLoginTime), common.showSystemMessage(msg, "alert-messsage-1", "only"), common.showSystemMessage("漏掉客服回复消息？使用京东咚咚客户端，消息一网打尽&nbsp;&nbsp;<a href='" + settings.downloadURL + "' onclick=\"javascript:urlClick('" + "http://dongdong.jd.com','CHAT_OTHER');\"" + "  target='_blank'>立即使用</a>", "alert-message-1", "only"), jQuery("#sendMsg").unbind("click"), jQuery("#sendMsg").click(function () {
                            add()
                        }), creakLink()
                    } else -100 === result.code ? systemInfo("系统提示：您的账号有安全风险，绑定手机号后继续咨询<a href='http://safe.jd.com/validate/verifyMobile' target='_blank'>立即绑定手机</a><br/>（如果已绑定，请30秒后重试）", "im-msg-prompt") : (systemInfo("当前客服繁忙，请您耐心等待！", "text_1_s"), $("#s_button li:eq(1)").click(), autoAllot())
                } else common.systemInfo("非常抱歉，当前无客服在线", "im-msg-prompt");
                0 == $(".jf:visible").length && (settings.donwloadURL || (settings.downloadURL = "http://dongdong.jd.com"), $(".jf").attr("onclick", "javascript:urlClick('http://dongdong.jd.com','CHAT_OTHER');"), $(".jf").attr("href", settings.downloadURL).html("下载京东咚咚客户端").show())
            }
        })
    }

    function topMsg(msg) {
        var width;
        $("<div id='chatTop'></div>").insertBefore("#scrollDiv"), $("#chatTop").css({
            padding: "10px",
            lineHeight: "17px",
            backgroundColor: "#fefcc6",
            borderBottom: "1px solid #e1e49a"
        }).html(msg).append("<span id='closetopmsg'>×</span>"), $("#scrollDiv").height($("#scrollDiv").height() - $("#chatTop").outerHeight()), document.documentMode && 8 > document.documentMode ? (width = $("#chatTop").width() - $(".im-tab-content").width(), $("#closetopmsg").css("marginLeft", width + "px")) : $("#closetopmsg").css("float", "right"), $("#closetopmsg").css({
            cursor: "pointer"
        }).click(function () {
            $("#scrollDiv").height($("#scrollDiv").height() + $("#chatTop").outerHeight()), $("#chatTop").remove()
        })
    }

    function creakLink(ckf) {
        jdchat.waitMessage.stop(), msgSendCount = 0;
        var data = {
            pid: settings.pid,
            sid: settings.sid,
            count: settings.count,
            pamsUid: settings.pamsUid,
            groupId: settings.groupId,
            orgId: settings.orgId
        };
        statusManager.currentStatus != statusManager.systemStatus.message && (data.receiver = encodeURI(ckf.pin)), jQuery.ajax({
            url: "link.action?t=" + (new Date).getTime(),
            type: "GET",
            data: data,
            timeout: 3e4,
            error: function () {
                common.systemInfo("网络繁忙，请稍后再试！<a href='#' onclick='location.reload(false);'> 点击这里重试</a>", "im-msg-error")
            },
            success: function (xml) {
                if (null != xml && "" != xml) {
                    var result = $.jdtoolbox.string2json("(" + xml + ")");
                    if (1 == result.code) {
                        if (result.account && (settings.customerAvatar = result.account.customerAvatar), statusManager.currentStatus != statusManager.systemStatus.message) {
                            switch (jdchat.serverType) {
                                case 1:
                                case 2:
                                    systemInfo("客服 " + ckf.nickName + " 已加入会话", "text_1_s"), settings.serviceNickName = ckf.nickName;
                                    break;
                                case 3:
                                    systemInfo("京东客服已加入会话", "text_1_s"), settings.serviceNickName = "京东客服";
                                    break;
                                default:
                                    systemInfo("客服 " + ckf.nickName + " 已加入会话", "text_1_s"), settings.serviceNickName = ckf.nickName
                            }
                            $("div [logictype='into-queue']").hide(), $("div [logictype='queue-progress']").hide(), $("div [logictype='alert-queue']").hide(), jQuery("#sendMsg").unbind("click"), jQuery("#sendMsg").click(function () {
                                add()
                            });
                            var t1 = setTimeout(function () {
                                    return settings.count > 0 ? (clearTimeout(t1), void 0) : (systemInfo("您长时间没发言，系统将在1分钟后断开连接", "text_1_s"), t1 && clearTimeout(t1), void 0)
                                }, 3e4),
                                t2 = setTimeout(function () {
                                    return settings.count > 0 ? (clearTimeout(t2), void 0) : (systemInfo("您在90秒内未发言，与客服连接已断开，点击<a href='#none'onclick='window.location.reload(true)'>这里</a>继续咨询", "text_1_s"), void 0)
                                }, 9e4);
                            if (msgSendCheckTimer) try {
                                clearInterval(msgSendCheckTimer)
                            } catch (e) {
                            }
                            if (msgSendCheckTimer = setInterval(function () {
                                    return settings.isPoll ? (msgSendCount > 0 ? msgSendCount = 0 : systemInfo("您已8分钟未发言，如果继续不发言2分钟后会自动断线，请您注意。", "text_1_s"), void 0) : (clearInterval(msgSendCheckTimer), void 0)
                                }, 48e4), settings.isChange) sendzjToOld(), settings.zj.curKf = ckf, settings.zj.sid = result.sid, settings.zj.cid = result.cid, settings.zj.receiver = ckf.pin, ckf.orgId && (settings.zj.orgId = ckf.orgId), statusManager.statusDataMap[statusManager.systemStatus.concat] = {
                                sid: result.sid,
                                cid: result.cid
                            }, statusManager.currentStatus = statusManager.systemStatus.concat, setTimeout(function () {
                                sendzj(result.account.waiterAvatar)
                            }, 2e3);
                            else {
                                settings.curKf = ckf, settings.sid = result.sid, settings.cid = result.cid, settings.receiver = ckf.pin, settings.customerAvatar = result.account.customerAvatar, settings.waiterAvatar = result.account.waiterAvatar, 1 != settings.msgElves && common.getHistory(profile), profile = null, statusManager.currentStatus = statusManager.systemStatus.chating;
                                var text_json = common.getTextJson("顾客 " + settings.pin + " 加入咨询");
                                sendMsg(text_json, !0)
                            }
                            settings.isClear = !1
                        }
                        statusManager.statusDataMap[statusManager.systemStatus.concat] = {
                            sid: result.sid,
                            cid: result.cid
                        }
                    } else systemInfo("当前客服繁忙，请您耐心等待！", "text_1_s"), settings.kf = null, autoAllot()
                } else common.systemInfo("网络繁忙，请稍后再试！<a href='#' onclick='location.reload(false)'>点击这里重试</a>", "im-msg-prompt")
            }
        })
    }

    function sendzjToOld() {
        var zjJson = common.getZjToOldJson(),
            start = (new Date).getTime();
        jQuery.ajax({
            url: "post.action?pamsUid=" + settings.pamsUid + "&t=" + start,
            type: "post",
            data: {
                pid: settings.pid,
                sid: settings.sid,
                cid: settings.cid,
                count: settings.count,
                receiver: encodeURI(settings.curKf.pin),
                message: encodeURI($.jdtoolbox.json2string(zjJson)),
                orgId: settings.orgId,
                uid: settings.uid,
                groupId: settings.groupId
            },
            timeout: 15e3,
            async: !1,
            error: function () {
                systemInfo("转接消息发送失败,请稍后再试!", "text_1_s")
            },
            success: function (xml) {
                if (null != xml && "" != xml) {
                    var result = $.jdtoolbox.string2json("(" + xml + ")");
                    if (result.systime) {
                        var rtp = (new Date).getTime() - start;
                        settings.systime = result.systime - 1e3 + rtp, common.clock()
                    }
                    1 == result.code ? (settings.isPoll || poll(), settings.count++) : 2 == result.code ? jdchat.switchMessage.showSysInfo(result.code) : 100 == result.code ? jdchat.switchMessage.showSysInfo(result.code) : 3 == result.code ? jdchat.switchMessage.showSysInfo(result.code) : 4 == result.code ? jdchat.switchMessage.showSysInfo(result.code) : 10 == result.code ? jdchat.switchMessage.showSysInfo(result.code) : 101 == result.code ? jdchat.switchMessage.showSysInfo(result.code) : 360 == result.code && jdchat.switchMessage.showSysInfo(result.code)
                }
                settings.newMsg = !1, $("#text_in").blur(), $("#text_in").focus()
            }
        })
    }

    function sendzj(waiterHeadImg) {
        var zjJson = common.getZjJson(),
            data = {
                pid: settings.pid,
                sid: settings.zj.sid,
                cid: settings.zj.cid,
                count: 0,
                receiver: encodeURI(settings.zj.receiver),
                message: encodeURI($.jdtoolbox.json2string(zjJson)),
                orgId: settings.orgId,
                uid: settings.linkuid
            },
            start = (new Date).getTime();
        jQuery.ajax({
            url: "post.action?pamsUid=" + settings.pamsUid + "&t=" + start,
            type: "post",
            data: data,
            timeout: 15e3,
            async: !1,
            error: function () {
                systemInfo("转接消息发送失败,请稍后再试!", "text_1_s")
            },
            success: function (xml) {
                if (null != xml && "" != xml) {
                    "undefined" != waiterHeadImg && (settings.waiterAvatar = waiterHeadImg, settings.waiterAvatarLoaded = !1);
                    var result = $.jdtoolbox.string2json("(" + xml + ")");
                    if (result.systime) {
                        var rtp = (new Date).getTime() - start;
                        settings.systime = result.systime - 1e3 + rtp, common.clock()
                    }
                    1 == result.code ? settings.isChange && setTimeout(function () {
                        jQuery.ajax({
                            url: "/close.action?t=" + (new Date).getTime() + "&sid=" + settings.sid + "&count=" + settings.count + "&resCode=" + settings.res_id + "&cid=" + settings.cid + "&pamsUid=" + settings.pamsUid + "&uid=" + settings.uid,
                            type: "get",
                            async: !1,
                            timeout: 3e3,
                            success: function () {
                                systemInfo("与前一个客服断开连接!")
                            }
                        }), settings.curKf = settings.zj.curKf, settings.sid = settings.zj.sid, settings.cid = settings.zj.cid, settings.receiver = settings.zj.receiver, settings.zj.orgId && (settings.orgId = settings.zj.orgId), settings.count = 0, msgSendCount = 0, settings.res_id = 0, settings.cur_ajax_request && (settings.cur_ajax_request.abort(), settings.isPoll = !1), settings.isChange = !1, settings.isDegree = !1, statusManager.currentStatus = statusManager.systemStatus.chating, null != settings.curKf.nickName ? 3 == jdchat.serverType ? systemInfo("客服转到京东客服") : systemInfo("客服转到" + settings.curKf.nickName) : 3 == jdchat.serverType ? systemInfo("客服转到京东客服") : systemInfo("客服转到" + settings.zj.accountPin), settings.isChange = !1, clearMany(), settings.count++, settings.isPoll || poll()
                    }, 5e3) : 2 == result.code ? jdchat.switchMessage.showSysInfo(result.code) : 100 == result.code ? jdchat.switchMessage.showSysInfo(result.code) : 3 == result.code ? jdchat.switchMessage.showSysInfo(result.code) : 4 == result.code ? common.systemInfo("您已与此商家建立了对话，请去原窗口继续咨询；如果原窗口刚刚被您关闭，请十分钟后再试！", "im-msg-prompt") : 10 == result.code ? jdchat.switchMessage.showSysInfo(result.code) : 101 == result.code ? jdchat.switchMessage.showSysInfo(result.code) : 360 == result.code && jdchat.switchMessage.showSysInfo(result.code)
                }
                settings.newMsg = !1, $("#text_in").blur(), $("#text_in").focus()
            }
        })
    }

    function login(callback) {
        jQuery.extend(jdModelCallCenter, {
            usefulComment: function () {
                jQuery.login({
                    modal: !0,
                    complete: function (result) {
                        result.IsAuthenticated && ($.startClient(result.Name, statusManager, settings, common), setTimeout(function () {
                            initService();
                            try {
                                callback && "function" == typeof callback && callback.call()
                            } catch (e) {
                            }
                        }, 1e3))
                    }
                })
            }
        }), jQuery.extend(jdModelCallCenter.settings, {
            object: this,
            fn: function () {
                jdModelCallCenter.usefulComment(this)
            }
        }), jdModelCallCenter.settings.fn()
    }

    function poll() {
        common.poll()
    }

    function parseMsg(msg) {
        common.parseMsg(msg)
    }

    function initV2Extend() {
        var preLoad = common.preLoad,
            loadFailed = common.loadFailed,
            fileQueued = common.fileQueued,
            fileQueueError = common.fileQueueError,
            fileDialogComplete = common.fileDialogComplete,
            uploadStart = common.uploadStart,
            uploadProgress = common.uploadProgress,
            uploadSuccess = common.uploadSuccess,
            uploadError = common.uploadError,
            uploadComplete = common.uploadComplete,
            queueComplete = common.queueComplete,
            sendImageSettings = {
                flash_url: jdchat.resourcesPath.getJSResource("plugin/SWFUpload_v250/demos/swfupload/swfupload.swf"),
                flash9_url: jdchat.resourcesPath.getJSResource("plugin/SWFUpload_v250/demos/swfupload/swfupload_fp9.swf"),
                upload_url: "/file/upload",
                post_params: {
                    fileType: "image",
                    JSESSIONID: JSessionId
                },
                file_size_limit: "2048 KB",
                file_types: "*.jpg;*.gif;*.png;*.bmp;*.jpeg",
                file_types_description: "不超过2M",
                file_upload_limit: 0,
                file_queue_limit: 0,
                custom_settings: {
                    progressTarget: "fsUploadProgress"
                },
                debug: !1,
                button_image_url: jdchat.resourcesPath.getSkinResource("send-image-buttons-20-20.png"),
                button_width: "30px",
                button_height: "30px",
                button_placeholder_id: "sendImageButton",
                button_text: "",
                button_text_style: ".theFont { font-size: 12; }",
                button_text_left_padding: 0,
                button_text_top_padding: 0,
                swfupload_preload_handler: preLoad,
                swfupload_load_failed_handler: loadFailed,
                file_queued_handler: fileQueued,
                file_queue_error_handler: fileQueueError,
                file_dialog_complete_handler: fileDialogComplete,
                upload_start_handler: uploadStart,
                upload_progress_handler: uploadProgress,
                upload_error_handler: uploadError,
                upload_success_handler: uploadSuccess,
                upload_complete_handler: uploadComplete,
                queue_complete_handler: queueComplete
            };
        new SWFUpload(sendImageSettings), sendImageSettings.post_params = {
            fileType: "file"
        }, sendImageSettings.file_types = "*.*", sendImageSettings.file_size_limit = "500 KB", sendImageSettings.file_types_description = "不超过500KB", sendImageSettings.button_image_url = jdchat.resourcesPath.getSkinResource("send-file - buttons.png"), sendImageSettings.button_placeholder_id = "sendFileButton", $(".im-icon-file").remove()
    }

    function autoSizeChatWindow(width, height) {
        if (isChatMaxSize) {
            if (baseClientHeight >= $(window).height() || baseClientWidth >= $(window).width()) return $("body").css("overflow", "auto"), void 0;
            $("body").css("overflow", "hidden")
        } else if (width && height) {
            if (baseClientHeight > height || baseClientWidth > width) return $("body").css("overflow", "auto"), void 0;
            $("body").css("overflow", "hidden")
        }
        if (width && height) {
            var marginTop = ($(window).innerHeight() - height) / 2,
                left = ($(window).width() - width) / 2;
            0 > marginTop && (marginTop = 0), 0 > left && (left = 0), $(".im-wrap").height(height), $(".im-wrap").width(width), $(".im-wrap").css({
                marginTop: marginTop,
                marginBottom: marginTop,
                left: left
            }), $(".im-main-content").height(height - $(".im-header").innerHeight() - 22), $(".im-right-sidebar").height(height - $(".im-header").innerHeight() - 22), $(".im-chat-window").height(height - $(".im-header").innerHeight() - $(".im-edit-area").innerHeight()), $("#scrollDiv").height(height - $(".im-header").innerHeight() - $(".im-edit-area").innerHeight() - 22), $("#chatTop").length > 0 && $("#scrollDiv").height($("#scrollDiv").height() - $("#chatTop").outerHeight()), $(".im-tab-contents").height($(".im-right-sidebar").innerHeight() - $(".im-tab ").innerHeight()), $(".im-common-question").height($(".im-edit-area").height()), $(".im-common-question").parent().find(".im-product-info").css("bottom", $(".im-edit-area").height()), $(".im-question-list").height($(".im-common-question").height() - $(".im-title").height())
        } else $(".im-wrap").css({
            marginTop: 0,
            marginBottom: 0,
            left: 0
        }), $(".im-wrap").width($(window).width()), $(".im-wrap").height($(window).innerHeight()), $(".im-main-content").height($(window).innerHeight() - $(".im-header").innerHeight()), $(".im-right-sidebar").height($(window).innerHeight() - $(".im-header").innerHeight() - 22), $(".im-chat-window").height($(window).height() - $(".im-header").innerHeight() - 22), $("#scrollDiv").height($(window).height() - $(".im-header").innerHeight() - $(".im-edit-area").innerHeight() - 22), $("#chatTop").length > 0 && $("#scrollDiv").height($("#scrollDiv").height() - $("#chatTop").outerHeight()), $(".im-tab-contents").height($(".im-right-sidebar").innerHeight() - $(".im-tab ").innerHeight()), $(".im-common-question").height($(".im-edit-area").height()), $(".im-common-question").parent().find(".im-product-info").css("bottom", $(".im-edit-area").height()), $(".im-question-list").height($(".im-common-question").height() - $(".im-title").height());
        $(".dd_tip_mask_v3").css("height", "100%"), $(".dd_tip_mask_v3").css("width", "100%"), $(".im-wrap").show(), common && common.messageImgResize()
    }

    function shopInfo(target) {
        var loading = null;
        return loading = $(".im-loading", $("#j_shopInfo").parent()).length > 0 ? $(".im-loading", $("#j_shopInfo").parent()) : $('<div class="im-loading"><img src="/img/loading.gif" alt="正在加载中"></div>').appendTo($("#j_shopInfo").parent()), settings.venderId ? ($.ajax({
            url: "/vender/shopInfo.action?venderId=" + settings.venderId || "",
            type: "GET",
            success: function (xml) {
                try {
                    var shopInfo = eval("(" + xml + ")");
                    $(target).attr("active", !0), xml ? ($("#j_shopInfo").show(), $("#j_noShopInfo").hide(), renderShopInfo(shopInfo)) : ($("#j_noShopInfo").show(), $("#j_shopInfo").hide())
                } catch (e) {
                    $("#j_noShopInfo").show(), $("#j_shopInfo").hide()
                }
                loading.remove()
            },
            error: function () {
                $("#j_noShopInfo").show(), $("#j_shopInfo").hide(), loading.remove()
            }
        }), void 0) : ($("#j_noShopInfo").show(), $("#j_shopInfo").hide(), loading.remove(), void 0)
    }

    function renderShopInfo(data) {
        if (data) {
            var link = "";
            $("#venderName").html(data.venderName).click(function () {
                settings.shopId && $(this).attr("href", "http://mall.jd.com/index-" + settings.shopId + ".html").attr("target", "_blank")
            }), settings.shopId && (link = "http://mall.jd.com/shopLevel-" + settings.shopId + ".html"), data.venderPhone && $("#venderPhone").html(data.venderPhone).parent().show();
            var complexScore = Math.floor(data.complexScore);
            if (complexScore > 10) complexScore = 10;
            else {
                if (1 > complexScore) return $("#complexScore").removeClass("im-grade").html("卖家暂未收到任何评价"), $(".im-shop-info-table").hide(), void 0;
                4 > score && (score = 4)
            }
            link = "" != link ? "<a href='" + link + "' title='" + data.complexScore + "分' target='_blank' >" + toFixed(data.complexScore, 1) + "</a>" : toFixed(data.complexScore, 1), $("#complexScore").attr("title", data.complexScore + "分").children("em").addClass("g" + complexScore).parent().siblings().html(link), $("#productScore").attr("title", data.productScore + "分").html(toFixed(data.productScore)), $("#serviceScore").attr("title", data.serviceScore + "分").html(toFixed(data.serviceScore)), $("#timeScore").attr("title", data.timeScore + "分").html(toFixed(data.timeScore));
            var productRatio = checkRatio(data.productRatio);
            $("#productRatio").find("span").html(productRatio.score).addClass(productRatio.status);
            var serviceRatio = checkRatio(data.serviceRatio);
            $("#serviceRatio").find("span").html(serviceRatio.score).addClass(serviceRatio.status);
            var timeRatio = checkRatio(data.timeRatio);
            $("#timeRatio").find("span").html(timeRatio.score).addClass(timeRatio.status)
        }
    }

    function toFixed(score, bit) {
        var num = Number(score);
        if (isNaN(num) || 0 == num) return "--";
        var a = 100;
        return bit && (a = 10), num = Math.floor(num * a) / a, num + "分"
    }

    function checkRatio(score) {
        var status = "im-txt-rise";
        return status = 0 > Number(score) ? "im-txt-drop" : Number(score) > 0 ? "im-txt-rise" : "", status ? (score = Math.floor(1e4 * Math.abs(score)) / 100 + "%", score = ("im-txt-rise" == status ? "&uarr;" : "&darr;") + score) : score = "--", {
            score: score,
            status: status
        }
    }

    function recommend(target) {
        $.ajax({
            url: "/product/recommend.action",
            type: "GET",
            data: {
                venderId: settings.venderId
            },
            success: function (xml) {
                if (xml) {
                    $(".im-recommend-list").html("");
                    var returnInfo = eval("(" + xml + ")");
                    $(target).attr("active", !0);
                    var Ln = returnInfo.length;
                    if (0 == Ln) $(".no-recommend").show(), $(".im-recommend-slide").hide();
                    else {
                        $(".no-recommend").hide(), $(".im-recommend-slide").show();
                        for (var i = 0; Ln > i; i++) $("#jCarouselLite ul").append(buildRecommendItem(returnInfo[i]));
                        var circular = $("#jCarouselLite ul").children().length > 2;
                        $("#jCarouselLite").jCarouselLite({
                            btnPrev: "#prev",
                            btnNext: "#next",
                            visible: 2,
                            vertical: !1,
                            circular: circular,
                            speed: 200
                        })
                    }
                } else $(".no-recommend").show(), $(".im-recommend-slide").hide()
            },
            error: function () {
                $(".no-recommend").show(), $(".im-recommend-slide").hide()
            }
        })
    }

    function buildRecommendItem(data) {
        var _itemArray = ['<li class="im-item">', '<a class="im-product-pic" href="#">', '	<img src="img/1.jpg" title="艾米 AMII时尚文艺气钩花双层单件毛线衣">', "</a>", '<p class="im-product-title"><a class="im-txt-link" href="#" title="艾米 AMII时尚文艺气钩花双层单件毛线衣">艾米 AMII时尚文艺气钩花双层单件毛线衣</a></p>', "</li>"],
            _item = $(_itemArray.join(""));
        return _item.data("openUri", data.link), _item.bind("click", function () {
            window.open($(this).data("openUri"))
        }), _item.find("img").attr({
            src: data.imgUrl,
            title: data.name
        }), _item.find(".im-product-title a").attr({
            title: data.name
        }).html(data.name), _item
    }

    var settings = {
            cur_ajax_request: null,
            send_error_count: 0
        },
        common = null,
        statusManager = null,
        delUpload = [],
        profile = null,
        chatBllOrder = new ChatBllOrder,
        msgSendCount = 0,
        msgSendCheckTimer, errorMessages = {
            INVALID_PARAM: {
                code: 100,
                description: "错误的参数"
            },
            OPENREG_ERR: {
                code: 101,
                description: "打开注册表失败"
            },
            READREG_ERR: {
                code: 102,
                description: "读取注册表失败"
            },
            CHECKFILE_NOT_EXIT: {
                code: 103,
                description: "检查EXE文件不存在"
            },
            PLUGIN_UNKNOW_ERR: {
                code: 104,
                description: "未知错误"
            },
            PLUGIN_INVOKE_EXCEPTION: {
                code: -1,
                description: "插件调用异常"
            }
        };
    $.fn.jdchat = function (options) {
        return settings = $.extend(settings, $.fn.jdchat.defaults, options),
            settings.domain = window.location.hostname,
            common = jdchat.common(settings, creakLink, null, null),
            statusManager = jdchat.statusManager,
            settings.downloadURL = "http://dongdong.jd.com",
            settings.errorMsgManager = errorMessages,
            $this = this,
            $("#shopLogo").attr("href", "http://dongdong.jd.com").children("img").attr("title", "京东咚咚"),
            $(".im-shop-service .im-shop-tab").Tab(),
            $(".im-shop-service .im-shop-tab").bind("tabOnClickHandle", function (evt, target) {
                var index = $(target).index(),
                    len = $(".im-service .im-tab").children(".im-item").length;
                return 1 == len ? (chatBllOrder.getOrder(target), void 0) : 2 == len ? (0 == index ? chatBllOrder.getConsultOrder(target) : chatBllOrder.getOrder(target), void 0) : 3 == len ? (0 == index || (1 == index ? chatBllOrder.getOrder(target) : 2 == index && chatBllOrder.getRepairs(target)), void 0) : 4 == len ? (0 == index ? chatBllOrder.getConsultOrder(target) : 1 == index || (2 == index ? chatBllOrder.getOrder(target) : 3 == index && chatBllOrder.getRepairs(target)), void 0) : void 0
            }), initExecute(), initV2Extend(), common.setFsUploadProgress(), this
    }, $.fn.jdchat.defaults = {
        cid: "",
        sid: "",
        pin: "",
        groupId: "",
        allowLeaveMsg: !0,
        count: 0,
        receiver: "",
        path: "/",
        domain: "chat.jd.com",
        expires: 365,
        isPoll: !1,
        isDegree: !1,
        newMsg: !1,
        isChange: !1,
        isClear: !0,
        res_id: 0,
        fontType: [{
            name: "宋体",
            value: "宋体",
            code: 0
        }, {
            name: "黑体",
            value: "黑体",
            code: 1
        }, {
            name: "仿宋",
            value: "仿宋",
            code: 2
        }, {
            name: "楷体",
            value: "楷体",
            code: 3
        }, {
            name: "隶书",
            value: "隶书",
            code: 4
        }, {
            name: "幼圆",
            value: "幼圆",
            code: 5
        }],
        fontSize: [{
            name: "14",
            value: "14pt",
            code: 14
        }, {
            name: "16",
            value: "16pt",
            code: 16
        }, {
            name: "18",
            value: "18pt",
            code: 18
        }, {
            name: "20",
            value: "20pt",
            code: 20
        }, {
            name: "24",
            value: "24pt",
            code: 24
        }, {
            name: "28",
            value: "28pt",
            code: 28
        }, {
            name: "32",
            value: "32pt",
            code: 32
        }, {
            name: "36",
            value: "36pt",
            code: 36
        }, {
            name: "40",
            value: "40pt",
            code: 40
        }, {
            name: "42",
            value: "42pt",
            code: 42
        }, {
            name: "46",
            value: "46pt",
            code: 46
        }]
    };
    var lastsend = null,
        reduceTop = $("#scrollDiv").offset().top,
        reduceBottom = $("#service").height() - reduceTop - $("#scrollDiv").height(),
        reduceHeight = reduceTop + reduceBottom,
        baseChatWidth = 966,
        baseWinWidth = 1366,
        baseChatHeight = 587,
        baseWinHeight = 667,
        baseClientWidth = 884,
        baseClientHeight = 484,
        isChatMaxSize = !0,
        defaultChatWidth = $(window).innerWidth() == baseWinWidth ? baseChatWidth : Math.round(baseChatWidth * $(window).innerWidth()) / baseWinWidth,
        defaultChatHeight = $(window).innerWidth() == baseWinWidth ? baseChatHeight : Math.round(baseChatHeight * $(window).innerHeight()) / baseWinHeight;
    $("#j_chatRestore").show(), $("#j_chatMax").hide(), autoSizeChatWindow(), $("#j_chatMax").click(function (ev) {
        return isChatMaxSize ? !1 : (isChatMaxSize = !0, $(ev.target).hide(), $("#j_chatRestore").show(), autoSizeChatWindow(), void 0)
    }), $("#j_chatRestore").click(function (ev) {
        return isChatMaxSize ? (isChatMaxSize = !1, $(ev.target).hide(), $("#j_chatMax").show(), autoSizeChatWindow(defaultChatWidth, defaultChatHeight), void 0) : !1
    }), $("#j_chatClose").click(function () {
        return $("#talk_c").trigger("click"), !1
    }), $(window).resize(function () {
        isChatMaxSize ? autoSizeChatWindow() : (defaultChatWidth = $(window).innerWidth() == baseWinWidth ? baseChatWidth : Math.round(baseChatWidth * $(window).innerWidth()) / baseWinWidth, defaultChatHeight = $(window).innerWidth() == baseWinWidth ? baseChatHeight : Math.round(baseChatHeight * $(window).innerHeight()) / baseWinHeight, autoSizeChatWindow(defaultChatWidth, defaultChatHeight))
    }), $(window).bind("jquery.lightboxClicked", function () {
        var $input = $("#text_in");
        $input.attr("contentEditable", !1)
    }), $(window).bind("jquery.lightboxClosed", function () {
        var $input = $("#text_in");
        $input.attr("contentEditable", !0), $input.focus()
    }), $("#j_font").bind("click", function () {
        $("#colorButton").removeClass("im-icon-color-hover"), $(".im-pop-color").hide()
    })
})(jQuery), function ($) {
    function init(_this) {
        $(_this).hasClass("im-pop-face") || $(_this).addClass("im-pop-face"), $(_this).attr("style", "display:none");
        var def = settings.expression,
            tab = "<div class='im-face-tab'>",
            showExp = "";
        for (var exp in def) def[exp].items && def[exp].items.length > 0 && def[exp].show && (create(exp, _this), bindEvent(), tab += "<a href='javascript:void(0)' class='im-face-switch-tab' type='" + exp + "'>" + def[exp].name + "</a>", def[exp].defaultShow && (showExp = exp));
        tab += "</div>", $(_this).append("<div id='__view_div__' style='display:none;'></div>"), $(_this).append(tab), $(".im-face-tab .im-face-switch-tab").click(function () {
            var type = $(this).attr("type");
            return $("table.im-table-face").hide(), $("table.im-table-face[type='" + type + "']").show(), $(".im-face-tab .im-face-switch-tab").removeClass("current"), $(this).addClass("current"), !1
        }), $(".im-face-switch-tab[type='" + showExp + "']").click()
    }

    function create(type, _this) {
        for (var exp = settings.expression[type], array = exp.items, html = '<table class="im-table-face" cellspacing="0" cellpadding="0" type="' + type + '">', i = 0, len = array.length; len > i; i++) {
            0 == i % exp.lineSize && (html += 0 == i ? "<tr>" : "</tr><tr>");
            var p = exp.getPosition(i);
            html += "<td><span  id='" + array[i].id + "' class='" + exp.style + "' style='background-position:" + p + "'></span></td>"
        }
        0 != array.length % exp.lineSize && (html += "</tr>"), html += "</table>", $(_this).append(html)
    }

    function bindEvent() {
        $(".im-table-face td").unbind("click").bind("click", function (e) {
            return settings.imgClick(e), !1
        }).unbind("mouseover").bind("mouseover", function (e) {
            var target = $(e.target),
                obj = getExpression(target.attr("id")),
                expression = null,
                index = -1;
            return target ? (null != obj && (expression = obj.expression, index = obj.index), null == expression || 0 > index ? !1 : (position = expression.getViewPosition(index), $("#__view_div__").attr("class", "").addClass(expression.viewStyle).show().html(getImg(target.attr("id"))).css("top", position.top).css("left", position.left), !1)) : !1
        }).unbind("mouseout").bind("mouseout", function () {
            return $("#__view_div__").hide(), !1
        })
    }

    function getImg(id) {
        for (var exp in settings.expression) {
            var obj = settings.expression[exp];
            if (obj.items && obj.items.length > 0) for (var i = 0; obj.items.length > i; i++) if (obj.items[i].id == id) {
                var src = obj.path + obj.items[i].img;
                return '<img src="' + src + '"> '
            }
        }
        return ""
    }

    function getExpression(id) {
        for (var exp in settings.expression) {
            var obj = settings.expression[exp];
            if (obj.items && obj.items.length > 0) for (var i = 0; obj.items.length > i; i++) if (obj.items[i].id == id) return {
                expression: obj,
                index: i
            }
        }
        return null
    }

    function getImgByObject(expression, obj) {
        var src = expression.path + obj.img;
        return '<img src="' + src + '"> '
    }

    var settings;
    $.fn.jdExpression = function (options) {
        settings = $.extend({}, $.fn.jdExpression.defaults, options), init(this)
    }, $.fn.jdExpression.replaceName = function (string) {
        for (var type in settings.expression) for (var expressionArray = settings.expression[type].items, i = 0; expressionArray.length > i; i++) {
            var img = expressionArray[i].name,
                src = getImgByObject(settings.expression[type], expressionArray[i]),
                reg = RegExp(img.replace(/(^\s*)|(\s*$)/g, ""), "gim");
            string = string.replace(reg, src)
        }
        return string
    }, $.fn.jdExpression.replaceImg = function (string) {
        for (var type in settings.expression) for (var expressionArray = settings.expression[type].items, i = 0; expressionArray.length > i; i++) {
            var img = getImgByObject(settings.expression[type], expressionArray[i]);
            img = img.replace(/(^\s*)|(\s*$)/g, "");
            var reg = RegExp(img, "gim");
            string = string.replace(reg, expressionArray[i].name)
        }
        return string
    }, $.fn.jdExpression.defaults = {
        expression: {
            classic: {
                items: [{
                    id: "_aixin_",
                    name: "#E-爱心",
                    img: "s01.gif"
                }, {
                    id: "_anwei_",
                    name: "#E-安慰",
                    img: "s02.gif"
                }, {
                    id: "_bishi_",
                    name: "#E-鄙视",
                    img: "s03.gif"
                }, {
                    id: "_daku_",
                    name: "#E-大哭",
                    img: "s04.gif"
                }, {
                    id: "_deyi_",
                    name: "#E-得意",
                    img: "s05.gif"
                }, {
                    id: "_dangao_",
                    name: "#E-蛋糕",
                    img: "s06.gif"
                }, {
                    id: "_feiwen_",
                    name: "#E-飞吻",
                    img: "s07.gif"
                }, {
                    id: "_fennu_",
                    name: "#E-愤怒",
                    img: "s08.gif"
                }, {
                    id: "_guzhang_",
                    name: "#E-鼓掌",
                    img: "s09.gif"
                }, {
                    id: "_guilian_",
                    name: "#E-鬼脸",
                    img: "s10.gif"
                }, {
                    id: "_haixiu_",
                    name: "#E-害羞",
                    img: "s11.gif"
                }, {
                    id: "_liuhan_",
                    name: "#E-流汗",
                    img: "s12.gif"
                }, {
                    id: "_heixian_",
                    name: "#E-黑线",
                    img: "s13.gif"
                }, {
                    id: "_aoman_",
                    name: "#E-傲慢",
                    img: "s14.gif"
                }, {
                    id: "_jianxiao_",
                    name: "#E-奸笑",
                    img: "s15.gif"
                }, {
                    id: "_jingya_",
                    name: "#E-惊讶",
                    img: "s16.gif"
                }, {
                    id: "_kelian_",
                    name: "#E-可怜",
                    img: "s17.gif"
                }, {
                    id: "_kuku_",
                    name: "#E-酷酷",
                    img: "s18.gif"
                }, {
                    id: "_liwu_",
                    name: "#E-礼物",
                    img: "s19.gif"
                }, {
                    id: "_hanxiao_",
                    name: "#E-憨笑",
                    img: "s20.gif"
                }, {
                    id: "_huaduo_",
                    name: "#E-花朵",
                    img: "s21.gif"
                }, {
                    id: "_xiangwen_",
                    name: "#E-香吻",
                    img: "s22.gif"
                }, {
                    id: "_sese_",
                    name: "#E-色色",
                    img: "s23.gif"
                }, {
                    id: "_shengbing_",
                    name: "#E-生病",
                    img: "s24.gif"
                }, {
                    id: "_shuaiyang_",
                    name: "#E-衰样",
                    img: "s25.gif"
                }, {
                    id: "_keshui_",
                    name: "#E-瞌睡",
                    img: "s26.gif"
                }, {
                    id: "_tanqi_",
                    name: "#E-叹气",
                    img: "s27.gif"
                }, {
                    id: "_touxiao_",
                    name: "#E-偷笑",
                    img: "s28.gif"
                }, {
                    id: "_outu_",
                    name: "#E-呕吐",
                    img: "s29.gif"
                }, {
                    id: "_tiaopi_",
                    name: "#E-调皮",
                    img: "s30.gif"
                }, {
                    id: "_weixiao_",
                    name: "#E-微笑",
                    img: "s31.gif"
                }, {
                    id: "_beishang_",
                    name: "#E-悲伤",
                    img: "s32.gif"
                }, {
                    id: "_woshou_",
                    name: "#E-握手",
                    img: "s33.gif"
                }, {
                    id: "_wenhao_",
                    name: "#E-问号",
                    img: "s34.gif"
                }, {
                    id: "_yinxian_",
                    name: "#E-阴险",
                    img: "s35.gif"
                }, {
                    id: "_yongbao_",
                    name: "#E-拥抱",
                    img: "s36.gif"
                }, {
                    id: "_xuanyun_",
                    name: "#E-眩晕",
                    img: "s37.gif"
                }, {
                    id: "_baibai_",
                    name: "#E-拜拜",
                    img: "s38.gif"
                }, {
                    id: "_henbang_",
                    name: "#E-很棒",
                    img: "s39.gif"
                }, {
                    id: "_zhuakuang_",
                    name: "#E-抓狂",
                    img: "s40.gif"
                }, {
                    id: "_baiyan_",
                    name: "#E-白眼",
                    img: "s41.gif"
                }, {
                    id: "_bizui_",
                    name: "#E-闭嘴",
                    img: "s42.gif"
                }, {
                    id: "_dabing_",
                    name: "#E-大兵",
                    img: "s43.gif"
                }, {
                    id: "_dengpao_",
                    name: "#E-灯泡",
                    img: "s44.gif"
                }, {
                    id: "_dianhua_",
                    name: "#E-电话",
                    img: "s45.gif"
                }, {
                    id: "_fadai_",
                    name: "#E-发呆",
                    img: "s46.gif"
                }, {
                    id: "_fankun_",
                    name: "#E-犯困",
                    img: "s47.gif"
                }, {
                    id: "_feiniao_",
                    name: "#E-飞鸟",
                    img: "s48.gif"
                }, {
                    id: "_fendou_",
                    name: "#E-奋斗",
                    img: "s49.gif"
                }, {
                    id: "_fengkuang_",
                    name: "#E-疯狂",
                    img: "s50.gif"
                }, {
                    id: "_ganga_",
                    name: "#E-尴尬",
                    img: "s51.gif"
                }, {
                    id: "_gaoxing_",
                    name: "#E-高兴",
                    img: "s52.gif"
                }, {
                    id: "_jida_",
                    name: "#E-击打",
                    img: "s53.gif"
                }, {
                    id: "_jie_",
                    name: "#E-饥饿",
                    img: "s54.gif"
                }, {
                    id: "_jingkong_",
                    name: "#E-惊恐",
                    img: "s55.gif"
                }, {
                    id: "_kafei_",
                    name: "#E-咖啡",
                    img: "s56.gif"
                }, {
                    id: "_keai_",
                    name: "#E-可爱",
                    img: "s57.gif"
                }, {
                    id: "_kele_",
                    name: "#E-可乐",
                    img: "s58.gif"
                }, {
                    id: "_kouzhao_",
                    name: "#E-口罩",
                    img: "s59.gif"
                }, {
                    id: "_kulou_",
                    name: "#E-骷髅",
                    img: "s60.gif"
                }, {
                    id: "_liulei_",
                    name: "#E-流泪",
                    img: "s61.gif"
                }, {
                    id: "_mifan_",
                    name: "#E-米饭",
                    img: "s62.gif"
                }, {
                    id: "_ningmeng_",
                    name: "#E-柠檬",
                    img: "s63.gif"
                }, {
                    id: "_nuhuo_",
                    name: "#E-怒火",
                    img: "s64.gif"
                }, {
                    id: "_peizui_",
                    name: "#E-撇嘴",
                    img: "s65.gif"
                }, {
                    id: "_shengli_",
                    name: "#E-胜利",
                    img: "s66.gif"
                }, {
                    id: "_shijian_",
                    name: "#E-时间",
                    img: "s67.gif"
                }, {
                    id: "_taiyang_",
                    name: "#E-太阳",
                    img: "s68.gif"
                }, {
                    id: "_zhouma_",
                    name: "#E-咒骂",
                    img: "s69.gif"
                }, {
                    id: "_zhuzhu_",
                    name: "#E-猪猪",
                    img: "s70.gif"
                }, {
                    id: "_zuqiu_",
                    name: "#E-足球",
                    img: "s71.gif"
                }, {
                    id: "_zhenjing_",
                    name: "#E-震惊",
                    img: "s72.gif"
                }],
                lineSize: 12,
                path: "http://static.360buyimg.com/im/img/classic/",
                name: "经典",
                style: "im-face-item-classic",
                show: !0,
                viewStyle: "im-face-view-classic",
                defaultShow: !0,
                getPosition: function (index) {
                    var row = Math.floor(index / this.lineSize),
                        col = (index - row * this.lineSize) % this.items.length,
                        lxp = ["0", "-38px", "-75px", "-114px", "-152px", "-190px", "-228px", "-266px", "-304px", "-342px", "-380px", "-418px"],
                        ryp = ["0", "-37px", "-76px", "-114px", "-152px", "-189px"];
                    return lxp[col] + " " + ryp[row]
                },
                getViewPosition: function (index) {
                    var row = Math.floor(index / this.lineSize),
                        col = (index - row * this.lineSize) % this.items.length;
                    return {
                        top: 31 * (row + 1) + 5 + "px",
                        left: 31 * (col + 1) + 4 + "px"
                    }
                }
            },
            dongdong: {
                items: [{
                    id: "_b01_",
                    name: "#E-b01",
                    img: "b01.gif"
                }, {
                    id: "_b02_",
                    name: "#E-b02",
                    img: "b02.gif"
                }, {
                    id: "_b03_",
                    name: "#E-b03",
                    img: "b03.gif"
                }, {
                    id: "_b04_",
                    name: "#E-b04",
                    img: "b04.gif"
                }, {
                    id: "_b05_",
                    name: "#E-b05",
                    img: "b05.gif"
                }, {
                    id: "_b06_",
                    name: "#E-b06",
                    img: "b06.gif"
                }, {
                    id: "_b07_",
                    name: "#E-b07",
                    img: "b07.gif"
                }, {
                    id: "_b08_",
                    name: "#E-b08",
                    img: "b08.gif"
                }, {
                    id: "_b09_",
                    name: "#E-b09",
                    img: "b09.gif"
                }, {
                    id: "_b10_",
                    name: "#E-b10",
                    img: "b10.gif"
                }, {
                    id: "_b11_",
                    name: "#E-b11",
                    img: "b11.gif"
                }, {
                    id: "_b12_",
                    name: "#E-b12",
                    img: "b12.gif"
                }, {
                    id: "_b13_",
                    name: "#E-b13",
                    img: "b13.gif"
                }, {
                    id: "_b14_",
                    name: "#E-b14",
                    img: "b14.gif"
                }, {
                    id: "_b15_",
                    name: "#E-b15",
                    img: "b15.gif"
                }, {
                    id: "_b16_",
                    name: "#E-b16",
                    img: "b16.gif"
                }],
                lineSize: 6,
                path: "http://static.360buyimg.com/im/img/dongdong/",
                name: "咚咚",
                style: "im-face-item-dongdong",
                show: !0,
                viewStyle: "im-face-view-dongdong",
                defaultShow: !1,
                getPosition: function (index) {
                    var row = Math.floor(index / this.lineSize),
                        col = (index - row * this.lineSize) % this.items.length,
                        lxp = ["0", "-76px", "-156px", "-235px", "-313px", "-393px"],
                        ryp = ["0", "-79px", "-160px"];
                    return 0 == row && col == this.lineSize - 1 ? lxp[col] + " -1px" : lxp[col] + " " + ryp[row]
                },
                getViewPosition: function (index) {
                    var row = Math.floor(index / this.lineSize),
                        col = (index - row * this.lineSize) % this.items.length;
                    return {
                        top: 62 * (row + 1) + 5 + "px",
                        left: 62 * (col + 1) + 4 + "px"
                    }
                }
            },
            joy: {
                items: [{
                    id: "_j01_",
                    name: "#E-j01",
                    img: "j01.gif"
                }, {
                    id: "_j02_",
                    name: "#E-j02",
                    img: "j02.gif"
                }, {
                    id: "_j03_",
                    name: "#E-j03",
                    img: "j03.gif"
                }, {
                    id: "_j04_",
                    name: "#E-j04",
                    img: "j04.gif"
                }, {
                    id: "_j05_",
                    name: "#E-j05",
                    img: "j05.gif"
                }, {
                    id: "_j06_",
                    name: "#E-j06",
                    img: "j06.gif"
                }, {
                    id: "_j07_",
                    name: "#E-j07",
                    img: "j07.gif"
                }, {
                    id: "_j08_",
                    name: "#E-j08",
                    img: "j08.gif"
                }, {
                    id: "_j09_",
                    name: "#E-j09",
                    img: "j09.gif"
                }, {
                    id: "_j10_",
                    name: "#E-j10",
                    img: "j10.gif"
                }, {
                    id: "_j11_",
                    name: "#E-j11",
                    img: "j11.gif"
                }, {
                    id: "_j12_",
                    name: "#E-j12",
                    img: "j12.gif"
                }, {
                    id: "_j13_",
                    name: "#E-j13",
                    img: "j13.gif"
                }, {
                    id: "_j14_",
                    name: "#E-j14",
                    img: "j14.gif"
                }, {
                    id: "_j15_",
                    name: "#E-j15",
                    img: "j15.gif"
                }, {
                    id: "_j16_",
                    name: "#E-j16",
                    img: "j16.gif"
                }],
                lineSize: 6,
                path: "http://static.360buyimg.com/im/img/joy/",
                name: "Joy",
                style: "im-face-item-dongdong",
                show: !1,
                viewStyle: "im-face-view-dongdong",
                defaultShow: !1,
                getPosition: function (index) {
                    var row = Math.floor(index / this.lineSize),
                        col = (index - row * this.lineSize) % this.items.length,
                        lxp = ["0", "-76px", "-156px", "-235px", "-313px", "-393px"],
                        ryp = ["0", "-79px", "-160px"];
                    return 0 == row && col == this.lineSize - 1 ? lxp[col] + " -1px" : lxp[col] + " " + ryp[row]
                },
                getViewPosition: function (index) {
                    var row = Math.floor(index / this.lineSize),
                        col = (index - row * this.lineSize) % this.items.length;
                    return {
                        top: 62 * (row + 1) + 5 + "px",
                        left: 62 * (col + 1) + 4 + "px"
                    }
                }
            }
        },
        imgClick: function () {
        },
        getImg: function (id) {
            return getImg(id)
        }
    }
}(jQuery);
var SWFUpload, swfobject;
void 0 == SWFUpload && (SWFUpload = function (settings) {
    this.initSWFUpload(settings)
}), SWFUpload.prototype.initSWFUpload = function (userSettings) {
    try {
        this.customSettings = {}, this.settings = {}, this.eventQueue = [], this.movieName = "SWFUpload_" + SWFUpload.movieCount++, this.movieElement = null, SWFUpload.instances[this.movieName] = this, this.initSettings(userSettings), this.loadSupport(), this.swfuploadPreload() && this.loadFlash(), this.displayDebugInfo()
    } catch (ex) {
        throw delete SWFUpload.instances[this.movieName], ex
    }
}, SWFUpload.instances = {}, SWFUpload.movieCount = 0, SWFUpload.version = "2.5.0 2010-01-15 Beta 2", SWFUpload.QUEUE_ERROR = {
    QUEUE_LIMIT_EXCEEDED: -100,
    FILE_EXCEEDS_SIZE_LIMIT: -110,
    ZERO_BYTE_FILE: -120,
    INVALID_FILETYPE: -130
}, SWFUpload.UPLOAD_ERROR = {
    HTTP_ERROR: -200,
    MISSING_UPLOAD_URL: -210,
    IO_ERROR: -220,
    SECURITY_ERROR: -230,
    UPLOAD_LIMIT_EXCEEDED: -240,
    UPLOAD_FAILED: -250,
    SPECIFIED_FILE_ID_NOT_FOUND: -260,
    FILE_VALIDATION_FAILED: -270,
    FILE_CANCELLED: -280,
    UPLOAD_STOPPED: -290,
    RESIZE: -300
}, SWFUpload.FILE_STATUS = {
    QUEUED: -1,
    IN_PROGRESS: -2,
    ERROR: -3,
    COMPLETE: -4,
    CANCELLED: -5
}, SWFUpload.UPLOAD_TYPE = {
    NORMAL: -1,
    RESIZED: -2
}, SWFUpload.BUTTON_ACTION = {
    SELECT_FILE: -100,
    SELECT_FILES: -110,
    START_UPLOAD: -120,
    JAVASCRIPT: -130,
    NONE: -130
}, SWFUpload.CURSOR = {
    ARROW: -1,
    HAND: -2
}, SWFUpload.WINDOW_MODE = {
    WINDOW: "window",
    TRANSPARENT: "transparent",
    OPAQUE: "opaque"
}, SWFUpload.RESIZE_ENCODING = {
    JPEG: -1,
    PNG: -2
}, SWFUpload.completeURL = function (url) {
    try {
        var path = "",
            indexSlash = -1;
        return "string" != typeof url || url.match(/^https?:\/\//i) || url.match(/^\//) || "" === url ? url : (indexSlash = window.location.pathname.lastIndexOf("/"), path = 0 >= indexSlash ? "/" : window.location.pathname.substr(0, indexSlash) + "/", path + url)
    } catch (ex) {
        return url
    }
}, SWFUpload.onload = function () {
}, SWFUpload.prototype.initSettings = function (userSettings) {
    this.ensureDefault = function (settingName, defaultValue) {
        var setting = userSettings[settingName];
        this.settings[settingName] = void 0 != setting ? setting : defaultValue
    }, this.ensureDefault("upload_url", ""), this.ensureDefault("preserve_relative_urls", !1), this.ensureDefault("file_post_name", "Filedata"), this.ensureDefault("post_params", {}), this.ensureDefault("use_query_string", !1), this.ensureDefault("requeue_on_error", !1), this.ensureDefault("http_success", []), this.ensureDefault("assume_success_timeout", 0), this.ensureDefault("file_types", "*.*"), this.ensureDefault("file_types_description", "All Files"), this.ensureDefault("file_size_limit", 0), this.ensureDefault("file_upload_limit", 0), this.ensureDefault("file_queue_limit", 0), this.ensureDefault("flash_url", "swfupload.swf"), this.ensureDefault("flash9_url", "swfupload_fp9.swf"), this.ensureDefault("prevent_swf_caching", !0), this.ensureDefault("button_image_url", ""), this.ensureDefault("button_width", 1), this.ensureDefault("button_height", 1), this.ensureDefault("button_text", ""), this.ensureDefault("button_text_style", "color: #000000; font-size: 16pt;"), this.ensureDefault("button_text_top_padding", 0), this.ensureDefault("button_text_left_padding", 0), this.ensureDefault("button_action", SWFUpload.BUTTON_ACTION.SELECT_FILES), this.ensureDefault("button_disabled", !1), this.ensureDefault("button_placeholder_id", ""), this.ensureDefault("button_placeholder", null), this.ensureDefault("button_cursor", SWFUpload.CURSOR.ARROW), this.ensureDefault("button_window_mode", SWFUpload.WINDOW_MODE.WINDOW), this.ensureDefault("debug", !1), this.settings.debug_enabled = this.settings.debug, this.settings.return_upload_start_handler = this.returnUploadStart, this.ensureDefault("swfupload_preload_handler", null), this.ensureDefault("swfupload_load_failed_handler", null), this.ensureDefault("swfupload_loaded_handler", null), this.ensureDefault("file_dialog_start_handler", null), this.ensureDefault("file_queued_handler", null), this.ensureDefault("file_queue_error_handler", null), this.ensureDefault("file_dialog_complete_handler", null), this.ensureDefault("upload_resize_start_handler", null), this.ensureDefault("upload_start_handler", null), this.ensureDefault("upload_progress_handler", null), this.ensureDefault("upload_error_handler", null), this.ensureDefault("upload_success_handler", null), this.ensureDefault("upload_complete_handler", null), this.ensureDefault("mouse_click_handler", null), this.ensureDefault("mouse_out_handler", null), this.ensureDefault("mouse_over_handler", null), this.ensureDefault("debug_handler", this.debugMessage), this.ensureDefault("custom_settings", {}), this.customSettings = this.settings.custom_settings, this.settings.prevent_swf_caching && (this.settings.flash_url = this.settings.flash_url + (0 > this.settings.flash_url.indexOf("?") ? "?" : "&") + "preventswfcaching=" + (new Date).getTime(), this.settings.flash9_url = this.settings.flash9_url + (0 > this.settings.flash9_url.indexOf("?") ? "?" : "&") + "preventswfcaching=" + (new Date).getTime()), this.settings.preserve_relative_urls || (this.settings.upload_url = SWFUpload.completeURL(this.settings.upload_url), this.settings.button_image_url = SWFUpload.completeURL(this.settings.button_image_url)), delete this.ensureDefault
}, SWFUpload.prototype.loadSupport = function () {
    this.support = {
        loading: swfobject.hasFlashPlayerVersion("9.0.28"),
        imageResize: swfobject.hasFlashPlayerVersion("10.0.0")
    }
}, SWFUpload.prototype.loadFlash = function () {
    var targetElement, tempParent, wrapperType, flashHTML, els;
    if (!this.support.loading) return this.queueEvent("swfupload_load_failed_handler", ["Flash Player doesn't support SWFUpload"]), void 0;
    if (null !== document.getElementById(this.movieName)) return this.support.loading = !1, this.queueEvent("swfupload_load_failed_handler", ["Element ID already in use"]), void 0;
    if (targetElement = document.getElementById(this.settings.button_placeholder_id) || this.settings.button_placeholder, void 0 == targetElement) return this.support.loading = !1, this.queueEvent("swfupload_load_failed_handler", ["button place holder not found"]), void 0;
    wrapperType = "block" !== (targetElement.currentStyle && targetElement.currentStyle.display || window.getComputedStyle && document.defaultView.getComputedStyle(targetElement, null).getPropertyValue("display")) ? "span" : "div", tempParent = document.createElement(wrapperType), flashHTML = this.getFlashHTML();
    try {
        tempParent.innerHTML = flashHTML
    } catch (ex) {
        return this.support.loading = !1, this.queueEvent("swfupload_load_failed_handler", ["Exception loading Flash HTML into placeholder"]), void 0
    }
    return els = tempParent.getElementsByTagName("object"), !els || els.length > 1 || 0 === els.length ? (this.support.loading = !1, this.queueEvent("swfupload_load_failed_handler", ["Unable to find movie after adding to DOM"]), void 0) : (1 === els.length && (this.movieElement = els[0]), targetElement.parentNode.replaceChild(tempParent.firstChild, targetElement), void 0 == window[this.movieName] && (window[this.movieName] = this.getMovieElement()), void 0)
}, SWFUpload.prototype.getFlashHTML = function () {
    return ['<object id="', this.movieName, '" type="application/x-shockwave-flash" data="', this.support.imageResize ? this.settings.flash_url : this.settings.flash9_url, '" width="', this.settings.button_width, '" height="', this.settings.button_height, '" class="swfupload">', '<param name="wmode" value="', this.settings.button_window_mode, '" />', '<param name="movie" value="', this.support.imageResize ? this.settings.flash_url : this.settings.flash9_url, '" />', '<param name="quality" value="high" />', '<param name="allowScriptAccess" value="always" />', '<param name="flashvars" value="' + this.getFlashVars() + '" />', "</object>"].join("")
}, SWFUpload.prototype.getFlashVars = function () {
    var httpSuccessString, paramString;
    return paramString = this.buildParamString(), httpSuccessString = this.settings.http_success.join(","), ["movieName=", encodeURIComponent(this.movieName), "&amp;uploadURL=", encodeURIComponent(this.settings.upload_url), "&amp;useQueryString=", encodeURIComponent(this.settings.use_query_string), "&amp;requeueOnError=", encodeURIComponent(this.settings.requeue_on_error), "&amp;httpSuccess=", encodeURIComponent(httpSuccessString), "&amp;assumeSuccessTimeout=", encodeURIComponent(this.settings.assume_success_timeout), "&amp;params=", encodeURIComponent(paramString), "&amp;filePostName=", encodeURIComponent(this.settings.file_post_name), "&amp;fileTypes=", encodeURIComponent(this.settings.file_types), "&amp;fileTypesDescription=", encodeURIComponent(this.settings.file_types_description), "&amp;fileSizeLimit=", encodeURIComponent(this.settings.file_size_limit), "&amp;fileUploadLimit=", encodeURIComponent(this.settings.file_upload_limit), "&amp;fileQueueLimit=", encodeURIComponent(this.settings.file_queue_limit), "&amp;debugEnabled=", encodeURIComponent(this.settings.debug_enabled), "&amp;buttonImageURL=", encodeURIComponent(this.settings.button_image_url), "&amp;buttonWidth=", encodeURIComponent(this.settings.button_width), "&amp;buttonHeight=", encodeURIComponent(this.settings.button_height), "&amp;buttonText=", encodeURIComponent(this.settings.button_text), "&amp;buttonTextTopPadding=", encodeURIComponent(this.settings.button_text_top_padding), "&amp;buttonTextLeftPadding=", encodeURIComponent(this.settings.button_text_left_padding), "&amp;buttonTextStyle=", encodeURIComponent(this.settings.button_text_style), "&amp;buttonAction=", encodeURIComponent(this.settings.button_action), "&amp;buttonDisabled=", encodeURIComponent(this.settings.button_disabled), "&amp;buttonCursor=", encodeURIComponent(this.settings.button_cursor)].join("")
}, SWFUpload.prototype.getMovieElement = function () {
    if (void 0 == this.movieElement && (this.movieElement = document.getElementById(this.movieName)), null === this.movieElement) throw "Could not find Flash element";
    return this.movieElement
}, SWFUpload.prototype.buildParamString = function () {
    var name, postParams, paramStringPairs = [];
    if (postParams = this.settings.post_params, "object" == typeof postParams) for (name in postParams) postParams.hasOwnProperty(name) && paramStringPairs.push(encodeURIComponent("" + name) + "=" + encodeURIComponent("" + postParams[name]));
    return paramStringPairs.join("&amp;")
}, SWFUpload.prototype.destroy = function () {
    var movieElement;
    try {
        if (this.cancelUpload(null, !1), movieElement = this.cleanUp()) try {
            movieElement.parentNode.removeChild(movieElement)
        } catch (ex) {
        }
        return window[this.movieName] = null, SWFUpload.instances[this.movieName] = null, delete SWFUpload.instances[this.movieName], this.movieElement = null, this.settings = null, this.customSettings = null, this.eventQueue = null, this.movieName = null, !0
    } catch (ex2) {
        return !1
    }
}, SWFUpload.prototype.displayDebugInfo = function () {
    this.debug(["---SWFUpload Instance Info---\n", "Version: ", SWFUpload.version, "\n", "Movie Name: ", this.movieName, "\n", "Settings:\n", "	", "upload_url:               ", this.settings.upload_url, "\n", "	", "flash_url:                ", this.settings.flash_url, "\n", "	", "flash9_url:                ", this.settings.flash9_url, "\n", "	", "use_query_string:         ", "" + this.settings.use_query_string, "\n", "	", "requeue_on_error:         ", "" + this.settings.requeue_on_error, "\n", "	", "http_success:             ", this.settings.http_success.join(", "), "\n", "	", "assume_success_timeout:   ", this.settings.assume_success_timeout, "\n", "	", "file_post_name:           ", this.settings.file_post_name, "\n", "	", "post_params:              ", "" + this.settings.post_params, "\n", "	", "file_types:               ", this.settings.file_types, "\n", "	", "file_types_description:   ", this.settings.file_types_description, "\n", "	", "file_size_limit:          ", this.settings.file_size_limit, "\n", "	", "file_upload_limit:        ", this.settings.file_upload_limit, "\n", "	", "file_queue_limit:         ", this.settings.file_queue_limit, "\n", "	", "debug:                    ", "" + this.settings.debug, "\n", "	", "prevent_swf_caching:      ", "" + this.settings.prevent_swf_caching, "\n", "	", "button_placeholder_id:    ", "" + this.settings.button_placeholder_id, "\n", "	", "button_placeholder:       ", this.settings.button_placeholder ? "Set" : "Not Set", "\n", "	", "button_image_url:         ", "" + this.settings.button_image_url, "\n", "	", "button_width:             ", "" + this.settings.button_width, "\n", "	", "button_height:            ", "" + this.settings.button_height, "\n", "	", "button_text:              ", "" + this.settings.button_text, "\n", "	", "button_text_style:        ", "" + this.settings.button_text_style, "\n", "	", "button_text_top_padding:  ", "" + this.settings.button_text_top_padding, "\n", "	", "button_text_left_padding: ", "" + this.settings.button_text_left_padding, "\n", "	", "button_action:            ", "" + this.settings.button_action, "\n", "	", "button_cursor:            ", "" + this.settings.button_cursor, "\n", "	", "button_disabled:          ", "" + this.settings.button_disabled, "\n", "	", "custom_settings:          ", "" + this.settings.custom_settings, "\n", "Event Handlers:\n", "	", "swfupload_preload_handler assigned:  ", "" + ("function" == typeof this.settings.swfupload_preload_handler), "\n", "	", "swfupload_load_failed_handler assigned:  ", "" + ("function" == typeof this.settings.swfupload_load_failed_handler), "\n", "	", "swfupload_loaded_handler assigned:  ", "" + ("function" == typeof this.settings.swfupload_loaded_handler), "\n", "	", "mouse_click_handler assigned:       ", "" + ("function" == typeof this.settings.mouse_click_handler), "\n", "	", "mouse_over_handler assigned:        ", "" + ("function" == typeof this.settings.mouse_over_handler), "\n", "	", "mouse_out_handler assigned:         ", "" + ("function" == typeof this.settings.mouse_out_handler), "\n", "	", "file_dialog_start_handler assigned: ", "" + ("function" == typeof this.settings.file_dialog_start_handler), "\n", "	", "file_queued_handler assigned:       ", "" + ("function" == typeof this.settings.file_queued_handler), "\n", "	", "file_queue_error_handler assigned:  ", "" + ("function" == typeof this.settings.file_queue_error_handler), "\n", "	", "upload_resize_start_handler assigned:      ", "" + ("function" == typeof this.settings.upload_resize_start_handler), "\n", "	", "upload_start_handler assigned:      ", "" + ("function" == typeof this.settings.upload_start_handler), "\n", "	", "upload_progress_handler assigned:   ", "" + ("function" == typeof this.settings.upload_progress_handler), "\n", "	", "upload_error_handler assigned:      ", "" + ("function" == typeof this.settings.upload_error_handler), "\n", "	", "upload_success_handler assigned:    ", "" + ("function" == typeof this.settings.upload_success_handler), "\n", "	", "upload_complete_handler assigned:   ", "" + ("function" == typeof this.settings.upload_complete_handler), "\n", "	", "debug_handler assigned:             ", "" + ("function" == typeof this.settings.debug_handler), "\n", "Support:\n", "	", "Load:                     ", this.support.loading ? "Yes" : "No", "\n", "	", "Image Resize:             ", this.support.imageResize ? "Yes" : "No", "\n"].join(""))
}, SWFUpload.prototype.addSetting = function (name, value, default_value) {
    return this.settings[name] = void 0 == value ? default_value : value
}, SWFUpload.prototype.getSetting = function (name) {
    return void 0 != this.settings[name] ? this.settings[name] : ""
}, SWFUpload.prototype.callFlash = function (functionName, argumentArray) {
    var movieElement, returnValue, returnString;
    argumentArray = argumentArray || [], movieElement = this.getMovieElement();
    try {
        void 0 != movieElement ? (returnString = movieElement.CallFunction('<invoke name="' + functionName + '" returntype="javascript">' + __flash__argumentsToXML(argumentArray, 0) + "</invoke>"), returnValue = eval(returnString)) : this.debug("Can't call flash because the movie wasn't found.")
    } catch (ex) {
        this.debug("Exception calling flash function '" + functionName + "': " + ex.message)
    }
    return void 0 != returnValue && "object" == typeof returnValue.post && (returnValue = this.unescapeFilePostParams(returnValue)), returnValue
}, SWFUpload.prototype.selectFile = function () {
    this.callFlash("SelectFile")
}, SWFUpload.prototype.selectFiles = function () {
    this.callFlash("SelectFiles")
}, SWFUpload.prototype.startUpload = function (fileID) {
    this.callFlash("StartUpload", [fileID])
}, SWFUpload.prototype.startResizedUpload = function (fileID, width, height, encoding, quality, allowEnlarging) {
    this.callFlash("StartUpload", [fileID,
        {
            width: width,
            height: height,
            encoding: encoding,
            quality: quality,
            allowEnlarging: allowEnlarging
        }])
}, SWFUpload.prototype.cancelUpload = function (fileID, triggerErrorEvent) {
    triggerErrorEvent !== !1 && (triggerErrorEvent = !0), this.callFlash("CancelUpload", [fileID, triggerErrorEvent])
}, SWFUpload.prototype.stopUpload = function () {
    this.callFlash("StopUpload")
}, SWFUpload.prototype.requeueUpload = function (indexOrFileID) {
    return this.callFlash("RequeueUpload", [indexOrFileID])
}, SWFUpload.prototype.getStats = function () {
    return this.callFlash("GetStats")
}, SWFUpload.prototype.setStats = function (statsObject) {
    this.callFlash("SetStats", [statsObject])
}, SWFUpload.prototype.getFile = function (fileID) {
    return "number" == typeof fileID ? this.callFlash("GetFileByIndex", [fileID]) : this.callFlash("GetFile", [fileID])
}, SWFUpload.prototype.getQueueFile = function (fileID) {
    return "number" == typeof fileID ? this.callFlash("GetFileByQueueIndex", [fileID]) : this.callFlash("GetFile", [fileID])
}, SWFUpload.prototype.addFileParam = function (fileID, name, value) {
    return this.callFlash("AddFileParam", [fileID, name, value])
}, SWFUpload.prototype.removeFileParam = function (fileID, name) {
    this.callFlash("RemoveFileParam", [fileID, name])
}, SWFUpload.prototype.setUploadURL = function (url) {
    this.settings.upload_url = "" + url, this.callFlash("SetUploadURL", [url])
}, SWFUpload.prototype.setPostParams = function (paramsObject) {
    this.settings.post_params = paramsObject, this.callFlash("SetPostParams", [paramsObject])
}, SWFUpload.prototype.addPostParam = function (name, value) {
    this.settings.post_params[name] = value, this.callFlash("SetPostParams", [this.settings.post_params])
}, SWFUpload.prototype.removePostParam = function (name) {
    delete this.settings.post_params[name], this.callFlash("SetPostParams", [this.settings.post_params])
}, SWFUpload.prototype.setFileTypes = function (types, description) {
    this.settings.file_types = types, this.settings.file_types_description = description, this.callFlash("SetFileTypes", [types, description])
}, SWFUpload.prototype.setFileSizeLimit = function (fileSizeLimit) {
    this.settings.file_size_limit = fileSizeLimit, this.callFlash("SetFileSizeLimit", [fileSizeLimit])
}, SWFUpload.prototype.setFileUploadLimit = function (fileUploadLimit) {
    this.settings.file_upload_limit = fileUploadLimit, this.callFlash("SetFileUploadLimit", [fileUploadLimit])
}, SWFUpload.prototype.setFileQueueLimit = function (fileQueueLimit) {
    this.settings.file_queue_limit = fileQueueLimit, this.callFlash("SetFileQueueLimit", [fileQueueLimit])
}, SWFUpload.prototype.setFilePostName = function (filePostName) {
    this.settings.file_post_name = filePostName, this.callFlash("SetFilePostName", [filePostName])
}, SWFUpload.prototype.setUseQueryString = function (useQueryString) {
    this.settings.use_query_string = useQueryString, this.callFlash("SetUseQueryString", [useQueryString])
}, SWFUpload.prototype.setRequeueOnError = function (requeueOnError) {
    this.settings.requeue_on_error = requeueOnError, this.callFlash("SetRequeueOnError", [requeueOnError])
}, SWFUpload.prototype.setHTTPSuccess = function (http_status_codes) {
    "string" == typeof http_status_codes && (http_status_codes = http_status_codes.replace(" ", "").split(",")), this.settings.http_success = http_status_codes, this.callFlash("SetHTTPSuccess", [http_status_codes])
}, SWFUpload.prototype.setAssumeSuccessTimeout = function (timeout_seconds) {
    this.settings.assume_success_timeout = timeout_seconds, this.callFlash("SetAssumeSuccessTimeout", [timeout_seconds])
}, SWFUpload.prototype.setDebugEnabled = function (debugEnabled) {
    this.settings.debug_enabled = debugEnabled, this.callFlash("SetDebugEnabled", [debugEnabled])
}, SWFUpload.prototype.setButtonImageURL = function (buttonImageURL) {
    void 0 == buttonImageURL && (buttonImageURL = ""), this.settings.button_image_url = buttonImageURL, this.callFlash("SetButtonImageURL", [buttonImageURL])
}, SWFUpload.prototype.setButtonDimensions = function (width, height) {
    this.settings.button_width = width, this.settings.button_height = height;
    var movie = this.getMovieElement();
    void 0 != movie && (movie.style.width = width + "px", movie.style.height = height + "px"), this.callFlash("SetButtonDimensions", [width, height])
}, SWFUpload.prototype.setButtonText = function (html) {
    this.settings.button_text = html, this.callFlash("SetButtonText", [html])
}, SWFUpload.prototype.setButtonTextPadding = function (left, top) {
    this.settings.button_text_top_padding = top, this.settings.button_text_left_padding = left, this.callFlash("SetButtonTextPadding", [left, top])
}, SWFUpload.prototype.setButtonTextStyle = function (css) {
    this.settings.button_text_style = css, this.callFlash("SetButtonTextStyle", [css])
}, SWFUpload.prototype.setButtonDisabled = function (isDisabled) {
    this.settings.button_disabled = isDisabled, this.callFlash("SetButtonDisabled", [isDisabled])
}, SWFUpload.prototype.setButtonAction = function (buttonAction) {
    this.settings.button_action = buttonAction, this.callFlash("SetButtonAction", [buttonAction])
}, SWFUpload.prototype.setButtonCursor = function (cursor) {
    this.settings.button_cursor = cursor, this.callFlash("SetButtonCursor", [cursor])
}, SWFUpload.prototype.queueEvent = function (handlerName, argumentArray) {
    var self = this;
    if (void 0 == argumentArray ? argumentArray = [] : argumentArray instanceof Array || (argumentArray = [argumentArray]), "function" == typeof this.settings[handlerName]) this.eventQueue.push(function () {
        this.settings[handlerName].apply(this, argumentArray)
    }), setTimeout(function () {
        self.executeNextEvent()
    }, 0);
    else if (null !== this.settings[handlerName]) throw "Event handler " + handlerName + " is unknown or is not a function"
}, SWFUpload.prototype.executeNextEvent = function () {
    var f = this.eventQueue ? this.eventQueue.shift() : null;
    "function" == typeof f && f.apply(this)
}, SWFUpload.prototype.unescapeFilePostParams = function (file) {
    var uk, k, match, reg = /[$]([0-9a-f]{4})/i,
        unescapedPost = {};
    if (void 0 != file) {
        for (k in file.post) if (file.post.hasOwnProperty(k)) {
            for (uk = k; null !== (match = reg.exec(uk));) uk = uk.replace(match[0], String.fromCharCode(parseInt("0x" + match[1], 16)));
            unescapedPost[uk] = file.post[k]
        }
        file.post = unescapedPost
    }
    return file
}, SWFUpload.prototype.swfuploadPreload = function () {
    var returnValue;
    if ("function" == typeof this.settings.swfupload_preload_handler) returnValue = this.settings.swfupload_preload_handler.call(this);
    else if (void 0 != this.settings.swfupload_preload_handler) throw "upload_start_handler must be a function";
    return void 0 === returnValue && (returnValue = !0), !!returnValue
}, SWFUpload.prototype.flashReady = function () {
    var movieElement = this.cleanUp();
    return movieElement ? (this.queueEvent("swfupload_loaded_handler"), void 0) : (this.debug("Flash called back ready but the flash movie can't be found."), void 0)
}, SWFUpload.prototype.cleanUp = function () {
    var key, movieElement = this.getMovieElement();
    try {
        if (movieElement && "unknown" == typeof movieElement.CallFunction) {
            this.debug("Removing Flash functions hooks (this should only run in IE and should prevent memory leaks)");
            for (key in movieElement) try {
                "function" == typeof movieElement[key] && (movieElement[key] = null)
            } catch (ex) {
            }
        }
    } catch (ex1) {
    }
    return window.__flash__removeCallback = function (instance, name) {
        try {
            instance && (instance[name] = null)
        } catch (flashEx) {
        }
    }, movieElement
}, SWFUpload.prototype.mouseClick = function () {
    this.queueEvent("mouse_click_handler")
}, SWFUpload.prototype.mouseOver = function () {
    this.queueEvent("mouse_over_handler")
}, SWFUpload.prototype.mouseOut = function () {
    this.queueEvent("mouse_out_handler")
}, SWFUpload.prototype.fileDialogStart = function () {
    this.queueEvent("file_dialog_start_handler")
}, SWFUpload.prototype.fileQueued = function (file) {
    file = this.unescapeFilePostParams(file), this.queueEvent("file_queued_handler", file)
}, SWFUpload.prototype.fileQueueError = function (file, errorCode, message) {
    file = this.unescapeFilePostParams(file), this.queueEvent("file_queue_error_handler", [file, errorCode, message])
}, SWFUpload.prototype.fileDialogComplete = function (numFilesSelected, numFilesQueued, numFilesInQueue) {
    this.queueEvent("file_dialog_complete_handler", [numFilesSelected, numFilesQueued, numFilesInQueue])
}, SWFUpload.prototype.uploadResizeStart = function (file, resizeSettings) {
    file = this.unescapeFilePostParams(file), this.queueEvent("upload_resize_start_handler", [file, resizeSettings.width, resizeSettings.height, resizeSettings.encoding, resizeSettings.quality])
}, SWFUpload.prototype.uploadStart = function (file) {
    file = this.unescapeFilePostParams(file), this.queueEvent("return_upload_start_handler", file)
}, SWFUpload.prototype.returnUploadStart = function (file) {
    var returnValue;
    if ("function" == typeof this.settings.upload_start_handler) file = this.unescapeFilePostParams(file), returnValue = this.settings.upload_start_handler.call(this, file);
    else if (void 0 != this.settings.upload_start_handler) throw "upload_start_handler must be a function";
    void 0 === returnValue && (returnValue = !0), returnValue = !!returnValue, this.callFlash("ReturnUploadStart", [returnValue])
}, SWFUpload.prototype.uploadProgress = function (file, bytesComplete, bytesTotal) {
    file = this.unescapeFilePostParams(file), this.queueEvent("upload_progress_handler", [file, bytesComplete, bytesTotal])
}, SWFUpload.prototype.uploadError = function (file, errorCode, message) {
    file = this.unescapeFilePostParams(file), this.queueEvent("upload_error_handler", [file, errorCode, message])
}, SWFUpload.prototype.uploadSuccess = function (file, serverData, responseReceived) {
    file = this.unescapeFilePostParams(file), this.queueEvent("upload_success_handler", [file, serverData, responseReceived])
}, SWFUpload.prototype.uploadComplete = function (file) {
    file = this.unescapeFilePostParams(file), this.queueEvent("upload_complete_handler", file)
}, SWFUpload.prototype.debug = function (message) {
    this.queueEvent("debug_handler", message)
}, SWFUpload.prototype.debugMessage = function (message) {
    var exceptionMessage, exceptionValues, key;
    if (this.settings.debug) if (exceptionValues = [], "object" == typeof message && "string" == typeof message.name && "string" == typeof message.message) {
        for (key in message) message.hasOwnProperty(key) && exceptionValues.push(key + ": " + message[key]);
        exceptionMessage = exceptionValues.join("\n") || "", exceptionValues = exceptionMessage.split("\n"), exceptionMessage = "EXCEPTION: " + exceptionValues.join("\nEXCEPTION: "), SWFUpload.Console.writeLine(exceptionMessage)
    } else SWFUpload.Console.writeLine(message)
}, SWFUpload.Console = {}, SWFUpload.Console.writeLine = function (message) {
    var console, documentForm;
    try {
        console = document.getElementById("SWFUpload_Console"), console || (documentForm = document.createElement("form"), document.getElementsByTagName("body")[0].appendChild(documentForm), console = document.createElement("textarea"), console.id = "SWFUpload_Console", console.style.fontFamily = "monospace", console.setAttribute("wrap", "off"), console.wrap = "off", console.style.overflow = "auto", console.style.width = "700px", console.style.height = "350px", console.style.margin = "5px", documentForm.appendChild(console)), console.value += message + "\n", console.scrollTop = console.scrollHeight - console.clientHeight
    } catch (ex) {
        alert("Exception: " + ex.name + " Message: " + ex.message)
    }
}, swfobject = function () {
    function f() {
        if (!J) {
            try {
                var Z = j.getElementsByTagName("body")[0].appendChild(C("span"));
                Z.parentNode.removeChild(Z)
            } catch (aa) {
                return
            }
            J = !0;
            for (var X = U.length, Y = 0; X > Y; Y++) U[Y]()
        }
    }

    function K(X) {
        J ? X() : U[U.length] = X
    }

    function s(Y) {
        if (typeof O.addEventListener != D) O.addEventListener("load", Y, !1);
        else if (typeof j.addEventListener != D) j.addEventListener("load", Y, !1);
        else if (typeof O.attachEvent != D) i(O, "onload", Y);
        else if ("function" == typeof O.onload) {
            var X = O.onload;
            O.onload = function () {
                X(), Y()
            }
        } else O.onload = Y
    }

    function h() {
        T ? V() : H()
    }

    function V() {
        var X = j.getElementsByTagName("body")[0],
            aa = C(r);
        aa.setAttribute("type", q);
        var Z = X.appendChild(aa);
        if (Z) {
            var Y = 0;
            (function () {
                if (typeof Z.GetVariable != D) {
                    var ab = Z.GetVariable("$version");
                    ab && (ab = ab.split(" ")[1].split(","), M.pv = [parseInt(ab[0], 10), parseInt(ab[1], 10), parseInt(ab[2], 10)])
                } else if (10 > Y) return Y++, setTimeout(arguments.callee, 10), void 0;
                X.removeChild(aa), Z = null, H()
            })()
        } else H()
    }

    function H() {
        var ag = o.length;
        if (ag > 0) for (var af = 0; ag > af; af++) {
            var Y = o[af].id,
                ab = o[af].callbackFn,
                aa = {
                    success: !1,
                    id: Y
                };
            if (M.pv[0] > 0) {
                var ae = c(Y);
                if (ae) if (!F(o[af].swfVersion) || M.wk && 312 > M.wk) if (o[af].expressInstall && A()) {
                    var ai = {};
                    ai.data = o[af].expressInstall, ai.width = ae.getAttribute("width") || "0", ai.height = ae.getAttribute("height") || "0", ae.getAttribute("class") && (ai.styleclass = ae.getAttribute("class")), ae.getAttribute("align") && (ai.align = ae.getAttribute("align"));
                    for (var ah = {}, X = ae.getElementsByTagName("param"), ac = X.length, ad = 0; ac > ad; ad++)"movie" != X[ad].getAttribute("name").toLowerCase() && (ah[X[ad].getAttribute("name")] = X[ad].getAttribute("value"));
                    P(ai, ah, Y, ab)
                } else p(ae), ab && ab(aa);
                else w(Y, !0), ab && (aa.success = !0, aa.ref = z(Y), ab(aa))
            } else if (w(Y, !0), ab) {
                var Z = z(Y);
                Z && typeof Z.SetVariable != D && (aa.success = !0, aa.ref = Z), ab(aa)
            }
        }
    }

    function z(aa) {
        var X = null,
            Y = c(aa);
        if (Y && "OBJECT" == Y.nodeName) if (typeof Y.SetVariable != D) X = Y;
        else {
            var Z = Y.getElementsByTagName(r)[0];
            Z && (X = Z)
        }
        return X
    }

    function A() {
        return !a && F("6.0.65") && (M.win || M.mac) && !(M.wk && 312 > M.wk)
    }

    function P(aa, ab, X, Z) {
        a = !0, E = Z || null, B = {
            success: !1,
            id: X
        };
        var ae = c(X);
        if (ae) {
            "OBJECT" == ae.nodeName ? (l = g(ae), Q = null) : (l = ae, Q = X), aa.id = R, (typeof aa.width == D || !/%$/.test(aa.width) && 310 > parseInt(aa.width, 10)) && (aa.width = "310"), (typeof aa.height == D || !/%$/.test(aa.height) && 137 > parseInt(aa.height, 10)) && (aa.height = "137"), j.title = j.title.slice(0, 47) + " - Flash Player Installation";
            var ad = M.ie && M.win ? "ActiveX" : "PlugIn",
                ac = "MMredirectURL=" + ("" + O.location).replace(/&/g, "%26") + "&MMplayerType=" + ad + "&MMdoctitle=" + j.title;
            if (typeof ab.flashvars != D ? ab.flashvars += "&" + ac : ab.flashvars = ac, M.ie && M.win && 4 != ae.readyState) {
                var Y = C("div");
                X += "SWFObjectNew", Y.setAttribute("id", X), ae.parentNode.insertBefore(Y, ae), ae.style.display = "none", function () {
                    4 == ae.readyState ? ae.parentNode.removeChild(ae) : setTimeout(arguments.callee, 10)
                }()
            }
            u(aa, ab, X)
        }
    }

    function p(Y) {
        if (M.ie && M.win && 4 != Y.readyState) {
            var X = C("div");
            Y.parentNode.insertBefore(X, Y), X.parentNode.replaceChild(g(Y), X), Y.style.display = "none", function () {
                4 == Y.readyState ? Y.parentNode.removeChild(Y) : setTimeout(arguments.callee, 10)
            }()
        } else Y.parentNode.replaceChild(g(Y), Y)
    }

    function g(ab) {
        var aa = C("div");
        if (M.win && M.ie) aa.innerHTML = ab.innerHTML;
        else {
            var Y = ab.getElementsByTagName(r)[0];
            if (Y) {
                var ad = Y.childNodes;
                if (ad) for (var X = ad.length, Z = 0; X > Z; Z++) 1 == ad[Z].nodeType && "PARAM" == ad[Z].nodeName || 8 == ad[Z].nodeType || aa.appendChild(ad[Z].cloneNode(!0))
            }
        }
        return aa
    }

    function u(ai, ag, Y) {
        var X, aa = c(Y);
        if (M.wk && 312 > M.wk) return X;
        if (aa) if (typeof ai.id == D && (ai.id = Y), M.ie && M.win) {
            var ah = "";
            for (var ae in ai) ai[ae] != Object.prototype[ae] && ("data" == ae.toLowerCase() ? ag.movie = ai[ae] : "styleclass" == ae.toLowerCase() ? ah += ' class="' + ai[ae] + '"' : "classid" != ae.toLowerCase() && (ah += " " + ae + '="' + ai[ae] + '"'));
            var af = "";
            for (var ad in ag) ag[ad] != Object.prototype[ad] && (af += '<param name="' + ad + '" value="' + ag[ad] + '" />');
            aa.outerHTML = '<object classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000"' + ah + ">" + af + "</object>", N[N.length] = ai.id, X = c(ai.id)
        } else {
            var Z = C(r);
            Z.setAttribute("type", q);
            for (var ac in ai) ai[ac] != Object.prototype[ac] && ("styleclass" == ac.toLowerCase() ? Z.setAttribute("class", ai[ac]) : "classid" != ac.toLowerCase() && Z.setAttribute(ac, ai[ac]));
            for (var ab in ag) ag[ab] != Object.prototype[ab] && "movie" != ab.toLowerCase() && e(Z, ab, ag[ab]);
            aa.parentNode.replaceChild(Z, aa), X = Z
        }
        return X
    }

    function e(Z, X, Y) {
        var aa = C("param");
        aa.setAttribute("name", X), aa.setAttribute("value", Y), Z.appendChild(aa)
    }

    function y(Y) {
        var X = c(Y);
        X && "OBJECT" == X.nodeName && (M.ie && M.win ? (X.style.display = "none", function () {
            4 == X.readyState ? b(Y) : setTimeout(arguments.callee, 10)
        }()) : X.parentNode.removeChild(X))
    }

    function b(Z) {
        var Y = c(Z);
        if (Y) {
            for (var X in Y)"function" == typeof Y[X] && (Y[X] = null);
            Y.parentNode.removeChild(Y)
        }
    }

    function c(Z) {
        var X = null;
        try {
            X = j.getElementById(Z)
        } catch (Y) {
        }
        return X
    }

    function C(X) {
        return j.createElement(X)
    }

    function i(Z, X, Y) {
        Z.attachEvent(X, Y), I[I.length] = [Z, X, Y]
    }

    function F(Z) {
        var Y = M.pv,
            X = Z.split(".");
        return X[0] = parseInt(X[0], 10), X[1] = parseInt(X[1], 10) || 0, X[2] = parseInt(X[2], 10) || 0, Y[0] > X[0] || Y[0] == X[0] && Y[1] > X[1] || Y[0] == X[0] && Y[1] == X[1] && Y[2] >= X[2] ? !0 : !1
    }

    function v(ac, Y, ad, ab) {
        if (!M.ie || !M.mac) {
            var aa = j.getElementsByTagName("head")[0];
            if (aa) {
                var X = ad && "string" == typeof ad ? ad : "screen";
                if (ab && (n = null, G = null), !n || G != X) {
                    var Z = C("style");
                    Z.setAttribute("type", "text/css"), Z.setAttribute("media", X), n = aa.appendChild(Z), M.ie && M.win && typeof j.styleSheets != D && j.styleSheets.length > 0 && (n = j.styleSheets[j.styleSheets.length - 1]), G = X
                }
                M.ie && M.win ? n && typeof n.addRule == r && n.addRule(ac, Y) : n && typeof j.createTextNode != D && n.appendChild(j.createTextNode(ac + " {" + Y + "}"))
            }
        }
    }

    function w(Z, X) {
        if (m) {
            var Y = X ? "visible" : "hidden";
            J && c(Z) ? c(Z).style.visibility = Y : v("#" + Z, "visibility:" + Y)
        }
    }

    function L(Y) {
        var Z = /[\\\"<>\.;]/,
            X = null != Z.exec(Y);
        return X && typeof encodeURIComponent != D ? encodeURIComponent(Y) : Y
    }

    var l, Q, E, B, n, G, D = "undefined",
        r = "object",
        S = "Shockwave Flash",
        W = "ShockwaveFlash.ShockwaveFlash",
        q = "application/x-shockwave-flash",
        R = "SWFObjectExprInst",
        x = "onreadystatechange",
        O = window,
        j = document,
        t = navigator,
        T = !1,
        U = [h],
        o = [],
        N = [],
        I = [],
        J = !1,
        a = !1,
        m = !0,
        M = function () {
            var aa = typeof j.getElementById != D && typeof j.getElementsByTagName != D && typeof j.createElement != D,
                ah = t.userAgent.toLowerCase(),
                Y = t.platform.toLowerCase(),
                ae = Y ? /win/.test(Y) : /win/.test(ah),
                ac = Y ? /mac/.test(Y) : /mac/.test(ah),
                af = /webkit/.test(ah) ? parseFloat(ah.replace(/^.*webkit\/(\d+(\.\d+)?).*$/, "$1")) : !1,
                X = !1,
                ag = [0, 0, 0],
                ab = null;
            if (typeof t.plugins != D && typeof t.plugins[S] == r) ab = t.plugins[S].description, !ab || typeof t.mimeTypes != D && t.mimeTypes[q] && !t.mimeTypes[q].enabledPlugin || (T = !0, X = !1, ab = ab.replace(/^.*\s+(\S+\s+\S+$)/, "$1"), ag[0] = parseInt(ab.replace(/^(.*)\..*$/, "$1"), 10), ag[1] = parseInt(ab.replace(/^.*\.(.*)\s.*$/, "$1"), 10), ag[2] = /[a-zA-Z]/.test(ab) ? parseInt(ab.replace(/^.*[a-zA-Z]+(.*)$/, "$1"), 10) : 0);
            else if (typeof O.ActiveXObject != D) try {
                var ad = new ActiveXObject(W);
                ad && (ab = ad.GetVariable("$version"), ab && (X = !0, ab = ab.split(" ")[1].split(","), ag = [parseInt(ab[0], 10), parseInt(ab[1], 10), parseInt(ab[2], 10)]))
            } catch (Z) {
            }
            return {
                w3: aa,
                pv: ag,
                wk: af,
                ie: X,
                win: ae,
                mac: ac
            }
        }();
    return function () {
        M.w3 && ((typeof j.readyState != D && "complete" == j.readyState || typeof j.readyState == D && (j.getElementsByTagName("body")[0] || j.body)) && f(), J || (typeof j.addEventListener != D && j.addEventListener("DOMContentLoaded", f, !1), M.ie && M.win && (j.attachEvent(x, function () {
            "complete" == j.readyState && (j.detachEvent(x, arguments.callee), f())
        }), O == top &&
        function () {
            if (!J) {
                try {
                    j.documentElement.doScroll("left")
                } catch (X) {
                    return setTimeout(arguments.callee, 0), void 0
                }
                f()
            }
        }()), M.wk &&
        function () {
            return J ? void 0 : /loaded|complete/.test(j.readyState) ? (f(), void 0) : (setTimeout(arguments.callee, 0), void 0)
        }(), s(f)))
    }(), function () {
        M.ie && M.win && window.attachEvent("onunload", function () {
            for (var ac = I.length, ab = 0; ac > ab; ab++) I[ab][0].detachEvent(I[ab][1], I[ab][2]);
            for (var Z = N.length, aa = 0; Z > aa; aa++) y(N[aa]);
            for (var Y in M) M[Y] = null;
            M = null;
            for (var X in swfobject) swfobject[X] = null;
            swfobject = null
        })
    }(), {
        registerObject: function (ab, X, aa, Z) {
            if (M.w3 && ab && X) {
                var Y = {};
                Y.id = ab, Y.swfVersion = X, Y.expressInstall = aa, Y.callbackFn = Z, o[o.length] = Y, w(ab, !1)
            } else Z && Z({
                success: !1,
                id: ab
            })
        },
        getObjectById: function (X) {
            return M.w3 ? z(X) : void 0
        },
        embedSWF: function (ab, ah, ae, ag, Y, aa, Z, ad, af, ac) {
            var X = {
                success: !1,
                id: ah
            };
            M.w3 && !(M.wk && 312 > M.wk) && ab && ah && ae && ag && Y ? (w(ah, !1), K(function () {
                ae += "", ag += "";
                var aj = {};
                if (af && typeof af === r) for (var al in af) aj[al] = af[al];
                aj.data = ab, aj.width = ae, aj.height = ag;
                var am = {};
                if (ad && typeof ad === r) for (var ak in ad) am[ak] = ad[ak];
                if (Z && typeof Z === r) for (var ai in Z) typeof am.flashvars != D ? am.flashvars += "&" + ai + "=" + Z[ai] : am.flashvars = ai + "=" + Z[ai];
                if (F(Y)) {
                    var an = u(aj, am, ah);
                    aj.id == ah && w(ah, !0), X.success = !0, X.ref = an
                } else {
                    if (aa && A()) return aj.data = aa, P(aj, am, ah, ac), void 0;
                    w(ah, !0)
                }
                ac && ac(X)
            })) : ac && ac(X)
        },
        switchOffAutoHideShow: function () {
            m = !1
        },
        ua: M,
        getFlashPlayerVersion: function () {
            return {
                major: M.pv[0],
                minor: M.pv[1],
                release: M.pv[2]
            }
        },
        hasFlashPlayerVersion: F,
        createSWF: function (Z, Y, X) {
            return M.w3 ? u(Z, Y, X) : void 0
        },
        showExpressInstall: function (Z, aa, X, Y) {
            M.w3 && A() && P(Z, aa, X, Y)
        },
        removeSWF: function (X) {
            M.w3 && y(X)
        },
        createCSS: function (aa, Z, Y, X) {
            M.w3 && v(aa, Z, Y, X)
        },
        addDomLoadEvent: K,
        addLoadEvent: s,
        getQueryParamValue: function (aa) {
            var Z = j.location.search || j.location.hash;
            if (Z) {
                if (/\?/.test(Z) && (Z = Z.split("?")[1]), null == aa) return L(Z);
                for (var Y = Z.split("&"), X = 0; Y.length > X; X++) if (Y[X].substring(0, Y[X].indexOf("=")) == aa) return L(Y[X].substring(Y[X].indexOf("=") + 1))
            }
            return ""
        },
        expressInstallCallback: function () {
            if (a) {
                var X = c(R);
                X && l && (X.parentNode.replaceChild(l, X), Q && (w(Q, !0), M.ie && M.win && (l.style.display = "block")), E && E(B)), a = !1
            }
        }
    }
}(), swfobject.addDomLoadEvent(function () {
    "function" == typeof SWFUpload.onload && SWFUpload.onload.call(window)
});
var SWFUpload;
"function" == typeof SWFUpload && (SWFUpload.queue = {}, SWFUpload.prototype.initSettings = function (oldInitSettings) {
    return function (userSettings) {
        "function" == typeof oldInitSettings && oldInitSettings.call(this, userSettings), this.queueSettings = {}, this.queueSettings.queue_cancelled_flag = !1, this.queueSettings.queue_upload_count = 0, this.queueSettings.user_upload_complete_handler = this.settings.upload_complete_handler, this.queueSettings.user_upload_start_handler = this.settings.upload_start_handler, this.settings.upload_complete_handler = SWFUpload.queue.uploadCompleteHandler, this.settings.upload_start_handler = SWFUpload.queue.uploadStartHandler, this.settings.queue_complete_handler = userSettings.queue_complete_handler || null
    }
}(SWFUpload.prototype.initSettings), SWFUpload.prototype.startUpload = function (fileID) {
    this.queueSettings.queue_cancelled_flag = !1, this.callFlash("StartUpload", [fileID])
}, SWFUpload.prototype.cancelQueue = function () {
    this.queueSettings.queue_cancelled_flag = !0, this.stopUpload();
    for (var stats = this.getStats(); stats.files_queued > 0;) this.cancelUpload(), stats = this.getStats()
}, SWFUpload.queue.uploadStartHandler = function (file) {
    var returnValue;
    return "function" == typeof this.queueSettings.user_upload_start_handler && (returnValue = this.queueSettings.user_upload_start_handler.call(this, file)), returnValue = returnValue === !1 ? !1 : !0, this.queueSettings.queue_cancelled_flag = !returnValue, returnValue
}, SWFUpload.queue.uploadCompleteHandler = function (file) {
    var continueUpload, user_upload_complete_handler = this.queueSettings.user_upload_complete_handler;
    if (file.filestatus === SWFUpload.FILE_STATUS.COMPLETE && this.queueSettings.queue_upload_count++, continueUpload = "function" == typeof user_upload_complete_handler ? user_upload_complete_handler.call(this, file) === !1 ? !1 : !0 : file.filestatus === SWFUpload.FILE_STATUS.QUEUED ? !1 : !0) {
        var stats = this.getStats();
        stats.files_queued > 0 && this.queueSettings.queue_cancelled_flag === !1 ? this.startUpload() : this.queueSettings.queue_cancelled_flag === !1 ? (this.queueEvent("queue_complete_handler", [this.queueSettings.queue_upload_count]), this.queueSettings.queue_upload_count = 0) : (this.queueSettings.queue_cancelled_flag = !1, this.queueSettings.queue_upload_count = 0)
    }
}), FileProgress.prototype.setTimer = function (timer) {
    this.fileProgressElement.FP_TIMER = timer
}, FileProgress.prototype.getTimer = function () {
    return this.fileProgressElement.FP_TIMER || null
}, FileProgress.prototype.reset = function () {
    this.fileProgressElement.className = "progressContainer", this.fileProgressElement.childNodes[2].innerHTML = "&nbsp;", this.fileProgressElement.childNodes[2].className = "progressBarStatus", this.fileProgressElement.childNodes[3].className = "progressBarInProgress", this.fileProgressElement.childNodes[3].style.width = "0%", this.appear()
}, FileProgress.prototype.setProgress = function (percentage) {
    this.fileProgressElement.className = "progressContainer green", this.fileProgressElement.childNodes[3].className = "progressBarInProgress", this.fileProgressElement.childNodes[3].style.width = percentage + "%", this.appear()
}, FileProgress.prototype.setComplete = function () {
    this.fileProgressElement.className = "progressContainer blue", this.fileProgressElement.childNodes[3].className = "progressBarComplete", this.fileProgressElement.childNodes[3].style.width = "";
    var oSelf = this;
    this.setTimer(setTimeout(function () {
        oSelf.disappear()
    }, 1500))
}, FileProgress.prototype.setError = function () {
    this.fileProgressElement.className = "progressContainer red", this.fileProgressElement.childNodes[3].className = "progressBarError", this.fileProgressElement.childNodes[3].style.width = "";
    var oSelf = this;
    this.setTimer(setTimeout(function () {
        oSelf.disappear()
    }, 2500))
}, FileProgress.prototype.setCancelled = function () {
    this.fileProgressElement.className = "progressContainer", this.fileProgressElement.childNodes[3].className = "progressBarError", this.fileProgressElement.childNodes[3].style.width = "";
    var oSelf = this;
    this.setTimer(setTimeout(function () {
        oSelf.disappear()
    }, 2e3))
}, FileProgress.prototype.setStatus = function (status) {
    this.fileProgressElement.childNodes[2].innerHTML = status
}, FileProgress.prototype.toggleCancel = function (show, swfUploadInstance) {
    if (this.fileProgressElement.childNodes[0].style.visibility = show ? "visible" : "hidden", swfUploadInstance) {
        var fileID = this.fileProgressID;
        this.fileProgressElement.childNodes[0].onclick = function () {
            return swfUploadInstance.cancelUpload(fileID), !1
        }
    }
}, FileProgress.prototype.appear = function () {
    if (null !== this.getTimer() && (clearTimeout(this.getTimer()), this.setTimer(null)), this.fileProgressWrapper.filters) try {
        this.fileProgressWrapper.filters.item("DXImageTransform.Microsoft.Alpha").opacity = 100
    } catch (e) {
        this.fileProgressWrapper.style.filter = "progid:DXImageTransform.Microsoft.Alpha(opacity=100)"
    } else this.fileProgressWrapper.style.opacity = 1;
    this.fileProgressWrapper.style.height = "", this.height = this.fileProgressWrapper.offsetHeight, this.opacity = 100, this.fileProgressWrapper.style.display = ""
}, FileProgress.prototype.disappear = function () {
    var reduceOpacityBy = 15,
        reduceHeightBy = 4,
        rate = 30;
    if (this.opacity > 0) if (this.opacity -= reduceOpacityBy, 0 > this.opacity && (this.opacity = 0), this.fileProgressWrapper.filters) try {
        this.fileProgressWrapper.filters.item("DXImageTransform.Microsoft.Alpha").opacity = this.opacity
    } catch (e) {
        this.fileProgressWrapper.style.filter = "progid:DXImageTransform.Microsoft.Alpha(opacity=" + this.opacity + ")"
    } else this.fileProgressWrapper.style.opacity = this.opacity / 100;
    if (this.height > 0 && (this.height -= reduceHeightBy, 0 > this.height && (this.height = 0), this.fileProgressWrapper.style.height = this.height + "px"), this.height > 0 || this.opacity > 0) {
        var oSelf = this;
        this.setTimer(setTimeout(function () {
            oSelf.disappear()
        }, rate))
    } else this.fileProgressWrapper.style.display = "none", this.setTimer(null)
}, function ($) {
    $.fn.lightBox = function (settings) {
        function _initialize() {
            return _start(this, jQueryMatchedObj), !1
        }

        function _start(objClicked, jQueryMatchedObj) {
            if ($("embed, object, select").css({
                    visibility: "hidden"
                }), _set_interface(), settings.imageArray.length = 0, settings.activeImage = 0, 1 == jQueryMatchedObj.length) settings.imageArray.push([objClicked.getAttribute("href"), objClicked.getAttribute("title")]);
            else for (var i = 0; jQueryMatchedObj.length > i; i++) settings.imageArray.push([jQueryMatchedObj[i].getAttribute("href"), jQueryMatchedObj[i].getAttribute("title")]);
            for (; settings.imageArray[settings.activeImage][0] != objClicked.getAttribute("href");) settings.activeImage++;
            _set_image_to_view(), $("#lightbox-nav").attr("title", "快捷键：上一张" + settings.keyToPrev + "，下一张" + settings.keyToNext + "，关闭" + settings.keyToClose), $(window).trigger("jquery.lightboxClicked")
        }

        function _set_interface() {
            $("body").append('<iframe id="jquery-overlay" name="overlay"></iframe><div id="jquery-overlay-div"></div><div id="jquery-lightbox" title="快捷键：上一张p，下一张n，关闭c"><a href="#" class="im-pop-close" id="lightbox-secNav-btnClose" title="关闭"></a><div class="im-pop-view-pic" id="lightboxWrap"><div class="pic-trigger"><a href="javascript:;" id="seeFull" class="pic-zoom" title="放大图片"></a></div><div id="lightbox-container-image-box"><div id="lightbox-container-image"><img id="lightbox-image"><div id="lightbox-loading"><a href="#" id="lightbox-loading-link"><img src="' + settings.imageLoading + '"></a></div></div></div><div id="lightbox-container-image-data-box"></div><a href="#" class="im-slide-prev im-icon-slide-prev" title="前一个" id="lightbox-nav-btnPrev"></a><a href="#" class="im-slide-next im-icon-slide-next" title="后一个" id="lightbox-nav-btnNext"></a>' + "</div>");
            var arrPageSizes = ___getPageSize();
            $("#jquery-overlay-div").css({
                position: "absolute",
                top: "0px",
                left: "0px",
                zIndex: 99,
                width: arrPageSizes[0],
                height: arrPageSizes[1]
            }), $("#jquery-overlay").css({
                backgroundColor: "transparent",
                opacity: settings.overlayOpacity,
                width: arrPageSizes[0],
                height: arrPageSizes[1]
            }).fadeIn(function () {
                seefullfigureFlag = !1;
                try {
                    $.browser.msie ? $("#lightboxWrap").css({
                        width: "100%"
                    }) : $("#lightboxWrap").css({
                        width: "100%",
                        "background-color": "rgba(0,0,0,.2)"
                    })
                } catch (e) {
                    $("#lightboxWrap").css({
                        width: "100%",
                        "background-color": "rgba(0,0,0,.2)"
                    })
                }
            });
            var arrPageScroll = ___getPageScroll();
            try {
                $.browser.msie ? $("#lightboxWrap").css({
                    width: "100%"
                }) : $("#lightboxWrap").css({
                    width: "100%",
                    "background-color": "rgba(0,0,0,.2)"
                })
            } catch (e) {
                $("#lightboxWrap").css({
                    width: "100%",
                    "background-color": "rgba(0,0,0,.2)"
                })
            }
            setImgWithFlag = !1, $("#seeFull").unbind("click").bind("click", _seefullfigure), defaultTop = arrPageScroll[1] + arrPageSizes[3] / 10, $("#jquery-lightbox").css({
                top: arrPageScroll[1] + arrPageSizes[3] / 10,
                left: "10%",
                width: "80%"
            }).show(), $("#jquery-overlay-div").click(function () {
                _finish()
            }), $("#lightbox-loading-link,#lightbox-secNav-btnClose").click(function () {
                return _finish(), !1
            }), $(window).resize(function () {
                var arrPageSizes = ___getPageSize();
                $("#jquery-overlay").css({
                    width: arrPageSizes[0],
                    height: arrPageSizes[1]
                }), $("#jquery-overlay-div").css({
                    position: "absolute",
                    top: "0px",
                    left: "0px",
                    zIndex: 9999,
                    width: arrPageSizes[0],
                    height: arrPageSizes[1]
                });
                var arrPageScroll = ___getPageScroll();
                defaultTop = arrPageScroll[1] + arrPageSizes[3] / 10, $("#jquery-lightbox").css({
                    top: arrPageScroll[1] + arrPageSizes[3] / 10,
                    left: "10%"
                });
                try {
                    $.browser.msie ? $("#lightboxWrap").css({
                        width: "100%"
                    }) : $("#lightboxWrap").css({
                        width: "100%",
                        "background-color": "rgba(0,0,0,.2)"
                    })
                } catch (e) {
                    $("#lightboxWrap").css({
                        width: "100%",
                        "background-color": "rgba(0,0,0,.2)"
                    })
                }
            })
        }

        function _set_image_to_view() {
            $("#lightbox-loading").show(), settings.fixedNavigation ? $("#lightbox-image,#lightbox-container-image-data-box,#lightbox-image-details-currentNumber").hide() : $("#lightbox-image,#lightbox-nav,#lightbox-nav-btnPrev,#lightbox-nav-btnNext,#lightbox-container-image-data-box,#lightbox-image-details-currentNumber").hide();
            var objImagePreloader = new Image;
            objImagePreloader.onload = function () {
                $("#lightbox-image").attr("src", settings.imageArray[settings.activeImage][0]), _resize_container_image_box(objImagePreloader.width, objImagePreloader.height), objImagePreloader.onload = function () {
                }
            }, objImagePreloader.src = settings.imageArray[settings.activeImage][0]
        }

        function _resize_container_image_box(intImageWidth, intImageHeight) {
            setImgWithFlag = !1, (intImageWidth > $(document).width() || intImageHeight > $(document).height()) && _seefullfigure(), intImageWidth > $(document).width() && (intImageWidth = $(document).width() - 200), intImageHeight > $(document).height() - 150 && (intImageHeight = $(document).height() - 150);
            var intCurrentWidth = $("#lightbox-container-image-box").width(),
                intCurrentHeight = $("#lightbox-container-image-box").height(),
                intWidth = intImageWidth + 2 * settings.containerBorderSize,
                intHeight = intImageHeight + 2 * settings.containerBorderSize,
                intDiffW = intCurrentWidth - intWidth,
                intDiffH = intCurrentHeight - intHeight;
            $("#lightbox-container-image-box").animate({
                width: "90%",
                height: intHeight
            }, settings.containerResizeSpeed, function () {
                _show_image(intWidth)
            }), 0 == intDiffW && 0 == intDiffH && ($.browser.msie ? ___pause(250) : ___pause(100)), $("#lightbox-container-image-data-box").css({
                width: intImageWidth
            })
        }

        function _show_image(intWidth) {
            $("#lightbox-loading").hide(), $("#lightbox-image").fadeIn(function () {
                _show_image_data(), _set_navigation()
            }), _preload_neighbor_images(), intWidth > $("#lightbox-container-image-box").width() && (setImgWithFlag = !0), setImgWithFlag ? $("#lightbox-image").css("width", "100%") : ($("#lightbox-image").removeAttr("style"), $("#lightbox-image").css("display", "inline"))
        }

        function _show_image_data() {
            $("#lightbox-container-image-data-box").slideDown("fast"), $("#lightbox-image-details-caption").hide(), settings.imageArray[settings.activeImage][1], settings.imageArray.length > 1
        }

        function _set_navigation() {
            $("#lightbox-nav").show(), 0 != settings.activeImage && (settings.fixedNavigation ? $("#lightbox-nav-btnPrev").unbind().bind("click", function () {
                return settings.activeImage = settings.activeImage - 1, _set_image_to_view(), !1
            }) : $("#lightbox-nav-btnPrev").unbind().show().bind("click", function () {
                return settings.activeImage = settings.activeImage - 1, _set_image_to_view(), !1
            })), settings.activeImage != settings.imageArray.length - 1 && (settings.fixedNavigation ? $("#lightbox-nav-btnNext").unbind().bind("click", function () {
                return settings.activeImage = settings.activeImage + 1, _set_image_to_view(), !1
            }) : $("#lightbox-nav-btnNext").unbind().show().bind("click", function () {
                return settings.activeImage = settings.activeImage + 1, _set_image_to_view(), !1
            })), _enable_keyboard_navigation()
        }

        function _enable_keyboard_navigation() {
            $(document).keydown(function (objEvent) {
                _keyboard_action(objEvent)
            })
        }

        function _disable_keyboard_navigation() {
            $(document).unbind()
        }

        function _keyboard_action(objEvent) {
            null == objEvent ? (keycode = event.keyCode, escapeKey = 27) : (keycode = objEvent.keyCode, escapeKey = objEvent.DOM_VK_ESCAPE), key = String.fromCharCode(keycode).toLowerCase(), (key == settings.keyToClose || "x" == key || keycode == escapeKey) && _finish(), (key == settings.keyToPrev || 37 == keycode) && 0 != settings.activeImage && (settings.activeImage = settings.activeImage - 1, _set_image_to_view(), _disable_keyboard_navigation()), (key == settings.keyToNext || 39 == keycode) && settings.activeImage != settings.imageArray.length - 1 && (settings.activeImage = settings.activeImage + 1, _set_image_to_view(), _disable_keyboard_navigation())
        }

        function _preload_neighbor_images() {
            settings.imageArray.length - 1 > settings.activeImage && (objNext = new Image, objNext.src = settings.imageArray[settings.activeImage + 1][0]), settings.activeImage > 0 && (objPrev = new Image, objPrev.src = settings.imageArray[settings.activeImage - 1][0])
        }

        function _finish() {
            $("#jquery-lightbox").remove(), $("#jquery-overlay-div").remove(), $("#jquery-overlay").fadeOut(function () {
                $("#jquery-overlay").remove()
            }), $("embed, object, select").css({
                visibility: "visible"
            }), $(window).trigger("jquery.lightboxClosed")
        }

        function ___getPageSize() {
            var xScroll, yScroll;
            window.innerHeight && window.scrollMaxY ? (xScroll = window.innerWidth + window.scrollMaxX, yScroll = window.innerHeight + window.scrollMaxY) : document.body.scrollHeight > document.body.offsetHeight ? (xScroll = document.body.scrollWidth, yScroll = document.body.scrollHeight) : (xScroll = document.body.offsetWidth, yScroll = document.body.offsetHeight);
            var windowWidth, windowHeight;
            return self.innerHeight ? (windowWidth = document.documentElement.clientWidth ? document.documentElement.clientWidth : self.innerWidth, windowHeight = self.innerHeight) : document.documentElement && document.documentElement.clientHeight ? (windowWidth = document.documentElement.clientWidth, windowHeight = document.documentElement.clientHeight) : document.body && (windowWidth = document.body.clientWidth, windowHeight = document.body.clientHeight), pageHeight = windowHeight > yScroll ? windowHeight : yScroll, pageWidth = windowWidth > xScroll ? xScroll : windowWidth, arrayPageSize = [pageWidth, pageHeight, windowWidth, windowHeight]
        }

        function ___getPageScroll() {
            var xScroll, yScroll;
            return self.pageYOffset ? (yScroll = self.pageYOffset, xScroll = self.pageXOffset) : document.documentElement && document.documentElement.scrollTop ? (yScroll = document.documentElement.scrollTop, xScroll = document.documentElement.scrollLeft) : document.body && (yScroll = document.body.scrollTop, xScroll = document.body.scrollLeft), arrayPageScroll = [xScroll, yScroll]
        }

        function ___pause(ms) {
            var date = new Date;
            curDate = null;
            do
                var curDate = new Date;
            while (ms > curDate - date)
        }

        function _seefullfigure() {
            seefullfigureFlag = !0, $("#jquery-lightbox").removeAttr("style");
            try {
                $.browser.msie && 8 > $.browser.version && window.setTimeout(function () {
                    $("#jquery-lightbox").css({
                        top: "0px",
                        left: "0px"
                    })
                }, 0)
            } catch (e) {
            }
            return $("#lightboxWrap").css("height", "100%"), $("#jquery-lightbox").css("height", "100%"), $("#seeFull").removeClass("pic-zoom").addClass("back-thumbnail"), setImgWithFlag && ($("#lightbox-image").removeAttr("style"), $("#lightbox-image").css("display", "inline")), $("#seeFull").unbind("click").bind("click", restoreScreen), $("#seeFull").attr("title", "缩小图片"), !1
        }

        function restoreScreen() {
            return seefullfigureFlag = !1, $("#jquery-lightbox").removeAttr("style"), $("#jquery-lightbox").css({
                top: defaultTop,
                left: "10%",
                width: "80%"
            }), $("#seeFull").addClass("pic-zoom").removeClass("back-thumbnail"), $("#lightbox-image").removeAttr("style"), setImgWithFlag ? $("#lightbox-image").css({
                display: "inline",
                width: "100%"
            }) : ($("#lightbox-image").removeAttr("style"), $("#lightbox-image").css("display", "inline")), $("#seeFull").unbind("click").bind("click", _seefullfigure), $("#seeFull").attr("title", "放大图片"), !1
        }

        settings = jQuery.extend({
            overlayBgColor: "#000",
            overlayOpacity: 0,
            fixedNavigation: !1,
            imageLoading: "/img/lightbox-ico-loading.gif",
            imageBtnPrev: "http://static.360buyimg.com/im/img/i/20130514A.png",
            imageBtnNext: "http://static.360buyimg.com/im/img/i/20130514A.png",
            imageBtnClose: "/img/lightbox-btn-close.gif",
            imageBlank: "/img/lightbox-blank.gif",
            containerBorderSize: 10,
            containerResizeSpeed: 400,
            txtImage: "Image",
            txtOf: "of",
            keyToClose: "c",
            keyToPrev: "p",
            keyToNext: "n",
            imageArray: [],
            activeImage: 0
        }, settings);
        var jQueryMatchedObj = this,
            seefullfigureFlag = !1,
            defaultTop = 0,
            setImgWithFlag = !1;
        return this.unbind("click").click(_initialize)
    }
}(jQuery), $(function () {
    $("#service_c").jdchat({
        pid: pid,
        imgUrl: imgUrl,
        advertiseWord: advertiseWord,
        wname: wname,
        price: price,
        stock: stock,
        score: score,
        commentNum: commentNum,
        pin: pin,
        pamsUid: pamsUid,
        evaluationRate: evaluationRate,
        msgElves: msgElves,
        lastwaiter: lastwaiter,
        wid: wid,
        oid: oid
    })
});