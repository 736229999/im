(function(pb, ra) {
    if (pb.top == pb && "undefined" == typeof pb.ruyitao_already_running && (pb.ruyitao_already_running = !0, !(/msie\s+([\w.]+)/.exec(pb.navigator.userAgent.toLowerCase()) && "BackCompat" == document.compatMode))) {
        var K = {},
            f, $a, Ea = K;
        Ea.loadJQuery = function() {
            if (!window.jQuery || "1.4.2" > window.jQuery.fn.jquery) {
                if (!Ea.jQuery) {
                    var a = K,
                        b = window,
                        c = void 0,
                        d = function(k, a, b) {
                            if (b === c && 1 === k.nodeType) if (b = "data-" + a.replace(Za, "-$1").toLowerCase(), b = k.getAttribute(b), "string" == typeof b) {
                                try {
                                    b = "true" === b ? !0 : "false" === b ? !1 : "null" === b ? null : g.isNumeric(b) ? parseFloat(b) : jd.test(b) ? g.parseJSON(b) : b
                                } catch (ab) {}
                                g.data(k, a, b)
                            } else b = c;
                            return b
                        },
                        e = function(k) {
                            for (var a in k) if (!("data" === a && g.isEmptyObject(k[a])) && "toJSON" !== a) return !1;
                            return !0
                        },
                        j = function(k, a, c) {
                            var b = a + "defer",
                                d = a + "queue",
                                e = a + "mark",
                                f = g._data(k, b);
                            f && ("queue" === c || !g._data(k, d)) && ("mark" === c || !g._data(k, e)) && setTimeout(function() {
                                !g._data(k, d) && !g._data(k, e) && (g.removeData(k, b, !0), f.fire())
                            }, 0)
                        },
                        m = function() {
                            return !1
                        },
                        r = function() {
                            return !0
                        },
                        x = function(k, a, c) {
                            a = a || 0;
                            if (g.isFunction(a)) return g.grep(k, function(k, b) {
                                return !!a.call(k, b, k) === c
                            });
                            if (a.nodeType) return g.grep(k, function(k) {
                                return k === a === c
                            });
                            if ("string" == typeof a) {
                                var b = g.grep(k, function(k) {
                                    return 1 === k.nodeType
                                });
                                if (yb.test(a)) return g.filter(a, b, !c);
                                a = g.filter(a, b)
                            }
                            return g.grep(k, function(k) {
                                return 0 <= g.inArray(k, a) === c
                            })
                        },
                        p = function(k) {
                            var a = kb.split("|"),
                                k = k.createDocumentFragment();
                            if (k.createElement) for (; a.length;) k.createElement(a.pop());
                            return k
                        },
                        A = function(k, a) {
                            if (1 === a.nodeType && g.hasData(k)) {
                                var c, b, d;
                                b = g._data(k);
                                var e = g._data(a, b),
                                    f = b.events;
                                if (f) for (c in delete e.handle, e.events = {}, f) {
                                    b = 0;
                                    for (d = f[c].length; b < d; b++) g.event.add(a, c + (f[c][b].namespace ? "." : "") + f[c][b].namespace, f[c][b], f[c][b].data)
                                }
                                e.data && (e.data = g.extend({}, e.data))
                            }
                        },
                        sa = function(k, a) {
                            var c;
                            if (1 === a.nodeType) {
                                a.clearAttributes && a.clearAttributes();
                                a.mergeAttributes && a.mergeAttributes(k);
                                c = a.nodeName.toLowerCase();
                                if ("object" === c) a.outerHTML = k.outerHTML;
                                else if ("input" !== c || "checkbox" !== k.type && "radio" !== k.type) if ("option" === c) a.selected = k.defaultSelected;
                                else {
                                    if ("input" === c || "textarea" === c) a.defaultValue = k.defaultValue
                                } else k.checked && (a.defaultChecked = a.checked = k.checked), a.value !== k.value && (a.value = k.value);
                                a.removeAttribute(g.expando)
                            }
                        },
                        C = function(k) {
                            return "undefined" != typeof k.getElementsByTagName ? k.getElementsByTagName("*") : "undefined" != typeof k.querySelectorAll ? k.querySelectorAll("*") : []
                        },
                        F = function(k) {
                            if ("checkbox" === k.type || "radio" === k.type) k.defaultChecked = k.checked
                        },
                        E = function(k) {
                            var a = (k.nodeName || "").toLowerCase();
                            "input" === a ? F(k) : "script" !== a && "undefined" != typeof k.getElementsByTagName && g.grep(k.getElementsByTagName("input"), F)
                        },
                        O = function(k, a) {
                            a.src ? g.ajax({
                                url: a.src,
                                async: !1,
                                dataType: "script"
                            }) : g.globalEval((a.text || a.textContent || a.innerHTML || "").replace(Rb, "/*$0*/"));
                            a.parentNode && a.parentNode.removeChild(a)
                        },
                        S = function(k, a, c) {
                            var b = "width" === a ? k.offsetWidth : k.offsetHeight,
                                d = "width" === a ? ec : gc,
                                e = 0,
                                f = d.length;
                            if (0 < b) {
                                if ("border" !== c) for (; e < f; e++) c || (b -= parseFloat(g.css(k, "padding" + d[e])) || 0), "margin" === c ? b += parseFloat(g.css(k, c + d[e])) || 0 : b -= parseFloat(g.css(k, "border" + d[e] + "Width")) || 0;
                                return b + "px"
                            }
                            b = Hb(k, a, a);
                            if (0 > b || null == b) b = k.style[a] || 0;
                            b = parseFloat(b) || 0;
                            if (c) for (; e < f; e++) b += parseFloat(g.css(k, "padding" + d[e])) || 0, "padding" !== c && (b += parseFloat(g.css(k, "border" + d[e] + "Width")) || 0), "margin" === c && (b += parseFloat(g.css(k, c + d[e])) || 0);
                            return b + "px"
                        },
                        T = function(k) {
                            return function(a, c) {
                                "string" != typeof a && (c = a, a = "*");
                                if (g.isFunction(c)) for (var b = a.toLowerCase().split(hc), d = 0, e = b.length, f, j; d < e; d++) f = b[d], (j = /^\+/.test(f)) && (f = f.substr(1) || "*"), f = k[f] = k[f] || [], f[j ? "unshift" : "push"](c)
                            }
                        },
                        R = function(k, a, b, ab, d, g) {
                            d = d || a.dataTypes[0];
                            g = g || {};
                            g[d] = !0;
                            for (var d = k[d], e = 0, f = d ? d.length : 0, j = k === Qc, n; e < f && (j || !n); e++) n = d[e](a, b, ab), "string" == typeof n && (!j || g[n] ? n = c : (a.dataTypes.unshift(n), n = R(k, a, b, ab, n, g)));
                            return (j || !n) && !g["*"] && (n = R(k, a, b, ab, "*", g)), n
                        },
                        N = function(k, a) {
                            var b, ab, d = g.ajaxSettings.flatOptions || {};
                            for (b in a) a[b] !== c && ((d[b] ? k : ab || (ab = {}))[b] = a[b]);
                            ab && g.extend(!0, k, ab)
                        },
                        V = function(k, a, c, b) {
                            if (g.isArray(a)) g.each(a, function(a, oa) {
                                c || qc.test(k) ? b(k, oa) : V(k + "[" + ("object" == typeof oa || g.isArray(oa) ? a : "") + "]", oa, c, b)
                            });
                            else if (!c && null != a && "object" == typeof a) for (var d in a) V(k + "[" + d + "]", a[d], c, b);
                            else b(k, a)
                        },
                        fa = function() {
                            try {
                                return new b.XMLHttpRequest
                            } catch (k) {}
                        },
                        Z = function() {
                            Ub = c
                        },
                        ba = function(k, a) {
                            var c = {};
                            return g.each(mc.concat.apply([], mc.slice(0, a)), function() {
                                c[this] = k
                            }), c
                        },
                        ca = function(k) {
                            if (!Rc[k]) {
                                var a = v.body,
                                    c = g("<" + k + ">").appendTo(a),
                                    b = c.css("display");
                                c.remove();
                                if ("none" === b || "" === b) {
                                    Wa || (Wa = v.createElement("iframe"), Wa.frameBorder = Wa.width = Wa.height = 0);
                                    a.appendChild(Wa);
                                    if (!Ib || !Wa.createElement) Ib = (Wa.contentWindow || Wa.contentDocument).document, Ib.write(("CSS1Compat" === v.compatMode ? "<!doctype html>" : "") + "<html><body>"), Ib.close();
                                    c = Ib.createElement(k);
                                    Ib.body.appendChild(c);
                                    b = g.css(c, "display");
                                    a.removeChild(Wa)
                                }
                                Rc[k] = b
                            }
                            return Rc[k]
                        },
                        X = function(k) {
                            return g.isWindow(k) ? k : 9 === k.nodeType ? k.defaultView || k.parentWindow : !1
                        },
                        v = b.document,
                        aa = b.location,
                        g, da = function() {
                            if (!t.isReady) {
                                try {
                                    v.documentElement.doScroll("left")
                                } catch (k) {
                                    setTimeout(da, 1);
                                    return
                                }
                                t.ready()
                            }
                        },
                        t = function(k, a) {
                            return new t.fn.init(k, a, ha)
                        },
                        ra = b.jQuery,
                        ea = b.$,
                        ha, J = /^(?:[^#<]*(<[\w\W]+>)[^>]*$|#([\w\-]*)$)/,
                        W = /\S/,
                        ia = /^\s+/,
                        na = /\s+$/,
                        U = /^<(\w+)\s*\/?>(?:<\/\1>)?$/,
                        ka = /^[\],:{}\s]*$/,
                        I = /\\(?:["\\\/bfnrt]|u[0-9a-fA-F]{4})/g,
                        ua = /"[^"\\\n\r]*"|true|false|null|-?\d+(?:\.\d*)?(?:[eE][+\-]?\d+)?/g,
                        va = /(?:^|:|,)(?:\s*\[)+/g,
                        n = /(webkit)[ \/]([\w.]+)/,
                        G = /(opera)(?:.*version)?[ \/]([\w.]+)/,
                        la = /(msie) ([\w.]+)/,
                        wa = /(mozilla)(?:.*? rv:([\w.]+))?/,
                        Ba = /-([a-z]|[0-9])/ig,
                        bb = /^-ms-/,
                        Vb = function(k, a) {
                            return (a + "").toUpperCase()
                        },
                        md = b.navigator.userAgent,
                        rc, sc, Jb, nd = Object.prototype.toString,
                        Sc = Object.prototype.hasOwnProperty,
                        Tc = Array.prototype.push,
                        $ = Array.prototype.slice,
                        qa = String.prototype.trim,
                        ta = Array.prototype.indexOf,
                        xa = {};
                    g = (t.fn = t.prototype = {
                        constructor: t,
                        init: function(k, a, b) {
                            var d, g, e;
                            if (!k) return this;
                            if (k.nodeType) return this.context = this[0] = k, this.length = 1, this;
                            if ("body" === k && !a && v.body) return this.context = v, this[0] = v.body, this.selector = k, this.length = 1, this;
                            if ("string" == typeof k) {
                                "<" !== k.charAt(0) || ">" !== k.charAt(k.length - 1) || 3 > k.length ? d = J.exec(k) : d = [null, k, null];
                                if (d && (d[1] || !a)) {
                                    if (d[1]) return a = a instanceof t ? a[0] : a, e = a ? a.ownerDocument || a : v, g = U.exec(k), g ? t.isPlainObject(a) ? (k = [v.createElement(g[1])], t.fn.attr.call(k, a, !0)) : k = [e.createElement(g[1])] : (g = t.buildFragment([d[1]], [e]), k = (g.cacheable ? t.clone(g.fragment) : g.fragment).childNodes), t.merge(this, k);
                                    if ((a = v.getElementById(d[2])) && a.parentNode) {
                                        if (a.id !== d[2]) return b.find(k);
                                        this.length = 1;
                                        this[0] = a
                                    }
                                    return this.context = v, this.selector = k, this
                                }
                                return !a || a.jquery ? (a || b).find(k) : this.constructor(a).find(k)
                            }
                            return t.isFunction(k) ? b.ready(k) : (k.selector !== c && (this.selector = k.selector, this.context = k.context), t.makeArray(k, this))
                        },
                        selector: "",
                        jquery: "1.7.1",
                        length: 0,
                        size: function() {
                            return this.length
                        },
                        toArray: function() {
                            return $.call(this, 0)
                        },
                        get: function(k) {
                            return null == k ? this.toArray() : 0 > k ? this[this.length + k] : this[k]
                        },
                        pushStack: function(k, a, c) {
                            var b = this.constructor();
                            return t.isArray(k) ? Tc.apply(b, k) : t.merge(b, k), b.prevObject = this, b.context = this.context, "find" === a ? b.selector = this.selector + (this.selector ? " " : "") + c : a && (b.selector = this.selector + "." + a + "(" + c + ")"), b
                        },
                        each: function(k, a) {
                            return t.each(this, k, a)
                        },
                        ready: function(k) {
                            return t.bindReady(), sc.add(k), this
                        },
                        eq: function(k) {
                            return k = +k, -1 === k ? this.slice(k) : this.slice(k, k + 1)
                        },
                        first: function() {
                            return this.eq(0)
                        },
                        last: function() {
                            return this.eq(-1)
                        },
                        slice: function() {
                            return this.pushStack($.apply(this, arguments), "slice", $.call(arguments).join(","))
                        },
                        map: function(k) {
                            return this.pushStack(t.map(this, function(a, c) {
                                return k.call(a, c, a)
                            }))
                        },
                        end: function() {
                            return this.prevObject || this.constructor(null)
                        },
                        push: Tc,
                        sort: [].sort,
                        splice: [].splice
                    }, t.fn.init.prototype = t.fn, t.extend = t.fn.extend = function() {
                        var k, a, b, d, g, e, f = arguments[0] || {},
                            j = 1,
                            n = arguments.length,
                            q = !1;
                        "boolean" == typeof f && (q = f, f = arguments[1] || {}, j = 2);
                        "object" != typeof f && !t.isFunction(f) && (f = {});
                        for (n === j && (f = this, --j); j < n; j++) if (null != (k = arguments[j])) for (a in k) b = f[a], d = k[a], f !== d && (q && d && (t.isPlainObject(d) || (g = t.isArray(d))) ? (g ? (g = !1, e = b && t.isArray(b) ? b : []) : e = b && t.isPlainObject(b) ? b : {}, f[a] = t.extend(q, e, d)) : d !== c && (f[a] = d));
                        return f
                    }, t.extend({
                        noConflict: function(k) {
                            return b.$ === t && (b.$ = ea), k && b.jQuery === t && (b.jQuery = ra), t
                        },
                        isReady: !1,
                        readyWait: 1,
                        holdReady: function(k) {
                            k ? t.readyWait++ : t.ready(!0)
                        },
                        ready: function(k) {
                            if (!0 === k && !--t.readyWait || !0 !== k && !t.isReady) {
                                if (!v.body) return setTimeout(t.ready, 1);
                                t.isReady = !0;
                                !0 !== k && 0 < --t.readyWait || (sc.fireWith(v, [t]), t.fn.trigger && t(v).trigger("ready").off("ready"))
                            }
                        },
                        bindReady: function() {
                            if (!sc) {
                                sc = t.Callbacks("once memory");
                                if ("complete" === v.readyState) return setTimeout(t.ready, 1);
                                if (v.addEventListener) v.addEventListener("DOMContentLoaded", Jb, !1), b.addEventListener("load", t.ready, !1);
                                else if (v.attachEvent) {
                                    v.attachEvent("onreadystatechange", Jb);
                                    b.attachEvent("onload", t.ready);
                                    var k = !1;
                                    try {
                                        k = null == b.frameElement
                                    } catch (a) {}
                                    v.documentElement.doScroll && k && da()
                                }
                            }
                        },
                        isFunction: function(k) {
                            return "function" === t.type(k)
                        },
                        isArray: Array.isArray ||
                        function(k) {
                            return "array" === t.type(k)
                        },
                        isWindow: function(k) {
                            return k && "object" == typeof k && "setInterval" in k
                        },
                        isNumeric: function(k) {
                            return !isNaN(parseFloat(k)) && isFinite(k)
                        },
                        type: function(k) {
                            return null == k ? k + "" : xa[nd.call(k)] || "object"
                        },
                        isPlainObject: function(k) {
                            if (!k || "object" !== t.type(k) || k.nodeType || t.isWindow(k)) return !1;
                            try {
                                if (k.constructor && !Sc.call(k, "constructor") && !Sc.call(k.constructor.prototype, "isPrototypeOf")) return !1
                            } catch (a) {
                                return !1
                            }
                            for (var b in k);
                            return b === c || Sc.call(k, b)
                        },
                        isEmptyObject: function(k) {
                            for (var a in k) return !1;
                            return !0
                        },
                        error: function(k) {
                            throw Error(k);
                        },
                        parseJSON: function(k) {
                            if ("string" != typeof k || !k) return null;
                            k = t.trim(k);
                            if (b.JSON && b.JSON.parse) return b.JSON.parse(k);
                            if (ka.test(k.replace(I, "@").replace(ua, "]").replace(va, ""))) return Function("return " + k)();
                            t.error("Invalid JSON: " + k)
                        },
                        parseXML: function(k) {
                            var a, d;
                            try {
                                b.DOMParser ? (d = new DOMParser, a = d.parseFromString(k, "text/xml")) : (a = new ActiveXObject("Microsoft.XMLDOM"), a.async = "false", a.loadXML(k))
                            } catch (g) {
                                a = c
                            }
                            return (!a || !a.documentElement || a.getElementsByTagName("parsererror").length) && t.error("Invalid XML: " + k), a
                        },
                        noop: function() {},
                        globalEval: function(k) {
                            k && W.test(k) && (b.execScript ||
                            function(k) {
                                b.eval.call(b, k)
                            })(k)
                        },
                        camelCase: function(k) {
                            return k.replace(bb, "ms-").replace(Ba, Vb)
                        },
                        nodeName: function(k, a) {
                            return k.nodeName && k.nodeName.toUpperCase() === a.toUpperCase()
                        },
                        each: function(k, a, b) {
                            var d, g = 0,
                                e = k.length,
                                f = e === c || t.isFunction(k);
                            if (b) if (f) for (d in k) {
                                if (!1 === a.apply(k[d], b)) break
                            } else for (; g < e && !1 !== a.apply(k[g++], b););
                            else if (f) for (d in k) {
                                if (!1 === a.call(k[d], d, k[d])) break
                            } else for (; g < e && !1 !== a.call(k[g], g, k[g++]););
                            return k
                        },
                        trim: qa ?
                            function(k) {
                                return null == k ? "" : qa.call(k)
                            } : function(k) {
                            return null == k ? "" : (k + "").replace(ia, "").replace(na, "")
                        },
                        makeArray: function(k, a) {
                            var c = a || [];
                            if (null != k) {
                                var b = t.type(k);
                                null == k.length || "string" === b || "function" === b || "regexp" === b || t.isWindow(k) ? Tc.call(c, k) : t.merge(c, k)
                            }
                            return c
                        },
                        inArray: function(k, a, c) {
                            var b;
                            if (a) {
                                if (ta) return ta.call(a, k, c);
                                b = a.length;
                                for (c = c ? 0 > c ? Math.max(0, b + c) : c : 0; c < b; c++) if (c in a && a[c] === k) return c
                            }
                            return -1
                        },
                        merge: function(k, a) {
                            var b = k.length,
                                d = 0;
                            if ("number" == typeof a.length) for (var g = a.length; d < g; d++) k[b++] = a[d];
                            else for (; a[d] !== c;) k[b++] = a[d++];
                            return k.length = b, k
                        },
                        grep: function(k, a, c) {
                            for (var b = [], d, c = !! c, g = 0, e = k.length; g < e; g++) d = !! a(k[g], g), c !== d && b.push(k[g]);
                            return b
                        },
                        map: function(k, a, b) {
                            var d, g, e = [],
                                f = 0,
                                j = k.length;
                            if (k instanceof t || j !== c && "number" == typeof j && (0 < j && k[0] && k[j - 1] || 0 === j || t.isArray(k))) for (; f < j; f++) d = a(k[f], f, b), null != d && (e[e.length] = d);
                            else for (g in k) d = a(k[g], g, b), null != d && (e[e.length] = d);
                            return e.concat.apply([], e)
                        },
                        guid: 1,
                        proxy: function(k, a) {
                            if ("string" == typeof a) var b = k[a],
                                a = k,
                                k = b;
                            if (!t.isFunction(k)) return c;
                            var d = $.call(arguments, 2),
                                b = function() {
                                    return k.apply(a, d.concat($.call(arguments)))
                                };
                            return b.guid = k.guid = k.guid || b.guid || t.guid++, b
                        },
                        access: function(k, a, b, d, g, e) {
                            var f = k.length;
                            if ("object" == typeof a) {
                                for (var j in a) t.access(k, j, a[j], d, g, b);
                                return k
                            }
                            if (b !== c) {
                                d = !e && d && t.isFunction(b);
                                for (j = 0; j < f; j++) g(k[j], a, d ? b.call(k[j], j, g(k[j], a)) : b, e);
                                return k
                            }
                            return f ? g(k[0], a) : c
                        },
                        now: function() {
                            return (new Date).getTime()
                        },
                        uaMatch: function(k) {
                            k = k.toLowerCase();
                            k = n.exec(k) || G.exec(k) || la.exec(k) || 0 > k.indexOf("compatible") && wa.exec(k) || [];
                            return {
                                browser: k[1] || "",
                                version: k[2] || "0"
                            }
                        },
                        sub: function() {
                            function k(a, c) {
                                return new k.fn.init(a, c)
                            }
                            t.extend(!0, k, this);
                            k.superclass = this;
                            k.fn = k.prototype = this();
                            k.fn.constructor = k;
                            k.sub = this.sub;
                            k.fn.init = function(c, b) {
                                return b && b instanceof t && !(b instanceof k) && (b = k(b)), t.fn.init.call(this, c, b, a)
                            };
                            k.fn.init.prototype = k.fn;
                            var a = k(v);
                            return k
                        },
                        browser: {}
                    }), t.each("Boolean Number String Function Array Date RegExp Object".split(" "), function(k, a) {
                        xa["[object " + a + "]"] = a.toLowerCase()
                    }), rc = t.uaMatch(md), rc.browser && (t.browser[rc.browser] = !0, t.browser.version = rc.version), t.browser.webkit && (t.browser.safari = !0), W.test(" ") && (ia = /^[\s ]+/, na = /[\s ]+$/), ha = t(v), v.addEventListener ? Jb = function() {
                        v.removeEventListener("DOMContentLoaded", Jb, !1);
                        t.ready()
                    } : v.attachEvent && (Jb = function() {
                        "complete" === v.readyState && (v.detachEvent("onreadystatechange", Jb), t.ready())
                    }), t);
                    var ya = {};
                    g.Callbacks = function(k) {
                        var a;
                        if (k) {
                            if (!(a = ya[k])) {
                                a = k;
                                var b = ya[a] = {},
                                    d, e;
                                a = a.split(/\s+/);
                                d = 0;
                                for (e = a.length; d < e; d++) b[a[d]] = !0;
                                a = b
                            }
                        } else a = {};
                        var k = a,
                            f = [],
                            j = [],
                            n, q, m, y, w, G = function(a) {
                                var c, b, oa, d;
                                c = 0;
                                for (b = a.length; c < b; c++) oa = a[c], d = g.type(oa), "array" === d ? G(oa) : "function" === d && (!k.unique || !Y.has(oa)) && f.push(oa)
                            },
                            P = function(a, c) {
                                c = c || [];
                                n = !k.memory || [a, c];
                                q = !0;
                                w = m || 0;
                                m = 0;
                                for (y = f.length; f && w < y; w++) if (!1 === f[w].apply(a, c) && k.stopOnFalse) {
                                    n = !0;
                                    break
                                }
                                q = !1;
                                f && (k.once ? !0 === n ? Y.disable() : f = [] : j && j.length && (n = j.shift(), Y.fireWith(n[0], n[1])))
                            },
                            Y = {
                                add: function() {
                                    if (f) {
                                        var k = f.length;
                                        G(arguments);
                                        q ? y = f.length : n && !0 !== n && (m = k, P(n[0], n[1]))
                                    }
                                    return this
                                },
                                remove: function() {
                                    if (f) for (var a = arguments, c = 0, b = a.length; c < b; c++) for (var oa = 0; oa < f.length && !(a[c] === f[oa] && (q && oa <= y && (y--, oa <= w && w--), f.splice(oa--, 1), k.unique)); oa++);
                                    return this
                                },
                                has: function(k) {
                                    if (f) for (var a = 0, c = f.length; a < c; a++) if (k === f[a]) return !0;
                                    return !1
                                },
                                empty: function() {
                                    return f = [], this
                                },
                                disable: function() {
                                    return f = j = n = c, this
                                },
                                disabled: function() {
                                    return !f
                                },
                                lock: function() {
                                    return j = c, (!n || !0 === n) && Y.disable(), this
                                },
                                locked: function() {
                                    return !j
                                },
                                fireWith: function(a, c) {
                                    return j && (q ? k.once || j.push([a, c]) : (!k.once || !n) && P(a, c)), this
                                },
                                fire: function() {
                                    return Y.fireWith(this, arguments), this
                                },
                                fired: function() {
                                    return !!n
                                }
                            };
                        return Y
                    };
                    var pa = [].slice;
                    g.extend({
                        Deferred: function(k) {
                            var a = g.Callbacks("once memory"),
                                c = g.Callbacks("once memory"),
                                b = g.Callbacks("memory"),
                                d = "pending",
                                e = {
                                    resolve: a,
                                    reject: c,
                                    notify: b
                                },
                                f = {
                                    done: a.add,
                                    fail: c.add,
                                    progress: b.add,
                                    state: function() {
                                        return d
                                    },
                                    isResolved: a.fired,
                                    isRejected: c.fired,
                                    then: function(k, a, c) {
                                        return j.done(k).fail(a).progress(c), this
                                    },
                                    always: function() {
                                        return j.done.apply(j, arguments).fail.apply(j, arguments), this
                                    },
                                    pipe: function(k, a, c) {
                                        return g.Deferred(function(b) {
                                            g.each({
                                                done: [k, "resolve"],
                                                fail: [a, "reject"],
                                                progress: [c, "notify"]
                                            }, function(k, a) {
                                                var c = a[0],
                                                    oa = a[1],
                                                    d;
                                                g.isFunction(c) ? j[k](function() {
                                                    (d = c.apply(this, arguments)) && g.isFunction(d.promise) ? d.promise().then(b.resolve, b.reject, b.notify) : b[oa + "With"](this === j ? b : this, [d])
                                                }) : j[k](b[oa])
                                            })
                                        }).promise()
                                    },
                                    promise: function(k) {
                                        if (null == k) k = f;
                                        else for (var a in f) k[a] = f[a];
                                        return k
                                    }
                                },
                                j = f.promise({}),
                                n;
                            for (n in e) j[n] = e[n].fire, j[n + "With"] = e[n].fireWith;
                            return j.done(function() {
                                d = "resolved"
                            }, c.disable, b.lock).fail(function() {
                                d = "rejected"
                            }, a.disable, b.lock), k && k.call(j, j), j
                        },
                        when: function(k) {
                            function a(k) {
                                return function(a) {
                                    b[k] = 1 < arguments.length ? pa.call(arguments, 0) : a;
                                    --j || n.resolveWith(n, b)
                                }
                            }
                            function c(k) {
                                return function(a) {
                                    f[k] = 1 < arguments.length ? pa.call(arguments, 0) : a;
                                    n.notifyWith(q, f)
                                }
                            }
                            var b = pa.call(arguments, 0),
                                d = 0,
                                e = b.length,
                                f = Array(e),
                                j = e,
                                n = 1 >= e && k && g.isFunction(k.promise) ? k : g.Deferred(),
                                q = n.promise();
                            if (1 < e) {
                                for (; d < e; d++) b[d] && b[d].promise && g.isFunction(b[d].promise) ? b[d].promise().then(a(d), n.reject, c(d)) : --j;
                                j || n.resolveWith(n, b)
                            } else n !== k && n.resolveWith(n, e ? [k] : []);
                            return q
                        }
                    });
                    var Fa = g,
                        Da;
                    var Ca, Ka, za, Ha, Ia, Na, Aa, rb, Ga, La, Ta, Ma, D = v.createElement("div");
                    D.setAttribute("className", "t");
                    D.innerHTML = "   <link/><table></table><a href='/a' style='top:1px;float:left;opacity:.55;'>a</a><input type='checkbox'/>";
                    Ka = D.getElementsByTagName("*");
                    za = D.getElementsByTagName("a")[0];
                    if (!Ka || !Ka.length || !za) Da = {};
                    else {
                        Ha = v.createElement("select");
                        Ia = Ha.appendChild(v.createElement("option"));
                        Na = D.getElementsByTagName("input")[0];
                        Ca = {
                            leadingWhitespace: 3 === D.firstChild.nodeType,
                            tbody: !D.getElementsByTagName("tbody").length,
                            htmlSerialize: !! D.getElementsByTagName("link").length,
                            style: /top/.test(za.getAttribute("style")),
                            hrefNormalized: "/a" === za.getAttribute("href"),
                            opacity: /^0.55/.test(za.style.opacity),
                            cssFloat: !! za.style.cssFloat,
                            checkOn: "on" === Na.value,
                            optSelected: Ia.selected,
                            getSetAttribute: "t" !== D.className,
                            enctype: !! v.createElement("form").enctype,
                            html5Clone: "<:nav></:nav>" !== v.createElement("nav").cloneNode(!0).outerHTML,
                            submitBubbles: !0,
                            changeBubbles: !0,
                            focusinBubbles: !1,
                            deleteExpando: !0,
                            noCloneEvent: !0,
                            inlineBlockNeedsLayout: !1,
                            shrinkWrapBlocks: !1,
                            reliableMarginRight: !0
                        };
                        Na.checked = !0;
                        Ca.noCloneChecked = Na.cloneNode(!0).checked;
                        Ha.disabled = !0;
                        Ca.optDisabled = !Ia.disabled;
                        try {
                            delete D.test
                        } catch (u) {
                            Ca.deleteExpando = !1
                        }!D.addEventListener && D.attachEvent && D.fireEvent && (D.attachEvent("onclick", function() {
                            Ca.noCloneEvent = !1
                        }), D.cloneNode(!0).fireEvent("onclick"));
                        Na = v.createElement("input");
                        Na.value = "t";
                        Na.setAttribute("type", "radio");
                        Ca.radioValue = "t" === Na.value;
                        Na.setAttribute("checked", "checked");
                        D.appendChild(Na);
                        rb = v.createDocumentFragment();
                        rb.appendChild(D.lastChild);
                        Ca.checkClone = rb.cloneNode(!0).cloneNode(!0).lastChild.checked;
                        Ca.appendChecked = Na.checked;
                        rb.removeChild(Na);
                        rb.appendChild(D);
                        D.innerHTML = "";
                        b.getComputedStyle && (Aa = v.createElement("div"), Aa.style.width = "0", Aa.style.marginRight = "0", D.style.width = "2px", D.appendChild(Aa), Ca.reliableMarginRight = 0 === (parseInt((b.getComputedStyle(Aa, null) || {
                            marginRight: 0
                        }).marginRight, 10) || 0));
                        if (D.attachEvent) for (Ta in {
                            submit: 1,
                            change: 1,
                            focusin: 1
                        }) La = "on" + Ta, (Ma = La in D) || (D.setAttribute(La, "return;"), Ma = "function" == typeof D[La]), Ca[Ta + "Bubbles"] = Ma;
                        Da = (rb.removeChild(D), rb = Ha = Ia = Aa = D = Na = null, g(function() {
                            var k, a, c, b, d = v.getElementsByTagName("body")[0];
                            d && (k = v.createElement("div"), k.style.cssText = "visibility:hidden;border:0;width:0;height:0;position:static;top:0;margin-top:1px", d.insertBefore(k, d.firstChild), D = v.createElement("div"), k.appendChild(D), D.innerHTML = "<table><tr><td style='padding:0;border:0;display:none'></td><td>t</td></tr></table>", Ga = D.getElementsByTagName("td"), Ma = 0 === Ga[0].offsetHeight, Ga[0].style.display = "", Ga[1].style.display = "none", Ca.reliableHiddenOffsets = Ma && 0 === Ga[0].offsetHeight, D.innerHTML = "", D.style.width = D.style.paddingLeft = "1px", g.boxModel = Ca.boxModel = 2 === D.offsetWidth, "undefined" != typeof D.style.zoom && (D.style.display = "inline", D.style.zoom = 1, Ca.inlineBlockNeedsLayout = 2 === D.offsetWidth, D.style.display = "", D.innerHTML = "<div style='width:4px;'></div>", Ca.shrinkWrapBlocks = 2 !== D.offsetWidth), D.style.cssText = "position:absolute;top:0;left:0;width:1px;height:1px;margin:0;visibility:hidden;border:0;", D.innerHTML = "<div style='position:absolute;top:0;left:0;width:1px;height:1px;margin:0;border:5px solid #000;padding:0;'><div></div></div><table style='position:absolute;top:0;left:0;width:1px;height:1px;margin:0;border:5px solid #000;padding:0;' cellpadding='0' cellspacing='0'><tr><td></td></tr></table>", a = D.firstChild, c = a.firstChild, b = a.nextSibling.firstChild.firstChild, b = {
                                doesNotAddBorder: 5 !== c.offsetTop,
                                doesAddBorderForTableAndCells: 5 === b.offsetTop
                            }, c.style.position = "fixed", c.style.top = "20px", b.fixedPosition = 20 === c.offsetTop || 15 === c.offsetTop, c.style.position = c.style.top = "", a.style.overflow = "hidden", a.style.position = "relative", b.subtractsBorderForOverflowNotVisible = -5 === c.offsetTop, b.doesNotIncludeMarginInBodyOffset = 1 !== d.offsetTop, d.removeChild(k), D = null, g.extend(Ca, b))
                        }), Ca)
                    }
                    Fa.support = Da;
                    var jd = /^(?:\{.*\}|\[.*\])$/,
                        Za = /([A-Z])/g;
                    g.extend({
                        cache: {},
                        uuid: 0,
                        expando: "jQuery" + (g.fn.jquery + Math.random()).replace(/\D/g, ""),
                        noData: {
                            embed: !0,
                            object: "clsid:D27CDB6E-AE6D-11cf-96B8-444553540000",
                            applet: !0
                        },
                        hasData: function(k) {
                            return k = k.nodeType ? g.cache[k[g.expando]] : k[g.expando], !! k && !e(k)
                        },
                        data: function(k, a, b, d) {
                            if (g.acceptData(k)) {
                                var e, f, j, n = g.expando,
                                    q = "string" == typeof a,
                                    m = k.nodeType,
                                    y = m ? g.cache : k,
                                    w = m ? k[n] : k[n] && n,
                                    G = "events" === a;
                                if (w && y[w] && (G || d || y[w].data) || !(q && b === c)) {
                                    w || (m ? k[n] = w = ++g.uuid : w = n);
                                    y[w] || (y[w] = {}, m || (y[w].toJSON = g.noop));
                                    if ("object" == typeof a || "function" == typeof a) d ? y[w] = g.extend(y[w], a) : y[w].data = g.extend(y[w].data, a);
                                    return e = f = y[w], d || (f.data || (f.data = {}), f = f.data), b !== c && (f[g.camelCase(a)] = b), G && !f[a] ? e.events : (q ? (j = f[a], null == j && (j = f[g.camelCase(a)])) : j = f, j)
                                }
                            }
                        },
                        removeData: function(k, a, c) {
                            if (g.acceptData(k)) {
                                var b, d, f, j = g.expando,
                                    n = k.nodeType,
                                    q = n ? g.cache : k,
                                    m = n ? k[j] : j;
                                if (q[m]) {
                                    if (a && (b = c ? q[m] : q[m].data)) {
                                        g.isArray(a) || (a in b ? a = [a] : (a = g.camelCase(a), a in b ? a = [a] : a = a.split(" ")));
                                        d = 0;
                                        for (f = a.length; d < f; d++) delete b[a[d]];
                                        if (!(c ? e : g.isEmptyObject)(b)) return
                                    }
                                    if (!c && (delete q[m].data, !e(q[m]))) return;
                                    g.support.deleteExpando || !q.setInterval ? delete q[m] : q[m] = null;
                                    n && (g.support.deleteExpando ? delete k[j] : k.removeAttribute ? k.removeAttribute(j) : k[j] = null)
                                }
                            }
                        },
                        _data: function(k, a, c) {
                            return g.data(k, a, c, !0)
                        },
                        acceptData: function(k) {
                            if (k.nodeName) {
                                var a = g.noData[k.nodeName.toLowerCase()];
                                if (a) return !0 !== a && k.getAttribute("classid") === a
                            }
                            return !0
                        }
                    });
                    g.fn.extend({
                        data: function(k, a) {
                            var b, e, f, j = null;
                            if ("undefined" == typeof k) {
                                if (this.length && (j = g.data(this[0]), 1 === this[0].nodeType && !g._data(this[0], "parsedAttrs"))) {
                                    e = this[0].attributes;
                                    for (var n = 0, q = e.length; n < q; n++) f = e[n].name, 0 === f.indexOf("data-") && (f = g.camelCase(f.substring(5)), d(this[0], f, j[f]));
                                    g._data(this[0], "parsedAttrs", !0)
                                }
                                return j
                            }
                            return "object" == typeof k ? this.each(function() {
                                g.data(this, k)
                            }) : (b = k.split("."), b[1] = b[1] ? "." + b[1] : "", a === c ? (j = this.triggerHandler("getData" + b[1] + "!", [b[0]]), j === c && this.length && (j = g.data(this[0], k), j = d(this[0], k, j)), j === c && b[1] ? this.data(b[0]) : j) : this.each(function() {
                                var c = g(this),
                                    d = [b[0], a];
                                c.triggerHandler("setData" + b[1] + "!", d);
                                g.data(this, k, a);
                                c.triggerHandler("changeData" + b[1] + "!", d)
                            }))
                        },
                        removeData: function(k) {
                            return this.each(function() {
                                g.removeData(this, k)
                            })
                        }
                    });
                    g.extend({
                        _mark: function(k, a) {
                            k && (a = (a || "fx") + "mark", g._data(k, a, (g._data(k, a) || 0) + 1))
                        },
                        _unmark: function(k, a, b) {
                            !0 !== k && (b = a, a = k, k = !1);
                            if (a) {
                                var b = b || "fx",
                                    c = b + "mark";
                                (k = k ? 0 : (g._data(a, c) || 1) - 1) ? g._data(a, c, k) : (g.removeData(a, c, !0), j(a, b, "mark"))
                            }
                        },
                        queue: function(k, a, b) {
                            var c;
                            if (k) return a = (a || "fx") + "queue", c = g._data(k, a), b && (!c || g.isArray(b) ? c = g._data(k, a, g.makeArray(b)) : c.push(b)), c || []
                        },
                        dequeue: function(a, b) {
                            var b = b || "fx",
                                c = g.queue(a, b),
                                d = c.shift(),
                                e = {};
                            "inprogress" === d && (d = c.shift());
                            d && ("fx" === b && c.unshift("inprogress"), g._data(a, b + ".run", e), d.call(a, function() {
                                g.dequeue(a, b)
                            }, e));
                            c.length || (g.removeData(a, b + "queue " + b + ".run", !0), j(a, b, "queue"))
                        }
                    });
                    g.fn.extend({
                        queue: function(a, b) {
                            return "string" != typeof a && (b = a, a = "fx"), b === c ? g.queue(this[0], a) : this.each(function() {
                                var c = g.queue(this, a, b);
                                "fx" === a && "inprogress" !== c[0] && g.dequeue(this, a)
                            })
                        },
                        dequeue: function(a) {
                            return this.each(function() {
                                g.dequeue(this, a)
                            })
                        },
                        delay: function(a, b) {
                            return a = g.fx ? g.fx.speeds[a] || a : a, b = b || "fx", this.queue(b, function(b, c) {
                                var oa = setTimeout(b, a);
                                c.stop = function() {
                                    clearTimeout(oa)
                                }
                            })
                        },
                        clearQueue: function(a) {
                            return this.queue(a || "fx", [])
                        },
                        promise: function(a) {
                            function b() {
                                --j || d.resolveWith(e, [e])
                            }
                            "string" != typeof a && (a = c);
                            for (var a = a || "fx", d = g.Deferred(), e = this, f = e.length, j = 1, n = a + "defer", q = a + "queue", a = a + "mark", m; f--;) if (m = g.data(e[f], n, c, !0) || (g.data(e[f], q, c, !0) || g.data(e[f], a, c, !0)) && g.data(e[f], n, g.Callbacks("once memory"), !0)) j++, m.add(b);
                            return b(), d.promise()
                        }
                    });
                    var bd = /[\n\t\r]/g,
                        B = /\s+/,
                        od = /\r/g,
                        pd = /^(?:button|input)$/i,
                        qd = /^(?:button|input|object|select|textarea)$/i,
                        M = /^a(?:rea)?$/i,
                        cd = /^(?:autofocus|autoplay|async|checked|controls|defer|disabled|hidden|loop|multiple|open|readonly|required|scoped|selected)$/i,
                        dd = g.support.getSetAttribute,
                        cb, ed, fd;
                    g.fn.extend({
                        attr: function(a, b) {
                            return g.access(this, a, b, !0, g.attr)
                        },
                        removeAttr: function(a) {
                            return this.each(function() {
                                g.removeAttr(this, a)
                            })
                        },
                        prop: function(a, b) {
                            return g.access(this, a, b, !0, g.prop)
                        },
                        removeProp: function(a) {
                            return a = g.propFix[a] || a, this.each(function() {
                                try {
                                    this[a] = c, delete this[a]
                                } catch (b) {}
                            })
                        },
                        addClass: function(a) {
                            var b, c, d, e, f, j, n;
                            if (g.isFunction(a)) return this.each(function(b) {
                                g(this).addClass(a.call(this, b, this.className))
                            });
                            if (a && "string" == typeof a) {
                                b = a.split(B);
                                c = 0;
                                for (d = this.length; c < d; c++) if (e = this[c], 1 === e.nodeType) if (!e.className && 1 === b.length) e.className = a;
                                else {
                                    f = " " + e.className + " ";
                                    j = 0;
                                    for (n = b.length; j < n; j++)~f.indexOf(" " + b[j] + " ") || (f += b[j] + " ");
                                    e.className = g.trim(f)
                                }
                            }
                            return this
                        },
                        removeClass: function(a) {
                            var b, d, e, f, j, n, q;
                            if (g.isFunction(a)) return this.each(function(b) {
                                g(this).removeClass(a.call(this, b, this.className))
                            });
                            if (a && "string" == typeof a || a === c) {
                                b = (a || "").split(B);
                                d = 0;
                                for (e = this.length; d < e; d++) if (f = this[d], 1 === f.nodeType && f.className) if (a) {
                                    j = (" " + f.className + " ").replace(bd, " ");
                                    n = 0;
                                    for (q = b.length; n < q; n++) j = j.replace(" " + b[n] + " ", " ");
                                    f.className = g.trim(j)
                                } else f.className = ""
                            }
                            return this
                        },
                        toggleClass: function(a, b) {
                            var c = typeof a,
                                d = "boolean" == typeof b;
                            return g.isFunction(a) ? this.each(function(c) {
                                g(this).toggleClass(a.call(this, c, this.className, b), b)
                            }) : this.each(function() {
                                if ("string" === c) for (var e, f = 0, j = g(this), n = b, q = a.split(B); e = q[f++];) n = d ? n : !j.hasClass(e), j[n ? "addClass" : "removeClass"](e);
                                else if ("undefined" === c || "boolean" === c) this.className && g._data(this, "__className__", this.className), this.className = this.className || !1 === a ? "" : g._data(this, "__className__") || ""
                            })
                        },
                        hasClass: function(a) {
                            for (var a = " " + a + " ", b = 0, c = this.length; b < c; b++) if (1 === this[b].nodeType && -1 < (" " + this[b].className + " ").replace(bd, " ").indexOf(a)) return !0;
                            return !1
                        },
                        val: function(a) {
                            var b, d, e, f = this[0];
                            if (arguments.length) return e = g.isFunction(a), this.each(function(d) {
                                var Tb = g(this),
                                    f;
                                if (1 === this.nodeType && (e ? f = a.call(this, d, Tb.val()) : f = a, null == f ? f = "" : "number" == typeof f ? f += "" : g.isArray(f) && (f = g.map(f, function(a) {
                                        return null == a ? "" : a + ""
                                    })), b = g.valHooks[this.nodeName.toLowerCase()] || g.valHooks[this.type], !b || !("set" in b) || b.set(this, f, "value") === c)) this.value = f
                            });
                            if (f) return b = g.valHooks[f.nodeName.toLowerCase()] || g.valHooks[f.type], b && "get" in b && (d = b.get(f, "value")) !== c ? d : (d = f.value, "string" == typeof d ? d.replace(od, "") : null == d ? "" : d)
                        }
                    });
                    g.extend({
                        valHooks: {
                            option: {
                                get: function(a) {
                                    var b = a.attributes.value;
                                    return !b || b.specified ? a.value : a.text
                                }
                            },
                            select: {
                                get: function(a) {
                                    var b, c, d = a.selectedIndex,
                                        e = [],
                                        f = a.options,
                                        j = "select-one" === a.type;
                                    if (0 > d) return null;
                                    a = j ? d : 0;
                                    for (c = j ? d + 1 : f.length; a < c; a++) if (b = f[a], b.selected && (g.support.optDisabled ? !b.disabled : null === b.getAttribute("disabled")) && (!b.parentNode.disabled || !g.nodeName(b.parentNode, "optgroup"))) {
                                        b = g(b).val();
                                        if (j) return b;
                                        e.push(b)
                                    }
                                    return j && !e.length && f.length ? g(f[d]).val() : e
                                },
                                set: function(a, b) {
                                    var c = g.makeArray(b);
                                    return g(a).find("option").each(function() {
                                        this.selected = 0 <= g.inArray(g(this).val(), c)
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
                        attr: function(a, b, d, e) {
                            var f, j, n = a.nodeType;
                            if (a && !(3 === n || 8 === n || 2 === n)) {
                                if (e && b in g.attrFn) return g(a)[b](d);
                                if ("undefined" == typeof a.getAttribute) return g.prop(a, b, d);
                                (e = 1 !== n || !g.isXMLDoc(a)) && (b = b.toLowerCase(), j = g.attrHooks[b] || (cd.test(b) ? ed : cb));
                                if (d !== c) {
                                    if (null === d) {
                                        g.removeAttr(a, b);
                                        return
                                    }
                                    return j && "set" in j && e && (f = j.set(a, d, b)) !== c ? f : (a.setAttribute(b, "" + d), d)
                                }
                                return j && "get" in j && e && null !== (f = j.get(a, b)) ? f : (f = a.getAttribute(b), null === f ? c : f)
                            }
                        },
                        removeAttr: function(a, b) {
                            var c, d, e, f, j = 0;
                            if (b && 1 === a.nodeType) {
                                d = b.toLowerCase().split(B);
                                for (f = d.length; j < f; j++)(e = d[j]) && (c = g.propFix[e] || e, g.attr(a, e, ""), a.removeAttribute(dd ? e : c), cd.test(e) && c in a && (a[c] = !1))
                            }
                        },
                        attrHooks: {
                            type: {
                                set: function(a, b) {
                                    if (pd.test(a.nodeName) && a.parentNode) g.error("type property can't be changed");
                                    else if (!g.support.radioValue && "radio" === b && g.nodeName(a, "input")) {
                                        var c = a.value;
                                        return a.setAttribute("type", b), c && (a.value = c), b
                                    }
                                }
                            },
                            value: {
                                get: function(a, b) {
                                    return cb && g.nodeName(a, "button") ? cb.get(a, b) : b in a ? a.value : null
                                },
                                set: function(a, b, c) {
                                    if (cb && g.nodeName(a, "button")) return cb.set(a, b, c);
                                    a.value = b
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
                        prop: function(a, b, d) {
                            var e, f, j, n = a.nodeType;
                            if (a && !(3 === n || 8 === n || 2 === n)) return j = 1 !== n || !g.isXMLDoc(a), j && (b = g.propFix[b] || b, f = g.propHooks[b]), d !== c ? f && "set" in f && (e = f.set(a, d, b)) !== c ? e : a[b] = d : f && "get" in f && null !== (e = f.get(a, b)) ? e : a[b]
                        },
                        propHooks: {
                            tabIndex: {
                                get: function(a) {
                                    var b = a.getAttributeNode("tabindex");
                                    return b && b.specified ? parseInt(b.value, 10) : qd.test(a.nodeName) || M.test(a.nodeName) && a.href ? 0 : c
                                }
                            }
                        }
                    });
                    g.attrHooks.tabindex = g.propHooks.tabIndex;
                    ed = {
                        get: function(a, b) {
                            var d, e = g.prop(a, b);
                            return !0 === e || "boolean" != typeof e && (d = a.getAttributeNode(b)) && !1 !== d.nodeValue ? b.toLowerCase() : c
                        },
                        set: function(a, b, c) {
                            var d;
                            return !1 === b ? g.removeAttr(a, c) : (d = g.propFix[c] || c, d in a && (a[d] = !0), a.setAttribute(c, c.toLowerCase())), c
                        }
                    };
                    dd || (fd = {
                        name: !0,
                        id: !0
                    }, cb = g.valHooks.button = {
                        get: function(a, b) {
                            var d;
                            return d = a.getAttributeNode(b), d && (fd[b] ? "" !== d.nodeValue : d.specified) ? d.nodeValue : c
                        },
                        set: function(a, b, c) {
                            var d = a.getAttributeNode(c);
                            return d || (d = v.createAttribute(c), a.setAttributeNode(d)), d.nodeValue = b + ""
                        }
                    }, g.attrHooks.tabindex.set = cb.set, g.each(["width", "height"], function(a, b) {
                        g.attrHooks[b] = g.extend(g.attrHooks[b], {
                            set: function(a, k) {
                                if ("" === k) return a.setAttribute(b, "auto"), k
                            }
                        })
                    }), g.attrHooks.contenteditable = {
                        get: cb.get,
                        set: function(a, b, c) {
                            "" === b && (b = "false");
                            cb.set(a, b, c)
                        }
                    });
                    g.support.hrefNormalized || g.each(["href", "src", "width", "height"], function(a, b) {
                        g.attrHooks[b] = g.extend(g.attrHooks[b], {
                            get: function(a) {
                                a = a.getAttribute(b, 2);
                                return null === a ? c : a
                            }
                        })
                    });
                    g.support.style || (g.attrHooks.style = {
                        get: function(a) {
                            return a.style.cssText.toLowerCase() || c
                        },
                        set: function(a, b) {
                            return a.style.cssText = "" + b
                        }
                    });
                    g.support.optSelected || (g.propHooks.selected = g.extend(g.propHooks.selected, {
                        get: function(a) {
                            a = a.parentNode;
                            return a && (a.selectedIndex, a.parentNode && a.parentNode.selectedIndex), null
                        }
                    }));
                    g.support.enctype || (g.propFix.enctype = "encoding");
                    g.support.checkOn || g.each(["radio", "checkbox"], function() {
                        g.valHooks[this] = {
                            get: function(a) {
                                return null === a.getAttribute("value") ? "on" : a.value
                            }
                        }
                    });
                    g.each(["radio", "checkbox"], function() {
                        g.valHooks[this] = g.extend(g.valHooks[this], {
                            set: function(a, b) {
                                if (g.isArray(b)) return a.checked = 0 <= g.inArray(g(a).val(), b)
                            }
                        })
                    });
                    var ja = /^(?:textarea|input|select)$/i,
                        Ya = /^([^\.]*)?(?:\.(.+))?$/,
                        fb = /\bhover(\.\S+)?\b/,
                        rd = /^key/,
                        sd = /^(?:mouse|contextmenu)|click/,
                        gd = /^(?:focusinfocus|focusoutblur)$/,
                        q = /^(\w*)(?:#([\w\-]+))?(?:\.([\w\-]+))?$/,
                        Y = function(a) {
                            a = q.exec(a);
                            return a && (a[1] = (a[1] || "").toLowerCase(), a[3] = a[3] && RegExp("(?:^|\\s)" + a[3] + "(?:\\s|$)")), a
                        },
                        w = function(a) {
                            return g.event.special.hover ? a : a.replace(fb, "mouseenter$1 mouseleave$1")
                        };
                    g.event = {
                        add: function(a, b, d, e, f) {
                            var j, n, q, m, y, G, P, la, z;
                            if (!(3 === a.nodeType || 8 === a.nodeType || !b || !d || !(j = g._data(a)))) {
                                d.handler && (P = d, d = P.handler);
                                d.guid || (d.guid = g.guid++);
                                (q = j.events) || (j.events = q = {});
                                (n = j.handle) || (j.handle = n = function(a) {
                                    return "undefined" == typeof g || a && g.event.triggered === a.type ? c : g.event.dispatch.apply(n.elem, arguments)
                                }, n.elem = a);
                                b = g.trim(w(b)).split(" ");
                                for (j = 0; j < b.length; j++) {
                                    m = Ya.exec(b[j]) || [];
                                    y = m[1];
                                    G = (m[2] || "").split(".").sort();
                                    z = g.event.special[y] || {};
                                    y = (f ? z.delegateType : z.bindType) || y;
                                    z = g.event.special[y] || {};
                                    m = g.extend({
                                        type: y,
                                        origType: m[1],
                                        data: e,
                                        handler: d,
                                        guid: d.guid,
                                        selector: f,
                                        quick: Y(f),
                                        namespace: G.join(".")
                                    }, P);
                                    la = q[y];
                                    if (!la && (la = q[y] = [], la.delegateCount = 0, !z.setup || !1 === z.setup.call(a, e, G, n))) a.addEventListener ? a.addEventListener(y, n, !1) : a.attachEvent && a.attachEvent("on" + y, n);
                                    z.add && (z.add.call(a, m), m.handler.guid || (m.handler.guid = d.guid));
                                    f ? la.splice(la.delegateCount++, 0, m) : la.push(m);
                                    g.event.global[y] = !0
                                }
                                a = null
                            }
                        },
                        global: {},
                        remove: function(a, b, c, d, e) {
                            var f = g.hasData(a) && g._data(a),
                                j, n, q, m, y, G, P, Y, z, la, H;
                            if (f && (P = f.events)) {
                                b = g.trim(w(b || "")).split(" ");
                                for (j = 0; j < b.length; j++) if (n = Ya.exec(b[j]) || [], q = m = n[1], n = n[2], q) {
                                    Y = g.event.special[q] || {};
                                    q = (d ? Y.delegateType : Y.bindType) || q;
                                    la = P[q] || [];
                                    y = la.length;
                                    n = n ? RegExp("(^|\\.)" + n.split(".").sort().join("\\.(?:.*\\.)?") + "(\\.|$)") : null;
                                    for (G = 0; G < la.length; G++) H = la[G], (e || m === H.origType) && (!c || c.guid === H.guid) && (!n || n.test(H.namespace)) && (!d || d === H.selector || "**" === d && H.selector) && (la.splice(G--, 1), H.selector && la.delegateCount--, Y.remove && Y.remove.call(a, H));
                                    0 === la.length && y !== la.length && ((!Y.teardown || !1 === Y.teardown.call(a, n)) && g.removeEvent(a, q, f.handle), delete P[q])
                                } else for (q in P) g.event.remove(a, q + b[j], c, d, !0);
                                g.isEmptyObject(P) && (z = f.handle, z && (z.elem = null), g.removeData(a, ["events", "handle"], !0))
                            }
                        },
                        customEvent: {
                            getData: !0,
                            setData: !0,
                            changeData: !0
                        },
                        trigger: function(a, d, e, f) {
                            if (!e || 3 !== e.nodeType && 8 !== e.nodeType) {
                                var j = a.type || a,
                                    n = [],
                                    q, m, y, w, G, P;
                                if (!gd.test(j + g.event.triggered) && (0 <= j.indexOf("!") && (j = j.slice(0, -1), q = !0), 0 <= j.indexOf(".") && (n = j.split("."), j = n.shift(), n.sort()), e && !g.event.customEvent[j] || g.event.global[j])) if (a = "object" == typeof a ? a[g.expando] ? a : new g.Event(j, a) : new g.Event(j), a.type = j, a.isTrigger = !0, a.exclusive = q, a.namespace = n.join("."), a.namespace_re = a.namespace ? RegExp("(^|\\.)" + n.join("\\.(?:.*\\.)?") + "(\\.|$)") : null, q = 0 > j.indexOf(":") ? "on" + j : "", e) {
                                    if (a.result = c, a.target || (a.target = e), d = null != d ? g.makeArray(d) : [], d.unshift(a), w = g.event.special[j] || {}, !(w.trigger && !1 === w.trigger.apply(e, d))) {
                                        P = [
                                            [e, w.bindType || j]
                                        ];
                                        if (!f && !w.noBubble && !g.isWindow(e)) {
                                            m = w.delegateType || j;
                                            n = gd.test(m + j) ? e : e.parentNode;
                                            for (y = null; n; n = n.parentNode) P.push([n, m]), y = n;
                                            y && y === e.ownerDocument && P.push([y.defaultView || y.parentWindow || b, m])
                                        }
                                        for (m = 0; m < P.length && !a.isPropagationStopped(); m++) n = P[m][0], a.type = P[m][1], (G = (g._data(n, "events") || {})[a.type] && g._data(n, "handle")) && G.apply(n, d), (G = q && n[q]) && g.acceptData(n) && !1 === G.apply(n, d) && a.preventDefault();
                                        return a.type = j, !f && !a.isDefaultPrevented() && (!w._default || !1 === w._default.apply(e.ownerDocument, d)) && ("click" !== j || !g.nodeName(e, "a")) && g.acceptData(e) && q && e[j] && ("focus" !== j && "blur" !== j || 0 !== a.target.offsetWidth) && !g.isWindow(e) && (y = e[q], y && (e[q] = null), g.event.triggered = j, e[j](), g.event.triggered = c, y && (e[q] = y)), a.result
                                    }
                                } else for (m in e = g.cache, e) e[m].events && e[m].events[j] && g.event.trigger(a, d, e[m].handle.elem, !0)
                            }
                        },
                        dispatch: function(a) {
                            var a = g.event.fix(a || b.event),
                                d = (g._data(this, "events") || {})[a.type] || [],
                                e = d.delegateCount,
                                f = [].slice.call(arguments, 0),
                                j = !a.exclusive && !a.namespace,
                                n = [],
                                q, m, y, w, G, P, Y;
                            f[0] = a;
                            a.delegateTarget = this;
                            if (e && !a.target.disabled && (!a.button || "click" !== a.type)) {
                                y = g(this);
                                y.context = this.ownerDocument || this;
                                for (m = a.target; m != this; m = m.parentNode || this) {
                                    G = {};
                                    P = [];
                                    y[0] = m;
                                    for (q = 0; q < e; q++) {
                                        w = d[q];
                                        Y = w.selector;
                                        if (G[Y] === c) {
                                            var la = G,
                                                z = Y,
                                                H;
                                            if (w.quick) {
                                                H = w.quick;
                                                var Ba = m.attributes || {};
                                                H = (!H[1] || m.nodeName.toLowerCase() === H[1]) && (!H[2] || (Ba.id || {}).value === H[2]) && (!H[3] || H[3].test((Ba["class"] || {}).value))
                                            } else H = y.is(Y);
                                            la[z] = H
                                        }
                                        G[Y] && P.push(w)
                                    }
                                    P.length && n.push({
                                        elem: m,
                                        matches: P
                                    })
                                }
                            }
                            d.length > e && n.push({
                                elem: this,
                                matches: d.slice(e)
                            });
                            for (q = 0; q < n.length && !a.isPropagationStopped(); q++) {
                                e = n[q];
                                a.currentTarget = e.elem;
                                for (d = 0; d < e.matches.length && !a.isImmediatePropagationStopped(); d++) if (w = e.matches[d], j || !a.namespace && !w.namespace || a.namespace_re && a.namespace_re.test(w.namespace)) a.data = w.data, a.handleObj = w, w = ((g.event.special[w.origType] || {}).handle || w.handler).apply(e.elem, f), w !== c && (a.result = w, !1 === w && (a.preventDefault(), a.stopPropagation()))
                            }
                            return a.result
                        },
                        props: "attrChange attrName relatedNode srcElement altKey bubbles cancelable ctrlKey currentTarget eventPhase metaKey relatedTarget shiftKey target timeStamp view which".split(" "),
                        fixHooks: {},
                        keyHooks: {
                            props: ["char", "charCode", "key", "keyCode"],
                            filter: function(a, b) {
                                return null == a.which && (a.which = null != b.charCode ? b.charCode : b.keyCode), a
                            }
                        },
                        mouseHooks: {
                            props: "button buttons clientX clientY fromElement offsetX offsetY pageX pageY screenX screenY toElement".split(" "),
                            filter: function(a, b) {
                                var d, g, e, f = b.button,
                                    j = b.fromElement;
                                return null == a.pageX && null != b.clientX && (d = a.target.ownerDocument || v, g = d.documentElement, e = d.body, a.pageX = b.clientX + (g && g.scrollLeft || e && e.scrollLeft || 0) - (g && g.clientLeft || e && e.clientLeft || 0), a.pageY = b.clientY + (g && g.scrollTop || e && e.scrollTop || 0) - (g && g.clientTop || e && e.clientTop || 0)), !a.relatedTarget && j && (a.relatedTarget = j === a.target ? b.toElement : j), !a.which && f !== c && (a.which = f & 1 ? 1 : f & 2 ? 3 : f & 4 ? 2 : 0), a
                            }
                        },
                        fix: function(a) {
                            if (a[g.expando]) return a;
                            var b, d, e = a,
                                f = g.event.fixHooks[a.type] || {},
                                j = f.props ? this.props.concat(f.props) : this.props,
                                a = g.Event(e);
                            for (b = j.length; b;) d = j[--b], a[d] = e[d];
                            return a.target || (a.target = e.srcElement || v), 3 === a.target.nodeType && (a.target = a.target.parentNode), a.metaKey === c && (a.metaKey = a.ctrlKey), f.filter ? f.filter(a, e) : a
                        },
                        special: {
                            ready: {
                                setup: g.bindReady
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
                                setup: function(a, b, c) {
                                    g.isWindow(this) && (this.onbeforeunload = c)
                                },
                                teardown: function(a, b) {
                                    this.onbeforeunload === b && (this.onbeforeunload = null)
                                }
                            }
                        },
                        simulate: function(a, b, c, d) {
                            a = g.extend(new g.Event, c, {
                                type: a,
                                isSimulated: !0,
                                originalEvent: {}
                            });
                            d ? g.event.trigger(a, null, b) : g.event.dispatch.call(b, a);
                            a.isDefaultPrevented() && c.preventDefault()
                        }
                    };
                    g.event.handle = g.event.dispatch;
                    g.removeEvent = v.removeEventListener ?
                        function(a, b, c) {
                            a.removeEventListener && a.removeEventListener(b, c, !1)
                        } : function(a, b, c) {
                        a.detachEvent && a.detachEvent("on" + b, c)
                    };
                    g.Event = function(a, b) {
                        if (!(this instanceof g.Event)) return new g.Event(a, b);
                        a && a.type ? (this.originalEvent = a, this.type = a.type, this.isDefaultPrevented = a.defaultPrevented || !1 === a.returnValue || a.getPreventDefault && a.getPreventDefault() ? r : m) : this.type = a;
                        b && g.extend(this, b);
                        this.timeStamp = a && a.timeStamp || g.now();
                        this[g.expando] = !0
                    };
                    g.Event.prototype = {
                        preventDefault: function() {
                            this.isDefaultPrevented = r;
                            var a = this.originalEvent;
                            a && (a.preventDefault ? a.preventDefault() : a.returnValue = !1)
                        },
                        stopPropagation: function() {
                            this.isPropagationStopped = r;
                            var a = this.originalEvent;
                            a && (a.stopPropagation && a.stopPropagation(), a.cancelBubble = !0)
                        },
                        stopImmediatePropagation: function() {
                            this.isImmediatePropagationStopped = r;
                            this.stopPropagation()
                        },
                        isDefaultPrevented: m,
                        isPropagationStopped: m,
                        isImmediatePropagationStopped: m
                    };
                    g.each({
                        mouseenter: "mouseover",
                        mouseleave: "mouseout"
                    }, function(a, b) {
                        g.event.special[a] = {
                            delegateType: b,
                            bindType: b,
                            handle: function(a) {
                                var k = a.relatedTarget,
                                    c = a.handleObj,
                                    d;
                                if (!k || k !== this && !g.contains(this, k)) a.type = c.origType, d = c.handler.apply(this, arguments), a.type = b;
                                return d
                            }
                        }
                    });
                    g.support.submitBubbles || (g.event.special.submit = {
                        setup: function() {
                            if (g.nodeName(this, "form")) return !1;
                            g.event.add(this, "click._submit keypress._submit", function(a) {
                                a = a.target;
                                (a = g.nodeName(a, "input") || g.nodeName(a, "button") ? a.form : c) && !a._submit_attached && (g.event.add(a, "submit._submit", function(a) {
                                    this.parentNode && !a.isTrigger && g.event.simulate("submit", this.parentNode, a, !0)
                                }), a._submit_attached = !0)
                            })
                        },
                        teardown: function() {
                            if (g.nodeName(this, "form")) return !1;
                            g.event.remove(this, "._submit")
                        }
                    });
                    g.support.changeBubbles || (g.event.special.change = {
                        setup: function() {
                            if (ja.test(this.nodeName)) {
                                if ("checkbox" === this.type || "radio" === this.type) g.event.add(this, "propertychange._change", function(a) {
                                    "checked" === a.originalEvent.propertyName && (this._just_changed = !0)
                                }), g.event.add(this, "click._change", function(a) {
                                    this._just_changed && !a.isTrigger && (this._just_changed = !1, g.event.simulate("change", this, a, !0))
                                });
                                return !1
                            }
                            g.event.add(this, "beforeactivate._change", function(a) {
                                a = a.target;
                                ja.test(a.nodeName) && !a._change_attached && (g.event.add(a, "change._change", function(a) {
                                    this.parentNode && !a.isSimulated && !a.isTrigger && g.event.simulate("change", this.parentNode, a, !0)
                                }), a._change_attached = !0)
                            })
                        },
                        handle: function(a) {
                            var b = a.target;
                            if (this !== b || a.isSimulated || a.isTrigger || "radio" !== b.type && "checkbox" !== b.type) return a.handleObj.handler.apply(this, arguments)
                        },
                        teardown: function() {
                            return g.event.remove(this, "._change"), ja.test(this.nodeName)
                        }
                    });
                    g.support.focusinBubbles || g.each({
                        focus: "focusin",
                        blur: "focusout"
                    }, function(a, b) {
                        var c = 0,
                            d = function(a) {
                                g.event.simulate(b, a.target, g.event.fix(a), !0)
                            };
                        g.event.special[b] = {
                            setup: function() {
                                0 === c++ && v.addEventListener(a, d, !0)
                            },
                            teardown: function() {
                                0 === --c && v.removeEventListener(a, d, !0)
                            }
                        }
                    });
                    g.fn.extend({
                        on: function(a, b, d, e, f) {
                            var j, n;
                            if ("object" == typeof a) {
                                "string" != typeof b && (d = b, b = c);
                                for (n in a) this.on(n, b, d, a[n], f);
                                return this
                            }
                            null == d && null == e ? (e = b, d = b = c) : null == e && ("string" == typeof b ? (e = d, d = c) : (e = d, d = b, b = c));
                            if (!1 === e) e = m;
                            else if (!e) return this;
                            return 1 === f && (j = e, e = function(a) {
                                return g().off(a), j.apply(this, arguments)
                            }, e.guid = j.guid || (j.guid = g.guid++)), this.each(function() {
                                g.event.add(this, a, e, d, b)
                            })
                        },
                        one: function(a, b, c, d) {
                            return this.on.call(this, a, b, c, d, 1)
                        },
                        off: function(a, b, d) {
                            if (a && a.preventDefault && a.handleObj) {
                                var e = a.handleObj;
                                return g(a.delegateTarget).off(e.namespace ? e.type + "." + e.namespace : e.type, e.selector, e.handler), this
                            }
                            if ("object" == typeof a) {
                                for (e in a) this.off(e, b, a[e]);
                                return this
                            }
                            if (!1 === b || "function" == typeof b) d = b, b = c;
                            return !1 === d && (d = m), this.each(function() {
                                g.event.remove(this, a, d, b)
                            })
                        },
                        bind: function(a, b, c) {
                            return this.on(a, null, b, c)
                        },
                        unbind: function(a, b) {
                            return this.off(a, null, b)
                        },
                        live: function(a, b, c) {
                            return g(this.context).on(a, this.selector, b, c), this
                        },
                        die: function(a, b) {
                            return g(this.context).off(a, this.selector || "**", b), this
                        },
                        delegate: function(a, b, c, d) {
                            return this.on(b, a, c, d)
                        },
                        undelegate: function(a, b, c) {
                            return 1 == arguments.length ? this.off(a, "**") : this.off(b, a, c)
                        },
                        trigger: function(a, b) {
                            return this.each(function() {
                                g.event.trigger(a, b, this)
                            })
                        },
                        triggerHandler: function(a, b) {
                            if (this[0]) return g.event.trigger(a, b, this[0], !0)
                        },
                        toggle: function(a) {
                            var b = arguments,
                                c = a.guid || g.guid++,
                                d = 0,
                                e = function(c) {
                                    var e = (g._data(this, "lastToggle" + a.guid) || 0) % d;
                                    return g._data(this, "lastToggle" + a.guid, e + 1), c.preventDefault(), b[e].apply(this, arguments) || !1
                                };
                            for (e.guid = c; d < b.length;) b[d++].guid = c;
                            return this.click(e)
                        },
                        hover: function(a, b) {
                            return this.mouseenter(a).mouseleave(b || a)
                        }
                    });
                    g.each("blur focus focusin focusout load resize scroll unload click dblclick mousedown mouseup mousemove mouseover mouseout mouseenter mouseleave change select submit keydown keypress keyup error contextmenu".split(" "), function(a, b) {
                        g.fn[b] = function(a, k) {
                            return null == k && (k = a, a = null), 0 < arguments.length ? this.on(b, null, a, k) : this.trigger(b)
                        };
                        g.attrFn && (g.attrFn[b] = !0);
                        rd.test(b) && (g.event.fixHooks[b] = g.event.keyHooks);
                        sd.test(b) && (g.event.fixHooks[b] = g.event.mouseHooks)
                    });
                    var y = function(a, b, c, d, e, g) {
                            for (var e = 0, f = d.length; e < f; e++) {
                                var j = d[e];
                                if (j) {
                                    for (var n = !1, j = j[a]; j;) {
                                        if (j[Oa] === c) {
                                            n = d[j.sizset];
                                            break
                                        }
                                        1 === j.nodeType && !g && (j[Oa] = c, j.sizset = e);
                                        if (j.nodeName.toLowerCase() === b) {
                                            n = j;
                                            break
                                        }
                                        j = j[a]
                                    }
                                    d[e] = n
                                }
                            }
                        },
                        P = function(a, b, c, d, e, g) {
                            for (var e = 0, f = d.length; e < f; e++) {
                                var j = d[e];
                                if (j) {
                                    for (var n = !1, j = j[a]; j;) {
                                        if (j[Oa] === c) {
                                            n = d[j.sizset];
                                            break
                                        }
                                        if (1 === j.nodeType) if (g || (j[Oa] = c, j.sizset = e), "string" != typeof b) {
                                            if (j === b) {
                                                n = !0;
                                                break
                                            }
                                        } else if (0 < z.filter(b, [j]).length) {
                                            n = j;
                                            break
                                        }
                                        j = j[a]
                                    }
                                    d[e] = n
                                }
                            }
                        },
                        Sa = /((?:\((?:\([^()]+\)|[^()]+)+\)|\[(?:\[[^\[\]]*\]|['"][^'"]*['"]|[^\[\]'"]+)+\]|\\.|[^ >+~,(\[\\]+)+|[>+~])(\s*,\s*)?((?:.|\r|\n)*)/g,
                        Oa = "sizcache" + (Math.random() + "").replace(".", ""),
                        Uc = 0,
                        tc = Object.prototype.toString,
                        ga = !1,
                        db = !0,
                        H = /\\/g,
                        Wb = /\r\n/g,
                        Q = /\W/;
                    [0, 0].sort(function() {
                        return db = !1, 0
                    });
                    var z = function(a, b, c, d) {
                        var c = c || [],
                            e = b = b || v;
                        if (1 !== b.nodeType && 9 !== b.nodeType) return [];
                        if (!a || "string" != typeof a) return c;
                        var g, f, j, n, q, m, y = !0,
                            w = z.isXML(b),
                            G = [],
                            P = a;
                        do
                            if (Sa.exec(""), g = Sa.exec(P)) if (P = g[3], G.push(g[1]), g[2]) {
                                n = g[3];
                                break
                            }
                        while (g);
                        if (1 < G.length && Xa.exec(a)) if (2 === G.length && L.relative[G[0]]) f = gb(G[0] + G[1], b, d);
                        else for (f = L.relative[G[0]] ? [b] : z(G.shift(), b); G.length;) a = G.shift(), L.relative[a] && (a += G.shift()), f = gb(a, f, d);
                        else if (!d && 1 < G.length && 9 === b.nodeType && !w && L.match.ID.test(G[0]) && !L.match.ID.test(G[G.length - 1]) && (q = z.find(G.shift(), b, w), b = q.expr ? z.filter(q.expr, q.set)[0] : q.set[0]), b) {
                            q = d ? {
                                expr: G.pop(),
                                set: Va(d)
                            } : z.find(G.pop(), 1 !== G.length || "~" !== G[0] && "+" !== G[0] || !b.parentNode ? b : b.parentNode, w);
                            f = q.expr ? z.filter(q.expr, q.set) : q.set;
                            for (0 < G.length ? j = Va(f) : y = !1; G.length;) g = m = G.pop(), L.relative[m] ? g = G.pop() : m = "", null == g && (g = b), L.relative[m](j, g, w)
                        } else j = [];
                        j || (j = f);
                        j || z.error(m || a);
                        if ("[object Array]" === tc.call(j)) if (y) if (b && 1 === b.nodeType) for (a = 0; null != j[a]; a++) j[a] && (!0 === j[a] || 1 === j[a].nodeType && z.contains(b, j[a])) && c.push(f[a]);
                        else for (a = 0; null != j[a]; a++) j[a] && 1 === j[a].nodeType && c.push(f[a]);
                        else c.push.apply(c, j);
                        else Va(j, c);
                        return n && (z(n, e, c, d), z.uniqueSort(c)), c
                    };
                    z.uniqueSort = function(a) {
                        if (uc && (ga = db, a.sort(uc), ga)) for (var b = 1; b < a.length; b++) a[b] === a[b - 1] && a.splice(b--, 1);
                        return a
                    };
                    z.matches = function(a, b) {
                        return z(a, null, null, b)
                    };
                    z.matchesSelector = function(a, b) {
                        return 0 < z(b, null, null, [a]).length
                    };
                    z.find = function(a, b, c) {
                        var d, e, g, f, j, n;
                        if (!a) return [];
                        e = 0;
                        for (g = L.order.length; e < g; e++) if (j = L.order[e], f = L.leftMatch[j].exec(a)) if (n = f[1], f.splice(1, 1), "\\" !== n.substr(n.length - 1) && (f[1] = (f[1] || "").replace(H, ""), d = L.find[j](f, b, c), null != d)) {
                            a = a.replace(L.match[j], "");
                            break
                        }
                        return d || (d = "undefined" != typeof b.getElementsByTagName ? b.getElementsByTagName("*") : []), {
                            set: d,
                            expr: a
                        }
                    };
                    z.filter = function(a, b, d, e) {
                        for (var g, f, j, n, q, m, y, w, G = a, P = [], Y = b, la = b && b[0] && z.isXML(b[0]); a && b.length;) {
                            for (j in L.filter) if (null != (g = L.leftMatch[j].exec(a)) && g[2]) if (m = L.filter[j], q = g[1], f = !1, g.splice(1, 1), "\\" !== q.substr(q.length - 1)) {
                                Y === P && (P = []);
                                if (L.preFilter[j]) if (g = L.preFilter[j](g, Y, d, P, e, la)) {
                                    if (!0 === g) continue
                                } else f = n = !0;
                                if (g) for (y = 0; null != (q = Y[y]); y++) q && (n = m(q, g, y, Y), w = e ^ n, d && null != n ? w ? f = !0 : Y[y] = !1 : w && (P.push(q), f = !0));
                                if (n !== c) {
                                    d || (Y = P);
                                    a = a.replace(L.match[j], "");
                                    if (!f) return [];
                                    break
                                }
                            }
                            if (a === G) {
                                if (null != f) break;
                                z.error(a)
                            }
                            G = a
                        }
                        return Y
                    };
                    z.error = function(a) {
                        throw Error("Syntax error, unrecognized expression: " + a);
                    };
                    var ma = z.getText = function(a) {
                            var b, c;
                            b = a.nodeType;
                            var d = "";
                            if (b) if (1 === b || 9 === b) {
                                if ("string" == typeof a.textContent) return a.textContent;
                                if ("string" == typeof a.innerText) return a.innerText.replace(Wb, "");
                                for (a = a.firstChild; a; a = a.nextSibling) d += ma(a)
                            } else {
                                if (3 === b || 4 === b) return a.nodeValue
                            } else for (b = 0; c = a[b]; b++) 8 !== c.nodeType && (d += ma(c));
                            return d
                        },
                        L = z.selectors = {
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
                                href: function(a) {
                                    return a.getAttribute("href")
                                },
                                type: function(a) {
                                    return a.getAttribute("type")
                                }
                            },
                            relative: {
                                "+": function(a, b) {
                                    var c = "string" == typeof b,
                                        d = c && !Q.test(b),
                                        c = c && !d;
                                    d && (b = b.toLowerCase());
                                    for (var d = 0, e = a.length, g; d < e; d++) if (g = a[d]) {
                                        for (;
                                            (g = g.previousSibling) && 1 !== g.nodeType;);
                                        a[d] = c || g && g.nodeName.toLowerCase() === b ? g || !1 : g === b
                                    }
                                    c && z.filter(b, a, !0)
                                },
                                ">": function(a, b) {
                                    var c, d = "string" == typeof b,
                                        e = 0,
                                        g = a.length;
                                    if (d && !Q.test(b)) for (b = b.toLowerCase(); e < g; e++) {
                                        if (c = a[e]) c = c.parentNode, a[e] = c.nodeName.toLowerCase() === b ? c : !1
                                    } else {
                                        for (; e < g; e++)(c = a[e]) && (a[e] = d ? c.parentNode : c.parentNode === b);
                                        d && z.filter(b, a, !0)
                                    }
                                },
                                "": function(a, b, c) {
                                    var d, e = Uc++,
                                        g = P;
                                    "string" == typeof b && !Q.test(b) && (b = b.toLowerCase(), d = b, g = y);
                                    g("parentNode", b, e, a, d, c)
                                },
                                "~": function(a, b, c) {
                                    var d, e = Uc++,
                                        g = P;
                                    "string" == typeof b && !Q.test(b) && (b = b.toLowerCase(), d = b, g = y);
                                    g("previousSibling", b, e, a, d, c)
                                }
                            },
                            find: {
                                ID: function(a, b, c) {
                                    if ("undefined" != typeof b.getElementById && !c) return (a = b.getElementById(a[1])) && a.parentNode ? [a] : []
                                },
                                NAME: function(a, b) {
                                    if ("undefined" != typeof b.getElementsByName) {
                                        for (var c = [], d = b.getElementsByName(a[1]), e = 0, g = d.length; e < g; e++) d[e].getAttribute("name") === a[1] && c.push(d[e]);
                                        return 0 === c.length ? null : c
                                    }
                                },
                                TAG: function(a, b) {
                                    if ("undefined" != typeof b.getElementsByTagName) return b.getElementsByTagName(a[1])
                                }
                            },
                            preFilter: {
                                CLASS: function(a, b, c, d, e, g) {
                                    a = " " + a[1].replace(H, "") + " ";
                                    if (g) return a;
                                    for (var g = 0, f; null != (f = b[g]); g++) f && (e ^ (f.className && 0 <= (" " + f.className + " ").replace(/[\t\n\r]/g, " ").indexOf(a)) ? c || d.push(f) : c && (b[g] = !1));
                                    return !1
                                },
                                ID: function(a) {
                                    return a[1].replace(H, "")
                                },
                                TAG: function(a) {
                                    return a[1].replace(H, "").toLowerCase()
                                },
                                CHILD: function(a) {
                                    if ("nth" === a[1]) {
                                        a[2] || z.error(a[0]);
                                        a[2] = a[2].replace(/^\+|\s*/g, "");
                                        var b = /(-?)(\d*)(?:n([+\-]?\d*))?/.exec("even" === a[2] && "2n" || "odd" === a[2] && "2n+1" || !/\D/.test(a[2]) && "0n+" + a[2] || a[2]);
                                        a[2] = b[1] + (b[2] || 1) - 0;
                                        a[3] = b[3] - 0
                                    } else a[2] && z.error(a[0]);
                                    return a[0] = Uc++, a
                                },
                                ATTR: function(a, b, c, d, e, g) {
                                    b = a[1] = a[1].replace(H, "");
                                    return !g && L.attrMap[b] && (a[1] = L.attrMap[b]), a[4] = (a[4] || a[5] || "").replace(H, ""), "~=" === a[2] && (a[4] = " " + a[4] + " "), a
                                },
                                PSEUDO: function(a, b, c, d, e) {
                                    if ("not" === a[1]) {
                                        if (1 >= (Sa.exec(a[3]) || "").length && !/^\w/.test(a[3])) return a = z.filter(a[3], b, c, 1 ^ e), c || d.push.apply(d, a), !1;
                                        a[3] = z(a[3], null, null, b)
                                    } else if (L.match.POS.test(a[0]) || L.match.CHILD.test(a[0])) return !0;
                                    return a
                                },
                                POS: function(a) {
                                    return a.unshift(!0), a
                                }
                            },
                            filters: {
                                enabled: function(a) {
                                    return !1 === a.disabled && "hidden" !== a.type
                                },
                                disabled: function(a) {
                                    return !0 === a.disabled
                                },
                                checked: function(a) {
                                    return !0 === a.checked
                                },
                                selected: function(a) {
                                    return a.parentNode && a.parentNode.selectedIndex, !0 === a.selected
                                },
                                parent: function(a) {
                                    return !!a.firstChild
                                },
                                empty: function(a) {
                                    return !a.firstChild
                                },
                                has: function(a, b, c) {
                                    return !!z(c[3], a).length
                                },
                                header: function(a) {
                                    return /h\d/i.test(a.nodeName)
                                },
                                text: function(a) {
                                    var b = a.getAttribute("type"),
                                        c = a.type;
                                    return "input" === a.nodeName.toLowerCase() && "text" === c && (b === c || null === b)
                                },
                                radio: function(a) {
                                    return "input" === a.nodeName.toLowerCase() && "radio" === a.type
                                },
                                checkbox: function(a) {
                                    return "input" === a.nodeName.toLowerCase() && "checkbox" === a.type
                                },
                                file: function(a) {
                                    return "input" === a.nodeName.toLowerCase() && "file" === a.type
                                },
                                password: function(a) {
                                    return "input" === a.nodeName.toLowerCase() && "password" === a.type
                                },
                                submit: function(a) {
                                    var b = a.nodeName.toLowerCase();
                                    return ("input" === b || "button" === b) && "submit" === a.type
                                },
                                image: function(a) {
                                    return "input" === a.nodeName.toLowerCase() && "image" === a.type
                                },
                                reset: function(a) {
                                    var b = a.nodeName.toLowerCase();
                                    return ("input" === b || "button" === b) && "reset" === a.type
                                },
                                button: function(a) {
                                    var b = a.nodeName.toLowerCase();
                                    return "input" === b && "button" === a.type || "button" === b
                                },
                                input: function(a) {
                                    return /input|select|textarea|button/i.test(a.nodeName)
                                },
                                focus: function(a) {
                                    return a === a.ownerDocument.activeElement
                                }
                            },
                            setFilters: {
                                first: function(a, b) {
                                    return 0 === b
                                },
                                last: function(a, b, c, d) {
                                    return b === d.length - 1
                                },
                                even: function(a, b) {
                                    return 0 === b % 2
                                },
                                odd: function(a, b) {
                                    return 1 === b % 2
                                },
                                lt: function(a, b, c) {
                                    return b < c[3] - 0
                                },
                                gt: function(a, b, c) {
                                    return b > c[3] - 0
                                },
                                nth: function(a, b, c) {
                                    return c[3] - 0 === b
                                },
                                eq: function(a, b, c) {
                                    return c[3] - 0 === b
                                }
                            },
                            filter: {
                                PSEUDO: function(a, b, c, d) {
                                    var e = b[1],
                                        g = L.filters[e];
                                    if (g) return g(a, c, b, d);
                                    if ("contains" === e) return 0 <= (a.textContent || a.innerText || ma([a]) || "").indexOf(b[3]);
                                    if ("not" === e) {
                                        b = b[3];
                                        c = 0;
                                        for (d = b.length; c < d; c++) if (b[c] === a) return !1;
                                        return !0
                                    }
                                    z.error(e)
                                },
                                CHILD: function(a, b) {
                                    var c, d, e, g, f, j;
                                    c = b[1];
                                    var n = a;
                                    switch (c) {
                                        case "only":
                                        case "first":
                                            for (; n = n.previousSibling;) if (1 === n.nodeType) return !1;
                                            if ("first" === c) return !0;
                                            n = a;
                                        case "last":
                                            for (; n = n.nextSibling;) if (1 === n.nodeType) return !1;
                                            return !0;
                                        case "nth":
                                            c = b[2];
                                            d = b[3];
                                            if (1 === c && 0 === d) return !0;
                                            e = b[0];
                                            if ((g = a.parentNode) && (g[Oa] !== e || !a.nodeIndex)) {
                                                f = 0;
                                                for (n = g.firstChild; n; n = n.nextSibling) 1 === n.nodeType && (n.nodeIndex = ++f);
                                                g[Oa] = e
                                            }
                                            return j = a.nodeIndex - d, 0 === c ? 0 === j : 0 === j % c && 0 <= j / c
                                    }
                                },
                                ID: function(a, b) {
                                    return 1 === a.nodeType && a.getAttribute("id") === b
                                },
                                TAG: function(a, b) {
                                    return "*" === b && 1 === a.nodeType || !! a.nodeName && a.nodeName.toLowerCase() === b
                                },
                                CLASS: function(a, b) {
                                    return -1 < (" " + (a.className || a.getAttribute("class")) + " ").indexOf(b)
                                },
                                ATTR: function(a, b) {
                                    var c = b[1],
                                        c = z.attr ? z.attr(a, c) : L.attrHandle[c] ? L.attrHandle[c](a) : null != a[c] ? a[c] : a.getAttribute(c),
                                        d = c + "",
                                        e = b[2],
                                        g = b[4];
                                    return null == c ? "!=" === e : !e && z.attr ? null != c : "=" === e ? d === g : "*=" === e ? 0 <= d.indexOf(g) : "~=" === e ? 0 <= (" " + d + " ").indexOf(g) : g ? "!=" === e ? d !== g : "^=" === e ? 0 === d.indexOf(g) : "$=" === e ? d.substr(d.length - g.length) === g : "|=" === e ? d === g || d.substr(0, g.length + 1) === g + "-" : !1 : d && !1 !== c
                                },
                                POS: function(a, b, c, d) {
                                    var e = L.setFilters[b[2]];
                                    if (e) return e(a, c, b, d)
                                }
                            }
                        },
                        Xa = L.match.POS,
                        td = function(a, b) {
                            return "\\" + (b - 0 + 1)
                        },
                        Xb;
                    for (Xb in L.match) L.match[Xb] = RegExp(L.match[Xb].source + /(?![^\[]*\])(?![^\(]*\))/.source), L.leftMatch[Xb] = RegExp(/(^(?:.|\r|\n)*?)/.source + L.match[Xb].source.replace(/\\(\d+)/g, td));
                    var Va = function(a, b) {
                        return a = Array.prototype.slice.call(a, 0), b ? (b.push.apply(b, a), b) : a
                    };
                    try {
                        Array.prototype.slice.call(v.documentElement.childNodes, 0)[0].nodeType
                    } catch (Ed) {
                        Va = function(a, b) {
                            var c = 0,
                                d = b || [];
                            if ("[object Array]" === tc.call(a)) Array.prototype.push.apply(d, a);
                            else if ("number" == typeof a.length) for (var e = a.length; c < e; c++) d.push(a[c]);
                            else for (; a[c]; c++) d.push(a[c]);
                            return d
                        }
                    }
                    var uc, Yb;
                    v.documentElement.compareDocumentPosition ? uc = function(a, b) {
                        return a === b ? (ga = !0, 0) : !a.compareDocumentPosition || !b.compareDocumentPosition ? a.compareDocumentPosition ? -1 : 1 : a.compareDocumentPosition(b) & 4 ? -1 : 1
                    } : (uc = function(a, b) {
                        if (a === b) return ga = !0, 0;
                        if (a.sourceIndex && b.sourceIndex) return a.sourceIndex - b.sourceIndex;
                        var c, d, e = [],
                            g = [];
                        c = a.parentNode;
                        d = b.parentNode;
                        var f = c;
                        if (c === d) return Yb(a, b);
                        if (!c) return -1;
                        if (!d) return 1;
                        for (; f;) e.unshift(f), f = f.parentNode;
                        for (f = d; f;) g.unshift(f), f = f.parentNode;
                        c = e.length;
                        d = g.length;
                        for (f = 0; f < c && f < d; f++) if (e[f] !== g[f]) return Yb(e[f], g[f]);
                        return f === c ? Yb(a, g[f], -1) : Yb(e[f], b, 1)
                    }, Yb = function(a, b, c) {
                        if (a === b) return c;
                        for (a = a.nextSibling; a;) {
                            if (a === b) return -1;
                            a = a.nextSibling
                        }
                        return 1
                    });
                    var vc = v.createElement("div"),
                        hd = "script" + (new Date).getTime(),
                        wc = v.documentElement;
                    vc.innerHTML = "<a name='" + hd + "'/>";
                    wc.insertBefore(vc, wc.firstChild);
                    v.getElementById(hd) && (L.find.ID = function(a, b, d) {
                        if ("undefined" != typeof b.getElementById && !d) return (b = b.getElementById(a[1])) ? b.id === a[1] || "undefined" != typeof b.getAttributeNode && b.getAttributeNode("id").nodeValue === a[1] ? [b] : c : []
                    }, L.filter.ID = function(a, b) {
                        var c = "undefined" != typeof a.getAttributeNode && a.getAttributeNode("id");
                        return 1 === a.nodeType && c && c.nodeValue === b
                    });
                    wc.removeChild(vc);
                    var wc = vc = null,
                        sb = v.createElement("div");
                    sb.appendChild(v.createComment(""));
                    0 < sb.getElementsByTagName("*").length && (L.find.TAG = function(a, b) {
                        var c = b.getElementsByTagName(a[1]);
                        if ("*" === a[1]) {
                            for (var d = [], e = 0; c[e]; e++) 1 === c[e].nodeType && d.push(c[e]);
                            c = d
                        }
                        return c
                    });
                    sb.innerHTML = "<a href='#'></a>";
                    sb.firstChild && "undefined" != typeof sb.firstChild.getAttribute && "#" !== sb.firstChild.getAttribute("href") && (L.attrHandle.href = function(a) {
                        return a.getAttribute("href", 2)
                    });
                    sb = null;
                    if (v.querySelectorAll) {
                        var Vc = z,
                            xc = v.createElement("div");
                        xc.innerHTML = "<p class='TEST'></p>";
                        if (!(xc.querySelectorAll && 0 === xc.querySelectorAll(".TEST").length)) {
                            var z = function(a, b, c, d) {
                                    b = b || v;
                                    if (!d && !z.isXML(b)) {
                                        var e = /^(\w+$)|^\.([\w\-]+$)|^#([\w\-]+$)/.exec(a);
                                        if (e && (1 === b.nodeType || 9 === b.nodeType)) {
                                            if (e[1]) return Va(b.getElementsByTagName(a), c);
                                            if (e[2] && L.find.CLASS && b.getElementsByClassName) return Va(b.getElementsByClassName(e[2]), c)
                                        }
                                        if (9 === b.nodeType) {
                                            if ("body" === a && b.body) return Va([b.body], c);
                                            if (e && e[3]) {
                                                var g = b.getElementById(e[3]);
                                                if (!g || !g.parentNode) return Va([], c);
                                                if (g.id === e[3]) return Va([g], c)
                                            }
                                            try {
                                                return Va(b.querySelectorAll(a), c)
                                            } catch (f) {}
                                        } else if (1 === b.nodeType && "object" !== b.nodeName.toLowerCase()) {
                                            var e = b,
                                                j = (g = b.getAttribute("id")) || "__sizzle__",
                                                n = b.parentNode,
                                                q = /^\s*[+~]/.test(a);
                                            g ? j = j.replace(/'/g, "\\$&") : b.setAttribute("id", j);
                                            q && n && (b = b.parentNode);
                                            try {
                                                if (!q || n) return Va(b.querySelectorAll("[id='" + j + "'] " + a), c)
                                            } catch (m) {} finally {
                                                g || e.removeAttribute("id")
                                            }
                                        }
                                    }
                                    return Vc(a, b, c, d)
                                },
                                Wc;
                            for (Wc in Vc) z[Wc] = Vc[Wc];
                            xc = null
                        }
                    }
                    var yc = v.documentElement,
                        Ua = yc.matchesSelector || yc.mozMatchesSelector || yc.webkitMatchesSelector || yc.msMatchesSelector;
                    if (Ua) {
                        var ib = !Ua.call(v.createElement("div"), "div"),
                            eb = !1;
                        try {
                            Ua.call(v.documentElement, "[test!='']:sizzle")
                        } catch (Ab) {
                            eb = !0
                        }
                        z.matchesSelector = function(a, b) {
                            b = b.replace(/\=\s*([^'"\]]*)\s*\]/g, "='$1']");
                            if (!z.isXML(a)) try {
                                if (eb || !L.match.PSEUDO.test(b) && !/!=/.test(b)) {
                                    var c = Ua.call(a, b);
                                    if (c || !ib || a.document && 11 !== a.document.nodeType) return c
                                }
                            } catch (d) {}
                            return 0 < z(b, null, null, [a]).length
                        }
                    }
                    var Qa = v.createElement("div");
                    Qa.innerHTML = "<div class='test e'></div><div class='test'></div>";
                    Qa.getElementsByClassName && 0 !== Qa.getElementsByClassName("e").length && (Qa.lastChild.className = "e", 1 !== Qa.getElementsByClassName("e").length && (L.order.splice(1, 0, "CLASS"), L.find.CLASS = function(a, b, c) {
                        if ("undefined" != typeof b.getElementsByClassName && !c) return b.getElementsByClassName(a[1])
                    }, Qa = null));
                    v.documentElement.contains ? z.contains = function(a, b) {
                        return a !== b && (a.contains ? a.contains(b) : !0)
                    } : v.documentElement.compareDocumentPosition ? z.contains = function(a, b) {
                        return !!(a.compareDocumentPosition(b) & 16)
                    } : z.contains = function() {
                        return !1
                    };
                    z.isXML = function(a) {
                        return (a = (a ? a.ownerDocument || a : 0).documentElement) ? "HTML" !== a.nodeName : !1
                    };
                    var gb = function(a, b, c) {
                        for (var d, e = [], g = "", b = b.nodeType ? [b] : b; d = L.match.PSEUDO.exec(a);) g += d[0], a = a.replace(L.match.PSEUDO, "");
                        a = L.relative[a] ? a + "*" : a;
                        d = 0;
                        for (var f = b.length; d < f; d++) z(a, b[d], e, c);
                        return z.filter(g, e)
                    };
                    z.attr = g.attr;
                    z.selectors.attrMap = {};
                    g.find = z;
                    g.expr = z.selectors;
                    g.expr[":"] = g.expr.filters;
                    g.unique = z.uniqueSort;
                    g.text = z.getText;
                    g.isXMLDoc = z.isXML;
                    g.contains = z.contains;
                    var lb = /Until$/,
                        pb = /^(?:parents|prevUntil|prevAll)/,
                        xb = /,/,
                        yb = /^.[^:#\[\.,]*$/,
                        zb = Array.prototype.slice,
                        jb = g.expr.match.POS,
                        Bb = {
                            children: !0,
                            contents: !0,
                            next: !0,
                            prev: !0
                        };
                    g.fn.extend({
                        find: function(a) {
                            var b = this,
                                c, d;
                            if ("string" != typeof a) return g(a).filter(function() {
                                c = 0;
                                for (d = b.length; c < d; c++) if (g.contains(b[c], this)) return !0
                            });
                            var e = this.pushStack("", "find", a),
                                f, j, n;
                            c = 0;
                            for (d = this.length; c < d; c++) if (f = e.length, g.find(a, this[c], e), 0 < c) for (j = f; j < e.length; j++) for (n = 0; n < f; n++) if (e[n] === e[j]) {
                                e.splice(j--, 1);
                                break
                            }
                            return e
                        },
                        has: function(a) {
                            var b = g(a);
                            return this.filter(function() {
                                for (var a = 0, c = b.length; a < c; a++) if (g.contains(this, b[a])) return !0
                            })
                        },
                        not: function(a) {
                            return this.pushStack(x(this, a, !1), "not", a)
                        },
                        filter: function(a) {
                            return this.pushStack(x(this, a, !0), "filter", a)
                        },
                        is: function(a) {
                            return !!a && ("string" == typeof a ? jb.test(a) ? 0 <= g(a, this.context).index(this[0]) : 0 < g.filter(a, this).length : 0 < this.filter(a).length)
                        },
                        closest: function(a, b) {
                            var c = [],
                                d, e, f = this[0];
                            if (g.isArray(a)) {
                                for (e = 1; f && f.ownerDocument && f !== b;) {
                                    for (d = 0; d < a.length; d++) g(f).is(a[d]) && c.push({
                                        selector: a[d],
                                        elem: f,
                                        level: e
                                    });
                                    f = f.parentNode;
                                    e++
                                }
                                return c
                            }
                            var j = jb.test(a) || "string" != typeof a ? g(a, b || this.context) : 0;
                            d = 0;
                            for (e = this.length; d < e; d++) for (f = this[d]; f;) {
                                if (j ? -1 < j.index(f) : g.find.matchesSelector(f, a)) {
                                    c.push(f);
                                    break
                                }
                                f = f.parentNode;
                                if (!f || !f.ownerDocument || f === b || 11 === f.nodeType) break
                            }
                            return c = 1 < c.length ? g.unique(c) : c, this.pushStack(c, "closest", a)
                        },
                        index: function(a) {
                            return a ? "string" == typeof a ? g.inArray(this[0], g(a)) : g.inArray(a.jquery ? a[0] : a, this) : this[0] && this[0].parentNode ? this.prevAll().length : -1
                        },
                        add: function(a, b) {
                            var c = "string" == typeof a ? g(a, b) : g.makeArray(a && a.nodeType ? [a] : a),
                                d = g.merge(this.get(), c);
                            return this.pushStack(!c[0] || !c[0].parentNode || 11 === c[0].parentNode.nodeType || !d[0] || !d[0].parentNode || 11 === d[0].parentNode.nodeType ? d : g.unique(d))
                        },
                        andSelf: function() {
                            return this.add(this.prevObject)
                        }
                    });
                    g.each({
                        parent: function(a) {
                            return (a = a.parentNode) && 11 !== a.nodeType ? a : null
                        },
                        parents: function(a) {
                            return g.dir(a, "parentNode")
                        },
                        parentsUntil: function(a, b, c) {
                            return g.dir(a, "parentNode", c)
                        },
                        next: function(a) {
                            return g.nth(a, 2, "nextSibling")
                        },
                        prev: function(a) {
                            return g.nth(a, 2, "previousSibling")
                        },
                        nextAll: function(a) {
                            return g.dir(a, "nextSibling")
                        },
                        prevAll: function(a) {
                            return g.dir(a, "previousSibling")
                        },
                        nextUntil: function(a, b, c) {
                            return g.dir(a, "nextSibling", c)
                        },
                        prevUntil: function(a, b, c) {
                            return g.dir(a, "previousSibling", c)
                        },
                        siblings: function(a) {
                            return g.sibling(a.parentNode.firstChild, a)
                        },
                        children: function(a) {
                            return g.sibling(a.firstChild)
                        },
                        contents: function(a) {
                            return g.nodeName(a, "iframe") ? a.contentDocument || a.contentWindow.document : g.makeArray(a.childNodes)
                        }
                    }, function(a, b) {
                        g.fn[a] = function(c, d) {
                            var e = g.map(this, b, c);
                            return lb.test(a) || (d = c), d && "string" == typeof d && (e = g.filter(d, e)), e = 1 < this.length && !Bb[a] ? g.unique(e) : e, (1 < this.length || xb.test(d)) && pb.test(a) && (e = e.reverse()), this.pushStack(e, a, zb.call(arguments).join(","))
                        }
                    });
                    g.extend({
                        filter: function(a, b, c) {
                            return c && (a = ":not(" + a + ")"), 1 === b.length ? g.find.matchesSelector(b[0], a) ? [b[0]] : [] : g.find.matches(a, b)
                        },
                        dir: function(a, b, d) {
                            for (var e = [], a = a[b]; a && 9 !== a.nodeType && (d === c || 1 !== a.nodeType || !g(a).is(d));) 1 === a.nodeType && e.push(a), a = a[b];
                            return e
                        },
                        nth: function(a, b, c) {
                            for (var b = b || 1, d = 0; a && !(1 === a.nodeType && ++d === b); a = a[c]);
                            return a
                        },
                        sibling: function(a, b) {
                            for (var c = []; a; a = a.nextSibling) 1 === a.nodeType && a !== b && c.push(a);
                            return c
                        }
                    });
                    var kb = "abbr|article|aside|audio|canvas|datalist|details|figcaption|figure|footer|header|hgroup|mark|meter|nav|output|progress|section|summary|time|video",
                        Db = / jQuery\d+="(?:\d+|null)"/g,
                        hb = /^\s+/,
                        mb = /<(?!area|br|col|embed|hr|img|input|link|meta|param)(([\w:]+)[^>]*)\/>/ig,
                        nb = /<([\w:]+)/,
                        Fb = /<tbody/i,
                        Gb = /<|&#?\w+;/,
                        Lb = /<(?:script|style)/i,
                        Mb = /<(?:script|object|embed|option|style)/i,
                        wb = RegExp("<(?:" + kb + ")", "i"),
                        Cb = /checked\s*(?:[^=]|=\s*.checked.)/i,
                        Ob = /\/(java|ecma)script/i,
                        Rb = /^\s*<!(?:\[CDATA\[|\-\-)/,
                        Pa = {
                            option: [1, "<select multiple='multiple'>", "</select>"],
                            legend: [1, "<fieldset>", "</fieldset>"],
                            thead: [1, "<table>", "</table>"],
                            tr: [2, "<table><tbody>", "</tbody></table>"],
                            td: [3, "<table><tbody><tr>", "</tr></tbody></table>"],
                            col: [2, "<table><tbody></tbody><colgroup>", "</colgroup></table>"],
                            area: [1, "<map>", "</map>"],
                            _default: [0, "", ""]
                        },
                        Eb = p(v);
                    Pa.optgroup = Pa.option;
                    Pa.tbody = Pa.tfoot = Pa.colgroup = Pa.caption = Pa.thead;
                    Pa.th = Pa.td;
                    g.support.htmlSerialize || (Pa._default = [1, "div<div>", "</div>"]);
                    g.fn.extend({
                        text: function(a) {
                            return g.isFunction(a) ? this.each(function(b) {
                                var c = g(this);
                                c.text(a.call(this, b, c.text()))
                            }) : "object" != typeof a && a !== c ? this.empty().append((this[0] && this[0].ownerDocument || v).createTextNode(a)) : g.text(this)
                        },
                        wrapAll: function(a) {
                            if (g.isFunction(a)) return this.each(function(b) {
                                g(this).wrapAll(a.call(this, b))
                            });
                            if (this[0]) {
                                var b = g(a, this[0].ownerDocument).eq(0).clone(!0);
                                this[0].parentNode && b.insertBefore(this[0]);
                                b.map(function() {
                                    for (var a = this; a.firstChild && 1 === a.firstChild.nodeType;) a = a.firstChild;
                                    return a
                                }).append(this)
                            }
                            return this
                        },
                        wrapInner: function(a) {
                            return g.isFunction(a) ? this.each(function(b) {
                                g(this).wrapInner(a.call(this, b))
                            }) : this.each(function() {
                                var b = g(this),
                                    c = b.contents();
                                c.length ? c.wrapAll(a) : b.append(a)
                            })
                        },
                        wrap: function(a) {
                            var b = g.isFunction(a);
                            return this.each(function(c) {
                                g(this).wrapAll(b ? a.call(this, c) : a)
                            })
                        },
                        unwrap: function() {
                            return this.parent().each(function() {
                                g.nodeName(this, "body") || g(this).replaceWith(this.childNodes)
                            }).end()
                        },
                        append: function() {
                            return this.domManip(arguments, !0, function(a) {
                                1 === this.nodeType && this.appendChild(a)
                            })
                        },
                        prepend: function() {
                            return this.domManip(arguments, !0, function(a) {
                                1 === this.nodeType && this.insertBefore(a, this.firstChild)
                            })
                        },
                        before: function() {
                            if (this[0] && this[0].parentNode) return this.domManip(arguments, !1, function(a) {
                                this.parentNode.insertBefore(a, this)
                            });
                            if (arguments.length) {
                                var a = g.clean(arguments);
                                return a.push.apply(a, this.toArray()), this.pushStack(a, "before", arguments)
                            }
                        },
                        after: function() {
                            if (this[0] && this[0].parentNode) return this.domManip(arguments, !1, function(a) {
                                this.parentNode.insertBefore(a, this.nextSibling)
                            });
                            if (arguments.length) {
                                var a = this.pushStack(this, "after", arguments);
                                return a.push.apply(a, g.clean(arguments)), a
                            }
                        },
                        remove: function(a, b) {
                            for (var c = 0, d; null != (d = this[c]); c++) if (!a || g.filter(a, [d]).length)!b && 1 === d.nodeType && (g.cleanData(d.getElementsByTagName("*")), g.cleanData([d])), d.parentNode && d.parentNode.removeChild(d);
                            return this
                        },
                        empty: function() {
                            for (var a = 0, b; null != (b = this[a]); a++) for (1 === b.nodeType && g.cleanData(b.getElementsByTagName("*")); b.firstChild;) b.removeChild(b.firstChild);
                            return this
                        },
                        clone: function(a, b) {
                            return a = null == a ? !1 : a, b = null == b ? a : b, this.map(function() {
                                return g.clone(this, a, b)
                            })
                        },
                        html: function(a) {
                            if (a === c) return this[0] && 1 === this[0].nodeType ? this[0].innerHTML.replace(Db, "") : null;
                            if ("string" == typeof a && !Lb.test(a) && (g.support.leadingWhitespace || !hb.test(a)) && !Pa[(nb.exec(a) || ["", ""])[1].toLowerCase()]) {
                                a = a.replace(mb, "<$1></$2>");
                                try {
                                    for (var b = 0, d = this.length; b < d; b++) 1 === this[b].nodeType && (g.cleanData(this[b].getElementsByTagName("*")), this[b].innerHTML = a)
                                } catch (e) {
                                    this.empty().append(a)
                                }
                            } else g.isFunction(a) ? this.each(function(b) {
                                var c = g(this);
                                c.html(a.call(this, b, c.html()))
                            }) : this.empty().append(a);
                            return this
                        },
                        replaceWith: function(a) {
                            return this[0] && this[0].parentNode ? g.isFunction(a) ? this.each(function(b) {
                                var c = g(this),
                                    d = c.html();
                                c.replaceWith(a.call(this, b, d))
                            }) : ("string" != typeof a && (a = g(a).detach()), this.each(function() {
                                var b = this.nextSibling,
                                    c = this.parentNode;
                                g(this).remove();
                                b ? g(b).before(a) : g(c).append(a)
                            })) : this.length ? this.pushStack(g(g.isFunction(a) ? a() : a), "replaceWith", a) : this
                        },
                        detach: function(a) {
                            return this.remove(a, !0)
                        },
                        domManip: function(a, b, d) {
                            var e, f, j, n = a[0],
                                q = [];
                            if (!g.support.checkClone && 3 === arguments.length && "string" == typeof n && Cb.test(n)) return this.each(function() {
                                g(this).domManip(a, b, d, !0)
                            });
                            if (g.isFunction(n)) return this.each(function(e) {
                                var f = g(this);
                                a[0] = n.call(this, e, b ? f.html() : c);
                                f.domManip(a, b, d)
                            });
                            if (this[0]) {
                                j = n && n.parentNode;
                                g.support.parentNode && j && 11 === j.nodeType && j.childNodes.length === this.length ? e = {
                                    fragment: j
                                } : e = g.buildFragment(a, this, q);
                                j = e.fragment;
                                1 === j.childNodes.length ? f = j = j.firstChild : f = j.firstChild;
                                if (f) {
                                    b = b && g.nodeName(f, "tr");
                                    f = 0;
                                    for (var m = this.length, y = m - 1; f < m; f++) d.call(b ? g.nodeName(this[f], "table") ? this[f].getElementsByTagName("tbody")[0] || this[f].appendChild(this[f].ownerDocument.createElement("tbody")) : this[f] : this[f], e.cacheable || 1 < m && f < y ? g.clone(j, !0, !0) : j)
                                }
                                q.length && g.each(q, O)
                            }
                            return this
                        }
                    });
                    g.buildFragment = function(a, b, c) {
                        var d, e, f, j, n = a[0];
                        return b && b[0] && (j = b[0].ownerDocument || b[0]), j.createDocumentFragment || (j = v), 1 === a.length && "string" == typeof n && 512 > n.length && j === v && "<" === n.charAt(0) && !Mb.test(n) && (g.support.checkClone || !Cb.test(n)) && (g.support.html5Clone || !wb.test(n)) && (e = !0, f = g.fragments[n], f && 1 !== f && (d = f)), d || (d = j.createDocumentFragment(), g.clean(a, j, d, c)), e && (g.fragments[n] = f ? d : 1), {
                            fragment: d,
                            cacheable: e
                        }
                    };
                    g.fragments = {};
                    g.each({
                        appendTo: "append",
                        prependTo: "prepend",
                        insertBefore: "before",
                        insertAfter: "after",
                        replaceAll: "replaceWith"
                    }, function(a, b) {
                        g.fn[a] = function(c) {
                            var d = [],
                                c = g(c),
                                e = 1 === this.length && this[0].parentNode;
                            if (e && 11 === e.nodeType && 1 === e.childNodes.length && 1 === c.length) return c[b](this[0]), this;
                            for (var e = 0, f = c.length; e < f; e++) {
                                var j = (0 < e ? this.clone(!0) : this).get();
                                g(c[e])[b](j);
                                d = d.concat(j)
                            }
                            return this.pushStack(d, a, c.selector)
                        }
                    });
                    g.extend({
                        clone: function(a, b, c) {
                            var d, e, f;
                            g.support.html5Clone || !wb.test("<" + a.nodeName) ? d = a.cloneNode(!0) : (d = v.createElement("div"), d = (Eb.appendChild(d), d.innerHTML = a.outerHTML, d.firstChild));
                            var j = d;
                            if ((!g.support.noCloneEvent || !g.support.noCloneChecked) && (1 === a.nodeType || 11 === a.nodeType) && !g.isXMLDoc(a)) {
                                sa(a, j);
                                d = C(a);
                                e = C(j);
                                for (f = 0; d[f]; ++f) e[f] && sa(d[f], e[f])
                            }
                            if (b && (A(a, j), c)) {
                                d = C(a);
                                e = C(j);
                                for (f = 0; d[f]; ++f) A(d[f], e[f])
                            }
                            return j
                        },
                        clean: function(a, b, c, d) {
                            b = b || v;
                            "undefined" == typeof b.createElement && (b = b.ownerDocument || b[0] && b[0].ownerDocument || v);
                            for (var e = [], f, j = 0, n; null != (n = a[j]); j++) if ("number" == typeof n && (n += ""), n) {
                                if ("string" == typeof n) if (Gb.test(n)) {
                                    n = n.replace(mb, "<$1></$2>");
                                    f = (nb.exec(n) || ["", ""])[1].toLowerCase();
                                    var q = Pa[f] || Pa._default,
                                        m = q[0],
                                        y = b.createElement("div");
                                    b === v ? Eb.appendChild(y) : p(b).appendChild(y);
                                    for (y.innerHTML = q[1] + n + q[2]; m--;) y = y.lastChild;
                                    if (!g.support.tbody) {
                                        m = Fb.test(n);
                                        q = "table" === f && !m ? y.firstChild && y.firstChild.childNodes : "<table>" === q[1] && !m ? y.childNodes : [];
                                        for (f = q.length - 1; 0 <= f; --f) g.nodeName(q[f], "tbody") && !q[f].childNodes.length && q[f].parentNode.removeChild(q[f])
                                    }!g.support.leadingWhitespace && hb.test(n) && y.insertBefore(b.createTextNode(hb.exec(n)[0]), y.firstChild);
                                    n = y.childNodes
                                } else n = b.createTextNode(n);
                                var w;
                                if (!g.support.appendChecked) if (n[0] && "number" == typeof(w = n.length)) for (f = 0; f < w; f++) E(n[f]);
                                else E(n);
                                n.nodeType ? e.push(n) : e = g.merge(e, n)
                            }
                            if (c) {
                                a = function(a) {
                                    return !a.type || Ob.test(a.type)
                                };
                                for (j = 0; e[j]; j++) d && g.nodeName(e[j], "script") && (!e[j].type || "text/javascript" === e[j].type.toLowerCase()) ? d.push(e[j].parentNode ? e[j].parentNode.removeChild(e[j]) : e[j]) : (1 === e[j].nodeType && (b = g.grep(e[j].getElementsByTagName("script"), a), e.splice.apply(e, [j + 1, 0].concat(b))), c.appendChild(e[j]))
                            }
                            return e
                        },
                        cleanData: function(a) {
                            for (var b, c, d = g.cache, e = g.event.special, f = g.support.deleteExpando, j = 0, n; null != (n = a[j]); j++) if (!n.nodeName || !g.noData[n.nodeName.toLowerCase()]) if (c = n[g.expando]) {
                                if ((b = d[c]) && b.events) {
                                    for (var q in b.events) e[q] ? g.event.remove(n, q) : g.removeEvent(n, q, b.handle);
                                    b.handle && (b.handle.elem = null)
                                }
                                f ? delete n[g.expando] : n.removeAttribute && n.removeAttribute(g.expando);
                                delete d[c]
                            }
                        }
                    });
                    var ob = /alpha\([^)]*\)/i,
                        Sb = /opacity=([^)]*)/,
                        ac = /([A-Z]|^ms)/g,
                        Nb = /^-?\d+(?:px)?$/i,
                        bc = /^-?\d/,
                        cc = /^([\-+])=([\-+.\de]+)/,
                        dc = {
                            position: "absolute",
                            visibility: "hidden",
                            display: "block"
                        },
                        ec = ["Left", "Right"],
                        gc = ["Top", "Bottom"],
                        Hb, Pb, Qb;
                    g.fn.css = function(a, b) {
                        return 2 === arguments.length && b === c ? this : g.access(this, a, b, !0, function(a, b, d) {
                            return d !== c ? g.style(a, b, d) : g.css(a, b)
                        })
                    };
                    g.extend({
                        cssHooks: {
                            opacity: {
                                get: function(a, b) {
                                    if (b) {
                                        var c = Hb(a, "opacity", "opacity");
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
                            "float": g.support.cssFloat ? "cssFloat" : "styleFloat"
                        },
                        style: function(a, b, d, e) {
                            if (a && !(3 === a.nodeType || 8 === a.nodeType || !a.style)) {
                                var f, j = g.camelCase(b),
                                    n = a.style,
                                    q = g.cssHooks[j],
                                    b = g.cssProps[j] || j;
                                if (d === c) return q && "get" in q && (f = q.get(a, !1, e)) !== c ? f : n[b];
                                e = typeof d;
                                "string" === e && (f = cc.exec(d)) && (d = +(f[1] + 1) * +f[2] + parseFloat(g.css(a, b)), e = "number");
                                if (!(null == d || "number" === e && isNaN(d))) if ("number" === e && !g.cssNumber[j] && (d += "px"), !q || !("set" in q) || (d = q.set(a, d)) !== c) try {
                                    n[b] = d
                                } catch (m) {}
                            }
                        },
                        css: function(a, b, d) {
                            var e, f, b = g.camelCase(b);
                            f = g.cssHooks[b];
                            b = g.cssProps[b] || b;
                            "cssFloat" === b && (b = "float");
                            if (f && "get" in f && (e = f.get(a, !0, d)) !== c) return e;
                            if (Hb) return Hb(a, b)
                        },
                        swap: function(a, b, c) {
                            var d = {},
                                e;
                            for (e in b) d[e] = a.style[e], a.style[e] = b[e];
                            c.call(a);
                            for (e in b) a.style[e] = d[e]
                        }
                    });
                    g.curCSS = g.css;
                    g.each(["height", "width"], function(a, b) {
                        g.cssHooks[b] = {
                            get: function(a, c, d) {
                                var e;
                                if (c) return 0 !== a.offsetWidth ? S(a, b, d) : (g.swap(a, dc, function() {
                                    e = S(a, b, d)
                                }), e)
                            },
                            set: function(a, b) {
                                if (!Nb.test(b)) return b;
                                b = parseFloat(b);
                                if (0 <= b) return b + "px"
                            }
                        }
                    });
                    g.support.opacity || (g.cssHooks.opacity = {
                        get: function(a, b) {
                            return Sb.test((b && a.currentStyle ? a.currentStyle.filter : a.style.filter) || "") ? parseFloat(RegExp.$1) / 100 + "" : b ? "1" : ""
                        },
                        set: function(a, b) {
                            var c = a.style,
                                d = a.currentStyle,
                                e = g.isNumeric(b) ? "alpha(opacity=" + 100 * b + ")" : "",
                                f = d && d.filter || c.filter || "";
                            c.zoom = 1;
                            if (1 <= b && "" === g.trim(f.replace(ob, "")) && (c.removeAttribute("filter"), d && !d.filter)) return;
                            c.filter = ob.test(f) ? f.replace(ob, e) : f + " " + e
                        }
                    });
                    g(function() {
                        g.support.reliableMarginRight || (g.cssHooks.marginRight = {
                            get: function(a, b) {
                                var c;
                                return g.swap(a, {
                                    display: "inline-block"
                                }, function() {
                                    b ? c = Hb(a, "margin-right", "marginRight") : c = a.style.marginRight
                                }), c
                            }
                        })
                    });
                    v.defaultView && v.defaultView.getComputedStyle && (Pb = function(a, b) {
                        var c, d, e;
                        return b = b.replace(ac, "-$1").toLowerCase(), (d = a.ownerDocument.defaultView) && (e = d.getComputedStyle(a, null)) && (c = e.getPropertyValue(b), "" === c && !g.contains(a.ownerDocument.documentElement, a) && (c = g.style(a, b))), c
                    });
                    v.documentElement.currentStyle && (Qb = function(a, b) {
                        var c, d, e, g = a.currentStyle && a.currentStyle[b],
                            f = a.style;
                        return null === g && f && (e = f[b]) && (g = e), !Nb.test(g) && bc.test(g) && (c = f.left, d = a.runtimeStyle && a.runtimeStyle.left, d && (a.runtimeStyle.left = a.currentStyle.left), f.left = "fontSize" === b ? "1em" : g || 0, g = f.pixelLeft + "px", f.left = c, d && (a.runtimeStyle.left = d)), "" === g ? "auto" : g
                    });
                    Hb = Pb || Qb;
                    g.expr && g.expr.filters && (g.expr.filters.hidden = function(a) {
                        var b = a.offsetHeight;
                        return 0 === a.offsetWidth && 0 === b || !g.support.reliableHiddenOffsets && "none" === (a.style && a.style.display || g.css(a, "display"))
                    }, g.expr.filters.visible = function(a) {
                        return !g.expr.filters.hidden(a)
                    });
                    var nc = /%20/g,
                        qc = /\[\]$/,
                        $b = /\r?\n/g,
                        Bc = /#.*$/,
                        Cc = /^(.*?):[ \t]*([^\r\n]*)\r?$/mg,
                        Dc = /^(?:color|date|datetime|datetime-local|email|hidden|month|number|password|range|search|tel|text|time|url|week)$/i,
                        Ec = /^(?:GET|HEAD)$/,
                        Fc = /^\/\//,
                        fc = /\?/,
                        Gc = /<script\b[^<]*(?:(?!<\/script>)<[^<]*)*<\/script>/gi,
                        Hc = /^(?:select|textarea)/i,
                        hc = /\s+/,
                        Ic = /([?&])_=[^&]*/,
                        ic = /^([\w\+\.\-]+:)(?:\/\/([^\/?#:]*)(?::(\d+))?)?/,
                        jc = g.fn.load,
                        Qc = {},
                        kc = {},
                        tb, ub, lc = ["*/"] + ["*"];
                    try {
                        tb = aa.href
                    } catch ($c) {
                        tb = v.createElement("a"), tb.href = "", tb = tb.href
                    }
                    ub = ic.exec(tb.toLowerCase()) || [];
                    g.fn.extend({
                        load: function(a, b, d) {
                            if ("string" != typeof a && jc) return jc.apply(this, arguments);
                            if (!this.length) return this;
                            var e = a.indexOf(" ");
                            if (0 <= e) var f = a.slice(e, a.length),
                                a = a.slice(0, e);
                            e = "GET";
                            b && (g.isFunction(b) ? (d = b, b = c) : "object" == typeof b && (b = g.param(b, g.ajaxSettings.traditional), e = "POST"));
                            var j = this;
                            return g.ajax({
                                url: a,
                                type: e,
                                dataType: "html",
                                data: b,
                                complete: function(a, b, c) {
                                    c = a.responseText;
                                    a.isResolved() && (a.done(function(a) {
                                        c = a
                                    }), j.html(f ? g("<div>").append(c.replace(Gc, "")).find(f) : c));
                                    d && j.each(d, [c, b, a])
                                }
                            }), this
                        },
                        serialize: function() {
                            return g.param(this.serializeArray())
                        },
                        serializeArray: function() {
                            return this.map(function() {
                                return this.elements ? g.makeArray(this.elements) : this
                            }).filter(function() {
                                return this.name && !this.disabled && (this.checked || Hc.test(this.nodeName) || Dc.test(this.type))
                            }).map(function(a, b) {
                                var c = g(this).val();
                                return null == c ? null : g.isArray(c) ? g.map(c, function(a) {
                                    return {
                                        name: b.name,
                                        value: a.replace($b, "\r\n")
                                    }
                                }) : {
                                    name: b.name,
                                    value: c.replace($b, "\r\n")
                                }
                            }).get()
                        }
                    });
                    g.each("ajaxStart ajaxStop ajaxComplete ajaxError ajaxSuccess ajaxSend".split(" "), function(a, b) {
                        g.fn[b] = function(a) {
                            return this.on(b, a)
                        }
                    });
                    g.each(["get", "post"], function(a, b) {
                        g[b] = function(a, d, e, f) {
                            return g.isFunction(d) && (f = f || e, e = d, d = c), g.ajax({
                                type: b,
                                url: a,
                                data: d,
                                success: e,
                                dataType: f
                            })
                        }
                    });
                    g.extend({
                        getScript: function(a, b) {
                            return g.get(a, c, b, "script")
                        },
                        getJSON: function(a, b, c) {
                            return g.get(a, b, c, "json")
                        },
                        ajaxSetup: function(a, b) {
                            return b ? N(a, g.ajaxSettings) : (b = a, a = g.ajaxSettings), N(a, b), a
                        },
                        ajaxSettings: {
                            url: tb,
                            isLocal: /^(?:about|app|app\-storage|.+\-extension|file|res|widget):$/.test(ub[1]),
                            global: !0,
                            type: "GET",
                            contentType: "application/x-www-form-urlencoded",
                            processData: !0,
                            async: !0,
                            accepts: {
                                xml: "application/xml, text/xml",
                                html: "text/html",
                                text: "text/plain",
                                json: "application/json, text/javascript",
                                "*": lc
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
                                "* text": b.String,
                                "text html": !0,
                                "text json": g.parseJSON,
                                "text xml": g.parseXML
                            },
                            flatOptions: {
                                context: !0,
                                url: !0
                            }
                        },
                        ajaxPrefilter: T(Qc),
                        ajaxTransport: T(kc),
                        ajax: function(a, b) {
                            function d(a, b, k, w) {
                                if (2 !== Ba) {
                                    Ba = 2;
                                    z && clearTimeout(z);
                                    la = c;
                                    P = w || "";
                                    Q.readyState = 0 < a ? 4 : 0;
                                    var G, Y, oa, w = b;
                                    if (k) {
                                        var H = e,
                                            ma = Q,
                                            Tb = H.contents,
                                            u = H.dataTypes,
                                            ga = H.responseFields,
                                            wa, r, Sa, Vb;
                                        for (r in ga) r in k && (ma[ga[r]] = k[r]);
                                        for (;
                                            "*" === u[0];) u.shift(), wa === c && (wa = H.mimeType || ma.getResponseHeader("content-type"));
                                        if (wa) for (r in Tb) if (Tb[r] && Tb[r].test(wa)) {
                                            u.unshift(r);
                                            break
                                        }
                                        if (u[0] in k) Sa = u[0];
                                        else {
                                            for (r in k) {
                                                if (!u[0] || H.converters[r + " " + u[0]]) {
                                                    Sa = r;
                                                    break
                                                }
                                                Vb || (Vb = r)
                                            }
                                            Sa = Sa || Vb
                                        }
                                        k = Sa ? (Sa !== u[0] && u.unshift(Sa), k[Sa]) : void 0
                                    } else k = c;
                                    if (200 <= a && 300 > a || 304 === a) {
                                        if (e.ifModified) {
                                            if (wa = Q.getResponseHeader("Last-Modified")) g.lastModified[y] = wa;
                                            if (wa = Q.getResponseHeader("Etag")) g.etag[y] = wa
                                        }
                                        if (304 === a) w = "notmodified", G = !0;
                                        else try {
                                            wa = e;
                                            wa.dataFilter && (k = wa.dataFilter(k, wa.dataType));
                                            var B = wa.dataTypes;
                                            r = {};
                                            var x, t, v = B.length,
                                                D, p = B[0],
                                                db, Wb, Xa, bb, Oa;
                                            for (x = 1; x < v; x++) {
                                                if (1 === x) for (t in wa.converters)"string" == typeof t && (r[t.toLowerCase()] = wa.converters[t]);
                                                db = p;
                                                p = B[x];
                                                if ("*" === p) p = db;
                                                else if ("*" !== db && db !== p) {
                                                    Wb = db + " " + p;
                                                    Xa = r[Wb] || r["* " + p];
                                                    if (!Xa) for (bb in Oa = c, r) if (D = bb.split(" "), D[0] === db || "*" === D[0]) if (Oa = r[D[1] + " " + p]) {
                                                        bb = r[bb];
                                                        !0 === bb ? Xa = Oa : !0 === Oa && (Xa = bb);
                                                        break
                                                    }!Xa && !Oa && g.error("No conversion from " + Wb.replace(" ", " to "));
                                                    !0 !== Xa && (k = Xa ? Xa(k) : Oa(bb(k)))
                                                }
                                            }
                                            Y = k;
                                            w = "success";
                                            G = !0
                                        } catch (tc) {
                                            w = "parsererror", oa = tc
                                        }
                                    } else if (oa = w, !w || a) w = "error", 0 > a && (a = 0);
                                    Q.status = a;
                                    Q.statusText = "" + (b || w);
                                    G ? n.resolveWith(f, [Y, w, Q]) : n.rejectWith(f, [Q, w, oa]);
                                    Q.statusCode(m);
                                    m = c;
                                    L && j.trigger("ajax" + (G ? "Success" : "Error"), [Q, e, G ? Y : oa]);
                                    q.fireWith(f, [Q, w]);
                                    L && (j.trigger("ajaxComplete", [Q, e]), --g.active || g.event.trigger("ajaxStop"))
                                }
                            }
                            "object" == typeof a && (b = a, a = c);
                            var b = b || {},
                                e = g.ajaxSetup({}, b),
                                f = e.context || e,
                                j = f !== e && (f.nodeType || f instanceof g) ? g(f) : g.event,
                                n = g.Deferred(),
                                q = g.Callbacks("once memory"),
                                m = e.statusCode || {},
                                y, w = {},
                                G = {},
                                P, Y, la, z, H, Ba = 0,
                                L, ma, Q = {
                                    readyState: 0,
                                    setRequestHeader: function(a, b) {
                                        if (!Ba) {
                                            var c = a.toLowerCase(),
                                                a = G[c] = G[c] || a;
                                            w[a] = b
                                        }
                                        return this
                                    },
                                    getAllResponseHeaders: function() {
                                        return 2 === Ba ? P : null
                                    },
                                    getResponseHeader: function(a) {
                                        var b;
                                        if (2 === Ba) {
                                            if (!Y) for (Y = {}; b = Cc.exec(P);) Y[b[1].toLowerCase()] = b[2];
                                            b = Y[a.toLowerCase()]
                                        }
                                        return b === c ? null : b
                                    },
                                    overrideMimeType: function(a) {
                                        return Ba || (e.mimeType = a), this
                                    },
                                    abort: function(a) {
                                        return a = a || "abort", la && la.abort(a), d(0, a), this
                                    }
                                };
                            n.promise(Q);
                            Q.success = Q.done;
                            Q.error = Q.fail;
                            Q.complete = q.add;
                            Q.statusCode = function(a) {
                                if (a) {
                                    var b;
                                    if (2 > Ba) for (b in a) m[b] = [m[b], a[b]];
                                    else b = a[Q.status], Q.then(b, b)
                                }
                                return this
                            };
                            e.url = ((a || e.url) + "").replace(Bc, "").replace(Fc, ub[1] + "//");
                            e.dataTypes = g.trim(e.dataType || "*").toLowerCase().split(hc);
                            null == e.crossDomain && (H = ic.exec(e.url.toLowerCase()), e.crossDomain = !(!H || H[1] == ub[1] && H[2] == ub[2] && (H[3] || ("http:" === H[1] ? 80 : 443)) == (ub[3] || ("http:" === ub[1] ? 80 : 443))));
                            e.data && e.processData && "string" != typeof e.data && (e.data = g.param(e.data, e.traditional));
                            R(Qc, e, b, Q);
                            if (2 === Ba) return !1;
                            L = e.global;
                            e.type = e.type.toUpperCase();
                            e.hasContent = !Ec.test(e.type);
                            L && 0 === g.active++ && g.event.trigger("ajaxStart");
                            if (!e.hasContent && (e.data && (e.url += (fc.test(e.url) ? "&" : "?") + e.data, delete e.data), y = e.url, !1 === e.cache)) {
                                H = g.now();
                                var u = e.url.replace(Ic, "$1_=" + H);
                                e.url = u + (u === e.url ? (fc.test(e.url) ? "&" : "?") + "_=" + H : "")
                            }(e.data && e.hasContent && !1 !== e.contentType || b.contentType) && Q.setRequestHeader("Content-Type", e.contentType);
                            e.ifModified && (y = y || e.url, g.lastModified[y] && Q.setRequestHeader("If-Modified-Since", g.lastModified[y]), g.etag[y] && Q.setRequestHeader("If-None-Match", g.etag[y]));
                            Q.setRequestHeader("Accept", e.dataTypes[0] && e.accepts[e.dataTypes[0]] ? e.accepts[e.dataTypes[0]] + ("*" !== e.dataTypes[0] ? ", " + lc + "; q=0.01" : "") : e.accepts["*"]);
                            for (ma in e.headers) Q.setRequestHeader(ma, e.headers[ma]);
                            if (!e.beforeSend || !1 !== e.beforeSend.call(f, Q, e) && 2 !== Ba) {
                                for (ma in {
                                    success: 1,
                                    error: 1,
                                    complete: 1
                                }) Q[ma](e[ma]);
                                if (la = R(kc, e, b, Q)) {
                                    Q.readyState = 1;
                                    L && j.trigger("ajaxSend", [Q, e]);
                                    e.async && 0 < e.timeout && (z = setTimeout(function() {
                                        Q.abort("timeout")
                                    }, e.timeout));
                                    try {
                                        Ba = 1, la.send(w, d)
                                    } catch (ga) {
                                        if (2 <= Ba) throw ga;
                                        d(-1, ga)
                                    }
                                } else d(-1, "No Transport");
                                return Q
                            }
                            return Q.abort(), !1
                        },
                        param: function(a, b) {
                            var d = [],
                                e = function(a, b) {
                                    b = g.isFunction(b) ? b() : b;
                                    d[d.length] = encodeURIComponent(a) + "=" + encodeURIComponent(b)
                                };
                            b === c && (b = g.ajaxSettings.traditional);
                            if (g.isArray(a) || a.jquery && !g.isPlainObject(a)) g.each(a, function() {
                                e(this.name, this.value)
                            });
                            else for (var f in a) V(f, a[f], b, e);
                            return d.join("&").replace(nc, "+")
                        }
                    });
                    g.extend({
                        active: 0,
                        lastModified: {},
                        etag: {}
                    });
                    var Jc = g.now(),
                        zc = /(\=)\?(&|$)|\?\?/i;
                    g.ajaxSetup({
                        jsonp: "callback",
                        jsonpCallback: function() {
                            return g.expando + "_" + Jc++
                        }
                    });
                    g.ajaxPrefilter("json jsonp", function(a, c, d) {
                        c = "application/x-www-form-urlencoded" === a.contentType && "string" == typeof a.data;
                        if ("jsonp" === a.dataTypes[0] || !1 !== a.jsonp && (zc.test(a.url) || c && zc.test(a.data))) {
                            var e, f = a.jsonpCallback = g.isFunction(a.jsonpCallback) ? a.jsonpCallback() : a.jsonpCallback,
                                j = b[f],
                                n = a.url,
                                q = a.data,
                                m = "$1" + f + "$2";
                            return !1 !== a.jsonp && (n = n.replace(zc, m), a.url === n && (c && (q = q.replace(zc, m)), a.data === q && (n += (/\?/.test(n) ? "&" : "?") + a.jsonp + "=" + f))), a.url = n, a.data = q, b[f] = function(a) {
                                e = [a]
                            }, d.always(function() {
                                b[f] = j;
                                e && g.isFunction(j) && b[f](e[0])
                            }), a.converters["script json"] = function() {
                                return e || g.error(f + " was not called"), e[0]
                            }, a.dataTypes[0] = "json", "script"
                        }
                    });
                    g.ajaxSetup({
                        accepts: {
                            script: "text/javascript, application/javascript, application/ecmascript, application/x-ecmascript"
                        },
                        contents: {
                            script: /javascript|ecmascript/
                        },
                        converters: {
                            "text script": function(a) {
                                return g.globalEval(a), a
                            }
                        }
                    });
                    g.ajaxPrefilter("script", function(a) {
                        a.cache === c && (a.cache = !1);
                        a.crossDomain && (a.type = "GET", a.global = !1)
                    });
                    g.ajaxTransport("script", function(a) {
                        if (a.crossDomain) {
                            var b, d = v.head || v.getElementsByTagName("head")[0] || v.documentElement;
                            return {
                                send: function(e, g) {
                                    b = v.createElement("script");
                                    b.async = "async";
                                    a.scriptCharset && (b.charset = a.scriptCharset);
                                    b.src = a.url;
                                    b.onload = b.onreadystatechange = function(a, e) {
                                        if (e || !b.readyState || /loaded|complete/.test(b.readyState)) b.onload = b.onreadystatechange = null, d && b.parentNode && d.removeChild(b), b = c, e || g(200, "success")
                                    };
                                    d.insertBefore(b, d.firstChild)
                                },
                                abort: function() {
                                    b && b.onload(0, 1)
                                }
                            }
                        }
                    });
                    var Xc = b.ActiveXObject ?
                        function() {
                            for (var a in Kb) Kb[a](0, 1)
                        } : !1, Kc = 0, Kb;
                    g.ajaxSettings.xhr = b.ActiveXObject ?
                        function() {
                            var a;
                            if (!(a = !this.isLocal && fa())) a: {
                                try {
                                    a = new b.ActiveXObject("Microsoft.XMLHTTP");
                                    break a
                                } catch (c) {}
                                a = void 0
                            }
                            return a
                        } : fa;
                    var Yc = g.ajaxSettings.xhr();
                    g.extend(g.support, {
                        ajax: !! Yc,
                        cors: !! Yc && "withCredentials" in Yc
                    });
                    g.support.ajax && g.ajaxTransport(function(a) {
                        if (!a.crossDomain || g.support.cors) {
                            var d;
                            return {
                                send: function(e, f) {
                                    var j = a.xhr(),
                                        n, q;
                                    a.username ? j.open(a.type, a.url, a.async, a.username, a.password) : j.open(a.type, a.url, a.async);
                                    if (a.xhrFields) for (q in a.xhrFields) j[q] = a.xhrFields[q];
                                    a.mimeType && j.overrideMimeType && j.overrideMimeType(a.mimeType);
                                    !a.crossDomain && !e["X-Requested-With"] && (e["X-Requested-With"] = "XMLHttpRequest");
                                    try {
                                        for (q in e) j.setRequestHeader(q, e[q])
                                    } catch (m) {}
                                    j.send(a.hasContent && a.data || null);
                                    d = function(b, e) {
                                        var q, m, y, w, G;
                                        try {
                                            if (d && (e || 4 === j.readyState)) if (d = c, n && (j.onreadystatechange = g.noop, Xc && delete Kb[n]), e) 4 !== j.readyState && j.abort();
                                            else {
                                                q = j.status;
                                                y = j.getAllResponseHeaders();
                                                w = {};
                                                (G = j.responseXML) && G.documentElement && (w.xml = G);
                                                w.text = j.responseText;
                                                try {
                                                    m = j.statusText
                                                } catch (P) {
                                                    m = ""
                                                }!q && a.isLocal && !a.crossDomain ? q = w.text ? 200 : 404 : 1223 === q && (q = 204)
                                            }
                                        } catch (Y) {
                                            e || f(-1, Y)
                                        }
                                        w && f(q, m, w, y)
                                    };
                                    !a.async || 4 === j.readyState ? d() : (n = ++Kc, Xc && (Kb || (Kb = {}, g(b).unload(Xc)), Kb[n] = d), j.onreadystatechange = d)
                                },
                                abort: function() {
                                    d && d(0, 1)
                                }
                            }
                        }
                    });
                    var Rc = {},
                        Wa, Ib, Lc = /^(?:toggle|show|hide)$/,
                        Mc = /^([+\-]=)?([\d+.\-]+)([a-z%]*)$/i,
                        Ac, mc = [
                            ["height", "marginTop", "marginBottom", "paddingTop", "paddingBottom"],
                            ["width", "marginLeft", "marginRight", "paddingLeft", "paddingRight"],
                            ["opacity"]
                        ],
                        Ub;
                    g.fn.extend({
                        show: function(a, b, c) {
                            var d;
                            if (a || 0 === a) return this.animate(ba("show", 3), a, b, c);
                            b = 0;
                            for (c = this.length; b < c; b++) a = this[b], a.style && (d = a.style.display, !g._data(a, "olddisplay") && "none" === d && (d = a.style.display = ""), "" === d && "none" === g.css(a, "display") && g._data(a, "olddisplay", ca(a.nodeName)));
                            for (b = 0; b < c; b++) if (a = this[b], a.style && (d = a.style.display, "" === d || "none" === d)) a.style.display = g._data(a, "olddisplay") || "";
                            return this
                        },
                        hide: function(a, b, c) {
                            if (a || 0 === a) return this.animate(ba("hide", 3), a, b, c);
                            for (var d, b = 0, c = this.length; b < c; b++) a = this[b], a.style && (d = g.css(a, "display"), "none" !== d && !g._data(a, "olddisplay") && g._data(a, "olddisplay", d));
                            for (b = 0; b < c; b++) this[b].style && (this[b].style.display = "none");
                            return this
                        },
                        _toggle: g.fn.toggle,
                        toggle: function(a, b, c) {
                            var d = "boolean" == typeof a;
                            return g.isFunction(a) && g.isFunction(b) ? this._toggle.apply(this, arguments) : null == a || d ? this.each(function() {
                                var b = d ? a : g(this).is(":hidden");
                                g(this)[b ? "show" : "hide"]()
                            }) : this.animate(ba("toggle", 3), a, b, c), this
                        },
                        fadeTo: function(a, b, c, d) {
                            return this.filter(":hidden").css("opacity", 0).show().end().animate({
                                opacity: b
                            }, a, c, d)
                        },
                        animate: function(a, b, c, d) {
                            function e() {
                                !1 === f.queue && g._mark(this);
                                var b = g.extend({}, f),
                                    c = 1 === this.nodeType,
                                    d = c && g(this).is(":hidden"),
                                    j, n, q, m, y, w, G, P;
                                b.animatedProperties = {};
                                for (q in a) {
                                    j = g.camelCase(q);
                                    q !== j && (a[j] = a[q], delete a[q]);
                                    n = a[j];
                                    g.isArray(n) ? (b.animatedProperties[j] = n[1], n = a[j] = n[0]) : b.animatedProperties[j] = b.specialEasing && b.specialEasing[j] || b.easing || "swing";
                                    if ("hide" === n && d || "show" === n && !d) return b.complete.call(this);
                                    c && ("height" === j || "width" === j) && (b.overflow = [this.style.overflow, this.style.overflowX, this.style.overflowY], "inline" === g.css(this, "display") && "none" === g.css(this, "float") && (!g.support.inlineBlockNeedsLayout || "inline" === ca(this.nodeName) ? this.style.display = "inline-block" : this.style.zoom = 1))
                                }
                                null != b.overflow && (this.style.overflow = "hidden");
                                for (q in a) c = new g.fx(this, b, q), n = a[q], Lc.test(n) ? (P = g._data(this, "toggle" + q) || ("toggle" === n ? d ? "show" : "hide" : 0), P ? (g._data(this, "toggle" + q, "show" === P ? "hide" : "show"), c[P]()) : c[n]()) : (m = Mc.exec(n), y = c.cur(), m ? (w = parseFloat(m[2]), G = m[3] || (g.cssNumber[q] ? "" : "px"), "px" !== G && (g.style(this, q, (w || 1) + G), y *= (w || 1) / c.cur(), g.style(this, q, y + G)), m[1] && (w = ("-=" === m[1] ? -1 : 1) * w + y), c.custom(y, w, G)) : c.custom(y, n, ""));
                                return !0
                            }
                            var f = g.speed(b, c, d);
                            return g.isEmptyObject(a) ? this.each(f.complete, [!1]) : (a = g.extend({}, a), !1 === f.queue ? this.each(e) : this.queue(f.queue, e))
                        },
                        stop: function(a, b, d) {
                            return "string" != typeof a && (d = b, b = a, a = c), b && !1 !== a && this.queue(a || "fx", []), this.each(function() {
                                var b, c = !1,
                                    e = g.timers,
                                    f = g._data(this);
                                d || g._unmark(!0, this);
                                if (null == a) for (b in f) {
                                    if (f[b] && f[b].stop && b.indexOf(".run") === b.length - 4) {
                                        var j = f[b];
                                        g.removeData(this, b, !0);
                                        j.stop(d)
                                    }
                                } else if (f[b = a + ".run"] && f[b].stop) f = f[b], g.removeData(this, b, !0), f.stop(d);
                                for (b = e.length; b--;) e[b].elem === this && (null == a || e[b].queue === a) && (d ? e[b](!0) : e[b].saveState(), c = !0, e.splice(b, 1));
                                (!d || !c) && g.dequeue(this, a)
                            })
                        }
                    });
                    g.each({
                        slideDown: ba("show", 1),
                        slideUp: ba("hide", 1),
                        slideToggle: ba("toggle", 1),
                        fadeIn: {
                            opacity: "show"
                        },
                        fadeOut: {
                            opacity: "hide"
                        },
                        fadeToggle: {
                            opacity: "toggle"
                        }
                    }, function(a, b) {
                        g.fn[a] = function(a, c, d) {
                            return this.animate(b, a, c, d)
                        }
                    });
                    g.extend({
                        speed: function(a, b, c) {
                            var d = a && "object" == typeof a ? g.extend({}, a) : {
                                complete: c || !c && b || g.isFunction(a) && a,
                                duration: a,
                                easing: c && b || b && !g.isFunction(b) && b
                            };
                            d.duration = g.fx.off ? 0 : "number" == typeof d.duration ? d.duration : d.duration in g.fx.speeds ? g.fx.speeds[d.duration] : g.fx.speeds._default;
                            if (null == d.queue || !0 === d.queue) d.queue = "fx";
                            return d.old = d.complete, d.complete = function(a) {
                                g.isFunction(d.old) && d.old.call(this);
                                d.queue ? g.dequeue(this, d.queue) : !1 !== a && g._unmark(this)
                            }, d
                        },
                        easing: {
                            linear: function(a, b, c, d) {
                                return c + d * a
                            },
                            swing: function(a, b, c, d) {
                                return (-Math.cos(a * Math.PI) / 2 + 0.5) * d + c
                            }
                        },
                        timers: [],
                        fx: function(a, b, c) {
                            this.options = b;
                            this.elem = a;
                            this.prop = c;
                            b.orig = b.orig || {}
                        }
                    });
                    g.fx.prototype = {
                        update: function() {
                            this.options.step && this.options.step.call(this.elem, this.now, this);
                            (g.fx.step[this.prop] || g.fx.step._default)(this)
                        },
                        cur: function() {
                            if (null == this.elem[this.prop] || this.elem.style && null != this.elem.style[this.prop]) {
                                var a, b = g.css(this.elem, this.prop);
                                return isNaN(a = parseFloat(b)) ? !b || "auto" === b ? 0 : b : a
                            }
                            return this.elem[this.prop]
                        },
                        custom: function(a, b, d) {
                            function e(a) {
                                return f.step(a)
                            }
                            var f = this,
                                j = g.fx;
                            this.startTime = Ub || (setTimeout(Z, 0), Ub = g.now());
                            this.end = b;
                            this.now = this.start = a;
                            this.pos = this.state = 0;
                            this.unit = d || this.unit || (g.cssNumber[this.prop] ? "" : "px");
                            e.queue = this.options.queue;
                            e.elem = this.elem;
                            e.saveState = function() {
                                f.options.hide && g._data(f.elem, "fxshow" + f.prop) === c && g._data(f.elem, "fxshow" + f.prop, f.start)
                            };
                            e() && g.timers.push(e) && !Ac && (Ac = setInterval(j.tick, j.interval))
                        },
                        show: function() {
                            var a = g._data(this.elem, "fxshow" + this.prop);
                            this.options.orig[this.prop] = a || g.style(this.elem, this.prop);
                            this.options.show = !0;
                            a !== c ? this.custom(this.cur(), a) : this.custom("width" === this.prop || "height" === this.prop ? 1 : 0, this.cur());
                            g(this.elem).show()
                        },
                        hide: function() {
                            this.options.orig[this.prop] = g._data(this.elem, "fxshow" + this.prop) || g.style(this.elem, this.prop);
                            this.options.hide = !0;
                            this.custom(this.cur(), 0)
                        },
                        step: function(a) {
                            var b, c, d = Ub || (setTimeout(Z, 0), Ub = g.now()),
                                e = !0,
                                f = this.elem,
                                j = this.options;
                            if (a || d >= j.duration + this.startTime) {
                                this.now = this.end;
                                this.pos = this.state = 1;
                                this.update();
                                j.animatedProperties[this.prop] = !0;
                                for (b in j.animatedProperties)!0 !== j.animatedProperties[b] && (e = !1);
                                if (e) {
                                    null != j.overflow && !g.support.shrinkWrapBlocks && g.each(["", "X", "Y"], function(a, b) {
                                        f.style["overflow" + b] = j.overflow[a]
                                    });
                                    j.hide && g(f).hide();
                                    if (j.hide || j.show) for (b in j.animatedProperties) g.style(f, b, j.orig[b]), g.removeData(f, "fxshow" + b, !0), g.removeData(f, "toggle" + b, !0);
                                    (a = j.complete) && (j.complete = !1, a.call(f))
                                }
                                return !1
                            }
                            return Infinity == j.duration ? this.now = d : (c = d - this.startTime, this.state = c / j.duration, this.pos = g.easing[j.animatedProperties[this.prop]](this.state, c, 0, 1, j.duration), this.now = this.start + (this.end - this.start) * this.pos), this.update(), !0
                        }
                    };
                    g.extend(g.fx, {
                        tick: function() {
                            for (var a, b = g.timers, c = 0; c < b.length; c++) a = b[c], !a() && b[c] === a && b.splice(c--, 1);
                            b.length || g.fx.stop()
                        },
                        interval: 13,
                        stop: function() {
                            clearInterval(Ac);
                            Ac = null
                        },
                        speeds: {
                            slow: 600,
                            fast: 200,
                            _default: 400
                        },
                        step: {
                            opacity: function(a) {
                                g.style(a.elem, "opacity", a.now)
                            },
                            _default: function(a) {
                                a.elem.style && null != a.elem.style[a.prop] ? a.elem.style[a.prop] = a.now + a.unit : a.elem[a.prop] = a.now
                            }
                        }
                    });
                    g.each(["width", "height"], function(a, b) {
                        g.fx.step[b] = function(a) {
                            g.style(a.elem, b, Math.max(0, a.now) + a.unit)
                        }
                    });
                    g.expr && g.expr.filters && (g.expr.filters.animated = function(a) {
                        return g.grep(g.timers, function(b) {
                            return a === b.elem
                        }).length
                    });
                    var Nc = /^t(?:able|d|h)$/i,
                        oc = /^(?:body|html)$/i;
                    "getBoundingClientRect" in v.documentElement ? g.fn.offset = function(a) {
                        var b = this[0],
                            c;
                        if (a) return this.each(function(b) {
                            g.offset.setOffset(this, a, b)
                        });
                        if (!b || !b.ownerDocument) return null;
                        if (b === b.ownerDocument.body) return g.offset.bodyOffset(b);
                        try {
                            c = b.getBoundingClientRect()
                        } catch (d) {}
                        var e = b.ownerDocument,
                            f = e.documentElement;
                        if (!c || !g.contains(f, b)) return c ? {
                            top: c.top,
                            left: c.left
                        } : {
                            top: 0,
                            left: 0
                        };
                        b = e.body;
                        e = X(e);
                        return {
                            top: c.top + (e.pageYOffset || g.support.boxModel && f.scrollTop || b.scrollTop) - (f.clientTop || b.clientTop || 0),
                            left: c.left + (e.pageXOffset || g.support.boxModel && f.scrollLeft || b.scrollLeft) - (f.clientLeft || b.clientLeft || 0)
                        }
                    } : g.fn.offset = function(a) {
                        var b = this[0];
                        if (a) return this.each(function(b) {
                            g.offset.setOffset(this, a, b)
                        });
                        if (!b || !b.ownerDocument) return null;
                        if (b === b.ownerDocument.body) return g.offset.bodyOffset(b);
                        var c, d = b.offsetParent,
                            e = b.ownerDocument,
                            f = e.documentElement,
                            j = e.body;
                        c = (e = e.defaultView) ? e.getComputedStyle(b, null) : b.currentStyle;
                        for (var n = b.offsetTop, q = b.offsetLeft;
                             (b = b.parentNode) && (b !== j && b !== f) && !(g.support.fixedPosition && "fixed" === c.position);) c = e ? e.getComputedStyle(b, null) : b.currentStyle, n -= b.scrollTop, q -= b.scrollLeft, b === d && (n += b.offsetTop, q += b.offsetLeft, g.support.doesNotAddBorder && (!g.support.doesAddBorderForTableAndCells || !Nc.test(b.nodeName)) && (n += parseFloat(c.borderTopWidth) || 0, q += parseFloat(c.borderLeftWidth) || 0), d = b.offsetParent), g.support.subtractsBorderForOverflowNotVisible && "visible" !== c.overflow && (n += parseFloat(c.borderTopWidth) || 0, q += parseFloat(c.borderLeftWidth) || 0);
                        if ("relative" === c.position || "static" === c.position) n += j.offsetTop, q += j.offsetLeft;
                        return g.support.fixedPosition && "fixed" === c.position && (n += Math.max(f.scrollTop, j.scrollTop), q += Math.max(f.scrollLeft, j.scrollLeft)), {
                            top: n,
                            left: q
                        }
                    };
                    g.offset = {
                        bodyOffset: function(a) {
                            var b = a.offsetTop,
                                c = a.offsetLeft;
                            return g.support.doesNotIncludeMarginInBodyOffset && (b += parseFloat(g.css(a, "marginTop")) || 0, c += parseFloat(g.css(a, "marginLeft")) || 0), {
                                top: b,
                                left: c
                            }
                        },
                        setOffset: function(a, b, c) {
                            var d = g.css(a, "position");
                            "static" === d && (a.style.position = "relative");
                            var e = g(a),
                                f = e.offset(),
                                j = g.css(a, "top"),
                                n = g.css(a, "left"),
                                q = {},
                                m = {},
                                y, w;
                            ("absolute" === d || "fixed" === d) && -1 < g.inArray("auto", [j, n]) ? (m = e.position(), y = m.top, w = m.left) : (y = parseFloat(j) || 0, w = parseFloat(n) || 0);
                            g.isFunction(b) && (b = b.call(a, c, f));
                            null != b.top && (q.top = b.top - f.top + y);
                            null != b.left && (q.left = b.left - f.left + w);
                            "using" in b ? b.using.call(a, q) : e.css(q)
                        }
                    };
                    g.fn.extend({
                        position: function() {
                            if (!this[0]) return null;
                            var a = this[0],
                                b = this.offsetParent(),
                                c = this.offset(),
                                d = oc.test(b[0].nodeName) ? {
                                    top: 0,
                                    left: 0
                                } : b.offset();
                            return c.top -= parseFloat(g.css(a, "marginTop")) || 0, c.left -= parseFloat(g.css(a, "marginLeft")) || 0, d.top += parseFloat(g.css(b[0], "borderTopWidth")) || 0, d.left += parseFloat(g.css(b[0], "borderLeftWidth")) || 0, {
                                top: c.top - d.top,
                                left: c.left - d.left
                            }
                        },
                        offsetParent: function() {
                            return this.map(function() {
                                for (var a = this.offsetParent || v.body; a && !oc.test(a.nodeName) && "static" === g.css(a, "position");) a = a.offsetParent;
                                return a
                            })
                        }
                    });
                    g.each(["Left", "Top"], function(a, b) {
                        var d = "scroll" + b;
                        g.fn[d] = function(b) {
                            var e, f;
                            return b === c ? (e = this[0], e ? (f = X(e), f ? "pageXOffset" in f ? f[a ? "pageYOffset" : "pageXOffset"] : g.support.boxModel && f.document.documentElement[d] || f.document.body[d] : e[d]) : null) : this.each(function() {
                                (f = X(this)) ? f.scrollTo(a ? g(f).scrollLeft() : b, a ? b : g(f).scrollTop()) : this[d] = b
                            })
                        }
                    });
                    g.each(["Height", "Width"], function(a, b) {
                        var d = b.toLowerCase();
                        g.fn["inner" + b] = function() {
                            var a = this[0];
                            return a ? a.style ? parseFloat(g.css(a, d, "padding")) : this[d]() : null
                        };
                        g.fn["outer" + b] = function(a) {
                            var b = this[0];
                            return b ? b.style ? parseFloat(g.css(b, d, a ? "margin" : "border")) : this[d]() : null
                        };
                        g.fn[d] = function(a) {
                            var e = this[0];
                            if (!e) return null == a ? null : this;
                            if (g.isFunction(a)) return this.each(function(b) {
                                var c = g(this);
                                c[d](a.call(this, b, c[d]()))
                            });
                            if (g.isWindow(e)) {
                                var f = e.document.documentElement["client" + b],
                                    j = e.document.body;
                                return "CSS1Compat" === e.document.compatMode && f || j && j["client" + b] || f
                            }
                            return 9 === e.nodeType ? Math.max(e.documentElement["client" + b], e.body["scroll" + b], e.documentElement["scroll" + b], e.body["offset" + b], e.documentElement["offset" + b]) : a === c ? (e = g.css(e, d), f = parseFloat(e), g.isNumeric(f) ? f : e) : this.css(d, "string" == typeof a ? a : a + "px")
                        }
                    });
                    a.jQuery = a.$ = g;
                    "function" == typeof define && define.amd && define.amd.jQuery && define("jquery", [], function() {
                        return g
                    })
                }
                f = $a = Ea.jQuery
            } else f = $a = window.jQuery;
            if (!f.fn.registerMutationObserver) {
                var vb = $a,
                    Oc = window;
                vb.fn.extend({
                    registerMutationObserver: function(a, b) {
                        "undefined" !== typeof Zb && Zb.register(this.selector, a, b)
                    }
                });
                var pc = window.MutationObserver || window.WebKitMutationObserver || window.MozMutationObserver;
                if (pc) {
                    var Zb = {
                        observer: null,
                        rootNode: Oc.document,
                        observerOptions: {
                            subtree: !0,
                            attributes: !0,
                            childList: !0,
                            characterData: !0,
                            characterDataOldValue: !0,
                            attributeOldValue: !0,
                            attributeFilter: []
                        },
                        mutationHandlers: {
                            attributes: {
                                style: {},
                                "class": {}
                            },
                            removedNodes: {},
                            characterData: {}
                        },
                        init: function() {
                            function a(b, c, d) {
                                c && c.forEach(function(a) {
                                    a.apply(b, d)
                                })
                            }
                            var b = this;
                            this.observer = new pc(function(c) {
                                b.disconnect();
                                c.forEach(function(c) {
                                    var d = c.target,
                                        e;
                                    if ("childList" == c.type) {
                                        if ((d = c.removedNodes) && d.length) for (var f = 0; f < d.length; f++) if (e = b.getMutationHandlers("removedNodes", d[f], c)) {
                                            a(d[f], e, ["removedNodes"]);
                                            break
                                        }
                                    } else "attributes" == c.type ? (e = b.getMutationHandlers("attributes", d, c), a(d, e, ["attributes", c.oldValue, c.attributeName])) : "characterData" == c.type && (e = b.getMutationHandlers("characterData", d.parentNode, c), a(d.parentNode, e, ["characterData", c.oldValue]))
                                });
                                b.reconnect()
                            });
                            this.reconnect()
                        },
                        reconnect: function() {
                            this.observer.observe(this.rootNode, this.observerOptions)
                        },
                        disconnect: function() {
                            this.observer.disconnect()
                        },
                        getMutationHandlers: function(a, b, c) {
                            function d(a) {
                                for (var e in a) if (1 == b.nodeType && vb(b).is(e) || vb(c.target).is(e) || a[e].subtree && 0 < vb(c.target).parents(e).length) return a[e].handlers;
                                return null
                            }
                            var e = this.mutationHandlers[a];
                            if (e) if ("attributes" == a) {
                                if (c && e[c.attributeName]) return d(e[c.attributeName])
                            } else return d(e);
                            return null
                        },
                        register: function(a, b, c) {
                            var d = this,
                                e = this.mutationHandlers,
                                f, g;
                            for (g in b) f = b[g], g in e && f && ("attributes" === g ? f.forEach(function(f) {
                                e[g][f][a] || (e[g][f][a] = {
                                    subtree: !0 === b.subtree,
                                    handlers: []
                                });
                                e[g][f][a].handlers.push(c);
                                var j = d.observerOptions.attributeFilter;
                                0 > j.indexOf(f) && j.push(f)
                            }) : (e[g][a] || (e[g][a] = {
                                subtree: !0 === b.subtree,
                                handlers: []
                            }), e[g][a].handlers.push(c)))
                        }
                    };
                    Zb.init();
                    var Zc = {};
                    "show hide addClass removeClass toggleClass css width height remove html text val attr removeAttr".split(" ").forEach(function(a) {
                        Zc["original_" + a] = vb.fn[a];
                        Zc[a] = function() {
                            var b = arguments;
                            Zb.disconnect();
                            b = vb.fn["original_" + a].apply(this, b);
                            Zb.reconnect();
                            return b
                        }
                    });
                    vb.fn.extend(Zc)
                }
            }
            return f
        };
        Ea.loadFlot = function() {
            if (!window.$ || window.jQuery) var a = $a = Ea.loadJQuery();
            if (!document.createElement("canvas").getContext) {
                var b = function() {
                        return this.context_ || (this.context_ = new K(this))
                    },
                    c = function(a, b, c) {
                        var d = ha.call(arguments, 2);
                        return function() {
                            return a.apply(b, d.concat(ha.call(arguments)))
                        }
                    },
                    d = function(a) {
                        return String(a).replace(/&/g, "&amp;").replace(/"/g, "&quot;")
                    },
                    e = function(a) {
                        a.namespaces.g_vml_ || a.namespaces.add("g_vml_", "urn:schemas-microsoft-com:vml", "#default#VML");
                        a.namespaces.g_o_ || a.namespaces.add("g_o_", "urn:schemas-microsoft-com:office:office", "#default#VML");
                        a.styleSheets.ex_canvas_ || (a = a.createStyleSheet(), a.owningElement.id = "ex_canvas_", a.cssText = "canvas{display:inline-block;overflow:hidden;text-align:left;width:300px;height:150px}")
                    },
                    f = function(a) {
                        var b = a.srcElement;
                        switch (a.propertyName) {
                            case "width":
                                b.getContext().clearRect();
                                b.style.width = b.attributes.width.nodeValue + "px";
                                b.firstChild.style.width = b.clientWidth + "px";
                                break;
                            case "height":
                                b.getContext().clearRect(), b.style.height = b.attributes.height.nodeValue + "px", b.firstChild.style.height = b.clientHeight + "px"
                        }
                    },
                    m = function(a) {
                        a = a.srcElement;
                        a.firstChild && (a.firstChild.style.width = a.clientWidth + "px", a.firstChild.style.height = a.clientHeight + "px")
                    },
                    r = function() {
                        return [[1, 0, 0], [0, 1, 0], [0, 0, 1]]
                    },
                    x = function(a, b) {
                        for (var c = r(), d = 0; 3 > d; d++) for (var e = 0; 3 > e; e++) {
                            for (var f = 0, g = 0; 3 > g; g++) f += a[d][g] * b[g][e];
                            c[d][e] = f
                        }
                        return c
                    },
                    p = function(a, b) {
                        b.fillStyle = a.fillStyle;
                        b.lineCap = a.lineCap;
                        b.lineJoin = a.lineJoin;
                        b.lineWidth = a.lineWidth;
                        b.miterLimit = a.miterLimit;
                        b.shadowBlur = a.shadowBlur;
                        b.shadowColor = a.shadowColor;
                        b.shadowOffsetX = a.shadowOffsetX;
                        b.shadowOffsetY = a.shadowOffsetY;
                        b.strokeStyle = a.strokeStyle;
                        b.globalAlpha = a.globalAlpha;
                        b.font = a.font;
                        b.textAlign = a.textAlign;
                        b.textBaseline = a.textBaseline;
                        b.arcScaleX_ = a.arcScaleX_;
                        b.arcScaleY_ = a.arcScaleY_;
                        b.lineScale_ = a.lineScale_
                    },
                    A = function(a) {
                        var b = a.indexOf("(", 3),
                            c = a.indexOf(")", b + 1),
                            b = a.substring(b + 1, c).split(",");
                        4 == b.length && "a" == a.substr(3, 1) ? alpha = Number(b[3]) : b[3] = 1;
                        return b
                    },
                    C = function(a, b, c) {
                        return Math.min(c, Math.max(b, a))
                    },
                    F = function(a, b, c) {
                        0 > c && c++;
                        1 < c && c--;
                        return 1 > 6 * c ? a + 6 * (b - a) * c : 1 > 2 * c ? b : 2 > 3 * c ? a + 6 * (b - a) * (2 / 3 - c) : a
                    },
                    E = function(a) {
                        var b = 1,
                            a = String(a);
                        if ("#" != a.charAt(0)) if (/^rgb/.test(a)) {
                            for (var b = A(a), a = "#", c, d = 0; 3 > d; d++) c = -1 != b[d].indexOf("%") ? Math.floor(255 * (parseFloat(b[d]) / 100)) : Number(b[d]), a += W[C(c, 0, 255)];
                            b = b[3]
                        } else if (/^hsl/.test(a)) {
                            a = b = A(a);
                            h = parseFloat(a[0]) / 360 % 360;
                            0 > h && h++;
                            s = C(parseFloat(a[1]) / 100, 0, 1);
                            l = C(parseFloat(a[2]) / 100, 0, 1);
                            if (0 == s) a = c = d = l;
                            else {
                                var d = 0.5 > l ? l * (1 + s) : l + s - l * s,
                                    e = 2 * l - d,
                                    a = F(e, d, h + 1 / 3);
                                c = F(e, d, h);
                                d = F(e, d, h - 1 / 3)
                            }
                            a = "#" + W[Math.floor(255 * a)] + W[Math.floor(255 * c)] + W[Math.floor(255 * d)];
                            b = b[3]
                        } else a = ia[a] || a;
                        return {
                            color: a,
                            alpha: b
                        }
                    },
                    O = function(a) {
                        switch (a) {
                            case "butt":
                                return "flat";
                            case "round":
                                return "round";
                            default:
                                return "square"
                        }
                    },
                    K = function(a) {
                        this.m_ = r();
                        this.mStack_ = [];
                        this.aStack_ = [];
                        this.currentPath_ = [];
                        this.fillStyle = this.strokeStyle = "#000";
                        this.lineWidth = 1;
                        this.lineJoin = "miter";
                        this.lineCap = "butt";
                        this.miterLimit = 1 * da;
                        this.globalAlpha = 1;
                        this.font = "10px sans-serif";
                        this.textAlign = "left";
                        this.textBaseline = "alphabetic";
                        this.canvas = a;
                        var b = a.ownerDocument.createElement("div");
                        b.style.width = a.clientWidth + "px";
                        b.style.height = a.clientHeight + "px";
                        b.style.overflow = "hidden";
                        b.style.position = "absolute";
                        a.appendChild(b);
                        this.element_ = b;
                        this.lineScale_ = this.arcScaleY_ = this.arcScaleX_ = 1
                    },
                    S = function(a, b, c, d) {
                        a.currentPath_.push({
                            type: "bezierCurveTo",
                            cp1x: b.x,
                            cp1y: b.y,
                            cp2x: c.x,
                            cp2y: c.y,
                            x: d.x,
                            y: d.y
                        });
                        a.currentX_ = d.x;
                        a.currentY_ = d.y
                    },
                    T = function(a, b) {
                        var c = E(a.strokeStyle),
                            d = c.color,
                            c = c.alpha * a.globalAlpha,
                            e = a.lineScale_ * a.lineWidth;
                        1 > e && (c *= e);
                        b.push("<g_vml_:stroke", ' opacity="', c, '"', ' joinstyle="', a.lineJoin, '"', ' miterlimit="', a.miterLimit, '"', ' endcap="', O(a.lineCap), '"', ' weight="', e, 'px"', ' color="', d, '" />')
                    },
                    R = function(a, b, c, d) {
                        var e = a.fillStyle,
                            f = a.arcScaleX_,
                            g = a.arcScaleY_,
                            j = d.x - c.x,
                            m = d.y - c.y;
                        if (e instanceof V) {
                            var r = 0,
                                p = d = 0,
                                x = 0,
                                t = 1;
                            if ("gradient" == e.type_) {
                                var r = e.x1_ / f,
                                    c = e.y1_ / g,
                                    v = a.getCoords_(e.x0_ / f, e.y0_ / g),
                                    r = a.getCoords_(r, c),
                                    r = 180 * Math.atan2(r.x - v.x, r.y - v.y) / Math.PI;
                                0 > r && (r += 360);
                                1E-6 > r && (r = 0)
                            } else v = a.getCoords_(e.x0_, e.y0_), d = (v.x - c.x) / j, p = (v.y - c.y) / m, j /= f * da, m /= g * da, t = Z.max(j, m), x = 2 * e.r0_ / t, t = 2 * e.r1_ / t - x;
                            f = e.colors_;
                            f.sort(function(a, b) {
                                return a.offset - b.offset
                            });
                            for (var g = f.length, v = f[0].color, c = f[g - 1].color, j = f[0].alpha * a.globalAlpha, a = f[g - 1].alpha * a.globalAlpha, m = [], A = 0; A < g; A++) {
                                var C = f[A];
                                m.push(C.offset * t + x + " " + C.color)
                            }
                            b.push('<g_vml_:fill type="', e.type_, '"', ' method="none" focus="100%"', ' color="', v, '"', ' color2="', c, '"', ' colors="', m.join(","), '"', ' opacity="', a, '"', ' g_o_:opacity2="', j, '"', ' angle="', r, '"', ' focusposition="', d, ",", p, '" />')
                        } else e instanceof fa ? j && m && b.push("<g_vml_:fill", ' position="', -c.x / j * f * f, ",", -c.y / m * g * g, '"', ' type="tile"', ' src="', e.src_, '" />') : (e = E(a.fillStyle), b.push('<g_vml_:fill color="', e.color, '" opacity="', e.alpha * a.globalAlpha, '" />'))
                    },
                    N = function(a, b, c) {
                        isFinite(b[0][0]) && (isFinite(b[0][1]) && isFinite(b[1][0]) && isFinite(b[1][1]) && isFinite(b[2][0]) && isFinite(b[2][1])) && (a.m_ = b, c && (a.lineScale_ = g(aa(b[0][0] * b[1][1] - b[0][1] * b[1][0]))))
                    },
                    V = function(a) {
                        this.type_ = a;
                        this.r1_ = this.y1_ = this.x1_ = this.r0_ = this.y0_ = this.x0_ = 0;
                        this.colors_ = []
                    },
                    fa = function(a, b) {
                        if (!a || 1 != a.nodeType || "IMG" != a.tagName) throw new ba("TYPE_MISMATCH_ERR");
                        if ("complete" != a.readyState) throw new ba("INVALID_STATE_ERR");
                        switch (b) {
                            case "repeat":
                            case null:
                            case "":
                                this.repetition_ = "repeat";
                                break;
                            case "repeat-x":
                            case "repeat-y":
                            case "no-repeat":
                                this.repetition_ = b;
                                break;
                            default:
                                throw new ba("SYNTAX_ERR");
                        }
                        this.src_ = a.src;
                        this.width_ = a.width;
                        this.height_ = a.height
                    },
                    ba = function(a) {
                        this.code = this[a];
                        this.message = a + ": DOM Exception " + this.code
                    },
                    Z = Math,
                    ca = Z.round,
                    X = Z.sin,
                    v = Z.cos,
                    aa = Z.abs,
                    g = Z.sqrt,
                    da = 10,
                    t = da / 2,
                    ha = Array.prototype.slice;
                e(document);
                var ea = {
                    init: function(a) {
                        /MSIE/.test(navigator.userAgent) && !window.opera && (a = a || document, a.createElement("canvas"), a.attachEvent("onreadystatechange", c(this.init_, this, a)))
                    },
                    init_: function(a) {
                        for (var a = a.getElementsByTagName("canvas"), b = 0; b < a.length; b++) this.initElement(a[b])
                    },
                    initElement: function(a) {
                        if (!a.getContext) {
                            a.getContext = b;
                            e(a.ownerDocument);
                            a.innerHTML = "";
                            a.attachEvent("onpropertychange", f);
                            a.attachEvent("onresize", m);
                            var c = a.attributes;
                            c.width && c.width.specified ? a.style.width = c.width.nodeValue + "px" : a.width = a.clientWidth;
                            c.height && c.height.specified ? a.style.height = c.height.nodeValue + "px" : a.height = a.clientHeight
                        }
                        return a
                    }
                };
                ea.init();
                for (var W = [], J = 0; 16 > J; J++) for (var $ = 0; 16 > $; $++) W[16 * J + $] = J.toString(16) + $.toString(16);
                var ia = {
                        aliceblue: "#F0F8FF",
                        antiquewhite: "#FAEBD7",
                        aquamarine: "#7FFFD4",
                        azure: "#F0FFFF",
                        beige: "#F5F5DC",
                        bisque: "#FFE4C4",
                        black: "#000000",
                        blanchedalmond: "#FFEBCD",
                        blueviolet: "#8A2BE2",
                        brown: "#A52A2A",
                        burlywood: "#DEB887",
                        cadetblue: "#5F9EA0",
                        chartreuse: "#7FFF00",
                        chocolate: "#D2691E",
                        coral: "#FF7F50",
                        cornflowerblue: "#6495ED",
                        cornsilk: "#FFF8DC",
                        crimson: "#DC143C",
                        cyan: "#00FFFF",
                        darkblue: "#00008B",
                        darkcyan: "#008B8B",
                        darkgoldenrod: "#B8860B",
                        darkgray: "#A9A9A9",
                        darkgreen: "#006400",
                        darkgrey: "#A9A9A9",
                        darkkhaki: "#BDB76B",
                        darkmagenta: "#8B008B",
                        darkolivegreen: "#556B2F",
                        darkorange: "#FF8C00",
                        darkorchid: "#9932CC",
                        darkred: "#8B0000",
                        darksalmon: "#E9967A",
                        darkseagreen: "#8FBC8F",
                        darkslateblue: "#483D8B",
                        darkslategray: "#2F4F4F",
                        darkslategrey: "#2F4F4F",
                        darkturquoise: "#00CED1",
                        darkviolet: "#9400D3",
                        deeppink: "#FF1493",
                        deepskyblue: "#00BFFF",
                        dimgray: "#696969",
                        dimgrey: "#696969",
                        dodgerblue: "#1E90FF",
                        firebrick: "#B22222",
                        floralwhite: "#FFFAF0",
                        forestgreen: "#228B22",
                        gainsboro: "#DCDCDC",
                        ghostwhite: "#F8F8FF",
                        gold: "#FFD700",
                        goldenrod: "#DAA520",
                        grey: "#808080",
                        greenyellow: "#ADFF2F",
                        honeydew: "#F0FFF0",
                        hotpink: "#FF69B4",
                        indianred: "#CD5C5C",
                        indigo: "#4B0082",
                        ivory: "#FFFFF0",
                        khaki: "#F0E68C",
                        lavender: "#E6E6FA",
                        lavenderblush: "#FFF0F5",
                        lawngreen: "#7CFC00",
                        lemonchiffon: "#FFFACD",
                        lightblue: "#ADD8E6",
                        lightcoral: "#F08080",
                        lightcyan: "#E0FFFF",
                        lightgoldenrodyellow: "#FAFAD2",
                        lightgreen: "#90EE90",
                        lightgrey: "#D3D3D3",
                        lightpink: "#FFB6C1",
                        lightsalmon: "#FFA07A",
                        lightseagreen: "#20B2AA",
                        lightskyblue: "#87CEFA",
                        lightslategray: "#778899",
                        lightslategrey: "#778899",
                        lightsteelblue: "#B0C4DE",
                        lightyellow: "#FFFFE0",
                        limegreen: "#32CD32",
                        linen: "#FAF0E6",
                        magenta: "#FF00FF",
                        mediumaquamarine: "#66CDAA",
                        mediumblue: "#0000CD",
                        mediumorchid: "#BA55D3",
                        mediumpurple: "#9370DB",
                        mediumseagreen: "#3CB371",
                        mediumslateblue: "#7B68EE",
                        mediumspringgreen: "#00FA9A",
                        mediumturquoise: "#48D1CC",
                        mediumvioletred: "#C71585",
                        midnightblue: "#191970",
                        mintcream: "#F5FFFA",
                        mistyrose: "#FFE4E1",
                        moccasin: "#FFE4B5",
                        navajowhite: "#FFDEAD",
                        oldlace: "#FDF5E6",
                        olivedrab: "#6B8E23",
                        orange: "#FFA500",
                        orangered: "#FF4500",
                        orchid: "#DA70D6",
                        palegoldenrod: "#EEE8AA",
                        palegreen: "#98FB98",
                        paleturquoise: "#AFEEEE",
                        palevioletred: "#DB7093",
                        papayawhip: "#FFEFD5",
                        peachpuff: "#FFDAB9",
                        peru: "#CD853F",
                        pink: "#FFC0CB",
                        plum: "#DDA0DD",
                        powderblue: "#B0E0E6",
                        rosybrown: "#BC8F8F",
                        royalblue: "#4169E1",
                        saddlebrown: "#8B4513",
                        salmon: "#FA8072",
                        sandybrown: "#F4A460",
                        seagreen: "#2E8B57",
                        seashell: "#FFF5EE",
                        sienna: "#A0522D",
                        skyblue: "#87CEEB",
                        slateblue: "#6A5ACD",
                        slategray: "#708090",
                        slategrey: "#708090",
                        snow: "#FFFAFA",
                        springgreen: "#00FF7F",
                        steelblue: "#4682B4",
                        tan: "#D2B48C",
                        thistle: "#D8BFD8",
                        tomato: "#FF6347",
                        turquoise: "#40E0D0",
                        violet: "#EE82EE",
                        wheat: "#F5DEB3",
                        whitesmoke: "#F5F5F5",
                        yellowgreen: "#9ACD32"
                    },
                    na = {},
                    J = K.prototype;
                J.clearRect = function() {
                    this.textMeasureEl_ && (this.textMeasureEl_.removeNode(!0), this.textMeasureEl_ = null);
                    this.element_.innerHTML = ""
                };
                J.beginPath = function() {
                    this.currentPath_ = []
                };
                J.moveTo = function(a, b) {
                    var c = this.getCoords_(a, b);
                    this.currentPath_.push({
                        type: "moveTo",
                        x: c.x,
                        y: c.y
                    });
                    this.currentX_ = c.x;
                    this.currentY_ = c.y
                };
                J.lineTo = function(a, b) {
                    var c = this.getCoords_(a, b);
                    this.currentPath_.push({
                        type: "lineTo",
                        x: c.x,
                        y: c.y
                    });
                    this.currentX_ = c.x;
                    this.currentY_ = c.y
                };
                J.bezierCurveTo = function(a, b, c, d, e, f) {
                    e = this.getCoords_(e, f);
                    a = this.getCoords_(a, b);
                    c = this.getCoords_(c, d);
                    S(this, a, c, e)
                };
                J.quadraticCurveTo = function(a, b, c, d) {
                    a = this.getCoords_(a, b);
                    c = this.getCoords_(c, d);
                    d = {
                        x: this.currentX_ + 2 / 3 * (a.x - this.currentX_),
                        y: this.currentY_ + 2 / 3 * (a.y - this.currentY_)
                    };
                    S(this, d, {
                        x: d.x + (c.x - this.currentX_) / 3,
                        y: d.y + (c.y - this.currentY_) / 3
                    }, c)
                };
                J.arc = function(a, b, c, d, e, f) {
                    var c = c * da,
                        g = f ? "at" : "wa",
                        j = a + v(d) * c - t,
                        m = b + X(d) * c - t,
                        d = a + v(e) * c - t,
                        e = b + X(e) * c - t;
                    j == d && !f && (j += 0.125);
                    a = this.getCoords_(a, b);
                    j = this.getCoords_(j, m);
                    d = this.getCoords_(d, e);
                    this.currentPath_.push({
                        type: g,
                        x: a.x,
                        y: a.y,
                        radius: c,
                        xStart: j.x,
                        yStart: j.y,
                        xEnd: d.x,
                        yEnd: d.y
                    })
                };
                J.rect = function(a, b, c, d) {
                    this.moveTo(a, b);
                    this.lineTo(a + c, b);
                    this.lineTo(a + c, b + d);
                    this.lineTo(a, b + d);
                    this.closePath()
                };
                J.strokeRect = function(a, b, c, d) {
                    var e = this.currentPath_;
                    this.beginPath();
                    this.moveTo(a, b);
                    this.lineTo(a + c, b);
                    this.lineTo(a + c, b + d);
                    this.lineTo(a, b + d);
                    this.closePath();
                    this.stroke();
                    this.currentPath_ = e
                };
                J.fillRect = function(a, b, c, d) {
                    var e = this.currentPath_;
                    this.beginPath();
                    this.moveTo(a, b);
                    this.lineTo(a + c, b);
                    this.lineTo(a + c, b + d);
                    this.lineTo(a, b + d);
                    this.closePath();
                    this.fill();
                    this.currentPath_ = e
                };
                J.createLinearGradient = function(a, b, c, d) {
                    var e = new V("gradient");
                    e.x0_ = a;
                    e.y0_ = b;
                    e.x1_ = c;
                    e.y1_ = d;
                    return e
                };
                J.createRadialGradient = function(a, b, c, d, e, f) {
                    var g = new V("gradientradial");
                    g.x0_ = a;
                    g.y0_ = b;
                    g.r0_ = c;
                    g.x1_ = d;
                    g.y1_ = e;
                    g.r1_ = f;
                    return g
                };
                J.drawImage = function(a, b) {
                    var c, d, e, f, g, j, m, r;
                    e = a.runtimeStyle.width;
                    f = a.runtimeStyle.height;
                    a.runtimeStyle.width = "auto";
                    a.runtimeStyle.height = "auto";
                    var t = a.width,
                        p = a.height;
                    a.runtimeStyle.width = e;
                    a.runtimeStyle.height = f;
                    if (3 == arguments.length) c = arguments[1], d = arguments[2], g = j = 0, m = e = t, r = f = p;
                    else if (5 == arguments.length) c = arguments[1], d = arguments[2], e = arguments[3], f = arguments[4], g = j = 0, m = t, r = p;
                    else if (9 == arguments.length) g = arguments[1], j = arguments[2], m = arguments[3], r = arguments[4], c = arguments[5], d = arguments[6], e = arguments[7], f = arguments[8];
                    else throw Error("Invalid number of arguments");
                    var x = this.getCoords_(c, d),
                        v = [];
                    v.push(" <g_vml_:group", ' coordsize="', 10 * da, ",", 10 * da, '"', ' coordorigin="0,0"', ' style="width:', 10, "px;height:", 10, "px;position:absolute;");
                    if (1 != this.m_[0][0] || this.m_[0][1] || 1 != this.m_[1][1] || this.m_[1][0]) {
                        var A = [];
                        A.push("M11=", this.m_[0][0], ",", "M12=", this.m_[1][0], ",", "M21=", this.m_[0][1], ",", "M22=", this.m_[1][1], ",", "Dx=", ca(x.x / da), ",", "Dy=", ca(x.y / da), "");
                        var C = this.getCoords_(c + e, d),
                            Ra = this.getCoords_(c, d + f);
                        c = this.getCoords_(c + e, d + f);
                        x.x = Z.max(x.x, C.x, Ra.x, c.x);
                        x.y = Z.max(x.y, C.y, Ra.y, c.y);
                        v.push("padding:0 ", ca(x.x / da), "px ", ca(x.y / da), "px 0;filter:progid:DXImageTransform.Microsoft.Matrix(", A.join(""), ", sizingmethod='clip');")
                    } else v.push("top:", ca(x.y / da), "px;left:", ca(x.x / da), "px;");
                    v.push(' ">', '<g_vml_:image src="', a.src, '"', ' style="width:', da * e, "px;", " height:", da * f, 'px"', ' cropleft="', g / t, '"', ' croptop="', j / p, '"', ' cropright="', (t - g - m) / t, '"', ' cropbottom="', (p - j - r) / p, '"', " />", "</g_vml_:group>");
                    this.element_.insertAdjacentHTML("BeforeEnd", v.join(""))
                };
                J.stroke = function(a) {
                    for (var b = {
                        x: null,
                        y: null
                    }, c = {
                        x: null,
                        y: null
                    }, d = 0; d < this.currentPath_.length; d += 5E3) {
                        var e = [];
                        e.push("<g_vml_:shape", ' filled="', !! a, '"', ' style="position:absolute;width:', 10, "px;height:", 10, 'px;"', ' coordorigin="0,0"', ' coordsize="', 10 * da, ",", 10 * da, '"', ' stroked="', !a, '"', ' path="');
                        for (var f = d; f < Math.min(d + 5E3, this.currentPath_.length); f++) {
                            0 == f % 5E3 && 0 < f && e.push(" m ", ca(this.currentPath_[f - 1].x), ",", ca(this.currentPath_[f - 1].y));
                            var g = this.currentPath_[f];
                            switch (g.type) {
                                case "moveTo":
                                    e.push(" m ", ca(g.x), ",", ca(g.y));
                                    break;
                                case "lineTo":
                                    e.push(" l ", ca(g.x), ",", ca(g.y));
                                    break;
                                case "close":
                                    e.push(" x ");
                                    g = null;
                                    break;
                                case "bezierCurveTo":
                                    e.push(" c ", ca(g.cp1x), ",", ca(g.cp1y), ",", ca(g.cp2x), ",", ca(g.cp2y), ",", ca(g.x), ",", ca(g.y));
                                    break;
                                case "at":
                                case "wa":
                                    e.push(" ", g.type, " ", ca(g.x - this.arcScaleX_ * g.radius), ",", ca(g.y - this.arcScaleY_ * g.radius), " ", ca(g.x + this.arcScaleX_ * g.radius), ",", ca(g.y + this.arcScaleY_ * g.radius), " ", ca(g.xStart), ",", ca(g.yStart), " ", ca(g.xEnd), ",", ca(g.yEnd))
                            }
                            if (g) {
                                if (null == b.x || g.x < b.x) b.x = g.x;
                                if (null == c.x || g.x > c.x) c.x = g.x;
                                if (null == b.y || g.y < b.y) b.y = g.y;
                                if (null == c.y || g.y > c.y) c.y = g.y
                            }
                        }
                        e.push(' ">');
                        a ? R(this, e, b, c) : T(this, e);
                        e.push("</g_vml_:shape>");
                        this.element_.insertAdjacentHTML("beforeEnd", e.join(""))
                    }
                };
                J.fill = function() {
                    this.stroke(!0)
                };
                J.closePath = function() {
                    this.currentPath_.push({
                        type: "close"
                    })
                };
                J.getCoords_ = function(a, b) {
                    var c = this.m_;
                    return {
                        x: da * (a * c[0][0] + b * c[1][0] + c[2][0]) - t,
                        y: da * (a * c[0][1] + b * c[1][1] + c[2][1]) - t
                    }
                };
                J.save = function() {
                    var a = {};
                    p(this, a);
                    this.aStack_.push(a);
                    this.mStack_.push(this.m_);
                    this.m_ = x(r(), this.m_)
                };
                J.restore = function() {
                    this.aStack_.length && (p(this.aStack_.pop(), this), this.m_ = this.mStack_.pop())
                };
                J.translate = function(a, b) {
                    N(this, x([
                        [1, 0, 0],
                        [0, 1, 0],
                        [a, b, 1]
                    ], this.m_), !1)
                };
                J.rotate = function(a) {
                    var b = v(a),
                        a = X(a);
                    N(this, x([
                        [b, a, 0],
                        [-a, b, 0],
                        [0, 0, 1]
                    ], this.m_), !1)
                };
                J.scale = function(a, b) {
                    this.arcScaleX_ *= a;
                    this.arcScaleY_ *= b;
                    N(this, x([
                        [a, 0, 0],
                        [0, b, 0],
                        [0, 0, 1]
                    ], this.m_), !0)
                };
                J.transform = function(a, b, c, d, e, g) {
                    N(this, x([
                        [a, b, 0],
                        [c, d, 0],
                        [e, g, 1]
                    ], this.m_), !0)
                };
                J.setTransform = function(a, b, c, d, e, g) {
                    N(this, [
                        [a, b, 0],
                        [c, d, 0],
                        [e, g, 1]
                    ], !0)
                };
                J.drawText_ = function(a, b, c, e, g) {
                    var f = this.m_,
                        e = 0,
                        j = 1E3,
                        m = 0,
                        r = [],
                        x;
                    x = this.font;
                    if (na[x]) x = na[x];
                    else {
                        var t = document.createElement("div").style;
                        try {
                            t.font = x
                        } catch (p) {}
                        x = na[x] = {
                            style: t.fontStyle || "normal",
                            variant: t.fontVariant || "normal",
                            weight: t.fontWeight || "normal",
                            size: t.fontSize || 10,
                            family: t.fontFamily || "sans-serif"
                        }
                    }
                    var t = x,
                        v = this.element_;
                    x = {};
                    for (var A in t) x[A] = t[A];
                    A = parseFloat(v.currentStyle.fontSize);
                    v = parseFloat(t.size);
                    x.size = "number" == typeof t.size ? t.size : -1 != t.size.indexOf("px") ? v : -1 != t.size.indexOf("em") ? A * v : -1 != t.size.indexOf("%") ? A / 100 * v : -1 != t.size.indexOf("pt") ? v / 0.75 : A;
                    x.size *= 0.981;
                    A = x.style + " " + x.variant + " " + x.weight + " " + x.size + "px " + x.family;
                    v = this.element_.currentStyle;
                    t = this.textAlign.toLowerCase();
                    switch (t) {
                        case "left":
                        case "center":
                        case "right":
                            break;
                        case "end":
                            t = "ltr" == v.direction ? "right" : "left";
                            break;
                        case "start":
                            t = "rtl" == v.direction ? "right" : "left";
                            break;
                        default:
                            t = "left"
                    }
                    switch (this.textBaseline) {
                        case "hanging":
                        case "top":
                            m = x.size / 1.75;
                            break;
                        case "middle":
                            break;
                        default:
                        case null:
                        case "alphabetic":
                        case "ideographic":
                        case "bottom":
                            m = -x.size / 2.25
                    }
                    switch (t) {
                        case "right":
                            e = 1E3;
                            j = 0.05;
                            break;
                        case "center":
                            e = j = 500
                    }
                    b = this.getCoords_(b + 0, c + m);
                    r.push('<g_vml_:line from="', -e, ' 0" to="', j, ' 0.05" ', ' coordsize="100 100" coordorigin="0 0"', ' filled="', !g, '" stroked="', !! g, '" style="position:absolute;width:1px;height:1px;">');
                    g ? T(this, r) : R(this, r, {
                        x: -e,
                        y: 0
                    }, {
                        x: j,
                        y: x.size
                    });
                    g = f[0][0].toFixed(3) + "," + f[1][0].toFixed(3) + "," + f[0][1].toFixed(3) + "," + f[1][1].toFixed(3) + ",0,0";
                    b = ca(b.x / da) + "," + ca(b.y / da);
                    r.push('<g_vml_:skew on="t" matrix="', g, '" ', ' offset="', b, '" origin="', e, ' 0" />', '<g_vml_:path textpathok="true" />', '<g_vml_:textpath on="true" string="', d(a), '" style="v-text-align:', t, ";font:", d(A), '" /></g_vml_:line>');
                    this.element_.insertAdjacentHTML("beforeEnd", r.join(""))
                };
                J.fillText = function(a, b, c, d) {
                    this.drawText_(a, b, c, d, !1)
                };
                J.strokeText = function(a, b, c, d) {
                    this.drawText_(a, b, c, d, !0)
                };
                J.measureText = function(a) {
                    this.textMeasureEl_ || (this.element_.insertAdjacentHTML("beforeEnd", '<span style="position:absolute;top:-20000px;left:0;padding:0;margin:0;border:none;white-space:pre;"></span>'), this.textMeasureEl_ = this.element_.lastChild);
                    var b = this.element_.ownerDocument;
                    this.textMeasureEl_.innerHTML = "";
                    this.textMeasureEl_.style.font = this.font;
                    this.textMeasureEl_.appendChild(b.createTextNode(a));
                    return {
                        width: this.textMeasureEl_.offsetWidth
                    }
                };
                J.clip = function() {};
                J.arcTo = function() {};
                J.createPattern = function(a, b) {
                    return new fa(a, b)
                };
                V.prototype.addColorStop = function(a, b) {
                    b = E(b);
                    this.colors_.push({
                        offset: a,
                        color: b.color,
                        alpha: b.alpha
                    })
                };
                J = ba.prototype = Error();
                J.INDEX_SIZE_ERR = 1;
                J.DOMSTRING_SIZE_ERR = 2;
                J.HIERARCHY_REQUEST_ERR = 3;
                J.WRONG_DOCUMENT_ERR = 4;
                J.INVALID_CHARACTER_ERR = 5;
                J.NO_DATA_ALLOWED_ERR = 6;
                J.NO_MODIFICATION_ALLOWED_ERR = 7;
                J.NOT_FOUND_ERR = 8;
                J.NOT_SUPPORTED_ERR = 9;
                J.INUSE_ATTRIBUTE_ERR = 10;
                J.INVALID_STATE_ERR = 11;
                J.SYNTAX_ERR = 12;
                J.INVALID_MODIFICATION_ERR = 13;
                J.NAMESPACE_ERR = 14;
                J.INVALID_ACCESS_ERR = 15;
                J.VALIDATION_ERR = 16;
                J.TYPE_MISMATCH_ERR = 17;
                G_vmlCanvasManager = ea;
                CanvasRenderingContext2D = K;
                CanvasGradient = V;
                CanvasPattern = fa;
                DOMException = ba
            }
            var U = $a;
            U.color = {};
            U.color.make = function(a, b, c, d) {
                var e = {};
                e.r = a || 0;
                e.g = b || 0;
                e.b = c || 0;
                e.a = null != d ? d : 1;
                e.add = function(a, b) {
                    for (var c = 0; c < a.length; ++c) e[a.charAt(c)] += b;
                    return e.normalize()
                };
                e.scale = function(a, b) {
                    for (var c = 0; c < a.length; ++c) e[a.charAt(c)] *= b;
                    return e.normalize()
                };
                e.toString = function() {
                    return 1 <= e.a ? "rgb(" + [e.r, e.g, e.b].join() + ")" : "rgba(" + [e.r, e.g, e.b, e.a].join() + ")"
                };
                e.normalize = function() {
                    function a(b, c, d) {
                        return c < b ? b : c > d ? d : c
                    }
                    e.r = a(0, parseInt(e.r), 255);
                    e.g = a(0, parseInt(e.g), 255);
                    e.b = a(0, parseInt(e.b), 255);
                    e.a = a(0, e.a, 1);
                    return e
                };
                e.clone = function() {
                    return U.color.make(e.r, e.b, e.g, e.a)
                };
                return e.normalize()
            };
            U.color.extract = function(a, b) {
                var c;
                do {
                    c = a.css(b).toLowerCase();
                    if ("" != c && "transparent" != c) break;
                    a = a.parent()
                } while (!U.nodeName(a.get(0), "body"));
                "rgba(0, 0, 0, 0)" == c && (c = "transparent");
                return U.color.parse(c)
            };
            U.color.parse = function(a) {
                var b, c = U.color.make;
                if (b = /rgb\(\s*([0-9]{1,3})\s*,\s*([0-9]{1,3})\s*,\s*([0-9]{1,3})\s*\)/.exec(a)) return c(parseInt(b[1], 10), parseInt(b[2], 10), parseInt(b[3], 10));
                if (b = /rgba\(\s*([0-9]{1,3})\s*,\s*([0-9]{1,3})\s*,\s*([0-9]{1,3})\s*,\s*([0-9]+(?:\.[0-9]+)?)\s*\)/.exec(a)) return c(parseInt(b[1], 10), parseInt(b[2], 10), parseInt(b[3], 10), parseFloat(b[4]));
                if (b = /rgb\(\s*([0-9]+(?:\.[0-9]+)?)\%\s*,\s*([0-9]+(?:\.[0-9]+)?)\%\s*,\s*([0-9]+(?:\.[0-9]+)?)\%\s*\)/.exec(a)) return c(2.55 * parseFloat(b[1]), 2.55 * parseFloat(b[2]), 2.55 * parseFloat(b[3]));
                if (b = /rgba\(\s*([0-9]+(?:\.[0-9]+)?)\%\s*,\s*([0-9]+(?:\.[0-9]+)?)\%\s*,\s*([0-9]+(?:\.[0-9]+)?)\%\s*,\s*([0-9]+(?:\.[0-9]+)?)\s*\)/.exec(a)) return c(2.55 * parseFloat(b[1]), 2.55 * parseFloat(b[2]), 2.55 * parseFloat(b[3]), parseFloat(b[4]));
                if (b = /#([a-fA-F0-9]{2})([a-fA-F0-9]{2})([a-fA-F0-9]{2})/.exec(a)) return c(parseInt(b[1], 16), parseInt(b[2], 16), parseInt(b[3], 16));
                if (b = /#([a-fA-F0-9])([a-fA-F0-9])([a-fA-F0-9])/.exec(a)) return c(parseInt(b[1] + b[1], 16), parseInt(b[2] + b[2], 16), parseInt(b[3] + b[3], 16));
                a = U.trim(a).toLowerCase();
                if ("transparent" == a) return c(255, 255, 255, 0);
                b = ka[a] || [0, 0, 0];
                return c(b[0], b[1], b[2])
            };
            var ka = {
                    aqua: [0, 255, 255],
                    azure: [240, 255, 255],
                    beige: [245, 245, 220],
                    black: [0, 0, 0],
                    blue: [0, 0, 255],
                    brown: [165, 42, 42],
                    cyan: [0, 255, 255],
                    darkblue: [0, 0, 139],
                    darkcyan: [0, 139, 139],
                    darkgrey: [169, 169, 169],
                    darkgreen: [0, 100, 0],
                    darkkhaki: [189, 183, 107],
                    darkmagenta: [139, 0, 139],
                    darkolivegreen: [85, 107, 47],
                    darkorange: [255, 140, 0],
                    darkorchid: [153, 50, 204],
                    darkred: [139, 0, 0],
                    darksalmon: [233, 150, 122],
                    darkviolet: [148, 0, 211],
                    fuchsia: [255, 0, 255],
                    gold: [255, 215, 0],
                    green: [0, 128, 0],
                    indigo: [75, 0, 130],
                    khaki: [240, 230, 140],
                    lightblue: [173, 216, 230],
                    lightcyan: [224, 255, 255],
                    lightgreen: [144, 238, 144],
                    lightgrey: [211, 211, 211],
                    lightpink: [255, 182, 193],
                    lightyellow: [255, 255, 224],
                    lime: [0, 255, 0],
                    magenta: [255, 0, 255],
                    maroon: [128, 0, 0],
                    navy: [0, 0, 128],
                    olive: [128, 128, 0],
                    orange: [255, 165, 0],
                    pink: [255, 192, 203],
                    purple: [128, 0, 128],
                    violet: [128, 0, 128],
                    red: [255, 0, 0],
                    silver: [192, 192, 192],
                    white: [255, 255, 255],
                    yellow: [255, 255, 0]
                },
                I = $a,
                pa = function(a, b, c, d) {
                    function e(a, b) {
                        for (var b = [ja].concat(b), c = 0; c < a.length; ++c) a[c].apply(this, b)
                    }
                    function g(a) {
                        for (var b = [], c = 0; c < a.length; ++c) {
                            var d = I.extend(!0, {}, u.series);
                            null != a[c].data ? (d.data = a[c].data, delete a[c].data, I.extend(!0, d, a[c]), a[c].data = d.data) : d.data = a[c];
                            b.push(d)
                        }
                        D = b;
                        b = D.length;
                        c = [];
                        d = [];
                        for (a = 0; a < D.length; ++a) {
                            var m = D[a].color;
                            null != m && (--b, "number" == typeof m ? d.push(m) : c.push(I.color.parse(D[a].color)))
                        }
                        for (a = 0; a < d.length; ++a) b = Math.max(b, d[a] + 1);
                        c = [];
                        for (a = d = 0; c.length < b;) m = u.colors.length == a ? I.color.make(100, 100, 100) : I.color.parse(u.colors[a]), m.scale("rgb", 1 + 0.2 * (1 == d % 2 ? -1 : 1) * Math.ceil(d / 2)), c.push(m), ++a, a >= u.colors.length && (a = 0, ++d);
                        for (a = b = 0; a < D.length; ++a) {
                            d = D[a];
                            null == d.color ? (d.color = c[b].toString(), ++b) : "number" == typeof d.color && (d.color = c[d.color].toString());
                            if (null == d.lines.show) {
                                var n, m = !0;
                                for (n in d) if (d[n] && d[n].show) {
                                    m = !1;
                                    break
                                }
                                m && (d.lines.show = !0)
                            }
                            d.xaxis = r(W, f(d, "x"));
                            d.yaxis = r($, f(d, "y"))
                        }
                        n = function(a, b, c) {
                            b < a.datamin && b != -v && (a.datamin = b);
                            c > a.datamax && c != v && (a.datamax = c)
                        };
                        var x = Number.POSITIVE_INFINITY,
                            t = Number.NEGATIVE_INFINITY,
                            v = Number.MAX_VALUE,
                            ga, p, H, B, Q;
                        I.each(j(), function(a, b) {
                            b.datamin = x;
                            b.datamax = t;
                            b.used = !1
                        });
                        for (a = 0; a < D.length; ++a) b = D[a], b.datapoints = {
                            points: []
                        }, e(da.processRawData, [b, b.data, b.datapoints]);
                        for (a = 0; a < D.length; ++a) {
                            var b = D[a],
                                z = b.data,
                                ma = b.datapoints.format;
                            if (!ma) {
                                ma = [];
                                ma.push({
                                    x: !0,
                                    number: !0,
                                    required: !0
                                });
                                ma.push({
                                    y: !0,
                                    number: !0,
                                    required: !0
                                });
                                if (b.bars.show || b.lines.show && b.lines.fill) ma.push({
                                    y: !0,
                                    number: !0,
                                    required: !1,
                                    defaultValue: 0
                                }), b.bars.horizontal && (delete ma[ma.length - 1].y, ma[ma.length - 1].x = !0);
                                b.datapoints.format = ma
                            }
                            if (null == b.datapoints.pointsize) {
                                b.datapoints.pointsize = ma.length;
                                p = b.datapoints.pointsize;
                                m = b.datapoints.points;
                                insertSteps = b.lines.show && b.lines.steps;
                                b.xaxis.used = b.yaxis.used = !0;
                                for (c = ga = 0; c < z.length; ++c, ga += p) {
                                    Q = z[c];
                                    var L = null == Q;
                                    if (!L) for (d = 0; d < p; ++d) {
                                        H = Q[d];
                                        if (B = ma[d]) B.number && null != H && (H = +H, isNaN(H) ? H = null : Infinity == H ? H = v : -Infinity == H && (H = -v)), null == H && (B.required && (L = !0), null != B.defaultValue && (H = B.defaultValue));
                                        m[ga + d] = H
                                    }
                                    if (L) for (d = 0; d < p; ++d) H = m[ga + d], null != H && (B = ma[d], B.x && n(b.xaxis, H, H), B.y && n(b.yaxis, H, H)), m[ga + d] = null;
                                    else if (insertSteps && 0 < ga && null != m[ga - p] && m[ga - p] != m[ga] && m[ga - p + 1] != m[ga + 1]) {
                                        for (d = 0; d < p; ++d) m[ga + p + d] = m[ga + d];
                                        m[ga + 1] = m[ga - p + 1];
                                        ga += p
                                    }
                                }
                            }
                        }
                        for (a = 0; a < D.length; ++a) b = D[a], e(da.processDatapoints, [b, b.datapoints]);
                        for (a = 0; a < D.length; ++a) {
                            b = D[a];
                            m = b.datapoints.points;
                            p = b.datapoints.pointsize;
                            Q = ga = x;
                            L = z = t;
                            for (c = 0; c < m.length; c += p) if (null != m[c]) for (d = 0; d < p; ++d) if (H = m[c + d], (B = ma[d]) && !(H == v || H == -v)) B.x && (H < ga && (ga = H), H > z && (z = H)), B.y && (H < Q && (Q = H), H > L && (L = H));
                            b.bars.show && (c = "left" == b.bars.align ? 0 : -b.bars.barWidth / 2, b.bars.horizontal ? (Q += c, L += c + b.bars.barWidth) : (ga += c, z += c + b.bars.barWidth));
                            n(b.xaxis, ga, z);
                            n(b.yaxis, Q, L)
                        }
                        I.each(j(), function(a, b) {
                            b.datamin == x && (b.datamin = null);
                            b.datamax == t && (b.datamax = null)
                        })
                    }
                    function f(a, b) {
                        var c = a[b + "axis"];
                        "object" == typeof c && (c = c.n);
                        "number" != typeof c && (c = 1);
                        return c
                    }
                    function j() {
                        return I.grep(W.concat($), function(a) {
                            return a
                        })
                    }
                    function m(a) {
                        var b = {},
                            c, d;
                        for (c = 0; c < W.length; ++c)(d = W[c]) && d.used && (b["x" + d.n] = d.c2p(a.left));
                        for (c = 0; c < $.length; ++c)(d = $[c]) && d.used && (b["y" + d.n] = d.c2p(a.top));
                        b.x1 !== ra && (b.x = b.x1);
                        b.y1 !== ra && (b.y = b.y1);
                        return b
                    }
                    function r(a, b) {
                        a[b - 1] || (a[b - 1] = {
                            n: b,
                            direction: a == W ? "x" : "y",
                            options: I.extend(!0, {}, a == W ? u.xaxis : u.yaxis)
                        });
                        return a[b - 1]
                    }
                    function x(b, c) {
                        var d = document.createElement("canvas");
                        d.className = c;
                        d.width = Ja;
                        d.height = aa;
                        b || I(d).css({
                            position: "absolute",
                            left: 0,
                            top: 0
                        });
                        I(d).appendTo(a);
                        d.getContext || (d = window.G_vmlCanvasManager.initElement(d));
                        d.getContext("2d").save();
                        return d
                    }
                    function t() {
                        Ja = a.width();
                        aa = a.height();
                        if (0 >= Ja || 0 >= aa) throw "Invalid dimensions for plot, width = " + Ja + ", height = " + aa;
                    }
                    function v(a) {
                        a.width != Ja && (a.width = Ja);
                        a.height != aa && (a.height = aa);
                        a = a.getContext("2d");
                        a.restore();
                        a.save()
                    }
                    function p(a) {
                        var b = a.labelWidth,
                            c = a.labelHeight,
                            d = a.options.position,
                            e = a.options.tickLength,
                            g = u.grid.axisMargin,
                            f = u.grid.labelMargin,
                            j = "x" == a.direction ? W : $,
                            m = I.grep(j, function(a) {
                                return a && a.options.position == d && a.reserveSpace
                            });
                        I.inArray(a, m) == m.length - 1 && (g = 0);
                        null == e && (e = "full");
                        j = I.grep(j, function(a) {
                            return a && a.reserveSpace
                        });
                        j = 0 == I.inArray(a, j);
                        !j && "full" == e && (e = 5);
                        isNaN(+e) || (f += +e);
                        "x" == a.direction ? (c += f, "bottom" == d ? (M.bottom += c + g, a.box = {
                            top: aa - M.bottom,
                            height: c
                        }) : (a.box = {
                            top: M.top + g,
                            height: c
                        }, M.top += c + g)) : (b += f, "left" == d ? (a.box = {
                            left: M.left + g,
                            width: b
                        }, M.left += b + g) : (M.right += b + g, a.box = {
                            left: Ja - M.right,
                            width: b
                        }));
                        a.position = d;
                        a.tickLength = e;
                        a.box.padding = f;
                        a.innermost = j
                    }
                    function A() {
                        var b, c = j();
                        I.each(c, function(a, b) {
                            b.show = b.options.show;
                            null == b.show && (b.show = b.used);
                            b.reserveSpace = b.show || b.options.reserveSpace;
                            var c = b.options,
                                d = +(null != c.min ? c.min : b.datamin),
                                e = +(null != c.max ? c.max : b.datamax),
                                g = e - d;
                            if (0 == g) {
                                if (g = 0 == e ? 1 : 0.01, null == c.min && (d -= g), null == c.max || null != c.min) e += g
                            } else {
                                var f = c.autoscaleMargin;
                                null != f && (null == c.min && (d -= g * f, 0 > d && (null != b.datamin && 0 <= b.datamin) && (d = 0)), null == c.max && (e += g * f, 0 < e && (null != b.datamax && 0 >= b.datamax) && (e = 0)))
                            }
                            b.min = d;
                            b.max = e
                        });
                        allocatedAxes = I.grep(c, function(a) {
                            return a.reserveSpace
                        });
                        M.left = M.right = M.top = M.bottom = 0;
                        if (u.grid.show) {
                            I.each(allocatedAxes, function(b, c) {
                                var d = c.options,
                                    e;
                                e = "number" == typeof d.ticks && 0 < d.ticks ? d.ticks : 0.3 * Math.sqrt("x" == c.direction ? Ja : aa);
                                e = (c.max - c.min) / e;
                                var g, f, j, m;
                                if ("time" == d.mode) {
                                    var q = {
                                        second: 1E3,
                                        minute: 6E4,
                                        hour: 36E5,
                                        day: 864E5,
                                        month: 2592E6,
                                        year: 525949.2 * 6E4
                                    };
                                    m = [
                                        [1, "second"],
                                        [2, "second"],
                                        [5, "second"],
                                        [10, "second"],
                                        [30, "second"],
                                        [1, "minute"],
                                        [2, "minute"],
                                        [5, "minute"],
                                        [10, "minute"],
                                        [30, "minute"],
                                        [1, "hour"],
                                        [2, "hour"],
                                        [4, "hour"],
                                        [8, "hour"],
                                        [12, "hour"],
                                        [1, "day"],
                                        [2, "day"],
                                        [3, "day"],
                                        [0.25, "month"],
                                        [0.5, "month"],
                                        [1, "month"],
                                        [2, "month"],
                                        [3, "month"],
                                        [6, "month"],
                                        [1, "year"]
                                    ];
                                    g = 0;
                                    null != d.minTickSize && (g = "number" == typeof d.tickSize ? d.tickSize : d.minTickSize[0] * q[d.minTickSize[1]]);
                                    for (f = 0; f < m.length - 1 && !(e < (m[f][0] * q[m[f][1]] + m[f + 1][0] * q[m[f + 1][1]]) / 2 && m[f][0] * q[m[f][1]] >= g); ++f);
                                    g = m[f][0];
                                    j = m[f][1];
                                    "year" == j && (f = Math.pow(10, Math.floor(Math.log(e / q.year) / Math.LN10)), m = e / q.year / f, g = (1.5 > m ? 1 : 3 > m ? 2 : 7.5 > m ? 5 : 10) * f);
                                    c.tickSize = d.tickSize || [g, j];
                                    f = function(a) {
                                        var b = [],
                                            c = a.tickSize[0],
                                            d = a.tickSize[1],
                                            e = new Date(a.min),
                                            g = c * q[d];
                                        "second" == d && e.setUTCSeconds(qa(e.getUTCSeconds(), c));
                                        "minute" == d && e.setUTCMinutes(qa(e.getUTCMinutes(), c));
                                        "hour" == d && e.setUTCHours(qa(e.getUTCHours(), c));
                                        "month" == d && e.setUTCMonth(qa(e.getUTCMonth(), c));
                                        "year" == d && e.setUTCFullYear(qa(e.getUTCFullYear(), c));
                                        e.setUTCMilliseconds(0);
                                        g >= q.minute && e.setUTCSeconds(0);
                                        g >= q.hour && e.setUTCMinutes(0);
                                        g >= q.day && e.setUTCHours(0);
                                        g >= 4 * q.day && e.setUTCDate(1);
                                        g >= q.year && e.setUTCMonth(0);
                                        var f = 0,
                                            j = Number.NaN,
                                            m;
                                        do
                                            if (m = j, j = e.getTime(), b.push(j), "month" == d) if (1 > c) {
                                                e.setUTCDate(1);
                                                var n = e.getTime();
                                                e.setUTCMonth(e.getUTCMonth() + 1);
                                                var y = e.getTime();
                                                e.setTime(j + f * q.hour + (y - n) * c);
                                                f = e.getUTCHours();
                                                e.setUTCHours(0)
                                            } else e.setUTCMonth(e.getUTCMonth() + c);
                                            else "year" == d ? e.setUTCFullYear(e.getUTCFullYear() + c) : e.setTime(j + g);
                                        while (j < a.max && j != m);
                                        return b
                                    };
                                    g = function(a, b) {
                                        var c = new Date(a);
                                        if (null != d.timeformat) return I.plot.formatDate(c, d.timeformat, d.monthNames);
                                        var e = b.tickSize[0] * q[b.tickSize[1]],
                                            g = b.max - b.min,
                                            f = d.twelveHourClock ? " %p" : "";
                                        fmt = e < q.minute ? "%h:%M:%S" + f : e < q.day ? g < 2 * q.day ? "%h:%M" + f : "%b %d %h:%M" + f : e < q.month ? "%b %d" : e < q.year ? g < q.year ? "%b" : "%b %y" : "%y";
                                        return I.plot.formatDate(c, fmt, d.monthNames)
                                    }
                                } else {
                                    j = d.tickDecimals;
                                    var y = -Math.floor(Math.log(e) / Math.LN10);
                                    null != j && y > j && (y = j);
                                    f = Math.pow(10, -y);
                                    m = e / f;
                                    if (1.5 > m) g = 1;
                                    else if (3 > m) {
                                        if (g = 2, 2.25 < m && (null == j || y + 1 <= j)) g = 2.5, ++y
                                    } else g = 7.5 > m ? 5 : 10;
                                    g *= f;
                                    null != d.minTickSize && g < d.minTickSize && (g = d.minTickSize);
                                    c.tickDecimals = Math.max(0, null != j ? j : y);
                                    c.tickSize = d.tickSize || g;
                                    f = function(a) {
                                        var b = [],
                                            c = qa(a.min, a.tickSize),
                                            d = 0,
                                            e = Number.NaN,
                                            g;
                                        do g = e, e = c + d * a.tickSize, b.push(e), ++d;
                                        while (e < a.max && e != g);
                                        return b
                                    };
                                    g = function(a, b) {
                                        return a.toFixed(b.tickDecimals)
                                    }
                                }
                                if (null != d.alignTicksWithAxis) {
                                    var w = ("x" == c.direction ? W : $)[d.alignTicksWithAxis - 1];
                                    w && (w.used && w != c) && (f = f(c), 0 < f.length && (null == d.min && (c.min = Math.min(c.min, f[0])), null == d.max && 1 < f.length && (c.max = Math.max(c.max, f[f.length - 1]))), f = function(a) {
                                        var b = [],
                                            c, d;
                                        for (d = 0; d < w.ticks.length; ++d) c = (w.ticks[d].v - w.min) / (w.max - w.min), c = a.min + c * (a.max - a.min), b.push(c);
                                        return b
                                    }, "time" != c.mode && null == d.tickDecimals && (e = Math.max(0, -Math.floor(Math.log(e) / Math.LN10) + 1), m = f(c), 1 < m.length && /\..*0$/.test((m[1] - m[0]).toFixed(e)) || (c.tickDecimals = e)))
                                }
                                c.tickGenerator = f;
                                c.tickFormatter = I.isFunction(d.tickFormatter) ?
                                    function(a, b) {
                                        return "" + d.tickFormatter(a, b)
                                    } : g;
                                g = c.options.ticks;
                                e = [];
                                null == g || "number" == typeof g && 0 < g ? e = c.tickGenerator(c) : g && (e = I.isFunction(g) ? g({
                                    min: c.min,
                                    max: c.max
                                }) : g);
                                c.ticks = [];
                                for (g = 0; g < e.length; ++g) m = null, j = e[g], "object" == typeof j ? (f = +j[0], 1 < j.length && (m = j[1])) : f = +j, null == m && (m = c.tickFormatter(f, c)), isNaN(f) || c.ticks.push({
                                    v: f,
                                    label: m
                                });
                                e = c.ticks;
                                c.options.autoscaleMargin && 0 < e.length && (null == c.options.min && (c.min = Math.min(c.min, e[0].v)), null == c.options.max && 1 < e.length && (c.max = Math.max(c.max, e[e.length - 1].v)));
                                g = function(b, d) {
                                    return I('<div style="position:absolute;top:-10000px;' + d + 'font-size:smaller"><div class="' + c.direction + "Axis " + c.direction + c.n + 'Axis">' + b.join("") + "</div></div>").appendTo(a)
                                };
                                j = c.options;
                                f = c.ticks || [];
                                m = [];
                                var P;
                                e = j.labelWidth;
                                j = j.labelHeight;
                                if ("x" == c.direction) {
                                    if (null == e && (e = Math.floor(Ja / (0 < f.length ? f.length : 1))), null == j) {
                                        m = [];
                                        for (y = 0; y < f.length; ++y)(P = f[y].label) && m.push('<div class="tickLabel" style="float:left;width:' + e + 'px">' + P + "</div>");
                                        0 < m.length && (m.push('<div style="clear:left"></div>'), g = g(m, "width:10000px;"), j = g.height(), g.remove())
                                    }
                                } else if (null == e || null == j) {
                                    for (y = 0; y < f.length; ++y)(P = f[y].label) && m.push('<div class="tickLabel">' + P + "</div>");
                                    0 < m.length && (g = g(m, ""), null == e && (e = g.children().width()), null == j && (j = g.find("div.tickLabel").height()), g.remove())
                                }
                                null == e && (e = 0);
                                null == j && (j = 0);
                                c.labelWidth = e;
                                c.labelHeight = j
                            });
                            for (b = allocatedAxes.length - 1; 0 <= b; --b) p(allocatedAxes[b]);
                            var d = u.grid.minBorderMargin;
                            if (null == d) for (b = d = 0; b < D.length; ++b) d = Math.max(d, D[b].points.radius + D[b].points.lineWidth / 2);
                            for (var e in M) M[e] += u.grid.borderWidth, M[e] = Math.max(d, M[e])
                        }
                        ha = Ja - M.left - M.right;
                        ea = aa - M.bottom - M.top;
                        I.each(c, function(a, b) {
                            var c = function(a) {
                                    return a
                                },
                                d, e, g = b.options.transform || c,
                                f = b.options.inverseTransform;
                            "x" == b.direction ? (d = b.scale = ha / Math.abs(g(b.max) - g(b.min)), e = Math.min(g(b.max), g(b.min))) : (d = b.scale = ea / Math.abs(g(b.max) - g(b.min)), d = -d, e = Math.max(g(b.max), g(b.min)));
                            b.p2c = g == c ?
                                function(a) {
                                    return (a - e) * d
                                } : function(a) {
                                return (g(a) - e) * d
                            };
                            b.c2p = f ?
                                function(a) {
                                    return f(e + a / d)
                                } : function(a) {
                                return e + a / d
                            }
                        });
                        if (u.grid.show) {
                            I.each(allocatedAxes, function(a, b) {
                                "x" == b.direction ? (b.box.left = M.left, b.box.width = ha) : (b.box.top = M.top, b.box.height = ea)
                            });
                            a.find(".tickLabels").remove();
                            b = ['<div class="tickLabels" style="font-size:smaller">'];
                            c = j();
                            for (d = 0; d < c.length; ++d) {
                                e = c[d];
                                var g = e.box;
                                if (e.show) {
                                    b.push('<div class="' + e.direction + "Axis " + e.direction + e.n + 'Axis" style="color:' + e.options.color + '">');
                                    for (var f = 0; f < e.ticks.length; ++f) {
                                        var m = e.ticks[f];
                                        if (m.label && !(m.v < e.min || m.v > e.max)) {
                                            var r = {},
                                                x;
                                            "x" == e.direction ? (x = "center", r.left = Math.round(M.left + e.p2c(m.v) - e.labelWidth / 2), "bottom" == e.position ? r.top = g.top + g.padding : r.bottom = aa - (g.top + g.height - g.padding)) : (r.top = Math.round(M.top + e.p2c(m.v) - e.labelHeight / 2), "left" == e.position ? (r.right = Ja - (g.left + g.width - g.padding), x = "right") : (r.left = g.left + g.padding, x = "left"));
                                            r.width = e.labelWidth;
                                            x = ["position:absolute", "text-align:" + x];
                                            for (var t in r) x.push(t + ":" + r[t] + "px");
                                            b.push('<div class="tickLabel" style="' + x.join(";") + '">' + m.label + "</div>")
                                        }
                                    }
                                    b.push("</div>")
                                }
                            }
                            b.push("</div>");
                            a.append(b.join(""))
                        }
                        a.find(".legend").remove();
                        if (u.legend.show) {
                            t = [];
                            b = !1;
                            c = u.legend.labelFormatter;
                            for (g = 0; g < D.length; ++g) if (d = D[g], e = d.label) 0 == g % u.legend.noColumns && (b && t.push("</tr>"), t.push("<tr>"), b = !0), c && (e = c(e, d)), t.push('<td class="legendColorBox"><div style="border:1px solid ' + u.legend.labelBoxBorderColor + ';padding:1px"><div style="width:4px;height:0;border:5px solid ' + d.color + ';overflow:hidden"></div></div></td><td class="legendLabel">' + e + "</td>");
                            b && t.push("</tr>");
                            0 != t.length && (b = '<table style="font-size:smaller;color:' + u.grid.color + '">' + t.join("") + "</table>", null != u.legend.container ? I(u.legend.container).html(b) : (t = "", c = u.legend.position, d = u.legend.margin, null == d[0] && (d = [d, d]), "n" == c.charAt(0) ? t += "top:" + (d[1] + M.top) + "px;" : "s" == c.charAt(0) && (t += "bottom:" + (d[1] + M.bottom) + "px;"), "e" == c.charAt(1) ? t += "right:" + (d[0] + M.right) + "px;" : "w" == c.charAt(1) && (t += "left:" + (d[0] + M.left) + "px;"), b = I('<div class="legend">' + b.replace('style="', 'style="position:absolute;' + t + ";") + "</div>").appendTo(a), 0 != u.legend.backgroundOpacity && (c = u.legend.backgroundColor, null == c && (c = (c = u.grid.backgroundColor) && "string" == typeof c ? I.color.parse(c) : I.color.extract(b, "background-color"), c.a = 1, c = c.toString()), d = b.children(), I('<div style="position:absolute;width:' + d.width() + "px;height:" + d.height() + "px;" + t + "background-color:" + c + ';"> </div>').prependTo(b).css("opacity", u.legend.backgroundOpacity))))
                        }
                    }
                    function C() {
                        B.clearRect(0, 0, Ja, aa);
                        var a = u.grid;
                        a.show && a.backgroundColor && (B.save(), B.translate(M.left, M.top), B.fillStyle = ad(u.grid.backgroundColor, ea, 0, "rgba(255, 255, 255, 0)"), B.fillRect(0, 0, ha, ea), B.restore());
                        a.show && !a.aboveData && F();
                        for (var b = 0; b < D.length; ++b) {
                            e(da.drawSeries, [B, D[b]]);
                            var c = D[b];
                            c.lines.show && J(c);
                            c.bars.show && kd(c);
                            c.points.show && sa(c)
                        }
                        e(da.draw, [B]);
                        a.show && a.aboveData && F()
                    }
                    function Ra(a, b) {
                        var c, d, e, g, f = j();
                        for (i = 0; i < f.length; ++i) if (c = f[i], c.direction == b && (g = b + c.n + "axis", !a[g] && 1 == c.n && (g = b + "axis"), a[g])) {
                            d = a[g].from;
                            e = a[g].to;
                            break
                        }
                        a[g] || (c = "x" == b ? W[0] : $[0], d = a[b + "1"], e = a[b + "2"]);
                        null != d && (null != e && d > e) && (g = d, d = e, e = g);
                        return {
                            from: d,
                            to: e,
                            axis: c
                        }
                    }
                    function F() {
                        var a;
                        B.save();
                        B.translate(M.left, M.top);
                        var b = u.grid.markings;
                        if (b) {
                            if (I.isFunction(b)) {
                                var c = ja.getAxes();
                                c.xmin = c.xaxis.min;
                                c.xmax = c.xaxis.max;
                                c.ymin = c.yaxis.min;
                                c.ymax = c.yaxis.max;
                                b = b(c)
                            }
                            for (a = 0; a < b.length; ++a) {
                                var c = b[a],
                                    d = Ra(c, "x"),
                                    e = Ra(c, "y");
                                null == d.from && (d.from = d.axis.min);
                                null == d.to && (d.to = d.axis.max);
                                null == e.from && (e.from = e.axis.min);
                                null == e.to && (e.to = e.axis.max);
                                d.to < d.axis.min || (d.from > d.axis.max || e.to < e.axis.min || e.from > e.axis.max) || (d.from = Math.max(d.from, d.axis.min), d.to = Math.min(d.to, d.axis.max), e.from = Math.max(e.from, e.axis.min), e.to = Math.min(e.to, e.axis.max), d.from == d.to && e.from == e.to || (d.from = d.axis.p2c(d.from), d.to = d.axis.p2c(d.to), e.from = e.axis.p2c(e.from), e.to = e.axis.p2c(e.to), d.from == d.to || e.from == e.to ? (B.beginPath(), B.strokeStyle = c.color || u.grid.markingsColor, B.lineWidth = c.lineWidth || u.grid.markingsLineWidth, B.moveTo(d.from, e.from), B.lineTo(d.to, e.to), B.stroke()) : (B.fillStyle = c.color || u.grid.markingsColor, B.fillRect(d.from, e.to, d.to - d.from, e.from - e.to))))
                            }
                        }
                        c = j();
                        b = u.grid.borderWidth;
                        for (d = 0; d < c.length; ++d) {
                            e = c[d];
                            a = e.box;
                            var g = e.tickLength,
                                f, m, n, r;
                            if (e.show && 0 != e.ticks.length) {
                                B.strokeStyle = e.options.tickColor || I.color.parse(e.options.color).scale("a", 0.22).toString();
                                B.lineWidth = 1;
                                "x" == e.direction ? (f = 0, m = "full" == g ? "top" == e.position ? 0 : ea : a.top - M.top + ("top" == e.position ? a.height : 0)) : (m = 0, f = "full" == g ? "left" == e.position ? 0 : ha : a.left - M.left + ("left" == e.position ? a.width : 0));
                                e.innermost || (B.beginPath(), n = r = 0, "x" == e.direction ? n = ha : r = ea, 1 == B.lineWidth && (f = Math.floor(f) + 0.5, m = Math.floor(m) + 0.5), B.moveTo(f, m), B.lineTo(f + n, m + r), B.stroke());
                                B.beginPath();
                                for (a = 0; a < e.ticks.length; ++a) {
                                    var t = e.ticks[a].v;
                                    n = r = 0;
                                    t < e.min || (t > e.max || "full" == g && 0 < b && (t == e.min || t == e.max)) || ("x" == e.direction ? (f = e.p2c(t), r = "full" == g ? -ea : g, "top" == e.position && (r = -r)) : (m = e.p2c(t), n = "full" == g ? -ha : g, "left" == e.position && (n = -n)), 1 == B.lineWidth && ("x" == e.direction ? f = Math.floor(f) + 0.5 : m = Math.floor(m) + 0.5), B.moveTo(f, m), B.lineTo(f + n, m + r))
                                }
                                B.stroke()
                            }
                        }
                        b && (B.lineWidth = b, B.strokeStyle = u.grid.borderColor, B.strokeRect(-b / 2, -b / 2, ha + b, ea + b));
                        B.restore()
                    }
                    function J(a) {
                        function b(a, c, d, e, g) {
                            var f = a.points,
                                a = a.pointsize,
                                j = null,
                                m = null;
                            B.beginPath();
                            for (var q = a; q < f.length; q += a) {
                                var n = f[q - a],
                                    w = f[q - a + 1],
                                    r = f[q],
                                    y = f[q + 1];
                                if (!(null == n || null == r)) {
                                    if (w <= y && w < g.min) {
                                        if (y < g.min) continue;
                                        n = (g.min - w) / (y - w) * (r - n) + n;
                                        w = g.min
                                    } else if (y <= w && y < g.min) {
                                        if (w < g.min) continue;
                                        r = (g.min - w) / (y - w) * (r - n) + n;
                                        y = g.min
                                    }
                                    if (w >= y && w > g.max) {
                                        if (y > g.max) continue;
                                        n = (g.max - w) / (y - w) * (r - n) + n;
                                        w = g.max
                                    } else if (y >= w && y > g.max) {
                                        if (w > g.max) continue;
                                        r = (g.max - w) / (y - w) * (r - n) + n;
                                        y = g.max
                                    }
                                    if (n <= r && n < e.min) {
                                        if (r < e.min) continue;
                                        w = (e.min - n) / (r - n) * (y - w) + w;
                                        n = e.min
                                    } else if (r <= n && r < e.min) {
                                        if (n < e.min) continue;
                                        y = (e.min - n) / (r - n) * (y - w) + w;
                                        r = e.min
                                    }
                                    if (n >= r && n > e.max) {
                                        if (r > e.max) continue;
                                        w = (e.max - n) / (r - n) * (y - w) + w;
                                        n = e.max
                                    } else if (r >= n && r > e.max) {
                                        if (n > e.max) continue;
                                        y = (e.max - n) / (r - n) * (y - w) + w;
                                        r = e.max
                                    }(n != j || w != m) && B.moveTo(e.p2c(n) + c, g.p2c(w) + d);
                                    j = r;
                                    m = y;
                                    B.lineTo(e.p2c(r) + c, g.p2c(y) + d)
                                }
                            }
                            B.stroke()
                        }
                        B.save();
                        B.translate(M.left, M.top);
                        B.lineJoin = "round";
                        var c = a.lines.lineWidth,
                            d = a.shadowSize;
                        if (0 < c && 0 < d) {
                            B.lineWidth = d;
                            B.strokeStyle = "rgba(0,0,0,0.1)";
                            var e = Math.PI / 18;
                            b(a.datapoints, Math.sin(e) * (c / 2 + d / 2), Math.cos(e) * (c / 2 + d / 2), a.xaxis, a.yaxis);
                            B.lineWidth = d / 2;
                            b(a.datapoints, Math.sin(e) * (c / 2 + d / 4), Math.cos(e) * (c / 2 + d / 4), a.xaxis, a.yaxis)
                        }
                        B.lineWidth = c;
                        B.strokeStyle = a.color;
                        if (d = K(a.lines, a.color, 0, ea)) {
                            B.fillStyle = d;
                            for (var g = a.datapoints, d = a.xaxis, e = a.yaxis, f = g.points, g = g.pointsize, j = Math.min(Math.max(0, e.min), e.max), m = 0, n = !1, r = 1, t = 0, x = 0; !(0 < g && m > f.length + g);) {
                                var m = m + g,
                                    u = f[m - g],
                                    z = f[m - g + r],
                                    v = f[m],
                                    p = f[m + r];
                                if (n) {
                                    if (0 < g && null != u && null == v) {
                                        x = m;
                                        g = -g;
                                        r = 2;
                                        continue
                                    }
                                    if (0 > g && m == t + g) {
                                        B.fill();
                                        n = !1;
                                        g = -g;
                                        r = 1;
                                        m = t = x + g;
                                        continue
                                    }
                                }
                                if (!(null == u || null == v)) {
                                    if (u <= v && u < d.min) {
                                        if (v < d.min) continue;
                                        z = (d.min - u) / (v - u) * (p - z) + z;
                                        u = d.min
                                    } else if (v <= u && v < d.min) {
                                        if (u < d.min) continue;
                                        p = (d.min - u) / (v - u) * (p - z) + z;
                                        v = d.min
                                    }
                                    if (u >= v && u > d.max) {
                                        if (v > d.max) continue;
                                        z = (d.max - u) / (v - u) * (p - z) + z;
                                        u = d.max
                                    } else if (v >= u && v > d.max) {
                                        if (u > d.max) continue;
                                        p = (d.max - u) / (v - u) * (p - z) + z;
                                        v = d.max
                                    }
                                    n || (B.beginPath(), B.moveTo(d.p2c(u), e.p2c(j)), n = !0);
                                    if (z >= e.max && p >= e.max) B.lineTo(d.p2c(u), e.p2c(e.max)), B.lineTo(d.p2c(v), e.p2c(e.max));
                                    else if (z <= e.min && p <= e.min) B.lineTo(d.p2c(u), e.p2c(e.min)), B.lineTo(d.p2c(v), e.p2c(e.min));
                                    else {
                                        var D = u,
                                            A = v;
                                        z <= p && z < e.min && p >= e.min ? (u = (e.min - z) / (p - z) * (v - u) + u, z = e.min) : p <= z && (p < e.min && z >= e.min) && (v = (e.min - z) / (p - z) * (v - u) + u, p = e.min);
                                        z >= p && z > e.max && p <= e.max ? (u = (e.max - z) / (p - z) * (v - u) + u, z = e.max) : p >= z && (p > e.max && z <= e.max) && (v = (e.max - z) / (p - z) * (v - u) + u, p = e.max);
                                        u != D && B.lineTo(d.p2c(D), e.p2c(z));
                                        B.lineTo(d.p2c(u), e.p2c(z));
                                        B.lineTo(d.p2c(v), e.p2c(p));
                                        v != A && (B.lineTo(d.p2c(v), e.p2c(p)), B.lineTo(d.p2c(A), e.p2c(p)))
                                    }
                                }
                            }
                        }
                        0 < c && b(a.datapoints, 0, 0, a.xaxis, a.yaxis);
                        B.restore()
                    }
                    function sa(a) {
                        function b(a, c, d, e, g, f, j, m) {
                            for (var n = a.points, a = a.pointsize, q = 0; q < n.length; q += a) {
                                var w = n[q],
                                    r = n[q + 1];
                                null == w || (w < f.min || w > f.max || r < j.min || r > j.max) || (B.beginPath(), w = f.p2c(w), r = j.p2c(r) + e, "circle" == m ? B.arc(w, r, c, 0, g ? Math.PI : 2 * Math.PI, !1) : m(B, w, r, c, g), B.closePath(), d && (B.fillStyle = d, B.fill()), B.stroke())
                            }
                        }
                        B.save();
                        B.translate(M.left, M.top);
                        var c = a.points.lineWidth,
                            d = a.shadowSize,
                            e = a.points.radius,
                            g = a.points.symbol;
                        0 < c && 0 < d && (d /= 2, B.lineWidth = d, B.strokeStyle = "rgba(0,0,0,0.1)", b(a.datapoints, e, null, d + d / 2, !0, a.xaxis, a.yaxis, g), B.strokeStyle = "rgba(0,0,0,0.2)", b(a.datapoints, e, null, d / 2, !0, a.xaxis, a.yaxis, g));
                        B.lineWidth = c;
                        B.strokeStyle = a.color;
                        b(a.datapoints, e, K(a.points, a.color), 0, !1, a.xaxis, a.yaxis, g);
                        B.restore()
                    }
                    function E(a, b, c, d, e, g, f, j, m, n, r, t) {
                        var u, v, x, p;
                        r ? (p = v = x = !0, u = !1, r = c, c = b + d, e = b + e, a < r && (b = a, a = r, r = b, u = !0, v = !1)) : (u = v = x = !0, p = !1, r = a + d, a += e, e = c, c = b, c < e && (b = c, c = e, e = b, p = !0, x = !1));
                        if (!(a < j.min || r > j.max || c < m.min || e > m.max)) if (r < j.min && (r = j.min, u = !1), a > j.max && (a = j.max, v = !1), e < m.min && (e = m.min, p = !1), c > m.max && (c = m.max, x = !1), r = j.p2c(r), e = m.p2c(e), a = j.p2c(a), c = m.p2c(c), f && (n.beginPath(), n.moveTo(r, e), n.lineTo(r, c), n.lineTo(a, c), n.lineTo(a, e), n.fillStyle = f(e, c), n.fill()), 0 < t && (u || v || x || p)) n.beginPath(), n.moveTo(r, e + g), u ? n.lineTo(r, c + g) : n.moveTo(r, c + g), x ? n.lineTo(a, c + g) : n.moveTo(a, c + g), v ? n.lineTo(a, e + g) : n.moveTo(a, e + g), p ? n.lineTo(r, e + g) : n.moveTo(r, e + g), n.stroke()
                    }
                    function kd(a) {
                        B.save();
                        B.translate(M.left, M.top);
                        B.lineWidth = a.bars.lineWidth;
                        B.strokeStyle = a.color;
                        for (var b = "left" == a.bars.align ? 0 : -a.bars.barWidth / 2, c = a.datapoints, d = b + a.bars.barWidth, e = a.bars.fill ?
                            function(b, c) {
                                return K(a.bars, a.color, b, c)
                            } : null, g = a.xaxis, f = a.yaxis, j = c.points, c = c.pointsize, m = 0; m < j.length; m += c) null != j[m] && E(j[m], j[m + 1], j[m + 2], b, d, 0, e, g, f, B, a.bars.horizontal, a.bars.lineWidth);
                        B.restore()
                    }
                    function K(a, b, c, d) {
                        var e = a.fill;
                        if (!e) return null;
                        if (a.fillColor) return ad(a.fillColor, c, d, b);
                        a = I.color.parse(b);
                        a.a = "number" == typeof e ? e : 0.4;
                        a.normalize();
                        return a.toString()
                    }
                    function O(a) {
                        u.grid.hoverable && R("plothover", a, function(a) {
                            return !1 != a.hoverable
                        })
                    }
                    function S(a) {
                        u.grid.hoverable && R("plothover", a, function() {
                            return !1
                        })
                    }
                    function T(a) {
                        R("plotclick", a, function(a) {
                            return !1 != a.clickable
                        })
                    }
                    function R(b, c, d) {
                        var e = X.offset(),
                            g = c.pageX - e.left - M.left,
                            f = c.pageY - e.top - M.top,
                            j = m({
                                left: g,
                                top: f
                            });
                        j.pageX = c.pageX;
                        j.pageY = c.pageY;
                        var c = u.grid.mouseActiveRadius,
                            r = c * c + 1,
                            t = null,
                            v, x;
                        for (v = D.length - 1; 0 <= v; --v) if (d(D[v])) {
                            var p = D[v],
                                B = p.xaxis,
                                A = p.yaxis,
                                z = p.datapoints.points,
                                G = p.datapoints.pointsize,
                                C = B.c2p(g),
                                Ra = A.c2p(f),
                                F = c / B.scale,
                                J = c / A.scale;
                            B.options.inverseTransform && (F = Number.MAX_VALUE);
                            A.options.inverseTransform && (J = Number.MAX_VALUE);
                            if (p.lines.show || p.points.show) for (x = 0; x < z.length; x += G) {
                                var I = z[x],
                                    sa = z[x + 1];
                                if (null != I && !(I - C > F || I - C < -F || sa - Ra > J || sa - Ra < -J)) I = Math.abs(B.p2c(I) - g), sa = Math.abs(A.p2c(sa) - f), sa = I * I + sa * sa, sa < r && (r = sa, t = [v, x / G])
                            }
                            if (p.bars.show && !t) {
                                B = "left" == p.bars.align ? 0 : -p.bars.barWidth / 2;
                                p = B + p.bars.barWidth;
                                for (x = 0; x < z.length; x += G) if (I = z[x], sa = z[x + 1], A = z[x + 2], null != I && (D[v].bars.horizontal ? C <= Math.max(A, I) && C >= Math.min(A, I) && Ra >= sa + B && Ra <= sa + p : C >= I + B && C <= I + p && Ra >= Math.min(A, sa) && Ra <= Math.max(A, sa))) t = [v, x / G]
                            }
                        }
                        t ? (v = t[0], x = t[1], G = D[v].datapoints.pointsize, d = {
                            datapoint: D[v].datapoints.points.slice(x * G, (x + 1) * G),
                            dataIndex: x,
                            series: D[v],
                            seriesIndex: v
                        }) : d = null;
                        d && (d.pageX = parseInt(d.series.xaxis.p2c(d.datapoint[0]) + e.left + M.left), d.pageY = parseInt(d.series.yaxis.p2c(d.datapoint[1]) + e.top + M.top));
                        if (u.grid.autoHighlight) {
                            for (e = 0; e < ka.length; ++e) g = ka[e], g.auto == b && (!d || !(g.series == d.series && g.point[0] == d.datapoint[0] && g.point[1] == d.datapoint[1])) && Z(g.series, g.point);
                            d && V(d.series, d.datapoint, b)
                        }
                        a.trigger(b, [j, d])
                    }
                    function N() {
                        pa || (pa = setTimeout(ld, 30))
                    }
                    function ld() {
                        pa = null;
                        U.save();
                        U.clearRect(0, 0, Ja, aa);
                        U.translate(M.left, M.top);
                        var a, b;
                        for (a = 0; a < ka.length; ++a) if (b = ka[a], b.series.bars.show) fa(b.series, b.point);
                        else {
                            var c = b.series,
                                d = b.point;
                            b = d[0];
                            var d = d[1],
                                g = c.xaxis,
                                f = c.yaxis;
                            if (!(b < g.min || b > g.max || d < f.min || d > f.max)) {
                                var j = c.points.radius + c.points.lineWidth / 2;
                                U.lineWidth = j;
                                U.strokeStyle = I.color.parse(c.color).scale("a", 0.5).toString();
                                j *= 1.5;
                                b = g.p2c(b);
                                d = f.p2c(d);
                                U.beginPath();
                                "circle" == c.points.symbol ? U.arc(b, d, j, 0, 2 * Math.PI, !1) : c.points.symbol(U, b, d, j, !1);
                                U.closePath();
                                U.stroke()
                            }
                        }
                        U.restore();
                        e(da.drawOverlay, [U])
                    }
                    function V(a, b, c) {
                        "number" == typeof a && (a = D[a]);
                        if ("number" == typeof b) var d = a.datapoints.pointsize,
                            b = a.datapoints.points.slice(d * b, d * (b + 1));
                        d = ba(a, b); - 1 == d ? (ka.push({
                            series: a,
                            point: b,
                            auto: c
                        }), N()) : c || (ka[d].auto = !1)
                    }
                    function Z(a, b) {
                        null == a && null == b && (ka = [], N());
                        "number" == typeof a && (a = D[a]);
                        "number" == typeof b && (b = a.data[b]);
                        var c = ba(a, b); - 1 != c && (ka.splice(c, 1), N())
                    }
                    function ba(a, b) {
                        for (var c = 0; c < ka.length; ++c) {
                            var d = ka[c];
                            if (d.series == a && d.point[0] == b[0] && d.point[1] == b[1]) return c
                        }
                        return -1
                    }
                    function fa(a, b) {
                        U.lineWidth = a.bars.lineWidth;
                        U.strokeStyle = I.color.parse(a.color).scale("a", 0.5).toString();
                        var c = I.color.parse(a.color).scale("a", 0.5).toString(),
                            d = "left" == a.bars.align ? 0 : -a.bars.barWidth / 2;
                        E(b[0], b[1], b[2] || 0, d, d + a.bars.barWidth, 0, function() {
                            return c
                        }, a.xaxis, a.yaxis, U, a.bars.horizontal, a.bars.lineWidth)
                    }
                    function ad(a, b, c, d) {
                        if ("string" == typeof a) return a;
                        for (var b = B.createLinearGradient(0, c, 0, b), c = 0, e = a.colors.length; c < e; ++c) {
                            var g = a.colors[c];
                            if ("string" != typeof g) {
                                var f = I.color.parse(d);
                                null != g.brightness && (f = f.scale("rgb", g.brightness));
                                null != g.opacity && (f.a *= g.opacity);
                                g = f.toString()
                            }
                            b.addColorStop(c / (e - 1), g)
                        }
                        return b
                    }
                    var D = [],
                        u = {
                            colors: ["#edc240", "#afd8f8", "#cb4b4b", "#4da74d", "#9440ed"],
                            legend: {
                                show: !0,
                                noColumns: 1,
                                labelFormatter: null,
                                labelBoxBorderColor: "#ccc",
                                container: null,
                                position: "ne",
                                margin: 5,
                                backgroundColor: null,
                                backgroundOpacity: 0.85
                            },
                            xaxis: {
                                show: null,
                                position: "bottom",
                                mode: null,
                                color: null,
                                tickColor: null,
                                transform: null,
                                inverseTransform: null,
                                min: null,
                                max: null,
                                autoscaleMargin: null,
                                ticks: null,
                                tickFormatter: null,
                                labelWidth: null,
                                labelHeight: null,
                                reserveSpace: null,
                                tickLength: null,
                                alignTicksWithAxis: null,
                                tickDecimals: null,
                                tickSize: null,
                                minTickSize: null,
                                monthNames: null,
                                timeformat: null,
                                twelveHourClock: !1
                            },
                            yaxis: {
                                autoscaleMargin: 0.02,
                                position: "left"
                            },
                            xaxes: [],
                            yaxes: [],
                            series: {
                                points: {
                                    show: !1,
                                    radius: 3,
                                    lineWidth: 2,
                                    fill: !0,
                                    fillColor: "#ffffff",
                                    symbol: "circle"
                                },
                                lines: {
                                    lineWidth: 2,
                                    fill: !1,
                                    fillColor: null,
                                    steps: !1
                                },
                                bars: {
                                    show: !1,
                                    lineWidth: 2,
                                    barWidth: 1,
                                    fill: !0,
                                    fillColor: null,
                                    align: "left",
                                    horizontal: !1
                                },
                                shadowSize: 3
                            },
                            grid: {
                                show: !0,
                                aboveData: !1,
                                color: "#545454",
                                backgroundColor: null,
                                borderColor: null,
                                tickColor: null,
                                labelMargin: 5,
                                axisMargin: 8,
                                borderWidth: 2,
                                minBorderMargin: null,
                                markings: null,
                                markingsColor: "#f4f4f4",
                                markingsLineWidth: 2,
                                clickable: !1,
                                hoverable: !1,
                                autoHighlight: !0,
                                mouseActiveRadius: 10
                            },
                            hooks: {}
                        },
                        qb = null,
                        ca = null,
                        X = null,
                        B = null,
                        U = null,
                        W = [],
                        $ = [],
                        M = {
                            left: 0,
                            right: 0,
                            top: 0,
                            bottom: 0
                        },
                        Ja = 0,
                        aa = 0,
                        ha = 0,
                        ea = 0,
                        da = {
                            processOptions: [],
                            processRawData: [],
                            processDatapoints: [],
                            drawSeries: [],
                            draw: [],
                            bindEvents: [],
                            drawOverlay: [],
                            shutdown: []
                        },
                        ja = this;
                    ja.setData = g;
                    ja.setupGrid = A;
                    ja.draw = C;
                    ja.getPlaceholder = function() {
                        return a
                    };
                    ja.getCanvas = function() {
                        return qb
                    };
                    ja.getPlotOffset = function() {
                        return M
                    };
                    ja.width = function() {
                        return ha
                    };
                    ja.height = function() {
                        return ea
                    };
                    ja.offset = function() {
                        var a = X.offset();
                        a.left += M.left;
                        a.top += M.top;
                        return a
                    };
                    ja.getData = function() {
                        return D
                    };
                    ja.getAxes = function() {
                        var a = {};
                        I.each(W.concat($), function(b, c) {
                            c && (a[c.direction + (1 != c.n ? c.n : "") + "axis"] = c)
                        });
                        return a
                    };
                    ja.getXAxes = function() {
                        return W
                    };
                    ja.getYAxes = function() {
                        return $
                    };
                    ja.c2p = m;
                    ja.p2c = function(a) {
                        var b = {},
                            c, d, e;
                        for (c = 0; c < W.length; ++c) if ((d = W[c]) && d.used) if (e = "x" + d.n, null == a[e] && 1 == d.n && (e = "x"), null != a[e]) {
                            b.left = d.p2c(a[e]);
                            break
                        }
                        for (c = 0; c < $.length; ++c) if ((d = $[c]) && d.used) if (e = "y" + d.n, null == a[e] && 1 == d.n && (e = "y"), null != a[e]) {
                            b.top = d.p2c(a[e]);
                            break
                        }
                        return b
                    };
                    ja.getOptions = function() {
                        return u
                    };
                    ja.highlight = V;
                    ja.unhighlight = Z;
                    ja.triggerRedrawOverlay = N;
                    ja.pointOffset = function(a) {
                        return {
                            left: parseInt(W[f(a, "x") - 1].p2c(+a.x) + M.left),
                            top: parseInt($[f(a, "y") - 1].p2c(+a.y) + M.top)
                        }
                    };
                    ja.shutdown = function() {
                        pa && clearTimeout(pa);
                        X.unbind("mousemove", O);
                        X.unbind("mouseleave", S);
                        X.unbind("click", T);
                        e(da.shutdown, [X])
                    };
                    ja.resize = function() {
                        t();
                        v(qb);
                        v(ca)
                    };
                    ja.hooks = da;
                    for (var na = 0; na < d.length; ++na) {
                        var Pc = d[na];
                        Pc.init(ja);
                        Pc.options && I.extend(!0, u, Pc.options)
                    }
                    I.extend(!0, u, c);
                    null == u.xaxis.color && (u.xaxis.color = u.grid.color);
                    null == u.yaxis.color && (u.yaxis.color = u.grid.color);
                    null == u.xaxis.tickColor && (u.xaxis.tickColor = u.grid.tickColor);
                    null == u.yaxis.tickColor && (u.yaxis.tickColor = u.grid.tickColor);
                    null == u.grid.borderColor && (u.grid.borderColor = u.grid.color);
                    null == u.grid.tickColor && (u.grid.tickColor = I.color.parse(u.grid.color).scale("a", 0.22).toString());
                    for (c = 0; c < Math.max(1, u.xaxes.length); ++c) u.xaxes[c] = I.extend(!0, {}, u.xaxis, u.xaxes[c]);
                    for (c = 0; c < Math.max(1, u.yaxes.length); ++c) u.yaxes[c] = I.extend(!0, {}, u.yaxis, u.yaxes[c]);
                    u.xaxis.noTicks && null == u.xaxis.ticks && (u.xaxis.ticks = u.xaxis.noTicks);
                    u.yaxis.noTicks && null == u.yaxis.ticks && (u.yaxis.ticks = u.yaxis.noTicks);
                    u.x2axis && (u.xaxes[1] = I.extend(!0, {}, u.xaxis, u.x2axis), u.xaxes[1].position = "top");
                    u.y2axis && (u.yaxes[1] = I.extend(!0, {}, u.yaxis, u.y2axis), u.yaxes[1].position = "right");
                    u.grid.coloredAreas && (u.grid.markings = u.grid.coloredAreas);
                    u.grid.coloredAreasColor && (u.grid.markingsColor = u.grid.coloredAreasColor);
                    u.lines && I.extend(!0, u.series.lines, u.lines);
                    u.points && I.extend(!0, u.series.points, u.points);
                    u.bars && I.extend(!0, u.series.bars, u.bars);
                    null != u.shadowSize && (u.series.shadowSize = u.shadowSize);
                    for (c = 0; c < u.xaxes.length; ++c) r(W, c + 1).options = u.xaxes[c];
                    for (c = 0; c < u.yaxes.length; ++c) r($, c + 1).options = u.yaxes[c];
                    for (var ia in da) u.hooks[ia] && u.hooks[ia].length && (da[ia] = da[ia].concat(u.hooks[ia]));
                    e(da.processOptions, [u]);
                    ia = a.children("canvas.base");
                    c = a.children("canvas.overlay");
                    0 == ia.length || 0 == c ? (a.html(""), a.css({
                        padding: 0
                    }), "static" == a.css("position") && a.css("position", "relative"), t(), qb = x(!0, "base"), ca = x(!1, "overlay"), ia = !1) : (qb = ia.get(0), ca = c.get(0), ia = !0);
                    B = qb.getContext("2d");
                    U = ca.getContext("2d");
                    X = I([ca, qb]);
                    ia && (a.data("plot").shutdown(), ja.resize(), U.clearRect(0, 0, Ja, aa), X.unbind(), a.children().not([qb, ca]).remove());
                    a.data("plot", ja);
                    g(b);
                    A();
                    C();
                    u.grid.hoverable && (X.mousemove(O), X.mouseleave(S));
                    u.grid.clickable && X.click(T);
                    e(da.bindEvents, [X]);
                    var ka = [],
                        pa = null
                },
                qa = function(a, b) {
                    return b * Math.floor(a / b)
                };
            I.plot = function(a, b, c) {
                return new pa(I(a), b, c, I.plot.plugins)
            };
            I.plot.version = "0.7";
            I.plot.plugins = [];
            I.plot.formatDate = function(a, b, c) {
                var d = function(a) {
                        a = "" + a;
                        return 1 == a.length ? "0" + a : a
                    },
                    e = [],
                    g = !1,
                    f = !1,
                    j = a.getUTCHours(),
                    m = 12 > j;
                null == c && (c = "Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec".split(" ")); - 1 != b.search(/%p|%P/) && (12 < j ? j -= 12 : 0 == j && (j = 12));
                for (var r = 0; r < b.length; ++r) {
                    var x = b.charAt(r);
                    if (g) {
                        switch (x) {
                            case "h":
                                x = "" + j;
                                break;
                            case "H":
                                x = d(j);
                                break;
                            case "M":
                                x = d(a.getUTCMinutes());
                                break;
                            case "S":
                                x = d(a.getUTCSeconds());
                                break;
                            case "d":
                                x = "" + a.getUTCDate();
                                break;
                            case "m":
                                x = "" + (a.getUTCMonth() + 1);
                                break;
                            case "y":
                                x = "" + a.getUTCFullYear();
                                break;
                            case "b":
                                x = "" + c[a.getUTCMonth()];
                                break;
                            case "p":
                                x = m ? "am" : "pm";
                                break;
                            case "P":
                                x = m ? "AM" : "PM";
                                break;
                            case "0":
                                x = "", f = !0
                        }
                        x && f && (x = d(x), f = !1);
                        e.push(x);
                        f || (g = !1)
                    } else "%" == x ? g = !0 : e.push(x)
                }
                return e.join("")
            };
            return a.plot || $a.plot
        };
        var Fa;
        Fa || (Fa = {});
        (function() {
            function a(a) {
                return 10 > a ? "0" + a : a
            }
            function b(a) {
                e.lastIndex = 0;
                return e.test(a) ? '"' + a.replace(e, function(a) {
                    var b = r[a];
                    return "string" === typeof b ? b : "\\u" + ("0000" + a.charCodeAt(0).toString(16)).slice(-4)
                }) + '"' : '"' + a + '"'
            }
            function c(a, d) {
                var e, r, p, A, C = f,
                    F, E = d[a];
                E && ("object" === typeof E && "function" === typeof E.toJSON) && (E = E.toJSON(a));
                "function" === typeof x && (E = x.call(d, a, E));
                switch (typeof E) {
                    case "string":
                        return b(E);
                    case "number":
                        return isFinite(E) ? String(E) : "null";
                    case "boolean":
                    case "null":
                        return String(E);
                    case "object":
                        if (!E) return "null";
                        f += m;
                        F = [];
                        if ("[object Array]" === Object.prototype.toString.apply(E)) {
                            A = E.length;
                            for (e = 0; e < A; e += 1) F[e] = c(e, E) || "null";
                            p = 0 === F.length ? "[]" : f ? "[\n" + f + F.join(",\n" + f) + "\n" + C + "]" : "[" + F.join(",") + "]";
                            f = C;
                            return p
                        }
                        if (x && "object" === typeof x) {
                            A = x.length;
                            for (e = 0; e < A; e += 1)"string" === typeof x[e] && (r = x[e], (p = c(r, E)) && F.push(b(r) + (f ? ": " : ":") + p))
                        } else for (r in E) Object.prototype.hasOwnProperty.call(E, r) && (p = c(r, E)) && F.push(b(r) + (f ? ": " : ":") + p);
                        p = 0 === F.length ? "{}" : f ? "{\n" + f + F.join(",\n" + f) + "\n" + C + "}" : "{" + F.join(",") + "}";
                        f = C;
                        return p
                }
            }
            "function" !== typeof Date.prototype.toJSON && (Date.prototype.toJSON = function() {
                return isFinite(this.valueOf()) ? this.getUTCFullYear() + "-" + a(this.getUTCMonth() + 1) + "-" + a(this.getUTCDate()) + "T" + a(this.getUTCHours()) + ":" + a(this.getUTCMinutes()) + ":" + a(this.getUTCSeconds()) + "Z" : null
            }, String.prototype.toJSON = Number.prototype.toJSON = Boolean.prototype.toJSON = function() {
                return this.valueOf()
            });
            var d = /[