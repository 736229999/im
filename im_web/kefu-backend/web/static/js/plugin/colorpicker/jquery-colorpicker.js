/**
 * jQuery插件：颜色拾取器
 *
 * @author  Karson
 * @url     http://www.songyouhui.com
 * @name    jquery.colorpicker.js
 * @since   2011-5-18 17:30:50
 */

var ColorHex = new Array('00', '33', '66', '99', 'CC', 'FF');
var SpColorHex = new Array('FF0000', '00FF00', '0000FF', 'FFFF00', '00FFFF', 'FF00FF');
var colorTable = initColorTable();
var colorPanel = null;

function initColorTable() {
    var colorTable = '';
    var colorValue = '';
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 6; j++) {
            colorTable = colorTable + '<tr height=12>'
            colorTable = colorTable + '<td width=11 rel="#000000" style="background-color:#000000">'
            colorValue = i == 0 ? ColorHex[j] + ColorHex[j] + ColorHex[j] : SpColorHex[j];
            colorTable = colorTable + '<td width=11 rel="#' + colorValue + '" style="background-color:#' + colorValue + '">'
            colorTable = colorTable + '<td width=11 rel="#000000" style="background-color:#000000">'
            for (k = 0; k < 3; k++) {
                for (l = 0; l < 6; l++) {
                    colorValue = ColorHex[k + i * 3] + ColorHex[l] + ColorHex[j];
                    colorTable = colorTable + '<td width=11 rel="#' + colorValue + '" style="background-color:#' + colorValue + '">'
                }
            }
        }
    }
    colorTable = '<table width=253 border="0" cellspacing="0" cellpadding="0" style="border:1px solid #000;">'
        + '<tr height=30><td colspan=21 bgcolor=#cccccc>'
        + '<table cellpadding="0" cellspacing="1" border="0" style="border-collapse: collapse">'
        + '<tr><td width="3"><td><input type="text" id="DisColor" size="6" disabled style="border:solid 1px #000000;background-color:#ffff00"></td>'
        + '<td width="3"><td><input type="text" id="HexColor" size="7" style="border:inset 1px;font-family:Arial;" value="#000000"><a href="javascript:void(0);" id="_cclose">关闭</a> | <a href="javascript:void(0);" id="_creset">清除</a></td></tr></table></td></table>'
        + '<table id="CT" border="1" cellspacing="0" cellpadding="0" style="border-collapse: collapse" bordercolor="000000"  style="cursor:pointer;">'
        + colorTable + '</table>';
    return colorTable;
};

function initColor() {
    $("body").append('<div id="colorpanel" style="position: absolute; display: none;"></div>');

    var panel = $("#colorpanel").html(colorTable);
    $("#_cclose").css({
        "font-size": "12px",
        "padding-left": "20px"
    });

    $("#CT tr td").mouseover(function () {
        var color = $(this).css("background-color");
        $("#DisColor").css("background", color);
        $("#HexColor").val($(this).attr("rel"));
    })

    return panel;
};


$.fn.colorpicker = function (options, param) {
    if (typeof options == 'string') {
        return $.fn.colorpicker.methods[options](this, param);
    }

    options = options || {};

    if (!colorPanel) {
        colorPanel = initColor()
    }
    ;

    return this.each(function () {
        var state = $.data(this, 'colorPicker');

        if (state) {
            opts = $.extend(state.options, options);
        } else {
            opts = $.extend({}, $.fn.colorpicker.defaults, options || {});
        }

        //存放数据
        $.data(this, 'colorPicker', {
            options: opts
        });

        var $target = $(this);

        function hidePanel() {
            $("#colorpanel").hide();
            opts.onHidePanel.call($target[0]);
        };

        $("#colorpanel").unbind('.colorPicker').bind("click.colorPicker", function () {
            return false;
        });

        $("#_cclose").unbind('.colorPicker').bind("click.colorPicker", function () {
            hidePanel();
            return false;
        })

        $("#_creset").unbind('.colorPicker').bind("click.colorPicker", function () {
            hidePanel();
            opts.onReset.call($target[0]);
            return false;
        });

        $("#CT tr td").unbind(".colorPicker").bind('click.colorPicker', function () {
            var aaa = $(this).css("background");
            var color = opts.ishex ? $(this).attr("rel") : aaa;
            hidePanel();
            opts.onSelect.call($target[0], color);
            return false;
        });
    });
};

$.fn.colorpicker.methods = {
    showPanel: function (jq) {
        $(document).unbind('.colorPicker');
        var state = $.data(jq[0], 'colorPicker');
        var opts = state.options;
        //定位
        var ttop = $(jq[0]).offset().top + opts.deltaY;     //控件的定位点高
        var tleft = $(jq[0]).offset().left + opts.deltaX;    //控件的定位点宽

        $("#colorpanel").css({
            top: ttop,
            left: tleft
        }).fadeIn(100, function () {
            $(document).bind('click.colorPicker', function () {
                opts.onHidePanel.call(jq[0]);
                $(document).unbind('.colorPicker');
                $('#colorpanel').hide();
            });
        });
    },
    hidePanel: function (jq) {
        var state = $.data(jq[0], 'colorPicker');
        var opts = state.options;
        opts.onHidePanel.call(jq[0]);
        $("#colorpanel").hide();
    }
};

$.fn.colorpicker.defaults = {
    deltaX: 0,
    deltaY: 0,
    ishex: true, //是否使用16进制颜色值
    onSelect: function () {
    }, //回调函数
    onReset: function () {
    },
    onHidePanel: function () {
    }
};