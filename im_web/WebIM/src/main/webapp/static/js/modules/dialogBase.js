define(function(require,exports,module){
	require('$');
	require('jquery/slider/jquery-slider');
	/**
	 * 监听窗口Resize初始化
	 * */
	function initResize() {
		var windowSize = null,
		content = $('.content'),
		navHeight = $('#nav').outerHeight(),
		layout = $('.all'),
		dialog = $('.top', content),
		typeHeight = $('.typing', content).outerHeight();

		function resizeFun() {
			var docWidth = Math.max($(window).width(), 620);
			var docHeight = Math.max($(window).height(), 542);
			var size = docWidth + 'x' + docHeight;
			if (windowSize == size)
				return;
			windowSize = size;
			var contentHeight = docHeight - navHeight - 2;
			layout.width(docWidth);
			content.height(contentHeight);
			dialog.height(contentHeight - typeHeight);
		};

		resizeFun();
		$(window).bind('resize.im', function () {
			resizeFun();
		});
	};

	/**
	 * 左侧工具条初始化
	 * */
	function initToolbar() {
		function initSingle(jThis) {
			var type = jThis.attr('class'),
			hoverClz = type + '_mouserover',
			clickClz = type + '_up';
			jThis.css('cursor', 'pointer').hover(function () {
				jThis.addClass(hoverClz);
			}, function () {
				jThis.removeClass(hoverClz);
			}).bind('click.im', function () {
				jThis.toggleClass(clickClz);
				// 表情判断
				if (type == 'face') {
					$(document).unbind('.im_face_popup');
					if ($('#J_face_popup').toggle().is(':visible')) {
						setTimeout(function () {
							$(document).bind('click.face_popup', function () {
								$('#J_face_popup').hide();
								jThis.removeClass(clickClz);
								$(document).unbind('.face_popup');
								return false;
							});
						}, 0);
					}
				} else if (type == 'ti') {
					$('#J_ti_tb').toggle();
				}

			});
		};

		/*初始化表情,取消事件冒牌*/
		$('#J_face_popup').bind('click.face_popup', function () {
			return false;
		});

		/*工具栏*/
		$('#D_left .title').children().each(function () {
			initSingle($(this));
		});

		//初始化字体条
		function initSingleTi(jThis) {
			var type = jThis.attr('class'),
			hoverClz = type + '_mouserover',
			clickClz = type + '_up';
			jThis.css('cursor', 'pointer').hover(function () {
				jThis.addClass(hoverClz);
			}, function () {
				jThis.removeClass(hoverClz);
			}).bind('click.im', function () {
				//关闭判断
				if (type == 'cha') {
					$('#J_ti_tb').hide();
					$('#D_left .title .ti').removeClass('ti_up');
					return;
				}
				
				//关闭判断
				if (type == 'b') {
					$('#J_ti_tb').hide();
					$('#D_left .title .ti').removeClass('ti_up');
					return;
				}
				
				//关闭判断
				if (type == 'b') {
					$('#J_ti_tb').hide();
					$('#D_left .title .ti').removeClass('ti_up');
					return;
				}
				
				if (jThis.hasClass(clickClz)) {
					jThis.removeClass(clickClz);
				} else {
					jThis.addClass(clickClz);
				}
				clicked = !clicked;
			});
		};

		/*Ti工具栏*/
		$('#J_ti_tb .font').children().each(function () {
			initSingleTi($(this));
		});

		/*CHA*/
		initSingleTi($('#J_ti_tb .cha'));
	};

	/**
	 * 发送按钮初始化
	 * */
	function initSendBtn() {
		var btns = [];

		$('#J_send_state').bind('click.im', function () {
			$(document).unbind('.im_send');
			$('#J_send_state_warp').toggle();
			$(this).addClass('up_up');

			setTimeout(function () {
				$(document).bind('click.im_send', function () {
					$('#J_send_state_warp').hide();
					$(document).unbind('.im_send');
				});
			}, 0);
		}).bind('mouseout.im', function () {
			$(this).removeClass('up_up');
		});

		$('#J_send_state_warp a').each(function () {
			var jThis = $(this),
			id = jThis.attr('id');
			btns[id] = jThis;

			jThis.bind('click.im', function () {
				for (var i in btns) {
					var b = btns[i];
					if (i != id) {
						b.removeClass('gou');
					} else {
						b.addClass('gou');
					}
				}
				return false;
			});
		});
	};

	function initShowMoreInfo() {
		$('#J_show_more_info').bind('click.im', function () {
			$('<div class="name"><p>difhsflsjlkds</p><ins>(2014-08-29 12:20:20)</ins></div><span>亲，您看看这款哦！</span><div class="buyers"><em>我心飞翔</em><ins>(2014-08-29 12:20:20)</ins></div><span>好的</span><div class="name"><p>difhsflsjlkds</p><ins>(2014-08-29 12:20:20)</ins></div><span>不客气！</span>').insertBefore($('#D_left .speak').children(":first"));
		});
	};

	initResize();
	initRightTab();
	initSlider();
	initToolbar();
	initSendBtn();
	initShowMoreInfo();
});