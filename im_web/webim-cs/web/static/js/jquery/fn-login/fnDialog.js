;
(function(window) {

    var fnDialog = function(config) {

        //默认设置
        var defaults = {

            },
            tpl = [];

        //合并对象
        var opts = $.extend({}, defaults, config);


        tpl.push('<div class="u-diaolog-login J-fn-login">');

        tpl.push('<div class="u-title mb30"><h3 class="pl15">登录飞牛网</h3><i class="J-dialog-close"></i></div>');
        tpl.push('<iframe id="loginIframe" frameBorder=0 scrolling="no" style="border: 0px none;margin:0 auto" width="100%" height="350" src="'+member_url_view+'/gateway/popLogin?originUrl='+encodeURIComponent(window.location.href)+'"></iframe>');


        tpl.push('</div>');

        tpl.push('<div class="u-diaolog-mask J-dialog-m"></div>');


        $("body").append(tpl.join(""));

        var outwidth = $(".J-fn-login").outerWidth(),

            outheight = $(".J-fn-login").outerHeight();

        //设置垂直水平居中
        $(".J-fn-login").css({

            "margin-left": "-" + outwidth / 2 + "px",

            "margin-top": "-" + outheight / 2 + "px"
        });

        $(document).on("click", ".J-dialog-close", function() {

            $(this).parent().parent(".J-fn-login").remove();

            $(".J-dialog-m").remove();


        });

        //弹出dialog之后，需要做什么
        opts.callback && $.isFunction(opts.callback) && opts.callback();


    };

    window.fnDialog = fnDialog;

})(window);
