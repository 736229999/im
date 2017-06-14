/**
 * Created by wangchen on 2015/2/10.
 */

function checkMobile(str,ishow) {
    var self =storeLightConfiger;
    if(str == "" && ishow == "1"){
        alert("请填写联系方式");
        return false;
    }
    if(str == "") return true;
    var
        re = /^1\d{10}$/
    if (re.test(str)) {
        return true;
    } else {
        alert("手机号格式不正确");
    }
    return false;
}

function checkPhone(str,ishow){
    var self =storeLightConfiger;

    if(str == "" && ishow == "1"){
        alert("请填写联系方式");
        return false;
    }
    if(str == "") return true;
    var re = /^0\d{2,3}-?\d{1,10}-?\d{1,8}$/;
    var re2 = /\d{1,10}-?\d{1,8}$/;
    if(re.test(str) || re2.test(str) ){
       return true;
    }else{
        alert("电话格式不正确");
    }
    return false;
};
var storeLightConfiger = (function () {
    var dataManager = {
        merchantID: -1,
        storeLightConfig: {
            merchantID: -1,
            firstWorkDayStart: '1',
            firstWorkDayEnd: '1',
            firstWorkTimeStart: '00:00',
            firstWorkTimeEnd: '00:00',
            isFirstWorkTimeShow: '0',
            secondWorkDayStart: '1',
            secondWorkDayEnd: '1',
            secondWorkTimeStart: '00:00',
            secondWorkTimeEnd: '00:00',
            isSecondWorkTimeShow: '0',
            telphone: '',
            phone: '',
            isTelphoneShow: '0',
            isPhoneShow: '0'
        },
        serviceGroupMap: {}
    };

    return appManager = {
        telphoneRegex: /^\d{11}$/,
        init: function () {
            this.initData();
            this.initUI();
            this.bindUI();
        },
        initData: function () {
            $tmpdata = $('tmpdata');

            var serviceGroupList = $tmpdata.data('servicegrouplist');
            for (var i = 0; i < serviceGroupList.length; i++) {
                var currentServiceGroup = serviceGroupList[i];
                currentServiceGroup.showOrder = serviceGroupList.length - i;
                dataManager.serviceGroupMap[currentServiceGroup.groupID] = currentServiceGroup;
            }

            dataManager.merchantID = parseInt($('tmpdata').data('merchantid'));
            dataManager.storeLightConfig.merchantID = dataManager.merchantID;

            $tmpdata.remove();
        },
        initUI: function () {

        },
        checkParams: function () {
            if ($('#firstTimePanel .start-time').val() == ''
                || $('#firstTimePanel .end-time').val() == ''
                || $('#secondTimePanel .start-time').val() == ''
                || $('#secondTimePanel .end-time').val() == '') {
                alert('请正确填写时间');
                return false;
            }

            if ( $('#phone + input.zzhCommonCheck').prop('checked') &&  $('#phone').val() == ''){
                alert('请填写联系电话');
                return false;
            }

            if( $('#phone + input.zzhCommonCheck').prop('checked') && $('#phone').val() == '' ){
                alert('请正确填写联系电话');
                return false;
            }

            if( $('#telphone + input.zzhCommonCheck').prop('checked') && ! this.telphoneRegex.test($('#telphone').val()) ){
                alert('请正确填写联系手机');
                return false;
            }
            return true;
        },
        bindUI: function () {
            var self = this;

            $('#serviceGroupList>div em').click(function (e) {
                var $this = $(this);
                var serviceGroupItem = $this.closest('div.service-group');
                var groupID = serviceGroupItem.data('groupid');
                var serviceGroupInfo = dataManager.serviceGroupMap[groupID];

                if ($this.hasClass('down')) {
                    var nextNum = serviceGroupItem.nextAll().length;
                    if (nextNum != 0) {
                        var next = serviceGroupItem.next();
                        var nextServiceGroupInfo = dataManager.serviceGroupMap[next.data('groupid')];

                        serviceGroupInfo.showOrder = nextServiceGroupInfo.showOrder;
                        nextServiceGroupInfo.showOrder = nextServiceGroupInfo.showOrder + 1;

                        serviceGroupItem.insertAfter(next);
                        e.preventDefault();
                        return false;
                    }
                } else {
                    var prevNum = serviceGroupItem.prevAll().length;
                    if (prevNum != 0) {
                        var prev = serviceGroupItem.prev();
                        var prevServiceGroupInfo = dataManager.serviceGroupMap[prev.data('groupid')];

                        prevServiceGroupInfo.showOrder = serviceGroupInfo.showOrder;
                        serviceGroupInfo.showOrder = serviceGroupInfo.showOrder + 1;

                        !!prev && serviceGroupItem.insertBefore(prev);
                        e.preventDefault();
                        return false;
                    }
                }
            });

            $(document).on('click', '#serviceGroupList>div input.zzhCommonCheck', function () {
                var $this = $(this);
                var serviceGroupItem = $this.closest('div.service-group');
                var groupID = serviceGroupItem.data('groupid');
                var serviceGroupInfo = dataManager.serviceGroupMap[groupID];

                serviceGroupInfo.isShowOnStore = $this.prop('checked') ? '1' : '0';
            })

            $(document).on('click', '#firstTimePanel>div.date-box>dl>dd li, #secondTimePanel>div.date-box>dl>dd li', function () {
                var $this = $(this);
                $this.closest('dd').prev('dt').data('value', $this.children('a').data('value'));
            });

            $('#saveStoreLightConfig').click(function () {
                if (!self.checkParams()) {
                    return;
                }

                $.extend(dataManager.storeLightConfig, {
                    firstWorkDayStart: $('#firstTimePanel .start-date dt').data('value'),
                    firstWorkDayEnd: $('#firstTimePanel .end-date dt').data('value'),
                    firstWorkTimeStart: $('#firstTimePanel input.start-time').val(),
                    firstWorkTimeEnd: $('#firstTimePanel input.end-time').val(),
                    isFirstWorkTimeShow: $('#firstTimePanel input.zzhCommonCheck').prop('checked') ? '1' : '0',
                    secondWorkDayStart: $('#secondTimePanel .start-date dt').data('value'),
                    secondWorkDayEnd: $('#secondTimePanel .end-date dt').data('value'),
                    secondWorkTimeStart: $('#secondTimePanel input.start-time').val(),
                    secondWorkTimeEnd: $('#secondTimePanel input.end-time').val(),
                    isSecondWorkTimeShow: $('#secondTimePanel input.zzhCommonCheck').prop('checked') ? '1' : '0',
                    telphone: $('#telphone').val(),
                    phone: $('#phone').val(),
                    isTelphoneShow: $('#isTelphoneChecked').prop('checked') ? '1' : '0',
                    isPhoneShow: $('#isPhoneChecked').prop('checked') ? '1' : '0'
                });

                self.updateStoreLightConfig();
            });
        },
        updateStoreLightConfig: function () {
            var serviceGroupList = [];
            for(var k in dataManager.serviceGroupMap) {
                serviceGroupList.push(dataManager.serviceGroupMap[k]);
            }

            var params = $.extend({}, dataManager.storeLightConfig, {
                serviceGroupListJson: JSON.stringify(serviceGroupList)
            });

            params.phone = params.phone.replace(/ /g,"");
            params.telphone= params.telphone.replace(/ /g,"");
            if(!checkPhone(params.phone,params.isPhoneShow) || !checkMobile(params.telphone,params.isTelphoneShow)){
                return
            }

            $.post(
                '../store-light-config/update',
                params,
                function (result) {
                    if (!result || !result.success) {
                        alert(result.message || '服务器发生故障');
                    }else {
                        alert('更新成功');
                    }
                },
                'json'
            )
        }
    };
})();

$(function () {
    storeLightConfiger.init();
});