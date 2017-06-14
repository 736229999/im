/*
 *zhushou360 Custom Protocol detect & support
 */
(function (lnkcfg) {
    var hostDomain = 'http://zhushou.360.cn';
    var dt = dt || {};
    var it = it || {};

    it.b = {};//browsers
    it.e = {};//events
    it.d = {};//doms

//Cookie
    it.cookie = {
        set: function (name, value, days) {
            var expire = "";
            if (days != null) {
                expire = new Date((new Date()).getTime() + days * 24 * 3600000);
                expire = "; expires=" + expire.toGMTString();
            }
            document.cookie = name + "=" + escape(value) + expire;
        },
        get: function (name) {
            var cookieValue = "";
            var search = name + "=";
            if (document.cookie.length > 0) {
                offset = document.cookie.indexOf(search);
                if (offset != -1) {
                    offset += search.length;
                    end = document.cookie.indexOf(";", offset);
                    if (end == -1) end = document.cookie.length;
                    cookieValue = unescape(document.cookie.substring(offset, end))
                }
            }
            return cookieValue;
        },
        del: function (name) {
            var date = new Date();
            date.setTime(date.getTime() - 1000);
            var cookieStr = escape(name) + "=; expires=" + date.toGMTString() + ";";
            document.cookie = cookieStr;
        }
    };


    it.d.gId = function (oID) {
        var node = typeof oID == "string" ? document.getElementById(oID) : oID;
        if (node != null) {
            return node;
        }
        return null;
    };

    it.d.gTag = function (oTag, oId) {
        if (!oId) {
            return document.getElementsByTagName(oTag);
        } else {
            return it.d.gId(oId).getElementsByTagName(oTag);
        }
    };

    it.d.cTag = function (tagName) {
        return document.createElement(tagName);
    };

    it.e.addEventListener = function (b, a, c) {
        if (b.addEventListener) {
            b.addEventListener(a, c, false)
        } else {
            if (b.attachEvent) {
                b.attachEvent("on" + a, function (d) {
                    c.call(b, d)
                })
            }
        }
    };

    it.e.stopDefault = function (e) {
        if (e && e.preventDefault) {
            e.preventDefault();
        } else {
            window.event.returnValue = false;
        }
        return false;
    };

    dt.bs = function () {
        if (( typeof (dt.bs.browser) === "undefined") || !(dt.bs.browser)) {
            var h = navigator;
            var a = h.userAgent;
            var f = h.appVersion;
            var d = parseFloat(f);
            var c = {};
            c.isOpera = (a.indexOf("Opera") >= 0) ? d : undefined;
            c.isKhtml = (f.indexOf("Konqueror") >= 0) ? d : undefined;
            c.isWebKit = parseFloat(a.split("WebKit/")[1]) || undefined;
            c.isChrome = parseFloat(a.split("Chrome/")[1]) || undefined;
            c.isFirefox = (/Firefox[\/\s](\d+\.\d+)/.test(a));
            var b = Math.max(f.indexOf("WebKit"), f.indexOf("Safari"), 0);
            if (b && !c.isChrome) {
                c.isSafari = parseFloat(f.split("Version/")[1]);
                if (!c.isSafari || parseFloat(f.substr(b + 7)) <= 419.3) {
                    c.isSafari = 2
                }
            }
            if (document.all && !c.isOpera) {
                c.isIE = parseFloat(f.split("MSIE ")[1]) || undefined;
                var e = new RegExp("MSIE ([0-9]{1,}[.0-9]{0,})");
                var g = -1;
                if (e.exec(a) != null) {
                    g = parseFloat(RegExp.$1)
                }
                c.lteIE6 = (g <= 6);
                c.isIE7 = (g >= 7 && g < 8);
                c.isIE8 = (g >= 8 && g < 9);
                c.ltIE9 = (g < 9);
            }
            dt.bs.browser = c
        }
        return dt.bs.browser
    };
    it.b.isIE = function () {
        return dt.bs().isIE
    };
    it.b.ltIE9 = function () {
        return dt.bs().ltIE9;
    };
    it.b.lteIE6 = function () {
        return dt.bs().lteIE6;
    };
    it.b.isOpera = function () {
        return dt.bs().isOpera
    };
    it.b.isChrome = function () {
        return dt.bs().isChrome
    };
    it.b.isSafari = function () {
        return dt.bs().isSafari
    };
    it.b.isFirefox = function () {
        return dt.bs().isFirefox
    };

    if (it.b.lteIE6()) {
        try {
            document.execCommand("BackgroundImageCache", false, true);
        } catch (e) {
        }
        ;
    }
    ;

    dt.plugDetect = function (cfg) {

        return ((it.b.isFirefox() || it.b.isChrome()) && dt.mozPluged(cfg.pluginfo.ffplugname));

    };

    dt.detectPlugInstalled = function (cfg) {
        if (dt.plugDetect(cfg)) {
            dt.storePlugInstalled(cfg.pluginfo.plugname);
        }
        ;
    };

    dt.storePlugInstalled = function (plugname) {
        it.cookie.set(plugname + 'Present', '1', 365);
    };

    dt.storeTipsDisplayed = function () {
        it.cookie.set('zspDisplayed', '1', 1);
    };

    dt.getTipsDisplayed = function () {
        return false;
        return it.cookie.get('zspDisplayed');
    };

    dt.mozPluged = function (plugname) {
        var a = false;
        if (navigator.plugins && navigator.plugins.length > 0) {
            for (var b = 0; b < navigator.plugins.length; b++) {
                var c = navigator.plugins[b];
                var d = c.name;
                if (new RegExp(plugname, 'i').test(d)) {
                    a = true
                }
            }
        }
        return a
    };

    dt.ffPluged = function (plugname) {
        var a = false;
        try {
            var mimetype = navigator.mimeTypes["application/" + plugname];
            if (mimetype) {
                a = true;
            } else {
                a = false;
            }
        }
        catch (e) {
            a = false;
        }
        return a;
    };

    dt.preparePopStyle = function () {

        if (it.d.gId('zsp_css')) return;
        var url = hostDomain + '/pop/style/pop_0411.css';
        var css = document.createElement('link');
        css.rel = 'stylesheet';
        css.href = url;
        css.id = 'zsp_css';
        css.media = 'all';
        css.type = 'text/css';
        it.d.gTag('head')[0].appendChild(css);
    };

    dt.preparePopDom = function (appdown) {
        if (it.d.gId('zsp_cover')) return;
        var appLink = '', iecls = '', downpage = 'href="http://www.360.cn/shoujizhushou/index.html" target="360zhushouOFS"', closeFn = 'document.getElementById(\'zsp_pop\').closeFn(); ';
        if (it.b.isIE()) {
            iecls = ' zsp_popIE';
        }
        ;
        if (appdown) {
            appLink = '<a href="#nogo" onclick="' + closeFn + 'return false;" class="zsp_btnApp" id="zsp_btnApp">\u4E0B\u8F7D\u8BE5\u8F6F\u4EF6\u5230\u7535\u8111<span>&gt;&gt;</span></a>';
        }
        ;
        var tmp = [
            '<div id="zsp_con" class="zsp_con">',
            '<h2>\u6B63\u5728\u6253\u5F00360\u624B\u673A\u52A9\u624B\u4E0B\u8F7D\u5E76\u5B89\u88C5<span id="zsp_sname">\u60A8\u9009\u62E9\u7684\u5E94\u7528</span><span class="zsp_show_for_ie">\u5230\u60A8\u7684\u624B\u673A</span></h2>',
            '<h3>\u5982\u679C\u5E94\u7528\u4E0B\u8F7D\u6CA1\u6709\u5F00\u59CB\uFF0C<span class="zsp_hide_for_ie">\u60A8\u53EF\u4EE5\uFF1A</span><span class="zsp_show_for_ie">\u8BF7\u70B9\u51FB <a ' + downpage + ' onclick="' + closeFn + 'return true;">\u5B89\u88C5360\u624B\u673A\u52A9\u624B<span>&gt;&gt;</span></a></span></h3>',
            '<ul>',
            '<li>\u68C0\u67E5\u662F\u5426\u5B89\u88C5\u4E86360\u624B\u673A\u52A9\u624B\uFF08\u5305\u542B\u5728\u5B89\u5168\u536B\u58EB\u7684\u529F\u80FD\u5927\u5168\u4E2D\uFF09</li>',
            '<li>\u5F00\u542F360\u624B\u673A\u52A9\u624B\u7684\u6D4F\u89C8\u5668\u652F\u6301\u9009\u9879\uFF0C\u5982\u4E0B\u56FE\u6240\u793A\uFF1A</li>',
            '</ul>',
            '<p class="zsp_btnCon">',
            '<span class="zsp_btnKnow" title="\u6211\u77E5\u9053\u4E86" onclick="' + closeFn + 'return false;">&nbsp;</span>',
            '<span class="zsp_show_for_ie zsp_ie_timer">\u8BE5\u63D0\u793A<em id="zsp_cls_counter">10</em>\u79D2\u540E\u81EA\u52A8\u5173\u95ED</span>',
            '<a class="zsp_btnInst" ' + downpage + ' onclick="' + closeFn + 'return true;">\u5B89\u88C5360\u624B\u673A\u52A9\u624B<span>&gt;&gt;</span></a>',
            appLink,
            '</p>',
            '<a id="zsp_cls" class="zsp_cls" href="#nogo" onclick="' + closeFn + '"></a>',
            '</div>'
        ];


        var frg = document.createDocumentFragment();
        var mask = it.d.cTag('div');
        mask.className = 'zsp_cover';
        mask.style.display = 'none';
        mask.id = 'zsp_cover';

        var popc = it.d.cTag('div');
        popc.className = 'zsp_pop zsp_bg_load ' + iecls;
        //popc.style.display = 'none';
        popc.id = 'zsp_pop';
        popc.innerHTML = tmp.join('');
        frg.appendChild(mask);
        frg.appendChild(popc);
        it.d.gTag('body')[0].appendChild(frg);
    };

    dt.createLinkRouter = function (win, zslink) {
        var ifm = document.createElement('iframe');
        ifm.id = 'zsp_LinkRouter';
        ifm.src = hostDomain + '/zslinkproxy.html?zslink=' + encodeURIComponent(zslink);
        win.appendChild(ifm);
    };


    dt.closeDefaultTips = function (rec) {
        var mask = it.d.gId('zsp_cover'),
            popc = it.d.gId('zsp_pop'),
            router = document.getElementById('zsp_LinkRouter');
        mask.style.display = 'none';
        popc.style.display = 'none';
        if (router) {
            dt.clearIframe(router);
        }
        ;
        dt.storeTipsDisplayed();
    };

    dt.clearIframe = function (f) {
        try {
            f.contentWindow.document.write('');
            f.contentWindow.close();
            f.parentNode.removeChild(f);
        } catch (e) {
        }
    };

    dt.helperDefaultTips = function (cfg, downloading) {

        if (!it.d.gId('zsp_css')) {
            dt.preparePopStyle();
        }
        ;

        if (!it.d.gId('zsp_cover')) {
            dt.preparePopDom();
        }
        ;

        var isIE = it.b.isIE(),
            softurl = cfg.curUrl.split('url=')[1],
            softname = decodeURIComponent(cfg.curUrl.split('name=')[1].split('&')[0]);
        var disp = it.d.gId('zsp_sname'),
            displen = isIE ? 10 : 16;
        disp.title = softname,
            router = document.getElementById('zsp_LinkRouter');
        softname = (function (str, len) {

            var retlen = 0,
                strlen = str.length,
                retstr = '';
            if (!strlen) {
                return "\u60A8\u9009\u62E9\u7684\u5E94\u7528";
            }
            ;
            for (var i = 0; i < strlen; i++) {
                var a = str.charAt(i);
                retlen++;
                if (escape(a).length > 4) {
                    retlen++;
                }
                retstr = retstr.concat(a);
                if (retlen >= len) {
                    retstr = retstr.concat("...");
                    return '"' + retstr + '"';
                }
            }
            if (retlen <= len) {
                return '"' + str + '"';
            }
            ;
        })(softname, displen);

        disp.innerHTML = softname;

        var mask = it.d.gId('zsp_cover');
        mask.style.height = document.body.scrollHeight + 'px';
        var popc = it.d.gId('zsp_pop');
        popc.className = popc.className.replace(' zsp_bg_load', '');

        if (isIE) {
            var ctr = it.d.gId('zsp_cls_counter'),
                lmt = 7;
            if (ctr.timer) {
                clearInterval(ctr.timer);
                ctr.timer = null;
            }
            ;
            it.d.gId('zsp_cls_counter').innerHTML = lmt;
            ctr.timer = window.setInterval(function () {
                ctr.innerHTML = --lmt;
                if (lmt <= -1) {
                    dt.closeDefaultTips();
                    clearInterval(ctr.timer);
                    ctr.timer = null;
                }
                ;
            }, 1E3);
        }
        ;

        dt.createLinkRouter(popc, cfg.curUrl);

        mask.style.display = 'block';
        popc.style.display = 'block';

        popc.closeFn = dt.closeDefaultTips;

        return false;
    };

    dt.isHelper = function (o) {
        return true;
        //return (o.protocol.toLowerCase() == 'zhushou360');
    };

    it.linkRec = function (c, t, u) {
        var tp = t || 'zyl';
        c.__monitor_imgs = {};
        var b = '' + +(new Date), a = 'http://s.360.cn/zhushou/web.html?type=' + tp + '&action=open&ref=' + encodeURIComponent(window.location.hostname) + (u ? '&url=' + u : '') + '&rdm=' + b, d = c.__monitor_imgs[b] = new Image;
        d.onload = d.onerror = function () {
            c.__monitor_imgs[b] = null, delete c.__monitor_imgs[b]
        }, d.src = a;
    };

    function setCustomProtocol(cfg) {
        if (typeof cfg != 'object') return;

        var plugDetected = dt.plugDetect(cfg);
        var plugname = cfg.pluginfo.plugname;
        dt.detectPlugInstalled(cfg);

        it.e.addEventListener(window, 'load', function () {
            if (!plugDetected /*&& !dt.getTipsDisplayed()*/) {
                dt.preparePopStyle();
                dt.preparePopDom();
            }
            ;
        });

        it.e.addEventListener(document, 'keyup', function (e) {
            if (e.keyCode == 27) {
                try {
                    dt.closeDefaultTips();
                } catch (e) {
                }
            }
        });

        it.e.addEventListener(document, 'click', function (e) {
            e = e || window.event;
            var eo = e.srcElement ? e.srcElement : e.target, isA = false;

            if (eo.tagName == 'A') {
                isA = true;
            } else {
                while (eo.parentNode && eo.parentNode.tagName) {
                    eo = eo.parentNode;
                    if (eo.tagName == 'A') {
                        isA = true;
                        break;
                    }
                }
            }
            ;

            var reg = new RegExp(cfg.protocol, 'i');
            if (isA) {
                if (eo.id == 'zs_so_dbtn' || /zs_apk_dbtn/.test(eo.className)) {
                    it.linkRec(cfg, 'apk', eo.href || '');
                }
                ;
            }
            ;
            if (isA && reg.test(eo.href)) {
                plugDetected = dt.plugDetect(cfg);

                it.linkRec(cfg);

                var ele = eo,
                    url = eo.href;

                cfg.curUrl = url;
                cfg.curButton = ele;

                function callUpHelper() {
                    if (it.b.lteIE6()) {
                        var lnkifm = it.d.gId('zsp_LinkRouter');
                        if (!lnkifm) {
                            lnkifm = it.d.cTag('iframe');
                            lnkifm.id = 'zsp_LinkRouter';
                            lnkifm.name = 'zsp_LinkRouter';
                            it.d.gTag('body')[0].appendChild(lnkifm);
                        }
                        ;
                        lnkifm.src = cfg.curUrl;
                    } else {
                        setTimeout(function () {
                            try {
                                window.location = cfg.curUrl;
                            } catch (e) {
                                return true;
                            }
                            ;
                        }, 100);
                    }
                    ;
                };

                if (plugDetected) {

                    window.location = url;

                } else {

                    dt.helperDefaultTips(cfg);
                    if (dt.getTipsDisplayed()) {
                        callUpHelper();
                    } else {
                        dt.helperDefaultTips(cfg);
                        callUpHelper();
                    }//end of if closed

                }
                ;

                it.e.stopDefault(e);
            }
            ;
        });
    };

    setCustomProtocol(lnkcfg);
})({
    pluginfo: {
        axname: 'C360MMPlugIn.Manager',
        plugname: '360mmplugin',
        ffplugname: '360MMPlugInForFF'
    },
    protocol: 'zhushou360'
});
