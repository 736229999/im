var TemplateOpt=
    '<li class="info"><p><em></em>$customerId</p></li>';

var TemplateQueue=
    '<li class="queueinfo"><p><em></em>$customerId</p></li>';

var testPage = {
    init: function () {
        var self =this;
        $('.testQueueInsert').click(function () {
            var type = $("input[name='role']:checked").val();
            var params = {
                statsNumber:type
            };

            $.post('../im-server/test/test-stats',
                params,
                function (resultInfo) {
                    if (resultInfo && !!resultInfo.success) {
                        alert((resultInfo || {}).message || '发送模拟场景数据成功');
                    } else {
                        alert((resultInfo || {}).message || '服务器发生故障，保存失败');
                    }
                },
                'json');

        });

        $('.testShuntInsert').click(function () {
            var type = $("input[name='shunt']:checked").val();
            var params = {
                statsNumber:type
            };

            $.post('../im-server/test/test-shunt',
                params,
                function (resultInfo) {
                    if (resultInfo && !!resultInfo.success) {
                        alert((resultInfo || {}).message || '发送模拟场景数据成功');
                    } else {
                        alert((resultInfo || {}).message || '服务器发生故障，保存失败');
                    }
                },
                'json');

        });

        $('.testInvitInsert').click(function () {
            var type = $("input[name='invit']:checked").val();
            var params = {
                statsNumber:type
            };

            $.post('../im-server/test/test-invit',
                params,
                function (resultInfo) {
                    if (resultInfo && !!resultInfo.success) {
                        alert((resultInfo || {}).message || '发送模拟场景数据成功');
                    } else {
                        alert((resultInfo || {}).message || '服务器发生故障，保存失败');
                    }
                },
                'json');

        });

        $('.testOFFlineInsert').click(function () {
            var type = $("input[name='offline']:checked").val();
            var params = {
                statsNumber:type
            };

            $.post('../im-server/test/test-offline',
                params,
                function (resultInfo) {
                    if (resultInfo && !!resultInfo.success) {
                        alert((resultInfo || {}).message || '发送模拟场景数据成功');
                    } else {
                        alert((resultInfo || {}).message || '服务器发生故障，保存失败');
                    }
                },
                'json');

        });


        $('.testONlineInsert').click(function () {
            var type = $("input[name='online']:checked").val();
            var params = {
                statsNumber:type
            };

            $.post('../im-server/test/test-online',
                params,
                function (resultInfo) {
                    if (resultInfo && !!resultInfo.success) {
                        alert((resultInfo || {}).message || '发送模拟场景数据成功');
                    } else {
                        alert((resultInfo || {}).message || '服务器发生故障，保存失败');
                    }
                },
                'json');

        });



        $('.getReceptionInfo').click(function () {
            var params = {
                groupId: 11
            }

            $.post('../im-server/test/test-get-queue-info',
                params,
                function (resultInfo) {
                    if (resultInfo && !!resultInfo.success) {
                        $('.queueinfo').remove();

                        var queueInfos= resultInfo.data.info;
                        var toAppend = '';
                        if (!!queueInfos) {
                            for (var i = 0; i < queueInfos.length; i++) {
                                var optIndex = queueInfos[i];
                                var id = optIndex;

                                var tmp = TemplateQueue;

                                var urlIndex = tmp.replace(/\$customerId/g, id.toString());

                                toAppend += urlIndex;
                            }
                            var askingPics = $('#ququeInfos' );
                            askingPics.append(toAppend);
                        }
                        self.getRecptionListToUI(11);
                    } else {
                        alert((resultInfo || {}).message || '获取排队信息失败');
                    }
                },
                'json');
        });

        $('.statsReset').click(function () {
            var type = this.getAttribute("value")
            var params = {
                statsNumber:type
            };

            $.post('../im-server/test/reset-stat',
                params,
                function (resultInfo) {
                    if (resultInfo && !!resultInfo.success) {
                        alert((resultInfo || {}).message || '重置场景成功');
                    } else {
                        alert((resultInfo || {}).message || '服务器发生故障，保存失败');
                    }
                },
                'json');

        });

        $('.shuntReset').click(function(){
            var type = this.getAttribute("value")
            var params = {
                statsNumber:type
            };

            $.post('../im-server/test/reset-shunt',
                params,
                function (resultInfo) {
                    if (resultInfo && !!resultInfo.success) {
                        alert((resultInfo || {}).message || '重置场景成功');
                    } else {
                        alert((resultInfo || {}).message || '服务器发生故障，保存失败');
                    }
                },
                'json');
        });

        $('.invitReset').click(function(){
            var type = this.getAttribute("value")
            var params = {
                statsNumber:type
            };

            $.post('../im-server/test/reset-invit',
                params,
                function (resultInfo) {
                    if (resultInfo && !!resultInfo.success) {
                        alert((resultInfo || {}).message || '重置场景成功');
                    } else {
                        alert((resultInfo || {}).message || '服务器发生故障，保存失败');
                    }
                },
                'json');
        });


        $('.onlineReset').click(function(){
            var type = this.getAttribute("value")
            var params = {
                statsNumber:type
            };

            $.post('../im-server/test/reset-online',
                params,
                function (resultInfo) {
                    if (resultInfo && !!resultInfo.success) {
                        alert((resultInfo || {}).message || '重置场景成功');
                    } else {
                        alert((resultInfo || {}).message || '服务器发生故障，保存失败');
                    }
                },
                'json');
        });
    },

    getRecptionListToUI:function(groupId){
        var params = {
            groupId:groupId
        };

        $.post('../im-server/test/test-service-info-in-group',
            params,
            function (resultInfo) {
                if (resultInfo && !!resultInfo.success) {
                    $('.info').remove();
                    var infos1 = resultInfo.data.recptionList1;
                    var toAppend1 ="";
                    for (var key in infos1) {
                        var id = key;
                        var tmp = TemplateOpt;
                        var urlIndex = tmp.replace(/\$customerId/g, id.toString());
                        toAppend1 += urlIndex;
                    }
                    var askingPics = $('.receptionList1' );
                    askingPics.append(toAppend1);

                    var infos2 = resultInfo.data.recptionList2;
                    var toAppend2 ="";
                    for (var key in infos2) {
                        var id = key;
                        var tmp = TemplateOpt;
                        var urlIndex = tmp.replace(/\$customerId/g, id.toString());
                        toAppend2 += urlIndex;
                    }
                    var askingPics = $('.receptionList2' );
                    askingPics.append(toAppend2);
                } else {
                    alert((resultInfo || {}).message || '服务器发生故障，获取接待信息失败');
                }
            },
            'json');
    },

}


$(function () {
    testPage.init();
});