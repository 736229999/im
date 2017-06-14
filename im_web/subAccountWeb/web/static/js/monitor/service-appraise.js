/**
 * Created by wangchen on 2015/2/12.
 */

var serviceAppraise = (function () {
    var dataManager = {
        subMerchantIDList: []
    }

    return appManager = {
        init: function () {
            this.initData();
            this.initUI();
            this.bindUI();
        },
        initData: function () {
            $('#subMerchantList dd ul li').each(function () {
                var $this = $(this);
                if ($this.data('submerchantid')) {
                    dataManager.subMerchantIDList.push($this.data('submerchantid'));
                }
            });
        },
        initUI: function () {
            $tempdata = $('tmpdata');

            var serviceAppraiseList = $tempdata.data('serviceappraiselist') || [];
            this.paintAppraiseInfo(serviceAppraiseList);

            $tempdata.remove();
        },
        bindUI: function () {
            var self = this;

            $('#startTime').click(function () {
                WdatePicker({dateFmt: 'yyyy-MM-dd', minDate: '%y-#{%M-3}-%d', maxDate: '%y-%M-#\{%d\}'});
            });

            $('#endTime').click(function () {
                WdatePicker({dateFmt: 'yyyy-MM-dd', minDate: '%y-#{%M-3}-%d', maxDate: '%y-%M-#\{%d\}'});
            });

            $('#queryAppraiseInfo').click(function () {
                if (!self.checkParams()) {
                    return;
                }

                var param = {};
                param['startTime'] = generalUtil.parseDate($('#startTime').val()).getTime() / 1000;
                param['endTime'] = generalUtil.parseDate($('#endTime').val()).getTime() / 1000 + 24 * 3600;
                param['subAccountIDListStr'] = $('#subMerchantID').val();

                var selectedSubMerchantIDList = param['subAccountIDListStr'].toString().split(';');

                $('#serviceAppraiseInfoPanel>tr').hide();
                for (var i = 0; i < selectedSubMerchantIDList.length; i++) {
                    $('#appraiseInfoForSubMerchant' + selectedSubMerchantIDList[i]).show();
                }

                self.retrieveSubMerchantAppraiseInfo(param, function (serviceAppraiseList) {
                    self.paintAppraiseInfo(serviceAppraiseList || []);
                });
            });
        },
        checkParams: function () {
            if (!$('#startTime').val()) {
                alert('请选择开始时间');
                return false;
            }
            if (!$('#endTime').val()) {
                alert('请选择结束时间');
                return false;
            }

            return true;
        },
        paintAppraiseInfo: function (serviceAppraiseList) {
            $('#serviceAppraiseInfoPanel>tr>td:not(:first-child)').each(function () {
                $(this).text(0).css('color', 'black');
            });

            for (var i = 0; i < serviceAppraiseList.length; i++) {
                var currentServiceAppraise = serviceAppraiseList[i];
                $('#appraiseInfoForSubMerchant' + currentServiceAppraise.subAccountID).
                    find('td[data-appraiseid="' + currentServiceAppraise.appraise + '"]').
                    text(currentServiceAppraise.counter).css('color', 'red');
            }
        },
        retrieveSubMerchantAppraiseInfo: function (param, successCallback) {
            var self = this;

            $.post(
                '../service-appraise/query',
                param,
                function (result) {
                    if (!result || !result.success) {
                        alert(result && result.message || '服务器发生故障');
                    } else {
                        successCallback.call(self, result.data.serviceAppraiseStat);
                    }
                }
            )
        }
    }
})();

$(function () {
    serviceAppraise.init();
})