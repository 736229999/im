/**
 * Created by wangchen on 2015/2/7.
 */
var diverseConfig = (function () {
    return {
        init: function (options) {
            this.initUI();
            this.bindUI();
            this.initData(options);
        },
        initData: function (options) {
        },
        initUI: function () {
        },
        bindUI: function () {
            var self = this;

            $(document).on('click', '#subMerchantList ul>li', function () {
                var $this = $(this);
                $('#subMerchantList dt').
                    data('submerchantid', $this.data('submerchantid')).
                    html($this.text());
            });

            $(document).on('click', '#saveConfigBtn', function () {
                if (!self.checkParams()) {
                    return;
                }

                var params = self.constructRequestParams();

                $.post(
                    '../diver-config/save-config.htm',
                    params,
                    function (result) {
                        if (!result || !result.success) {
                            alert(result.message || '服务器出现故障');
                            return;
                        } else {
                            alert('保存成功');
                        }
                    },
                    'json'
                )
            });
        },
        constructRequestParams: function () {
            var params = {};

            params['merchantID'] = $('#subMerchantList').data('merchantid');
            params['subMerchantID'] = $('#subMerchantList dt').data('submerchantid');
            params['diverseType'] = parseInt($('#diverseTypeConfigPanel input[name="flowSetting"]:checked').val());

            return params;
        },
        checkParams: function () {
            var divserseType = $('#diverseTypeConfigPanel input[name="flowSetting"]:checked');
            if (divserseType.length == 0) {
                alert('请选择分流方式');
                return;
            }

            var selectedSubMerchantID = $('#subMerchantList dt').data('submerchantid');
            if (!selectedSubMerchantID) {
                alert('请选择要代理的账户');
                return false;
            }

            return true;
        }
    }
})();

$(function () {
    diverseConfig.init();
});
