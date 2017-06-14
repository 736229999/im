seajs.use([
    '$',
    'jquery/slider/jquery-slider'
], function (jq,
             slider) {
    $(function () {
        /**
         * 右侧店铺TAB效果初始化
         * */

        function initRightTab() {
            $(document).on('click.im', '#J_info_tab p', function () {
                $('#J_info_tab p').each(function () {
                    var self = $(this);

                    self.removeClass('on');
                    $('#' + self.attr('id') + '_info').hide();
                });

                var $this = $(this);
                $this.addClass('on');
                $('#' + $this.attr('id') + '_info').show();
            });

            $('#J_slider_wrap').css('height', 0);
        }

        /**
         * 右下角Slider效果初始化
         * */
        function initSlider() {
            var show = true;

            $('#J_slider_toggle').closest('.title').add('#J_slider_toggle').bind('click.im', function (e) {
                show ? myhide() : myshow();
                show = !show;

                e.preventDefault();
                return false;
            });

            $("#J_slider").easySlider({
                auto: true,
                continuous: true,
                numeric: true,
                controlsShow: false,
                numericCustom: true,
                numericId: 'J_slider_control'
            });

            myshow();
        };

        function myshow() {
            $("#J_slider_wrap").animate({
                'height': '132px'
            }, 300);
            $("#J_slider_toggle").removeClass('triangle');
        }

        function myhide() {
            $("#J_slider_wrap").animate({
                'height': '0'
            }, 300);
            $("#J_slider_toggle").addClass('triangle');
        }

        initRightTab();

        $(document).bind('INIT_SLIDER', function () {
            initSlider();
        });

        $(document).bind('HIDE_SLIDER', function () {
            $('#J_slider_toggle').closest('.title').add('#J_slider_toggle').unbind('click.im');
            myhide();
        });
    });
});