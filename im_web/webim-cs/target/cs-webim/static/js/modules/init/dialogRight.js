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
					// initSetTimeout();

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

	});
});