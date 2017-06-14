var storeLightDiagnose = (function () {
    var dataManager = {
        merchantID: -1
    };

    return appManager = {
        init: function () {
            this.initUI();
            this.bindUI();
            this.initData();
        },
        initUI: function () {
        },
        bindUI: function () {
            var self = this;
            $('#diagnoseBtn').click(function () {
                self.diagnoseStoreLight();
            });
        },
        initData: function () {
            dataManager.merchantID = parseInt($('tmpdata').data('merchantid'));
            $('tmpdata').remove();
            this.diagnoseStoreLight();
        },
        diagnoseStoreLight: function () {
            $.ajax({
                url:'../store-light/diagnose',
                dataType: 'json',
                type:'post',
                data: {
                    merchantID: dataManager.merchantID
                },
                success: function (result) {
                    if (!result) {
                        alert('服务器发生故障');
                    } else {
                        if (!result.success) {
                            $('#diagnoseResultPanel>p.wrong-panel').show().children('font').text('亮灯发生错误：' + result.message);
                            $('#diagnoseResultPanel>p.right-panel').hide();
                        } else {
                            $('#diagnoseResultPanel>p.right-panel').show().children('font').text('恭喜，您的亮灯全部正常');
                            $('#diagnoseResultPanel>p.wrong-panel').hide();
                        }
                    }
                }
            });
        }
    }
})();

$(function () {
    storeLightDiagnose.init();
});