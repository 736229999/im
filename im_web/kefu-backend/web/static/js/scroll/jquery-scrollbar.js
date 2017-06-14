/* ============================== */
/*
author      : HQY
description : 滚动条插件
date        : 2014-09-25
email       : hqy332211@163.com
 */
/* ============================== */
;(function($,window){
    
	function Scrollbar(root, options) {
		var oSelf = this,
		onselectstart,
		ondrag,
		oWrapper = root,
		oViewport = {
			obj : $('.fnj-scrollbar-viewport', root)
		},
		oContent = {
			obj : $('.fnj-scrollbar-overview', root)
		},
		oScrollbar = {
			obj : $('.fnj-scrollbar-scrollbar', root)
		},
		oTrack = {
			obj : $('.fnj-scrollbar-track', oScrollbar.obj)
		},
		oThumb = {
			obj : $('.fnj-scrollbar-thumb', oScrollbar.obj)
		},
		sAxis = options.axis === 'x',
		sDirection = sAxis ? 'left' : 'top',
		sSize = sAxis ? 'Width' : 'Height',
		iScroll = 0,
		iPosition = {
			start : 0,
			now : 0
		},
		iMouse = {},
		touchEvents = 'ontouchstart' in document.documentElement;

		function initialize() {
			oSelf.update();
			setEvents();

			return oSelf;
		}
		
		
		

		this.update = function (sScroll) {
			oViewport[options.axis] = oViewport.obj[0]['offset' + sSize];
			oContent[options.axis] = oContent.obj[0]['scroll' + sSize];
			oContent.ratio = oViewport[options.axis] / oContent[options.axis];

			oScrollbar.obj.toggleClass('fnj-scrollbar-disable', oContent.ratio >= 1);

			oTrack[options.axis] = options.size === 'auto' ? oViewport[options.axis] : options.size;
			oThumb[options.axis] = Math.min(oTrack[options.axis], Math.max(0, (options.sizethumb === 'auto' ? (oTrack[options.axis] * oContent.ratio) : options.sizethumb)));

			oScrollbar.ratio = options.sizethumb === 'auto' ? (oContent[options.axis] / oTrack[options.axis]) : (oContent[options.axis] - oViewport[options.axis]) / (oTrack[options.axis] - oThumb[options.axis]);

			iScroll = (sScroll === 'relative' && oContent.ratio <= 1) ? Math.min((oContent[options.axis] - oViewport[options.axis]), Math.max(0, iScroll)) : 0;
			iScroll = (sScroll === 'bottom' && oContent.ratio <= 1) ? (oContent[options.axis] - oViewport[options.axis]) : isNaN(parseInt(sScroll, 10)) ? iScroll : parseInt(sScroll, 10);

			setSize();
		};

		function setSize() {
			var sCssSize = sSize.toLowerCase();

			oThumb.obj.css(sDirection, iScroll / oScrollbar.ratio);
			oContent.obj.css(sDirection, -iScroll);
			iMouse.start = oThumb.obj.offset()[sDirection];

			oScrollbar.obj.css(sCssSize, oTrack[options.axis]);
			oTrack.obj.css(sCssSize, oTrack[options.axis]);
			oThumb.obj.css(sCssSize, oThumb[options.axis]);
		}

		function setEvents() {
			if (!touchEvents) {
				oThumb.obj.bind('mousedown.fnscrollbar', start);
				oTrack.obj.bind('mouseup.fnscrollbar', drag);
			} else {
				oViewport.obj[0].ontouchstart = function (event) {
					if (1 === event.touches.length) {
						start(event.touches[0]);
						event.stopPropagation();
					}
				};
			}

			if (options.scroll && window.addEventListener) {
				oWrapper[0].addEventListener('DOMMouseScroll', wheel, false);
				oWrapper[0].addEventListener('mousewheel', wheel, false);
			} else if (options.scroll) {
				oWrapper[0].onmousewheel = wheel;
			}
		}

		function start(event) {
			$("body").addClass("fnj-noSelect");
			onselectstart = document.body.onselectstart;
			ondrag = document.body.ondrag;
			document.body.onselectstart = document.body.ondrag = function(){
				return false;
			};

			var oThumbDir = parseInt(oThumb.obj.css(sDirection), 10);
			iMouse.start = sAxis ? event.pageX : event.pageY;
			iPosition.start = oThumbDir == 'auto' ? 0 : oThumbDir;

			if (!touchEvents) {
				$(document).bind('mousemove.fnscrollbar', drag);
				$(document).bind('mouseup.fnscrollbar', end);
				oThumb.obj.bind('mouseup.fnscrollbar', end);
			} else {
				document.ontouchmove = function (event) {
					event.preventDefault();
					drag(event.touches[0]);
				};
				document.ontouchend = end;
			}
		}

		function wheel(event) {
			if (oContent.ratio < 1) {
				var oEvent = event || window.event,
				iDelta = oEvent.wheelDelta ? oEvent.wheelDelta / 120 : -oEvent.detail / 3;

				iScroll -= iDelta * options.wheel;
				iScroll = Math.min((oContent[options.axis] - oViewport[options.axis]), Math.max(0, iScroll));

				oThumb.obj.css(sDirection, iScroll / oScrollbar.ratio);
				oContent.obj.css(sDirection, -iScroll);

				if (options.lockscroll || (iScroll !== (oContent[options.axis] - oViewport[options.axis]) && iScroll !== 0)) {
					oEvent = $.event.fix(oEvent);
					oEvent.preventDefault();
					// Trigger scroll events (TRANSIFEX patch)
					if (options.onscroll)
						options.onscroll();
				}
			}
		}

		function drag(event) {
			if (oContent.ratio < 1) {
				if (options.invertscroll && touchEvents) {
					iPosition.now = Math.min((oTrack[options.axis] - oThumb[options.axis]), Math.max(0, (iPosition.start + (iMouse.start - (sAxis ? event.pageX : event.pageY)))));
				} else {
					iPosition.now = Math.min((oTrack[options.axis] - oThumb[options.axis]), Math.max(0, (iPosition.start + ((sAxis ? event.pageX : event.pageY) - iMouse.start))));
				}

				iScroll = iPosition.now * oScrollbar.ratio;
				oContent.obj.css(sDirection, -iScroll);
				oThumb.obj.css(sDirection, iPosition.now);
				// Trigger scroll events (TRANSIFEX patch)
				if (options.onscroll)
					options.onscroll();
			}
		}

		function end() {
			document.body.onselectstart = onselectstart;
			document.body.ondrag = ondrag;
			$("body").removeClass('fnj-noSelect');
			
			$(document).unbind('mousemove.fnscrollbar', drag);
			$(document).unbind('mouseup.fnscrollbar', end);
			oThumb.obj.unbind('mouseup.fnscrollbar', end);
			document.ontouchmove = document.ontouchend = null;
		}

		return initialize();
	};

    
    
    
	$.fn.fnscrollbar = function (options, params) {
		if (typeof options == 'string'){
			return $.fn.fnscrollbar.methods[options](this, params);
		}
		
		options = options || {};
		
		return this.each(function(){
			var opts;
			var state = $.data(this, 'fnscrollbar');
			if (state){
				$(document).unbind('.fnscrollbar');//清空docment绑定事件
				opts = $.extend(state.options, options);
			}else{
				opts = $.extend({}, $.fn.fnscrollbar.defaults, options || {});
			}
			
			//存放数据
			$.data(this, 'fnscrollbar', {
				options: opts,
				bar : new Scrollbar($(this),opts)
			});
		});
	};
	
	
	$.fn.fnscrollbar.methods = {
		options: function(jq){
			return $.data(jq[0], 'fnscrollbar').options;
		},
		update : function(jq,param){
			return jq.each(function(){
				var state = $.data(this, 'fnscrollbar');
				if(state){
					state.bar.update(param);
				}
			});
			//return $(this).data('tsb').update(sScroll);
		}
	};
	
    $.fn.fnscrollbar.defaults = {
            axis         : 'y',    // vertical or horizontal scrollbar? ( x || y ).
            wheel        : 40,      // how many pixels must the mouswheel scroll at a time.
            scroll       : true,   // enable or disable the mousewheel.
            lockscroll   : true,   // return scrollwheel to browser if there is no more content.
            size         : 'auto',  // set the size of the scrollbar to auto or a fixed number.
            sizethumb    : 'auto',  // set the size of the thumb to auto or a fixed number.
            invertscroll : false,  // Enable mobile invert style scrolling
            onscroll     : null    // A callback function to capture scroll events (TRANSIFEX patch)
    };
}(jQuery,window));