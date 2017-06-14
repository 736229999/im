seajs.use([
    '$',
   	'jquery/slider/jquery-slider',
   	'jquery/switchable/fnx-switchable',
],function(
	jq,
	slider,
	switchable
){
$(function(){
	/**
	 * 右侧店铺TAB效果初始化
	 * */
	var timer_1 = null,timer_2 = null;
	FNX.include('fnx/cmp/widget/widget').autoRenderAll();
	function initRightTab() {
		var tabs = [];
		$('#J_info_tab p').each(function () {
			var jThis = $(this),
			id = jThis.attr('id');
			tabs[id] = jThis;

			jThis.bind('click.im', function () {
				for (var i in tabs) {
					var e = tabs[i];
					if (i != id) {
						e.removeClass('on');
						$('#' + i + '_info').hide();
					} else {
						e.addClass('on');
						$('#' + i + '_info').show();
					}
				}

				$("#J_slider_wrap").css('height', '0');
				initSetTimeout();

			});
		});
	}
	 /*常见问题	*/

	 function initQa(){
	 	// alert($('[js-hook="cont-tab"').size())
	 	var oWrap = $('[js-hook="cont-tab"');
	 	var oQ = oWrap.find('.question');
	 	var oA = oWrap.find('.p_answer');


	 	oQ.click(function(){
	 		oA.hide();
	 			$(this).next('.p_answer').show();
	 	})

	 }
	 /**
	 * 包裹数据
	 * */
	 function initPakages(){
	 	var oWrap = $('[js-hook="tab"');
	 	var oPre = oWrap.find('.prev');
	 	var aNav = oWrap.find('.ui-navs li');
	 	var aPanel = oWrap.find('.ui-panels');
	 	var oNext = oWrap.find('.next');
	 	var num =0;


	 	aNav.each(function(i){
	 		$(this).index = i;
	 	});
	 	aNav.on('click',function(){
	 		var num = $(this).index();
	 		show(num);
	 	});

	 	oPre.on('click',function(){
	 		num = $(this).next().find('.cur').index();
	 		num--;
	 		aNav.removeClass('cur');

	 		if(num < 0){
	 			num = 0;
	 		}else if(num > aNav.length - 1){
	 			num = aNav.length - 1;
	 		}
	 		show(num);
	 	});

	 	oNext.on('click',function(){
	 		num = $(this).prev().find('.cur').index();
	 		num++;


	 		if(num < 0){
	 			num = 0;
	 		}else if(num > aNav.length - 1){
	 			num = aNav.length - 1;
	 		}

	 		show(num);
	 	});

	 	function show(num){
	 		aNav.removeClass('cur');
	 		aPanel.removeClass('cur');
	 		aNav.eq(num).addClass('cur');
	 		aPanel.eq(num).addClass('cur');
	 	}
	 }

	/**
	 * 右下角Slider效果初始化
	 * */
	function initSlider() {
		var show = true;
		$("#J_slider_wrap").css('height', '0');
		initSetTimeout();

		$('#J_slider_toggle').bind('click.im', function () {
			clearTimeout(timer_1);
			clearTimeout(timer_2);
			show ? myshow() : myhide();
			show = !show;
		});

		$("#J_slider").easySlider({
			auto : true,
			continuous : true,
			numeric : true,
			controlsShow : false,
			numericCustom : true,
			numericId : 'J_slider_control'
		});

		$("[js-hook='slider']").easySlider({
			auto : false,
			prevText: 'prevId',
		nextText: 'nextBtn',
			continuous : true,
			numeric : true,
			controlsShow : false,
			numericCustom : true,
			// numericId : 'J_slider_control'
		});
	};

	function myshow() {
		$("#J_slider_wrap").animate({
			'height' : '132px'
		}, 300);
		$("#J_slider_toggle").removeClass('triangle');
	}

	function myhide() {
		$(this).addClass('triangle');
		$("#J_slider_wrap").animate({
			'height' : '0'
		}, 300);
		$("#J_slider_toggle").addClass('triangle');
	}

	/*延迟6秒动画*/
	function initSetTimeout() {
		clearTimeout(timer_1);
		clearTimeout(timer_2);
		timer_1 = setTimeout(myshow, 6000);
		timer_2 = setTimeout(myhide, 12000);
	}

	initRightTab();
	// initSlider();
	initQa();
	initPakages();
});
});