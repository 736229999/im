// JavaScript Document


//==================================
//table奇偶行变色
//==================================
$(function(){
	$(".structure-Table tr:even").css("background","#eeeeee");
});

//==================================
//员工管理展开部门下拉框
//==================================
$(function(){
	$(".aShowSelectBox").click(function(){		
		$(this).next().slideDown(200,function(){
			var $target = $(this);
			
			$(document).bind('click.structure',function(){
				setTimeout(function(){
					$(document).unbind('.structure');
					$target.hide();
				},0);
			});
		});
		
	});
});

// 可修改宽度的select下拉框
$(function(){
	$(".commonSelect").each(function(){
		var s=$(this);
		var z=parseInt(s.css("z-index"));
		var dt=$(this).children("dt");
		var dd=$(this).children("dd");
		var _show=function(){dd.slideDown(100);dt.addClass("cur");s.css("z-index",z+1);};   //展开效果
		var _hide=function(){dd.slideUp(100);dt.removeClass("cur");s.css("z-index",z);};    //关闭效果
		dt.click(function(){
		dd.is(":hidden")?_show():_hide();});
		dd.find("a").click(function(){dt.html($(this).html());_hide();});     //选择效果（如需要传值，可自定义参数，在此处返回对应的"value"值 ）
		$("body").click(function(i){ !$(i.target).parents(".commonSelect").first().is(s) ? _hide():"";
		});
		
	})
	
})
//==================================
//显示编辑角色div
//==================================
$(function(){
	$(".A-for-ShowroleEdit").click(function(){
		$(this).css("display","none").next().css("display","block");
		});
})

//==================================
//员工管理角色权限预览收起
//==================================
$(function(){
	$(".sq").click(function(){
		var text =  $("font",this).text();
		if(text == "收起"){$("font",this).delay(1000).text("显示");}
		else{if(text == "显示"){$("font",this).delay(1000).text("收起");}}
		 $(this).toggleClass("sqs");
 $(this).parents("li.short").next().animate({height: 'toggle', opacity: 'toggle'}, "slow");
});
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

                    $mainContainer.css("height", contentHeight);

                };

            };
window.onload = function() { 
initHeight();
}; 

