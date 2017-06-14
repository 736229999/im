// JavaScript Document


// select下拉框
$(function(){
	$(".p-common-select").each(function(){
		var s=$(this);
		var z=parseInt(s.css("z-index"));
		var dt=$(this).children("dt");
		var dd=$(this).children("dd");
		var _show=function(){dd.slideDown(200);dt.addClass("cur");s.css("z-index",z+1);};   //展开效果
		var _hide=function(){dd.slideUp(200);dt.removeClass("cur");s.css("z-index",z);};    //关闭效果
		dt.click(function(){dd.is(":hidden")?_show():_hide();});
		dd.find("a").click(function(){dt.html($(this).html());_hide();});     //选择效果（如需要传值，可自定义参数，在此处返回对应的"value"值 ）
		$("body").click(function(i){ !$(i.target).parents(".p-common-select").first().is(s) ? _hide():"";});
	})
	
})


// 可修改宽度的select下拉框
$(function(){
	$(".p-common-autowidthselect").each(function(){
		var s=$(this);
		var z=parseInt(s.css("z-index"));
		var dt=$(this).children("dt");
		var dd=$(this).children("dd");
		var _show=function(){dd.slideDown(200);dt.addClass("cur");s.css("z-index",z+1);};   //展开效果
		var _hide=function(){dd.slideUp(200);dt.removeClass("cur");s.css("z-index",z);};    //关闭效果
		dt.click(function(){dd.is(":hidden")?_show():_hide();});
		dd.find("a").click(function(){dt.html($(this).html());_hide();});     //选择效果（如需要传值，可自定义参数，在此处返回对应的"value"值 ）
		$("body").click(function(i){ !$(i.target).parents(".p-common-autowidthselect").first().is(s) ? _hide():"";});
	})
	
})

//品牌资质上传
$(function(){

	//添加资质材料
	$('#js_tabsBoxTable').on('click','#p-ppglBtn',(function(){
		var sTr='<tr><td><input type="text" class="w220 p-tablecommonTxt p-tablecommonTxthackIE" /></td><td><input type="button" class="upload-file" value="上传文件" /></td><td><input type="text" class="p-tablecommonTxt w220 mr10 icon_calendar" onClick="WdatePicker()" /><input type="checkbox" class="commonCheckBox" id="" /><font>长期</font></td><td><div class="table-right-gd w60"><a><font class="col0066bb js_delete fonthackIE">删除</font></a></div></td></tr>';

		var target=$(this).closest('tr');

		target.before(sTr);

	}));

	//删除
	$('#js_tabsBoxTable').on('click','.js_delete',(function(){
		if(confirm("确定删除吗？")){
			$(this).closest('tr').remove();
		}

	}));

	



})
