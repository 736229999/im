// JavaScript Document


//公共select控件
$(".commonSelect").each(function(){
	var s=$(this);
	var z=parseInt(s.css("z-index"));
	var dt=$(this).children("dt");
	var dd=$(this).children("dd");
	var _show=function(){dd.show();dt.addClass("cur");s.css("z-index",z+1);};   //展开效果
	var _hide=function(){dd.hide();dt.removeClass("cur");s.css("z-index",z);};    //关闭效果
	dt.click(function(){dd.is(":hidden")?_show():_hide();});
	dd.find("a").click(function(){dt.html($(this).html());_hide();});     //选择效果（如需要传值，可自定义参数，在此处返回对应的"value"值 ）
	$("body").click(function(i){ !$(i.target).parents(".commonSelect").first().is(s) ? _hide():"";});
});

//==================================
//table奇偶行变色
//==================================
$("table tr:even").css("background","#f6f6f6");

  //分组分流设置右侧tab切换
$('.g-right-Tabs a').click(function(){
    $(".g-right-Tabs a").removeClass("selected");
    $(this).addClass("selected");
    var index = $('.g-right-Tabs a').index(this);
    $(".g-right-content").hide().eq(index).show();

});

//==================================
//计算显示器高度
//==================================
 function initHeight() {

                var $mainContainer = $("#mainContainer");

                if($mainContainer.length) {

                    var nOffsetTop = $mainContainer.offset().top,

                        clientH = document.documentElement.clientHeight,

                        contentHeight = clientH - nOffsetTop;

                    $mainContainer.css("min-height", contentHeight-52);

                };

            };
window.onload = function() {
initHeight();
};

//==================================
//员工管理展开部门下拉框
//==================================
//==================================
//添加客服弹出层
//==================================
