/**
 * Created by chenqing on 2016/2/18.
 */
seajs.use(['modules/feiniu', 'modules/util/generalServiceHelper', 'modules/util/generalUtil', 'JSON'],
    function (feiniu, serviceHelper, generalUtil, JSON) {

        var params = {
            objStaffId: 71   //飞牛员工编号Id
        };
        function getUrlParam(name) {
            var reg = new RegExp("(^|&)" + name + "=([^&]*)(&|$)"); //构造一个含有目标参数的正则表达式对象
            var r = window.location.search.substr(1).match(reg);  //匹配目标参数
            if (r != null) return unescape(r[2]);
            return null; //返回参数值
        };

        var im_in = {
            init: function () {
                this.getparams();
                this.getStaffInfo();
            },
            config: {
                SERVER: 'vender'
            },
            /**
             * 获取参数
             */
            getparams: function () {
                params.objStaffId = getUrlParam("staffId");
            },
            /**
             * 获取飞牛员工信息
             */
            getStaffInfo: function () {
                var self = this;
                generalUtil.isNumber(params.objStaffId) &&
                $.post(self.config.SERVER,
                    {
                        VENDER_INFO_TYPE: 'GET_STAFF_INFO',
                        staff_id: params.objStaffId
                    },
                    function (result){
                        if (result.success) {
                            var staffInfo = (result.data || {})['staffInfo'] || {};
                            $(document).find('#J_staff_info').html(staffInfo.groupName+" - "+staffInfo.nickName);
                            } else {
                            $(document).find('#J_staff_info').html("获取飞牛员工信息失败~");
                        }
                    },
                    'json'
                );
            }



        };

        (function () {
            im_in.init.call(im_in);
        })();

    });